/*
 * Created on 10.mar.2006 15:53:35
 * Filename: Progr.java
 */
package epox.util;

public class Bits {
	private int mIlen;
	private byte[] mBuse;
	public Bits(int len){
		mIlen = len;
		mBuse = new byte[(len+7)/8];
	}
	public boolean get(int i){
		if(i>=mIlen) return false;
		return (1&(mBuse[i/8]>>(i%8)))==1;
	}
	/**
	 * Sets the value at specified index.
	 * @param i Index of bit.
	 * @param b Value of bit.
	 * @return false if the index is out of bounds.
	 */
	public boolean set(int i, boolean b){
		if(i>=mIlen) return false;
		if(b) mBuse[i/8] |= (byte)(1<<(i%8));
		else mBuse[i/8] &= (byte)~(1<<(i%8));
		return true;
	}
	/**
	 * Set/unset, the next unset/set bit from 'left to right'/'right to left'.
	 * @param b Set/unset
	 * @return true if one bit was changed.
	 */
	public boolean fill(boolean b){
		if(b){
			for(int i=0; i<mIlen; i++)
				if(!get(i)){
					set(i, true);
					return true;
				}
			return false;
		}
		for(int i=mIlen-1; i>=0; i--)
			if(get(i)){
				set(i, false);
				return true;
			}
		return false;
	}
	public String toString(){
		StringBuffer sb = new StringBuffer(mIlen);
		for(int i=0; i<mIlen; i++)
			sb.append(get(i)==true?1:0);
		return sb.toString();
	}
	/**
	 * Counts all set bits.
	 * @return Number of set bits.
	 */
	public int cnt(){
		int x = 0;
		for(int i=0; i<mIlen; i++)
			if(get(i)) x++;
		return x;
	}
	/**
	 * First bit.
	 * @return The value of the first bit.
	 */
	public boolean first(){
		return get(0);
	}
	/**
	 * Last bit.
	 * @return The value of the last bit.
	 */
	public boolean last(){
		return get(mIlen-1);
	}
	public int switchCount(){
		//if(mIlen<1) return 0;
		boolean b = get(0);
		int n = 0;
		for(int i=1; i<mIlen; i++)
			if(get(i)!=b){
				n++;
				b = !b;
			}
		return n;
	}
	public boolean holex(){
		boolean f = false;
		for(int i=mIlen-1; i>=0; i--)
			if(f){
				if(!get(i)) return true;
			}else if(get(i)&&!f) f = true;
		return false;
	}
	public void reset(){
		for(int i=0; i<mBuse.length; i++)
			mBuse[i] = 0;
	}
}
