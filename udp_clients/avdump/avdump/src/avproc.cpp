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

#include "tinyxml/tinyxml.h"
#include <process.h>
#include "config.h"

#include <iomanip>

#define SESSION_REFRESH 16

#ifdef ENABLE_WXW
#include <wx/wfstream.h>
#include <wx/filename.h>
wxFFile *ed2k_export=NULL;

void hash_setExportFile(void* link){
	ed2k_export = (wxFFile*)link;
}
#else
#include <io.h>
#include <time.h>
#include <tchar.h>
#include <windows.h>
#endif

const char base16Table[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1,
	-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
};

void tinyAddX(TiXmlElement *e, const char* tag, string value, bool attr=false, bool cdata=false){
	if(value.length()>0 && value.length()<1024){
		if(attr){
			e->SetAttribute(tag, value.c_str());
		}else{
			TiXmlElement * tmp = new TiXmlElement( tag );
			if(cdata){
				char buffer[2048];
				sprintf_s(buffer, 2048, "<![CDATA[%s]]>", value.c_str());
				tmp->LinkEndChild(new TiXmlText(buffer));
			}else{
				tmp->LinkEndChild(new TiXmlText(value.c_str()));
			}
			e->LinkEndChild(tmp);
		}
	}
}

#ifdef ENABLE_HASH
#define PRNT(type,name,sum) if(BC(mode, type)){if(BC(mode, HT_INDENT)) sst << "  ";sst << name << sum << endl;}
void printHashes(stringstream &sst, hashc* chk, int mode, bool show, wstring path, __int64 file_size){
	
	if(BC(mode, HASH_ED2K)){

		if(BC(mode, HT_INDENT))
		sst << "  ";

		char anidb_link[255];
		sprintf_s(anidb_link, 255, "http://anidb.info/perl-bin/animedb.pl?show=file&size=%I64d&hash=%s", file_size, chk->ed2k);
		string file_name(path.begin(), path.end());
		size_t x = file_name.find_last_of("\\");
		if(x!=string::npos) file_name = file_name.substr(x+1);
		char ed2k_link[1024];
		sprintf_s(ed2k_link, 1024, "ed2k://|file|%s|%I64d|%s|/", file_name.c_str(), file_size, chk->ed2k);
		if(BC(mode, HT_JUMP))
			_spawnlp(_P_DETACH, "rundll32", "rundll32", "url.dll,FileProtocolHandler", anidb_link, NULL);
		if(show && !BC(mode, HT_LINK) && !BC(mode, HT_ELINK) && mode>=0){
			sst << "ed2k: " << chk->ed2k;
			if(/*file_size%9728000==0 && */strcmp(chk->ed2k, chk->ed2k_blue))
				sst << " (" << chk->ed2k_blue << ")";
			sst << endl;
		}
		else{
			if(BC(mode, HT_ELINK))
				sst << ed2k_link << endl;
			if(BC(mode, HT_LINK)){
				if(BC(mode, HT_ELINK) && BC(mode, HT_INDENT))
					sst << "  ";
				sst << anidb_link << endl;
			}
		}
	}
	if(show){
		PRNT(HASH_CRC, "crc : ", chk->crc)
		PRNT(HASH_MD5, "md5 : ", chk->md5)
		PRNT(HASH_SHA1, "sha1: ", chk->sha1)
		PRNT(HASH_TTH, "tth : ", chk->tth)
		PRNT(HASH_AICH, "aich: ", chk->aich)
	}
}
#endif

