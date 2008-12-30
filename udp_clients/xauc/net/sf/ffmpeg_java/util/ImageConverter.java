package net.sf.ffmpeg_java.util;

import net.sf.ffmpeg_java.AVCodecLibrary.AVFrame;

/**
 * Interface encapsulating AVCodec's img_convert function, allowing it to be swapped out
 * with an SWScale implementation.
 * @author Ken Larson
 *
 */
public interface ImageConverter
{
//	public int img_convert(AVPicture dst, int dst_pix_fmt,
//			AVPicture src, int pix_fmt,
//            int width, int height);
// JNA: This is the same exact function, for convenience, since AVFrame is basically AVPicture plus fields on the end.
	public int img_convert(AVFrame dst, int dst_pix_fmt,
		AVFrame src, int pix_fmt,
	    int width, int height);
}
