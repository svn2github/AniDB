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

#ifndef __DISKIO_H
#define __DISKIO_H

class wxThread;
class wxFrame;
class AmeJob;
class AmeThreadDiskIO : public wxThread
{
private:
	wxFrame* m_frame;
	wxString m_dirs;
	wxArrayString m_exts;

	bool hash(AmeJob* j);
	bool move(AmeJob* j);
	void checkDirs(void);
	wxString check(AmeJob* j);
public:
	AmeThreadDiskIO(wxFrame* frame, wxString, wxArrayString);
	virtual void *Entry();
	virtual void OnExit();
};

#endif //__DISKIO_H