int process_file(config &conf, stringstream &sst, const wchar_t *path, __int64 file_size){

	//open
	addr_t id = avinfo_file_open((wchar_t*)path);

	if(id<1){
		sst << " ERROR: Failed to parse the file! (Possibly not a media file)" << endl;
		return 1;
	}

	int calls = 1;
	float f = avinfo_file_parse(id);

	for( ; calls<10 && f>=0; calls++)
		f = avinfo_file_parse(id);
	
	if(conf.b_full_media_parsing){
#ifdef ENABLE_HASH
		int prog = !BC(conf.i_hash_format, HT_SILENT);
		int care = BC(conf.i_hash_format, HT_NOCARE);
#else
		int prog=1, care=0;
#endif
		while( f>=0 ){
			if(prog && calls%10==0){
				if(care)
					printf("\nP %05.2f%%",f);
				else
					printf("\rP %05.2f%%",f);
			}
			calls++;
			f = avinfo_file_parse(id);
		}
		if(prog && calls>1){
			if(care)
				printf("\n");
			else
				printf("\r        \r");
		}
	}
#ifdef ENABLE_HASH
	if(
#ifdef ENABLE_AC
		conf.b_enable_auto_creq || 
#endif
		conf.i_master_mode<0){
		TiXmlDocument doc;
		//doc.LinkEndChild( new TiXmlDeclaration( "1.0", "utf8", "" ) );
		TiXmlElement * e_file = new TiXmlElement( "file" );
		
		if( file_size>0 )
		{
			char buffer[255];
			sprintf_s(buffer, 255, "%I64d", file_size);
			TiXmlElement * e_size = new TiXmlElement( "size" );
			e_size->LinkEndChild(new TiXmlText(buffer));
			e_file->LinkEndChild(e_size);
		}

		tinyAddX(e_file, "crc", conf.chk.crc);
		tinyAddX(e_file, "ed2k", conf.chk.ed2k);
		if(/*file_size%9728000==0 */strcmp(conf.chk.ed2k, conf.chk.ed2k_blue))
			tinyAddX(e_file, "ed2k_alt", conf.chk.ed2k_blue);
		tinyAddX(e_file, "md5", conf.chk.md5);
		tinyAddX(e_file, "sha1", conf.chk.sha1);
		tinyAddX(e_file, "tth", conf.chk.tth);

		int ret = avinfo_file_tinyxml(id, (addr_t)e_file);
		doc.LinkEndChild( e_file );

#ifdef UNTESTED_CVS
		printf("Autocreqing is disabled in this version!\n");
#else
#ifdef ENABLE_AC
		if(conf.b_enable_auto_creq && ret==0){
			conf.i_session_count++;
			TiXmlPrinter printer;
			printer.SetLineBreak( "");
			printer.SetIndent( "" );
			doc.Accept( &printer );
			if(conf.i_session_count%SESSION_REFRESH!=0 || do_your_session_retry(conf.i_timeout_retries))
			{
				const char* xml = printer.CStr();
				if(!do_your_thing(xml, strlen(xml), conf.i_timeout_retries)){
					printf("Giving up!\n");
					return 3;
				}
			}else{
				avinfo_file_close(id);
				return 2;
			}
		}
#endif
#endif
		if(conf.i_master_mode<0){
			TiXmlPrinter printer;
			printer.SetLineBreak( "\n");
			printer.SetIndent( " " );
			doc.Accept( &printer );
			sst << printer.CStr();
		}
		doc.Clear();
	}
#endif
	//get data
	if(conf.i_master_mode>=0){
		unsigned int size = 1024*64;
		char * buffer = new char[size];
		size_t r = avinfo_file_formatted(id, buffer, size, conf.i_master_mode);
		
		wstring pw(path);
		string s(pw.begin(), pw.end());
		
		if(r>0){
			buffer[r] = '\0';
			
			if(conf.i_master_mode==FORMAT_LIST)
				sst << buffer << "\t" << s <<endl;
			else
				sst << buffer;
			if(size-1==r) sst << endl << "Warning: The buffer was too small!" << endl;
		}else if(conf.i_master_mode!=FORMAT_LIST)
			sst << "Failed." << endl;
		//else sst << s <<endl;
		delete buffer;
	}

	//close
	avinfo_file_close(id);

	if(conf.b_delete_files)
		DeleteFile(path);
	return 0;
}
#ifdef ENABLE_HASH
void doMp3HashFile(config &conf, stringstream &sst, FILE* file_handle, wstring file, __int64 file_size){

#ifdef ENABLE_FP
	int foosize = NULL;
#endif
	int x = file.find_last_of(L".");
	if(x>0 && (x+1)<file.length()){
		wstring ext = file.substr(x+1);
		transform(ext.begin(), ext.end(), ext.begin(), tolower);
		if(ext==L"mp3"){
			char mp3sum[64];
			int duration;
			int mp3len = hash_mp3(file, file_handle, file_size, (char*)&mp3sum, duration);
			if(mp3len<1){
				wprintf(L"IGNORED: %s\n", file.c_str());
				return;
			}
#ifdef ENABLE_FP
			else{
				foosize = avfp_decode_mp3(file.c_str(), conf.o_foosicbuf, duration);
			}
#endif

			TiXmlDocument doc;
			TiXmlElement * e_file = new TiXmlElement( "ostfile" );

			if( file_size>0 )
			{
				char buffer[255];
				sprintf_s(buffer, 255, "%d", mp3len);
				TiXmlElement * e_size = new TiXmlElement( "size" );
				e_size->LinkEndChild(new TiXmlText(buffer));
				e_file->LinkEndChild(e_size);
			}


			tinyAddX(e_file, "hash", mp3sum);

#ifdef ENABLE_WXW
			wxFileName fn(file.c_str());
			{
				libebml::UTFstring test(fn.GetFullName().c_str());
				tinyAddX(e_file, "name", test.GetUTF8(), false, true);
			}
			{
				size_t cnt = fn.GetDirCount();
				if(cnt>0){
					libebml::UTFstring test(fn.GetDirs()[cnt-1].c_str());
					tinyAddX(e_file, "folder", test.GetUTF8(), false, true);
				}
			}
#endif

			addr_t id = avinfo_file_open((wchar_t*)file.c_str());
			avinfo_file_parse(id);
			int ret = avinfo_file_tinyxml_ost(id, (addr_t)e_file);
			avinfo_file_close(id);

			doc.LinkEndChild( e_file );

#ifdef ENABLE_AC
			if(conf.b_enable_auto_creq && ret==0){
				conf.i_session_count++;
				TiXmlPrinter printer;
				printer.SetLineBreak( "");
				printer.SetIndent( "" );
				doc.Accept( &printer );
				if(conf.i_session_count%SESSION_REFRESH!=0 || do_your_session_retry(conf.i_timeout_retries))
				{
					const char *xml = printer.CStr();
					if(!do_your_thing(xml, strlen(xml), conf.i_timeout_retries)){
						printf("Giving up!\n");
					}else if(foosize){
						char bin[20];
						for (int i = 0, j; i<20; i++){
							j = i*2;
							bin[i] = base16Table[mp3sum[j]]*16+base16Table[mp3sum[j+1]];
						}

						char* buf = new char[foosize+28];
						((int*)buf)[0] = 0;
						((int*)buf)[1] = mp3len;
						memcpy(buf+8, bin, 20);
						memcpy(buf+28, conf.o_foosicbuf, foosize);
						if(!do_your_thing(buf, foosize+28, conf.i_timeout_retries)){
							printf("Giving up!\n");
						}
						delete buf;
					}
				}
			}else{
#endif
				TiXmlPrinter printer;
				printer.SetLineBreak( "\n");
				printer.SetIndent( " " );
				doc.Accept( &printer );
				sst << printer.CStr();
#ifdef ENABLE_AC
			}
#endif
			doc.Clear();
		}
	}
}
#endif

