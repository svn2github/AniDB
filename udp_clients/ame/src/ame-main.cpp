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

//#if defined(__WXMSW__) && !defined(__WXWINCE__)
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df'\"")
//#endif

//#include <wx/wxprec.h>

//#ifndef WX_PRECOMP
#include <wx/wx.h>
//#endif

#include <wx/db.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/timer.h>
#include <wx/socket.h>
#include <wx/tokenzr.h>

#include <wx/gbsizer.h>
#include <wx/listctrl.h>
#include <wx/html/htmlwin.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <process.h>

#include "ame-id.h"
#include "ame-tools.h"
#include "ame-db.h"
#include "ame-job.h"
#include "ame-main.h"
#include "ame-cache.h"
#include "anidb-base.h"
#include "ame-rules.h"
#include "ame-thread-netio.h"
#include "ame-thread-diskio.h"
#include "ame-options.h"
#include "ame-diag-login.h"
#include "ame-strc.h"

//BEGIN_EVENT_TABLE(MyHtmlFrame, wxFrame)
//	EVT_CLOSE( MyHtmlFrame::OnCloseWindow )
//END_EVENT_TABLE()
BEGIN_EVENT_TABLE(AmeFrame, wxFrame)
	EVT_MENU(ID_Save, AmeFrame::OnSave)
    EVT_MENU(ID_File, AmeFrame::OnFile)
    EVT_MENU(ID_Folder, AmeFrame::OnFolder)
    EVT_MENU(ID_Disk, AmeFrame::OnDisk)
	EVT_MENU(ID_Net, AmeFrame::OnNet)
	EVT_MENU(ID_Database, AmeFrame::OnDatabase)
	EVT_MENU(ID_Quit, AmeFrame::OnQuit)
    EVT_MENU(ID_WIKI, AmeFrame::OnWiki)
	EVT_MENU(ID_About, AmeFrame::OnAbout)
	EVT_MENU(EVT_MESSAGE, AmeFrame::OnMessageEvt)
	EVT_MENU(EVT_GAUGE, AmeFrame::OnGaugeEvt)
    EVT_MENU(EVT_DB_BYE, AmeFrame::OnDbByeEvt)
    EVT_MENU(EVT_NET_BYE, AmeFrame::OnNetByeEvt)
    EVT_MENU(EVT_DISK_BYE, AmeFrame::OnDiskByeEvt)
    EVT_MENU(EVT_NET_LOGIN, AmeFrame::OnNetLoginEvt)
    EVT_MENU(EVT_JOB_LIST_LEN, AmeFrame::OnJobListLenEvt)
    EVT_MENU(EVT_JOB_LIST_REFRESH, AmeFrame::OnJobListRefreshEvt)
    EVT_RADIOBUTTON(EVT_RADIO_NAME, AmeFrame::OnRadioNameEvt)
	EVT_RADIOBUTTON(EVT_RADIO_PATH, AmeFrame::OnRadioPathEvt)
	EVT_BUTTON(EVT_BUTTON_APPLY_RULES, AmeFrame::OnButtonApplyRulesEvt)
	EVT_BUTTON(EVT_BUTTON_ADD_REPLACE, AmeFrame::OnButtonAddReplaceEvt)
	EVT_TEXT_ENTER(EVT_BUTTON_ADD_REPLACE, AmeFrame::OnButtonAddReplaceEvt)
	EVT_TEXT_ENTER(EVT_ADD_EXT, AmeFrame::OnExtensionAdd)
	EVT_TEXT_ENTER(ID_Database, AmeFrame::OnDatabase)
	EVT_LIST_KEY_DOWN(EVT_DEL_EXT, AmeFrame::OnExtensionRemove)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(AmeCtrlJobList, wxListCtrl)
	EVT_MOUSE_EVENTS(AmeCtrlJobList::OnMouseEvent)
	EVT_MENU(EVT_PAUSE, AmeCtrlJobList::DoPause)
	EVT_MENU(EVT_SHOW_INFO, AmeCtrlJobList::DoShowInfo)
	EVT_MENU(EVT_WATCH_NOW, AmeCtrlJobList::DoWatchNow)
	EVT_MENU(EVT_EXPLORE_FOLDER, AmeCtrlJobList::DoExploreFolder)
	EVT_MENU(EVT_REHASH, AmeCtrlJobList::DoReHash)
	EVT_MENU(EVT_REID, AmeCtrlJobList::DoReId)
	EVT_MENU(EVT_READD, AmeCtrlJobList::DoReAdd)
	EVT_MENU(EVT_REMOVE, AmeCtrlJobList::DoRemove)
	EVT_MENU(EVT_APPLY_RULES, AmeCtrlJobList::DoApplyRules)
	EVT_MENU(EVT_SET_FINISHED, AmeCtrlJobList::DoSetFinished)
	EVT_MENU(EVT_RESTORE_NAME, AmeCtrlJobList::DoRestoreName)
	EVT_MENU(EVT_SET_FOLDER, AmeCtrlJobList::DoSetFolder)
	EVT_MENU(EVT_SET_PARENT_FOLDER, AmeCtrlJobList::DoSetParentFolder)
	EVT_MENU(EVT_EDIT_FOLDER_PATH, AmeCtrlJobList::DoEditFolderPath)
	EVT_MENU(EVT_EDIT_FILE_NAME, AmeCtrlJobList::DoEditFileName)
	EVT_MENU(EVT_PARSE_WITH_AVINFO, AmeCtrlJobList::DoParseWithAvinfo)
	EVT_MENU(EVT_SET_FID, AmeCtrlJobList::DoSetFid)
	EVT_MENU(EVT_REMOVE_FROM_DB, AmeCtrlJobList::DoRemoveFromDb)
	EVT_KEY_DOWN(AmeCtrlJobList::OnChar)
	EVT_LIST_KEY_DOWN(LIST_JOBS, AmeCtrlJobList::OnListKeyDown)
