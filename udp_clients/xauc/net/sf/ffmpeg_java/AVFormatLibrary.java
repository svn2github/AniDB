package net.sf.ffmpeg_java;
import com.sun.jna.Callback;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.LongByReference;
import com.sun.jna.ptr.PointerByReference;

/**
 * Based on FFMPEG revision 12162, Feb 20 2008.
 * From avformat.h, avio.h
 * @author Ken Larson, Miguel Gomes
 *
 */
public interface AVFormatLibrary extends FFMPEGLibrary 
{
    public static final AVFormatLibrary INSTANCE = (AVFormatLibrary) Native.loadLibrary(
    		System.getProperty("os.name").startsWith("Windows") ? "avformat-52" : "avformat",
    		AVFormatLibrary.class);

//------------------------------------------------------------------------------------------------------------------------
// avformat.h
    
    // #define LIBAVFORMAT_VERSION_TRIPLET 52,7,0
    public static final int LIBAVFORMAT_VERSION_INT = ((52<<16)+(7<<8)+0);
    public static final String LIBAVFORMAT_VERSION     = "52.7.0";
    public static final int LIBAVFORMAT_BUILD       = LIBAVFORMAT_VERSION_INT;

    public static final String LIBAVFORMAT_IDENT       = "Lavf" + LIBAVFORMAT_VERSION;
	
    interface Destruct extends Callback 
    {
        void callback(AVPacket pkt);
    }

	public static class AVPacket extends Structure
	{
		/** < presentation time stamp in time_base units */
		public long pts;
		/** < decompression time stamp in time_base units */
		public long dts;
		public Pointer data;
		public int size;
		public int stream_index;
		public int flags;
		/** < presentation duration in time_base units (0 if not available) */
		public int duration;
		public Destruct destruct;
		public Pointer priv;
		/** < byte position in stream, -1 if unknown */
		public long pos;
	}
	
	public static final int PKT_FLAG_KEY   = 0x0001;
	
	void av_destruct_packet_nofree(AVPacket pkt);
	void av_destruct_packet(AVPacket pkt);
	void av_init_packet(AVPacket pkt);
	int av_new_packet(AVPacket pkt, int size);
	int av_get_packet(ByteIOContext s, AVPacket pkt, int size);
	int av_dup_packet(AVPacket pkt);

	
//	/**
//	 * Free a packet
//	 *
//	 * @param pkt packet to free
//	 */
//	static inline void av_free_packet(AVPacket *pkt)
//	{
//	    if (pkt && pkt->destruct) {
//	        pkt->destruct(pkt);
//	    }
//	}
	/**
	 * Free a packet<br />
	 * <b>note:</b> this won't work with JNA, since it is inlined.<br />
	 * use: if (pkt.destruct != null) pkt.destruct.callback(pkt);
	 * @param pkt packet to free
	 */
	void av_free_packet(AVPacket pkt);	// TODO: this won't work with JNA, since it is inlined.
	
	//	typedef struct AVFrac {
	//    int64_t val, num, den;
	//} AVFrac attribute_deprecated;
	
	public static class AVFrac extends Structure
	{
	    public long val;
	    public long num;
	    public long den;
	}

	/** this structure contains the data a format has to probe a file */
	public static class AVProbeData extends Structure
	{
	    public Pointer filename;
	    public Pointer buf;
	    public int buf_size;
	}

	public static final int AVPROBE_SCORE_MAX =100;               ///< max score, half of that is used for file extension based detection
	public static final int AVPROBE_PADDING_SIZE =32;             ///< extra allocated bytes at the end of the probe buffer
	
	public static class AVFormatParameters extends Structure
	{
	    public AVRational time_base;
	    public int sample_rate;
	    public int channels;
	    public int width;
	    public int height;
	    public int pix_fmt;
	    /**< used to select dv channel */
	    public int channel; 
	    /**< tv standard, NTSC, PAL, SECAM */
	    public Pointer standard; 
		public int bitFields;	// bit fields not supported by JNA
//	    int mpeg2ts_raw:1;  /**< force raw MPEG2 transport stream output, if possible */
//	    int mpeg2ts_compute_pcr:1; /**< compute exact PCR for each transport
//	                                  stream packet (only meaningful if
//	                                  mpeg2ts_raw is TRUE) */
//	    int initial_pause:1;       /**< do not begin to play the stream
//	                                  immediately (RTSP only) */
//	    int prealloced_context:1;
//#if LIBAVFORMAT_VERSION_INT < (53<<16)
		public int video_codec_id;
	    public int audio_codec_id;
//#endif
}
	
