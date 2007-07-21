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

#define MYEXEC(x) _wspawnlp(_P_DETACH, L"rundll32", L"rundll32", L"url.dll,FileProtocolHandler", SP1("\"%s\"",x), NULL)
#define MYEXPL(x) _wspawnlp(_P_DETACH, L"explorer", L"explorer", L"", x, NULL)
#define AVDUMP(x) _wspawnlp(_P_DETACH, L"cmd", L"cmd", L"/C", L"start", L"avdump", L"-ps", SP1("\"%s\"",x), NULL)