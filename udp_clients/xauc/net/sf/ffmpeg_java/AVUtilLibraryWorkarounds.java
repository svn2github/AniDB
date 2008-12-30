package net.sf.ffmpeg_java;

import net.sf.ffmpeg_java.FFMPEGLibrary.AVRational;

import com.sun.jna.Pointer;

/**
 * Pure java implentation of some functions from {@link AVUtilLibrary}, as well as functions to deal with
 * JNA's inability to return structures..
 * @author	Uwe Mannl
 * @author Ken Larson
 *
 */
public class AVUtilLibraryWorkarounds
{
    final static long INT64_MIN = Long.MIN_VALUE; ///-9223372036854775808i64;

    // TODO: implement using JNA:

    /** This one works with JNA as well. */
    public static long av_rescale_q(long a, AVRational bq, AVRational cq){
		long b= bq.num * (long)cq.den;
		long c= cq.num * (long)bq.den;
	    return av_rescale_rnd(a, b, c, AVUtilLibrary.AV_ROUND_NEAR_INF);
	}
    
    /** This one works with JNA as well. */
    public static long av_rescale(long a, long b, long c){
        return av_rescale_rnd(a, b, c, AVUtilLibrary.AV_ROUND_NEAR_INF);
    }

	
	public static long av_rescale_rnd(long a, long b, long c, int rnd){
		long r=0;
	    assert(c > 0);
	    assert(b >=0);
	    assert(rnd >=0 && rnd<=5 && rnd!=4);
	    
	    if(a<0 && a != INT64_MIN) return -av_rescale_rnd(-a, b, c, rnd ^ ((rnd>>1)&1));

	    if(rnd==AVUtilLibrary.AV_ROUND_NEAR_INF) r= c/2;
	    else if((rnd&1)==1)             r= c-1;

	    if(b<=Integer.MAX_VALUE && c<=Integer.MAX_VALUE){
	        if(a<=Integer.MAX_VALUE)
	            return (a * b + r)/c;
	        else
	            return a/c*b + (a%c*b + r)/c;
	    }else{
	//#if 1
	    	long a0= a&0xFFFFFFFF;	// uint64_t
	    	long a1= a>>32;			// uint64_t
	        long b0= b&0xFFFFFFFF;	// uint64_t
	        long b1= b>>32;			// uint64_t
	    	long t1= a0*b1 + a1*b0;	// uint64_t
	    	long t1a= t1<<32;		// uint64_t
	        int i;

	        a0 = a0*b0 + t1a;
	        a1 = a1*b1 + (t1>>32) + ((a0<t1a) ? 1 : 0);
	        a0 += r;
	        a1 += (a0<r) ? 1 : 0;

	        for(i=63; i>=0; i--){
	            a1+= a1 + ((a0>>i)&1);
	            t1+=t1;
	            if(c <= a1){
	                a1 -= c;
	                t1++;
	            }
	        }
	        return t1;
	    }
	//#else
//	        AVInteger ai;
//	        ai= av_mul_i(av_int2i(a), av_int2i(b));
//	        ai= av_add_i(ai, av_int2i(r));
//
//	        return av_i2int(av_div_i(ai, av_int2i(c)));
//	    }
	//#endif
	}
	
	public static AVRational av_d2q(double d, int max)
	{
		return new AVRational(AVUtilLibrary.INSTANCE.av_d2q(d, max));
	}

	private static Boolean bigEndian;
	
	public static boolean isBigEndian()
	{
		if (bigEndian != null)
			return bigEndian;
		// first, try easy way:
		if (System.getProperty("sun.cpu.endian") != null)
		{	bigEndian = System.getProperty("sun.cpu.endian").equals("big"); 
			return bigEndian;
		}
		// otherwise figure it out ourselves
		final Pointer p = AVUtilLibrary.INSTANCE.av_malloc(4);
		p.setInt(0, 1);
		bigEndian = p.getByte(0) == 0;
		AVUtilLibrary.INSTANCE.av_free(p);
		return bigEndian;
		
	}
	
}
