package structures;

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

import net.sf.ffmpeg_java.AVCodecLibrary;

import utils.Utils;
import xml.XmlObject;
import xml.XmlObjectException;

/**
 * Audio stream data wrapper
 */
public class AVStreamDataAudio extends AVStreamData {
	/** Stream audio language */
	public String language = "";
	/** Stream audio sample rate */
	public int sampleRate = 0; // in Hz
	/** Stream audio sample format */
	public String sampleFormat = "";
	/** Stream audio channels number */
	public int channels = 0;
	/** Stream audio channels layout */
	public String layout = "";
	// ANIDB data
	/** Stream audio type */
	public int audioType = 10;
	/** AnidbCodec Id use AnidbCodec getters for the rest */
	public AnidbCodec anidbcodecid = AnidbCodec.UNKNOWN;
	/** Codec mappings for anidb */
	public enum AnidbCodec {
		OTHER (15,"other","other","other"),
		UNKNOWN (1,"unknown","unk","unknown"),
		AC3 (2,"AC3","ac3","ac3"),
		WMA (3,"WMA (also DivX Audio)","wma","wma"),
		WMAV1 (3,"WMA (also DivX Audio)","wma","wma-v1"),
		WMAV2 (3,"WMA (also DivX Audio)","wma","wma-v2"),
		WMAPRO (3,"WMA (also DivX Audio)","wma","wma-pro"),
		WMAVOICE (3,"WMA (also DivX Audio)","wma","wma-voice"),
		WMALOSSLESS (3,"WMA (also DivX Audio)","wma","wma-lossless"),
		MP3 (5,"MP3","mp3","mp3"), // hack, need to merge both
		MP3CBR (5,"MP3 CBR","mp3cbr","mp3-cbr"),
		MP3VBR (6,"MP3 VBR","mp3vbr","mp3-vbr"),
		MSAUDIO (7,"MSAudio","msaudio","msaudio"),
		VORBIS (8,"Vorbis (Ogg Vorbis)","vorbis","vorbis"),
		AAC (9,"AAC","aac","aac"),
		PMC (10,"PCM","pcm","pcm"),
		PCM_ALAW (10,"PCM","pcm","pcm-alaw"),
		PCM_DVD (10,"PCM","pcm","pcm-dvd"),
		PCM_S8 (10,"PCM","pcm","pcm-s8"),
		PCM_S16BE (10,"PCM","pcm","pcm-s16be"),
		PCM_S16LE (10,"PCM","pcm","pcm-s16le"),
		PCM_S16LE_PLANAR (10,"PCM","pcm","pcm-s16le-planar"),
		PCM_S24BE (10,"PCM","pcm","pcm-s24be"),
		PCM_S24DAUD (10,"PCM","pcm","pcm-s24daud"),
		PCM_S24LE (10,"PCM","pcm","pcm-f24le"),
		PCM_F32BE (10,"PCM","pcm","pcm-f32be"),
		PCM_F32LE (10,"PCM","pcm","pcm-f32le"),
		PCM_F64BE (10,"PCM","pcm","pcm-f64be"),
		PCM_F64LE (10,"PCM","pcm","pcm-f64le"),
		PCM_U8 (10,"PCM","pcm","pcm-u8"),
		PCM_U16BE (10,"PCM","pcm","pcm-u16be"),
		PCM_U16LE (10,"PCM","pcm","pcm-u16le"),
		PCM_U24BE (10,"PCM","pcm","pcm-u24be"),
		PCM_U24LE (10,"PCM","pcm","pcm-u24le"),
		PCM_U32BE (10,"PCM","pcm","pcm-u32be"),
		PCM_U32LE (10,"PCM","pcm","pcm-u32le"),
		PCM_MULAW (10,"PCM","pcm","pcm-mulaw"),
		PCM_ZORK (10,"PCM","pcm","pcm-zork"),
		MP2 (11,"MP2","mp2","mp2"),
		DTS (13,"DTS","dts","dts");
		
		/** AniDB codec ID */
		private final int id;
		/** AniDB codec name */
		private final String name;
		/** AniDB short codec name */
		private final String shortname;
		/** FFmpeg codec name reference */
		private final String codecname;
		/**
		 * AniDBCodec enumeration
		 * @param id AniDB codec id 
		 * @param name AniDB codec name
		 * @param shortname AniDB codec shortname
		 * @param codecname FFmpeg codec name reference
		 */
		AnidbCodec (int id, String name, String shortname, String codecname) {
			this.id = id;
			this.name = name;
			this.shortname = shortname;
			this.codecname = codecname;
		}
		/** Gets the AniDB codec id associated with the current Codec */
		public int getAnidbID() { return id; };
		/** Gets the AniDB codec name associated with the current Codec */
		public String getAnidbName() { return name; };
		/** Gets the AniDB codec shortname associated with the current Codec */
		public String getAnidbShortName() { return shortname; }
		/** Gets the FFmpeg codec reference associated with the current Codec */
		public String getCodecName() { return codecname; }
		/** Gets the AniDB codec name plus the FFmpeg codec reference associated with the current Codec */
		public String getCodecString() { return name+" ("+codecname+")"; }
	}
	