//	EVT_LIST_COL_CLICK(LIST_JOBS, AmeCtrlJobList::OnColumnClick)//EVT_SORT_JOBS
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(AmeCtrlReplaceList, wxListCtrl)
	EVT_LIST_KEY_DOWN(LIST_REPLACE, AmeCtrlReplaceList::OnListKeyDown)
END_EVENT_TABLE()

IMPLEMENT_APP(AmeApp)

bool AmeApp::OnInit(){
	wxSocketBase::Initialize();
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(2430);

	m_disk_thread = NULL;
	m_net_thread = NULL;
	
	m_jl = new AmeJobList();
    
	m_opt = new AmeOptions();
	m_opt->load();

	m_rules = new AmeRules();
	m_rules->setRuleName(m_opt->getS(oSVRLSREN));
	m_rules->setRulePath(m_opt->getS(oSVRLSMOV));

	wxStringTokenizer st(m_opt->getS(oSREPLSYS), wxT("\1"));	
	{long i = 0;
	while(st.HasMoreTokens()){
		wxString from = st.GetNextToken(), to;
		if(st.HasMoreTokens()){
			to = st.GetNextToken();
			to.Replace(wxT("\\0"), wxT(""));
			if(!from.StartsWith(wxT("#")))
				m_rules->addReplaceRule(from, to);
		}else break;
	}}
	
	m_frame = new AmeFrame( this, wxT(STR_NAME), wxDefaultPosition/*wxPoint(150,150)*/, wxSize(800,600) );
    m_frame->Show(true);

	wxString user = m_opt->getS(oSUSRNAME);
	int x = user.Find(':');
	if(x>0) user = user.SubString(0, x-1);
	
	m_up = new AdbUserPass( user, wxT(""), wxT(""));

	m_db = new AmeDB(m_frame);

	SetTopWindow(m_frame);
    return TRUE;
}
int AmeApp::OnExit(){
	if(m_disk_thread!=NULL)
		m_disk_thread->Delete();
	if(m_net_thread!=NULL)
		m_net_thread->Delete();
	delete m_jl;
	delete m_rules;
	delete m_opt;
	delete m_up;
	delete m_db;
	AmeCache::clean();
	return 1;
}
MyHtmlWindow::MyHtmlWindow(wxWindow *parent) : wxHtmlWindow(parent){}
void MyHtmlWindow::OnLinkClicked(const wxHtmlLinkInfo& link){
	MYEXEC(link.GetHref());
}
MyHtmlFrame::MyHtmlFrame(const wxString& title, const wxString& url)
: wxFrame(A.m_frame, wxID_ANY, title, wxDefaultPosition, wxSize(600,600)/*, wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCAPTION*/){
	htm = new MyHtmlWindow(this);
	htm->SetPage(url);
	Show(true);
}
/*void MyHtmlFrame::OnCloseWindow(wxCloseEvent& event){
	Destroy();
}*/
AmeFrame::AmeFrame(AmeApp *a, const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size/*, wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCAPTION*/) {
	m_app = a;
	//
    wxMenu *menuFile = new wxMenu;
	wxMenu *menuWork = new wxMenu;
	wxMenu *menuData = new wxMenu;
	wxMenu *menuHelp = new wxMenu;
	
    menuFile->Append( ID_File, L"F&ile...", L"Select files." );
	menuFile->Append( ID_Folder, L"F&older...", L"Select folders." );
	menuFile->AppendSeparator();
    menuFile->Append( ID_Save, L"&Save Options", L"Save options. Not done automatically atm." );
	menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, L"E&xit" );
	
	m_miDisk = new wxMenuItem(menuWork, ID_Disk, wxT("&Disk Thread"), wxT("Start/stop disk operations."), wxITEM_CHECK);
	m_miNet = new wxMenuItem(menuWork, ID_Net, wxT("&Communication Thread"), wxT("Start/stop net operations."), wxITEM_CHECK);
	
	menuWork->Append( m_miDisk );
	menuWork->Append( m_miNet );
    
	m_miDB = new wxMenuItem(menuWork, ID_Database, wxT("&Database"), wxT("Load database."));
	menuData->Append( m_miDB );
	menuData->AppendSeparator();
    menuData->Append( ID_Import, L"&Import", L"Not implemented." );
	menuData->Append( ID_Export, L"&Export", L"Not implemented." );
    
	menuHelp->Append( ID_WIKI, L"&WIKI", L"Goto WebAOM wiki page for help." );
	menuHelp->Append( ID_About, L"&About...", L"About this application." );
    
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, L"&File" );
	menuBar->Append( menuWork, L"&Start" );
	menuBar->Append( menuData, L"&Data" );
	menuBar->Append( menuHelp, L"&Help" );

    SetMenuBar( menuBar );

    //int style = wxNO_FULL_REPAINT_ON_RESIZE|wxCLIP_CHILDREN|wxBC_DEFAULT;
	wxBookCtrlBase *m_book = new wxBookCtrl(this, wxID_ANY);//, wxDefaultPosition,wxDefaultSize,style);
