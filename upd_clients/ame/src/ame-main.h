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

#ifndef __AME_MAIN_H
#define __AME_MAIN_H

static const wxString STRFSTATE[] = {wxT("Unknown"),wxT("On HDD"),wxT("On CD"),wxT("Deleted")};

class MyTextDialog : public wxDialog{
public:
	MyTextDialog(wxWindow *parent, wxString t, wxString s)
		: wxDialog(parent, wxID_ANY, t, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxDIALOG_MODAL|wxRESIZE_BORDER){
		wxSizer *sizer = new wxBoxSizer( wxHORIZONTAL);
		sizer->Add(new wxTextCtrl( this, wxID_ANY, s, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE), 1, wxEXPAND);
		SetSizer(sizer);
	} 
};

class AmeCtrlJobList: public wxListCtrl{
public:
    AmeCtrlJobList(wxWindow *parent,
               const wxWindowID id,
               const wxPoint& pos,
               const wxSize& size,
               long style)
        : wxListCtrl(parent, id, pos, size, style),
          m_attr(*wxBLUE, *wxLIGHT_GREY, wxNullFont)
        {
			int x = 0;
			InsertColumn(x++, wxT("#"), wxLIST_FORMAT_LEFT, 40);
			InsertColumn(x++, wxT("File"), wxLIST_FORMAT_LEFT, 600);
			InsertColumn(x++, wxT("Status"), wxLIST_FORMAT_LEFT, 140);
			InsertColumn(x++, wxT("Anime"));
			InsertColumn(x++, wxT("Episode"));
			InsertColumn(x++, wxT("Group"));
			InsertColumn(x++, wxT("lid"));
			InsertColumn(x++, wxT("Mds"));
        }

    // add one item to the listctrl in report mode
    //void InsertItemInReportView(int i);

    /*void OnColClick(wxListEvent& event);
    void OnColRightClick(wxListEvent& event);
    void OnColBeginDrag(wxListEvent& event);
    void OnColDragging(wxListEvent& event);
    void OnColEndDrag(wxListEvent& event);
    void OnBeginDrag(wxListEvent& event);
    void OnBeginRDrag(wxListEvent& event);
    void OnBeginLabelEdit(wxListEvent& event);
    void OnEndLabelEdit(wxListEvent& event);
    void OnDeleteItem(wxListEvent& event);
    void OnDeleteAllItems(wxListEvent& event);
    void OnGetInfo(wxListEvent& event);
    void OnSetInfo(wxListEvent& event);
    void OnSelected(wxListEvent& event);
    void OnDeselected(wxListEvent& event);
    void OnListKeyDown(wxListEvent& event);
    void OnActivated(wxListEvent& event);
    void OnFocused(wxListEvent& event);
    void OnCacheHint(wxListEvent& event);*/

    void OnChar(wxKeyEvent& event);
	void OnMouseEvent(wxMouseEvent& event);
	void OnColumnClick(wxListEvent &event);


	void DoPause(wxCommandEvent& event);
	void DoShowInfo(wxCommandEvent& event);
	void DoApplyRules(wxCommandEvent& event);
	void DoWatchNow(wxCommandEvent& event);
	void DoExploreFolder(wxCommandEvent& event);
	void DoReHash(wxCommandEvent& event);
	void DoReId(wxCommandEvent& event);
	void DoReAdd(wxCommandEvent& event);
	void DoRemove(wxCommandEvent& event);
	void DoSetFinished(wxCommandEvent& event);
	void DoRestoreName(wxCommandEvent& event);
	void DoSetFolder(wxCommandEvent& event);
	void DoSetParentFolder(wxCommandEvent& event);
	void DoEditFolderPath(wxCommandEvent& event);
	void DoEditFileName(wxCommandEvent& event);
	void DoSetFid(wxCommandEvent& event);
	void DoParseWithAvinfo(wxCommandEvent& event);
	void DoRemoveFromDb(wxCommandEvent& event);	
	void OnListKeyDown(wxListEvent& event);

private:
	AmeJob* GetSelectedJob(void);
    virtual wxString OnGetItemText(long item, long column) const;

    wxListItemAttr m_attr;

