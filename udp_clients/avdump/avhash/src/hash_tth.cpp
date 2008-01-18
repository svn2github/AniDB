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

#include "hash_wrapper.h"

#include "../src/stdinc.h"
#include "../src/DCPlusPlus.h"
#include "../src/TigerHash.h"
#include "../src/MerkleTree.h"

DigestTTH::DigestTTH(){
	type = HASH_TTH;
	addr = (void*)new TigerTree();
	/*int len = 1024;
	char* testbuf = new char[1024];

	TigerTree *a = new TigerTree();
	a->update(testbuf, 1024);
	a->update(testbuf, 1024);
	a->update(testbuf, 1024);
	a->update(testbuf, 1024);
	a->finalize();
	string s = a->getRoot().toBase32();
	printf("%s\n", s.c_str());
	delete a;
	delete testbuf;*/
}
void DigestTTH::clean(){
	TigerTree* tt = (TigerTree*)addr;
	delete tt;
}
void DigestTTH::update(char* buf, int len){
	((TigerTree*)addr)->update(buf, len);
}
int DigestTTH::digest(char* sum, int len){
	TigerTree* tt = (TigerTree*)addr;
	tt->finalize();
	string s = tt->getRoot().toBase32();
	strcpy_s((char*)sum, len,s.c_str());

	return (int)s.length();
}