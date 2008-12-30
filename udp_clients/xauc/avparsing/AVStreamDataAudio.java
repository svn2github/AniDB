package avparsing;

/**
 * Audio stream data wrapper
 */
public class AVStreamDataAudio extends AVStreamData {
	public String language = "";
	public int sampleRate = 0; // in Hz
	public String sampleFormat = "";
	public int channels = 0;
	public String layout = "";
	public int audioType = 0;
	public AVStreamDataAudio() { super(); }
}
