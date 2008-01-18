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

#include "avinfo.h"
#include "epox_av_avinfo.h"

JNIEXPORT jint JNICALL Java_epox_av_AVInfo_version(JNIEnv *, jclass){
	return 1;
}
JNIEXPORT jlong JNICALL Java_epox_av_AVInfo_fileOpen(JNIEnv *env, jobject obj, jstring jstr, jint type){
	const jchar* jbuf = env->GetStringChars(jstr, NULL);
	jsize len = env->GetStringLength(jstr);
	wchar_t* path = new wchar_t[len+1];
	for(int i=0; i<len; i++)
		path[i] = jbuf[i];
	path[len] = L'\0';
	env->ReleaseStringChars(jstr, jbuf);

	addr_t x = avinfo_file_open(path);
	delete path;
	return x;
}
JNIEXPORT void JNICALL Java_epox_av_AVInfo_fileClose (JNIEnv *, jobject, jlong addr){
	avinfo_file_close((addr_t)addr);
}
JNIEXPORT jfloat JNICALL Java_epox_av_AVInfo_fileParse(JNIEnv *, jobject, jlong addr){
	return avinfo_file_parse((addr_t)addr);
}
JNIEXPORT jstring JNICALL Java_epox_av_AVInfo_fileFormatted(JNIEnv *env, jobject, jlong addr, jint type, jint blen){
	char* buffer = new char[blen];
	jsize len = avinfo_file_formatted((addr_t)addr, buffer, blen, type);
	jstring jstr;
	if(len>0){
		buffer[len] = '\0';
		jstr = env->NewStringUTF(buffer);
	} else jstr = env->NewStringUTF("Failed");
	delete buffer;
	return jstr;
}
