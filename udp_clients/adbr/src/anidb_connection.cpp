#include "anidb_connection.h"
#include "anidb.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "cryptopp/md5.h"
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"

#include <boost/bind.hpp>
#include <boost/timer.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#if defined(_WIN32)
#include <windows.h>
#endif

using namespace std;
using namespace AniDB;
using namespace CryptoPP;
using namespace CryptoPP::Weak;

Connection::Connection()
{
  debug = false;
  encrypt = false;
  recvd_reply = false;
  last_access = 0;
  killfunc = NULL;

  udp::resolver resolver(io_service);
  udp::resolver::query query(ANIDB_ADDRESS, ANIDB_PORT);
  anidb = *resolver.resolve(query);

  socket = new udp::socket(io_service, udp::endpoint(udp::v4(), 0));
}

Connection::~Connection()
{
  socket->close();
  delete socket;
}

size_t Connection::send_recv()
{
  long next_access = clock() + 2 * CLOCKS_PER_SEC;
  while(clock() < next_access) {}
  last_access = clock();

  if(debug)
    printf("\n--> %s\n\t\t\t\t\t", request);

  if(encrypt)
  {
    ECB_Mode<AES>::Encryption encryptor(key, AES::DEFAULT_KEYLENGTH);

    size_t st = strlen(request), l = 16-(st%16);
    for(size_t i = 0; i < l; i++)
    {
      request[st++] = l;
    }
    request[st] = '\0';

    encryptor.ProcessData((byte*)crequest, (byte*)request, st);
    send_recv(crequest, st, creply, BUFFER_SIZE);

    request[st-l] = '\0';
  }
  else
  {
    send_recv(request, strlen(request), reply, BUFFER_SIZE);
  }

  if(encrypt)
  {
    ECB_Mode<AES>::Decryption decryptor(key, AES::DEFAULT_KEYLENGTH);
    decryptor.ProcessData((byte*)reply, (byte*)creply, reply_length);

    reply[reply_length - (size_t)reply[reply_length-1]] = '\0';
  }

  if(debug)
    printf("\n<-- %s\n\t\t\t\t\t", reply);

  size_t ret = 0;
  sscanf(reply, "%u", &ret);

  if(ret > 500)
  {
    printf("Critical error: %s\n", reply);
    if(killfunc)
    {
      this->killfunc();
    }
  }

  return ret;
}

void Connection::send_recv(const char *out, size_t out_length,
                                 char *in, size_t in_length)
{
  socket->send_to(boost::asio::buffer(out, out_length), anidb);

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

  for(int i = 0; i < 3; i++)
  {
    recvd_reply = false;
    in[0] = 0;

    io_service.reset();
    deadline_timer deadline(io_service);

    socket->async_receive(boost::asio::buffer(in, in_length),
                          boost::bind(&Connection::handle_receive, this,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));

    deadline.expires_from_now(boost::posix_time::seconds(5));
    deadline.async_wait(boost::bind(&Connection::close, this));
    io_service.run();

    if(!recvd_reply)
    {
      printf("Timeout | ");
    }
    else
    {
#if defined(_WIN32)
      if(formatted_print)
      {
        SetConsoleCursorPosition(hStdOut, coord);
      }
#endif
      return;
    }
  }
}

void Connection::handle_receive(const boost::system::error_code& error, size_t l)
{
  if(error)
  {
    printf("Error receiving data\n");
  }

  reply_length = l;
  recvd_reply = true;
}

void Connection::close()
{
  io_service.stop();
}