	public static final int AVFMT_NOFILE        =0x0001;
	/**< needs '%d' in filename */
	public static final int AVFMT_NEEDNUMBER    =0x0002; 
	/**< show format stream IDs numbers */
	public static final int AVFMT_SHOW_IDS      =0x0008; 
	/**< format wants AVPicture structure for raw picture data */
	public static final int AVFMT_RAWPICTURE    =0x0020; 
	/**< format wants global header */
	public static final int AVFMT_GLOBALHEADER  =0x0040; 
	/**< format does not need / have any timestamps */
	public static final int AVFMT_NOTIMESTAMPS  =0x0080; 
	/**< use generic index building code */
	public static final int AVFMT_GENERIC_INDEX =0x0100; 
	
	public static class AVOutputFormat extends Structure
	{
	    public String name;
	    public String long_name;
	    public String mime_type;
	    /**< comma separated filename extensions */
	    public String extensions;
	    public int priv_data_size;
	    /**< default audio codec */
	    public int audio_codec;
	    /**< default video codec */
	    public int video_codec; 
	    public Pointer write_header;
	    public Pointer write_packet;
	    public Pointer write_trailer;
	    /** can use flags: AVFMT_NOFILE, AVFMT_NEEDNUMBER, AVFMT_GLOBALHEADER */
	    public int flags;
	    /** currently only used to set pixel format if not YUV420P */
	    public Pointer set_parameters;
	    public Pointer interleave_packet;
	    /**
	     * list of supported codec_id-codec_tag pairs, ordered by "better choice first"
	     * the arrays are all CODEC_ID_NONE terminated
	     */
	    public Pointer codec_tag;
	    /**< default subtitle codec */
	    public int subtitle_codec; 
	    /* private fields */
	    public Pointer next;

	    public AVOutputFormat() {
	    	super();
	    }
	    
	    public AVOutputFormat(Pointer p)
		{	super();
			useMemory(p);
			read();
		}
	}
	
	
	public static class AVInputFormat extends Structure
	{
	    public String name;
	    public String long_name;
	    public int priv_data_size;
	    public Pointer read_probe;
	    public Pointer read_header;
	    public Pointer read_packet;
	    public Pointer read_close;
	    public Pointer read_seek;
	    public Pointer read_timestamp;
	    public int flags;
	    public String extensions;
	    public int value;
	    public Pointer read_play;
	    public Pointer read_pause;
	    public Pointer codec_tag;
	    public Pointer next;

	    public AVInputFormat() {
	    	super();
	    }
	    
	    public AVInputFormat(Pointer p)
		{	super();
			useMemory(p);
			read();
		}
	}
	
	//enum AVStreamParseType:
    public static final int AVSTREAM_PARSE_NONE = 0;
    /**< full parsing and repack */
    public static final int AVSTREAM_PARSE_FULL = 1;
    /**< only parse headers, don't repack */
    public static final int AVSTREAM_PARSE_HEADERS = 2;
    /**< full parsing and interpolation of timestamps for frames not starting on packet boundary */
    public static final int AVSTREAM_PARSE_TIMESTAMPS = 3; 

   public static class AVIndexEntry extends Structure
	{
    	public long pos;
        public long timestamp;
        	public static final int AVINDEX_KEYFRAME = 0x0001;
        public int bit_fields; // JNA does not support bit fields
//        int flags:2;
//        int size:30; //Yeah, trying to keep the size of this small to reduce memory requirements (it is 24 vs 32 byte due to possible 8byte align).
        /**< min distance between this and the previous keyframe, used to avoid unneeded searching */
        public int min_distance;         
    }
	

	public static class AVStream extends Structure {
		/**< stream index in AVFormatContext */
		public int index;
		/**< format-specific stream ID */
		public int id;
		/**< codec context */
		public Pointer codec;
	    /**
	     * Real base frame rate of the stream.
	     * This is the lowest frame rate with which all timestamps can be
	     * represented accurately (it is the least common multiple of all
	     * frame rates in the stream). Note, this value is just a guess!
	     * For example if the time base is 1/90000 and all frames have either
	     * approximately 3600 or 1800 timer ticks, then r_frame_rate will be 50/1.
	     */
		public AVRational r_frame_rate;
		public Pointer priv_data;
		/* internal data used in av_find_stream_info() */
		public long first_dts;
		/** encoding: pts generation when outputting stream */
		public AVFrac pts;
		/**
	     * This is the fundamental unit of time (in seconds) in terms
	     * of which frame timestamps are represented. For fixed-fps content,
	     * time base should be 1/frame rate and timestamp increments should be 1.
	     */
		public AVRational time_base;
		/**< number of bits in pts (used for wrapping control) */
		public int pts_wrap_bits; 
		/* ffmpeg.c private use */
		/**< If set, just copy stream. */
		public int stream_copy; 
		/**< Selects which packets can be discarded at will and do not need to be demuxed. */
		public int discard; 
		//FIXME move stuff to a flags field?
	    /** Quality, as it has been removed from AVCodecContext and put in AVVideoFrame.
	     * MN: dunno if that is the right place for it */
		public float quality;
		/**
	     * Decoding: pts of the first frame of the stream, in stream time base.
	     * Only set this if you are absolutely 100% sure that the value you set
	     * it to really is the pts of the first frame.
	     * This may be undefined (AV_NOPTS_VALUE).
	     * @note The ASF header does NOT contain a correct start_time the ASF
	     * demuxer must NOT set this.
	     */
		public long start_time;
		/**
	     * Decoding: duration of the stream, in stream time base.
	     * If a source file does not specify a duration, but does specify
	     * a bitrate, this value will be estimated from bitrate and file size.
	     */
		public long duration;
		/** ISO 639 3-letter language code (empty string if undefined) */
		public byte[] language = new byte[4];

