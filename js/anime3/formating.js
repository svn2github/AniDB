/* file message page scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 * @contents Core Functions
 *           Formating
 * version 1.2 (18.06.2007) - Previous version
 * version 2.0 (20.04.2008) - Rather full support for every major browser Firefox, IE, Opera and Safari
 */

// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
// WYSIWYG Width and Height
var wysiwygWidth = 500;
var wysiwygHeight = 250;
// Create viewTextMode global variable and set to 0
// enabling all toolbar commands while in HTML mode
var viewTextMode = 0;
var current_ed; // current editor
var isIE = (document.selection != undefined && window.getSelection == undefined) ? true : false;
var isFF = (document.selection == undefined && window.getSelection != undefined) ? true : false;
var isOP = (document.selection != undefined && window.getSelection != undefined) ? true : false;
var isWK = (navigator.userAgent.toLowerCase().indexOf('applewebkit') >= 0);

/* Change view mode
 * @param type Type of view mode (source,wysiwyg)
 * @param n Number of the textarea
 */
function viewMode(type,n) {
	var getDocument = document.getElementById("wysiwyg_" + n).contentWindow.document;
	if (!getDocument) return;
	var fTA = document.getElementById('textArea_'+n);
	var browserName = navigator.appName;
	if (browserName == "Microsoft Internet Explorer") { // View Source for IE 	 
		if (type == 'source') {
			var iHTML = getDocument.body.innerHTML;
			getDocument.body.innerText = iHTML;
		} else {
			var iText = getDocument.body.innerText;
			getDocument.body.innerHTML = iText;
		}
	} else { // View Source for Mozilla/Netscape
		if (type == 'source') {
			var html = document.createTextNode(getDocument.body.innerHTML);
			getDocument.body.innerHTML = "";
			getDocument.body.appendChild(html);
		} else {
			var html = getDocument.body.ownerDocument.createRange();
			html.selectNodeContents(getDocument.body);
			getDocument.body.innerHTML = html.toString();
		}
	}
	// Hide the HTML Mode button and show the Text Mode button
	var html = getElementsByClassName(fTA.form.getElementsByTagName('input'),'f_view_source',true)[0];
	var text = getElementsByClassName(fTA.form.getElementsByTagName('input'),'f_view_text',true)[0];
	if (!html || !text) return; // no buttons nothing, to do
	if (type == 'source') {
		if (html) html.style.display = 'none'; 
		if (text) text.style.display = '';
		// set the font values for displaying HTML source
		getDocument.body.style.fontSize = "12px";
		getDocument.body.style.fontFamily = "Courier New"; 
		viewTextMode = 1;
	} else {
		if (html) html.style.display = ''; 
		if (text) text.style.display = 'none';
		// reset the font values
		getDocument.body.style.fontSize = "";
		getDocument.body.style.fontFamily = ""; 
		viewTextMode = 0;
	}
	var cln = this.className;
	if (cln.indexOf('f_selected') > 0) this.className = this.className.replace(' f_selected','');
}

/* Updates a textarea
 * @param n Number of the textarea to update
 */
function updateTextArea(n) {
	document.getElementById("textArea_"+n).value = document.getElementById("wysiwyg_" + n).contentWindow.document.body.innerHTML;
	format_output(n);
}

/* Updates something */
function updateTextAreaCK() {
	var tx = this.form.getElementsByTagName('textarea')[0];
	if (!tx) return;
	var n = tx.id.replace('textArea_','');
	updateTextArea(n);
}

/* Creates a select, this function is diferent from the ones in utils
 * @param node Node where to append this select
 * @param item Item to create
 * @param className Class name of the select
 */
function createSelect(node,item,className) {
	var select = document.createElement('select');
	select.className = 'f_'+item;
	for (var i=0; i < item.length; i++) {
		var curItem = item[i];
		createSelectOption(select,curItem,curItem,false);
	}
	select.className = className;
	node.appendChild(select);
}

/* Creates a button, this function is diferent from the ones in utils
 * @param node Node where to append this button
 * @param item Item to create
 * @param curItem item data
 */
