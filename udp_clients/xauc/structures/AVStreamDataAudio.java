package structures;

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

import net.sf.ffmpeg_java.AVCodecLibrary;

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

	/** Default AVStreamDataAudio constructor */
	public AVStreamDataAudio() { super(); }

	public String[] getAniDBAudioCodec(int codec_id, String codec_tag) {
		codec_tag = codec_tag.toUpperCase();
		String[] result = null;// shortname, name, id
		switch (codec_id) {
			case AVCodecLibrary.CODEC_ID_AAC: result = new String[]{"aac","AAC","9"}; break;
			case AVCodecLibrary.CODEC_ID_AC3: result = new String[]{"ac3","AC3","2"}; break;
			case AVCodecLibrary.CODEC_ID_DTS: result = new String[]{"dts","DTS","13"}; break;
			case AVCodecLibrary.CODEC_ID_MP2: result = new String[]{"mp2","MP2","11"}; break;
			case AVCodecLibrary.CODEC_ID_MP3:
				if (this.fullParsed) {
					if (!this.isVBR) result = new String[]{"mp3cbr","MP3 CBR","5"};
					else result = new String[]{"mp3vbr","MP3 VBR","6"};
				} else result = new String[]{"mp3","MP3","4"};
				break;
			case AVCodecLibrary.CODEC_ID_VORBIS:  result = new String[]{"vorbis","Vorbis (Ogg Vorbis)","10"}; break;
			case AVCodecLibrary.CODEC_ID_WMAV1:
			case AVCodecLibrary.CODEC_ID_WMAV2:
			case AVCodecLibrary.CODEC_ID_WMAVOICE:
			case AVCodecLibrary.CODEC_ID_WMAPRO:
			case AVCodecLibrary.CODEC_ID_WMALOSSLESS:
				result = new String[]{"wma","WMA (also DivX Audio)","3"}; break;
			case AVCodecLibrary.CODEC_ID_PCM_ALAW:
			case AVCodecLibrary.CODEC_ID_PCM_DVD:
			case AVCodecLibrary.CODEC_ID_PCM_S8:
			case AVCodecLibrary.CODEC_ID_PCM_S16BE:
			case AVCodecLibrary.CODEC_ID_PCM_S16LE:
			case AVCodecLibrary.CODEC_ID_PCM_S16LE_PLANAR:
			case AVCodecLibrary.CODEC_ID_PCM_S24BE:
			case AVCodecLibrary.CODEC_ID_PCM_S24DAUD:
			case AVCodecLibrary.CODEC_ID_PCM_S24LE:
			case AVCodecLibrary.CODEC_ID_PCM_F32BE:
			case AVCodecLibrary.CODEC_ID_PCM_F32LE:
			case AVCodecLibrary.CODEC_ID_PCM_F64BE:
			case AVCodecLibrary.CODEC_ID_PCM_F64LE:
			case AVCodecLibrary.CODEC_ID_PCM_U8:
			case AVCodecLibrary.CODEC_ID_PCM_U16BE:
			case AVCodecLibrary.CODEC_ID_PCM_U16LE:
			case AVCodecLibrary.CODEC_ID_PCM_U24BE:
			case AVCodecLibrary.CODEC_ID_PCM_U24LE:
			case AVCodecLibrary.CODEC_ID_PCM_U32BE:
			case AVCodecLibrary.CODEC_ID_PCM_U32LE:
			case AVCodecLibrary.CODEC_ID_PCM_MULAW:
			case AVCodecLibrary.CODEC_ID_PCM_ZORK:
				result = new String[]{"pcm","PCM","10"}; break;
			default: result = new String[]{"unk","unknown","1"}; 
		}
		return result;
	}
	
	/**
	 * Get the Audio Type
	 * @return
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
	 * @return
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
		out.append("\t\tcodec: "+this.codecName+(!this.codecTag.equals("") ? " ["+this.codecTag+"]" : "")+(!this.codecNameLong.equals("") ? " ("+this.codecNameLong+")" : "")+'\n');
		if (this.fullParsed && this.size > 0) out.append("\t\tsize: "+this.size+" bytes"+'\n');
		if (this.fullParsed && this.duration > 0) out.append("\t\tduration: "+formatDurationSecs(this.duration)+" ("+this.duration+")"+'\n');
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
		out.println("\t\tcodec: "+this.codecName+(!this.codecTag.equals("") ? " ["+this.codecTag+"]" : "")+(!this.codecNameLong.equals("") ? " ("+this.codecNameLong+")" : ""));
		if (this.fullParsed && this.size > 0) out.println("\t\tsize: "+this.size+" bytes");
		if (this.fullParsed && this.duration > 0) out.println("\t\tduration: "+formatDurationSecs(this.duration)+" ("+this.duration+")");
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
		xml.addValue(new XmlObject("codec",this.codecName));
		xml.addValue(new XmlObject("chan",mapAudioChannels()));
		if (this.sampleRate > 0) xml.addValue(new XmlObject("sr",""+this.sampleRate));
		if (this.sampleFormat != "") xml.addValue(new XmlObject("sf",this.sampleFormat));
		return xml;
	}
}
