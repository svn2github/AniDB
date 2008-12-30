package net.sf.ffmpeg_java.util;

import net.sf.ffmpeg_java.AVCodecLibrary;
import net.sf.ffmpeg_java.AVCodecLibrary.AVFrame;

/**
 * 
 * @author Ken Larson
 *
 */
public class AVCodecLibraryImageConverter implements ImageConverter
{
	private final AVCodecLibrary AVCODEC;
	
	public AVCodecLibraryImageConverter()
	{
		super();
		AVCODEC = AVCodecLibrary.INSTANCE;
	}

	public int img_convert(AVFrame dst, int dst_pix_fmt, AVFrame src, int pix_fmt, int width, int height)
	{
		return  AVCODEC.img_convert(dst, dst_pix_fmt, src, pix_fmt, width, height);
	}

}
