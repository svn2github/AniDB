/*
 * Created on 2. des.. 2007 11.22.44
 * Filename: JPanelStatus.java
 */
package epox.swing;

import java.awt.Color;
import java.awt.GridLayout;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import javax.swing.JLabel;
import javax.swing.JPanel;

public class JPanelStatus extends JPanel implements Log{
	protected final Queue queue;
	protected final JLabel[] labels;

	public JPanelStatus(int size){
		queue = new ConcurrentLinkedQueue();
		labels = new JLabel[size];
		setLayout(new GridLayout(size,1));
		for(int i=0; i<labels.length; i++){
			labels[i] = new JLabel();
			int gray = i*25;
			labels[i].setForeground(new Color(gray,gray,gray));
			add(labels[i]);
		}
		Thread t = new Thread(){
			public void run(){
				while(true){
					update();
					try{
						Thread.sleep(250);
					}catch(InterruptedException e){
						e.printStackTrace();
					}
				}
			}
		};
		t.setDaemon(true);
		t.start();
	}
	protected void update(){
		if(!queue.isEmpty()){
			String s = queue.poll().toString();
			if(!labels[0].getText().equals(s)){
				for(int i=labels.length-1; i>0; i--)
					labels[i].setText(labels[i-1].getText());
				labels[0].setText(s);
			}
		}
	}
	public void add(int type, Object o){
		String s = o.toString();
		if(s.indexOf("idle")<1 )
			queue.add("<html>"+s+"</html>");
	}
	public void add(Object o){
		add(Log.LOG, o);
	}
}
