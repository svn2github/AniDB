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
 * Edonkey is an implementation of the AbstractChecksum in order to calculate a
 * message digest in the form of edonkey/emule hash. It uses the MD4 algorithm
 * as an auxiliary algorithm, from the GNU crypto project,
 * http://www.gnu.org/software/classpathx/crypto
 *
 *****************************************************************************/
package hashing;

import java.security.NoSuchAlgorithmException;

/**
 * A class that can be used to compute the eDonkey of a data stream.
 */

public class Edonkey extends AbstractChecksum {

    private final static String AUX_ALGORITHM = "md4";
    private IMessageDigest md4 = null;
    private IMessageDigest md4final = null;
    private boolean virgin=true;


    private final static int BLOCKSIZE = 9728000; // 9500 * 1024;
    private byte[] edonkeyHash = new byte[16]; // 16 bytes, 128 bits
    private byte[] digest = null;

    /** Creates a new Edonkey object */
    public Edonkey() throws NoSuchAlgorithmException {
        super();
        separator=" ";
        encoding=HEX;
        md4 = HashFactory.getInstance(AUX_ALGORITHM);
        if (md4 == null) throw new NoSuchAlgorithmException(AUX_ALGORITHM + " is an unknown algorithm.");
        md4final = HashFactory.getInstance(AUX_ALGORITHM);
        virgin=true;
    }

    public void reset() {
        md4.reset();
        md4final.reset();
        length=0;
        virgin=true;
    }

    public void update(byte b) {
        md4.update(b);
        length++;

        if ((length % BLOCKSIZE) == 0) {
          System.arraycopy(md4.digest(), 0, edonkeyHash, 0, 16);
          md4final.update(edonkeyHash,0,16);
          md4.reset();
        }
    }

    public void update(int b) {
        update((byte)(b & 0xFF));
    }

    public void update(byte[] buffer, int offset, int len) {
        int zuSchreiben = len-offset; // XXX
        int passed = (int)(length % BLOCKSIZE);
        int platz = BLOCKSIZE-passed;

        // |___________XXX....|_____
        if (platz > zuSchreiben) {
           md4.update(buffer,offset,len);
           length+=len;
        } else
        // |_______________XXX|_____
        if (platz == zuSchreiben) {
          md4.update(buffer,offset,len);
          length+=len;
          System.arraycopy(md4.digest(), 0, edonkeyHash, 0, 16);
          md4final.update(edonkeyHash,0,16);
          md4.reset();
        } else
        // |________________XX|X____
        if (platz < zuSchreiben) {
          md4.update(buffer,offset,platz);
          length+=platz;

          System.arraycopy(md4.digest(), 0, edonkeyHash, 0, 16);
          md4final.update(edonkeyHash,0,16);
          md4.reset();

          md4.update(buffer,offset+platz,zuSchreiben-platz);
          length+=zuSchreiben-platz;
        }

    }

    public String toString() {
        return getFormattedValue()  + separator +
        (isTimestampWanted() ? getTimestampFormatted() + separator : "")+
        getFilename();
    }

    public byte[] getByteArray() {
        if (virgin) {
          if (length < BLOCKSIZE) {
              // if only one block, partial md4 hash = final hash
              System.arraycopy(md4.digest(), 0, edonkeyHash, 0, 16);
          } else {
              // let's copy the md4final object first
              // so we can launch getHexValue multiple times
              IMessageDigest md4temp = (IMessageDigest)md4final.clone();

              // if more then one block, final hash = hash of all partial hashes
              md4temp.update(md4.digest(), 0, 16);
              System.arraycopy(md4temp.digest(), 0, edonkeyHash, 0, 16);
          }
          virgin=false;
          digest=edonkeyHash;
        }
        // we don't expose internal representations
        byte[] save = new byte[digest.length];
        System.arraycopy(digest,0,save,0,digest.length);
        return save;
    }

   /*
     Testcase:
     a 3 GB file (length=3221225472), filled with random bytes
     (Java seed=0), returns the value 0121DA2F201ADA2E2AC81DB26F8DA5EC
   */

}
