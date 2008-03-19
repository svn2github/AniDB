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

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "config.h"

#ifdef ENABLE_WXW
#include <wx/tokenzr.h>
#include <wx/hashset.h>

class myHashSet;
WX_DECLARE_HASH_SET( wxString, wxStringHash, wxStringEqual, myHashSet );
myHashSet *file_register;
#else
#include <windows.h>
#include <algorithm>
#endif

#define STR_HOST "api.anidb.net"
#define STR_VERSION "0.34.00"
#define STR_DATE "080318"
#define STR_USAGE "\
Help:      http://wiki.anidb.info/w/Avdump\n\
Usage:     avdump [-options] <media file/folder> [<media file/folder> ...]\n\
Options:    (one letter switches can be put in one string)\n\
 Input:     (all multiple letter switches requires colon)\n\
   ac      autocreq: '-ac:<username>:<api password>' YOU SHOULD USE THIS\n\
   ms      monitor sleep duration in milliseconds, default is 60000\n\
   exp     export ed2k-links to file\n\
   ext     comma separated extension list\n\
   log     write output to file\n\
   port    udp port used by ac\n\
   done	   save processed-file-paths to file and exlude existing from proc\n\
   tout    timeout: '-tout:<seconds>:<number of retries>'\n\
   bsize   buffer for hashing: '-bsize:<size in KB (1024)>:<num of bufs (8)>'\n\
   host    host name of anidb udp api server, default is '"STR_HOST"'\n\
  Output:   (exclusive)\n\
   s       Short (creq friendly)\n\
   l       List (codecs)\n\
   x       XML Old format\n\
   y       XML Creq format\n\
   N       No data output. Does not affect error messages or progress.\n\
  Control:\n\
   c       do _not_ reCurse into subfolders\n\
   i       skIp full parsing (only for testing)\n\
   m       Monitor folder(s)\n\
   n       add extra Newline after each file\n\
   p       Pause when done (hold cmd window)\n\
   q       pause after each file\n\
   r       Random file order\n\
   t       print Time used for each file\n\
   z       delete files after parsing\n\
   o       wait for respOnse when sending dumps\n\
   v       use \\n instead of \\r @ progress\n\
   w       supress progress (silent)\n\
  Hash:\n\
   0       CRC32 (Cyclic Redundancy Check)\n\
   1       ED2K  (eDonkey2000 hash)\n\
   2       MD5   (Message-Digest algorithm 5)\n\
   3       SHA1  (Secure Hash Algorithm 1)\n\
   5       TTH   (Tiger Tree Hash)\n\
   6       AICH  (Advanced Intelligent Corruption Handler)\n\
   9       mp3 hash w/o metadata + foosic fingerprint (test)\n\
   a       All hash algorithms\n\
   h       Hash mode (no a/v parsing)\n\
   e       print Ed2k link\n\
   d       print aniDb link\n\
   g       print and Goto anidb link\n\
   u       print time Used\n\
Version:"

void init(config &conf){
	{
		//<pelican> Had rar test, works
		char locale[7] = ".";
		GetLocaleInfoA(GetThreadLocale(), LOCALE_IDEFAULTCODEPAGE, &locale[1], 6);
		setlocale(LC_CTYPE, locale);
	}

	conf.b_pause_at_end =
	conf.b_show_proc_time =
	conf.b_newline_after_each_file =
	conf.b_randomize_order =
	conf.b_pause_after_each_file =
	conf.b_delete_files =
	conf.b_silent = false;
	conf.b_full_media_parsing = true;
	conf.i_master_mode = FORMAT_OLD;

#ifdef ENABLE_AC
	conf.b_enable_auto_creq=false;
	conf.b_enable_pong=false;
	conf.i_timeout_secs = 30;
	conf.i_timeout_retries = 6;
#endif

#ifdef ENABLE_HASH
	conf.b_skip_media_parsing=false;
	conf.b_enable_audio_hashing=false;
	conf.i_hash_mode = 0;
	conf.i_hash_format = 0;
	conf.o_ring_buffer.parts = 8;
	conf.o_ring_buffer.partsize = 1024*1024;
	conf.o_ring_buffer.buffer = NULL;

	conf.chk.crc[0] =
	conf.chk.ed2k[0] =
	conf.chk.ed2k_blue[0] =
	conf.chk.sha1[0] =
	conf.chk.md5[0] =
	conf.chk.tth[0] =
	conf.chk.aich[0] = 0;

#ifdef ENABLE_FP
	avfp_init();
	conf.o_foosicbuf = new char[FOOSIZE];
#endif

#endif

#ifdef ENABLE_WXW
	conf.f_log=NULL;
	conf.f_files_done=NULL;
	conf.f_ed2k_export=NULL;
#endif
}

