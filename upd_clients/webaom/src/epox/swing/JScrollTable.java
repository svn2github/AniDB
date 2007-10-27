/*
 * Created on 28.mai.2006 11:14:54
 * Filename: JScrollTable.java
 */
package epox.swing;

import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JScrollPane;
import javax.swing.JTable;

public class JScrollTable extends JScrollPane implements MouseListener {
	private JTable m_jt;
	public JScrollTable(JTable jt){
		super(jt);
		m_jt = jt;
		getViewport().setBackground(java.awt.Color.white);

		setFocusable(true);
		addMouseListener(this);
	}
	public int getTopVisibleRow() {
		return m_jt.rowAtPoint(getViewport().getViewPosition());
	}
	public int getBottomVisibleRow() {
		int y = getViewport().getViewPosition().y;
		y += getViewport().getExtentSize().getHeight();
		int row = m_jt.rowAtPoint(new Point(0,y));
		return (row > -1) ? row : (m_jt.getRowCount() - 1);
	}
	public void mouseClicked(MouseEvent e){
		requestFocusInWindow();
	}
	public void mousePressed(MouseEvent e){
		//
	}
	public void mouseReleased(MouseEvent e){
		//
	}
	public void mouseEntered(MouseEvent e){
		//
	}
	public void mouseExited(MouseEvent e){
		//
	}
}
