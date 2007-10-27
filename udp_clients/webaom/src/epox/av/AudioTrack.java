/*
 * Created on 23.mai.2006 14:52:58
 * Filename: Audio.java
 */
package epox.av;

public class AudioTrack extends GenericTrack{
	public int channels, samplerate;
	public AudioTrack(GenericTrack gt){
		super(gt);
	}
}
