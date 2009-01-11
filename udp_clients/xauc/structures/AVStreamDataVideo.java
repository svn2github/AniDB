package structures;

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

import net.sf.ffmpeg_java.AVCodecLibrary;

import xml.XmlObject;
import xml.XmlObjectException;

/**
 * Video stream data wrapper
 */
public class AVStreamDataVideo extends AVStreamData {
	/** Video Aspect Ratio Tolerance */
	protected static final double AR_TOLERANCE = 0.01;
	
	/** Picture format */
	public String pictureFormat = "";
	/** Picture Pixel Width */
	public int p_width = 0;
	/** Picture Pixel Height */
	public int p_height = 0;
	/** Picture Display Width (Anamorphic videos) */
	public int d_width = -1;
	/** Picture Display Height (Anamorphic videos) */
	public int d_height = -1;
	/** Picture Pixel Resolution */
	public String p_resolution = "";
	/** Picture Display Resolution */
	public String d_resolution = "";
	/** Frames Per Second */
	public double fps = 0;
	/** Picture Pixel Aspect Ratio */
	public double p_ar = 0;
	/** Picutre Display Aspect Ratio (Anamorphic videos)*/
	public double d_ar = -1;
	/** True if stream is anamorphic */
	public boolean isAnamorphic = false;
	/** True if stream has a wrong Aspect Ratio */
	public boolean isWrongAR = false;
	/** True if stream is clean (no hard subs) */
	public boolean isCleanVideo = false;
	// ANIDB data
	/** AnidbCodec Id use AnidbCodec getters for the rest */
	public AnidbCodec anidbcodecid = AnidbCodec.UNKNOWN;
	/** Codec mappings for anidb */
	public enum AnidbCodec {
		OTHER (1,"unknown","unk","other"),
		UNKNOWN (1,"unknown","unk","unknown"),
		DX00 (2,"DivX UNK","divxu","mpeg4-unk"),
		DX30 (3,"DivX3","divx3","mpeg4-dx30"),
		DX31 (3,"DivX3","divx3","mpeg4-dx31"),
		DX40 (5,"DivX4","divx4","mpeg4-dx40"),
		DX50 (7,"DivX5/6","divx5","mpeg4-dx50"),
		DX60 (7,"DivX5/6","divx5","mpeg4-dx60"),
		MPEG1 (9,"MPEG-1","mpeg1","mpeg1video"),
		MPEG2 (10,"MPEG-2","mpeg2","mpeg2video"),
		MPEG4ASP (11,"Other (MPEG-4 ASP)","asp","mpeg4-asp"),
		MPEG4SP (12,"Other (MPEG-4 SP)","other","mpeg4-sp"),
		RV (14,"RealVideo Other (RV1x-RV3x)","rvo","rv10-rv30"),
		RV10 (14,"RealVideo Other (RV1x-RV3x)","rvo","rv10"),
		RV20 (14,"RealVideo Other (RV1x-RV3x)","rvo","rv20"),
		RV30 (14,"RealVideo Other (RV1x-RV3x)","rvo","rv30"),
		XVID (17,"XviD","xvid","mpeg4-xvid"),
		MSMPEG4 (18,"MS MP4x (also WMV1/2)","msmp4x","msmpeg4"),
		WMV1 (18,"MS MP4x (also WMV1/2)","msmp4x","msmpeg4-wmv1"),
		WMV2 (18,"MS MP4x (also WMV1/2)","msmp4x","msmpeg4-wmv2"),
		MSMPEG4V1 (18,"MS MP4x (also WMV1/2)","msmp4x","msmpeg4-v1"),
		MSMPEG4V2 (18,"MS MP4x (also WMV1/2)","msmp4x","msmpeg4-v2"),
		MSMPEG4V3 (18,"MS MP4x (also WMV1/2)","msmp4x","msmpeg4-v3"),
		WMV3 (19,"WMV9 (also WMV3/VC1)","wmv9","msvc1-wmv3"),
		VC1 (19,"WMV9 (also WMV3/VC1)","wmv9","msvc1-wvc1"),
		RV40 (20,"RealVideo 9/10 (RV40)","rv40","rv40"),
		AVC (22,"H264/AVC","avc","avc");
		
		private final int id;
		private final String name;
		private final String shortname;
		private final String codecname;
		AnidbCodec (int id, String name, String shortname, String codecname) {
			this.id = id;
			this.name = name;
			this.shortname = shortname;
			this.codecname = codecname;
		}
		public int getAnidbID() { return id; };
		public String getAnidbName() { return name; };
		public String getAnidbShortName() { return shortname; }
		public String getCodecName() { return codecname; }
		public String getCodecString() { return name+" ("+codecname+")"; }
	}
	
	/** Default Video Stream Data constructor */ 
	public AVStreamDataVideo() { super(); }

