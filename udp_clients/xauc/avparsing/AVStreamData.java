package avparsing;

/**
 * AV Stream Wrapper
 */
public class AVStreamData {
	public int size = 0; // in bytes
	public int duration = 0; // in seconds
	public String formatedDuration = "";
	public double bitrate = 0;
	public double timebase = 0;
	public String codecName = "";
	public String codecTag = "";
	public int type = -1;
	public String typeName = "unknown";
	public int streamId = 0;
	public AVStreamData() {}
}
