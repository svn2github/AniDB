// Copyright (C) 2006 epoximator
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "hash-base.h"

#include <string>

#define rol(i, shift_left) (((i) << (shift_left)) | ((i) >> (32 - (shift_left))))

HashMD4::HashMD4():HashBase("md4", DIGEST_LENGTH, BLOCK_LENGTH){
	DIGEST0 = "31D6CFE0D16AE931B73C59D7E0C089C0";
	resetContext();
}
HashMD4::HashMD4(HashMD4 *that):HashBase("md4", DIGEST_LENGTH, BLOCK_LENGTH){
    DIGEST0 = "31D6CFE0D16AE931B73C59D7E0C089C0";
	resetContext();

	a = that->a;
    b = that->b;
    c = that->c;
    d = that->d;
    count = that->count;
    memcpy(buffer, that->buffer, blockSize);
}
void HashMD4::dumpVar(){
	printf("%d %d %d %d\r\n", a, b, c, d);
}
HashDigest* HashMD4::clone() {
    return new HashMD4(this);
}

void HashMD4::getResult(char* digest){
    digest[0] = (char) a;
	digest[1] = (char)(a>>8);
	digest[2] = (char)(a>>16);
	digest[3] = (char)(a>>24);

	digest[4] = (char) b;
	digest[5] = (char)(b >> 8);
	digest[6] = (char)(b >> 16);
	digest[7] = (char)(b >> 24);

	digest[8] = (char) c;
	digest[9] = (char)(c >> 8);
	digest[10] = (char)(c >> 16);
	digest[11] = (char)(c >> 24);

	digest[12] = (char) d;
	digest[13] = (char)(d >> 8);
	digest[14] = (char)(d >> 16);
	digest[15] = (char)(d >> 24);
}

