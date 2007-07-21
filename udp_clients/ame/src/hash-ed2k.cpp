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

HashED2K::HashED2K() : HashChecksum(){
	hex=true;
}
void HashED2K::reset(){
	md4.reset();
	md4fin.reset();
	length = 0;
}
void HashED2K::update(char b){
	md4.update(b);
    length++;

	if ((length % BLOCKSIZE)==0){
		md4.digest((char*)hash);
		
        md4fin.update((char*)&hash,0,16);
		md4.reset();
	}
}
void HashED2K::update(char* buffer, int offset, size_t len){
	int write = len-offset;
	int passed = (int)(length % BLOCKSIZE);
	int place = BLOCKSIZE-passed;

	// underflow
	if (place > write) { 
		md4.update(buffer,offset,len);
		length+=len;
	} else 
	// perfect
	if (place == write) {
		md4.update(buffer,offset,len);
		length+=len;
		
		md4.digest((char*)hash);

		md4fin.update(hash,0,16);
		md4.reset();
	} else
	// overflow
	if (place < write) {
		md4.update(buffer,offset,place);
		length+=place;

		md4.digest((char*)hash);

		md4fin.update(hash,0,16);
		md4.reset();

		md4.update(buffer,offset+place,write-place);
		length+=write-place;
	}
}
void HashED2K::copyHash(){
	md4.digest((char*)hash);
}
char* HashED2K::toString(){
	return 0;
}
void HashED2K::getHexValue(char* hex){
	if (length < BLOCKSIZE) {
		md4.digest((char*)hash);
	} else {
        HashMD4 *mdt = (HashMD4*)md4fin.clone();

		char dig[16];
		md4.digest((char*)&dig);
		mdt->update((char*)dig, 0, 16);
		mdt->digest((char*)&hash);
		delete mdt;
	}
	HashBase::format((char*)&hash, 16, uppercase, hex);
}
