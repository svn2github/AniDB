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

#ifndef __JOB_H
#define __JOB_H

#define AL() wxCriticalSectionLocker locker(m_cs)

class StringHashMap;

class StringMap{
private:
	StringHashMap *m_data;
public:
	StringMap();
	~StringMap();
	void set(wxString key, wxString val);
	wxString get(wxString key);
	wxString toString(void);
};
enum{
	jS_DONE		= 0x00000001,//status
	jS_DO		= 0x00000002,
	jS_DOING	= 0x00000012,
	jS_FAILED	= 0x00000008,
	jU_2		= 0x00000020,//sub status
	jH_NORMAL	= 0x00000100,//health
	jH_PAUSED	= 0x00000200,
	jH_MISSING	= 0x00000400,
	jH_DELETED	= 0x00000800,
	jM_DIO		= 0x00010000,//mode
	jM_NIO		= 0x00020000,
//	jM_USR		= 0x00040000,
	jT_1		= 0x00100000,//type
	jT_2		= 0x00200000,
	jT_4		= 0x00400000,
//	jT_8		= 0x00800000,
	jF_DB		= 0x01000000,//flag
	jF_UNK		= 0x02000000,
	jM_H		= 0x0000ff00,//mask
	jM_S		= 0x0fff00ff,
	jM_R		= 0x0f0fff0f,

	jFAILED		= jS_FAILED|jF_DB,		//0x00,0   -> 16777224
	jUNKNOWN	= jS_DONE|jF_UNK|jF_DB,	//0x10,16  -> 50331649
	jFINISHED	= jS_DONE|jF_DB, 			//0xf0,240 -> 16777217

	jHASHWAIT	= jM_DIO|jT_1|jS_DO|jF_DB,	//0x30,48  -> 17891330
	jHASHING	= jM_DIO|jT_1|jS_DOING,	//0x32,
	jHASHED		= jM_DIO|jT_1|jS_DONE,		//0x3f,

	jIDENTWAIT	= jM_NIO|jT_1|jS_DO|jF_DB,	//0x50,80  -> 17956866
	jIDENTIFYING= jM_NIO|jT_1|jS_DOING,	//0x52,
	jIDENTIFIED	= jM_NIO|jT_1|jS_DONE,		//0x5f,

	jADDWAIT	= jM_NIO|jT_2|jS_DO|jF_DB,	//0x70,112 -> 19005442
	jADDING		= jM_NIO|jT_2|jS_DOING,	//0x72,
	jADDED		= jM_NIO|jT_2|jS_DONE,		//0x7f,

	jMOVEWAIT	= jM_DIO|jT_2|jS_DO|jF_DB,	//0x90,144 -> 18939906
	jMOVING		= jM_DIO|jT_2|jS_DOING,	//0x92,
	jMOVECHECK	= jM_DIO|jT_2|jU_2|jS_DOING,//0x94,
	jMOVED		= jM_DIO|jT_2|jS_DONE,		//0x9f,

//	INPUTWAIT	= M_USR|U_1|S_DO|F_DB,	//0xb0,

	jREMWAIT	= jM_NIO|jT_4|jS_DO|jF_DB,	//0xc0,
	jREMING		= jM_NIO|jT_4|jS_DOING,	//0xc2,

	jPARSEWAIT	= jM_DIO|jT_4|jS_DO|jF_DB,	//0xd0,
	jPARSING	= jM_DIO|jT_4|jS_DOING	//0xd2,
};

class myArray;
class myHashSet;
class AdbFile;

class AmeJob{
	friend class AmeJobMan;
	friend class AmeJobList;
private:
	wxCriticalSection m_cs;
	//wxString m_file; //wxFile
	wxFileName m_file_name;
	wxString m_ed2, m_err, m_fn;

	AdbFile* m_fa;
	
	long m_size;
	size_t m_idx;
	int m_status;//, m_substa, m_health;
	
	void setHealth(int health);
	void setHealth0(int health);
	void setStatus(int status, bool test=false);
	bool rename(wxString path);
	void find(wxString path);
public:
	bool m_new;
	int m_did;
	wxString m_md5, m_sha, m_crc, m_tth, m_ori, m_avx;
	AmeJob(wxString path, int status, size_t id, long size=0);
	~AmeJob(void);

	size_t getID(void){ return m_idx; }
	long getSize(void){ return m_size;};
	AdbFile* getAdbFile(void){ AL(); return m_fa; };
	void setAdbFile(AdbFile* f){ AL(); m_fa = f;};
	void deleteAdbFile();
	wxString getEd2k(){ AL(); return m_ed2; }
	void setEd2k(wxString s){ AL(); m_ed2 = s; }
	wxString getNewFilePath(){ AL(); return m_fn; }
	void setNewFilePath(wxString s){ AL(); m_fn = s; }
	wxString getAbsolutePath(void);
	void setAbsolutePath(wxString s);

	int getStatus(void);
	int getHealth(void);
	int getRegVal(void);
	bool check(int s);
	bool checkOr(int s);
	bool isLocked(int s);
	wxString getStatusText(void);
	wxString getFileName(void);
	wxString getFolderPath(void);
	wxString getFileExt(void);
	wxString dump(void);
	void setError(wxString err);
	wxString getError(void){ return m_err; }
	void updateHealth(int i);

	void makeMap(StringMap *sm);
};
class AmeJobMan{
public:
	static bool makeDirs(wxFileName f);
	static bool removeFile(wxString path);
	//static bool removeDirs(wxFileName f);
	static void updateStatus(AmeJob *j, int s, bool chck=false);
	static bool updatePath(AmeJob *j);
	static bool updatePath(AmeJob *job, wxString f);
	static bool add_to_mylist;
	static void setPath(AmeJob *job, wxString &path, bool parent);
	static void setName(AmeJob *job, wxString &s);
	static void restoreName(AmeJob *job);
};
class AmeJobList{
private:
	myArray* m_list;
	myHashSet* m_hs;
	wxCriticalSection m_cs;
public:
	AmeJobList(void);
	~AmeJobList(void);
	bool addFile(wxString path);
	bool addJob(AmeJob *j);
	void addPath(wxString path);
	size_t count(void);
	AmeJob* get(size_t i);
	void sortColumn(int i);
};

#endif //__JOB_H
