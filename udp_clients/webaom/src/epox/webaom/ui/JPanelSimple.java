/*
 * Created on 1. des.. 2007 23.29.22
 * Filename: JPanelSimple.java
 */
package epox.webaom.ui;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.BevelBorder;
import javax.swing.border.EmptyBorder;

import epox.swing.JPanelStatus;
import epox.swing.Log;
import epox.webaom.A;

public class JPanelSimple extends JPanel implements ActionListener{
	protected Container main;
	protected JLabel head, body;
	private JPanel control, inner;
	private JButton next = new JButton("Next");
	private JButton adva = new JButton("Adv.");
	private JPanelStatus status = new JPanelStatus(10);

	private int step = 0;

	public JPanelSimple(String name){
		if(name==null)
			name = "stranger";
		else
			setBorder(new BevelBorder(BevelBorder.LOWERED));

		inner = new JPanel(new BorderLayout());
		inner.setBorder(new EmptyBorder(6,9,6,9));

		FlowLayout layout = new FlowLayout();
		layout.setAlignment(FlowLayout.RIGHT);
		control = new JPanel(layout);
		control.add(next);
		control.add(adva);
		head = new JLabel("Welcome "+name+"!");
		head.setFont(new Font("Tahoma", Font.BOLD, 25));
		body = new JLabel(A.getResourceAsString("welcome.html"));
		setLayout(new BorderLayout());
		inner.add(head, BorderLayout.NORTH);
		inner.add(control, BorderLayout.SOUTH);
		inner.add(body, BorderLayout.CENTER);
		add(inner);
		next.setEnabled(false);
		adva.setEnabled(false);
		next.addActionListener(this);
		adva.addActionListener(this);
	}
	public void start(){
		next.setEnabled(true);
		adva.setEnabled(true);
	}
	public Log getLog(){
		return status;
	}
	public void actionPerformed(ActionEvent e){
		if(e.getSource()==next){
			switch(step){
			case 0:{
				inner.remove(body);
				A.gui.nioToggle();
				head.setText("Drag and drop or paste files here");
				main = A.gui.getTable();
				inner.add(main, BorderLayout.CENTER);
				updateUI();
				main.requestFocus(); } break;
			case 1:{
				head.setText("Please wait...");
				inner.remove(main);
				inner.add(status);
				updateUI();
				A.gui.dioToggle();
				next.setEnabled(false); } break;
			//case 2:{} break;
			}
			step++;
		}else if(e.getSource()==adva){
			next.setEnabled(false);
			adva.setEnabled(false);
			new Thread(new Runnable(){
				public void run(){
					head.setText("Please wait...");
					try{
						Thread.sleep(100);
						//removeAll();
					}catch(InterruptedException x){
						x.printStackTrace();
					}
					A.switchToAdvanced();
				}
			}).start();
		}
	}
}
