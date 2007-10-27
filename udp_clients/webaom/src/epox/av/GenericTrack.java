/*
 * Created on 23.mai.2006 14:14:47
 * Filename: GenericTrack.java
 */
package epox.av;

public class GenericTrack{
	public int num, type_id, codec_id;
	public String codec, codecExtra;
	public String lang, name;
	public double size;
	public float dur;
	public boolean def;

	public GenericTrack(){
		//
	}
	public GenericTrack(GenericTrack gt){
		this.num = gt.num;
		this.type_id = gt.type_id;
		this.codec_id = gt.codec_id;
		this.codec = gt.codec;
		this.codecExtra = gt.codecExtra;
		this.lang = gt.lang;
		this.name = gt.name;
		this.size = gt.size;
		this.dur = gt.dur;
		this.def = gt.def;
	}
}