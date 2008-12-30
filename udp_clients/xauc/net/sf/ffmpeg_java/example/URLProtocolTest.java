package net.sf.ffmpeg_java.example;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import net.sf.ffmpeg_java.AVCodecLibrary;
import net.sf.ffmpeg_java.AVFormatLibrary;
import net.sf.ffmpeg_java.AVUtilLibrary;
import net.sf.ffmpeg_java.AVCodecLibrary.AVCodec;
import net.sf.ffmpeg_java.AVCodecLibrary.AVCodecContext;
import net.sf.ffmpeg_java.AVCodecLibrary.AVFrame;
import net.sf.ffmpeg_java.AVFormatLibrary.AVFormatContext;
import net.sf.ffmpeg_java.AVFormatLibrary.AVPacket;
import net.sf.ffmpeg_java.AVFormatLibrary.AVStream;
import net.sf.ffmpeg_java.custom_protocol.CallbackURLProtocolMgr;
import net.sf.ffmpeg_java.custom_protocol.FileCallbackURLProtocolHandler;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

/**
 * Demonstrates ffmpeg getting data from a Java callback, rather than a file directly, using CallbackURLProtocolMgr.
 * @author Ken Larson
 *
 */
public class URLProtocolTest
{
	public static void main(String[] args) throws Exception
	{
		if (args.length < 1)
			throw new RuntimeException("First argument must be path to movie file");
		
		final String url = args[0];
		if (!url.startsWith("file://"))
			throw new RuntimeException("Expected first argument to start with file://");

		final AVFormatLibrary AVFORMAT = AVFormatLibrary.INSTANCE;
		final AVCodecLibrary AVCODEC = AVCodecLibrary.INSTANCE;
		final AVUtilLibrary AVUTIL = AVUtilLibrary.INSTANCE;
		
		
		// not sure what the consequences of such a mismatch are, but it is worth logging a warning:
		if (AVCODEC.avcodec_version() != AVCodecLibrary.LIBAVCODEC_VERSION_INT)
			System.err.println("ffmpeg-java and ffmpeg versions do not match: avcodec_version=" + AVCODEC.avcodec_version() + " LIBAVCODEC_VERSION_INT=" + AVCodecLibrary.LIBAVCODEC_VERSION_INT);

		AVFORMAT.av_register_all();
		
		CallbackURLProtocolMgr.register(AVFORMAT);
		
		final String filePath = url.substring("file://".length());
		final String callbackURL = CallbackURLProtocolMgr.addCallbackURLProtocolHandler(new FileCallbackURLProtocolHandler(new File(filePath)));

		final PointerByReference ppFormatCtx = new PointerByReference();
		
		// Open video file
		if (AVFORMAT.av_open_input_file(ppFormatCtx, callbackURL, null, 0, null) != 0)
		    throw new RuntimeException("Couldn't open file"); // Couldn't open file
		
		final AVFormatContext formatCtx = new AVFormatContext(ppFormatCtx.getValue());
		System.out.println(new String(formatCtx.filename));
			
		// Retrieve stream information
		if (AVFORMAT.av_find_stream_info(formatCtx) < 0)
		    throw new RuntimeException("Couldn't find stream information"); // Couldn't find stream information
	
		AVFORMAT.dump_format(formatCtx, 0, url, 0);
	
		
	    // Find the first video stream
	    int videoStream=-1;
	    for (int i=0; i<formatCtx.nb_streams; i++)
	    {   final AVStream stream = new AVStream(formatCtx.getStreams()[i]);
	    	final AVCodecContext codecCtx = new AVCodecContext(stream.codec);
	    	//System.out.println("codecCtx " + i + ": " + codecCtx);
	    	if (codecCtx.codec_type == AVCodecLibrary.CODEC_TYPE_VIDEO)
	        {
	            videoStream=i;
	            break;
	        }
	    }
	    if (videoStream==-1)
	        throw new RuntimeException("Didn't find a video stream"); // Didn't find a video stream
	    
	    //System.out.println("Video stream index: " + videoStream);
	    
	    // Get a pointer to the codec context for the video stream
	    final Pointer pCodecCtx = new AVStream(formatCtx.getStreams()[videoStream]).codec;
	    final AVCodecContext codecCtx = new AVCodecContext(pCodecCtx);
	    
	    //System.out.println("Codec id: " + codecCtx.codec_id);
	    
	    if (codecCtx.codec_id == 0)
	    	throw new RuntimeException("Codec id is zero (no codec)");
	    
	    // Find the decoder for the video stream
	    final AVCodec codec = AVCODEC.avcodec_find_decoder(codecCtx.codec_id);
	    if (codec == null)
	        throw new RuntimeException("Codec not found for codec_id " + codecCtx.codec_id); // Codec not found
	    
	    // Open codec
	    if (AVCODEC.avcodec_open(codecCtx, codec) < 0)
	    	 throw new RuntimeException("Could not open codec"); // Could not open codec
	    
	    // Allocate video frame
	    final AVFrame frame = AVCODEC.avcodec_alloc_frame();
	    if (frame == null)
	    	throw new RuntimeException("Could not allocate frame");
	     
	    // Allocate an AVFrame structure
	    final AVFrame frameRGB = AVCODEC.avcodec_alloc_frame();
	    if (frameRGB == null)
	    	throw new RuntimeException("Could not allocate frame");
	    
	    // Determine required buffer size and allocate buffer
	    final int numBytes = AVCODEC.avpicture_get_size(AVCodecLibrary.PIX_FMT_RGB24, codecCtx.width, codecCtx.height);
	    final Pointer buffer = AVUTIL.av_malloc(numBytes);
	    
	    // Assign appropriate parts of buffer to image planes in pFrameRGB
	    AVCODEC.avpicture_fill(frameRGB, buffer, AVCodecLibrary.PIX_FMT_RGB24, codecCtx.width, codecCtx.height);

	    // Read frames and save first five frames to disk
	    int i=0;
	    final AVPacket packet = new AVPacket();
	    while (AVFORMAT.av_read_frame(formatCtx, packet) >= 0)
	    {
	    	
	    	// Is this a packet from the video stream?
	        if (packet.stream_index == videoStream)
	        {
	        	final IntByReference frameFinished = new IntByReference();
	            // Decode video frame
	        	AVCODEC.avcodec_decode_video(codecCtx, frame, frameFinished, packet.data, packet.size);

	            // Did we get a video frame?
	            if (frameFinished.getValue() != 0)
	            {
	                // Convert the image from its native format to RGB
	                AVCODEC.img_convert(frameRGB, AVCodecLibrary.PIX_FMT_RGB24, 
	                    frame, codecCtx.pix_fmt, codecCtx.width, 
	                    codecCtx.height);

	                // Save the frame to disk
	                if(++i<=5)
	                    SaveFrame(frameRGB, codecCtx.width, codecCtx.height, 
	                        i);
	            }
	        }

	        // Free the packet that was allocated by av_read_frame
	        // AVFORMAT.av_free_packet(packet.getPointer()) - cannot be called because it is an inlined function.
	        // so we'll just do the JNA equivalent of the inline:
	        if (packet.destruct != null)
	        	packet.destruct.callback(packet);

	    }

	    // Free the RGB image
	    AVUTIL.av_free(frameRGB.getPointer());

	    // Free the YUV frame
	    AVUTIL.av_free(frame.getPointer());

	    // Close the codec
	    AVCODEC.avcodec_close(codecCtx);

	    // Close the video file
	    AVFORMAT.av_close_input_file(formatCtx);
	    
		System.out.println("Done");
	
	}
	
	static void SaveFrame(AVFrame frame, int width, int height, int iFrame) throws IOException
	{
		
	    
		final String filename = "frame" + iFrame + ".ppm";
		System.out.println("Writing: " + filename);
	    final FileOutputStream os = new FileOutputStream(filename);
	    os.write(("P6\n" + width + " " + height + "\n255\n").getBytes());

	    final int len = height * frame.linesize[0];
		final byte[] data = frame.data0.getByteArray(0, len);
		//System.out.println("got data");
	    
	    // Write pixel data
	    for (int y=0; y<height; y++)
	    	os.write(data, y * frame.linesize[0], width * 3);
	        
	    // Close file
	    os.close();
	    System.out.println("Wrote: " + filename);
	}
}