bool do_work_inner(config &conf, vector<wstring> *files){
	size_t len = files->size();
	if(len<1)
		return true;
	size_t xlen = len;
	srand((unsigned int)GetTickCount());

	//process files
	//if(len>1) wcout << "Files: " << len << endl;

	size_t *lookup = new size_t[len];
	for(size_t i=0; i<len; i++)
		lookup[i] = i;
	
	size_t i;
	for(i=0; i</*100/*/len; i++){

		DWORD t0 = GetTickCount();
		
		wstring file;
		if(conf.b_randomize_order)	file = files->at( (size_t)pick(lookup,xlen--) );
		else file = files->at( /*0/*/i );

		if(conf.i_master_mode!=FORMAT_LIST && !conf.b_silent){
			if(len>1) 
				wprintf(L"File number %d of %d: ", i+1, len);
			wprintf(L"%s\n", file.c_str());//wcout fails on unicode!?
		}
		FILE* file_handle = _tfsopen(file.c_str(), L"rbS", _SH_DENYWR);
		if(!file_handle){
			wprintf(L"File not found or locked: %s\n", file.c_str());
			continue;
		}
		__int64 file_size = _filelengthi64(file_handle->_file);
		if(file_size<1){
			wprintf(L"Skipped 0 byte file: %s\n", file.c_str());
			fclose(file_handle);
			continue;
		}
		stringstream sst;

#ifdef ENABLE_HASH
		hash_reset(&conf.chk);
		if(conf.b_enable_audio_hashing)
			doMp3HashFile(conf, sst, file_handle, file, file_size);
		if(conf.i_hash_mode){
			int mode = conf.i_hash_mode|conf.i_hash_format|(conf.i_master_mode==FORMAT_OLD?HT_INDENT:0);
			if(BC(mode, HT_INDENT))
				sst << " Hash:" << endl;
			unsigned int t0 = GetTickCount();

			if(conf.o_ring_buffer.parts>RB_PARTS_MIN)
				hash_file(&conf.chk, mode, file_handle, file_size, &conf.o_ring_buffer);
			else
				hash_file(&conf.chk, mode, file_handle, file_size, conf.o_ring_buffer.partsize);

			printHashes(sst, &conf.chk, mode, conf.i_master_mode>=0||conf.b_skip_media_parsing, file, file_size);

			if(BC(mode, HT_HTIME)){
				unsigned int t = GetTickCount()-t0;
				float mbps = 1000.0*file_size/t/1024/1024;
				sst << "  time: " << t << " ms, " << fixed << setprecision(2) << mbps << " MB/s" << endl;
			}
		}
#endif
		fclose(file_handle);
#ifdef ENABLE_HASH
		if(!conf.b_skip_media_parsing)
#endif		
			if(process_file(conf, sst, file.c_str(), file_size) > 1) //refresh session failed -> abort
				break;
		{
			string s = sst.str();
			if(!conf.b_silent)
				printf("%s", s.c_str());
#ifdef ENABLE_WXW
			if(conf.f_log){
				if(conf.i_master_mode!=FORMAT_LIST){
					conf.f_log->Write(wxString::Format(L"File: %s\n", file.c_str()), wxConvUTF8);
				}
				wstring x(s.begin(), s.end());
				conf.f_log->Write(x.c_str(), wxConvUTF8);
				conf.f_log->Flush();
			}
			if(conf.f_files_done){
				conf.f_files_done->Write(file.c_str(), wxConvUTF8);
				conf.f_files_done->Write(L"\n", wxConvUTF8);
				conf.f_files_done->Flush();
			}
#ifdef ENABLE_HASH
			if(ed2k_export){
				wxFileName fn(file.c_str());
				string esum(conf.chk.ed2k);
				wstring ssum(esum.begin(), esum.end());
				ed2k_export->Write(wxString::Format(wxT("ed2k://|file|%s|%I64d|%s|/\n"), fn.GetFullName().c_str(), file_size, ssum.c_str()), wxConvUTF8);
				ed2k_export->Flush();
			}
#endif
#endif
		}

		if(conf.b_show_proc_time){
			struct tm *newtime;
			__time64_t long_time;

			_time64( &long_time );
			newtime = _localtime64( &long_time );

			char buf[256];
			strftime( buf, 256, "(%Y-%m-%d %H:%M:%S)", newtime );
			cout << " Proc time: " << (GetTickCount()-t0) << " ms " << buf << endl;
		}

		if(conf.b_newline_after_each_file) cout << endl;
		if(conf.b_pause_after_each_file) system("pause");
	}
	delete lookup;
	return i==len;
}
