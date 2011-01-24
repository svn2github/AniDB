package processing;

import aniAdd.misc.Misc;
import java.util.ArrayList;
import java.util.TreeMap;

public class TagSystem {

    private static char[] delimiters = new char[]{',', ')', ']', '}', '=', '?', ':'};

    public static void Evaluate(String sourceCode, TreeMap<String, String> vars) throws Exception {
        Environment e = new Environment(sourceCode.split("[\n\r]"), vars, null);
        Start(e);
    }

    // <editor-fold defaultstate="collapsed" desc="Evaluation">
    private static void Start(Environment e) throws Exception {
        Function.depth = 0;
        try {
            for (e.lineIndex = 0; e.lineIndex < e.src.length; e.lineIndex++, e.charIndex = 0) {
                if (e.Count() == 0 || e.Peek() == '#') {
                    continue;
                }
                Asign(e);
            }
        } catch (Exception ex) {
            //ex.printStackTrace();
            throw new Exception(ex.getMessage() + " | Error at Row: " + (e.lineIndex + 1) + " Column: " + (e.charIndex + 1));
        }
    }

    private static String Asign(Environment e) throws Exception {
        e.SkipEmpty();
        if (e.EOL()) {
            return null;
        }

        e.Check(Character.isLetter(e.Peek()), "Expected 'Name' construct");
        String varName = Name(e);

        ArrayList<String> paramNames = null;
        if (e.Peek() == '(') {
            e.Advance();
            e.SkipEmpty();

            paramNames = new ArrayList<String>();
            while (e.Peek() != ')') {
                paramNames.add(Name(e));

                e.SkipEmpty();
                if (e.Peek() != ')') {
                    e.Check(",", "Missing parameter delimiter ,");
                    e.SkipEmpty();
                    e.Check(e.Peek() != ')', "Empty parameternames are not allowed");
                    e.SkipEmpty();
                }
            }
            e.Check(")", "Function definition is missing closure character )");
        }

        e.SkipEmpty();
        e.Check(":=", "Missing asignment :=");

        if (paramNames != null) {
            e.funcs.put(varName, new Function(paramNames, e.ReadToEnd()));
            return null;

        } else if (Character.isLetter(e.Peek())) {
            e.vars.put(varName, Asign(e));
            return null;

        } else {
            e.SkipEmpty();
            String varValue = ExpressionList(e);

            e.SkipEmpty();
            e.Check(e.EOL(), "Expected end of line.");


            e.vars.put(varName, varValue);
            return varValue;
        }

    }

    private static String ExpressionList(Environment e) throws Exception {
        String retVal = "";
        while (!e.EOL() && !e.IsDelimiter()) {
            retVal += Expression(e);
            e.SkipEmpty();
        }
        return retVal;
    }

    private static String Expression(Environment e) throws Exception {
        String retVal = "";
        switch (e.Peek()) {
            case '"':
            case '\'':
            case '´':
                return Value(e);
            case '%':
                return Variable(e);
            case '[':
                return Choose(e);
            case '{':
                return Condition(e);
            case '$':
                return Function(e);
            case '#':
                e.ReadToEnd();
                return "";
            default:
                e.Throw("Unknown construct");
        }
        return retVal;
    }

