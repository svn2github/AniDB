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

#ifndef __ACACHE_H
#define __ACACHE_H

#define CACHE_ANIME(x) (AdbAnime*)AmeCache::get(dA, x)
#define CACHE_EPISODE(x) (AdbEpisode*)AmeCache::get(dE, x)
#define CACHE_GROUP(x) (AdbGroup*)AmeCache::get(dG, x)

class AdbBase;

class AmeCache{
public:
	static AdbBase* get(int type, int id);
	static void clean(void);
	static int count(void);
};

#endif //__ACACHE_H