		/* av_read_frame() support */
		public int need_parsing;
		public Pointer parser;
		public long cur_dts;
		public int last_IP_duration;
		public long last_IP_pts;
		
		/* av_seek_frame() support */
		/**< Only used if the format does not support seeking natively. */
		public Pointer index_entries; 
		public int nb_index_entries;
		public int index_entries_allocated_size;
		/**< number of frames in this stream if known or 0 */
		public long nb_frames;              
		//public static final int MAX_REORDER_DELAY = 4;
		public long[] unused = new long[4+1];
		/**< source filename of the stream */
		public Pointer filename;
		/**< AV_DISPOSITION_* bit field */
		public int disposition;
	    public AVProbeData probe_data;
		public static final int MAX_REORDER_DELAY = 16;
	    public long[] pts_buffer = new long [MAX_REORDER_DELAY+1];
	    /**
	     * sample aspect ratio (0 if unknown)
	     * - encoding: Set by user.
	     * - decoding: Set by libavformat.
	     */
	    public AVRational sample_aspect_ratio;

		public AVStream()
		{
			super();
		}
		
		public AVStream(Pointer p)
		{	super();
			useMemory(p);
			read();
		}
	}
	public static final int AV_PROGRAM_RUNNING = 1;
	
	/**
	 * New fields can be added to the end with minor version bumps.
	 * Removal, reordering and changes to existing fields require a major
	 * version bump.
	 * sizeof(AVProgram) must not be used outside libav*.
	 */
	public static class AVProgram extends Structure
	{
	    public int id;
	    /**< Network name for DVB streams */
	    public Pointer provider_name;
	    /**< Service name for DVB streams */
	    public Pointer name;
	    public int flags;
	    /**< selects which program to discard and which to feed to the caller */
	    public int discard;
	    public Pointer stream_index;
	    public /*unsigned*/ int   nb_stream_indexes;
	};
	
	/**< signal that no header is present (streams are added dynamically) */
	public static final int AVFMTCTX_NOHEADER      = 0x0001; 

	public static final int MAX_STREAMS = 20;
	

