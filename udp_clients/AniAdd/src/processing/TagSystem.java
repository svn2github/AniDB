package processing;



import aniAdd.misc.Misc;
import java.util.TreeMap;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Attr;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

public class TagSystem {
	
	public void parseAndTransform(String formatStr, TreeMap<String, String> tags) throws Exception{
		ParseString parse = new ParseString(formatStr);
		Document xmlDoc = parse.formatStr2XMLStr();

		TransformXML transform = new TransformXML(xmlDoc, tags);
		transform.transform();
	}
	
	private class ParseString{
		private Document xmlDoc;
		private String[] formatStr;
		
		public ParseString(String formatStr) throws Exception {
			this.formatStr = formatStr.split("\n");
			
            DocumentBuilderFactory dbfac = DocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = dbfac.newDocumentBuilder();
            xmlDoc = docBuilder.newDocument();
		}
		
		private Document formatStr2XMLStr() throws Exception{
			Node root = xmlDoc.appendChild(xmlDoc.createElement("root"));
			Node subNode;
			int index;
			for (int i = 0; i < formatStr.length; i++) {
				formatStr[i] = formatStr[i].trim();
				if(formatStr[i].isEmpty()) continue;
				index = formatStr[i].indexOf(":=");
				
				subNode = root.appendChild(xmlDoc.createElement("Format"));
				Attr attr = xmlDoc.createAttribute("id");
				attr.setNodeValue(formatStr[i].substring(0, index).trim());
				subNode.getAttributes().setNamedItem(attr);
				
				formatStr[i] = formatStr[i].substring(index+2);
				formatStr2XMLStr(subNode, 0, i);
			}
			
			return xmlDoc;
		}
		private void formatStr2XMLStr(Node node, int pos, int line) throws Exception{	
			boolean doLoop = true;
			
			while(doLoop){
				while(pos < formatStr[line].length() && formatStr[line].charAt(pos) == ' '){
					formatStr[line] = formatStr[line].substring(0, pos) + formatStr[line].substring(pos+1);
				}
				
				doLoop &= formatStr[line].length() != 0;
				if(doLoop){
					doLoop &= formatStr[line].charAt(pos) != '}';
					doLoop &= formatStr[line].charAt(pos) != '=';
					doLoop &= formatStr[line].charAt(pos) != '!';
					doLoop &= formatStr[line].charAt(pos) != ']';
					doLoop &= formatStr[line].charAt(pos) != ')';
					doLoop &= formatStr[line].charAt(pos) != ',';
					doLoop &= formatStr[line].charAt(pos) != '?';
					doLoop &= formatStr[line].charAt(pos) != ':';
				}
				
				if(doLoop){
					//System.out.println("formatStr2XMLStr " + pos + " " + formatStr[line]);
					switch(formatStr[line].charAt(pos)){
						case '"': parseQuotes(node, pos, line); break;
						case '%': parseVariables(node, pos, line); break;
						case '$': parseFunction(node, pos, line); break;
						case '[': parseChoose(node, pos, line); break;
						case '{': parseCondition(node, pos, line); break;
						default: throw new Exception();
					}
				}
			}
		}
		
