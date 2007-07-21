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

#ifndef __NETIO_H
#define __NETIO_H

class wxThread;
class wxFrame;
class AmeJob;
class MyCallBack;
class AdbConnection;
class AdbConOptions;

class AmeThreadNetIO : public wxThread
{
private:
	AdbConnection* ac;
	AdbConOptions* m_opt;
	MyCallBack *m_cb;
	wxString m_me;
	wxFrame* m_frame;
	bool identify(AmeJob* j);
	bool add(AmeJob* j);
	bool remove(AmeJob* j);
public:
	AmeThreadNetIO(wxFrame* frame, AdbConOptions *opt, AdbUserPass *up, wxString me);
	~AmeThreadNetIO();
	virtual void *Entry();
	virtual void OnExit();
};

#endif //__NETIO_H