	public static class AVFormatContext extends Structure
	{
		/**< Set by av_alloc_format_context. */
		public Pointer av_class;
		public Pointer iformat;
		public Pointer oformat;
		public Pointer priv_data;
		public Pointer /* ByteIOContext* */ pb;
		public int nb_streams;
		public static final int MAX_STREAMS = 20;
		// don't know how to do an array of pointers with JNA, so we'll just do 20 pointers:
		public Pointer streams0;
		public Pointer streams1;
		public Pointer streams2;
		public Pointer streams3;
		public Pointer streams4;
		public Pointer streams5;
		public Pointer streams6;
		public Pointer streams7;
		public Pointer streams8;
		public Pointer streams9;
		public Pointer streams10;
		public Pointer streams11;
		public Pointer streams12;
		public Pointer streams13;
		public Pointer streams14;
		public Pointer streams15;
		public Pointer streams16;
		public Pointer streams17;
		public Pointer streams18;
		public Pointer streams19;
		public Pointer[] getStreams()
		{	return new Pointer[] {
				streams0,
				streams1,
				streams2,
				streams3,
				streams4,
				streams5,
				streams6,
				streams7,
				streams8,
				streams9,
				streams10,
				streams11,
				streams12,
				streams13,
				streams14,
				streams15,
				streams16,
				streams17,
				streams18,
				streams19
			};
		}
		//public Pointer[] streams = new Pointer[MAX_STREAMS]; //	    AVStream *streams[MAX_STREAMS];
		/**< input or output filename */
		public byte[] filename = new byte[1024]; 
		public long timestamp;
		public byte[] title = new byte[512];
		public byte[] author = new byte[512];
		public byte[] copyright = new byte[512];
		public byte[] comment = new byte[512];
		public byte[] album = new byte[512];
		/**< ID3 year, 0 if none */
		public int year;
		/**< track number, 0 if none */
		public int track; 
		/**< ID3 genre */
		public byte[] genre = new byte[32];
		/**< Format-specific flags, see AVFMTCTX_xx */
		public int ctx_flags; 
	    /* private data for pts handling (do not modify directly). */
	    /** This buffer is only needed when packets were already buffered but
	       not decoded, for example to get the codec parameters in MPEG
	       streams. */
		public Pointer packet_buffer;
	    /** Decoding: position of the first frame of the component, in
	       AV_TIME_BASE fractional seconds. NEVER set this value directly:
	       It is deduced from the AVStream values.  */
		public long start_time;
	    /** Decoding: duration of the stream, in AV_TIME_BASE fractional
	       seconds. NEVER set this value directly: it is deduced from the
	       AVStream values.  */
		public long duration;
		/** decoding: total file size, 0 if unknown */
		public long file_size;
		/** Decoding: total stream bitrate in bit/s, 0 if not
	       available. Never set it directly if the file_size and the
	       duration are known as ffmpeg can compute it automatically. */
		public int bit_rate;
		/* av_read_frame() support */
		public Pointer cur_st;
		public Pointer cur_ptr;
		public int cur_len;
		public AVPacket cur_pkt;
		/* av_seek_frame() support */
		/** offset of the first packet */
		public long data_offset;
		public int index_built;
		public int mux_rate;
		public int packet_size;
		public int preload;
		public int max_delay;
			public static final int AVFMT_NOOUTPUTLOOP = -1;
			public static final int AVFMT_INFINITEOUTPUTLOOP = 0;
		/** number of times to loop output in formats that support it */
		public int loop_output;
		public int flags;
		/**< generate pts if missing even if it requires parsing future frames */
			public static final int AVFMT_FLAG_GENPTS       =0x0001;
		/**< ignore index */
			public static final int AVFMT_FLAG_IGNIDX       =0x0002;
		/**< do not block when reading packets from input */
			public static final int AVFMT_FLAG_NONBLOCK     =0x0004;
		public int loop_input;
		/** Decoding: size of data to probe; encoding: unused. */
		public int probesize;
	    /**
	     * Maximum time (in AV_TIME_BASE units) during which the input should
	     * be analyzed in av_find_stream_info().
	     */
		public int max_analyze_duration;
		public Pointer key;
		public int keylen;
	    public /*unsigned*/ int nb_programs;
	    public Pointer /*AVProgram ** */ programs;

	    /**
	     * Forced video codec_id.
	     * demuxing: set by user
	     */
	    public int /*enum CodecID*/ video_codec_id;
	    /**
	     * Forced audio codec_id.
	     * demuxing: set by user
	     */
	    public int /*enum CodecID*/ audio_codec_id;
	    /**
	     * Forced subtitle codec_id.
	     * demuxing: set by user
	     */
	    public int /*enum CodecID*/ subtitle_codec_id;
	    /**
	     * Maximum amount of memory in bytes to use per stream for the index.
	     * If the needed index exceeds this size entries will be discarded as
	     * needed to maintain a smaller size. This can lead to slower or less
	     * accurate seeking (depends on demuxer).
	     * Demuxers for which a full in memory index is mandatory will ignore
	     * this.
	     * muxing  : unused
	     * demuxing: set by user
	     */
	    public /*unsigned*/ int max_index_size;
	    /**
	     * Maximum amount of memory in bytes to use for buffering frames
	     * obtained from realtime capture devices.
	     */
	    public /*unsigned*/ int max_picture_buffer;
	    /** Number of chapters */
	    public /*unsigned*/ int nb_chapters;
	    /** AVChapter */
	    public Pointer chapters;
	    /**
	     * Flags to enable debugging.
	     */
	    public int debug;
	    	public static int FF_FDEBUG_TS        = 0x0001;
	    /**
	     * Raw packets from the demuxer, prior to parsing and decoding.
	     * This buffer is used for buffering packets until the codec can
	     * be identified, as parsing cannot be done without knowing the
	     * codec.
	     * AVPacketList
	     */
	    public Pointer raw_packet_buffer;
	    /** AVPacketList */
	    public Pointer raw_packet_buffer_end;
	    /** AVPacketList */
	    public Pointer packet_buffer_end;
	    
		public AVFormatContext()
		{
			super();
		}
		
		public AVFormatContext(Pointer p)
		{	super();
			useMemory(p);
			read();
		}
	}
	
	public static class AVPacketList extends Structure
	{
	    public AVPacket pkt;
	    public Pointer next;
	}
	
// #if LIBAVFORMAT_VERSION_INT < (53<<16)	
//	extern AVInputFormat *first_iformat;
//	extern AVOutputFormat *first_oformat;
// #endif
	
