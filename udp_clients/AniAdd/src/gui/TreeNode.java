/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gui;

import java.awt.Color;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 *
 * @author Arokh
 */
public class TreeNode extends DefaultMutableTreeNode{
    String name;
    Color backColor;

    public TreeNode() {
        super();
    }
    public TreeNode(String value) {
        super(value);
    }
    public TreeNode(String value, String name) {
        super(value);
        this.name = name;
    }

    public TreeNode(DefaultMutableTreeNode node){
        super(node.getUserObject());
    }

    public void Name(String name){ this.name = name; }
    public String Name(){ return name; }

    public void BackColor(Color backColor){ this.backColor = backColor; }
    public Color BackColor(){ return backColor; }


}
