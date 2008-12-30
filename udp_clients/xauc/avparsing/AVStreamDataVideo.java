package avparsing;

/**
 * Video stream data wrapper
 */
public class AVStreamDataVideo extends AVStreamData {
	public String pictureFormat = "";
	public int width = 0;
	public int height = 0;
	public String resolution = "";
	public double fps = 0;
	public double ar = 0;
	public boolean isVFR = false;
	public boolean isAnamorphic = false;
	public boolean isWrongAR = false;
	public boolean isCleanVideo = false;
	public AVStreamDataVideo() { super(); }
}