	// TODO: JNA does not support global variable access
	
	AVInputFormat  av_iformat_next(AVInputFormat  f);
	AVOutputFormat av_oformat_next(AVOutputFormat f);
	
	//enum CodecID av_guess_image2_codec(const char *filename);
	int av_guess_image2_codec(String filename);
	
	
	void av_register_input_format(AVInputFormat format);
	void av_register_output_format(AVOutputFormat format);
	AVOutputFormat guess_stream_format(String short_name,
	                                    String filename, String mime_type);
	AVOutputFormat guess_format(String short_name,
	                             String filename, String mime_type);
	
	
	int av_guess_codec(AVOutputFormat fmt, String short_name,
            String filename, String mime_type, int type);

	void av_hex_dump(Pointer f, Pointer buf, int size);
	void av_hex_dump_log(Pointer avcl, int level, Pointer buf, int size);
	void av_pkt_dump(Pointer f, AVPacket pkt, int dump_payload);
	
	void av_pkt_dump_log(Pointer avcl, int level, AVPacket pkt, int dump_payload);
	

	void av_register_all();
	
	int av_codec_get_id(PointerByReference tags, int tag);
	int av_codec_get_tag(PointerByReference tags, int id);
	
	AVInputFormat av_find_input_format(String short_name);

	AVInputFormat av_probe_input_format(AVProbeData pd, int is_opened);

	/**
	 * Allocates all the structures needed to read an input stream.
	 *        This does not open the needed codecs for decoding the stream[s].
	 */
	int av_open_input_stream(PointerByReference ic_ptr,
			ByteIOContext pb, String filename,
			AVInputFormat fmt, AVFormatParameters ap);
	/**
	 * Open a media file as input. The codecs are not opened. Only the file
	 * header (if present) is read.
	 *
	 * @param ic_ptr the opened media file handle is put here
	 * @param filename filename to open.
	 * @param fmt if non NULL, force the file format to use
	 * @param buf_size optional buffer size (zero if default is OK)
	 * @param ap additional parameters needed when opening the file (NULL if default)
	 * @return 0 if OK. AVERROR_xxx otherwise.
	 */
	int av_open_input_file(PointerByReference ic_ptr, String filename,
			AVInputFormat fmt,
	        int buf_size,
	        AVFormatParameters ap);

