package avparsing;

import java.io.File;
import java.io.IOException;
import java.util.Collections;
import java.util.Iterator;

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

import structures.AVStreamData;
import structures.AVStreamDataAudio;
import structures.AVStreamDataSubtitle;
import structures.AVStreamDataVideo;
import structures.AniDBFile;
import utils.Log;
import utils.Progress;
import utils.ThreadedWorker;
import utils.Utils;

/**
 * AVParser implementation using FFmpeg-java to interface with FFmpeg
 * <p><b>Workflow:</b><br>
 * <ol><li>Instantiate class using AVParser parser = new AVParserFFmpegJava(File file); or any of the other constructors</li>
 * <li>Optionally use setters to set class parameters</li>
 * <li>If you wish to directly execute the parsing use work() method</li>
 * <li>If you wish to run this in a new thread, first enable the thread using the setEnabled(true) method and the use the run() method</li></ol>
 * <p><b>Error handling:</b><br>To know if there was some non-critical error, as in no native crash, just use the getErrorMessage() method, if it's a null string, there was no error.
 * <p><b>Notes:</b><br>FFmpeg-java access is not stable enough, specialy if doing a full parse.</p>
 * @author fahrenheit
 */
public class AVParserFFmpegJava extends ThreadedWorker implements AVParser {
	// Implementation related fields
	/** FFmpeg-java AVFORMAT instance */
	protected AVFormatLibrary AVFORMAT = null;
	/** FFmpeg-java AVCODEC instance */
	protected AVCodecLibrary AVCODEC = null;
	/** Stream data (internal usage) */
	protected AVStreamData[] streams = null;
	// Protected class fields
	/** File reference */
	protected File file = null;
	/** AniDBFile for this file */
	protected AniDBFile anidbFile = null;
	/** Log where messages will be written to */
	protected Log log = null;
	/** The current parsing progress will be shown here */
	protected Progress progress = null;
	/** If set to true debug messages will be shown */
	protected boolean showDebug = false;
	/** If set to true a full file parsing will be done */
	protected boolean parseFile = false;
	/** If some error occours during processing its error message will be stored here */
	protected String errorMessage = "";
	/** VBR Calculation mode (will only be used if parseFile is enabled) */
	protected int vbr_calc_mode = VBR_BY_PACKET_BITRATE;
	// Public fields
	/** Parsed filename */
	public String filename = "";
	/** Formated duration of the file in hh:mm:ss format */
	public String formatedDuration = "";
	/** Duration of the file in seconds */
	public float duration = 0;
	/** Bitrate of the file in bps if known or 0 otherwise */
	public float bitrate = 0;
	/** Format of the file (usualy container format) */
	public String format = "";
	/** Number of streams of the file */
	public int numStreams = 0;	
	
	public synchronized File getFile() { return file; }
	public synchronized void setFile(File file) { this.file = file; }
	public synchronized AniDBFile getAnidbFile() { return anidbFile; }
	public synchronized void setAnidbFile(AniDBFile anidbFile) { this.anidbFile = anidbFile; }
	public synchronized Log getLog() { return log; }
	public synchronized void setLog(Log log) { this.log = log; }
	public synchronized Progress getProgress() { return progress; }
	public synchronized void setProgress(Progress progress) { this.progress = progress; }
	public synchronized boolean isShowDebug() { return showDebug; }
	public synchronized void setShowDebug(boolean showDebug) { this.showDebug = showDebug; }
	public synchronized boolean isParseFile() { return parseFile; }
	public synchronized void setParseFile(boolean parseFile) { this.parseFile = parseFile; }
	public synchronized String getErrorMessage() { return errorMessage;	}
	public synchronized int getVbr_calc_mode() { return vbr_calc_mode; }
	public synchronized void setVbr_calc_mode(int vbr_calc_mode) { this.vbr_calc_mode = vbr_calc_mode; }

