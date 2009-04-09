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

// This public domain class has been modified by jonelo for Jacksum (GPL)
// - changed the package name
// - removed the main method
// - removed System.exit(0)
// - provided a named constructor to be able to calculate both
//   tiger and tiger2
// - replaced the cryptix Tiger provider with GNU's implementation
// - replaced MessageDigest with the AbstractChecksum
// - minor bug in blockUpdate() fixed
// - code reformatted

/* (PD) 2003 The Bitzi Corporation
 * Please see http://bitzi.com/publicdomain for more info.
 *
 * $Id: TigerTree.java,v 1.2 2003/04/13 03:22:15 gojomo Exp $
 */
package hashing;

import java.security.DigestException;
import java.security.MessageDigest;
import java.util.Enumeration;
import java.util.Vector;

/**
 * Implementation of THEX tree hash algorithm, with Tiger
 * as the internal algorithm (using the approach as revised
 * in December 2002, to add unique prefixes to leaf and node
 * operations)
 *
 * For simplicity, calculates one entire generation before
 * starting on the next. A more space-efficient approach
 * would use a stack, and calculate each node as soon as
 * its children ara available.
 */
public class TigerTree extends MessageDigest {
    private static final int BLOCKSIZE = 1024;
    private static final int HASHSIZE = 24;
    
    /** 1024 byte buffer */
    private final byte[] buffer;
    
    /** Buffer offset */
    private int bufferOffset;
    
    /** Number of bytes hashed until now. */
    private long byteCount;
    
    /** Internal Tiger MD instance */
    private Tiger tiger;
    
    /** Interim tree node hash values */
    @SuppressWarnings("unchecked")
	private Vector nodes;
    
    /**
     * Constructor
     */
	@SuppressWarnings("unchecked")
	public TigerTree(){
		super("TigerTree");
		buffer = new byte[BLOCKSIZE];
		bufferOffset = 0;
		byteCount = 0;

		tiger = new Tiger();
		nodes = new Vector/*!<byte[]>*/();
	}
    
    protected int engineGetDigestLength() {
        return HASHSIZE;
    }
    
    protected void engineUpdate(byte in) {
        byteCount += 1;
        buffer[bufferOffset++] = in;
        if( bufferOffset==BLOCKSIZE ) {
            blockUpdate();
            bufferOffset = 0;
        }
    }

    protected void engineUpdate(byte[] in, int offset, int length) {
        byteCount += length;
        
        int remaining;
        while( length >= (remaining = BLOCKSIZE - bufferOffset) ) {
            System.arraycopy(in, offset, buffer, bufferOffset, remaining);
            bufferOffset += remaining;
            blockUpdate();
            length -= remaining;
            offset += remaining;
            bufferOffset = 0;
        }
        
        System.arraycopy(in, offset, buffer, bufferOffset, length);
        bufferOffset += length;
    }
    
    
    protected byte[] engineDigest() {
        byte[] hash = new byte[HASHSIZE];
        try {
            engineDigest(hash, 0, HASHSIZE);
        } catch (DigestException e) {
            return null;
        }
        return hash;
    }
    
    
    @SuppressWarnings("unchecked")
	protected int engineDigest(byte[] buf, int offset, int len)
	throws DigestException
	{
		if(len<HASHSIZE)
			throw new DigestException();

		// hash any remaining fragments
		blockUpdate();
		// composite neighboring nodes together up to top value
		while (nodes.size() > 1) {
			Vector/*!<byte[]>*/ newNodes = new Vector/*!<byte[]>*/();
			Enumeration iter = nodes.elements();
			while (iter.hasMoreElements()) {
				byte[] left = (byte[])iter.nextElement();
				if(iter.hasMoreElements()) {
					byte[] right = (byte[])iter.nextElement();
					tiger.reset();
					tiger.update((byte)1); // node prefix
					tiger.update(left, 0, left.length);
					tiger.update(right, 0, right.length);
					newNodes.addElement(tiger.digest());
				} else
					newNodes.addElement(left);
			}
			nodes = newNodes;
		}
		System.arraycopy(nodes.elementAt(0), 0, buf, offset, HASHSIZE);
		engineReset();
		return HASHSIZE;
	}
    
    
    @SuppressWarnings("unchecked")
	protected void engineReset() {
        bufferOffset = 0;
        byteCount = 0;
        nodes = new Vector();
        tiger.reset();
    }
    
    public Object clone() throws CloneNotSupportedException {
        throw new CloneNotSupportedException();
    }
    
    /**
     * Update the internal state with a single block of size 1024
     * (or less, in final block) from the internal buffer.
     */
    @SuppressWarnings("unchecked")
	protected void blockUpdate() {
		tiger.reset();
		tiger.update((byte)0); // leaf prefix
		tiger.update(buffer,0,bufferOffset);
		if((bufferOffset==0)&(nodes.size()>0))
			return; // don't remember a zero-size hash except at very beginning
		byte [] b = tiger.digest();

		nodes.addElement(b);
	}
    
}