function createLocalButton(node,item,curItem) {
	var button = document.createElement('input');
	button.type = 'button';
	if (!curItem['onclick']) button.onclick = make;
	else button.onclick = curItem['onclick'];
	button.className = 'f_button f_'+item;
	if (curItem['desc']) button.title = curItem['desc'];
	button.alt = item;
	if (curItem['accesskey']) button.accessKey = curItem['accesskey'];
	if (curItem['accesskey']) button.setAttribute('accessKey',curItem['accesskey']);
	button.onmouseover = buttonMouseOver;
	button.onmouseout = buttonMouseOut;
	try {
		if (curItem['text']) button.appendChild(document.createTextNode(curItem['text']));
	} catch(e) { // making ie work
		true;
	}
	if (node) node.appendChild(button);
	else return(button);
}

/* Function that replaces classnames
 * @param node Node to replace className
 * @param replaceText replace text
 */
function buttonMouseReplace(node,replaceText) {
	node.className = node.className.replace('replaceme',replaceText);
}

/* Function that replaces clears mouseover classes
 * @param node Node to replace stuff
 * @param type Unnused it seems..
 */
function buttonMouseFReplace(node,type) {
	if      (node.className.indexOf('f_mouseover') > 0) node.className = node.className.replace('f_mouseover','replaceme');
	else if (node.className.indexOf('f_mouseout') > 0)  node.className = node.className.replace('f_mouseout','replaceme');
	else node.className += ' replaceme';
	return;
}
/* Function that adds the mouseover class */
function buttonMouseOver() {
	buttonMouseFReplace(this);
	buttonMouseReplace(this,'f_mouseover');
}
/* Function that adds the mouseout class */
function buttonMouseOut() {
	buttonMouseFReplace(this);
	buttonMouseReplace(this,'f_mouseout');
}

/* Function that inserts text or Nodes at the current cursor position/selection
 * @param myField Where the cursor/selection is located
 * @param myValue text that will replace the current
 * @param text if true will insert text instead of nodes
 */
function insertAtSelection(myField, myValue, text) {
	if (isIE) {	//IE support
		myField = myField.document;
		myField.focus();
		var sel = myField.selection.createRange();
		if (text) sel.text = myValue;
		else sel.pasteHTML(myValue);
		myField.focus();
	} else if (isFF || isOP) { //MOZILLA/NETSCAPE support
		if (isFF) myField.focus();
		var sel = myField.getSelection();
		var range = sel.getRangeAt(0);
		range.deleteContents();
		var doc = myField.document;
		if (text) range.insertNode(doc.createTextNode(myValue));
		else range.insertNode(myValue);
	} else {
		if (seeDebug) alert('insertAtSelection: unknown selection method');
		return;
	}
}

/* Function that gets the current selection
 * @param myField Where the cursor/selection is located
 * @return 
 */
function getSelection(myField) {
	if (isIE) {	//IE support
		myField = myField.document;
		myField.focus();
		var sel = myField.selection.createRange();
		return (sel.text);
	} else if (isFF || isOP) { //MOZILLA/NETSCAPE support
		if (isFF) myField.focus();
		var sel = myField.getSelection();
		var range = sel.getRangeAt(0);
		return (range.toString());
	} else {
		if (seeDebug) alert('getSelection: unknown selection method');
		return;
	}
}

/* Find current selection */
function findSelection() {
	var myField = this;
	myField.focus();
	var sel = myField.getSelection();
	var range = sel.getRangeAt(0);
	if (range.toString().length) {
		// check to see if we have some node
		var aux = range.cloneContents();
		for (var i = 0; i < aux.childNodes.length; i++)
			var node = aux.childNodes[i];
	}
}

/* This function creates and inserts an href at the selection
 * @param obj The RTE obj
 * @param fTA The underlying textArea
 * @param val The type of the link
 * @param attribute The attribute of the link
 * @param textOnly true if only the text of the link is needed
 */