		private void parseQuotes(Node node, int pos, int line){
			int endIndex = formatStr[line].indexOf('"', pos+1);
			Node subNode = xmlDoc.createElement("Quote");
			subNode.appendChild(xmlDoc.createTextNode(formatStr[line].substring(pos+1, endIndex).replace(" ", "?")));
			node.appendChild(subNode);
			
			formatStr[line] = formatStr[line].substring(0, pos) +   formatStr[line].substring(endIndex+1);
			node.appendChild(subNode);
		}
		private void parseVariables(Node node, int pos, int line){
			int endIndex = formatStr[line].indexOf('%', pos+1);
			Node subNode = xmlDoc.createElement("Variable");
			subNode.appendChild(xmlDoc.createTextNode(formatStr[line].substring(pos+1, endIndex)));
			node.appendChild(subNode);
			
			//System.out.println("Var1 " + pos + " " + formatStr[line]);
			formatStr[line] = formatStr[line].substring(0, pos) +  formatStr[line].substring(endIndex+1);
			//System.out.println("Var2 " + pos + " " + formatStr[line]);
		}
		private void parseFunction(Node node, int pos, int line) throws Exception{
			int beginIndex = pos;
			int endIndex = formatStr[line].indexOf('(', pos+1);
			Node subNode = xmlDoc.createElement("Function");
			Attr attr = xmlDoc.createAttribute("id");
			attr.setNodeValue(formatStr[line].substring(pos+1, endIndex));
			subNode.getAttributes().setNamedItem(attr);
			//subNode.appendChild(attr);
			node.appendChild(subNode);
			
			Node param;
			pos = endIndex+1;
			while(formatStr[line].charAt(pos) != ')'){
				param = xmlDoc.createElement("Param");
				formatStr2XMLStr(param, pos, line);
				subNode.appendChild(param);
				//System.out.println(formatStr[line] + " " + formatStr[line].charAt(pos));
				
				if(formatStr[line].charAt(pos) == ',') formatStr[line] = formatStr[line].substring(0, pos) + formatStr[line].substring(pos+1);
			}
			
			formatStr[line] = formatStr[line].substring(0, beginIndex) + formatStr[line].substring(pos+1);
		}
		private void parseCondition(Node node, int pos, int line) throws Exception{
			int beginIndex = pos; 
			Node subNode = xmlDoc.createElement("Condition");
			node.appendChild(subNode);

			Node ifNode = xmlDoc.createElement("If");
			subNode.appendChild(ifNode);
			
			Node param = xmlDoc.createElement("Param");
			formatStr2XMLStr(param, pos+1, line);
			ifNode.appendChild(param);
			
			//System.out.println("Cond Eq " + formatStr[line].charAt(pos+1) + " " + formatStr[line]);
			if(formatStr[line].charAt(pos+1) == '!' || formatStr[line].charAt(pos+1) == '='){
				ifNode.appendChild(xmlDoc.createElement(formatStr[line].charAt(pos+1) == '!'?"NotEqual":"Equal"));
				param = xmlDoc.createElement("Param");
				formatStr2XMLStr(param, pos+2, line);
				ifNode.appendChild(param);
				pos++;
			}
						
			//System.out.println("Cond If1 " + pos + " " + formatStr[line]);
			if(formatStr[line].charAt(pos+1) == '?'){
				//formatStr = formatStr.substring(0, pos) + formatStr.substring(pos+2);
				pos++;
			} else {
				throw new Exception();
			}
			//System.out.println("Cond If2 " + pos + " " + formatStr[line]);
			
			
			Node thenNode = xmlDoc.createElement("Then");
			param = xmlDoc.createElement("Param");
			formatStr2XMLStr(param, pos+1, line);
			thenNode.appendChild(param);
			subNode.appendChild(thenNode);
			
			//System.out.println("Cond Then1 " + pos + " " + formatStr[line]);
			if(formatStr[line].charAt(pos+1) == ':'){
				//formatStr = formatStr.substring(0, pos) + formatStr.substring(pos+2);
				pos++;
			} else {
				throw new Exception();
			}
			//System.out.println("Cond Then2 " + pos + " " + formatStr[line]);
			
			Node elseNode = xmlDoc.createElement("Else");
			param = xmlDoc.createElement("Param");
			formatStr2XMLStr(param, pos+1, line);
			elseNode.appendChild(param);
			subNode.appendChild(elseNode);
			
			//System.out.println("Cond Else1 " + pos + " " + formatStr[line]);
			if(formatStr[line].charAt(pos+1) == '}'){
				formatStr[line] = formatStr[line].substring(0, beginIndex) + formatStr[line].substring(pos+2);
			} else {
				throw new Exception();
			}
			//System.out.println("Cond Else2 " + pos + " " + formatStr[line]);
		}
		private void parseChoose(Node node, int pos, int line) throws Exception{
			int beginIndex = pos;
			Node subNode = xmlDoc.createElement("Choose");
			node.appendChild(subNode);
			
			Node param;
			pos++;
			while(formatStr[line].charAt(pos) != ']'){
				param = xmlDoc.createElement("Param");
				formatStr2XMLStr(param, pos, line);
				subNode.appendChild(param);
				
				if(formatStr[line].charAt(pos) == ',') formatStr[line] = formatStr[line].substring(0, pos) + formatStr[line].substring(pos+1);
			}
			
			formatStr[line] = formatStr[line].substring(0, beginIndex) + formatStr[line].substring(pos+1);			
		}
	}
	
