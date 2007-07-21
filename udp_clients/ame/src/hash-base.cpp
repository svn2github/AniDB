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

const char* HEX = "0123456789abcdef";

HashBase::HashBase(char *n, int hs, int bs) {
	name = n;
	hashSize = hs;
	blockSize = bs;
	buffer = new char[bs];
	count = 0;
}
HashBase::~HashBase(){
	delete buffer;
}
char* HashBase::getName() {
    return name;
}
int HashBase::getHashSize(){
    return hashSize;
}
int HashBase::getBlockSize(){
    return blockSize;
}
void HashBase::update(char b) {
    int i = (int)(count % blockSize);
    count++;
    buffer[i] = b;
    if (i == (blockSize - 1)) {
        transform(buffer, 0);
    }
}
void HashBase::update(char* in, int offset, int len) {
    int n = (int)(count % blockSize);
    count += len;
    int partLen = blockSize - n;
    int i = 0;

    if (len >= partLen) {
		memcpy(buffer+n, in+offset, partLen);
		transform(buffer, 0);
        for (i = partLen; i + blockSize - 1 < len; i+= blockSize) {
			transform(in, offset + i);
        }
        n = 0;
    }

    if (i < len) {
        memcpy(buffer+n, in+offset+i, len-i);
    }
}
void HashBase::digest(char* result) {
	int len;
    char* tail = padBuffer(len);
	update(tail, 0, len);
	delete tail;
   
    getResult(result);
    reset();
}
void HashBase::reset() {
    count = 0L;
    for (int i = 0; i < blockSize; ) {
        buffer[i++] = 0;
    }
    resetContext();
}
void HashBase::format(char* chars, int len, bool uppercase, char *name) {
	int b, n=0;
	for (int i=0; i < len; i++) {
        b = chars[i] & 0xFF;
        name[n++] = HEX[b >> 4];
        name[n++] = HEX[b & 0x0F];
    }
}
