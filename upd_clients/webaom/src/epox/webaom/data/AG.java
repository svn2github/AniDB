/*
 * Created on 27.feb.2006 12:44:44
 * Filename: aAGroup.java
 */
package epox.webaom.data;

public class AG extends Base{
	private Group m_g;
	private Anime m_a;
	public AG(Anime a, Group g){
		this.id = g.id;
		m_g = g;
		m_a = a;
	}
	/*public Object getKey(){
		return m_g.getKey();
	}*/
	public String toString(){
		return m_g.name+" ("+m_g.sname+")";
	}
	public int getPct(){
		int max = m_a.eps;
		if(max==0) max = -m_a.lep;
		if(max==0) return 0;
		return (size()*100)/max;
	}
}
