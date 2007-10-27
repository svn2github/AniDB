/*
 * Created on 23.mai.2006 14:52:46
 * Filename: VideoTrack.java
 */
package epox.av;

public class VideoTrack extends GenericTrack{
	public int pixel_width, pixel_height, display_width, display_height, fps;
	public boolean vfr;
	public VideoTrack(GenericTrack gt){
		super(gt);
	}
}
