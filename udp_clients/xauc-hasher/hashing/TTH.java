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

package hashing;

public class TTH extends AbstractChecksum{
	private TigerTree md;
	private boolean virgin = true;
	private byte[] digest = null;
	
	public TTH() {
        // value=0; we don't use value, we use md
        length=0;
        filename=null;
        separator=" ";
        encoding=HEX;
        virgin=true;
        md = new TigerTree();
    }

    public void reset() {
        md.reset();
        length=0;
        virgin=true;
    }

    public void update(byte[] buffer, int offset, int len) {
        md.update(buffer,offset,len);
        length+=len;
    }

    public void update(byte b) {
        md.update(b);
        length++;
    }

    public void update(int b) {
        update((byte)(b & 0xFF));
    }

    public String toString() {
        return getFormattedValue()+separator+
        (isTimestampWanted()? getTimestampFormatted()+separator:"")+
        getFilename();
    }

    public byte[] getByteArray() {
        if (virgin) {
            digest=md.digest();
            virgin=false;
        }
        // we don't expose internal representations
        byte[] save = new byte[digest.length];
        System.arraycopy(digest,0,save,0,digest.length);
        return save;
    }

}