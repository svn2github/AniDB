package epox.webaom;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Stack;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JTextArea;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class RuleMenu extends DefaultHandler{
	private JMenuItem m_item = null;
	private final Stack m_subs;
	private final JTextArea m_jta;
	public RuleMenu(JTextArea jta){
		m_jta = jta;
		m_subs = new Stack();
	}
	public JPopupMenu getMenu(){
		return ((JMenu)m_subs.peek()).getPopupMenu();
	}
	public void startElement(String uri, String localName, String qName, final Attributes attributes) throws SAXException {
		super.startElement(uri, localName, qName, attributes);

		if(qName.equals("elem")){
			m_item = new JMenuItem(attributes.getValue("title"));
			//m_item.setToolTipText(attributes.getValue("tip"));
			MyListener my = new MyListener(m_jta, attributes.getValue("value"));
			m_item.addActionListener(my);
			//m_item.addMouseListener(my);
			((JMenu)m_subs.peek()).add(m_item);
		}else if(qName.equals("item")){
			m_subs.push(new JMenu(attributes.getValue("title")));
		}else if(qName.equals("menu")){
			m_subs.add(new JMenu());
		}
	}
	public void endElement(String uri, String localName, String qName) throws SAXException {
		super.endElement(uri, localName, qName);
		if(qName.equals("item")){
			JMenu sub = (JMenu)m_subs.pop();
			((JMenu)m_subs.peek()).add(sub);
			//System.out.println("pop  "+sub.getText());
		}
		m_item = null;
	}
	public void characters(char[] ch, int start, int length) throws SAXException {
		super.characters(ch, start, length);
		String str = new String(ch,start,length).trim();
		if(m_item!=null && str.length()>0)
			m_item.setToolTipText(str);
	}
	private class MyListener/* extends MouseAdapter*/ implements ActionListener{
		private final JTextArea jta;
		//private final JMenuItem jmi;
		private String s;
		public MyListener(JTextArea ta, /*JMenuItem mi, */String s){
			jta = ta;
			//jmi = mi;
			this.s = s;
		}
		public void actionPerformed(ActionEvent e){
			//System.out.println(s);
			jta.insert(s, jta.getCaretPosition());
		}
		/*public void mouseEntered(MouseEvent e) {
			super.mouseEntered(e);
			m_pb.setString(m_item.getToolTipText());
		}
		public void mouseExited(MouseEvent e) {
			super.mouseExited(e);
			m_pb.setString("");
		}*/
	}
	/*protected final JProgressBar m_pb=new JProgressBar();
	protected JTextArea m_tf = new JTextArea();
	private static Font MYF = new Font("Tahoma", Font.PLAIN, 11);
	public static void main (String [] args) {
		final RuleMenu handler = new RuleMenu();
		SAXParserFactory factory = SAXParserFactory.newInstance();
		try {
			SAXParser saxParser = factory.newSAXParser();
			saxParser.parse( new File("C:\\rule-helper.xml"), handler );

			final JPopupMenu x = handler.getMenu();
			x.setFont(MYF);

			JFrame f = new JFrame("test");
			f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			handler.m_tf.setFont(new Font("Tahoma", Font.PLAIN, 18));
			handler.m_tf.setMargin(new Insets(4,4,4,4));
			handler.m_tf.addMouseListener(new MouseAdapter(){
				@Override
				public void mouseClicked(MouseEvent e) {
					super.mouseClicked(e);
					if(e.getButton()==MouseEvent.BUTTON3)
						x.show(handler.m_tf, e.getX(),e.getY());
				}
			});
			f.getContentPane().setLayout(new BorderLayout());
			f.getContentPane().add(handler.m_tf, BorderLayout.CENTER);
			f.getContentPane().add(handler.m_pb, BorderLayout.SOUTH);
			handler.m_pb.setStringPainted(true);
			f.setBounds(100,200, 640, 480);
			f.setVisible(true);

			handler.m_tf.setText("IF A(Naruto);G(zx) DO FAIL //Do not rename file if it is Naruto\nDO ADD '%eng (%ann) - %enr - %epn ' //Add the base, same for all files\nIF D(japanese);S(english) DO ADD '(SUB)' //Add (SUB) if the file is subbed in english\nIF D(japanese);S(none) DO ADD '(RAW)' //Add (RAW) if the file is not subbed.\nIF G(!unknown) DO ADD '[%grp]' //Add group name if it is not unknown\nDO ADD '(%CRC)' //Always add crc");


		} catch (Throwable t) {
			t.printStackTrace();
		}
	}*/
}