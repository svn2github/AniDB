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

#ifndef __RULES_H
#define __RULES_H

class AmeJob;
class StringMap;
//class wxCriticalSection;

class AmeRules{
private:
	wxArrayString m_rf, m_rt;
	wxString m_smov, m_sren;

	wxString fin(AmeJob *j, StringMap &m, wxString s);
	wxString get(AmeJob *j, StringMap &m, wxString s);
	void build(AmeJob *j, StringMap &m, wxString s, wxArrayString &sch);
	bool handle(wxString s, wxArrayString &sch);
	bool check_a(AmeJob *j, StringMap &m, wxString s);
	bool check_b(AmeJob *j, StringMap &m, wxString s);
	bool check_c(AmeJob *j, StringMap &m, wxString s, wxChar c);

	wxCriticalSection m_cs;
public:
	wxString apply(AmeJob *j);

	wxString getRuleName(void);
	wxString getRulePath(void);
	wxString getEncRepl(void);
	void setRuleName(wxString s);
	void setRulePath(wxString s);

	bool addReplaceRule(wxString f, wxString t);
	bool getReplaceRule(size_t i, wxString &f, wxString &t);
	bool removeReplaceRule(size_t i);
	size_t getReplaceCount(void);
};

#endif //__RULES_H