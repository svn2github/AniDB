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

#include "../src/SHAHashSet.h"
#include "../src/SHA.h"
#include "../src/Encoder.h"

void ASSERT2(bool b){
	if(!b)
		printf("ASSERT2!\n");
}
bool CreateHash(uchar *Y, UINT Length, CAICHHashTree* pShaHashOut);

DigestAICH::DigestAICH(unsigned __int64 size){
	type = HASH_AICH;
	CAICHHashSet* as = new CAICHHashSet();
	as->SetFileSize(size);
	parts = 0;
	lpos = 0;
	part = new unsigned char[PARTSIZE];
	addr = (void*)as;
}
void DigestAICH::clean(){
	CAICHHashSet* as = (CAICHHashSet*) addr;
	delete as;
	delete part;
}
void DigestAICH::update(char* buf, int len){
	CAICHHashSet* as = (CAICHHashSet*) addr;

	int n = len;
	if(len>(PARTSIZE-lpos)){
		n = PARTSIZE-lpos;
		memcpy(part+lpos, buf, n);
		if(!CreateHash(part, PARTSIZE, as->m_pHashTree.FindHash(parts*PARTSIZE, PARTSIZE))){
			printf("CreateHash FAILED!\n");
			return;
		}
		parts++;
		lpos = len-n;
		memcpy(part, buf+n, lpos);
	}else{
		memcpy(part+lpos, buf, n);
		lpos += n;
	}
}
int DigestAICH::digest(char* sum, int len){
	CAICHHashSet* as = (CAICHHashSet*) addr;

	if(!CreateHash(part, lpos, as->m_pHashTree.FindHash(parts*PARTSIZE, lpos)))
		return -6;

	as->ReCalculateHash(false);
	if (as->VerifyHashTree(true)){
		as->SetStatus(AICH_HASHSETCOMPLETE);
	}else{
		return -1;
	}

	int hlen = as->GetMasterHash().GetHashSize();
	if(hlen>len) return -3;
	byte * h = as->GetMasterHash().GetRawHash();
	if(as->HasValidMasterHash() && (as->GetStatus() == AICH_VERIFIED || as->GetStatus() == AICH_HASHSETCOMPLETE)){
		string s = Encoder::toBase32(h, hlen);
		strcpy_s((char*)sum, len, s.c_str());
		return (int)s.length();
	}
	return -2;
}

bool CreateHash(uchar *Y, UINT Length, CAICHHashTree* pShaHashOut){
	if(pShaHashOut==NULL) return false;
	uint32  Required = Length;
	uchar   X[64*128];
	uint32	posCurrentEMBlock = 0;
	uint32	nIACHPos = 0;
	CAICHHashAlgo* pHashAlg = new CSHA();

	unsigned int off=0;

	while (Required >= 64){
        uint32 len = Required / 64;
        if (len > sizeof(X)/(64 * sizeof(X[0])))
             len = sizeof(X)/(64 * sizeof(X[0]));

		memcpy(&X,(uchar*)(Y+off),len*64);
		off += len*64;

		// SHA hash needs 180KB blocks
		if (pShaHashOut != NULL){
			if (nIACHPos + len*64 >= EMBLOCKSIZE){
				uint32 nToComplete = EMBLOCKSIZE - nIACHPos;
				pHashAlg->Add(X, nToComplete);
				ASSERT2( nIACHPos + nToComplete == EMBLOCKSIZE );
				pShaHashOut->SetBlockHash(EMBLOCKSIZE, posCurrentEMBlock, pHashAlg);
				posCurrentEMBlock += EMBLOCKSIZE;
				pHashAlg->Reset();
				pHashAlg->Add(X+nToComplete,(len*64) - nToComplete);
				nIACHPos = (len*64) - nToComplete;
			}
			else{
				pHashAlg->Add(X, len*64);
				nIACHPos += len*64;
			}
		}
		Required -= len*64;
	}

	Required = Length % 64;
	if (Required != 0){
		memcpy(&X,(uchar*)(Y+off),Required);
		off += Required;

		if (pShaHashOut != NULL){
			if (nIACHPos + Required >= EMBLOCKSIZE){
				uint32 nToComplete = EMBLOCKSIZE - nIACHPos;
				pHashAlg->Add(X, nToComplete);
				ASSERT2( nIACHPos + nToComplete == EMBLOCKSIZE );
				pShaHashOut->SetBlockHash(EMBLOCKSIZE, posCurrentEMBlock, pHashAlg);
				posCurrentEMBlock += EMBLOCKSIZE;
				pHashAlg->Reset();
				pHashAlg->Add(X+nToComplete, Required - nToComplete);
				nIACHPos = Required - nToComplete;
			}
			else{
				pHashAlg->Add(X, Required);
				nIACHPos += Required;
			}
		}
	}
	if (pShaHashOut != NULL){
		if(nIACHPos > 0){
			pShaHashOut->SetBlockHash(nIACHPos, posCurrentEMBlock, pHashAlg);
			posCurrentEMBlock += nIACHPos;
		}
		ASSERT2( posCurrentEMBlock == Length );
		ASSERT2( pShaHashOut->ReCalculateHash(pHashAlg, false) );
	}
	delete pHashAlg;
	return true;
}