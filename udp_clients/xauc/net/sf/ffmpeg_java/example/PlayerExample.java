package net.sf.ffmpeg_java.example;

import java.util.ArrayList;

import net.sf.ffmpeg_java.AVCodecLibrary;
import net.sf.ffmpeg_java.AVFormatLibrary;
import net.sf.ffmpeg_java.AVUtilLibrary;
import net.sf.ffmpeg_java.AVCodecLibrary.AVCodec;
import net.sf.ffmpeg_java.AVCodecLibrary.AVCodecContext;
import net.sf.ffmpeg_java.AVCodecLibrary.AVFrame;
import net.sf.ffmpeg_java.AVFormatLibrary.AVFormatContext;
import net.sf.ffmpeg_java.AVFormatLibrary.AVInputFormat;
import net.sf.ffmpeg_java.AVFormatLibrary.AVOutputFormat;
import net.sf.ffmpeg_java.AVFormatLibrary.AVPacket;
import net.sf.ffmpeg_java.AVFormatLibrary.AVStream;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

import avparsing.AVStreamData;
import avparsing.AVparser;

/**
 * Based on AVCodecSample, but shows the movie in a window.
 * TODO: audio
 * TODO: timing
 * @author Ken Larson
 *
 */
public class PlayerExample
{
	public static void main(String[] args) throws Exception
	{
		if (args.length < 1)
			throw new RuntimeException("First argument must be path to movie file");
		
		final String filename = args[0];

		final AVFormatLibrary AVFORMAT = AVFormatLibrary.INSTANCE;
		final AVCodecLibrary AVCODEC = AVCodecLibrary.INSTANCE;
		final AVUtilLibrary AVUTIL = AVUtilLibrary.INSTANCE;
		
		
		// not sure what the consequences of such a mismatch are, but it is worth logging a warning:
		if (AVCODEC.avcodec_version() != AVCodecLibrary.LIBAVCODEC_VERSION_INT)
			System.err.println("ffmpeg-java and ffmpeg versions do not match: avcodec_version=" + AVCODEC.avcodec_version() + " LIBAVCODEC_VERSION_INT=" + AVCodecLibrary.LIBAVCODEC_VERSION_INT);

		AVFORMAT.av_register_all();
		

		final PointerByReference ppFormatCtx = new PointerByReference();
		
		// Open video file
		if (AVFORMAT.av_open_input_file(ppFormatCtx, filename, null, 0, null) != 0)
		    throw new RuntimeException("Couldn't open file"); // Couldn't open file
		
		final AVFormatContext formatCtx = new AVFormatContext(ppFormatCtx.getValue());

		// Retrieve stream information
		if (AVFORMAT.av_find_stream_info(formatCtx) < 0)
		    throw new RuntimeException("Couldn't find stream information"); // Couldn't find stream information

		//AVFORMAT.dump_format(formatCtx, 0, filename, 0);
		
		AVparser avparser = new AVparser();
		
		// Okay, let's do this by steps
		long timeMillis = formatCtx.duration;
		float durinsecs = (float)timeMillis / 1000000;
		
		AVInputFormat format = new AVInputFormat(formatCtx.iformat);
		// Duration
		System.out.println("Duration: "+format.name+" "+avparser.formatDuration(timeMillis)+" ("+durinsecs+") bitrate: "+(formatCtx.bit_rate > 0 ? formatCtx.bit_rate/1000 + " kb/s" : "N/A")+"\n");

		// Okay, now get stream data
		Pointer[] streams = formatCtx.getStreams();
		int streamId = 1;
		for (int i = 0; i < formatCtx.nb_streams; i++) {
			AVStream stream = new AVStream(streams[i]);
			AVCodecContext codecCtx = new AVCodecContext(stream.codec);
			AVCodec codec = AVCODEC.avcodec_find_decoder(codecCtx.codec_id);
			if (codecCtx.codec_type != AVCodecLibrary.CODEC_TYPE_VIDEO &&
					codecCtx.codec_type != AVCodecLibrary.CODEC_TYPE_AUDIO &&
					codecCtx.codec_type != AVCodecLibrary.CODEC_TYPE_SUBTITLE) continue;

			if (codec == null && codecCtx.codec_type != AVCodecLibrary.CODEC_TYPE_SUBTITLE)
		        System.err.println("Codec not found for codec_id " + codecCtx.codec_id); // Codec not found
		    // Open codec
		    if (codec != null && AVCODEC.avcodec_open(codecCtx, codec) < 0)
		    	 throw new RuntimeException("Could not open codec"); // Could not open codec
		    
			String trackType = avparser.getCodecType(codecCtx.codec_type);
			String lang = avparser.convertByteArray(stream.language);
			String codecName = (codec != null ? codec.name : "");
			if (codec == null && codecCtx.codec_type == AVCodecLibrary.CODEC_TYPE_SUBTITLE) // try one more thing
				codecName = avparser.mapSubtitleCodec(codecCtx.codec_id);
			System.out.println("Track #"+streamId+" ["+trackType+"]:");
			System.out.println("\tcodec: "+codecName+(codec != null && codecCtx.codec_tag != 0? " ["+avparser.getCodecTag(codecCtx.codec_tag)+"]" : ""));
			System.out.println("\ttimebase: "+stream.time_base.toDouble());
			if (codecCtx.codec_type == AVCodecLibrary.CODEC_TYPE_VIDEO) {
		        if(stream.r_frame_rate.den != 0 && stream.r_frame_rate.num != 0)
		        	System.out.printf("\tfps: %5.3f tb(r)\n", stream.r_frame_rate.av_q2d());
		        else
		        	System.out.printf("\tfps: %5.3f tb(c)\n", 1/codecCtx.time_base.av_q2d());
		        System.out.println("\tpicture format: "+AVCODEC.avcodec_get_pix_fmt_name(codecCtx.pix_fmt));
				if (codecCtx.height > 0 && codecCtx.width > 0) System.out.println("\tresolution: "+codecCtx.width+"x"+codecCtx.height);
			}
			if (codecCtx.codec_type == AVCodecLibrary.CODEC_TYPE_AUDIO) {
				if (codecCtx.channels > 0) System.out.println("\tchannels: "+avparser.mapAudioChannels(codecCtx.channel_layout));
				if (codecCtx.sample_rate > 0) System.out.println("\tsample rate: "+codecCtx.sample_rate+" Hz");
				System.out.println("\tsample format: "+avparser.getSampleFormat(codecCtx.sample_fmt));
			}
			if (codecCtx.bit_rate > 0) System.out.println("\tbitrate: "+(codecCtx.bit_rate/1000)+" kb/s");
			if (lang != "") System.out.println("\tlanguage: "+lang);
			if (stream.duration > 0) {
				double dursecs = stream.duration*stream.time_base.toDouble();
				System.out.println("\tduration: "+avparser.formatDurationSecs(dursecs)+" ("+dursecs+")");
			}
			if (stream.nb_frames > 0) System.out.println("\tnumber of frames: "+stream.nb_frames);
			streamId++;
			if (codec != null) AVCODEC.avcodec_close(codecCtx);
		}
		
		// Parser

		final AVPacket packet = new AVPacket();
		AVStreamData[] data = new AVStreamData[formatCtx.nb_streams];
		long totalSize = formatCtx.file_size;
		long parsedSize = 0;
	    while (AVFORMAT.av_read_frame(formatCtx, packet) >= 0) {
	    	if (data[packet.stream_index] == null) data[packet.stream_index] = new AVStreamData();
	    	data[packet.stream_index].size += packet.size;
	    	data[packet.stream_index].duration += packet.duration;
	    	parsedSize += packet.size;
	    	//System.out.print("parsing: "+((int)((parsedSize/totalSize)*100))+"%\r");
	    }
	    
	    for (int i = 0; i < formatCtx.nb_streams; i++) {
	    	if (data[i] == null) continue;
	    	if (data[i].duration > 0)
	    		data[i].bitrate = data[i].size * 8 / data[i].duration;
	    	System.out.println("Stream "+i+": size: "+data[i].size+" | duration: "+data[i].duration+" | bitrate: "+data[i].bitrate+" kb/s");
	    }

	    // Close the video file
	    AVFORMAT.av_close_input_file(formatCtx);
	}
}
