package net.sf.ffmpeg_java.custom_protocol;

import net.sf.ffmpeg_java.AVFormatLibrary.URLContext;

import com.sun.jna.Pointer;

/**
 * A Java version ffmpeg URLProtocol, bug specific to a single stream.
 * Subclasses should be constructed once for each stream, with the information needed to connect to the
 * stream provided in the constructor.
 * @author Ken Larson
 *
 */
public interface CallbackURLProtocolHandler
{	
	/** Flags may be a combination of: AVFormatLibrary.URL_RDONLY, AVFormatLibrary.URL_WRONLY, AVFormatLibrary.URL_RDWR. */
    int open(URLContext h, String filename, int flags);
	int read(URLContext h, Pointer buf, int size);
	int write(URLContext h, Pointer buf, int size);
	// values for whence
	public static final int SEEK_SET = 0;
	public static final int SEEK_CUR = 1;
	public static final int SEEK_END = 2;
	/** Implementations need to be very careful about returning -1.  Basically, they should support full seek capability,
	 * or not at all, because much code in ffmpeg never checks the return value of a seek. */
	long seek(URLContext h, long pos, int whence);
    int close(URLContext h);
    
}