	/** Default AVStreamDataAudio constructor */
	public AVStreamDataAudio() { super(); }

	/**
	 * Sets Anidb's Audio codec ids
	 * @param codec_id AVCodecLibrary.CODEC_ID_* identifier
	 */
	public void setAniDBAudioCodec(int codec_id) {
		switch (codec_id) {
			case AVCodecLibrary.CODEC_ID_AAC: this.anidbcodecid = AnidbCodec.AAC; break;
			case AVCodecLibrary.CODEC_ID_AC3: this.anidbcodecid = AnidbCodec.AC3; break;
			case AVCodecLibrary.CODEC_ID_DTS: this.anidbcodecid = AnidbCodec.DTS; break;
			case AVCodecLibrary.CODEC_ID_MP2: this.anidbcodecid = AnidbCodec.MP2; break;
			case AVCodecLibrary.CODEC_ID_MP3:
				if (this.fullParsed) {
					if (!this.isVBR) this.anidbcodecid = AnidbCodec.MP3CBR;
					else this.anidbcodecid = AnidbCodec.MP3VBR;
				} else this.anidbcodecid = AnidbCodec.MP3;
				break;
			case AVCodecLibrary.CODEC_ID_VORBIS: this.anidbcodecid = AnidbCodec.VORBIS; break;
			case AVCodecLibrary.CODEC_ID_WMAV1: this.anidbcodecid = AnidbCodec.WMAV1; break;
			case AVCodecLibrary.CODEC_ID_WMAV2: this.anidbcodecid = AnidbCodec.WMAV2; break;
			case AVCodecLibrary.CODEC_ID_WMAVOICE: this.anidbcodecid = AnidbCodec.WMAVOICE; break;
			case AVCodecLibrary.CODEC_ID_WMAPRO: this.anidbcodecid = AnidbCodec.WMAPRO; break;
			case AVCodecLibrary.CODEC_ID_WMALOSSLESS: this.anidbcodecid = AnidbCodec.WMALOSSLESS; break;
			case AVCodecLibrary.CODEC_ID_PCM_ALAW: this.anidbcodecid = AnidbCodec.PCM_ALAW; break;
			case AVCodecLibrary.CODEC_ID_PCM_DVD: this.anidbcodecid = AnidbCodec.PCM_DVD; break;
			case AVCodecLibrary.CODEC_ID_PCM_S8: this.anidbcodecid = AnidbCodec.PCM_S8; break;
			case AVCodecLibrary.CODEC_ID_PCM_S16BE: this.anidbcodecid = AnidbCodec.PCM_S16BE; break;
			case AVCodecLibrary.CODEC_ID_PCM_S16LE: this.anidbcodecid = AnidbCodec.PCM_S16LE; break;
			case AVCodecLibrary.CODEC_ID_PCM_S16LE_PLANAR: this.anidbcodecid = AnidbCodec.PCM_S16LE_PLANAR; break;
			case AVCodecLibrary.CODEC_ID_PCM_S24BE: this.anidbcodecid = AnidbCodec.PCM_S24BE; break;
			case AVCodecLibrary.CODEC_ID_PCM_S24DAUD: this.anidbcodecid = AnidbCodec.PCM_S24DAUD; break;
			case AVCodecLibrary.CODEC_ID_PCM_S24LE: this.anidbcodecid = AnidbCodec.PCM_S24LE; break;
			case AVCodecLibrary.CODEC_ID_PCM_F32BE: this.anidbcodecid = AnidbCodec.PCM_F32BE; break;
			case AVCodecLibrary.CODEC_ID_PCM_F32LE: this.anidbcodecid = AnidbCodec.PCM_F32LE; break;
			case AVCodecLibrary.CODEC_ID_PCM_F64BE: this.anidbcodecid = AnidbCodec.PCM_F64BE; break;
			case AVCodecLibrary.CODEC_ID_PCM_F64LE: this.anidbcodecid = AnidbCodec.PCM_F64LE; break;
			case AVCodecLibrary.CODEC_ID_PCM_U8: this.anidbcodecid = AnidbCodec.PCM_U8; break;
			case AVCodecLibrary.CODEC_ID_PCM_U16BE: this.anidbcodecid = AnidbCodec.PCM_U16BE; break;
			case AVCodecLibrary.CODEC_ID_PCM_U16LE: this.anidbcodecid = AnidbCodec.PCM_U16LE; break;
			case AVCodecLibrary.CODEC_ID_PCM_U24BE: this.anidbcodecid = AnidbCodec.PCM_U24BE; break;
			case AVCodecLibrary.CODEC_ID_PCM_U24LE: this.anidbcodecid = AnidbCodec.PCM_U24LE; break;
			case AVCodecLibrary.CODEC_ID_PCM_U32BE: this.anidbcodecid = AnidbCodec.PCM_U32BE; break;
			case AVCodecLibrary.CODEC_ID_PCM_U32LE: this.anidbcodecid = AnidbCodec.PCM_U32LE; break;
			case AVCodecLibrary.CODEC_ID_PCM_MULAW: this.anidbcodecid = AnidbCodec.PCM_MULAW; break;
			case AVCodecLibrary.CODEC_ID_PCM_ZORK: this.anidbcodecid = AnidbCodec.PCM_ZORK; break;
			default: this.anidbcodecid = AnidbCodec.OTHER; break;
		}
	}
	