	/**
	 * Gets the integer with the video flags
	 * @return flags
	 */
	public synchronized int getVideoFlags() {
		int flags = 0;
		if (this.isAnamorphic) flags += 1;
		if (this.isWrongAR) flags += 2;
		if (this.isVFR) flags += 4;
		if (this.isCleanVideo) flags += 8;
		return flags;
	}
	
	/**
	 * Gets the Aspect Ratio string
	 * @param ar aspect ratio value (width/height)
	 * @return Aspect Ratio
	 */
	public synchronized String getAspectRatio(double ar) {
		if (Math.abs(ar - 1.33) < AVStreamDataVideo.AR_TOLERANCE) return "4:3";
		else if (Math.abs(ar - 1.77) < AVStreamDataVideo.AR_TOLERANCE) return "16:9";
		else if (Math.abs(ar - 1.66) < AVStreamDataVideo.AR_TOLERANCE) return "1.66:1";
		else if (Math.abs(ar - 1.85) < AVStreamDataVideo.AR_TOLERANCE) return "1.85:1";
		else if (Math.abs(ar - 2.00) < AVStreamDataVideo.AR_TOLERANCE) return "2.00:1";
		else if (Math.abs(ar - 2.21) < AVStreamDataVideo.AR_TOLERANCE) return "2.21:1";
		else if (Math.abs(ar - 2.35) < AVStreamDataVideo.AR_TOLERANCE) return "2.35:1";
		else return "other ("+String.format("%1.2f", ar)+":1)";
	}

	/**
	 * Gets the Aspect Ratio string
	 * @param width Picture width
	 * @param height Picture height
	 * @return Aspect Ratio
	 */
	public synchronized String getAspectRatio(int width, int height) {
		return getAspectRatio((double)width/(double)height);
	}
	
	/**
	 * Gets a Picture resolution string
	 * @param width Picture width
	 * @param height Picture height
	 * @return Resolution
	 */
	public synchronized String getResolution(int width, int height) {
		return width+"x"+height;
	}
	
	/**
	 * Sets Anidb's Video codec ids
	 * @param codec_id AVCodecLibrary.CODEC_ID_* identifier
	 */
	public synchronized void setAniDBVideoCodec(int codec_id, String codec_tag) {
		codec_tag = codec_tag.toUpperCase();
		switch (codec_id) {
			case AVCodecLibrary.CODEC_ID_H264: this.anidbcodecid = AnidbCodec.AVC; break;
			case AVCodecLibrary.CODEC_ID_MPEG1VIDEO: this.anidbcodecid = AnidbCodec.MPEG1; break;
			case AVCodecLibrary.CODEC_ID_MPEG2VIDEO:
			case AVCodecLibrary.CODEC_ID_MPEG2VIDEO_XVMC: this.anidbcodecid = AnidbCodec.MPEG2; break;
			case AVCodecLibrary.CODEC_ID_MPEG4: // the mother of all avi codecs
				if (codec_tag.equals("DX50")) this.anidbcodecid = AnidbCodec.DX50;
				else if (codec_tag.equals("DX60")) this.anidbcodecid = AnidbCodec.DX60;
				else if (codec_tag.equals("DIV3")) this.anidbcodecid = AnidbCodec.DX30;
				else if (codec_tag.equals("DIV4")) this.anidbcodecid = AnidbCodec.DX31;
				else if (codec_tag.equals("DIVX")) this.anidbcodecid = AnidbCodec.DX40;
				else if (codec_tag.equals("XVID")) this.anidbcodecid = AnidbCodec.XVID;
				else if (codec_tag.equals("FMP4")) this.anidbcodecid = AnidbCodec.MPEG4ASP;
				else if (codec_tag.equals("MP41")) this.anidbcodecid = AnidbCodec.MSMPEG4V1;
				else if (codec_tag.equals("MP42")) this.anidbcodecid = AnidbCodec.MSMPEG4V2;
				else if (codec_tag.equals("MP43")) this.anidbcodecid = AnidbCodec.MSMPEG4V3;
				else if (codec_tag.equals("WMV7")) this.anidbcodecid = AnidbCodec.WMV1;
				else if (codec_tag.equals("WMV8")) this.anidbcodecid = AnidbCodec.WMV2;
				else this.anidbcodecid = AnidbCodec.MPEG4SP;
				break;
			case AVCodecLibrary.CODEC_ID_RV10: this.anidbcodecid = AnidbCodec.RV10; break;
			case AVCodecLibrary.CODEC_ID_RV20: this.anidbcodecid = AnidbCodec.RV20; break;
			case AVCodecLibrary.CODEC_ID_RV30: this.anidbcodecid = AnidbCodec.RV30; break;
			case AVCodecLibrary.CODEC_ID_MSMPEG4V1: this.anidbcodecid = AnidbCodec.MSMPEG4V1; break;
			case AVCodecLibrary.CODEC_ID_MSMPEG4V2: this.anidbcodecid = AnidbCodec.MSMPEG4V2; break;
			case AVCodecLibrary.CODEC_ID_MSMPEG4V3: this.anidbcodecid = AnidbCodec.MSMPEG4V3; break;
			case AVCodecLibrary.CODEC_ID_WMV1: this.anidbcodecid = AnidbCodec.WMV1; break;
			case AVCodecLibrary.CODEC_ID_WMV2: this.anidbcodecid = AnidbCodec.WMV1; break;
			case AVCodecLibrary.CODEC_ID_VC1: this.anidbcodecid = AnidbCodec.VC1; break;
			case AVCodecLibrary.CODEC_ID_WMV3: this.anidbcodecid = AnidbCodec.WMV3; break;
			default: this.anidbcodecid = AnidbCodec.OTHER; break;
		}
	}
	
