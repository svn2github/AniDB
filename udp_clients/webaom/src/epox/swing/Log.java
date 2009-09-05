// Copyright (C) 2005-2006 epoximator
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

/*
 * Created on 29.01.05
 *
 * @version 	1.06, 1.05, 1.00
 * @author 		epoximator
 */
package epox.swing;

public interface Log{
	public void println(Object o);
	public void status0(String str);
	public void status1(String str);
}/*
class DebugLog implements Log{
	public void println(String str){
		System.out.println(str);
	}
	public void status0(String str){}
	public void status1(String str){}
}*/