bool Connection::login(const char *username, const char *password,
                       const char *apipass, bool enc)
{
  if(enc)
  {
    sprintf(request, "ENCRYPT user=%s&type=1", username);

    int ret = send_recv();

    if(ret == AniDB::ENCRYPTION_ENABLED)
    {
      char salt[10];

      sscanf(reply, "%d %10s", &ret, salt);

      char prehash[20];
      strcpy(prehash, apipass);
      strcat(prehash, salt);

      MD5 md5;
      md5.CalculateDigest(key, (byte*)prehash, strlen(prehash));

//    printf("salt|%s|, apipass|%s|, prehash|%s|, key|%16s|",
//           salt, apipass, prehash, key);

      encrypt = true;
    }
  }

  sprintf(request, "AUTH user=%s&pass=%s&protover=3&client=adbr&clientver=2&nat=1",
          username, password);
  size_t ret = send_recv();

  memset(session_id, 0x00, SESSION_LENGTH);
  sscanf(reply, "%d %10s", &ret, session_id);

  if(session_id)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Connection::logout()
{
  sprintf(request, "LOGOUT s=%s", session_id);

  size_t ret = send_recv();

  return ret == AniDB::LOGGED_OUT;
}

bool Connection::file(adbr_attr &attrib, uint file_id)
{
  sprintf(request, "FILE s=%s&fid=%u&amask=%s&fmask=%s",
                   session_id, file_id, AMASK, FMASK);

  return file(attrib);
}

bool Connection::file(adbr_attr &attrib, const char *ed2k, ulong size)
{
  sprintf(request, "FILE s=%s&ed2k=%s&size=%lu&amask=%s&fmask=%s",
                   session_id, ed2k, size, AMASK, FMASK);

  return file(attrib);
}

bool Connection::file(adbr_attr &attrib)
{
  size_t ret = send_recv();

  if(ret != AniDB::FILE)
  {
    return false;
  }

  parse_tokens(attrib, reply, FILE_ANIME_TOKENS, FILE_ANIME_TOKEN_COUNT);

  attrib["ANIME_YEAR"] = boost::regex_replace(attrib["ANIME_YEAR"],
                         boost::regex("(\\d\\d\\d\\d)-\\1"), "\\1");

  if(!attrib["ANIME_TYPE"].compare("Movie"))
  {
    boost::cmatch eps;
    boost::regex exp("(\\d+).*?(\\d+)");
    if(boost::regex_match(attrib["EPISODE_NAME"].c_str(), eps, exp))
    {
      attrib["EPISODE"] = eps[1];
      attrib["EPISODE_TOTAL"] = eps[2];
    }

    attrib["ANIME_TYPE"] = "Film";
  }
  else
  {
    size_t start = 0, end = 0;
    boost::cmatch eps;
    boost::regex exp("^(\\d+)-(\\d+)$");
    if(boost::regex_match(attrib["EPISODE"].c_str(), eps, exp))
    {
      start = atoi(string(eps[1].first, eps[1].second).c_str());
      end   = atoi(string(eps[2].first, eps[2].second).c_str());
    }

    size_t aid = 0;
    sscanf(attrib["AID"].c_str(), "%u", &aid);

    if(aid)
    {
      for(size_t i = start+1; i <= end; i++)
      {
        adbr_attr att;
        if(episode(att, aid, i))
        {
          attrib["EPISODE_NAME"] += ", " + att["EPISODE_ENGLISH"];
        }
      }
    }

    clean(attrib["EPISODE_NAME"]);
  }

  size_t status = 0;
  sscanf(attrib["STATUS"].c_str(), "%u", &status);
  if(status & status::FILE_ISV2)
  {
    attrib["VERSION"] = "v2";
  }
  else if(status & status::FILE_ISV3)
  {
    attrib["VERSION"] = "v3";
  }
  else if(status & status::FILE_ISV4)
  {
    attrib["VERSION"] = "v4";
  }
  else if(status & status::FILE_ISV5)
  {
    attrib["VERSION"] = "v5";
  }

  if(!attrib["ANIME_ENGLISH"].compare(""))
  {
    attrib["ANIME_ENGLISH"] = attrib["ANIME_ROMAJI"];
  }

  return true;
}

bool Connection::myfile(adbr_attr &attrib, uint file_id)
{
  sprintf(request, "MYLIST s=%s&fid=%u", session_id, file_id);

  size_t ret = send_recv();

  if(ret != AniDB::MYLIST)
  {
    return false;
  }

  parse_tokens(attrib, reply, MYLIST_TOKENS, MYLIST_TOKEN_COUNT);

  return true;
}

bool Connection::episode(adbr_attr &attrib, uint anime_id, uint ep_no)
{
  sprintf(request, "EPISODE s=%s&aid=%u&epno=%u", session_id, anime_id, ep_no);

  size_t ret = send_recv();

  if(ret != AniDB::EPISODE)
  {
    return false;
  }

  parse_tokens(attrib, reply, EPISODE_TOKENS, EPISODE_TOKEN_COUNT);

  return true;
}

bool Connection::mylistadd(uint file_id, const options *opt)
{
  sprintf(request, "MYLISTADD s=%s&fid=%u&state=%u",
                    session_id, file_id, opt->set_mylist_state);

  return mylistadd(opt);
}

bool Connection::mylistadd(const char *ed2k, ulong size, const options *opt)
{
  sprintf(request, "MYLISTADD s=%s&ed2k=%s&size=%lu&state=%u",
                   session_id, ed2k, size, opt->set_mylist_state);

  return mylistadd(opt);
}

bool Connection::mylistadd(const options *opt)
{
  if(opt->set_watched)
    strcat(request, "&viewed=1");

  size_t ret = send_recv();

  if(ret == AniDB::FILE_ALREADY_IN_MYLIST)
  {
    strcat(request, "&edit=1");
    ret = send_recv();
  }

  if(ret != AniDB::MYLIST_ENTRY_ADDED &&
     ret != AniDB::MYLIST_ENTRY_EDITED)
  {
    return false;
  }

  return true;
}

void Connection::parse_tokens(adbr_attr &attrib, const char *input,
                              const char **tokens, const size_t token_count)
{
  const char *str = strchr(input, '\n');

  if(!str)
    return;

  for(size_t i = 1, j = 0, st = 1, l = token_count; j < l; i++)
  {
    if(str[i] == '|' || str[i] == '\0')
    {
      attrib[tokens[j]] = string(&str[st], i-st);
      clean(attrib[tokens[j]]);
      st = i+1;

      if(str[i] == '\0')
        break;

      j++;
    }
  }

//for(adbr_attr::const_iterator i = attrib.begin(); i != attrib.end(); i++)
//{
//  cout << "key: " << i->first << " value: " << i->second << endl;
//}
}
