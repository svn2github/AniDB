/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.gui.components;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import javax.swing.AbstractAction;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
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

	public GroupBox() {
		super(null);
	}



	public GroupBox(JComponent jc_header) {
		this.jc_Header = jc_header;

		pnl_Header.setOpaque(false);
		pnl_Content.setOpaque(false);

		pnl_Content.setBorder(new AbstractBorder() {
			@Override
			public Insets getBorderInsets(Component c) {
				return new Insets(6, 6, 6, 6);

				//Insets insets = getBorder().getBorderInsets(c);
				//insets.top = 0;
				//return insets;
			}

			@Override
			public Insets getBorderInsets(Component c, Insets insets) {
				return getBorderInsets(c);
			}
		});

		if(jc_header != null) {
			GroupLayout pnl_HeaderLayout = new GroupLayout(pnl_Header);
			pnl_Header.setLayout(pnl_HeaderLayout);
			pnl_HeaderLayout.setHorizontalGroup(
				pnl_HeaderLayout.createParallelGroup()
				.addGroup(pnl_HeaderLayout.createSequentialGroup()
					.addGap(8)
					.addComponent(jc_header, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
			);
			pnl_HeaderLayout.setVerticalGroup(
				pnl_HeaderLayout.createParallelGroup()
				.addComponent(jc_header, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
			);
		}

		GroupLayout layout = new GroupLayout(this);
		layout.setHorizontalGroup(
			layout.createParallelGroup()
			.addComponent(pnl_Header, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
			.addComponent(pnl_Content, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE)
		);
		layout.setVerticalGroup(
			layout.createParallelGroup()
			.addGroup(layout.createSequentialGroup()
				.addComponent(pnl_Header, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
				.addComponent(pnl_Content, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE))
		);

		setLayout(layout);
	}

	public JPanel getContentPane(){
		return pnl_Content;
	}

	@Override
	public Border getBorder() {
		return border == null ? UIManager.getBorder("TitledBorder.border") : border;
	}

	@Override
	public void setBorder(Border border) {
		this.border = border;
	}

	@Override
	protected void paintBorder(Graphics g) {
		int margin = jc_Header == null? 0 : jc_Header.getHeight() / 2;

		getBorder().paintBorder(this, g, 2, margin, getWidth() - 4, getHeight() - margin);
		//if(jc_Header != null) g.copyArea(20, 20, jc_Header.getWidth(), jc_Header.getHeight(), -20 + 8, -20); //TODO
	}
	
	public static GroupBox createExpander(JComponent jc_header) {
		JPanel pnl_Header = new JPanel(new BorderLayout(4, 0));
		JButton btn_expander = new JButton();
		btn_expander.setIcon(new ImageIcon(GroupBox.class.getResource("/aniAdd2/exts/gui/exts/processing/icons/FGreen.png")));


		//btn_expander.setBorder(BorderFactory.createEmptyBorder());
		btn_expander.setMargin(new Insets(0, 0, 0, 0));

		Dimension d = jc_header.getPreferredSize();
		d.width = d.height;

		//btn_expander.setPreferredSize(d);

		pnl_Header.add(btn_expander, BorderLayout.WEST);
		pnl_Header.add(jc_header, BorderLayout.CENTER);

		
		final GroupBox groupBox = new GroupBox(pnl_Header);
		btn_expander.setAction(new AbstractAction() {
			public void actionPerformed(ActionEvent e) {
				groupBox.getContentPane().setVisible(!groupBox.getContentPane().isVisible());
			}
		});

		pnl_Header.setOpaque(false);
		return groupBox;
	}
}
