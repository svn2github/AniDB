/* $Id: ed2k.c,v 1.3 2002/09/29 17:53:58 sdalu Exp $ */

/**
 * @file ed2k.c
 * @date 2002/09/29 13:47:05
 * @author Stephane D'Alu
 *
 * $Revision: 1.3 $
 * $Date: 2002/09/29 17:53:58 $
 *
 * Except for the MD4 algorithm/code (starting at ==>)
 * Copyright (C) 2002  Stephane D'Alu
 * Code is under GPL v2 (see http://www.gnu.org/licenses/gpl.txt)
 *
 * WWW: http://www.sdalu.com/software/
 *
 * Credit:
 *   - edonkey computation id: http://xpoop.free.fr/desc.htm
 *
 * Compile:
 *   diet -Os gcc ed2k.c /usr/lib/dietlibc/lib/libowfat.a  -o ed2k
 */

#include "hash_wrapper.h"
#include "hash-base.h"
/*#include "../src/SHAHashSet.h"
#include "../src/SHA.h"
#include "../src/Encoder.h"*/

/*#include <fmt.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>*/

#define BLOCKSIZE 4096
#define EDK_BLOCK 2375

typedef unsigned char md4_t[16];

//#ifndef u_int32_t
//typedef unsigned int u_int32_t;
//#endif

/*typedef struct {
    u_int32_t		state[4];
    u_int32_t		count[2];
    unsigned char	buffer[64];
} md4_ctx_t;

void md4_init(md4_ctx_t *);
void md4_update(md4_ctx_t *, unsigned char *, unsigned int);
void md4_finish(md4_ctx_t *, md4_t);*/


//static char *progname;
//static char fsize[FMT_ULONG];

/*#ifndef rindex
char *rindex(const char *s, int c)
{
  char *last_found, *new_found = 0;

  if( strlen(s) == 0 && (const char*)c == s )
  {
    return (char *)s;
  }
  last_found = (char *)s;
  while( strlen(last_found) > 0 && (last_found = index(last_found+1, c)) )
  {
    new_found = last_found;
  }
  return new_found;
}
#endif*/

/*void hash_fd(int fd, md4_t digest) {
    unsigned char buf[BLOCKSIZE];
    int           len   = -1;
    int		  count =  0;
    md4_ctx_t	  ctx_i;
    md4_ctx_t	  ctx_f;

    md4_init(&ctx_f);

    do {
	int eb = EDK_BLOCK;
	md4_init(&ctx_i);
	while ((eb-- > 0) && ((len = read(fd, buf, BLOCKSIZE)) > 0))
	    md4_update(&ctx_i, buf, len);
	md4_finish(&ctx_i, digest);
	count++;

	md4_update(&ctx_f, digest, sizeof(md4_t));
    } while (len > 0);

    if (count > 1)
	md4_finish(&ctx_f, digest);
}*/

