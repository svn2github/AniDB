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

#ifndef __MYUTILS_H
#define __MYUTILS_H

#include <vector>
#include <string>

using namespace std;

#ifdef _WIN32
#include <direct.h>
#else 
#include <unistd.h>
#endif

#ifdef _WIN32
const char dirsep = '\\';
typedef __int64 mylong;
#else
const char dirsep = '/';
typedef long long mylong;
#endif

unsigned int getTime();
size_t pick(size_t* a, size_t len);
int input_waiting();

extern bool timestamp;


int getms();
void MySleep(unsigned int ms);

void directory_listing(vector<wstring>* files, vector<wstring>* types, wstring path, bool recurse=true, bool(check(wstring))=0);

void tokenize(vector<string>* v, string str, char c);

string mylower(string str);
string mytime();
void println(string str);
void print(string str);
void printflush(string str);

wstring toWideString( string str );
wstring toWideString( const char* pStr , int len );

void split(const wstring& str, vector<wstring>& _container, const wstring& delim=L",", bool ins_empty=false);

#endif // __MYUTILS_H