void clean(config &conf){
#ifdef ENABLE_HASH

#ifdef ENABLE_AC
	do_your_cleanup();
#endif

	if(conf.o_ring_buffer.parts>RB_PARTS_MIN)
		rb_destroy(&conf.o_ring_buffer);

#ifdef ENABLE_FP
	avfp_destroy();
	delete conf.o_foosicbuf;
#endif

#endif
}

#ifdef ENABLE_WXW
inline bool check(wstring s){
	return file_register->find(wxString(s.c_str()))==file_register->end();
}
inline void add(vector<wstring> *list){
	for(size_t i=0; i<list->size(); i++){
		if(check(list->at(i).c_str())){
			file_register->insert(list->at(i).c_str());
		}
	}
}
#endif

const char* do_work(config &conf, int argc, wchar_t* argv[]){
	if(argc<2)
		return STR_USAGE;

	string name, pass, host=STR_HOST;
	int port = 0;
	int monitor = 0;
	bool recurse=true;
	vector<wstring> arg_param;
	vector<wstring> arg_path;
	wstring exts_str = L"mkv,mp4,ogm,avi,mpeg,mpg,mpe,mov,asf,wmv,rm,rmvb";

	for(int i=1; i<argc; i++)
		if(argv[i][0]==L'-')
			arg_param.push_back(argv[i]);
		else arg_path.push_back(argv[i]);
	if(arg_path.size()<1)
		return STR_USAGE;
	
	for(size_t i=0; i<arg_param.size(); i++){
		wstring arg = arg_param[i];
		if(arg.length()>4 && !wcsncmp(arg.c_str(), L"-log:", 5)){
			#ifdef ENABLE_WXW
				if(conf.f_log)
					return "You can't have more that one -log:";
				arg = arg.substr(5);
				if(arg.length()<1)
					return "No log file name defined.";
				conf.f_log = new wxFFile(arg.c_str(), wxT("a"));
				if(conf.f_log->Error())
					return "Failed to open log file.";
			#endif
		}else if(arg.length()>4 && !wcsncmp(arg.c_str(), L"-exp:", 5)){
			#ifdef ENABLE_WXW
				if(conf.f_ed2k_export)
					return "You can't have more that one -exp:";
				arg = arg.substr(5);
				if(arg.length()<1)
					return "No export file name defined.";
				conf.f_ed2k_export = new wxFFile(arg.c_str(), wxT("a"));
				if(conf.f_ed2k_export->Error())
					return "Failed to open export file.";
				hash_setExportFile(conf.f_ed2k_export);
				conf.i_hash_mode |= HASH_ED2K;
			#endif
		}else if(arg.length()>4 && !wcsncmp(arg.c_str(), L"-done:", 6)){
			#ifdef ENABLE_WXW
				if(conf.f_files_done)
					return "You can't have more that one -done:";
				arg = arg.substr(6);
				if(arg.length()<1)
					return "No file name defined.";

				conf.f_files_done = new wxFFile(arg.c_str(), wxT("a+"));
				if(conf.f_files_done->Error())
					return "Failed to open \"done\" file.";
			#endif
		}else if(arg.length()>4 && !wcsncmp(arg.c_str(), L"-ext:", 5)){
			exts_str = arg.substr(5);
		}else if(arg.length()>5 && !wcsncmp(arg.c_str(), L"-port:", 6)){
#ifdef ENABLE_AC
			port = _wtoi(arg.substr(6).c_str());
#else
			return "Auto creq is enabled not in this build!";
#endif
		}else if(arg.length()>7 && !wcsncmp(arg.c_str(), L"-bsize:", 7)){
#ifdef ENABLE_HASH
			arg = arg.substr(7);
			string full(arg.begin(), arg.end());
			int x = (int)full.find(':');
			if(x<1)
				return "Format is: '-bsize:<size in KB>:<number of buffers>'";
			int size = atoi(full.substr(0, x).c_str());
			int buffers = atoi(full.substr(x+1).c_str());
			if(size<128)
				return "Buffer size must be at least 128 KB.";
			if(size>(1024*16))
				return "Buffer size can not be more than 16MB.";
			size *= 1024;
			if(buffers<1)
				return "Number of buffers must be at least 1";
			if(buffers>64)
				return "Number of buffers can't be more than 64";
			conf.o_ring_buffer.parts = buffers;
			conf.o_ring_buffer.partsize = size;
#else
			return "Hashing is not enabled in this build!";
#endif
		}else if(arg.length()>5 && !wcsncmp(arg.c_str(), L"-tout:", 6)){
#ifdef ENABLE_AC
			arg = arg.substr(6);
			string full(arg.begin(), arg.end());
			int x = (int)full.find(':');
			if(x<1)
				return "Format is: '-tout:<seconds>:<number of retries>'";
			conf.i_timeout_secs = atoi(full.substr(0, x).c_str());
			conf.i_timeout_retries = atoi(full.substr(x+1).c_str());
			if(conf.i_timeout_secs<20)
				return "Timeout must be at least 20 seconds.";
			if(conf.i_timeout_retries<0)
				return "Number of retries can not be less than 0.";
#else
			return "Auto creq is enabled not in this build!";
#endif
		}else if(arg.length()>3 && !wcsncmp(arg.c_str(), L"-ac:", 4)){
#ifdef ENABLE_AC
			arg = arg.substr(4);
			string full(arg.begin(), arg.end());
			int x = (int)full.find(':');
			if(x<3)
				return "Format is: '-ac:<username>:<apipass>'";
			name = full.substr(0, x);
			pass = full.substr(x+1);
			conf.i_hash_mode = HT_CREQ_HASH;
			conf.b_enable_auto_creq = true;
#else
			return "Auto creq is enabled not in this build!";
#endif
		}else if(arg.length()>5 && !wcsncmp(arg.c_str(), L"-host:", 6)){
#ifdef ENABLE_AC
			arg = arg.substr(6);
			string full(arg.begin(), arg.end());
			host = full;
#else
			return "Auto creq is not in this build!";
#endif
		}else if(arg.length()>3 && !wcsncmp(arg.c_str(), L"-ms:", 4)){
#ifdef ENABLE_WXW
			arg = arg.substr(4);
			string num(arg.begin(), arg.end());
			monitor = atoi(num.c_str());
			if(monitor < 100)
				return "Invalid monitor sleep duration.";
#else
			return "Monitor is not enabled in this version.";
#endif
		}else{
			for(size_t i=1; i<arg.length(); i++)
				switch(arg.at(i)){
					case L'p': conf.b_pause_at_end = true; break;
					case L'q': conf.b_pause_after_each_file = true; break;
					case L'N': conf.b_silent = true; break;
					case L's': conf.i_master_mode = FORMAT_SHORT; break;
					case L'x': conf.i_master_mode = FORMAT_XML; break;
					case L'l': conf.i_master_mode = FORMAT_LIST; break;
					case L't': conf.b_show_proc_time = true; break;
					case L'n': conf.b_newline_after_each_file = true; break;
					case L'r': conf.b_randomize_order = true; break;
					case L'i': conf.b_full_media_parsing = false; break;
					case L'c': recurse = false; break;
					case L'z':{
						wprintf(L"Are you sure you want to delete the files after parsing? (y/n) ");
						char x;
						cin >> x;
						if(x!='y')
							return "ABORTED";
						conf.b_delete_files = true; break;
					}
#ifdef ENABLE_WXW
					case L'm': monitor = 60000; break;
#endif
						//HASH
#ifdef ENABLE_HASH
					case L'h': conf.b_skip_media_parsing = true; break;
					case L'y': conf.i_master_mode = -1; break;
					case L'0': conf.i_hash_mode |= HASH_CRC; break;
					case L'e': conf.i_hash_format |= HT_ELINK; conf.i_hash_mode |= HASH_ED2K; break;
					case L'g': conf.i_hash_format |= HT_JUMP;
					case L'd': conf.i_hash_format |= HT_LINK;
					case L'1': conf.i_hash_mode |= HASH_ED2K; break;
					case L'2': conf.i_hash_mode |= HASH_MD5; break;
					case L'3': conf.i_hash_mode |= HASH_SHA1; break;
					case L'5': conf.i_hash_mode |= HASH_TTH; break;
					case L'6': conf.i_hash_mode |= HASH_AICH; break;
					case L'a': conf.i_hash_mode = HT_FULL_HASH; break;
					case L'u': conf.i_hash_format |= HT_HTIME; break;
					case L'v': conf.i_hash_format |= HT_NOCARE; break;
					case L'w': conf.i_hash_format |= HT_SILENT; break;
					case L'9': conf.b_enable_audio_hashing = true; break;
#endif
#ifdef ENABLE_AC
					case L'o': conf.b_enable_pong = true; break;
#endif
				}
		}
	}

#ifdef ENABLE_HASH
	if(conf.o_ring_buffer.parts>RB_PARTS_MIN)
		rb_init(&conf.o_ring_buffer);
	
	if(conf.b_enable_audio_hashing){
		exts_str = L"mp3";
		conf.b_skip_media_parsing = true;
		conf.i_hash_mode = 0;
	}else{
		if(!conf.i_hash_mode && conf.b_skip_media_parsing)
			return "You have to select at least one hashing algorithm in hash mode.";
#ifdef ENABLE_AC
		if(conf.b_enable_auto_creq && !conf.b_full_media_parsing)
			return "Skip mode and autocreq is incompatible.";
		if(conf.b_enable_auto_creq && conf.b_skip_media_parsing)
			return "Hash mode and autocreq is incompatible.";
	}
	if(conf.b_enable_auto_creq && !do_your_init(name.c_str(), pass.c_str(), host.c_str(), port, conf.i_timeout_secs, conf.b_enable_pong))
		return "FAIL";
#else
	}