DigestED2K::DigestED2K(){
	type = HASH_ED2K;
	/*ctx_f = new md4_ctx_t();
	ctx_i = new md4_ctx_t();
	md4_init(ctx_f);
	md4_init(ctx_i);*/
	addr = (void*) new HashED2K();
	//addr2 = (void*) ctx_f;
}
void DigestED2K::clean(){
	//delete (md4_ctx_t*)addr;
	//delete (md4_ctx_t*)addr2;
	//delete ctx_f;
	//delete ctx_i;
	delete (HashED2K*)addr;
}
void DigestED2K::update(char* buf, int len){
	//md4_update((md4_ctx_t*)addr, buf, len);
	((HashED2K*)addr)->update(buf, 0, len);
}
int DigestED2K::digest(char* sum, int len){
	//md4_t dig;
	//md4_finish((md4_ctx_t*)addr, dig);
	((HashED2K*)addr)->getHexValue(sum);
	//std::string s = Encoder::toBase16(dig, 16);
	//strcpy_s((char*)sum, len, s.c_str());
	//return (int)s.length();
	return 64;
}
/*static __inline__ char hdigit(q) {
    return ((q >= 10) ? 'W' : '0') + q;
}

char *digest_str(void *digest) {
    static char internal_str[sizeof(md4_t)*2 + 1];
    char *str   = internal_str;
    int   dsize = sizeof(md4_t);
    unsigned char *uc_digest = ((unsigned char *)digest);
    while (dsize--) {
	unsigned char d = *(uc_digest)++;
	*(str++) = hdigit(d >> 4 );
	*(str++) = hdigit(d & 0xf);
    }
    *str = '\0';
    return internal_str;
}

void usage(void) {
    fputs("usage: ", stderr);
    fputs(progname, stderr);
    fputs(" [-rql] file...\n", stderr);
    fputs("\t-r\tReverse the output format.\n", stderr);
    fputs("\t-q\tQuiet mode - only print hash.\n", stderr);
    fputs("\t-l\tCompute ed2k link (can't read from stdin).\n", stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int            fd       = 0;
    struct stat    statdata;
    int		   ch;
    int		   reverse  = 0;
    int		   quiet    = 0;
    int		   elink    = 0;
    md4_t	   digest;

    if ((progname = rindex(argv[0], '/')))
	progname++;
    else
	progname = argv[0];

    while ((ch = getopt(argc, argv, "rql")) != -1)
	switch (ch) {
	case 'r': reverse = 1; break;
	case 'q': quiet   = 1; break;
	case 'l': elink   = 1; break;
	case '?':
	default : usage();
	}
    argc -= optind;
    argv += optind;

    if (argc == 0) {
	if (elink) usage();
	hash_fd(0, digest);
        puts(digest_str(digest));
	exit(0);
    }

    while (argc--) {
	char *curfile = *argv++;
	if ((fd = open(curfile, O_RDONLY)) < 0) {
            fputs(progname, stderr);
            fputs(": ", stderr);
            fputs(curfile, stderr);
            fputs(": ", stderr);
            fputs(strerror(errno), stderr);
            fputs("\n", stderr);
	} else {
	    if ((fstat(fd, &statdata) < 0) ||
		((statdata.st_mode & S_IFMT) != S_IFREG)) {
                fputs(progname, stderr);
                fputs(": ", stderr);
                fputs(curfile, stderr);
                fputs(": ", stderr);
                fputs("Not a regular file\n", stderr);
	    } else {
		char *dstr;
		hash_fd(fd, digest);
		dstr = digest_str(digest);
		if (elink) {
		    char *filename = curfile;
                    int lastbyte;
		    if ((curfile = rindex(curfile, '/')))
			filename = curfile+1;
                    fputs("ed2k://|file|", stdout);
                    fputs(filename, stdout);
                    fputs("|", stdout);
                    lastbyte = fmt_ulonglong(fsize, statdata.st_size);
                    fsize[lastbyte] = '\0';
		    fputs(fsize, stdout);
                    fputs("|", stdout);
                    fputs(dstr, stdout);
                    fputs("|/\n", stdout);
		} else {
		    if (quiet) {
			puts(dstr);
		    } else {
			if (reverse)
                        {
			    fputs(dstr, stdout);
                            fputs(" ", stdout);
                            puts(curfile);
                        }
			else
                        {
                            fputs("ED2K (", stdout);
                            fputs(curfile, stdout);
                            fputs(") = ", stdout);
                            puts(dstr);
                        }
		    }
		}
	    }
	    close(fd);
	}
    }
    exit(0);
}*/



/*==> RSA Data Security, Inc. MD4 Message-Digest Algorithm =============*/

/* Copyright (C) 1990-2, RSA Data Security, Inc. All rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD4 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD4 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.
 */
