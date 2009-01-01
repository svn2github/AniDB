package avparsing;

import java.io.File;

import net.sf.ffmpeg_java.AVCodecLibrary;
import net.sf.ffmpeg_java.AVFormatLibrary;
import net.sf.ffmpeg_java.AVCodecLibrary.AVCodec;
import net.sf.ffmpeg_java.AVCodecLibrary.AVCodecContext;
import net.sf.ffmpeg_java.AVFormatLibrary.AVFormatContext;
import net.sf.ffmpeg_java.AVFormatLibrary.AVInputFormat;
import net.sf.ffmpeg_java.AVFormatLibrary.AVPacket;
import net.sf.ffmpeg_java.AVFormatLibrary.AVStream;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.PointerByReference;

import utils.Log;
import utils.Progress;

/**
 * Audio/Video file parser
 */
public class AVparser implements Runnable {
	protected File file;
	protected Log log;
	protected Progress progress;
	protected boolean showDebug = false;
	protected boolean parseFile = false;
	protected String errorMessage = "";

	// Public fields
	public String filename;
	public String formatedDuration = "";
	public float duration = 0;
	public float bitrate = 0;
	public String format = "";
	public int numStreams = 0;
	public AVStreamData[] streams = null;
	
	public AVparser() {
		this.log = new Log();
		this.progress = new Progress();
	}
	public AVparser(File file) {
		this();
		this.filename = file.getAbsolutePath();
		this.file = file;
	}
	public AVparser(String filename) { this(new File(filename)); }
	public AVparser(File file, AVParserOptions avparserOptions) {
		this(file);
		this.parseFile = avparserOptions.isFullParse();
		this.showDebug = avparserOptions.isSeeDebug();
	}
	
	/** @return the log */
	public synchronized Log getLog() { return log; }
	/** @param log the log to set */
	public synchronized void setLog(Log log) { this.log = log; }
	/** @return the progress */
	public synchronized Progress getProgress() { return progress; }
	/** @param progress the progress to set */
	public synchronized void setProgress(Progress progress) { this.progress = progress; }
	/** @return the showDebug */
	public synchronized boolean isShowDebug() { return showDebug; }
	/** @param showDebug the showDebug to set */
	public synchronized void setShowDebug(boolean showDebug) { this.showDebug = showDebug; }
	/** @return the parseFile */
	public synchronized boolean isParseFile() { return parseFile; }
	/** @param parseFile the parseFile to set */
	public synchronized void setParseFile(boolean parseFile) { this.parseFile = parseFile; }
	/** @return the errorMessage */
	public synchronized String getErrorMessage() { return errorMessage;	}

	/**
	 * Gets the codec type
	 * @param codecType AVCodecContext codec_type
	 * @return Codec type
	 */
	public String getCodecType(int codecType) {
		String trackType = "unknown";
		switch(codecType) {
			case AVCodecLibrary.CODEC_TYPE_VIDEO: trackType = "video"; break;
			case AVCodecLibrary.CODEC_TYPE_AUDIO: trackType = "audio"; break;
			case AVCodecLibrary.CODEC_TYPE_SUBTITLE: trackType = "subtitle"; break;
			case AVCodecLibrary.CODEC_TYPE_DATA: trackType = "data"; break;
			case AVCodecLibrary.CODEC_TYPE_ATTACHMENT: trackType = "attachement"; break;
			default: trackType = "unknown";
		}
		return trackType;
	}
	
	/**
	 * Formats a duration time in seconds to an hh:mm:ss representation
	 * @param time The time
	 * @return Formated time
	 */
	public String formatDurationSecs(double time) {
		String seconds = Integer.toString((int)(time % 60));
		String minutes = Integer.toString((int)((time % 3600) / 60));
		String hours = Integer.toString((int)(time / 3600));
		for (int i = 0; i < 2; i++) {
			if (seconds.length() < 2) seconds = "0" + seconds;
			if (minutes.length() < 2) minutes = "0" + minutes;
			if (hours.length() < 2) hours = "0" + hours;
		}
		return hours+":"+minutes+":"+seconds;
	}
	
	/**
	 * Formats a duration time in nanoseconds to an hh:mm:ss representation
	 * @param timeMillis The time
	 * @return Formated time
	 */
	public String formatDuration(long timeMillis) {
		long time = timeMillis / 1000000;
		return formatDurationSecs(time);
	}
	
	/**
	 * Converts a Byte Array to a String
	 * @param array The Byte Array to convert
	 * @return Converted String
	 */
	public String convertByteArray(byte[] array) {
		String string = "";
		for (int i = 0; i < array.length; i++) if ((int) array[i] != 0) string += (char)array[i];
		return string;
	}
	
