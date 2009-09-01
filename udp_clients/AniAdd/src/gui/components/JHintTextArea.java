package gui.components;

import java.awt.*;

import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import javax.swing.*;

public class JHintTextArea extends JTextArea implements FocusListener {
    private boolean isEmpty;

    private Color FORGROUND_COLOR;
    private Color FORGROUND_COLOR_EMPTY;

    public JHintTextArea() {
        this(10, 20);
    }

    public JHintTextArea(int rows, int columns) {
        this("", rows, columns);
    }

    public JHintTextArea(String text) {
        this(text, 10, 20);
    }

    public JHintTextArea(String text, int rows, int columns) {
        super(text, rows, columns);
        addFocusListener(this);
        isEmpty = text.isEmpty();

        FORGROUND_COLOR_EMPTY = new Color(128, 128, 128); //getSelectionColor();
        FORGROUND_COLOR = getForeground();
    }

    public String getTextHint() {
        return (String) getClientProperty("textHint");
    }

    public void setTextHint(String hint) {
        putClientProperty("textHint", hint);
        if(isEmpty){
            setHintStyle();
        }
    }

    @Override
    public void setText(String text){
        if(text==null || text.isEmpty()){
            isEmpty = true;
            setHintStyle();
        } else {
            isEmpty = false;
            setNormalStyle();
            super.setText(text);
        }
    }

    @Override
    public String getText(){
        return isEmpty?"":super.getText();
    }

    private void setHintStyle(){
        setForeground(FORGROUND_COLOR_EMPTY);
        super.setText(getTextHint());
    }

    private void setNormalStyle(){
        setForeground(FORGROUND_COLOR);
        super.setText("");
    }

    public void focusGained(FocusEvent e) {
        if (isEmpty) {
            isEmpty = false;
            setNormalStyle();
        }
    }

    public void focusLost(FocusEvent e) {
        isEmpty = getText().isEmpty();

        if (getText().isEmpty()) {
            setHintStyle();
        }
    }
}