	/**
	 * Writes the Stream to a string.
	 * <p>Uses the same format as writeToFile()</p>
	 * @see writeToFile
	 */
	public synchronized String writeToString() {
		StringBuffer out = new StringBuffer();
		out.append("\tstream [video:"+this.relStreamId+"]:"+'\n');
		out.append("\t\tcodec: "+this.anidbcodecid.getCodecString()+'\n');
		if (this.fullParsed && this.size > 0) out.append("\t\tsize: "+this.size+" bytes"+'\n');
		if (this.fullParsed && this.duration > 0) out.append("\t\tduration: "+formatDurationSecs(this.duration)+" ("+this.duration+")"+'\n');
		if (this.fullParsed && this.bitrate > 0) out.append("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps"+'\n');
		if (!this.p_resolution.equals("")) out.append("\t\tpixel resolution: "+this.p_resolution+'\n');
		if (this.p_ar > 0) out.append("\t\tpixel aspect ratio: "+getAspectRatio(this.p_ar)+'\n');
		if (!this.d_resolution.equals("")) out.append("\t\tdisplay resolution: "+this.d_resolution+'\n');
		if (this.d_ar > 0) out.append("\t\tdisplay aspect ratio: "+getAspectRatio(this.d_ar)+'\n');
		if (this.fps > 0) out.append(String.format("\t\tfps: %2.3f\n",this.fps));
		if (this.pictureFormat != "") out.append("\t\tpicture format: "+this.pictureFormat+'\n');
		if (this.isAnamorphic) out.append("\t\tanamorphic"+'\n');
		if (this.isWrongAR) out.append("\t\twrong aspect ratio"+'\n');
		if (this.fullParsed && this.isVFR) out.append("\t\tvariable frame rate"+'\n');
		if (this.isCleanVideo) out.append("\t\tclean video (no hardsubs...)"+'\n');
		return out.toString();
	}
	
	/**
	 * Writes the Stream to a file
	 * @param out PrintStream where to output the stream
	 */
	public synchronized void writeToFile(PrintStream out) {
		out.println("\tstream [video"+this.relStreamId+"]:");
		out.println("\t\tcodec: "+this.anidbcodecid.getCodecString());
		if (this.fullParsed && this.size > 0) out.println("\t\tsize: "+this.size+" bytes");
		if (this.fullParsed && this.duration > 0) out.println("\t\tduration: "+formatDurationSecs(this.duration)+" ("+this.duration+")");
		if (this.fullParsed && this.bitrate > 0) out.println("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps");
		if (!this.p_resolution.equals("")) out.println("\t\tpixel resolution: "+this.p_resolution);
		if (this.p_ar > 0) out.println("\t\tpixel aspect ratio: "+getAspectRatio(this.p_ar));
		if (!this.d_resolution.equals("")) out.println("\t\tdisplay resolution: "+this.d_resolution);
		if (this.d_ar > 0) out.println("\t\tdisplay aspect ratio: "+getAspectRatio(this.d_ar));
		if (this.fps > 0) out.printf("\t\tfps: %2.3f\n",this.fps);
		if (this.pictureFormat != "") out.println("\t\tpicture format: "+this.pictureFormat);
		if (this.isAnamorphic) out.println("\t\tanamorphic");
		if (this.isWrongAR) out.println("\t\twrong aspect ratio");
		if (this.fullParsed && this.isVFR) out.println("\t\tvariable frame rate");
		if (this.isCleanVideo) out.println("\t\tclean video (no hardsubs...)");
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
		if (this.duration > 0) xml.addValue(new XmlObject("len",""+(int)(this.duration*this.timebase)));
		if (this.bitrate > 0) xml.addValue(new XmlObject("br",""+(int)this.bitrate));
		xml.addValue(new XmlObject("res",this.p_resolution));
		xml.addValue(new XmlObject("codec",this.anidbcodecid.getAnidbName()));
		if (this.fps > 0) xml.addValue(new XmlObject("fps",String.format("%2.3f",this.fps)));
		if (this.p_ar > 0) xml.addValue(new XmlObject("ar",getAspectRatio(this.p_ar)));
		if (this.pictureFormat != "") xml.addValue(new XmlObject("pf",this.pictureFormat));
		int flags = getVideoFlags();
		if (flags > 0) xml.addValue(new XmlObject("flags",""+flags));
		return xml;
	}
}
