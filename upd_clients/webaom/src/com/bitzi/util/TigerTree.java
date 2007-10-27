/* (PD) 2003 The Bitzi Corporation
 * Please see http://bitzi.com/publicdomain for more info.
 *
 * $Id: TigerTree.java,v 1.2 2003/04/13 03:22:15 gojomo Exp $
 */
package com.bitzi.util;

import gnu.crypto.hash.Tiger;

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
	//private MessageDigest tiger;
	private Tiger tiger;

	/** Interim tree node hash values */
	private Vector/*!<byte[]>*/ nodes;

	/**
	 * Constructor
	 */
	public TigerTree(){
		super("TigerTree");
		buffer = new byte[BLOCKSIZE];
		bufferOffset = 0;
		byteCount = 0;

		tiger = new Tiger();
		nodes = new Vector/*!<byte[]>*/();
	}
	protected int engineGetDigestLength()
	{
	    return HASHSIZE;
	}
	protected void engineUpdate(byte in)
	{
		byteCount += 1;
		buffer[bufferOffset++] = in;
		if( bufferOffset==BLOCKSIZE )
		{
			blockUpdate();
			bufferOffset = 0;
		}
	}
	protected void engineUpdate(byte[] in, int offset, int length)
	{
		byteCount += length;

		int remaining;
		while( length >= (remaining = BLOCKSIZE - bufferOffset) )
		{
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
	protected byte[] engineDigest()
	{
		byte[] hash = new byte[HASHSIZE];
		try{
			engineDigest(hash, 0, HASHSIZE);
		}catch(DigestException e){ e.printStackTrace();
			return null;
		}
		return hash;
	}
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
	protected void engineReset()
	{
		bufferOffset = 0;
		byteCount = 0;
		nodes = new Vector/*!<byte[]>*/();
		tiger.reset();
	}
	public Object clone() throws CloneNotSupportedException
	{
		throw new CloneNotSupportedException();
	}
	/**
	 * Update the internal state with a single block of size 1024
	 * (or less, in final block) from the internal buffer.
	 */
	protected void blockUpdate() {
		tiger.reset();
		tiger.update((byte)0); // leaf prefix
		tiger.update(buffer,0,bufferOffset);
		if((bufferOffset==0)&(nodes.size()>0))
			return; // don't remember a zero-size hash except at very beginning
		byte [] b = tiger.digest();

		nodes.addElement(b);
	}
	public static String tl(byte[] b){
		String str = "";
		for(int i=0; i<24; i++)
			str+=(b[i]<0?256+b[i]:b[i])+",";
		return str;
	}
}