    private static String Function(Environment e) throws Exception {
        e.Check("$", "Invalid beginning of a 'Function' construct");
        String funcName = Name(e);
        e.Check("(", "Function is missing opening bracket");

        e.SkipEmpty();
        ArrayList<String> params = new ArrayList<String>();
        while (!e.EOL() && e.Peek() != ')') {
            params.add(ExpressionList(e));
            e.SkipEmpty();
            if (e.Peek() != ')') {
                e.Check(",", "Missing parameter delimiter ,");
            }
            e.SkipEmpty();
        }

        e.Check(")", "Function is missing closure character )");

        //System.out.println("Function: " + funcName );

        try {
            if(e.funcs.containsKey(funcName)) {
                return e.funcs.get(funcName).Invoke(e, params);
            } else if (funcName.equals("pad")) {
                e.Check(params.size() == 3, "Invalid Parameter count");
                int padCount = Integer.parseInt(params.get(1));
                char padChar = params.get(2).charAt(0);
                return Misc.stringPadding(params.get(0), padCount, padChar);
            } else if (funcName.equals("max")) {
                e.Check(params.size() == 2, "Invalid Parameter count");
                int valA = Integer.parseInt(params.get(0));
                int valB = Integer.parseInt(params.get(1));
                return String.valueOf(valA > valB ? valA : valB);
            } else if (funcName.equals("min")) {
                e.Check(params.size() == 2, "Invalid Parameter count");
                int valA = Integer.parseInt(params.get(0));
                int valB = Integer.parseInt(params.get(1));
                return String.valueOf(valA < valB ? valA : valB);
            } else if (funcName.equals("len")) {
                e.Check(params.size() == 1, "Invalid Parameter count");
                return String.valueOf(params.get(0).length());
            } else if (funcName.equals("uc")) {
                e.Check(params.size() == 1, "Invalid Parameter count");
                return params.get(0).toUpperCase();
            } else if (funcName.equals("lc")) {
                e.Check(params.size() == 1, "Invalid Parameter count");
                return params.get(0).toLowerCase();
            } else if (funcName.equals("add")) {
                e.Check(params.size() == 2, "Invalid Parameter count");
                return String.valueOf(Integer.parseInt(params.get(0)) + Integer.parseInt(params.get(1)));
            } else if (funcName.equals("sub")) {
                e.Check(params.size() == 2, "Invalid Parameter count");
                return String.valueOf(Integer.parseInt(params.get(0)) - Integer.parseInt(params.get(1)));
            } else if (funcName.equals("mul")) {
                e.Check(params.size() == 2, "Invalid Parameter count");
                return String.valueOf(Integer.parseInt(params.get(0)) * Integer.parseInt(params.get(1)));
            } else if (funcName.equals("div")) {
                e.Check(params.size() == 2, "Invalid Parameter count");
                return String.valueOf(Integer.parseInt(params.get(0)) / Integer.parseInt(params.get(1)));
            } else if (funcName.equals("substr")) {
                e.Check(params.size() == 2 || params.size() == 3, "Invalid Parameter count");
                String valA = params.get(0);
                int valB = Integer.parseInt(params.get(1));
                return params.size() == 2? valA.substring(valB) : valA.substring(valB,  Integer.parseInt(params.get(2)));
            } else if (funcName.equals("repl")) {
                e.Check(params.size() == 3, "Invalid Parameter count");
                String valA = params.get(0);
                String valB = params.get(1);
                String valC = params.get(2);
                return valA.replaceAll(valB, valC);
            } else if (funcName.equals("match")) {
                e.Check(params.size() == 2, "Invalid Parameter count");
                String valA = params.get(0);
                String valB = params.get(1);
                return valA.matches(valB)?"1":"";
            } else if (funcName.equals("indexof")) {
                e.Check(params.size() == 2, "Invalid Parameter count");
                String valA = params.get(0);
                String valB = params.get(1);
                return String.valueOf(valA.indexOf(valB));
            } else if (funcName.equals("lindexof")) {
                e.Check(params.size() == 2, "Invalid Parameter count");
                String valA = params.get(0);
                String valB = params.get(1);
                return String.valueOf(valA.lastIndexOf(valB));
            } else {
                e.Throw("No such functionname");
                return null;
            }
        } catch (Exception ex) {
            e.Throw(ex.getMessage()==null?"Function threw an error": ex.getMessage());
            return null;
        }
    }

    private static String Choose(Environment e) throws Exception {
        e.Check("[", "Invalid beginning of a 'Choose' construct");

        e.SkipEmpty();
        String value = "";
        while (!e.EOL() && e.Peek() != ']') {
            if (value.isEmpty()) {
                value = ExpressionList(e);
            } else {
                ExpressionList(e);
            }
            e.SkipEmpty();
            if (e.Peek() != ']') {
                e.Check(",", "Missing parameter delimiter ,");
            }
            e.SkipEmpty();
        }

        e.Check("]", "Choose is missing closure character ]");

        //System.out.println("Choose: " + value );

        return value;
    }

