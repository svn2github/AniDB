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
	/** Video Aspect Ratio Tolerance */
	protected static final double AR_TOLERANCE = 0.01; 
	
	
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
	
	public String[] getAniDBVideoCodec(int codec_id, String codec_tag) {
		codec_tag = codec_tag.toUpperCase();
		String[] result = null;// shortname, name, id
		switch (codec_id) {
			case AVCodecLibrary.CODEC_ID_H264: result = new String[]{"avc","H264/AVC","22"}; break;
			case AVCodecLibrary.CODEC_ID_MPEG1VIDEO: result = new String[]{"mpeg1","MPEG-1","9"}; break;
			case AVCodecLibrary.CODEC_ID_MPEG2VIDEO:
			case AVCodecLibrary.CODEC_ID_MPEG2VIDEO_XVMC: result = new String[]{"mpeg2","MPEG-2","10"}; break;
			case AVCodecLibrary.CODEC_ID_MPEG4: // the mother of all avi codecs
				if (codec_tag.equals("DX50") || codec_tag.equals("DX60")) result = new String[]{"divx5","DivX5/6","7"};
				else if (codec_tag.equals("DIV3") || codec_tag.equals("DIV4")) result = new String[]{"divx3","DivX3","3"};
				else if (codec_tag.equals("DIVX")) result = new String[]{"divx4","DivX 4","5"};
				else if (codec_tag.equals("XVID")) result = new String[]{"xvid","XviD","3"};
				else if (codec_tag.equals("FMP4")) result = new String[]{"asp","Other (MPEG-4 ASP)","4"};
				else if (codec_tag.contains("MP4")) result = new String[]{"msmp4x","MS MP4x (also WMV1/2)","11"};
				else result = new String[]{"other","Other (MPEG-4 non-ASP)","15"};
				break;
			case AVCodecLibrary.CODEC_ID_RV10:
			case AVCodecLibrary.CODEC_ID_RV20:
			case AVCodecLibrary.CODEC_ID_RV30: result = new String[]{"rvo","RealVideo Other (6,G2,8)","14"}; break;
			case AVCodecLibrary.CODEC_ID_MSMPEG4V1:
			case AVCodecLibrary.CODEC_ID_MSMPEG4V2:
			case AVCodecLibrary.CODEC_ID_MSMPEG4V3:
			case AVCodecLibrary.CODEC_ID_WMV1:
			case AVCodecLibrary.CODEC_ID_WMV2: result = new String[]{"msmp4x","MS MP4x (also WMV1/2)","11"}; break;
			case AVCodecLibrary.CODEC_ID_VC1:
			case AVCodecLibrary.CODEC_ID_WMV3: result = new String[]{"wmv9","WMV9 (also WMV3/VC1)","19"}; break;
			default: result = new String[]{"unk","unknown","1"}; 
		}
		return result;
	}
	
	/**
	 * Writes the Stream to a string.
	 * <p>Uses the same format as writeToFile()</p>
	 * @see writeToFile
	 */
	public synchronized String writeToString() {
		StringBuffer out = new StringBuffer();
//		AVParser parser = new AVParser();
//		String[] codecInfo = parser.getCodecName(this.codec_id);
//		this.codecName = codecInfo[0];
//		this.codecNameLong = codecInfo[1];
		out.append("\tstream [video:"+this.relStreamId+"]:"+'\n');
		out.append("\t\tcodec: "+this.codecName+(!this.codecTag.equals("") ? " ["+this.codecTag+"]" : "")+(!this.codecNameLong.equals("") ? " ("+this.codecNameLong+")" : "")+'\n');
		//out.append("\t\tcodec: "+this.anidbCodec[1]+'\n');//+" ("+this.codecName+(!this.codecTag.equals("") ? " ["+this.codecTag+"]" : "")+")"+'\n');
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
//		AVParser parser = new AVParser();
//		String[] codecInfo = parser.getCodecName(this.codec_id);
//		this.codecName = codecInfo[0];
//		this.codecNameLong = codecInfo[1];
		out.println("\tstream [video"+this.relStreamId+"]:");
		out.println("\t\tcodec: "+this.codecName+(!this.codecTag.equals("") ? " ["+this.codecTag+"]" : "")+(!this.codecNameLong.equals("") ? " ("+this.codecNameLong+")" : ""));
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
		xml.addValue(new XmlObject("codec",this.codecName));
		if (this.fps > 0) xml.addValue(new XmlObject("fps",String.format("%2.3f",this.fps)));
		if (this.p_ar > 0) xml.addValue(new XmlObject("ar",getAspectRatio(this.p_ar)));
		if (this.pictureFormat != "") xml.addValue(new XmlObject("pf",this.pictureFormat));
		int flags = getVideoFlags();
		if (flags > 0) xml.addValue(new XmlObject("flags",""+flags));
		return xml;
	}
}
