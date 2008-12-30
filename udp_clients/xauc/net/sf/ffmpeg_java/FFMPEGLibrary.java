package net.sf.ffmpeg_java;
import com.sun.jna.Library;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

/**
 * Based on FFMPEG Aug 12 2007.  
 * Contains general structure definitions common to libavcodec and libavformat.
 * From: libavutil/log.h, rational.h, and avutil.h - all incomplete.
 * No method/function declarations should be in this class, they should be in subclasses, as this
 * is a base interface with no actual associated native library.
 * @author Ken Larson
 *
 */
public interface FFMPEGLibrary extends Library 
{
//------------------------------------------------------------------------------------------------------------------------
// libavutil/log.h:
	
//  struct AVCLASS {
//  const char* class_name;
//  const char* (*item_name)(void*); /* actually passing a pointer to an AVCodecContext
//                                      or AVFormatContext, which begin with an AVClass.
//                                      Needed because av_log is in libavcodec and has no visibility
//                                      of AVIn/OutputFormat */
//  const struct AVOption *option;
//};
	
	public static class AVClass extends Structure
	{
		public String class_name;	
		public Pointer item_name;	
		public Pointer option;		
	}
	
	
	
// TODO: other structures from log.h
// end log.h
//------------------------------------------------------------------------------------------------------------------------

	
//------------------------------------------------------------------------------------------------------------------------
// libavutil/rational.h:
	
//	/**
//	 * Rational number num/den.
//	 */
//	typedef struct AVRational{
//	    int num; ///< numerator
//	    int den; ///< denominator
//	} AVRational;
	
	public static class AVRational extends Structure
	{
	    public int num; 
	    public int den; 
	    
	    public AVRational()
	    {	super();
	    }

		public AVRational(int num, int den)
		{
			super();
			this.num = num;
			this.den = den;
		}
		
		/**
		 * Workaround to allow this structure to be returned.
		 * @param value
		 */
		public AVRational(long value)
		{
			final int hi = (int) ((value >>> 32) & 0xffffffffL);
			final int lo = (int) (value & 0xffffffffL);
			
			if (AVUtilLibraryWorkarounds.isBigEndian())
			{	this.num = hi;
				this.den = lo;
			}
			else
			{	this.num = lo;
				this.den = hi;
			}
		}
		
		public long toLong()
		{
			if (!AVUtilLibraryWorkarounds.isBigEndian())
				return this.num | (this.den << 32);
			else
				return this.den | (this.num << 32);
				
			
		}
		
		public double toDouble()
		{
			return this.num / (double) this.den;
		}

		/**
		 * Rational to double conversion.
		 * @param a rational (implied as this one)
		 * @return (double) a
		 */
		public double av_q2d() { return toDouble(); }