#endif

#endif

#ifdef ENABLE_WXW
	file_register = new myHashSet();
	if(conf.f_files_done){
		wxString str;
		conf.f_files_done->ReadAll(&str);
		wxStringTokenizer tkz(str, wxT("\r\n"));
		while ( tkz.HasMoreTokens() )
			file_register->insert(tkz.GetNextToken());
	}
#endif

	//list files
	vector<wstring> *files = new vector<wstring>;
	vector<wstring> *exts = new vector<wstring>;

	if(exts_str.length()>0)
		split(exts_str, (*exts));
	if(exts->size()<1)
		return "No extensions defined.";

	for (size_t i=0; i<arg_path.size(); i++) {
		directory_listing(files, exts, arg_path[i], recurse
#ifdef ENABLE_WXW
			, check
#endif
		);
	}
#ifdef ENABLE_WXW
	add(files);
#endif

	bool found_files = files->size()>0, slept = false;
	sort(files->begin(), files->end());
	
	while(do_work_inner(conf, files) && monitor){
#ifdef ENABLE_WXW
		files->clear();
		if(!conf.b_silent && !slept){
			slept = true;
			wcout << "Sleeping.." << endl;
		}
		Sleep(monitor);
		for (size_t i=0; i<arg_path.size(); i++) {
			directory_listing(files, exts, arg_path[i], recurse, check);
		}
		sort(files->begin(), files->end());
		add(files);
		if(files->size()>0){
			slept = false;
			found_files = true;
		}
#endif
	}

#ifdef ENABLE_WXW
	if(conf.f_log)
		delete conf.f_log;
	if(conf.f_ed2k_export)
		delete conf.f_ed2k_export;
	if(conf.f_files_done)
		delete conf.f_files_done;
	if(file_register)
		delete file_register;
#endif

	delete files;
	delete exts;

	if(found_files){
		if(conf.b_full_media_parsing)
			return "OK";
		else return "SKIP MODE";
	}else return "No files found.";
}

int wmain(int argc, wchar_t* argv[]){
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(448865);
#endif
	config conf;
	init(conf);

	MediaInfoLib::MediaInfo_Config c;

	wcout << do_work(conf, argc, argv) << " "STR_VERSION" - "STR_DATE" OK" << " (" << c.Info_Version_Get() << " CVS)" << endl;

	clean(conf);

	if(conf.b_pause_at_end) system("pause");


#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
