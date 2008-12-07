#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN32)
#include <windows.h>
#endif

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "adbr_hash.h"
#include "cryptopp/md4.h"

#define BLOCKSIZE 9728000
#define BUFFSIZE 1048576

using namespace std;
using namespace CryptoPP::Weak;

// compute the ed2k hash
// requires ed2k[33]
// returns hex-formatted ascii string
void ed2k_hash(path filename, char *ed2k)
{
  char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                     'a', 'b', 'c', 'd', 'e', 'f'};

  long length = file_size(filename);
  size_t l = (length/BLOCKSIZE + (length % BLOCKSIZE ? 1 : 0));

#if defined(_WIN32)
  bool formatted_print = false;

  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
  {
    formatted_print = true;
  }

  COORD coord = {csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y};
#endif

  assert(length > 0);

  char *buffer = new char[BLOCKSIZE];
  char *accum = new char[l*16];

  ifstream is;
  is.open(filename.string().c_str(), ios::binary);

  MD4 *md4 = new MD4();

  for(size_t i = 0, r; i < l; i++)
  {
    if(length - i*BLOCKSIZE >= BLOCKSIZE)
      r = BLOCKSIZE;
    else
      r = length - (i*BLOCKSIZE);

    for(size_t j = 0, ll  = (r/BUFFSIZE + (r % BUFFSIZE ? 1:0)); j < ll; j++)
    {
      size_t rr;

      if(r - j*BUFFSIZE >= BUFFSIZE)
        rr = BUFFSIZE;
      else
        rr = r - (j*BUFFSIZE);

      is.read(buffer, rr);
      md4->Update((byte*)buffer, rr);
      //md4->CalculateDigest((byte*)(&accum[i*16]), (byte*)buffer, r);

#if defined(_WIN32)
      if(formatted_print)
      {
        SetConsoleCursorPosition(hStdOut, coord);
        printf("%3.1f%%", (float)100.0*(i*BLOCKSIZE+j*BUFFSIZE)/length);
      }
#endif
    }

#if defined(_WIN32)
    SetConsoleCursorPosition(hStdOut, coord);
#endif

    md4->Final((byte*)(&accum[i*16]));
  }
  is.close();

  char fed2k[33];
  md4->CalculateDigest((byte*)fed2k, (byte*)accum, l*16);

  delete[] accum;
  delete[] buffer;
  delete md4;

  for(size_t i = 0; i < 16; i++)
  {
    ed2k[i*2] = hexval[((fed2k[i] >> 4) & 0xF)];
    ed2k[(i*2) + 1] = hexval[(fed2k[i]) & 0x0F];
  }
  ed2k[32] = '\0';

//printf("(%ld, %ld) ", rtime, htime);
}