void HashMD4::resetContext(){
	a = mA; b = mB;
    c = mC; d = mD;
}
char* HashMD4::padBuffer(int &len){

	int n = (int) (count % BLOCK_LENGTH);
    int padding = (n < 56) ? (56 - n) : (120 - n);

	len = padding + 8;
	char* pad = new char[len];

	for(int i=0; i<len; i++)
		pad[i] = 0;

	pad[0] = (char) 0x80;
    mylong bits = count << 3;

	pad[padding++] = (char) (bits);
    pad[padding++] = (char) (bits>> 8);
	pad[padding++] = (char) (bits>>16);
    pad[padding++] = (char) (bits>>24);
    pad[padding++] = (char) (bits>>32);
    pad[padding++] = (char) (bits>>40);
    pad[padding++] = (char) (bits>>48);
    pad[padding  ] = (char) (bits>>56);

	return pad;
}
void HashMD4::transform(char* in, int i) {
	X[0] = (in[i+ 0]&0xFF)|(in[i+ 1]&0xFF)<<8 | (in[i+ 2]&0xFF)<<16 | in[i+ 3]<<24;
    X[1] = (in[i+ 4]&0xFF)|(in[i+ 5]&0xFF)<<8 | (in[i+ 6]&0xFF)<<16 | in[i+ 7]<<24;
    X[2] = (in[i+ 8]&0xFF)|(in[i+ 9]&0xFF)<<8 | (in[i+10]&0xFF)<<16 | in[i+11]<<24;
    X[3] = (in[i+12]&0xFF)|(in[i+13]&0xFF)<<8 | (in[i+14]&0xFF)<<16 | in[i+15]<<24;
    X[4] = (in[i+16]&0xFF)|(in[i+17]&0xFF)<<8 | (in[i+18]&0xFF)<<16 | in[i+19]<<24;
    X[5] = (in[i+20]&0xFF)|(in[i+21]&0xFF)<<8 | (in[i+22]&0xFF)<<16 | in[i+23]<<24;
    X[6] = (in[i+24]&0xFF)|(in[i+25]&0xFF)<<8 | (in[i+26]&0xFF)<<16 | in[i+27]<<24;
    X[7] = (in[i+28]&0xFF)|(in[i+29]&0xFF)<<8 | (in[i+30]&0xFF)<<16 | in[i+31]<<24;
    X[8] = (in[i+32]&0xFF)|(in[i+33]&0xFF)<<8 | (in[i+34]&0xFF)<<16 | in[i+35]<<24;
    X[9] = (in[i+36]&0xFF)|(in[i+37]&0xFF)<<8 | (in[i+38]&0xFF)<<16 | in[i+39]<<24;
    X[10]= (in[i+40]&0xFF)|(in[i+41]&0xFF)<<8 | (in[i+42]&0xFF)<<16 | in[i+43]<<24;
    X[11]= (in[i+44]&0xFF)|(in[i+45]&0xFF)<<8 | (in[i+46]&0xFF)<<16 | in[i+47]<<24;
    X[12]= (in[i+48]&0xFF)|(in[i+49]&0xFF)<<8 | (in[i+50]&0xFF)<<16 | in[i+51]<<24;
    X[13]= (in[i+52]&0xFF)|(in[i+53]&0xFF)<<8 | (in[i+54]&0xFF)<<16 | in[i+55]<<24;
    X[14]= (in[i+56]&0xFF)|(in[i+57]&0xFF)<<8 | (in[i+58]&0xFF)<<16 | in[i+59]<<24;
    X[15]= (in[i+60]&0xFF)|(in[i+61]&0xFF)<<8 | (in[i+62]&0xFF)<<16 | in[i+63]<<24;

    unsigned int aa, bb, cc, dd;

	aa = a;  bb = b;  cc = c;  dd = d;

	aa += ((bb & cc) | ((~bb) & dd)) + X[ 0];	 aa = rol(aa, 3);
	dd += ((aa & bb) | ((~aa) & cc)) + X[ 1];    dd = rol(dd, 7);
    cc += ((dd & aa) | ((~dd) & bb)) + X[ 2];    cc = rol(cc, 11);
    bb += ((cc & dd) | ((~cc) & aa)) + X[ 3];    bb = rol(bb, 19);

	aa += ((bb & cc) | ((~bb) & dd)) + X[ 4];    aa = rol(aa, 3);
    dd += ((aa & bb) | ((~aa) & cc)) + X[ 5];    dd = rol(dd, 7);
    cc += ((dd & aa) | ((~dd) & bb)) + X[ 6];    cc = rol(cc, 11);
    bb += ((cc & dd) | ((~cc) & aa)) + X[ 7];    bb = rol(bb, 19);

	aa += ((bb & cc) | ((~bb) & dd)) + X[ 8];    aa = rol(aa, 3);
    dd += ((aa & bb) | ((~aa) & cc)) + X[ 9];    dd = rol(dd, 7);
    cc += ((dd & aa) | ((~dd) & bb)) + X[10];    cc = rol(cc, 11);
    bb += ((cc & dd) | ((~cc) & aa)) + X[11];    bb = rol(bb, 19);

	aa += ((bb & cc) | ((~bb) & dd)) + X[12];    aa = rol(aa, 3);
    dd += ((aa & bb) | ((~aa) & cc)) + X[13];    dd = rol(dd, 7);
    cc += ((dd & aa) | ((~dd) & bb)) + X[14];    cc = rol(cc, 11);
    bb += ((cc & dd) | ((~cc) & aa)) + X[15];    bb = rol(bb, 19);

	aa += ((bb & (cc | dd)) | (cc & dd)) + X[ 0] + 0x5a827999;    aa = rol(aa, 3);
    dd += ((aa & (bb | cc)) | (bb & cc)) + X[ 4] + 0x5a827999;    dd = rol(dd, 5);
    cc += ((dd & (aa | bb)) | (aa & bb)) + X[ 8] + 0x5a827999;    cc = rol(cc, 9);
    bb += ((cc & (dd | aa)) | (dd & aa)) + X[12] + 0x5a827999;    bb = rol(bb, 13);

    aa += ((bb & (cc | dd)) | (cc & dd)) + X[ 1] + 0x5a827999;    aa = rol(aa, 3);
    dd += ((aa & (bb | cc)) | (bb & cc)) + X[ 5] + 0x5a827999;    dd = rol(dd, 5);
    cc += ((dd & (aa | bb)) | (aa & bb)) + X[ 9] + 0x5a827999;    cc = rol(cc, 9);
    bb += ((cc & (dd | aa)) | (dd & aa)) + X[13] + 0x5a827999;    bb = rol(bb, 13);

    aa += ((bb & (cc | dd)) | (cc & dd)) + X[ 2] + 0x5a827999;    aa = rol(aa, 3);
    dd += ((aa & (bb | cc)) | (bb & cc)) + X[ 6] + 0x5a827999;    dd = rol(dd, 5);
    cc += ((dd & (aa | bb)) | (aa & bb)) + X[10] + 0x5a827999;    cc = rol(cc, 9);
    bb += ((cc & (dd | aa)) | (dd & aa)) + X[14] + 0x5a827999;    bb = rol(bb, 13);

    aa += ((bb & (cc | dd)) | (cc & dd)) + X[3]  + 0x5a827999;    aa = rol(aa, 3);
    dd += ((aa & (bb | cc)) | (bb & cc)) + X[7]  + 0x5a827999;    dd = rol(dd, 5);
    cc += ((dd & (aa | bb)) | (aa & bb)) + X[11] + 0x5a827999;    cc = rol(cc, 9);
    bb += ((cc & (dd | aa)) | (dd & aa)) + X[15] + 0x5a827999;    bb = rol(bb, 13);

	// ROUND 3
    aa += (bb ^ cc ^ dd) + X[ 0] + 0x6ed9eba1;    aa = rol(aa, 3);
    dd += (aa ^ bb ^ cc) + X[ 8] + 0x6ed9eba1;    dd = rol(dd, 9);
    cc += (dd ^ aa ^ bb) + X[ 4] + 0x6ed9eba1;    cc = rol(cc, 11);
    bb += (cc ^ dd ^ aa) + X[12] + 0x6ed9eba1;    bb = rol(bb, 15);

    aa += (bb ^ cc ^ dd) + X[ 2] + 0x6ed9eba1;    aa = rol(aa, 3);
    dd += (aa ^ bb ^ cc) + X[10] + 0x6ed9eba1;    dd = rol(dd, 9);
    cc += (dd ^ aa ^ bb) + X[ 6] + 0x6ed9eba1;    cc = rol(cc, 11);
    bb += (cc ^ dd ^ aa) + X[14] + 0x6ed9eba1;    bb = rol(bb, 15);

    aa += (bb ^ cc ^ dd) + X[ 1] + 0x6ed9eba1;    aa = rol(aa, 3);
    dd += (aa ^ bb ^ cc) + X[ 9] + 0x6ed9eba1;    dd = rol(dd, 9);
    cc += (dd ^ aa ^ bb) + X[ 5] + 0x6ed9eba1;    cc = rol(cc, 11);
    bb += (cc ^ dd ^ aa) + X[13] + 0x6ed9eba1;    bb = rol(bb, 15);

    aa += (bb ^ cc ^ dd) + X[ 3] + 0x6ed9eba1;    aa = rol(aa, 3);
    dd += (aa ^ bb ^ cc) + X[11] + 0x6ed9eba1;    dd = rol(dd, 9);
    cc += (dd ^ aa ^ bb) + X[ 7] + 0x6ed9eba1;    cc = rol(cc, 11);
    bb += (cc ^ dd ^ aa) + X[15] + 0x6ed9eba1;    bb = rol(bb, 15);

	a += aa; b += bb; c += cc; d += dd;
}
bool HashMD4::selfTest(){
	HashMD4 md4;
	char buf[DIGEST_LENGTH];
	md4.digest((char*)&buf);
	char name[33];
	HashBase::format((char*)&buf, 16, true, (char*)&name);
	name[32]='\0';
	int ret = strcmp(DIGEST0, name);
	printf("%s %s %d\r\n", DIGEST0, name, ret);
	return ret==0;
}
