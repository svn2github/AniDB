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

#define HASH_TTH	1
#define HASH_SHA1	2
#define HASH_AICH	4
#define HASH_ED2K	8
#define HASH_CRC	16
#define HASH_MD5	32

// creates a hash object, one of the types defined above
// define the size of the data (file) if the type is HASH_AICH
// returns the address to the hash object, or 0 if type is unsupported
unsigned int hash_create(int type, unsigned __int64 size);

// len is length of buffer (and sum)
// returns 1 if ok
int hash_update(unsigned int addr, char* buf, int len);

// copies the result to sum, if sum!=0 (and len>0)
// TTH and AICH in Base32, SHA1 in Base16
// returns length of the sum
int hash_digest(unsigned int addr, int type, char* sum, int len);

// destroys the hash object
void hash_destroy(unsigned int addr);