    private static String Condition(Environment e) throws Exception {
        e.Check("{", "Invalid beginning of a 'Condition' construct");

        e.SkipEmpty();
        String condA = ExpressionList(e);

        e.SkipEmpty();
        String condB = null;
        if (e.Peek() == '=') {
            e.Advance();
            e.SkipEmpty();
            condB = ExpressionList(e);
        }

        e.SkipEmpty();
        e.Check("?", "Missing condition delimiter '?'");

        e.SkipEmpty();
        String trueVal = ExpressionList(e);

        e.SkipEmpty();
        e.Check(":", "Missing true/false delimiter ':'");

        e.SkipEmpty();
        String falseVal = ExpressionList(e);

        e.Check("}", "Condition is missing closure character }");

        //System.out.println("Condition A='" + condA + "' B='" + condB + "' TrueVal='" + trueVal + "' FalseVal='" + falseVal + "'" );


        if ((!condA.isEmpty() && condB == null) || condA.equals(condB)) {
            return trueVal;
        } else {
            return falseVal;
        }
    }

    private static String Variable(Environment e) throws Exception {
        e.Check("%", "Invalid beginning of a 'Variable' construct");

        String varName = "";
        while (!e.EOL() && !e.Peek().equals('%')) {
            varName += e.Read().toString();
        }
        e.Check("%", "Variable is missing closure character %");

        String varValue = e.vars.get(varName);
        //System.out.println("Variablename: " + varName + "VarValue: " + varValue );
        return varValue == null ? "" : varValue;
    }

    private static String Value(Environment e) throws Exception {
        char delimiter = e.Peek() == '"' ? '"' : '\'';

        e.Check(delimiter + "", "Invalid beginning of a 'Value' construct");

        String value = "";
        while (!e.EOL() && e.Peek() != delimiter) {
            value += e.Read().toString();
        }

        e.Check(delimiter + "", "Value is missing closure character " + delimiter);

        //System.out.println("Value: " + value );
        return value;
    }

    private static String Name(Environment e) throws Exception {
        e.Check(Character.isLetter(e.Peek()), "Invalid beginning of a 'Name' construct");

        String name = e.Read().toString();
        while (!e.EOL() && (Character.isLetter(e.Peek()) || Character.isDigit(e.Peek()))) {
            name += e.Read().toString();
        }

        //System.out.println("Name: " + name );

        return name;
    }
    // </editor-fold>
    
    private static class Function {
        static int depth = 0;
        
        ArrayList<String> paramNames;
        String code;

        public Function(ArrayList<String> paramNames, String code) {
            this.paramNames = paramNames;
            this.code = code;
        }

        public String Invoke(Environment e, ArrayList<String> params) throws Exception {
            e.Check(params.size() == paramNames.size(), "Parameter count missmatch");

            TreeMap<String, String> varsClone = new TreeMap<String, String>(e.vars);
            for (int i = 0; i < params.size(); i++) {
                varsClone.put(paramNames.get(i), params.get(i));
            }
            Environment eFunc = new Environment(new String[]{code}, varsClone, e.funcs);
            
            depth++;
            eFunc.SkipEmpty();
            e.Check(depth < 20, depth + ". function call. Aborted");

            String retVal = ExpressionList(eFunc);
            depth--;

            return retVal;
        }
    }

    private static class Environment {

        TreeMap<String, Function> funcs;
        TreeMap<String, String> vars;
        int charIndex, lineIndex;
        String[] src;

        public Environment(String[] src, TreeMap<String, String> vars, TreeMap<String, Function> funcs) {
            this.src = src;
            this.funcs = funcs;
            this.vars = vars;

            if (this.vars == null) {
                this.vars = new TreeMap<String, String>();
            }
            if (this.funcs == null) {
                this.funcs = new TreeMap<String, Function>();
            }
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

        private String ReadToEnd(){
            String val = src[lineIndex].substring(charIndex);
            charIndex = src[lineIndex].length();
            return val;
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
    }
}