	private class TransformXML{
		private TreeMap<String, String> tags;
		private Document xmlDoc;
		
		
		public TransformXML(Document xmlDoc, TreeMap<String, String> tags) {
			this.tags = tags;
			this.xmlDoc = xmlDoc;
		}
		
		public void transform(){
			String key, value;
			Node root = xmlDoc.getFirstChild();
			for(int i=0; i<root.getChildNodes().getLength(); i++) {
				key = root.getChildNodes().item(i).getAttributes().getNamedItem("id").getNodeValue();
				value = transform(root.getChildNodes().item(i));
				//System.out.println(value);
				tags.put(key, value);
			}
		}
		
		private String transform(Node node){
			String outputStr = "";
			
			for(int i=0; i<node.getChildNodes().getLength(); i++) {
				Node subNode = node.getChildNodes().item(i);
				
				if(subNode.getNodeName().equals("Quote")){
					outputStr += evalQuotes(subNode);
				} else if(subNode.getNodeName().equals("Variable")){
					outputStr += evalVariables(subNode);
				} else if(subNode.getNodeName().equals("Function")){
					outputStr += evalFunction(subNode);
				} else if(subNode.getNodeName().equals("Choose")){
					outputStr += evalChoose(subNode);
				} else if(subNode.getNodeName().equals("Condition")){
					outputStr += evalCondition(subNode);
				}
			}
			
			return outputStr;
		}
		
		private String evalQuotes(Node node){
			return node.getFirstChild().getNodeValue().replace('?', ' ');
		}
		private String evalVariables(Node node){
			return tags.get(node.getFirstChild().getNodeValue());
		}
		private String evalFunction(Node node){
			String funcId = node.getAttributes().getNamedItem("id").getNodeValue();
			
			if(funcId.equals("pad")){
				int padCount = Integer.parseInt(transform(node.getChildNodes().item(1)));
				char padChar = transform(node.getChildNodes().item(2)).charAt(0);
				return Misc.stringPadding(transform(node.getChildNodes().item(0)), padCount, padChar);
				
			} else if(funcId.equals("len")){
				return String.valueOf(transform(node.getChildNodes().item(0)).length());
				
			} else if(funcId.equals("max")){
				int valA = Integer.parseInt(transform(node.getChildNodes().item(0)));
				int valB = Integer.parseInt(transform(node.getChildNodes().item(1)));
				return String.valueOf(valA>valB?valA:valB);
			}
			
			return funcId+"()";
		}
		private String evalCondition(Node node){
			Node ifNode = node.getChildNodes().item(0);
			Node thenNode = node.getChildNodes().item(1);
			Node elseNode = node.getChildNodes().item(2);
			boolean isTrue;
			
			String valA = transform(ifNode.getChildNodes().item(0));
			if(ifNode.getChildNodes().getLength() != 1){
				boolean isEqualOp = ifNode.getChildNodes().item(1).getNodeName().equals("Equal");
				String valB = transform(ifNode.getChildNodes().item(2));
				isTrue = valA.equals(valB) ^ !isEqualOp;
				
			} else {
				isTrue = !valA.isEmpty() && !valA.toLowerCase().equals("false");
			}
			
			return transform(isTrue?thenNode.getFirstChild():elseNode.getFirstChild());
		}
		private String evalChoose(Node node){
			Node subNode;
			String value;
			for (int i = 0; i < node.getChildNodes().getLength(); i++) {
				subNode = node.getChildNodes().item(i);
				value = transform(subNode);
				for (int j = 0; j < subNode.getChildNodes().getLength(); j++) {
					if(!subNode.getChildNodes().item(j).getNodeName().equals("Quote")){
						subNode.removeChild(subNode.getChildNodes().item(j));
					}
				}
				if(!value.equals(transform(subNode)) && !value.isEmpty()){
					return value;
				}
			}
			
			return null;
		}


	}
}