/*
static unsigned char md4_padding[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


#define FF(a, b, c, d, x, s) {						\
    (a) += (((b) & (c)) | ((~b) & (d))) + (x);				\
    (a)  = ((a) << (s)) | ((a) >> (32-(s)));				\
}
#define GG(a, b, c, d, x, s) {						\
    (a) += (((b)&(c))|((b)&(d))|((c)&(d))) + (x) + (u_int32_t)0x5a827999; \
    (a)  = ((a) << (s)) | ((a) >> (32-(s)));				\
}
#define HH(a, b, c, d, x, s) {						\
    (a) += ((b) ^ (c) ^ (d)) + (x) + (u_int32_t)0x6ed9eba1;		\
    (a)  = ((a) << (s)) | ((a) >> (32-(s)));				\
}

static void md4_transform (u_int32_t state[4], unsigned char block[64]) {
    unsigned int i, j;

    u_int32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    for (i = 0, j = 0 ; j < 64 ; i++, j += 4)
	x[i] = ((u_int32_t)block[j]) | (((u_int32_t)block[j+1]) << 8) |
	    (((u_int32_t)block[j+2]) << 16) | (((u_int32_t)block[j+3]) << 24);

    FF (a, b, c, d, x[ 0],   3);	FF (d, a, b, c, x[ 1],   7);
    FF (c, d, a, b, x[ 2],  11);	FF (b, c, d, a, x[ 3],  19);
    FF (a, b, c, d, x[ 4],   3);	FF (d, a, b, c, x[ 5],   7);
    FF (c, d, a, b, x[ 6],  11);	FF (b, c, d, a, x[ 7],  19);
    FF (a, b, c, d, x[ 8],   3);	FF (d, a, b, c, x[ 9],   7);
    FF (c, d, a, b, x[10],  11);	FF (b, c, d, a, x[11],  19);
    FF (a, b, c, d, x[12],   3);	FF (d, a, b, c, x[13],   7);
    FF (c, d, a, b, x[14],  11);	FF (b, c, d, a, x[15],  19);

    GG (a, b, c, d, x[ 0],   3);	GG (d, a, b, c, x[ 4],   5);
    GG (c, d, a, b, x[ 8],   9);	GG (b, c, d, a, x[12],  13);
    GG (a, b, c, d, x[ 1],   3);	GG (d, a, b, c, x[ 5],   5);
    GG (c, d, a, b, x[ 9],   9);	GG (b, c, d, a, x[13],  13);
    GG (a, b, c, d, x[ 2],   3);	GG (d, a, b, c, x[ 6],   5);
    GG (c, d, a, b, x[10],   9);	GG (b, c, d, a, x[14],  13);
    GG (a, b, c, d, x[ 3],   3);	GG (d, a, b, c, x[ 7],   5);
    GG (c, d, a, b, x[11],   9);	GG (b, c, d, a, x[15],  13);

    HH (a, b, c, d, x[ 0],   3);	HH (d, a, b, c, x[ 8],   9);
    HH (c, d, a, b, x[ 4],  11);	HH (b, c, d, a, x[12],  15);
    HH (a, b, c, d, x[ 2],   3);	HH (d, a, b, c, x[10],   9);
    HH (c, d, a, b, x[ 6],  11);	HH (b, c, d, a, x[14],  15);
    HH (a, b, c, d, x[ 1],   3);	HH (d, a, b, c, x[ 9],   9);
    HH (c, d, a, b, x[ 5],  11);	HH (b, c, d, a, x[13],  15);
    HH (a, b, c, d, x[ 3],   3);	HH (d, a, b, c, x[11],   9);
    HH (c, d, a, b, x[ 7],  11);	HH (b, c, d, a, x[15],  15);

    state[0] += a;    state[1] += b;
    state[2] += c;    state[3] += d;
}

static void md4_encode(unsigned char *output,
		       u_int32_t *input, unsigned int len) {
    unsigned int i, j;

    for (i = 0, j = 0; j < len; i++, j += 4) {
	output[j]   = (unsigned char) (input[i]        & 0xff);
	output[j+1] = (unsigned char)((input[i] >> 8)  & 0xff);
	output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
	output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
    }
}

void md4_init(md4_ctx_t *context) {
    context->count[0] = context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}

void md4_update(md4_ctx_t *context, unsigned char *input, unsigned int len) {
    unsigned int i, index, partLen;
    index = (unsigned int)((context->count[0] >> 3) & 0x3F);

    if ((context->count[0] += ((u_int32_t)len << 3)) < ((u_int32_t)len << 3))
	context->count[1]++;
    context->count[1] += ((u_int32_t)len >> 29);

    partLen = 64 - index;

    if (len >= partLen) {
	memcpy(&context->buffer[index], input, partLen);
	md4_transform(context->state, context->buffer);

	for (i = partLen; i + 63 < len; i += 64)
	    md4_transform(context->state, &input[i]);

	index = 0;
    } else {
	i = 0;
    }

    memcpy(&context->buffer[index], &input[i], len-i);
}

void md4_finish (md4_ctx_t *context, md4_t digest) {
    unsigned char bits[8];
    unsigned int index, padLen;

    md4_encode (bits, context->count, 8);

    index = (unsigned int)((context->count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    md4_update (context, md4_padding, padLen);

    md4_update (context, bits, 8);
    md4_encode (digest, context->state, 16);
}
*/