function createLink(obj,fTA,val,attribute,sel,textOnly) {
	if (!textOnly && (!obj || !fTA)) return; // can not continue
	if (!val) {
		var selects = fTA.form.getElementsByTagName('select');
		for (var s in selects) {
			var select = selects[s];
			if (select.className && select.className.indexOf('f_links') >= 0) { val = select.value; break; }
		}
		if (!val) return; // there is some error
	}
	var base = "http://anidb.net/";
	var type = ""; // link type
	if (!attribute && val != 'wiki') {
		attribute = window.prompt("Please type in the attribute for your "+val+" link.");
		if (!attribute || attribute == '') return; // the attribute doesn't conform to rules
	}
	switch (val) {
		case 'anime':
		case 'creq':
		case 'ep':
		case 'file':
		case 'group':
		case 'producer':
		case 'reviews':
		case 'mylist':
		case 'votes': type = val.charAt(0); break;
		case 'titles': type = 'at'; break;
		case 'producers': type = 'ap'; break;
		case 'genres': type = 'ag'; break;
		case 'cats': type = 'ac'; break;
		case 'relations': type = 'ar'; break;
		case 'user': type = 'up'; break;
		case 'review': type = 'rs'; break;
		case 'groupcmts': type = 'agc'; break;
		case 'wiki': base = "http://wiki.anidb.net/w/"; break;
		case 'forum':
		case 'forum.topic':
		case 'forum.board':
		case 'forum.post': 
			base = "http://forum.anidb.net/";
			if      (val == 'forum.board') base += "viewforum.php?f=";
			else if (val == 'forum.topic') base += "viewtopic.php?t=";
			else if (val == 'forum.post' ) base += "viewtopic.php?p=";
			break;
		case 'tracker': base = "http://tracker.anidb.net/view.php?id="; break;
		default: base = "";
	}
	if (base == "") return; //no base no link
	if (!sel) {
		sel = getSelection(obj);
		if (!sel) sel = val + " " + attribute;
	}
	var hyperLink = '['+val+':'+attribute+':'+sel+']';
	if (textOnly) return hyperLink;
	if (isIE) {	//IE support
		// IE fails miserably here, don't show the link thingie
		insertAtSelection(obj, hyperLink, true);
	} else if (isFF || isOP) { //MOZILLA/NETSCAPE support
		hyperLink = obj.document.createElement('a');
		hyperLink.href = base+type+attribute;
		hyperLink.type = val;
		hyperLink.setAttribute('att',attribute);
		hyperLink.appendChild(document.createTextNode(sel));
		insertAtSelection(obj, hyperLink, false);
	} else {
		if (seeDebug) alert('getSelection: unknown selection method');
		return;
	}
}

/* Function that formats text
 * @param id 
 * @param n Number of textArea
 * @param selected Is select
 */
function formatText(id, n, selected) {
	// When user clicks toolbar button make sure it always targets its respective WYSIWYG
	var obj = document.getElementById("wysiwyg_" + n);
	if (!obj) return;
	obj.contentWindow.focus();
	var fTA = document.getElementById('textArea_' + n);
	// When in Text Mode these execCommands are disabled
	var formatIDs = new Array("Bold","Italic","Underline","Strikethrough","InsertUnorderedList","InsertOrderedList","CreateLink");
	for (var i = 0; i <= formatIDs.length;) { // Check if button clicked is in disabled list
		if (formatIDs[i] == id)
			 var disabled_id = 1; 
		i++;
	}
	if (viewTextMode == 1 && disabled_id == 1) { // Check if in Text Mode and disabled button was clicked
		alert ("You are in TEXT Mode. This feature has been disabled.");	
	} else {
		if (id == "CreateLink") { // CreateLink
			createLink(obj.contentWindow, fTA);
		} else if (id == "ViewSource") { // ViewSource
			viewMode('source',n);
		} else if (id == "ViewText") { // ViewText
			viewMode('text',n);
		} else { // Every other command
			obj.contentWindow.document.execCommand(id, false, null);
		}
	}
}

/* Function that inserts RTE commands */
function make () {
	var start = this.className.indexOf('f_button f_')+11;
	var formatingFunc = this.className.substring(start,this.className.indexOf(' ',start));
	var fMap = FunctionMap[formatingFunc];
	allowOut = false;
	var execID = fMap.id;
	var iframeId = this.parentNode.id.replace('controls_','');
	formatText(execID, iframeId, null);
}

