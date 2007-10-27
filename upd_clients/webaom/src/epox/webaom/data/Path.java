/*
 * Created on 28.feb.2006 19:11:21
 * Filename: Path.java
 */
package epox.webaom.data;

public class Path extends Base{
	private String mSp;

	public Path(String p){
		mSp = p;
	}
	public Object getKey(){
		return mSp;
	}
	public String toString(){
		return mSp;
	}
}