	/**
	 * Get the Audio Type
	 * @return Audio type name
	 */
	public synchronized String mapAudioType() {
		switch (this.audioType) {
			case 10: return "normal";
			case 20: return "commentary";
			case 30: return "fandub";
			case 40: return "alt. voiceover";
			case 100: return "other";
			default: return "unknown";
		}
	}
	
	/**
	 * Convert audio channels to anidb representation
	 * @return Audio channels representation
	 */
	public synchronized String mapAudioChannels() {
		switch(this.channels) {
			case 0: return "unknown"; 
			case 1: return "mono";
			case 2: return "stereo";
			case 3: return "2.1";
			case 4: return "4.0";
			case 6: return "5.1";
			case 7: return "6.1";
			case 8: return "7.1";
			default: return "other";
		}
	}

	/**
	 * Writes the Stream to a string.
	 * <p>Uses the same format as writeToFile()</p>
	 * @see writeToFile
	 */
	public synchronized String writeToString() {
		StringBuffer out = new StringBuffer();
		out.append("\tstream [audio:"+this.relStreamId+"]:"+'\n');
		out.append("\t\ttype: "+mapAudioType()+'\n');
		out.append("\t\tcodec: "+this.anidbcodecid.getCodecString()+'\n');
		if (this.fullParsed && this.size > 0) out.append("\t\tsize: "+this.size+" bytes"+'\n');
		if (this.fullParsed && this.duration > 0) out.append("\t\tduration: "+Utils.formatDurationSecs(this.duration)+" ("+this.duration+")"+'\n');
		if (this.fullParsed && this.bitrate > 0) out.append("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps"+'\n');
		if (this.language != "") out.append("\t\tlanguage: "+this.language+'\n');
		out.append("\t\tchannels: "+this.channels+" ("+mapAudioChannels()+")"+'\n');
		if (this.sampleRate > 0) out.append("\t\tsample rate: "+this.sampleRate+ " Hz"+'\n');
		if (this.sampleFormat != "") out.append("\t\tsample format: "+this.sampleFormat+'\n');
		if (this.fullParsed && this.isVBR) out.append("\t\tvariable bit rate"+'\n');
		return out.toString();
	}
	
	/**
	 * Writes the Stream to a file
	 * @param out PrintStream where to output the stream
	 */
	public synchronized void writeToFile(PrintStream out) {
		out.println("\tstream [audio:"+this.relStreamId+"]:");
		out.println("\t\ttype: "+mapAudioType());
		out.println("\t\tcodec: "+this.anidbcodecid.getCodecString());
		if (this.fullParsed && this.size > 0) out.println("\t\tsize: "+this.size+" bytes");
		if (this.fullParsed && this.duration > 0) out.println("\t\tduration: "+Utils.formatDurationSecs(this.duration)+" ("+this.duration+")");
		if (this.fullParsed && this.bitrate > 0) out.println("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps");
		if (this.language != "") out.println("\t\tlanguage: "+this.language);
		out.println("\t\tchannels: "+this.channels+" ("+mapAudioChannels()+")");
		if (this.sampleRate > 0) out.println("\t\tsample rate: "+this.sampleRate+ " Hz");
		if (this.sampleFormat != "") out.println("\t\tsample format: "+this.sampleFormat);
		if (this.fullParsed && this.isVBR) out.println("\t\tvariable bit rate");
	}
	
	/**
	 * Creates a xml object of the audio stream
	 * @return the xml object
	 * @throws XmlObjectException
	 * @throws UnsupportedEncodingException
	 */
	public synchronized XmlObject getXmlObject() throws XmlObjectException, UnsupportedEncodingException {
		XmlObject xml = new XmlObject("stream");
		if (this.size > 0) xml.addValue(new XmlObject("size",""+this.size));
		if (this.duration > 0) xml.addValue(new XmlObject("len",""+(int)this.duration));
		if (this.bitrate > 0) xml.addValue(new XmlObject("br",""+(int)this.bitrate));
		xml.addValue(new XmlObject("type",mapAudioType()));
		if (this.language != "") xml.addValue(new XmlObject("lang",this.language));
		xml.addValue(new XmlObject("codec",this.anidbcodecid.getAnidbName()));
		xml.addValue(new XmlObject("chan",mapAudioChannels()));
		if (this.sampleRate > 0) xml.addValue(new XmlObject("sr",""+this.sampleRate));
		if (this.sampleFormat != "") xml.addValue(new XmlObject("sf",this.sampleFormat));
		return xml;
	}
}