/* Link replacement functions */
function convertLinksInput(mstr, m1, m2, m3, offset, s) {
	return createLink(null,null,m1,m2,m3,true);
}
function convertLinksOutput(mstr, m1, m2, m3, offset, s) {
	return ("["+m1+":"+m2+":"+m3+"]");
}
/* Replacement functions
 * @param str String to replace identifiers
 */
function convert_input(str) {
	str = str.replace(/\[b\]/mgi,'<b>');
	str = str.replace(/\[\/b\]/mgi,'</b>');
	str = str.replace(/\[i\]/mgi,'<i>');
	str = str.replace(/\[\/i\]/mgi,'</i>');
	str = str.replace(/\[u\]/mgi,'<u>');
	str = str.replace(/\[\/u\]/mgi,'</u>');
	str = str.replace(/\[s\]/mgi,'<strike>');
	str = str.replace(/\[\/s\]/mgi,'</strike>');
	str = str.replace(/\[ul\]/mgi,'<ul>');
	str = str.replace(/\[\/ul\]/mgi,'</ul>');
	str = str.replace(/\[ol\]/mgi,'<ol>');
	str = str.replace(/\[\/ol\]/mgi,'</ol>');
	str = str.replace(/\[li\]/mgi,'<li>');
	str = str.replace(/\[\/li\]/mgi,'</li>');
	str = str.replace(/\[br\]/mgi,'<br>');
	str = str.replace(/\n/mgi,'<br>');
	str = str.replace(/\%nbsp\;/mgi,' ');
	/* IE and opera support */
	str = str.replace(/\<strong\>/mgi,'<b>');
	str = str.replace(/\<\/strong\>/mgi,'<\b>');
	str = str.replace(/\<STRONG\>/mgi,'<b>');
	str = str.replace(/\<\/STRONG\>/mgi,'<\b>');
	str = str.replace(/\<em\>/mgi,'<i>');
	str = str.replace(/\<\/em\>/mgi,'<\i>');
	str = str.replace(/\<p\>/mgi,'');
	str = str.replace(/\<\/p\>/mgi,'<br>');
	str = str.replace(/\[([a-z].+?)\:(\d+)\:([^\:\\\/\[\]].+?)\]/mgi,convertLinksInput);
	return (str);
}
/* Function that formats outputs
 * @param n Number of the textArea
 */