	DECLARE_NO_COPY_CLASS(AmeCtrlJobList)
    DECLARE_EVENT_TABLE()
};
class AmeCtrlReplaceList: public wxListCtrl{
public:
    AmeCtrlReplaceList(wxWindow *parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
        : wxListCtrl(parent, id, pos, size, style), m_attr(*wxBLUE, *wxLIGHT_GREY, wxNullFont)//, m_imageList(16, 16, TRUE)
        {
			//m_imageList.Add(wxICON(unchecked));
			//m_imageList.Add(wxICON(checked));
			//SetImageList(& m_imageList, wxIMAGE_LIST_SMALL);
			
			InsertColumn(0, wxT("#"), wxLIST_FORMAT_RIGHT, 40);
			InsertColumn(1, wxT("From"), wxLIST_FORMAT_CENTER, 370);
			InsertColumn(2, wxT("To"), wxLIST_FORMAT_CENTER, 365);
			//InsertColumn(3, wxT("Item"), wxLIST_FORMAT_LEFT, 200);
			//InsertColumn(4, wxT("Value"), wxLIST_FORMAT_LEFT, 80);
        }
	void OnListKeyDown(wxListEvent& event);
	//void addItem(wxString f, wxString t);
	bool IsChecked(long item) const;
	void SetChecked(long item, bool checked);
	void ClearChecked();
	
	virtual wxString OnGetItemText(long item, long column) const;
	virtual int OnGetItemImage(long item) const;

private:
	void OnMouseEvent(wxMouseEvent& event);

	//wxImageList m_imageList;
	wxListItemAttr m_attr;

	DECLARE_NO_COPY_CLASS(AmeCtrlReplaceList)
    DECLARE_EVENT_TABLE()
};
class AmeFrame: public wxFrame{
private:
	AmeApp * m_app;
public:
    AmeFrame(AmeApp *a, const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnWiki(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnFile(wxCommandEvent& event);
	void OnFolder(wxCommandEvent& event);
	void OnDisk(wxCommandEvent& event);
	void OnNet(wxCommandEvent& event);
	void OnDatabase(wxCommandEvent& event);
	
	
	void OnMessageEvt(wxCommandEvent& event);	
	
	void OnGaugeEvt(wxCommandEvent& event);
	void OnDiskByeEvt(wxCommandEvent& event);
	void OnNetByeEvt(wxCommandEvent& event);
	void OnNetLoginEvt(wxCommandEvent& event);
	void OnJobListLenEvt(wxCommandEvent& event);
	void OnJobListRefreshEvt(wxCommandEvent& event);
	void OnDbByeEvt(wxCommandEvent& event);	

	void OnRadioNameEvt(wxCommandEvent& event);
	void OnRadioPathEvt(wxCommandEvent& event);
	void OnButtonApplyRulesEvt(wxCommandEvent& event);
	void OnButtonAddReplaceEvt(wxCommandEvent& event);
	void OnExtensionAdd(wxCommandEvent& event);
	void OnExtensionRemove(wxListEvent& event);

	
	void InitWithReportItems();
	
	//GUI
	wxStatusBar *m_sb;
	AmeCtrlJobList *m_listJobs;
	AmeCtrlReplaceList *m_listReplace;
	
	wxTextCtrl *m_log, *m_debug, *m_hash, *m_info;
	wxTextCtrl *m_wxTCrules;
	wxTextCtrl *m_wxTCrf, *m_wxTCrt;

	wxListCtrl *m_oeList;
	wxTextCtrl *m_oeNew;
	
	wxGauge *m_gauge;
	wxRadioButton *m_wxRB0, *m_wxRB1;
	wxButton *m_buttonApplyRules;
	wxPanel *panel_oc, *panel_od, *panel_oe, *panel_of;

	wxTextCtrl *m_ocHost, *m_ocRport, *m_ocLport;
	wxSlider *m_ocDelay, *m_ocTimeout;
	wxMenuItem *m_miDisk, *m_miNet, *m_miDB;

	wxCheckBox *m_odSpsw,*m_odAldb,*m_odAlog,*m_odHcrc,*m_odHmd5,*m_odHsha,*m_odHtth,*m_odHaic;
	wxTextCtrl *m_odHashd,*m_odBrows,*m_odMydb,*m_odMylog;
	
	//Mylist data
	wxCheckBox *m_ofAdd, *m_ofWatched;
	wxTextCtrl *m_ofSource, *m_ofStorage, *m_ofOther;
	wxComboBox *m_ofState;

    DECLARE_EVENT_TABLE()
};
class MyHtmlWindow : public wxHtmlWindow{
public:
	MyHtmlWindow(wxWindow *parent);
	void OnLinkClicked(const wxHtmlLinkInfo& link);
};
class MyHtmlFrame : public wxFrame{
private:
	MyHtmlWindow *htm;
public:
    MyHtmlFrame(const wxString& title, const wxString& url);
	//void OnCloseWindow(wxCloseEvent& event);
	//DECLARE_EVENT_TABLE()
};

#endif //__AME_MAIN_H
