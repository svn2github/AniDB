/******************************************************************************
 *
 * Jacksum version 1.7.0 - checksum utility in Java
 * Copyright (C) 2001-2006 Dipl.-Inf. (FH) Johann Nepomuk Loefflmann,
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
package hashing;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * A wrapper class that can be used to compute MD5, SHA-1, SHA-256, SHA-384 and SHA-512
 * (provided by your JVM vendor).
 */
public class MD extends AbstractChecksum {

    private MessageDigest md = null;
    private boolean virgin=true;
    private byte[] digest = null;

    public MD(String arg) throws NoSuchAlgorithmException {
        // value=0; we don't use value, we use md
        length=0;
        filename=null;
        separator=" ";
        encoding=HEX;
        virgin=true;
        md = MessageDigest.getInstance(arg);
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
