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
 * Created on 22.01.05
 *
 * @version 	01 (1.07)
 * @author 		epoximator
 */

package epox.util;

import com.bitzi.util.Base32;
import com.bitzi.util.TigerTree;

public class TTH extends jonelo.jacksum.algorithm.AbstractChecksum{
	private TigerTree tt;
	public TTH(){
		tt = new TigerTree();
	}
	public void update (byte buffer[], int offset, int len) {
		tt.update(buffer, offset, len);
	}
	public void reset(){
		tt.reset();
	}
	public void update(int b) {
		tt.update((byte)b);
	}
	public String getHexValue(){
		return Base32.encode(tt.digest()).toLowerCase();
	}
}