		/**
		 * Compare two rationals.
		 * @param a first rational (implied as this one)
		 * @param b second rational
		 * @return 0 if a==b, 1 if a>b and -1 if a<b.
		 */
		public int av_cmp_q(AVRational b) {
			long tmp= this.num * (long)b.den - b.num * (long)this.den;
			if(tmp != 0) return (int)(tmp >> 63) | 1;
		    else return 0;
		}
	};

// TODO: other structures from rational.h
// end rational.h
//------------------------------------------------------------------------------------------------------------------------
	
	
//------------------------------------------------------------------------------------------------------------------------
// libavutil/avutil.h:
	

	
	public static final int PIX_FMT_NONE = -1;
	public static final int PIX_FMT_YUV420P = 0;   ///< Planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
	public static final int PIX_FMT_YUYV422 = 1;   ///< Packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
	public static final int PIX_FMT_RGB24 = 2;     ///< Packed RGB 8:8:8, 24bpp, RGBRGB...
	public static final int PIX_FMT_BGR24 = 3;     ///< Packed RGB 8:8:8, 24bpp, BGRBGR...
	public static final int PIX_FMT_YUV422P = 4;   ///< Planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
	public static final int PIX_FMT_YUV444P = 5;   ///< Planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
	public static final int PIX_FMT_RGB32 = 6;     ///< Packed RGB 8:8:8, 32bpp, (msb)8A 8R 8G 8B(lsb), in cpu endianness
	public static final int PIX_FMT_YUV410P = 7;   ///< Planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
	public static final int PIX_FMT_YUV411P = 8;   ///< Planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)
	public static final int PIX_FMT_RGB565 = 9;    ///< Packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), in cpu endianness
	public static final int PIX_FMT_RGB555 = 10;    ///< Packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), in cpu endianness most significant bit to 0
	public static final int PIX_FMT_GRAY8 = 11;     ///<        Y        ,  8bpp
	public static final int PIX_FMT_MONOWHITE = 12; ///<        Y        ,  1bpp, 0 is white, 1 is black
	public static final int PIX_FMT_MONOBLACK = 13; ///<        Y        ,  1bpp, 0 is black, 1 is white
	public static final int PIX_FMT_PAL8 = 14;      ///< 8 bit with PIX_FMT_RGB32 palette
	public static final int PIX_FMT_YUVJ420P = 15;  ///< Planar YUV 4:2:0, 12bpp, full scale (jpeg)
	public static final int PIX_FMT_YUVJ422P = 16;  ///< Planar YUV 4:2:2, 16bpp, full scale (jpeg)
	public static final int PIX_FMT_YUVJ444P = 17;  ///< Planar YUV 4:4:4, 24bpp, full scale (jpeg)
	public static final int PIX_FMT_XVMC_MPEG2_MC = 18;///< XVideo Motion Acceleration via common packet passing(xvmc_render.h)
	public static final int PIX_FMT_XVMC_MPEG2_IDCT = 19;
	public static final int PIX_FMT_UYVY422 = 20;   ///< Packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1
	public static final int PIX_FMT_UYYVYY411 = 21; ///< Packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3
	public static final int PIX_FMT_BGR32 = 22;     ///< Packed RGB 8:8:8, 32bpp, (msb)8A 8B 8G 8R(lsb), in cpu endianness
	public static final int PIX_FMT_BGR565 = 23;    ///< Packed RGB 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), in cpu endianness
	public static final int PIX_FMT_BGR555 = 24;    ///< Packed RGB 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), in cpu endianness most significant bit to 1
	public static final int PIX_FMT_BGR8 = 25;      ///< Packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)
	public static final int PIX_FMT_BGR4 = 26;      ///< Packed RGB 1:2:1,  4bpp, (msb)1B 2G 1R(lsb)
	public static final int PIX_FMT_BGR4_BYTE = 27; ///< Packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)
	public static final int PIX_FMT_RGB8 = 28;      ///< Packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)
	public static final int PIX_FMT_RGB4 = 29;      ///< Packed RGB 1:2:1,  4bpp, (msb)2R 3G 3B(lsb)
	public static final int PIX_FMT_RGB4_BYTE = 30; ///< Packed RGB 1:2:1,  8bpp, (msb)2R 3G 3B(lsb)
	public static final int PIX_FMT_NV12 = 31;      ///< Planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 for UV
	public static final int PIX_FMT_NV21 = 32;      ///< as above, but U and V bytes are swapped
	public static final int PIX_FMT_RGB32_1 = 33;   ///< Packed RGB 8:8:8, 32bpp, (msb)8R 8G 8B 8A(lsb), in cpu endianness
	public static final int PIX_FMT_BGR32_1 = 34;   ///< Packed RGB 8:8:8, 32bpp, (msb)8B 8G 8R 8A(lsb), in cpu endianness
	public static final int PIX_FMT_GRAY16BE = 35;  ///<        Y        , 16bpp, big-endian
	public static final int PIX_FMT_GRAY16LE = 36;  ///<        Y        , 16bpp, little-endian
	public static final int PIX_FMT_YUV440P = 37;   ///< Planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples)
	public static final int PIX_FMT_YUVJ440P = 38;  ///< Planar YUV 4:4:0 full scale (jpeg)
	public static final int PIX_FMT_YUVA420P = 39;  ///< Planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples)
	public static final int PIX_FMT_NB = 40;        ///< number of pixel formats, DO NOT USE THIS if you want to link with shared libav* because the number of formats might differ between versions
	
	// TODO: other definitions from libavutil/avutil.h
	
// end libavutil/avutil.h
//------------------------------------------------------------------------------------------------------------------------

}
