package net.sf.ffmpeg_java;

import net.sf.ffmpeg_java.FFMPEGLibrary.AVRational;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.PointerByReference;

/**
 * Pure java implentation of some functions from {@link AVCodecLibrary}, as well as functions to deal with
 * JNA's inability to return structures..
 * @author	Uwe Mannl
 * @author Ken Larson
 *
 */
public class AVCodecLibraryWorkarounds
{

	public static AVRational av_get_q(Pointer obj, String name, PointerByReference o_out)
	{
		return new AVRational(AVCodecLibrary.INSTANCE.av_get_q(obj, name, o_out));
	}
}
