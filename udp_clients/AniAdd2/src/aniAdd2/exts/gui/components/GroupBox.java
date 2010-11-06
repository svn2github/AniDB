/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.gui.components;

import java.awt.Component;
import java.awt.Graphics;
import java.awt.Insets;
import javax.swing.GroupLayout;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.UIManager;
import javax.swing.border.AbstractBorder;
import javax.swing.border.Border;

/**
 *
 * @author Arokh
 */
public class GroupBox extends JPanel {
	Border border;
	JComponent jc_Header;
	JPanel pnl_Header = new JPanel();
	JPanel pnl_Content = new JPanel();

	public GroupBox(JComponent jc_header) {
		this.jc_Header = jc_header;

		pnl_Header.setOpaque(false);
		pnl_Content.setOpaque(false);

		pnl_Content.setBorder(new AbstractBorder() {
			@Override
			public Insets getBorderInsets(Component c) {
				Insets insets = getBorder().getBorderInsets(c);
				insets.top = 0;
				return insets;
			}

			@Override
			public Insets getBorderInsets(Component c, Insets insets) {
				return getBorderInsets(c);
			}
		});

		GroupLayout pnl_HeaderLayout = new GroupLayout(pnl_Header);
		pnl_Header.setLayout(pnl_HeaderLayout);
		pnl_HeaderLayout.setHorizontalGroup(
			pnl_HeaderLayout.createParallelGroup()
			.addGroup(pnl_HeaderLayout.createSequentialGroup()
				.addGap(4)
				.addComponent(jc_header))
		);
		pnl_HeaderLayout.setVerticalGroup(
			pnl_HeaderLayout.createParallelGroup()
			.addComponent(jc_header)
		);

		GroupLayout layout = new GroupLayout(this);
		layout.setHorizontalGroup(
			layout.createParallelGroup()
			.addComponent(pnl_Header, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
			.addComponent(pnl_Content, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
		);
		layout.setVerticalGroup(
			layout.createParallelGroup()
			.addGroup(layout.createSequentialGroup()
				.addComponent(pnl_Header, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
				.addComponent(pnl_Content, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
		);

		setLayout(layout);
	}

	public JPanel getContantPane(){
		return pnl_Content;
	}

	@Override
	public Border getBorder()       {
		return border == null ? UIManager.getBorder("TitledBorder.border") : border;
	}

	@Override
	public void setBorder(Border border) {
		this.border = border;
	}

	@Override
	protected void paintBorder(Graphics g) {
		int margin = jc_Header.getHeight() / 2;

		getBorder().paintBorder(this, g, 0, margin, getWidth(), getHeight() - margin);
		g.copyArea(20, 20, jc_Header.getWidth(), jc_Header.getHeight(), -20 + 4, -20); //TODO
	}
}