	/** Gets the libavformat java interface instance used by ffmpeg-java
	 * @return AVFORMAT instance */
	public synchronized AVFormatLibrary getAVFORMAT() { return AVFORMAT; }
	/** Sets the libavformat java interface instance used by ffmpeg-java 
	 * @param avformat the AVFORMAT instance to set */
	public synchronized void setAVFORMAT(AVFormatLibrary avformat) { AVFORMAT = avformat; }
	/** Gets the libavcodec java interface instance used by ffmpeg-java
	 * @return AVCODEC instance */
	public synchronized AVCodecLibrary getAVCODEC() { return AVCODEC; }
	/** Sets the libavcodec java interface instance used by ffmpeg-java 
	 * @param avcodec the AVCODEC instance to set */
	public synchronized void setAVCODEC(AVCodecLibrary avcodec) { AVCODEC = avcodec; }

	
	/**
	 * AVParserFFmpegJava Class constructor
	 */
	protected AVParserFFmpegJava() {
		this.log = new Log();
		this.progress = new Progress();
	}
	/**
	 * AVParserFFmpegJava Class constructor 
	 * @param file File to parse
	 */
	public AVParserFFmpegJava(File file) {
		this();
		try {
			this.filename = file.getCanonicalPath();
		} catch (IOException e) {
			String message = "error while getting canonical path for file \""+file.getAbsolutePath()+"\", reverting to absolute path (avparser)";
			System.err.println(message);
			log.println(message);
			this.filename = file.getAbsolutePath();
		}
		this.file = file;
	}
	/**
	 * AVParserFFmpegJava Class constructor 
	 * @param filename Filename of the file to parse
	 */
	public AVParserFFmpegJava(String filename) { this(new File(filename)); }
	/**
	 * AVParserFFmpegJava Class constructor 
	 * @param file File to parse
	 * @param parsingOptions Parsing Options
	 */
	public AVParserFFmpegJava(File file, AVParserOptions options) {
		this(file);
		this.parseFile = options.isFullParse();
		this.showDebug = options.isSeeDebug();
		this.AVCODEC = options.getAVCODEC();
		this.AVFORMAT = options.getAVFORMAT();
		this.vbr_calc_mode = options.getVbr_calc_mode();
	}
	/**
	 * AVParserFFmpegJava Class constructor 
	 * @param filename Filename of the file to parse
	 * @param parsingOptions Parsing Options
	 */
	public AVParserFFmpegJava(String filename, AVParserOptions options) {
		this(new File(filename),options);
	}

	/**
	 * Gets the codec/stream type
	 * @param codecType AVCodecContext codec_type
	 * @return Codec type
	 */
	public String getCodecType(int codecType) {
		switch(codecType) {
			case AVCodecLibrary.CODEC_TYPE_VIDEO: return "video";
			case AVCodecLibrary.CODEC_TYPE_AUDIO: return "audio";
			case AVCodecLibrary.CODEC_TYPE_SUBTITLE: return "subtitle";
			case AVCodecLibrary.CODEC_TYPE_DATA: return "data";
			case AVCodecLibrary.CODEC_TYPE_ATTACHMENT: return "attachement";
			default: return "unknown";
		}
	}

	/**
	 * Because AVCodec doesn't have codecs for Subtitles, mapping is done here
	 * @param id CODEC_ID for unknown types
	 * @return A string with the codec name if known
	 */
	public String mapSubtitleCodec(int id) {
		switch (id) {
			case AVCodecLibrary.CODEC_ID_DVD_SUBTITLE: return "vobsub/dvd subtitle";
			case AVCodecLibrary.CODEC_ID_DVB_SUBTITLE: return "dvb subtitle";
			case AVCodecLibrary.CODEC_ID_TEXT: return "text/srt/utf8";
			case AVCodecLibrary.CODEC_ID_XSUB: return "xsub";
			case AVCodecLibrary.CODEC_ID_SSA: return "ass/ssa";
			case AVCodecLibrary.CODEC_ID_MOV_TEXT: return "mov text";
			case AVCodecLibrary.CODEC_ID_TTF: return "ttf";
			case AVCodecLibrary.CODEC_ID_BMP: return "bmp";
			case AVCodecLibrary.CODEC_ID_GIF: return "gif";
			case AVCodecLibrary.CODEC_ID_PNG: return "png";
			case AVCodecLibrary.CODEC_ID_JPEG2000: return "jpeg2000";
			case AVCodecLibrary.CODEC_ID_JPEGLS: return "jpegls";
			default: return "other/unknown";
		}
	}

