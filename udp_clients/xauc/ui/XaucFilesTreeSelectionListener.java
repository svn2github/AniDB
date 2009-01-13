package ui;

import javax.swing.JTree;
import javax.swing.JEditorPane;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;

import clients.XaucOptions;
import clients.XaucShared;

import structures.AniDBFile;
import structures.SwingFile;
import utils.Log;

public class XaucFilesTreeSelectionListener implements TreeSelectionListener {
	protected JEditorPane infoPane = null;
	protected Log log = null;
	protected JTree tree = null;
	protected XaucShared sharedParent = null;
	
	public XaucFilesTreeSelectionListener(JTree tree, JEditorPane infoPane, Log log, XaucShared sharedParent) {
		super();
		this.infoPane = infoPane;
		this.log = log;
		this.tree = tree;
		this.sharedParent = sharedParent;
	}
	
	public void valueChanged(TreeSelectionEvent evt) {
		DefaultMutableTreeNode node = (DefaultMutableTreeNode) evt.getPath().getLastPathComponent();
		if (node == null) return;
		if (node.getUserObject() == null || node.getUserObject() instanceof String) return;
		SwingFile sf = (SwingFile)node.getUserObject();
		AniDBFile af = sf.getAniDBFile();
		if (this.infoPane != null) {
			if (sharedParent.getClientOptions().getTreeFileOuputFormat() == XaucOptions.OutputFormats.TXT)
				this.infoPane.setText(af.writeToString());
			else if (sharedParent.getClientOptions().getTreeFileOuputFormat() == XaucOptions.OutputFormats.XML)
				this.infoPane.setText(af.getXml());
			else
				return; // redundant, i know...
		}
	}

}
