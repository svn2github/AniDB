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

#include <iostream>
#include <sstream>

#include <MediaInfo/MediaInfo.h>

#ifdef ENABLE_WXW
#include <wx/wxprec.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#endif

#include "avinfo.h"
#include "utils.h"

#ifdef ENABLE_HASH

#include "avfp.h"
#include "avhash.h"
#include "ebml\EbmlUnicodeString.h"

#ifdef ENABLE_AC
#include "autocreq.h"
#endif

#endif

using namespace std;

struct config{
	bool
		b_pause_at_end,
		b_show_proc_time,
		b_newline_after_each_file,
		b_randomize_order,
		b_full_media_parsing,
		b_pause_after_each_file,
		b_delete_files,
		b_silent;

	int i_session_count,
		i_master_mode;

#ifdef ENABLE_HASH

#ifdef ENABLE_AC
	bool b_enable_auto_creq, b_enable_pong;
	int i_timeout_secs;
	int i_timeout_retries;
#endif
	
	int i_hash_mode;
	int i_hash_format;

	bool b_skip_media_parsing;
	bool b_enable_audio_hashing;

	#define RB_PARTS_MIN 2
	RingBuffer o_ring_buffer;

	hashc chk;

#ifdef ENABLE_FP
	char *o_foosicbuf;
#endif

#endif

#ifdef ENABLE_WXW
	wxFFile *f_log;
	wxFFile *f_files_done;
	wxFFile *f_ed2k_export;
#endif

};

bool do_work_inner(config &conf, vector<wstring> *files);