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

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#include <time.h>
#include "utils.h"

#include <iostream>
#include <algorithm>

#include <io.h>
#include <fcntl.h>

bool timestamp = true;

unsigned int getTime(){
	unsigned int last_idle_time;
	#ifdef _WIN32
	last_idle_time = GetTickCount();
	#else
	timeval time;
	gettimeofday(&time, NULL);
	last_idle_time = (((long)time.tv_sec * (long)1000) + ((long)time.tv_usec / (long)1000));;
	#endif
	return last_idle_time;
}
size_t pick(size_t* a, size_t len){
	size_t x = len*rand()/RAND_MAX;
	if(x==len) return a[x-1];
	size_t r = a[x];
	a[x] = a[len-1];
	return r;
}
/*int input_waiting()
{
	if(std::cin.gcount()==0) return false;
	char c;
	while (std::cin.get(c) && isspace(c));

	if (c) {
		std::cin.putback(c);
		return true;
	}
	return false;
}*/
/*bool endswith(const wchar_t *a, const wchar_t *b){
	wxString y(a);
	int x = y.find_last_of('\\');
	if(x>0) y = y.substr(x+1);
	return !wcscmp(y.c_str(), b);
}*/
void tokenize(vector<string>* v, string str, char c){
	size_t i = str.find(c);
	while(i<str.length()){
		if(i>0){
			string s = str.substr(0, i);
			v->push_back(s);
		}
		str = str.substr(i+1);
		i = str.find(c);
	}
	if(str.length()>0)
		v->push_back(str);
}
string mylower(string str){
#ifdef _WIN32
	transform(str.begin(),str.end(),str.begin(),tolower);
#else
	for(unsigned int i=0;i<str.length();i++)
	{
		str[i] = tolower(str[i]);
	}
#endif
	return str;
}
char timeStr [9];
struct tm *ptr;


string mytime(){
	time_t tm;	
	tm = time(NULL);
	ptr = localtime(&tm);
	strftime(timeStr , 9, "%H:%M:%S", ptr);
	return timeStr;
}

void println(string str){
	if(timestamp)//MyUtils::timestamp)
        cout << mytime() << ' ';
	cout << str << endl;
}
void print(string str){
	if(timestamp)//MyUtils::timestamp)
        cout << mytime() << ' ';
	cout << str;
}
void printflush(string str){
	cout << str << endl;
}
wstring toWideString( string str ){
	return toWideString(str.c_str(), (int)str.length()); 
}
wstring toWideString( const char* pStr , int len ){
   // ASSERT_PTR( pStr ) ; 
   // ASSERT( len >= 0 || len == -1 , _T("Invalid string length: ") << len ) ; 

    // figure out how many wide characters we are going to get 
    int nChars = MultiByteToWideChar( CP_ACP , 0 , pStr , len , NULL , 0 ) ; 
    if ( len == -1 )
        -- nChars ; 
    if ( nChars == 0 )
        return L"" ;

    // convert the narrow string to a wide string 
    // nb: slightly naughty to write directly into the string like this
    wstring buf ;
    buf.resize( nChars ) ; 
    MultiByteToWideChar( CP_ACP , 0 , pStr , len , 
        const_cast<wchar_t*>(buf.c_str()) , nChars ) ; 

    return buf ;
}
#ifdef _WIN32
void MySleep(unsigned int ms){
	Sleep(ms);
}
void fill(wstring &file){
#define BUFSIZE 4096
	DWORD retval=0;
    //BOOL success; 
    wchar_t buffer[BUFSIZE]=L""; 
    wchar_t * lpPart[BUFSIZE]={NULL};

	retval = GetFullPathNameW(file.c_str(),
			     BUFSIZE, buffer, lpPart);
	if(retval>0){
		file = buffer;
	}
}
void directory_listing(vector<wstring>* files, vector<wstring>* types, wstring path, bool recurse, bool(check(wstring))) {
	/*if(path=="."){
        char cwd[256];
		getcwd(cwd, sizeof(cwd));
		path = cwd;
	}*/
	
	//printf("list %ls\n", path.c_str());
	WIN32_FIND_DATA file;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(path.c_str(), &file);
	if (hFind!=INVALID_HANDLE_VALUE && 
		(file.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY){
		fill(path);
		files->push_back(path);
		return;
	}

	if(path[path.length()-1]!=dirsep) path += dirsep;
	wstring dir = path;
	dir.append(L"*");

	hFind = FindFirstFile(dir.c_str(), &file);

	if (hFind != INVALID_HANDLE_VALUE){
		do{
			{
				wstring tmp(file.cFileName);
				if( tmp==L"." || tmp==L".." )
					continue;
			}
			dir = path;
			dir += file.cFileName;
			if((file.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY){
				if(recurse)
					directory_listing(files, types, dir, recurse, check);
			}
			else{
				wstring fp = path;
				fp.append(file.cFileName);
				int x = fp.find_last_of(L".");
				if(x>0 && (x+1)<fp.length()){
					wstring ext = fp.substr(x+1);
					transform(ext.begin(), ext.end(), ext.begin(), tolower);
					for(unsigned int i=0; i<types->size();i++){
						if(ext==(*types)[i]){
							fill(fp);
							//int x = check(fp);
							//printf("%d %ls\n", check, fp.c_str());
							if(check==0 || check(fp)){
								int f = _wopen(fp.c_str(), _O_BINARY|_O_WRONLY);
								if(f!=-1){
									_close(f);
									files->push_back(fp);
								}
							}
							break;
						}
					}
				}
			}
		}while(FindNextFile(hFind, &file) != 0);
	}// else wcout << "Not found: " << path << endl;
}
#else

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
void MySleep(unsigned int ms){
	usleep(ms*1000);
}
void directory_listing(vector<string>* files, vector<string>* types, string path){
	
	DIR* dir = opendir(path.c_str());
	dirent* ent;
	struct stat statbuf;

	stat(path.c_str(), &statbuf);
	if(S_ISDIR(statbuf.st_mode) && path[path.length()-1]!='/')
		path += '/';
	else
	{
		string ext = path.substr(path.size()-3);
		for(unsigned int i=0; i<types->size();i++)
			if(ext==(*types)[i]){
				files->push_back(path);
				break;
			}
		return;
	}

	while (ent=readdir(dir)) {

		if(strcmp(".", ent->d_name)==0 || strcmp("..",ent->d_name)==0)
			continue;

		string fp = path + ent->d_name;
		
		stat(fp.c_str(), &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{
			directory_listing(files, types, fp);
		}else
		{
			if(fp.size()>3){
				string ext = fp.substr(fp.size()-3);
				for(unsigned int i=0; i<types->size();i++)
					if(ext==(*types)[i]){
						files->push_back(fp);
						break;
					}
				}
		}
	}
	closedir(dir);
}
#endif

//from http://www.codeproject.com/string/stringsplit.asp
void split(const wstring& str, vector<wstring>& _container, const wstring& delim, bool ins_empty)
{
    wstring::size_type lpos = 0;
    wstring::size_type pos = str.find_first_of(delim, lpos);
	while(lpos != string::npos)
    {
		if(ins_empty || (pos>lpos && lpos<str.length()))
			_container.insert(_container.end(), str.substr(lpos,pos - lpos));

        lpos = ( pos == string::npos ) ?  string::npos : pos + 1;
        pos = str.find_first_of(delim, lpos);
    }
}