	/**
	 * Because AVCodec doesn't have codecs for Subtitles, mapping is done here
	 * @param id CODEC_ID for unknown types
	 * @return A string with the codec name if known
	 */
	public String mapSubtitleCodec(int id) {
		switch (id) {
			case AVCodecLibrary.CODEC_ID_DVD_SUBTITLE: return "dvd subtitle";
			case AVCodecLibrary.CODEC_ID_DVB_SUBTITLE: return "dvb subtitle";
			case AVCodecLibrary.CODEC_ID_TEXT: return "text";
			case AVCodecLibrary.CODEC_ID_XSUB: return "xsub";
			case AVCodecLibrary.CODEC_ID_SSA: return "ssa";
			case AVCodecLibrary.CODEC_ID_MOV_TEXT: return "mov text";
			case AVCodecLibrary.CODEC_ID_TTF: return "ttf";
			default: return "unknown";
		}
	}
	
	/**
	 * Audio Channels layout
	 * @param layout Audio Channels Layout Id (CH_LAYOUT_*)
	 * @return A string with the channel layout if known
	 */
	public String mapAudioChannels(long layout) {
		if (layout == AVCodecLibrary.CH_LAYOUT_MONO) return "monoraul";
		else if (layout == AVCodecLibrary.CH_LAYOUT_STEREO) return "stereo";
		else if (layout == AVCodecLibrary.CH_LAYOUT_SURROUND) return "surround";
		else if (layout == AVCodecLibrary.CH_LAYOUT_QUAD) return "quadraphonic";
		else if (layout == AVCodecLibrary.CH_LAYOUT_5POINT0) return "5.0";
		else if (layout == AVCodecLibrary.CH_LAYOUT_5POINT1) return "5.1";
		else if (layout == (AVCodecLibrary.CH_LAYOUT_5POINT1&AVCodecLibrary.CH_LAYOUT_STEREO_DOWNMIX)) return "5.1 (downmixed)";
		else if (layout == AVCodecLibrary.CH_LAYOUT_7POINT1) return "7.1";
		else if (layout == (AVCodecLibrary.CH_LAYOUT_7POINT1&AVCodecLibrary.CH_LAYOUT_STEREO_DOWNMIX)) return "7.1 (downmixed)";
		else if (layout == AVCodecLibrary.CH_LAYOUT_7POINT1_WIDE) return "7.1 (wide)";
		else if (layout == AVCodecLibrary.CH_LAYOUT_STEREO_DOWNMIX) return "stereo downmix";
		else return "unknown";
	}
	
	/**
	 * Gets a string representation of a codec tag
	 * @param codec_tag A codecCtx.codec_tag reference
	 * @return A codec tag
	 */
	public String getCodecTag(int codec_tag) {
		String name = "";
		if (Character.isLetterOrDigit(codec_tag&0xFF) && Character.isLetterOrDigit((codec_tag>>8)&0xFF) &&
				Character.isLetterOrDigit((codec_tag>>16)&0xFF) && Character.isLetterOrDigit((codec_tag>>24)&0xFF)) {
			name = String.format("%c%c%c%c", (codec_tag&0xFF),((codec_tag>>8)&0xFF),((codec_tag>>16)&0xFF),((codec_tag>>24)&0xFF));
		} else
			name = String.format("0x%04x", codec_tag); 
		return name;
	}
	
	/**
	 * Gets an audio sample format
	 * @param sample_fmt A codecCtx.sample_fmt reference
	 * @return String with sample format
	 */
	public String getSampleFormat(int sample_fmt) {
		switch (sample_fmt) {
			case AVCodecLibrary.SAMPLE_FMT_NONE: return "none";
			case AVCodecLibrary.SAMPLE_FMT_U8: return "u8";
			case AVCodecLibrary.SAMPLE_FMT_S16: return "s16";
			case AVCodecLibrary.SAMPLE_FMT_S24: return "s24";
			case AVCodecLibrary.SAMPLE_FMT_S32: return "s32";
			case AVCodecLibrary.SAMPLE_FMT_FLT: return "flt";
			case AVCodecLibrary.SAMPLE_FMT_DBL: return "dbl";
			default: return "";
		}
	}
	
