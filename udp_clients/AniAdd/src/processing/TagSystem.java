package processing;

import aniAdd.misc.Misc;
import java.util.ArrayList;
import java.util.TreeMap;

public class TagSystem {

    private TreeMap<String, String> vars;
    private String[] src;
    private int charIndex, lineIndex;
    private char[] delimiters = new char[]{',', ')', ']', '}', '=', '?', ':'};

    public static void Evaluate(String sourceCode, TreeMap<String, String> vars) throws Exception {
        TagSystem ts = new TagSystem(sourceCode, vars);
        ts.Start();
    }

    private TagSystem(String sourceCode, TreeMap<String, String> vars) {
        this.src = sourceCode.replaceAll("\r", "").split("\n");
        this.vars = vars;
    }

    // <editor-fold defaultstate="collapsed" desc="Help Methods"> 
    private Character Peek() {
        return src[lineIndex].charAt(charIndex);
    }

    private Character Read() {
        char srcChar = Peek();

        Advance();
        return srcChar;
    }

    private void SkipEmpty() {
        while (!EOL() && Peek() == ' ') {
            Advance();
        }
    }

    private void Advance() {
        charIndex++;
    }

    private int Count() {
        return src[lineIndex].length();
    }

    private boolean EOL() {
        return Count() == charIndex;
    }

    private void Check(String match, String errorMsg) throws Exception {
        boolean isInRange = charIndex + match.length() <= Count();
        boolean isEqual = isInRange && match.equals(src[lineIndex].substring(charIndex, charIndex + match.length()));
        if (!isInRange || !isEqual) {
            throw new Exception(errorMsg);
        } else if (isInRange) {
            charIndex += match.length();
        } else {
            charIndex = Count();
        }
    }

    private void Check(boolean isValid, String errorMsg) throws Exception {
        if (!isValid) {
            throw new Exception(errorMsg);
        }
    }

    private void Throw(String errorMsg) throws Exception {
        throw new Exception(errorMsg);
    }

    private boolean IsDelimiter() {
        for (char delimiter : delimiters) {
            if (delimiter == Peek()) {
                return true;
            }
        }
        return false;
    }
    // </editor-fold> 

    private void Start() throws Exception {
        try {
            for (lineIndex = 0; lineIndex < src.length; lineIndex++) {
                charIndex = 0;
                if (Count() == 0) {
                    continue;
                }

                Asign();
            }
        } catch (Exception ex) {
            throw new Exception(ex.getMessage() + " | Error at Row: " + (lineIndex+1) + " Column: " + (charIndex+1));
        }
    }

    private String Asign() throws Exception {
        SkipEmpty();
        if (EOL()) {
            return null;
        }

        Check(Character.isLetter(Peek()), "Expected 'Name' construct");
        String varName = Name();

        SkipEmpty();
        Check(":=", "Missing asignment :=");

        if (Character.isLetter(Peek())) {
            vars.put(varName, Asign());
            return null;
        }

        SkipEmpty();
        String varValue = ExpressionList();

        SkipEmpty();
        Check(EOL(), "Expected end of line.");


        vars.put(varName, varValue);
        return varValue;
    }

    private String ExpressionList() throws Exception {
        String retVal = "";
        while (!EOL() && !IsDelimiter()) {
            retVal += Expression();
            SkipEmpty();
        }
        return retVal;
    }

    private String Expression() throws Exception {
        String retVal = "";
        switch (Peek()) {
            case '"':
            case '\'':
                return Value();
            case '%':
                return Variable();
            case '[':
                return Choose();
            case '{':
                return Condition();
            case '$':
                return Function();
            default:
                Throw("Unknown construct");
        }
        return retVal;
    }