	AVFormatContext av_alloc_format_context();
	int av_find_stream_info(AVFormatContext ic);
	int av_read_packet(AVFormatContext s, AVPacket pkt);
	/**
	 * Return the next frame of a stream.
	 *
	 * The returned packet is valid
	 * until the next av_read_frame() or until av_close_input_file() and
	 * must be freed with av_free_packet. For video, the packet contains
	 * exactly one frame. For audio, it contains an integer number of
	 * frames if each frame has a known fixed size (e.g. PCM or ADPCM
	 * data). If the audio frames have a variable size (e.g. MPEG audio),
	 * then it contains one frame.
	 *
	 * pkt->pts, pkt->dts and pkt->duration are always set to correct
	 * values in AVStream.timebase units (and guessed if the format cannot
	 * provided them). pkt->pts can be AV_NOPTS_VALUE if the video format
	 * has B frames, so it is better to rely on pkt->dts if you do not
	 * decompress the payload.
	 *
	 * @return 0 if OK, < 0 if error or end of file.
	 */
	int av_read_frame(AVFormatContext s, AVPacket pkt);
	/**
	 * Seek to the key frame at timestamp.
	 * 'timestamp' in 'stream_index'.
	 * @param stream_index If stream_index is (-1), a default
	 * stream is selected, and timestamp is automatically converted
	 * from AV_TIME_BASE units to the stream specific time_base.
	 * @param timestamp timestamp in AVStream.time_base units
	 *        or if there is no stream specified then in AV_TIME_BASE units
	 * @param flags flags which select direction and seeking mode
	 * @return >= 0 on success
	 */
	int av_seek_frame(AVFormatContext s, int stream_index, long timestamp, int flags);
	/**
	 * start playing a network based stream (e.g. RTSP stream) at the
	 * current position
	 */
	int av_read_play(AVFormatContext s);
	/**
	 * Pause a network based stream (e.g. RTSP stream).
	 *
	 * Use av_read_play() to resume it.
	 */
	int av_read_pause(AVFormatContext s);
	void av_close_input_stream(AVFormatContext s);
	void av_close_input_file(AVFormatContext s);
	AVStream av_new_stream(AVFormatContext s, int id);
	AVProgram av_new_program(AVFormatContext s, int id);
	void av_set_pts_info(AVStream s, int pts_wrap_bits,
	                     int pts_num, int pts_den);

	
	/**< seek backward */
	public static final int AVSEEK_FLAG_BACKWARD =1;
	/**< seeking based on position in bytes */
	public static final int AVSEEK_FLAG_BYTE     =2;
	/**< seek to any frame, even non keyframes */
	public static final int AVSEEK_FLAG_ANY      =4;
	
	
	int av_find_default_stream_index(AVFormatContext s);
	int av_index_search_timestamp(AVStream st, long timestamp, int flags);
	void ff_reduce_index(AVFormatContext s, int stream_index);
	int av_add_index_entry(AVStream st,
	                       long pos, long timestamp, int size, int distance, int flags);
	int av_seek_frame_binary(AVFormatContext s, int stream_index, long target_ts, int flags);
	void av_update_cur_dts(AVFormatContext s, AVStream ref_st, long timestamp);
	long av_gen_search(AVFormatContext s, int stream_index, long target_ts, long pos_min, long pos_max, long pos_limit, long ts_min, long ts_max, int flags, LongByReference ts_ret, Pointer read_timestamp);
	int av_set_parameters(AVFormatContext s, AVFormatParameters ap);
	int av_write_header(AVFormatContext s);
	int av_write_frame(AVFormatContext s, AVPacket pkt);
	int av_interleaved_write_frame(AVFormatContext s, AVPacket pkt);
	int av_interleave_packet_per_dts(AVFormatContext s, AVPacket out, AVPacket pkt, int flush);
	int av_write_trailer(AVFormatContext s);
	void dump_format(AVFormatContext ic,
	                 int index,
	                 String url,
	                 int is_output);
	/** @deprecated */
	int parse_image_size(IntByReference width_ptr, IntByReference height_ptr, String str);
	/** @deprecated */
	int parse_frame_rate(IntByReference frame_rate, IntByReference frame_rate_base, String arg);
	long parse_date(String datestr, int duration);
	long av_gettime();
	public static final int FFM_PACKET_SIZE = 4096;
	long ffm_read_write_index(int fd);
	void ffm_write_write_index(int fd, long pos);
	void ffm_set_write_index(AVFormatContext s, long pos, long file_size);
	int find_info_tag(Pointer arg, int arg_size, String tag1, String info);
	int av_get_frame_filename(Pointer buf, int buf_size,
	                          String path, int number);
	int av_filename_number_test(String filename);
	int avf_sdp_create(Pointer ac, int n_files, Pointer buff, int size);

//	#ifdef HAVE_AV_CONFIG_H

//	#include "os_support.h"
//
//	void __dynarray_add(unsigned long **tab_ptr, int *nb_ptr, unsigned long elem);
//
//	#ifdef __GNUC__
//	#define dynarray_add(tab, nb_ptr, elem)\
//	do {\
//	    typeof(tab) _tab = (tab);\
//	    typeof(elem) _elem = (elem);\
//	    (void)sizeof(**_tab == _elem); /* check that types are compatible */\
//	    __dynarray_add((unsigned long **)_tab, nb_ptr, (unsigned long)_elem);\
//	} while(0)
//	#else
//	#define dynarray_add(tab, nb_ptr, elem)\
//	do {\
//	    __dynarray_add((unsigned long **)(tab), nb_ptr, (unsigned long)(elem));\
//	} while(0)
//	#endif

	// TODO: __dynarray_add and dynarray_add with JNA
	
//	time_t mktimegm(struct tm *tm);
//	struct tm *brktimegm(time_t secs, struct tm *tm);
	// TODO: mktimegm with JNA
	// TODO: brktimegm with JNA
	
	String small_strptime(String p, String fmt,
	                           Pointer dt);

	int resolve_host(Pointer sin_addr, String hostname);

	void url_split(Pointer proto, int proto_size,
	               Pointer authorization, int authorization_size,
	               Pointer hostname, int hostname_size,
	               IntByReference port_ptr,
	               Pointer path, int path_size,
	               String url);

	int match_ext(String filename, String extensions);
	
	
// end avformat.h
//------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
// avio.h


//	struct URLContext {
//	    struct URLProtocol *prot;
//	    int flags;
//	    int is_streamed;  /**< true if streamed (no seek possible), default = false */
//	    int max_packet_size;  /**< if non zero, the stream is packetized with this max packet size */
//	    void *priv_data;
//	#if LIBAVFORMAT_VERSION_INT >= (52<<16)
//	    char *filename; /**< specified filename */
//	#else
//	    char filename[1]; /**< specified filename */
//	#endif
//	};
	
	public static class URLContext extends Structure
	{
	    public Pointer prot;
	    public int flags;
	    /**< true if streamed (no seek possible), default = false */
	    public int is_streamed;  
	    /**< if non zero, the stream is packetized with this max packet size */
	    public int max_packet_size;
	    public Pointer priv_data;
	    /**< specified filename */
	    public Pointer /* char * */ filename;
	}
	
