/******************************************************************************
 *
 * Jacksum version 1.5.0 - checksum utility in Java
 * Copyright (C) 2001-2004 Dipl.-Inf. (FH) Johann Nepomuk Loefflmann,
 * All Rights Reserved, http://www.jonelo.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * E-mail: jonelo@jonelo.de
 *
 *****************************************************************************/

package jonelo.jacksum.algorithm;

import java.util.zip.CRC32;

public class Crc32 extends AbstractChecksum {

	private CRC32 crc32 = null;

	public Crc32() {
		crc32 = new CRC32();
	}
	public void reset() {
		crc32.reset();
		length=0;
	}
	public void update(byte[] buffer, int offset, int len) {
		crc32.update(buffer,offset,len);
		length+=len;
	}
	public void update(int b) {
		crc32.update(b);
		length++;
	}
	public long getValue() {
		return crc32.getValue();
	}
	public String getHexValue() {
		String s = hexformat(getValue(),8);
		return (uppercase ? s.toUpperCase() : s);
	}
}