	/**
	 * Audio Channels layout
	 * @param layout Audio Channels Layout Id (CH_LAYOUT_*)
	 * @return A string with the channel layout if known
	 */
	public String mapAudioChannels(int layout) {
		if (Utils.checkBitmask(layout,AVCodecLibrary.CH_LAYOUT_MONO)) return "monoraul";
		else if (Utils.checkBitmask(layout,AVCodecLibrary.CH_LAYOUT_STEREO)) return "stereo";
		else if (Utils.checkBitmask(layout,AVCodecLibrary.CH_LAYOUT_SURROUND)) return "surround";
		else if (Utils.checkBitmask(layout,AVCodecLibrary.CH_LAYOUT_QUAD)) return "quadraphonic";
		else if (Utils.checkBitmask(layout,AVCodecLibrary.CH_LAYOUT_5POINT0)) return "5.0";
		else if (Utils.checkBitmask(layout,AVCodecLibrary.CH_LAYOUT_5POINT1)) return "5.1";
		else if (Utils.checkBitmask(layout,(AVCodecLibrary.CH_LAYOUT_5POINT1|AVCodecLibrary.CH_LAYOUT_STEREO_DOWNMIX))) return "5.1 (downmixed)";
		else if (Utils.checkBitmask(layout,AVCodecLibrary.CH_LAYOUT_7POINT1)) return "7.1";
		else if (Utils.checkBitmask(layout,(AVCodecLibrary.CH_LAYOUT_7POINT1|AVCodecLibrary.CH_LAYOUT_STEREO_DOWNMIX))) return "7.1 (downmixed)";
		else if (Utils.checkBitmask(layout,AVCodecLibrary.CH_LAYOUT_7POINT1_WIDE)) return "7.1 (wide)";
		else if (Utils.checkBitmask(layout,AVCodecLibrary.CH_LAYOUT_STEREO_DOWNMIX)) return "stereo downmix";
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
	
	/** Method that parses a file */
	public synchronized void work() {
		if (AVFORMAT == null) { AVFORMAT = AVFormatLibrary.SYNC_INSTANCE; AVFORMAT.av_register_all(); }
		if (AVCODEC == null) { AVCODEC = AVCodecLibrary.SYNC_INSTANCE; AVCODEC.avcodec_init(); }

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

		if (showDebug) System.out.println("Started parsing of \""+file.getName()+"\" ["+file.length()+" bytes]");
		long start = System.currentTimeMillis();

		// Okay, let's do this by steps
		// Basic fields
		this.duration = (float)formatCtx.duration / 1000000;
		this.formatedDuration = Utils.formatDuration(formatCtx.duration);
		this.bitrate = (formatCtx.bit_rate > 0 ? formatCtx.bit_rate/1000 : 0);
		this.format = format.name;
		this.numStreams = formatCtx.nb_streams;
		if (this.anidbFile != null) {
			this.anidbFile.duration = (float)formatCtx.duration / 1000000;
			this.anidbFile.formatedDuration = Utils.formatDuration(formatCtx.duration);
			this.anidbFile.bitrate = (formatCtx.bit_rate > 0 ? formatCtx.bit_rate/1000 : 0);
			this.anidbFile.format = format.name;
			this.anidbFile.numStreams = formatCtx.nb_streams;
			this.anidbFile.state |= AniDBFile.PARSED;
		}

		//AVFORMAT.dump_format(formatCtx, 0, filename, 0);

		// Now get stream data
		Pointer[] streams = formatCtx.getStreams();
		this.streams = new AVStreamData[formatCtx.nb_streams];
		int streamId = 1;
		int vidStreamIndex = 0;
		int audStreamIndex = 0;
		int subStreamIndex = 0;
		int othStreamIndex = 0;
		// This is equivalent to an header only parse (will not confirm bitrates, durations, vbr and vfr)
		for (int i = 0; i < formatCtx.nb_streams; i++) {
			AVStream stream = new AVStream(streams[i]);
			AVCodecContext codecCtx = new AVCodecContext(stream.codec);
			AVCodec codec = null;
			
			// FULLPARSE support
			this.streams[i] = new AVStreamData();
			this.streams[i].streamId = streamId;
			this.streams[i].timebase = stream.time_base.toDouble();
			this.streams[i].type = codecCtx.codec_type;
			this.streams[i].codec_id = codecCtx.codec_id;
			// HEADERPARSE
			switch (codecCtx.codec_type) {
				case AVCodecLibrary.CODEC_TYPE_VIDEO:
					this.streams[i].relStreamId = vidStreamIndex;
					// ERROR CHECKS
					if (codecCtx.codec_id != 0) codec = AVCODEC.avcodec_find_decoder(codecCtx.codec_id);
					if (codec == null) log.println("video codec not found for codec_id " + codecCtx.codec_id + " (avparser.run)");
					if (codec != null && AVCODEC.avcodec_open(codecCtx, codec) < 0) log.println("could not open video codec (avparser.run)");
					// STREAM PARSING
					AVStreamDataVideo vidstream = new AVStreamDataVideo();
					vidstream.streamId = streamId;
					vidstream.relStreamId = vidStreamIndex;
					vidstream.timebase = stream.time_base.toDouble();
					vidstream.type = codecCtx.codec_type;
					vidstream.codec_id = codecCtx.codec_id;

					vidstream.setAniDBVideoCodec(codecCtx.codec_id, getCodecTag(codecCtx.codec_tag));
					if(stream.r_frame_rate.den != 0 && stream.r_frame_rate.num != 0) vidstream.fps = stream.r_frame_rate.av_q2d();
					else vidstream.fps = 1/codecCtx.time_base.av_q2d();
					vidstream.p_width = codecCtx.width;
					vidstream.p_height = codecCtx.height;
					vidstream.p_resolution = vidstream.getResolution(codecCtx.width, codecCtx.height);
					vidstream.p_ar = (double)codecCtx.width / (double)codecCtx.height;
					if (stream.sample_aspect_ratio.toDouble() != 0) {
						vidstream.d_ar = stream.sample_aspect_ratio.toDouble() * vidstream.p_ar;
						if (!String.format("%1.3f", vidstream.d_ar).equals(String.format("%1.3f", vidstream.p_ar))) {
							vidstream.isAnamorphic = true;
							vidstream.d_width = (int)((double)vidstream.p_height*vidstream.d_ar);
							vidstream.d_height = vidstream.p_height;
							vidstream.d_resolution = vidstream.getResolution(vidstream.d_width, vidstream.d_height);
						} else {
							vidstream.d_ar = 0;
							vidstream.d_height = -1;
							vidstream.d_width = -1;
							vidstream.d_resolution = "";
						}
					}
					vidstream.pictureFormat = AVCODEC.avcodec_get_pix_fmt_name(codecCtx.pix_fmt);
					if (codecCtx.bit_rate > 0) vidstream.bitrate = codecCtx.bit_rate;
					if (this.anidbFile != null) this.anidbFile.vidstreams.add(vidstream);
					vidStreamIndex++;
					streamId++;
					break;
				case AVCodecLibrary.CODEC_TYPE_AUDIO:
					this.streams[i].relStreamId = audStreamIndex;
					// ERROR CHECKS
					if (codecCtx.codec_id != 0) codec = AVCODEC.avcodec_find_decoder(codecCtx.codec_id);
					if (codec == null) log.println("video codec not found for codec_id " + codecCtx.codec_id + " (avparser.run)");
					if (codec != null && AVCODEC.avcodec_open(codecCtx, codec) < 0) log.println("could not open video codec (avparser.run)");
					// STREAM PARSING
					AVStreamDataAudio audstream = new AVStreamDataAudio();
					audstream.streamId = streamId;
					audstream.relStreamId = audStreamIndex;
					audstream.timebase = stream.time_base.toDouble();
					audstream.type = codecCtx.codec_type;
					audstream.codec_id = codecCtx.codec_id;

					audstream.setAniDBAudioCodec(codecCtx.codec_id);
					audstream.language = Utils.convertByteArray(stream.language);
					audstream.channels = codecCtx.channels;
					audstream.layout = mapAudioChannels((int)codecCtx.channel_layout);
					audstream.sampleFormat = getSampleFormat(codecCtx.sample_fmt);
					audstream.sampleRate = codecCtx.sample_rate;
					if (codecCtx.bit_rate > 0) audstream.bitrate = codecCtx.bit_rate;
					if (this.anidbFile != null) this.anidbFile.audstreams.add(audstream);
					audStreamIndex++;
					streamId++;
					break;
				case AVCodecLibrary.CODEC_TYPE_SUBTITLE:
					this.streams[i].relStreamId = subStreamIndex;
					// STREAM PARSING
					AVStreamDataSubtitle substream = new AVStreamDataSubtitle();
					substream.streamId = streamId;
					substream.relStreamId = subStreamIndex;
					substream.timebase = stream.time_base.toDouble();
					substream.type = codecCtx.codec_type;
					substream.codec_id = codecCtx.codec_id;

					substream.codecName = mapSubtitleCodec(codecCtx.codec_id);
					substream.subtitleType = 20; // soft subtitle otherwise we wouldn't find it
					substream.language = Utils.convertByteArray(stream.language);
					switch(codecCtx.codec_id) {
						case AVCodecLibrary.CODEC_ID_TEXT: substream.isUnstyledSubs = true; break;
						case AVCodecLibrary.CODEC_ID_SSA: substream.isStyledSubs = true; break;
						case AVCodecLibrary.CODEC_ID_DVD_SUBTITLE:
						case AVCodecLibrary.CODEC_ID_XSUB:
						case AVCodecLibrary.CODEC_ID_BMP: substream.isImageSubs = true; break;
					}
					if (this.anidbFile != null) this.anidbFile.substreams.add(substream);
					subStreamIndex++;
					streamId++;
					break;
				default: // other data
					this.streams[i].relStreamId = othStreamIndex;
					//STREAM PARSING
					AVStreamData avstream = new AVStreamData();
					avstream.streamId = streamId;
					avstream.relStreamId = othStreamIndex;
					avstream.timebase = stream.time_base.toDouble();
					avstream.type = codecCtx.codec_type;
					avstream.codec_id = codecCtx.codec_id;
					
					avstream.codecName = mapSubtitleCodec(codecCtx.codec_id);
					if (this.anidbFile != null) this.anidbFile.othstreams.add(avstream);
					othStreamIndex++;
					streamId++;
			}
			if (codec != null) AVCODEC.avcodec_close(codecCtx);
		}

		// this ocasionaly crashes, so it's disabled by default
		if (this.parseFile) {
			final AVPacket packet = new AVPacket();
			long totalSize = formatCtx.file_size;
			long step = totalSize/100;
			long parsedSize = 0;
			long curStep = 0;
			float curprogress = 0;
			while (AVFORMAT.av_read_frame(formatCtx, packet) >= 0) {
				int index = packet.stream_index;
				if (this.streams[index] == null) this.streams[index] = new AVStreamData();
				int size = packet.size;
				long pts = packet.pts;
				long duration = packet.duration;
				this.streams[index].size += size;
				this.streams[index].pts = pts;
				this.streams[index].ptsArray.add(pts);
				// try for vbr */
				if (!this.streams[index].isVBR) {
					switch (this.vbr_calc_mode) {
					case AVParser.VBR_BY_PACKET_SIZE:
						if (this.streams[index].lastPacketSize != 0 && Math.abs(size - this.streams[index].lastPacketSize) > AVParser.VBR_DELTA_SIZE) {
							if (seeDebug) System.out.println("Possible VBR (by size): "+size+" vs "+this.streams[index].lastPacketSize);
							this.streams[index].isVBR = true;
						}
						this.streams[index].lastPacketSize = packet.size;
						break;
					case AVParser.VBR_BY_PACKET_BITRATE:
					default:
						double curPacketBitrate = ((double)(size*8) / ((double)duration))*1000;
						if (this.streams[index].lastPacketBitrate != 0 && Math.abs(curPacketBitrate - this.streams[index].lastPacketBitrate) > 1000*AVParser.VBR_DELTA_BITRATE) {
							if (seeDebug) System.out.println("Possible VBR (by bitrate): "+curPacketBitrate+" vs "+this.streams[index].lastPacketBitrate);
							this.streams[index].isVBR = true;
						}
						this.streams[index].lastPacketBitrate = curPacketBitrate;
						break;
					}
				}
				parsedSize += size;
				curStep += size;
				if (curStep >= step) { // this is needed because otherwise it would update too many times
					curprogress = (float)parsedSize/(float)totalSize;
					if (progress != null) this.progress.setProgress(curprogress);
					curStep = curStep % step; // reset step
				}

				// Free the packet that was allocated by av_read_frame
				// AVFORMAT.av_free_packet(packet.getPointer()) - cannot be called because it is an inlined function.
				// so we'll just do the JNA equivalent of the inline:
				if (packet.destruct != null)
					packet.destruct.callback(packet);
			}

			// fill in computed bitrate for audio and video streams
			for (int i = 0; i < formatCtx.nb_streams; i++) {
				if (this.streams[i] == null) continue;
				if (this.streams[i].type == AVCodecLibrary.CODEC_TYPE_VIDEO || this.streams[i].type == AVCodecLibrary.CODEC_TYPE_AUDIO) {
					this.streams[i].duration = this.streams[i].pts * this.streams[i].timebase + this.streams[i].timebase;
					this.streams[i].bitrate = (double)(this.streams[i].size * 8) / this.streams[i].duration;
					this.streams[i].formatedDuration = Utils.formatDurationSecs(this.streams[i].duration);
					// check if a file is VFR
					if (this.streams[i].type == AVCodecLibrary.CODEC_TYPE_VIDEO) {
						Collections.sort(this.streams[i].ptsArray);
						Iterator<Long> it = this.streams[i].ptsArray.iterator();
						long lastpts = 0;
						long lastPtsInterval = 0;
						while(it.hasNext()) {
							long curpts = it.next();
							if (lastpts == 0) lastpts = curpts;
							long curPtsInterval = (curpts-lastpts);
							if (lastPtsInterval != 0 && Math.abs(curPtsInterval - lastPtsInterval) > AVParser.VFR_DELTA) {
								if (seeDebug) System.out.println("Possible VFR: "+curPtsInterval+" vs "+lastPtsInterval);
								this.streams[i].isVFR = true;
								break; // no need to check any more
							}
							lastpts = curpts;
							lastPtsInterval = curPtsInterval;
						}
					}
				}
				if (this.anidbFile != null) {
					// now update anidbFile streams
					switch (this.streams[i].type) {
					case AVCodecLibrary.CODEC_TYPE_VIDEO:
						this.anidbFile.vidstreams.get(this.streams[i].relStreamId).isVFR = this.streams[i].isVFR;
						this.anidbFile.vidstreams.get(this.streams[i].relStreamId).isVBR = this.streams[i].isVBR;
						this.anidbFile.vidstreams.get(this.streams[i].relStreamId).size = this.streams[i].size;
						this.anidbFile.vidstreams.get(this.streams[i].relStreamId).bitrate = this.streams[i].bitrate;
						this.anidbFile.vidstreams.get(this.streams[i].relStreamId).duration = this.streams[i].duration;
						this.anidbFile.vidstreams.get(this.streams[i].relStreamId).formatedDuration = this.streams[i].formatedDuration;
						this.anidbFile.vidstreams.get(this.streams[i].relStreamId).fullParsed = true;
						break;
					case AVCodecLibrary.CODEC_TYPE_AUDIO:
						this.anidbFile.audstreams.get(this.streams[i].relStreamId).isVFR = this.streams[i].isVFR;
						this.anidbFile.audstreams.get(this.streams[i].relStreamId).isVBR = this.streams[i].isVBR;
						this.anidbFile.audstreams.get(this.streams[i].relStreamId).size = this.streams[i].size;
						this.anidbFile.audstreams.get(this.streams[i].relStreamId).bitrate = this.streams[i].bitrate;
						this.anidbFile.audstreams.get(this.streams[i].relStreamId).duration = this.streams[i].duration;
						this.anidbFile.audstreams.get(this.streams[i].relStreamId).formatedDuration = this.streams[i].formatedDuration;
						this.anidbFile.audstreams.get(this.streams[i].relStreamId).fullParsed = true;
						break;
					case AVCodecLibrary.CODEC_TYPE_SUBTITLE:
						this.anidbFile.substreams.get(this.streams[i].relStreamId).size = this.streams[i].size;
						this.anidbFile.substreams.get(this.streams[i].relStreamId).fullParsed = true;
						break;
					default:
						this.anidbFile.othstreams.get(this.streams[i].relStreamId).size = this.streams[i].size;
						this.anidbFile.othstreams.get(this.streams[i].relStreamId).fullParsed = true;
					}
				}
				if (this.seeDebug) System.out.println("stream["+this.streams[i].typeName+":"+i+"] timebase: "+this.streams[i].timebase);
			}
			// set full parsed state for anidb file
			if (this.anidbFile != null) this.anidbFile.state |= AniDBFile.PARSED_FULL;
			this.progress.setProgress(1); // finish it up
		}

		long time = (System.currentTimeMillis() - start);
		log.println("Completed parsing of \""+file.getName()+"\" in "+time+"ms @"+Utils.calculateProcessingRate(file.length(),time));

		//AVFORMAT.av_close_input_file(formatCtx);
		return;
	}
}
