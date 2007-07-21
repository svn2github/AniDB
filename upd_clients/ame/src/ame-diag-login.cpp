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

#include <wx/wx.h>

#include <wx/gbsizer.h>
#include <wx/regex.h>

#include "ame-diag-login.h"
#include "anidb-base.h"

BEGIN_EVENT_TABLE(AmeDialogLogin, wxDialog)
    EVT_BUTTON(wxID_OK, AmeDialogLogin::OnOK)
END_EVENT_TABLE()

void AmeDialogLogin::OnOK(wxCommandEvent& WXUNUSED(event) ){
    wxString u = m_tc_user->GetValue();
	wxString p = m_tc_pass->GetValue();
	wxString a = m_tc_apip->GetValue();

	wxRegEx exp(wxT("^[a-zA-Z0-9]{3,10}$"));

	if(u.Length()<3)
		SetTitle(wxT("Username too short!"));
	else if(u.Length()>10)
		SetTitle(wxT("Username too long!"));
	else if(!exp.Matches(u))
		SetTitle(wxT("Only letters and digits in username!"));
	else if(p.Length()<4)
		SetTitle(wxT("Password too short!"));
	else{
		m_up->username = u.Lower();
		m_up->password = p;
		m_up->apipass = a;
		EndModal(wxID_OK);
	}
}

AmeDialogLogin::AmeDialogLogin(wxWindow *parent, AdbUserPass *up)
: wxDialog(parent, wxID_ANY, wxT("Login:"), wxDefaultPosition, wxSize(500,150), wxDEFAULT_DIALOG_STYLE|wxDIALOG_MODAL|wxRESIZE_BORDER){
	m_up = up;

    wxBeginBusyCursor();

	wxPanel *panel = new wxPanel(this, wxID_ANY);

	m_tc_user = new wxTextCtrl(panel, wxID_ANY, m_up->username);
	m_tc_pass = new wxTextCtrl(panel, wxID_ANY, m_up->password, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	m_tc_apip = new wxTextCtrl(panel, wxID_ANY, m_up->apipass);
	m_tc_apip->Enable(false);

	wxGridBagSizer *sizer = new wxGridBagSizer(2,2);
	sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Username")), wxGBPosition(0,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer->Add(m_tc_user, wxGBPosition(0,1), wxDefaultSpan, wxEXPAND);
	sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Password")), wxGBPosition(1,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer->Add(m_tc_pass, wxGBPosition(1,1), wxDefaultSpan, wxEXPAND);
	sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("API Pass")), wxGBPosition(2,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer->Add(m_tc_apip, wxGBPosition(2,1), wxDefaultSpan, wxEXPAND);
	
	sizer->AddGrowableCol(1);
	sizer->AddGrowableRow(0);
	sizer->AddGrowableRow(1);
	sizer->AddGrowableRow(2);

	panel->SetSizer(sizer);

	wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
    sizerTop->Add(panel, wxSizerFlags(1).Expand().Border());
	sizerTop->Add( CreateButtonSizer( wxOK|wxCANCEL|wxCENTRE|wxWS_EX_VALIDATE_RECURSIVELY ), 0, wxALIGN_CENTRE|wxEXPAND|wxALL, 10 );

    SetSizerAndFit(sizerTop);
	if(m_up->username.Length()<1) m_tc_user->SetFocus();
	else if(m_up->password.Length()<1) m_tc_pass->SetFocus();

    wxEndBusyCursor();
}