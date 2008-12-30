package net.sf.ffmpeg_java.util;

import java.awt.image.BufferedImage;

/**
 * In progress
 * @author Ken Larson
 *
 */
public class FrameDataToImage
{

	/**
	 * Assumes frame data from ffmpeg with format AVCodecLibrary.PIX_FMT_RGB24
	 */
	public static BufferedImage createImage(byte[] bytes, int w, int h)
	{	
		final int redMask = 1;
		final int greenMask = 2;
		final int blueMask = 3;
		final int lineStride = w * 3; 
		final int pixelStride = 3;
		final boolean flipped = false;
			
		// TODO: would it be quicker to use a type that took a byte array directly?

		final BufferedImage bi = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
		final int [] pixels = new int[w * h];
		int pixelIndex = 0;
		int lineOffset = 0;
		if (flipped)
			lineOffset = (h - 1) * lineStride;
		
		for (int y = 0; y < h; ++y)
		{
			int off = lineOffset;
			for (int x = 0; x < w; ++x)
			{
				final byte r = bytes[off + redMask - 1];
				final byte g = bytes[off + greenMask - 1];
				final byte b = bytes[off + blueMask - 1];
				int pixel = 0;
				pixel += r & 0xff;	// red
				pixel *= 256;
				pixel += g & 0xff; // green
				pixel *= 256;
				pixel += b & 0xff;	// blue
				pixels[pixelIndex++] = pixel;
				off += pixelStride;
			}
			if (flipped)
				lineOffset -= lineStride;
			else
				lineOffset += lineStride;
		}
		
		bi.setRGB(0,0,w,h,pixels,0,w);
		return bi;
		
	}
	

}