	public static class URLPollEntry extends Structure
	{
	    public Pointer handle;
	    public int events;
	    public int revents;
	} 

	public static final int URL_RDONLY =0;
	public static final int URL_WRONLY =1;
	public static final int URL_RDWR   =2;

	//typedef int URLInterruptCB(void);

	int url_open(PointerByReference h, String filename, int flags);
	int url_read(URLContext h, Pointer buf, int size);
	int url_write(URLContext h, Pointer buf, int size);
	long url_seek(URLContext h, long pos, int whence);
	int url_close(URLContext h);
	int url_exist(String filename);
	long url_filesize(URLContext h);

	int url_get_max_packet_size(URLContext h);
	void url_get_filename(URLContext h, Pointer buf, int buf_size);
	void url_set_interrupt_cb(Pointer/*URLInterruptCB*/ interrupt_cb);
	int url_poll(URLPollEntry poll_table, int n, int timeout);
	int av_url_read_pause(URLContext h, int pause);
	public static final int AVSEEK_SIZE = 0x10000;

//	typedef struct URLProtocol {
//	    const char *name;
//	    int (*url_open)(URLContext *h, const char *filename, int flags);
//	    int (*url_read)(URLContext *h, unsigned char *buf, int size);
//	    int (*url_write)(URLContext *h, unsigned char *buf, int size);
//	    offset_t (*url_seek)(URLContext *h, offset_t pos, int whence);
//	    int (*url_close)(URLContext *h);
//	    struct URLProtocol *next;
//	} URLProtocol;

    interface URLOpen extends Callback 
    {
        int callback(URLContext h, String filename, int flags);
    }
    interface URLRead extends Callback 
    {
    	int callback(URLContext h, Pointer buf, int size);
    }
    interface URLWrite extends Callback 
    {
    	int callback(URLContext h, Pointer buf, int size);
    }
    interface URLSeek extends Callback 
    {
        long callback(URLContext h, long pos, int whence);
    }
    interface URLClose extends Callback 
    {
        int callback(URLContext h);
    }
    interface URLReadPause extends Callback 
    {
        int callback(URLContext h, int pause);
    }
    interface URLReadSeek extends Callback 
    {
        int callback(URLContext h, int stream_index, long timestamp, int flags);
    }
	
	public static class URLProtocol extends Structure
	{
	    public String name;
	    public URLOpen url_open;
	    public URLRead url_read;
	    public URLWrite url_write;
	    public URLSeek url_seek;
	    public URLClose url_close;
	    public Pointer next;
	    public URLReadPause url_read_pause;
	    public URLReadSeek url_read_seek;
	}

	
//	extern URLProtocol *first_protocol;
//	extern URLInterruptCB *url_interrupt_cb;
	// TODO: JNA does not allow access to global variables
	URLProtocol av_protocol_next(URLProtocol p);

	int register_protocol(URLProtocol protocol);

	public static class ByteIOContext extends Structure
	{
		public Pointer buffer;
		public int buffer_size;
		public Pointer buf_ptr;
		public Pointer buf_end;
		public Pointer opaque;
		public Pointer read_packet;
		public Pointer write_packet;
		public Pointer seek;
		/**< position in the file of the current buffer */
		public long pos; 
		/**< true if the next seek should flush */
		public int must_flush;
		/**< true if eof reached */
		public int eof_reached; 
		/**< true if open for writing */
		public int write_flag; 
		public int is_streamed;
		public int max_packet_size;
		public NativeLong checksum;	
		public Pointer checksum_ptr;
		public Pointer update_checksum;
		/**< contains the error code or 0 if no error happened */
		public int error;  
		public Pointer read_pause;
	    public Pointer read_seek;
	    
	    public ByteIOContext()
	    {
	    	super();
	    }
	    
	    public ByteIOContext(Pointer p)
	    {
	    	super();
			useMemory(p);
			read();
	    }
	}

	int init_put_byte(Pointer s,
            Pointer buffer,
            int buffer_size,
            int write_flag,
            Pointer opaque,
            Pointer read_packet,
            Pointer write_packet,
            Pointer seek);
	ByteIOContext av_alloc_put_byte(
            Pointer buffer,
            int buffer_size,
            int write_flag,
            Pointer opaque,
            Pointer read_packet,
            Pointer write_packet,
            Pointer seek);

	void put_byte(ByteIOContext s, int b);
	void put_buffer(ByteIOContext s, Pointer buf, int size);
	void put_le64(ByteIOContext s, long val);
	void put_be64(ByteIOContext s, long val);
	void put_le32(ByteIOContext s, int val);
	void put_be32(ByteIOContext s, int val);
	void put_le24(ByteIOContext s, int val);
	void put_be24(ByteIOContext s, int val);
	void put_le16(ByteIOContext s, int val);
	void put_be16(ByteIOContext s, int val);
	void put_tag(ByteIOContext s, String tag);
	
