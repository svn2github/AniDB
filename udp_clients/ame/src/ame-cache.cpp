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

#include <wx/wx.h>

#include <wx/hashset.h>

#include "anidb-base.h"
#include "ame-db.h"
#include "ame-cache.h"

WX_DECLARE_HASH_MAP( int, AdbBase*, wxIntegerHash, wxIntegerEqual, AdbBaseMap );

AdbBaseMap hm;

AdbBase* AmeCache::get(int type, int id){
	if(type<dA||type>dG) return NULL;
	id += type*10000000;
	AdbBaseMap::iterator it = hm.find(id);
	if(hm.find(id)==hm.end()){
		AdbBase *b;
		switch(type){
			case dA: b = new AdbAnime(); break;
			case dE: b = new AdbEpisode(); break;
			case dG: b = new AdbGroup(); break;
		}
		b->src = 0;
		hm[id] = b;
		return b;
	}
	return it->second;
}
void AmeCache::clean(void){
	AdbBaseMap::iterator it;
    for( it = hm.begin(); it != hm.end(); ++it ){
		if(it->first<10000000)
			delete (AdbAnime*)it->second;
		else if(it->first<20000000)
			delete (AdbEpisode*)it->second;
		else if(it->first<30000000)
			delete (AdbGroup*)it->second;
		it->second = NULL;
    }
	hm.clear();
}
int AmeCache::count(void){
	return hm.size();
}