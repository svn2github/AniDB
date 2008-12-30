package net.sf.ffmpeg_java.custom_protocol;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;

import net.sf.ffmpeg_java.AVFormatLibrary;
import net.sf.ffmpeg_java.AVFormatLibrary.URLContext;

import com.sun.jna.Pointer;

/**
 * Implements CallbackURLProtocolHandler by using standard Java file I/O.
 * @author Ken Larson
 *
 */
public class FileCallbackURLProtocolHandler implements CallbackURLProtocolHandler
{
	// TODO: use logging instead of std error.
	private final File file;
	
	private RandomAccessFile f;
	
	public static final boolean TRACE = false;
	
	
	public FileCallbackURLProtocolHandler(File file)
	{
		super();
		this.file = file;
	}

	public int open(URLContext h, String filename, int flags)
	{
		if (TRACE) System.out.print("open: flags=" + flags);
		
		final String mode;
		if ((flags & AVFormatLibrary.URL_RDWR) != 0)
			mode = "rw";	// TODO: ffmpeg impl uses create and truncate flags too
		else if ((flags & AVFormatLibrary.URL_WRONLY) != 0)
			mode = "w";		// TODO: ffmpeg impl uses create and truncate flags too
		else
			mode = "r";
		
		if (TRACE) System.out.println(" (mode=" + mode + ") ");
		
		try
		{
			f = new RandomAccessFile(file, mode);
		} catch (FileNotFoundException e)
		{
			if (TRACE) System.out.println(" return " + -1);
			e.printStackTrace();
			return -1;
		}
		
		if (TRACE) System.out.println(" return " + 0);
		return 0;
	}

	public int read(URLContext h, Pointer buf, int size)
	{
		if (TRACE) System.out.print("read: size=" + size);
		
		// TODO: is there a way to do this without having a copy of the byte array?
		final byte[] ba = new byte[size]; //buf.getByteArray(0, size);
		
		try
		{
			final int ret = f.read(ba, 0, size);
			buf.write(0, ba, 0, size);
			
			if (TRACE) System.out.println(" return " + ret);
			return ret;
		} catch (IOException e)
		{
			if (TRACE) System.out.println(" return " + -1);
			e.printStackTrace();
			return -1;
		}

	}
	
	public int write(URLContext h, Pointer buf, int size)
	{
		if (TRACE) System.out.print("write: size=" + size);
		final byte[] ba = buf.getByteArray(0, size);
		
		try
		{
			f.write(ba, 0, size);
			if (TRACE) System.out.println(" return " + size);
			return size;
		} catch (IOException e)
		{
			if (TRACE) System.out.println(" return " + -1);
			e.printStackTrace();
			return -1;
		}
	}

	public long seek(URLContext h, long pos, int whence)
	{
		if (TRACE) System.out.print("seek: pos=" + pos + " whence=" + whence);
		try
		{
			final long seekTo;
			if (whence == SEEK_SET)
				seekTo = pos;
			else if (whence == SEEK_CUR)
				seekTo = f.getFilePointer() + pos;
			else if (whence == SEEK_END)
				seekTo = f.length() + pos;
			else if (whence == AVFormatLibrary.AVSEEK_SIZE)
			{
				final long ret = f.length();
				if (TRACE) System.out.println(" return " + ret);
				return ret;
			}
			else
			{	if (TRACE) System.out.println(" return " + -1);
				System.err.println("seek: Invalid whence value: " + whence);
				return -1;
			}
			
			f.seek(seekTo);
			if (TRACE) System.out.println(" return " + seekTo);
			return seekTo;
		}
		catch (IOException e)
		{
			if (TRACE) System.out.println(" return " + -1);
			e.printStackTrace();
			
			return -1;
		}

	}
	
	public int close(URLContext h)
	{
		if (TRACE) System.out.print("close");
		try
		{
			f.close();
		} catch (IOException e)
		{
			if (TRACE) System.out.println(" return " + -1);
			e.printStackTrace();
			return -1;
		}
		
		if (TRACE) System.out.println(" return " + 0);
		return 0;
	}



}