//
//	OPTION PANEL
//
	wxPanel *panel_opt = new wxPanel(m_book);
	wxSizer *sizer_opt = new wxGridSizer(2,2,2,2);
	panel_opt->SetSizer(sizer_opt);
	
	wxStaticBox *box_oc = new wxStaticBox(panel_opt, wxID_ANY, wxT("Connection Options"));
	wxStaticBox *box_od = new wxStaticBox(panel_opt, wxID_ANY, wxT("Other Options"));
	wxStaticBox *box_oe = new wxStaticBox(panel_opt, wxID_ANY, wxT("File Extensions"));
	wxStaticBox *box_of = new wxStaticBox(panel_opt, wxID_ANY, wxT("Mylist Options"));
	
	wxSizer *sizer_boc = new wxStaticBoxSizer( box_oc, wxHORIZONTAL );
	wxSizer *sizer_bod = new wxStaticBoxSizer( box_od, wxHORIZONTAL );
	wxSizer *sizer_boe = new wxStaticBoxSizer( box_oe, wxHORIZONTAL );
	wxSizer *sizer_bof = new wxStaticBoxSizer( box_of, wxHORIZONTAL );
	
	panel_oc = new wxPanel(panel_opt);
	panel_od = new wxPanel(panel_opt);
	panel_oe = new wxPanel(panel_opt);
	panel_of = new wxPanel(panel_opt);
	
	sizer_boc->Add(panel_oc, 1, wxEXPAND|wxALL, 2);	
	sizer_bod->Add(panel_od, 1, wxEXPAND|wxALL, 2);	
	sizer_boe->Add(panel_oe, 1, wxEXPAND|wxALL, 2);	
	sizer_bof->Add(panel_of, 1, wxEXPAND|wxALL, 2);

	//Connection
	AmeOptions *o = m_app->m_opt;

	m_ocHost = new wxTextCtrl(panel_oc, wxID_ANY, o->getS(oSHOSTURL) );
	m_ocRport = new wxTextCtrl(panel_oc, wxID_ANY, wxString::Format(wxT("%d"), o->getI(oIRPORT)) );
	m_ocLport = new wxTextCtrl(panel_oc, wxID_ANY, wxString::Format(wxT("%d"), o->getI(oILPORT)) );
	m_ocDelay = new wxSlider(panel_oc, wxID_ANY, o->getI(oIDELAY), 3, 10, wxDefaultPosition, wxDefaultSize, wxSL_LABELS|wxSL_AUTOTICKS|wxSL_TOP);
	m_ocTimeout = new wxSlider(panel_oc, wxID_ANY, o->getI(oITIMEO), 10, 60, wxDefaultPosition, wxDefaultSize, wxSL_LABELS|wxSL_AUTOTICKS|wxSL_TOP);

	wxGridBagSizer *sizer_oc = new wxGridBagSizer(4,4);
	sizer_oc->Add(new wxStaticText(panel_oc, wxID_ANY, wxT("Host")), wxGBPosition(0,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_oc->Add(m_ocHost, wxGBPosition(0,1), wxGBSpan(1,2), wxEXPAND);
	sizer_oc->Add(new wxStaticText(panel_oc, wxID_ANY, wxT("Remote Port")), wxGBPosition(1,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_oc->Add(m_ocRport, wxGBPosition(1,1), wxGBSpan(1,2), wxEXPAND);
	sizer_oc->Add(new wxStaticText(panel_oc, wxID_ANY, wxT("Local Port")), wxGBPosition(2,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_oc->Add(m_ocLport, wxGBPosition(2,1), wxGBSpan(1,2), wxEXPAND);
	sizer_oc->Add(new wxStaticText(panel_oc, wxID_ANY, wxT("Delay (sec)")), wxGBPosition(3,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_oc->Add(m_ocDelay, wxGBPosition(3,1), wxGBSpan(1,2), wxEXPAND);
	sizer_oc->Add(new wxStaticText(panel_oc, wxID_ANY, wxT("Timeout (sec)")), wxGBPosition(4,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_oc->Add(m_ocTimeout, wxGBPosition(4,1), wxGBSpan(1,2), wxEXPAND);
	
	sizer_oc->AddGrowableCol(1);

	panel_oc->SetSizerAndFit(sizer_oc);

	//File

	m_ofAdd = new wxCheckBox(panel_of, wxID_ANY, wxT("Add files to mylist automatically"));
	m_ofAdd->SetValue(o->getB(oBADDFILE));
	m_ofWatched = new wxCheckBox(panel_of, wxID_ANY, wxT("Watched"));
	m_ofState = new wxComboBox(panel_of, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_DROPDOWN);
	m_ofSource = new wxTextCtrl(panel_of, wxID_ANY, o->getS(oSSOURCEF));
	m_ofStorage = new wxTextCtrl(panel_of, wxID_ANY, o->getS(oSSTORAGE));
	m_ofOther = new wxTextCtrl(panel_of, wxID_ANY, o->getS(oSOTHERIN), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	m_ofState->Append(STRFSTATE[0]);
	m_ofState->Append(STRFSTATE[1]);
	m_ofState->Append(STRFSTATE[2]);
	m_ofState->Append(STRFSTATE[3]);
	m_ofState->SetValue(STRFSTATE[o->getI(oISTATE)]);

	wxGridBagSizer *sizer_of = new wxGridBagSizer(4,4);
	sizer_of->Add(m_ofAdd,wxGBPosition(0,0),wxGBSpan(1,3), wxALIGN_CENTRE);
	sizer_of->Add(new wxStaticText(panel_of, wxID_ANY, wxT("State")), wxGBPosition(1,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_of->Add(m_ofState, wxGBPosition(1,1), wxGBSpan(1,2), wxEXPAND);
	sizer_of->Add(new wxStaticText(panel_of, wxID_ANY, wxT("Source")),wxGBPosition(2,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_of->Add(m_ofSource, wxGBPosition(2,1), wxGBSpan(1,2), wxEXPAND);
	sizer_of->Add(new wxStaticText(panel_of, wxID_ANY, wxT("Storage")),wxGBPosition(3,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_of->Add(m_ofStorage, wxGBPosition(3,1), wxGBSpan(1,2), wxEXPAND);
	sizer_of->Add(new wxStaticText(panel_of, wxID_ANY, wxT("Other")),wxGBPosition(4,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_of->Add(m_ofOther, wxGBPosition(4,1), wxGBSpan(2,2), wxEXPAND);
	sizer_of->Add(m_ofWatched,wxGBPosition(5,0),wxGBSpan(1,1), wxEXPAND);	

	sizer_of->AddGrowableRow(5);
    sizer_of->AddGrowableCol(1);

	panel_of->SetSizerAndFit(sizer_of);

	//Other
	m_odHashd = new wxTextCtrl(panel_od, wxID_ANY, o->getS(oSHASHDIR));
	m_odBrows = new wxTextCtrl(panel_od, wxID_ANY, o->getS(oSBROWSER));
	m_odMydb = new wxTextCtrl(panel_od, ID_Database, o->getS(oSMYDBURL), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_odMylog = new wxTextCtrl(panel_od, wxID_ANY, o->getS(oSLOGFILE));

	m_odSpsw = new wxCheckBox(panel_od, wxID_ANY, wxT("Store Password"));
	m_odSpsw->SetValue(o->getB(oBSTORPSW));
	m_odAldb = new wxCheckBox(panel_od, wxID_ANY, wxT("Auto db"));
	m_odAldb->SetValue(o->getB(oBALOADDB));
	m_odAlog = new wxCheckBox(panel_od, wxID_ANY, wxT("Auto log"));
	m_odAlog->SetValue(o->getB(oBAUTOLOG));
	m_odHcrc = new wxCheckBox(panel_od, wxID_ANY, wxT("CRC"));
	m_odHcrc->SetValue(o->getB(oBHASHCRC));
	m_odHmd5 = new wxCheckBox(panel_od, wxID_ANY, wxT("HashMD5"));
	m_odHmd5->SetValue(o->getB(oBHASHMD5));
	m_odHsha = new wxCheckBox(panel_od, wxID_ANY, wxT("SHA"));
	m_odHsha->SetValue(o->getB(oBHASHSHA));
	m_odHtth = new wxCheckBox(panel_od, wxID_ANY, wxT("TTH"));
	m_odHtth->SetValue(o->getB(oBHASHTTH));
	m_odHaic = new wxCheckBox(panel_od, wxID_ANY, wxT("AICH"));
	m_odHaic->SetValue(0);
	
	wxGridBagSizer *sizer_od = new wxGridBagSizer(4,4);
	sizer_od->Add(new wxStaticText(panel_od, wxID_ANY, wxT("Hash Dirs")), wxGBPosition(0,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_od->Add(m_odHashd, wxGBPosition(0,1), wxGBSpan(1,4), wxEXPAND);
	sizer_od->Add(new wxStaticText(panel_od, wxID_ANY, wxT("Browser Path")), wxGBPosition(1,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_od->Add(m_odBrows, wxGBPosition(1,1), wxGBSpan(1,4), wxEXPAND);
	sizer_od->Add(new wxStaticText(panel_od, wxID_ANY, wxT("My Database")), wxGBPosition(2,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_od->Add(m_odMydb, wxGBPosition(2,1), wxGBSpan(1,4), wxEXPAND);
	sizer_od->Add(new wxStaticText(panel_od, wxID_ANY, wxT("Log File")), wxGBPosition(3,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	sizer_od->Add(m_odMylog, wxGBPosition(3,1), wxGBSpan(1,4), wxEXPAND);
	sizer_od->Add(new wxStaticText(panel_od, wxID_ANY, wxT("Renaming")), wxGBPosition(4,0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL);
	//sizer_oc->Add(m_ocTimeout, wxGBPosition(4,1), wxGBSpan(1,2), wxEXPAND);
	sizer_od->Add(m_odAlog, wxGBPosition(5,0), wxDefaultSpan, wxALIGN_CENTRE);
	sizer_od->Add(m_odAldb, wxGBPosition(5,1), wxGBSpan(1,2), wxALIGN_CENTRE);
	sizer_od->Add(m_odSpsw, wxGBPosition(5,3), wxGBSpan(1,2), wxALIGN_CENTRE);
	sizer_od->Add(m_odHcrc, wxGBPosition(6,0), wxDefaultSpan, wxALIGN_CENTRE);
	sizer_od->Add(m_odHmd5, wxGBPosition(6,1), wxDefaultSpan, wxALIGN_CENTRE);
	sizer_od->Add(m_odHsha, wxGBPosition(6,2), wxDefaultSpan, wxALIGN_CENTRE);
	sizer_od->Add(m_odHtth, wxGBPosition(6,3), wxDefaultSpan, wxALIGN_CENTRE);
	sizer_od->Add(m_odHaic, wxGBPosition(6,4), wxDefaultSpan, wxALIGN_CENTRE);
		
	sizer_od->AddGrowableRow(5);
	sizer_od->AddGrowableCol(0);
	sizer_od->AddGrowableCol(1);
	sizer_od->AddGrowableCol(2);
	sizer_od->AddGrowableCol(3);
	sizer_od->AddGrowableCol(4);

	panel_od->SetSizer(sizer_od);

	m_odBrows->Enable(false);
	m_odMylog->Enable(false);
	m_odAlog->Enable(false);
	m_odAldb->Enable(false);
	m_odSpsw->Enable(false);
	m_odHcrc->Enable(false);
	m_odHmd5->Enable(false);
	m_odHsha->Enable(false);
	m_odHtth->Enable(false);
	m_odHaic->Enable(false);

	//Extensions
	m_oeNew = new wxTextCtrl(panel_oe, EVT_ADD_EXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_oeList = new wxListCtrl(panel_oe, EVT_DEL_EXT, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_NO_HEADER|wxSUNKEN_BORDER|wxLC_SINGLE_SEL);
	m_oeList->InsertColumn(0, wxT("ext"), wxLIST_FORMAT_LEFT, 200);
	{
		wxStringTokenizer st(o->getS(oSEXTENSN), wxT("\1"));
		long i=0;
		while(st.HasMoreTokens())
			m_oeList->InsertItem(i++,st.GetNextToken());
	}

	wxSizer *sizer_oe = new wxBoxSizer(wxVERTICAL);
	sizer_oe->Add(m_oeNew, 0, wxEXPAND|wxALL, 2);
	sizer_oe->Add(m_oeList,1, wxEXPAND|wxALL, 2);

	panel_oe->SetSizer(sizer_oe);

	//End

	sizer_opt->Add(sizer_boc, 1, wxEXPAND|wxALL, 2);
	sizer_opt->Add(sizer_bof, 1, wxEXPAND|wxALL, 2);
	sizer_opt->Add(sizer_bod, 1, wxEXPAND|wxALL, 2);
	sizer_opt->Add(sizer_boe, 1, wxEXPAND|wxALL, 2);
//
//	RULE PANEL
//
	wxPanel *panel_rls = new wxPanel(m_book);
	wxPanel *panel_rlsHeader = new wxPanel(panel_rls);
	wxPanel *panel_rlsFooter = new wxPanel(panel_rls);
	wxBoxSizer *sizerRules = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *sizerHeader = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *sizerFooter = new wxBoxSizer( wxHORIZONTAL );
	panel_rls->SetSizer( sizerRules );
	panel_rlsHeader->SetSizer( sizerHeader );
	panel_rlsFooter->SetSizer( sizerFooter );

	m_wxRB0 = new wxRadioButton( panel_rlsHeader, EVT_RADIO_NAME, wxT("Name") );
	m_wxRB0->SetValue(1);
	m_wxRB1 = new wxRadioButton( panel_rlsHeader, EVT_RADIO_PATH, wxT("Path") );
	m_buttonApplyRules = new wxButton( panel_rlsHeader, EVT_BUTTON_APPLY_RULES, wxT("Apply") );
	m_wxTCrules = new wxTextCtrl( panel_rls, -1, o->getS(oSVRLSREN), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	m_listReplace = new AmeCtrlReplaceList(panel_rls, LIST_REPLACE, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_VIRTUAL|wxSUNKEN_BORDER|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);
	m_listReplace->SetItemCount((long)wxGetApp().m_rules->getReplaceCount());

	m_wxTCrf = new wxTextCtrl(panel_rlsFooter, EVT_BUTTON_ADD_REPLACE, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_wxTCrt = new wxTextCtrl(panel_rlsFooter, EVT_BUTTON_ADD_REPLACE, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER  );
	wxButton * wxBins = new wxButton( panel_rlsFooter, EVT_BUTTON_ADD_REPLACE, wxT("Add") );

    sizerHeader->Add(m_wxRB0, 1, wxEXPAND|wxLEFT|wxRIGHT, 10);
	sizerHeader->Add(m_wxRB1, 1, wxEXPAND|wxLEFT|wxRIGHT, 10);
	sizerHeader->Add(m_buttonApplyRules, 0, wxLEFT|wxRIGHT, 10);	
	sizerFooter->Add(m_wxTCrf, 1, wxEXPAND|wxRIGHT, 4);
	sizerFooter->Add(m_wxTCrt, 1, wxEXPAND|wxRIGHT, 4);
	sizerFooter->Add(wxBins, 0);
	
	sizerRules->Add(panel_rlsHeader, 0, wxALIGN_CENTER|wxALL, 2);
	sizerRules->Add(m_wxTCrules, 1, wxEXPAND|wxALL, 2);
	sizerRules->Add(m_listReplace, 1, wxEXPAND|wxALL, 2);
	sizerRules->Add(panel_rlsFooter, 0, wxEXPAND|wxALL, 2);	

//
//	TABS
//
	m_listJobs = new AmeCtrlJobList(m_book, LIST_JOBS, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL | wxSUNKEN_BORDER);
	m_log = new wxTextCtrl( m_book, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	m_log->SetEditable(FALSE);

	m_debug = new wxTextCtrl( m_book, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	m_debug->SetEditable(FALSE);

	m_hash = new wxTextCtrl( m_book, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	//m_hash->SetEditable(FALSE);

	m_info = new wxTextCtrl( m_book, wxID_ANY, wxT(STR_INFO), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	m_info->SetEditable(FALSE);
	
	m_book->AddPage(panel_rls, wxT("Rules"));
	m_book->AddPage(panel_opt, wxT("Options"));	
	m_book->AddPage(m_listJobs, wxT("Jobs"));
	m_book->AddPage(m_hash, wxT("Hash"));
	m_book->AddPage(m_info, wxT("Info"));
	m_book->AddPage(m_log, wxT("Log"));
	m_book->AddPage(m_debug, wxT("Debug"));

	m_book->SetSelection(1);

	m_gauge = new wxGauge(this, -1, 100);
	m_gauge->SetValue(100);

	wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
	sizer->Add(m_book, 1, wxEXPAND | wxALL, 0 );
	sizer->Add(m_gauge, 0, wxEXPAND | wxALL, 0);
	SetSizer( sizer );

	m_sb = new wxStatusBar(this);
	m_sb->SetStatusText(wxT("..."));
	SetStatusBar(m_sb);
}
