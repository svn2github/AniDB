package gui.components;

import java.awt.*;

import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.Highlighter.HighlightPainter;

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

    private void setHintStyle(){
        setForeground(FORGROUND_COLOR_EMPTY);
        setText(getTextHint());
    }

    private void setNormalStyle(){
        setForeground(FORGROUND_COLOR);
        setText("");
    }

    public void focusGained(FocusEvent e) {
        if (isEmpty) {
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