	/**
	 * Method that parses a file
	 * @return 0 if operation is sucessful
	 */
	public synchronized void run() {
		final AVFormatLibrary AVFORMAT = AVFormatLibrary.INSTANCE;
		final AVCodecLibrary AVCODEC = AVCodecLibrary.INSTANCE;

		// not sure what the consequences of such a mismatch are, but it is worth logging a warning:
		if (showDebug && AVCODEC.avcodec_version() != AVCodecLibrary.LIBAVCODEC_VERSION_INT)
			System.err.println("ffmpeg-java and ffmpeg versions do not match: avcodec_version=" + AVCODEC.avcodec_version() + " LIBAVCODEC_VERSION_INT=" + AVCodecLibrary.LIBAVCODEC_VERSION_INT);
		
		// register formats
		AVFORMAT.av_register_all();
		final PointerByReference ppFormatCtx = new PointerByReference();
		
		// Open video file
		if (AVFORMAT.av_open_input_file(ppFormatCtx, filename, null, 0, null) != 0) {
			this.errorMessage = "couldn't open file (avparser.run)";
			return;
		}
		
		// Get pointer
		final AVFormatContext formatCtx = new AVFormatContext(ppFormatCtx.getValue());
			
		// Retrieve stream information
		if (AVFORMAT.av_find_stream_info(formatCtx) < 0) {
			this.errorMessage = "couldn't find stream information (avparser.run)";
		    return;
		}
		
		AVInputFormat format = new AVInputFormat(formatCtx.iformat);

		// Okay, let's do this by steps
		// Basic fields
		this.duration = (float)formatCtx.duration / 1000000;
		this.formatedDuration = formatDuration(formatCtx.duration);
		this.bitrate = (formatCtx.bit_rate > 0 ? formatCtx.bit_rate/1000 : 0);
		this.format = format.name;
		this.numStreams = formatCtx.nb_streams;
		
		//AVFORMAT.dump_format(formatCtx, 0, filename, 0);
		
		// Now get stream data
		Pointer[] streams = formatCtx.getStreams();
		this.streams = new AVStreamData[formatCtx.nb_streams];
		int streamId = 1;
		for (int i = 0; i < formatCtx.nb_streams; i++) {
			AVStream stream = new AVStream(streams[i]);
			AVCodecContext codecCtx = new AVCodecContext(stream.codec);
			AVCodec codec = null;
			// now handle streams
			switch (codecCtx.codec_type) {
				case AVCodecLibrary.CODEC_TYPE_VIDEO:
					codec = AVCODEC.avcodec_find_decoder(codecCtx.codec_id);
					if (codec == null)						
						log.println("codec not found for codec_id " + codecCtx.codec_id + " (avparser.run)");
					if (codec != null && AVCODEC.avcodec_open(codecCtx, codec) < 0)
						log.println("could not open codec (avparser.run)");
					AVStreamDataVideo vidstream = new AVStreamDataVideo();
					vidstream.timebase = stream.time_base.toDouble();
					vidstream.type = codecCtx.codec_type;
					vidstream.typeName = getCodecType(codecCtx.codec_type);
					vidstream.streamId = streamId;
					vidstream.codecName = (codec != null ? codec.name : "");
					vidstream.codecTag = (codecCtx.codec_tag != 0 ? getCodecTag(codecCtx.codec_tag) : "");
					// Video specific
					if(stream.r_frame_rate.den != 0 && stream.r_frame_rate.num != 0)
						vidstream.fps = stream.r_frame_rate.av_q2d();
			        else
			        	vidstream.fps = 1/codecCtx.time_base.av_q2d();
					vidstream.width = codecCtx.width;
					vidstream.height = codecCtx.height;
					vidstream.cwidth = codecCtx.coded_width;
					vidstream.cheight = codecCtx.coded_height;
					vidstream.resolution = vidstream.width + "x" + vidstream.height;
					vidstream.cresolution = vidstream.cwidth + "x" + vidstream.cheight;
					vidstream.ar = (double)codecCtx.width / (double)codecCtx.height;
					vidstream.car = (double)codecCtx.coded_width / (double)codecCtx.coded_height;
					vidstream.pictureFormat = AVCODEC.avcodec_get_pix_fmt_name(codecCtx.pix_fmt);
					if (codecCtx.bit_rate > 0) vidstream.bitrate = codecCtx.bit_rate;
					this.streams[i] = vidstream;
					streamId++;
					break;
				case AVCodecLibrary.CODEC_TYPE_AUDIO:
					codec = AVCODEC.avcodec_find_decoder(codecCtx.codec_id);
					if (codec == null)						
						log.println("codec not found for codec_id " + codecCtx.codec_id + " (avparser.run)");
					if (codec != null && AVCODEC.avcodec_open(codecCtx, codec) < 0)
						log.println("could not open codec (avparser.run)");
					AVStreamDataAudio audstream = new AVStreamDataAudio();
					audstream.timebase = stream.time_base.toDouble();
					audstream.type = codecCtx.codec_type;
					audstream.typeName = getCodecType(codecCtx.codec_type);
					audstream.streamId = streamId;
					audstream.codecName = (codec != null ? codec.name : "");
					audstream.codecTag = (codecCtx.codec_tag != 0 ? getCodecTag(codecCtx.codec_tag) : "");
					audstream.language = convertByteArray(stream.language);
					audstream.channels = codecCtx.channels;
					audstream.layout = mapAudioChannels(codecCtx.channel_layout);
					audstream.sampleFormat = getSampleFormat(codecCtx.sample_fmt);
					audstream.sampleRate = codecCtx.sample_rate;
					if (codecCtx.bit_rate > 0) audstream.bitrate = codecCtx.bit_rate;
					this.streams[i] = audstream;
					streamId++;
					break;
				case AVCodecLibrary.CODEC_TYPE_SUBTITLE:
					AVStreamDataSubtitle substream = new AVStreamDataSubtitle();
					substream.timebase = stream.time_base.toDouble();
					substream.type = codecCtx.codec_type;
					substream.typeName = getCodecType(codecCtx.codec_type);
					substream.streamId = streamId;
					substream.codecName = mapSubtitleCodec(codecCtx.codec_id);
					substream.codecTag = (codecCtx.codec_tag != 0 ? getCodecTag(codecCtx.codec_tag) : "");
					substream.subtitleType = 20; // soft subtitle otherwise we wouldn't find it
					substream.language = convertByteArray(stream.language);
					if (codecCtx.codec_id == AVCodecLibrary.CODEC_ID_TEXT) substream.isUnstyledSubs = true;
					if (codecCtx.codec_id == AVCodecLibrary.CODEC_ID_SSA) substream.isStyledSubs = true;
					if (codecCtx.codec_id == AVCodecLibrary.CODEC_ID_DVD_SUBTITLE) substream.isImageSubs = true;
					this.streams[i] = substream;
					streamId++;
					break;
				default: // other data
					AVStreamData avstream = new AVStreamData();
					avstream.timebase = codecCtx.time_base.toDouble();
					avstream.type = codecCtx.codec_type;
					avstream.typeName = getCodecType(codecCtx.codec_type);
					avstream.codecName = mapSubtitleCodec(codecCtx.codec_id);
					avstream.codecTag = (codecCtx.codec_tag != 0 ? getCodecTag(codecCtx.codec_tag) : "");
					this.streams[i] = avstream;
			}
			if (codec != null) AVCODEC.avcodec_close(codecCtx);
		}

		// this ocasionaly crashes, so it's disabled by default
		if (this.parseFile) {
			final AVPacket packet = new AVPacket();
			long totalSize = formatCtx.file_size;
			long parsedSize = 0;
			float curprogress = 0;
		    while (AVFORMAT.av_read_frame(formatCtx, packet) >= 0) {
		    	if (this.streams[packet.stream_index] == null) this.streams[packet.stream_index] = new AVStreamData();
		    	this.streams[packet.stream_index].size += packet.size;
		    	this.streams[packet.stream_index].duration += packet.duration;
		    	parsedSize += packet.size;
		    	curprogress = (parsedSize/totalSize);
		    	if (progress != null) progress.setProgress(curprogress);
		    	
		    	// Free the packet that was allocated by av_read_frame
		        // AVFORMAT.av_free_packet(packet.getPointer()) - cannot be called because it is an inlined function.
		        // so we'll just do the JNA equivalent of the inline:
		    	if (packet.destruct != null)
		        	packet.destruct.callback(packet);
		    }
		    
		    // fill in computed bitrate for audio and video streams
		    for (int i = 0; i < formatCtx.nb_streams; i++) {
		    	if (this.streams[i] == null) continue;
		    	if (this.streams[i].type != AVCodecLibrary.CODEC_TYPE_VIDEO && this.streams[i].type != AVCodecLibrary.CODEC_TYPE_AUDIO) continue;
		    	if (this.streams[i].duration <= 0) continue; // can't have this
		    	int den = (int)(this.streams[i].duration * this.streams[i].timebase);
		    	this.streams[i].duration = den;
		    	long num = this.streams[i].size * 8;
		    	this.streams[i].bitrate = num / den;
		    	this.streams[i].formatedDuration = formatDurationSecs(this.streams[i].duration); 
		    }
		}
	    
	    AVFORMAT.av_close_input_file(formatCtx);
		return;
	}
}