function format_output(n) {
	var textArea = document.getElementById('textArea_'+n);
	if (!textArea) return;
	var str = textArea.value;
	str = str.replace(/\<b\>/mgi,'[b]');
	str = str.replace(/\<\/b\>/mgi,'[/b]');
	str = str.replace(/\<i\>/mgi,'[i]');
	str = str.replace(/\<\/i\>/mgi,'[/i]');
	str = str.replace(/\<u\>/mgi,'[u]');
	str = str.replace(/\<\/u\>/mgi,'[/u]');
	str = str.replace(/\&lt\;span style\=\&quot;text\-decoration\: underline\;\&quot\;\&gt\;(.+?)\&lt\;\/span\&gt\;/mgi,'[u]$1[/u]');
	str = str.replace(/\<strike\>/mgi,'[s]');
	str = str.replace(/\<\/strike\>/mgi,'[/s]');
	str = str.replace(/\&lt\;span style\=\&quot;text\-decoration\: line\-through\;\&quot\;\&gt\;(.+?)\&lt\;\/span\&gt\;/mgi,'[s]$1[/s]');
	str = str.replace(/\<ul\>/mgi,'[ul]');
	str = str.replace(/\<\/ul\>/mgi,'[/ul]');
	str = str.replace(/\<ol\>/mgi,'[ol]');
	str = str.replace(/\<\/ol\>/mgi,'[/ol]');
	str = str.replace(/\<li\>/mgi,'[li]');
	//str = str.replace(/\<li\>\<br \/\>/mgi,'[li]');
	str = str.replace(/\<\/li\>/mgi,'[/li]');
	str = str.replace(/\<br\>/mgi,'[br]');
	/* IE and opera support */
	str = str.replace(/\<p\>/mgi,'');
	str = str.replace(/\<\/p\>/mgi,'[br]');
	str = str.replace(/\<STRONG\>/mgi,'[b]');
	str = str.replace(/\<\/STRONG\>/mgi,'[/b]');
	str = str.replace(/\<strong\>/mgi,'[b]');
	str = str.replace(/\<\/strong\>/mgi,'[/b]');
	str = str.replace(/\<em\>/mgi,'[i]');
	str = str.replace(/\<\/em\>/mgi,'[/i]');
	str = str.replace(/\<a href\=\".+?\" type="(.+?)" att="(.+?)"\>(.+?)\<\/a\>/mgi,convertLinksOutput);
	/* Safari support */
	if (isWK) {
		str = str.replace(/\<span class\=\"Apple\-style\-span\" style\=\"text\-decoration\: underline\; \"\>(.+?)\<\/span\>/mgi,'[u]$1[/u]');
		str = str.replace(/\<span class\=\"Apple\-style\-span\" style\=\"text\-decoration\: line-through\; \"\>(.+?)\<\/span\>/mgi,'[i]$1[/i]');
		str = str.replace(/\<div\>/mgi,'');
		str = str.replace(/\<\/div\>/mgi,'[br]');
	}
	textArea.value = str;
}

/* Initializes RTE's */
function init_formating() {
	// detection rar wants for his shinny firefox 1.0.7
	if (!document.getElementsByTagName) return; // Can't do a thing..
	if (!document.designMode) return;
	var textAreas = document.getElementsByTagName('textarea');
	if (!textAreas.length) return; // Still no nodes..

	FunctionMap = {'bold':{'id':"Bold",'desc':"Bold text: *text* (alt+b)",'accesskey':"b",'text':"B",'start':"*",'end':"*",'active':false},
	               'italic':{'id':"Italic",'desc':"Italic text: /text/ (alt+i)",'accesskey':"i",'text':"I",'start':"/",'end':"/",'active':false},
	               'underline':{'id':"Underline",'desc':"Underline text: __text__ (alt+u)",'accesskey':"u",'text':"U",'start':"__",'end':"__",'active':false},
	               'strikethrough':{'id':"Strikethrough",'desc':"Strike-through text: --text-- (alt+s)",'accesskey':"S",'text':"s",'start':"--",'end':"--",'active':false},
	               'orderedlist':{'id':"InsertOrderedList",'desc':"Ordered item: # text (alt+o)",'accesskey':"o",'text':"oLi",'start':"# ",'end':"",'active':false},
	               'unorderedlist':{'id':"InsertUnorderedList",'desc':"Unordered item: * text (alt+l)",'accesskey':"l",'text':"uLi",'start':"* ",'end':"",'active':false},
	               'link':{'id':"CreateLink",'desc':"Link text: [type:attribute:text] (alt+h)",'accesskey':"h",'text':"LNK",'start':"[",'end':"]",'active':false}};
	OptionsMap = new Array('anime','ep','file','group','producer','producers','user','mylist','votes','creq','review','titles','genres',
							'cats','relations','groupcmts','reviews','tracker','wiki','forum.board','forum.topic','forum.post');
	
	// This will create the formating icons for each TextArea
	for (var i = 0; i < textAreas.length; i++) {
		var textArea = textAreas[i];
		textArea.id = "textArea_"+i;
		//textArea.style.display = "none";
		var fTA = textArea.form;
		var span = document.createElement('span');
		span.className = 'f_controls';
		span.id = 'controls_'+ i;
		// yes, yes, i know, blame IE
		createLocalButton(span,'bold',FunctionMap['bold']);
		createLocalButton(span,'italic',FunctionMap['italic']);
		createLocalButton(span,'underline',FunctionMap['underline']);
		createLocalButton(span,'strikethrough',FunctionMap['strikethrough']);
		createLocalButton(span,'orderedlist',FunctionMap['orderedlist']);
		createLocalButton(span,'unorderedlist',FunctionMap['unorderedlist']);
		createLocalButton(span,'link',FunctionMap['link']);
		createSelect(span,OptionsMap,'f_links');
		var html = getElementsByClassName(span.getElementsByTagName('input'),'f_view_source',true)[0];
		var text = getElementsByClassName(span.getElementsByTagName('input'),'f_view_text',true)[0];
		if (html) html.style.display = ''; 
		if (text) text.style.display = 'none';
		span.appendChild(document.createElement('br'));
		textArea.parentNode.insertBefore(span,textArea);
		// Create iframe which will be used for rich text editing
		var iframe = document.createElement('iframe');
		iframe.frameborder = 0;
		iframe.id = "wysiwyg_" + i;
		iframe.style.height = wysiwygHeight + "px";
		iframe.style.width = wysiwygWidth + "px";
		iframe.borderWidth = 0;
		textArea.parentNode.insertBefore(iframe,textArea);
		// Pass the textarea's existing text over to the content variable
		var content = convert_input(textArea.value);
		var doc = iframe.contentWindow.document;
		// Write the textarea's content into the iframe
		doc.open();
		var htmlNode = doc.firstChild; // only works for firefox
		if (htmlNode) {
			var head = null;
			for (var n = 0; n < htmlNode.childNodes.length; n++) {
				if (htmlNode.childNodes[n].nodeName.toLowerCase() == 'head') 
					head = htmlNode.childNodes[n];
			}
			if (!head) {
				head = document.createElement('head');
				htmlNode.appendChild(head);
			}
			for (var ss = 0; ss < document.styleSheets.length; ss++) {
				var stylesheet = document.createElement('link');
				stylesheet.title = document.styleSheets[ss].title;
				stylesheet.type = 'text/css';
				stylesheet.href = document.styleSheets[ss].href;
				stylesheet.rel = (ss ? 'alternate ' : '' ) + 'stylesheet';
				head.appendChild(stylesheet);
			}
			// creating the new stylesheet
			var stylesheet = document.createElement('style');
			stylesheet.type = 'text/css';
			head.appendChild(stylesheet);
			var x = doc.styleSheets[doc.styleSheets.length - 1];
			//x.insertRule('body { background-image: none; }',x.cssRules.length);
			x.insertRule('html { background-image: none; }',x.cssRules.length);
		}
		doc.write(content);
		doc.close();
		// Make the iframe editable in both Mozilla and IE
		doc.body.contentEditable = true;
		doc.designMode = "on";
		// need to activate this on firefox
		// this is a stupid detection method but it works, 
		// all browsers support queryCommandSupported (at least there's native code for it)
		// but only IE and Opera return false for not supported Firefox only styleWithCSS
		// Firefox trows, you have to love this kind of stuff or else you will go mad
		try { if (document.queryCommandSupported('styleWithCSS')) true; } 
		catch(e) { 
			try { doc.execCommand('styleWithCSS', null, false); } // FF 1.5+
			catch (e) {  } // FF 1.5-
		}
		var inputs = fTA.getElementsByTagName('input');
		for (var s = 0; s < inputs.length; s++) {
			var input = inputs[s];
			if (input.type != 'submit') continue;
			input.onclick = updateTextAreaCK;
			input.type = 'button';
			break;
		}
	}
}

function prepPage() {
	uriObj = parseURI(); // update the uriObj
	if (!uriObj['show']) return; // go away evil page!
	switch (uriObj['show']) { // list of pages where to apply formating stuff
		case 'animeatt':
			if      (uriObj['new'] && uriObj['new'] == '1') init_formating();
			else if (uriObj['showcomment'] && uriObj['showcomment'] == '1') init_formating();
			else if (uriObj['edit'] && uriObj['edit'] == '1') init_formating();
			else return;
			break;
		case 'msg':
			if      (uriObj['do'] && uriObj['do'] == 'new') init_formating();
			else return;
			break;
		case 'cmt':
			if      (uriObj['cmt.do.add'] || uriObj['cmt.do.edit']) init_formating();
			else return;
			break;
		default: return;
	}
}

//window.onload = prepPage;
addLoadEvent(prepPage);
