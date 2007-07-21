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

#ifndef __AME_DIR_TRAV_H
#define __AME_DIR_TRAV_H

class wxDirTraverserSimple : public wxDirTraverser{
public:
    wxDirTraverserSimple(wxArrayString& files, wxArrayString& exts) : m_files(files),m_exts(exts) {}
    virtual wxDirTraverseResult OnFile(const wxString& filename){
		int x = filename.Find('.', TRUE);
		if(x>0){
			wxString ext = filename.SubString(x+1, filename.Length());
			for(size_t i=0; i<m_exts.Count();i++)
				if(!ext.CmpNoCase(m_exts.Item(i))){
					m_files.Add(filename);
					break;
				}
		}
		return wxDIR_CONTINUE;
	}
    virtual wxDirTraverseResult OnDir(const wxString& dirname){ return wxDIR_CONTINUE; }
private:
    wxArrayString& m_files;
	wxArrayString& m_exts;
};

#endif //__AME_DIR_TRAV_H