	void put_strz(ByteIOContext s, String buf);
	
	long url_fseek(ByteIOContext s, long offset, int whence);
	void url_fskip(ByteIOContext s, long offset);
	long url_ftell(ByteIOContext s);
	long url_fsize(ByteIOContext s);
	int url_feof(ByteIOContext s);
	int url_ferror(ByteIOContext s);

	int av_url_read_fpause(ByteIOContext h, int pause);
	long av_url_read_fseek(ByteIOContext h,
	                      int stream_index, long timestamp, int flags);
	
	public static final int URL_EOF = -1;
	int url_fgetc(ByteIOContext s);
	
//	/** @warning currently size is limited */
//	#ifdef __GNUC__
//	int url_fprintf(ByteIOContext s, const char *fmt, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
//	#else
//	int url_fprintf(ByteIOContext s, const char *fmt, ...);
//	#endif
// TODO: don't know how to map url_fprintf with JNA, given variable parameters.
	
	/** note unlike fgets, the EOL character is not returned and a whole
	line is parsed. return NULL if first char read was EOF */
	Pointer url_fgets(ByteIOContext s, Pointer buf, int buf_size);
	
	void put_flush_packet(ByteIOContext s);
	
	int get_buffer(ByteIOContext s, Pointer buf, int size);
	int get_partial_buffer(ByteIOContext s, Pointer buf, int size);
	
	/** note return 0 if EOF, so you cannot use it if EOF handling is
	necessary */
	int get_byte(ByteIOContext s);
	int get_le24(ByteIOContext s);
	int get_le32(ByteIOContext s);
	long get_le64(ByteIOContext s);
	int get_le16(ByteIOContext s);
	
	Pointer get_strz(ByteIOContext s, Pointer buf, int maxlen);
	int get_be16(ByteIOContext s);
	int get_be24(ByteIOContext s);
	int get_be32(ByteIOContext s);
	long get_be64(ByteIOContext s);
	long ff_get_v(ByteIOContext bc);

	
//	static inline int url_is_streamed(ByteIOContext s)
//	{
//	return s->is_streamed;
//	}
	// TODO: cannot call inlined functions with JNA.
	
	int url_fdopen(PointerByReference s, Pointer h);
	
	/** warning must be called before any I/O */
	int url_setbufsize(ByteIOContext s, int buf_size);
	int url_resetbuf(ByteIOContext s, int flags);
	/** note when opened as read/write, the buffers are only used for
	reading */
	int url_fopen(PointerByReference s, String filename, int flags);
	int url_fclose(ByteIOContext s);
	URLContext url_fileno(ByteIOContext s);
	
	/**
	* Return the maximum packet size associated to packetized buffered file
	* handle. If the file is not packetized (stream like http or file on
	* disk), then 0 is returned.
	*
	* @param s buffered file handle
	* @return maximum packet size in bytes
	*/
	int url_fget_max_packet_size(Pointer s);
	
	int url_open_buf(PointerByReference s, Pointer buf, int buf_size, int flags);
	
	/** return the written or read size */
	int url_close_buf(ByteIOContext s);
	
	/**
	* Open a write only memory stream.
	*
	* @param s new IO context
	* @return zero if no error.
	*/
	int url_open_dyn_buf(PointerByReference s);
	
	/**
	* Open a write only packetized memory stream with a maximum packet
	* size of 'max_packet_size'.  The stream is stored in a memory buffer
	* with a big endian 4 byte header giving the packet size in bytes.
	*
	* @param s new IO context
	* @param max_packet_size maximum packet size (must be > 0)
	* @return zero if no error.
	*/
	int url_open_dyn_packet_buf(PointerByReference s, int max_packet_size);
	
	/**
	* Return the written size and a pointer to the buffer. The buffer
	*  must be freed with av_free().
	* @param s IO context
	* @param pbuffer to a byte buffer
	* @return the length of the byte buffer
	*/
	int url_close_dyn_buf(ByteIOContext s, PointerByReference pbuffer);
	
	NativeLong ff_crc04C11DB7_update(NativeLong checksum, Pointer buf, int len);
	NativeLong get_checksum(ByteIOContext s);
	void init_checksum(ByteIOContext s, Pointer update_checksum, NativeLong checksum);
	
	int udp_set_remote_url(URLContext h, String uri);
	int udp_get_local_port(URLContext h);
	int udp_get_file_handle(URLContext h);
	

// end avio.h
//------------------------------------------------------------------------------------------------------------------------

	
}