    private String Function() throws Exception {
        Check("$", "Invalid beginning of a 'Function' construct");
        String funcName = Name();
        Check("(", "Function is missing opening bracket");

        SkipEmpty();
        ArrayList<String> params = new ArrayList<String>();
        while (!EOL() && Peek() != ')') {
            params.add(ExpressionList());
            SkipEmpty();
            if (Peek() != ')') {
                Check(",", "Missing parameter delimiter ,");
            }
            SkipEmpty();
        }

        Check(")", "Function is missing closure character )");

        //System.out.println("Function: " + funcName );

        try {
            if (funcName.equals("pad")) {
                int padCount = Integer.parseInt(params.get(1));
                char padChar = params.get(2).charAt(0);
                return Misc.stringPadding(params.get(0), padCount, padChar);
            } else if (funcName.equals("max")) {
                int valA = Integer.parseInt(params.get(0));
                int valB = Integer.parseInt(params.get(1));
                return String.valueOf(valA > valB ? valA : valB);
            } else if (funcName.equals("min")) {
                int valA = Integer.parseInt(params.get(0));
                int valB = Integer.parseInt(params.get(1));
                return String.valueOf(valA < valB ? valA : valB);
            } else if (funcName.equals("len")) {
                return String.valueOf(params.get(0).length());
            } else if (funcName.equals("repl")) {
                String valA = params.get(0);
                String valB = params.get(1);
                String valC = params.get(2);
                return valA.replaceAll(valB, valC);
            } else {
                Throw("No such functionname");
                return null;
            }
        } catch (Exception e) {
            Throw("Function threw an error");
            return null;
        }
    }

    private String Choose() throws Exception {
        Check("[", "Invalid beginning of a 'Choose' construct");

        SkipEmpty();
        String value = "";
        while (!EOL() && Peek() != ']') {
            if (value.isEmpty()) {
                value = ExpressionList();
            } else {
                ExpressionList();
            }
            SkipEmpty();
            if (Peek() != ']') {
                Check(",", "Missing parameter delimiter ,");
            }
            SkipEmpty();
        }

        Check("]", "Choose is missing closure character ]");

        //System.out.println("Choose: " + value );

        return value;
    }

    private String Condition() throws Exception {
        Check("{", "Invalid beginning of a 'Condition' construct");

        SkipEmpty();
        String condA = ExpressionList();

        SkipEmpty();
        String condB = null;
        if (Peek() == '=') {
            Advance();
            SkipEmpty();
            condB = ExpressionList();
        }

        SkipEmpty();
        Check("?", "Missing condition delimiter '?'");

        SkipEmpty();
        String trueVal = ExpressionList();

        SkipEmpty();
        Check(":", "Missing true/false delimiter ':'");

        SkipEmpty();
        String falseVal = ExpressionList();

        Check("}", "Condition is missing closure character }");

        //System.out.println("Condition A='" + condA + "' B='" + condB + "' TrueVal='" + trueVal + "' FalseVal='" + falseVal + "'" );


        if ((!condA.isEmpty() && condB == null) || condA.equals(condB)) {
            return trueVal;
        } else {
            return falseVal;
        }
    }

    private String Variable() throws Exception {
        Check("%", "Invalid beginning of a 'Variable' construct");

        String varName = "";
        while (!EOL() && !Peek().equals('%')) {
            varName += Read().toString();
        }
        Check("%", "Variable is missing closure character %");

        String varValue = vars.get(varName);
        //System.out.println("Variablename: " + varName + "VarValue: " + varValue );
        return varValue == null ? "" : varValue;
    }

    private String Value() throws Exception {
        char delimiter = Peek() == '"'? '"' : '\'';

        Check(delimiter+"", "Invalid beginning of a 'Value' construct");

        String value = "";
        while (!EOL() && Peek()!=delimiter) {
            value += Read().toString();
        }

        Check(delimiter+"", "Value is missing closure character " + delimiter);

        //System.out.println("Value: " + value );
        return value;
    }

    private String Name() throws Exception {
        Check(Character.isLetter(Peek()), "Invalid beginning of a 'Name' construct");

        String name = Read().toString();
        while (!EOL() && (Character.isLetter(Peek()) || Character.isDigit(Peek()))) {
            name += Read().toString();
        }

        //System.out.println("Name: " + name );

        return name;
    }
}
