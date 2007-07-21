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

#ifndef __LOGINDIAG_H__
#define __LOGINDIAG_H__

class AdbUserPass;

class AmeDialogLogin : public wxDialog{
public:
    AmeDialogLogin(wxWindow *parent, AdbUserPass *up);
    void OnOK(wxCommandEvent& event);

protected:
    wxTextCtrl *m_tc_user, *m_tc_pass, *m_tc_apip;

private:
	AdbUserPass *m_up;
    DECLARE_EVENT_TABLE()
};

#endif //__LOGINDIAG_H__