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

#ifndef __AUTOCREQ_H
#define __AUTOCREQ_H

#ifndef _DEBUG
int do_your_init(const char *name, const char *pass, const char *host, int port, int tout, bool enable_pong=false);
int do_your_session(bool first = true);
int do_your_session_retry(int tries_left);
bool do_your_thing(const char *buffer, int len, int tries_left);
void do_your_cleanup();
#endif

#endif //__AUTOCREQ_H