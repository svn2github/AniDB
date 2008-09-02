/* file message page scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 * @contents Core Functions
 *           Formating
 * version 1.2 (18.06.2007) - Previous version
 * version 2.0 (20.04.2008) - Rather full support for every major browser Firefox, IE, Opera and Safari
 * version 2.1 (27.07.2008) - Introduction of Assisted Mode
 */

// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
// WYSIWYG Width and Height
var wysiwygWidth = 100;
var wysiwygHeight = 20;
// Create viewTextMode global variable and set to 0
// enabling all toolbar commands while in HTML mode
var viewTextMode = 0;
var current_ed; // current editor
var isIE = (document.selection != undefined && window.getSelection == undefined) ? true : false;
var isFF = (document.selection == undefined && window.getSelection != undefined) ? true : false;
var isOP = (document.selection != undefined && window.getSelection != undefined) ? true : false;
var isWK = (navigator.userAgent.toLowerCase().indexOf('applewebkit') >= 0);
if (navigator.userAgent.toLowerCase().indexOf('msie') >= 0) isIE = true; // new ie's work for everything
var currentFMode = CookieGet('currentFMode') || 2;
if (isWK) currentFMode = 1;

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
	var html = getElementsByClassName(fTA.form.getElementsByTagName('input'),'f_viewsource',true)[0];
	var text = getElementsByClassName(fTA.form.getElementsByTagName('input'),'f_viewrte',true)[0];
	var clean = getElementsByClassName(fTA.form.getElementsByTagName('input'),'f_cleansource',true)[0];
	var preview = getElementsByClassName(fTA.form.getElementsByTagName('input'),'f_preview',true)[0];
	if (!html && !text && !clean && !preview) return; // no buttons nothing, to do
	if (type == 'source') {
		if (html) html.style.display = 'none'; 
		if (text) text.style.display = '';
		if (clean) clean.style.display = 'none';
		if (preview) preview.style.display = 'none';
		// set the font values for displaying HTML source
		getDocument.body.style.fontSize = "12px";
		getDocument.body.style.fontFamily = "Courier New"; 
		viewTextMode = 1;
	} else {
		if (html) html.style.display = ''; 
		if (text) text.style.display = 'none';
		if (clean) clean.style.display = '';
		if (preview) preview.style.display = '';
		// reset the font values
		getDocument.body.style.fontSize = "";
		getDocument.body.style.fontFamily = ""; 
		viewTextMode = 0;
	}
	var cln = this.className;
	if (cln && cln.indexOf('f_selected') > 0) this.className = this.className.replace(' f_selected','');
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
	if (currentFMode == 2) {
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
	} else {
		if (isIE) {
			myField.focus();
			sel = document.selection.createRange();
			sel.text = myValue;
			myField.focus();
		} else if (isFF || isOp) {
			var startPos = myField.selectionStart;
			var endPos = myField.selectionEnd;
			myField.value = myField.value.substring(0, startPos)
		                + myValue
		                + myField.value.substring(endPos, myField.value.length);
			myField.focus();
			myField.selectionStart = startPos + myValue.length;
			myField.selectionEnd = startPos + myValue.length;
		} else {
			if (seeDebug) alert('insertAtSelection: unknown selection method');
			return;		
		}
	}
}

/* Function that gets the current selection
 * @param myField Where the cursor/selection is located
 * @return 
 */
function getSelection(myField) {
	if (currentFMode == 2) {
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
	} else {
		if (isIE) { //IE support
			myField.focus();
			sel = document.selection.createRange();
			return (sel.text);
		} else if (isFF || isOP) { //MOZILLA/NETSCAPE support
			var startPos = myField.selectionStart;
			var endPos = myField.selectionEnd;
			return myField.value.substring(startPos, endPos);
		} else {
			if (seeDebug) alert('getSelection: unknown selection method');
			return;
		}
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
	var acceptedVals = ['anime','creq','ep','file','group','producer','reviews','mylist',
						'votes','titles','producers','genres','cats','relations','user',
						'review','groupcmts','wiki','forum','furum.topic','forum.board',
						'forum.post','tracker'];
	if ((!obj && !fTA) && acceptedVals.indexOf(val) < 0) return; // not a valid thingie
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
	if (currentFMode == 1) {
		insertAtSelection(fTA, hyperLink, true);
		return;
	}
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

/* This function creates and inserts spoiler tags at the selection
 * @param obj The RTE obj
 * @param fTA The underlying textArea
 * @param textOnly true if only the text of the link is needed
 */
function createSpoiler(obj,fTA,sel,textOnly) {
	if (!obj && !fTA) return; // can not continue
	if (!sel) {
		sel = getSelection(obj);
		if (!sel) sel = "";
	}
	var spoiler = '[spoiler]'+sel+'[/spoiler]';
	if (textOnly) return spoiler;
	if (isIE) {	//IE support
		// IE fails miserably here, don't show the link thingie
		insertAtSelection(obj, spoiler, true);
	} else if (isFF || isOP) { //MOZILLA/NETSCAPE support
		insertAtSelection(obj, obj.document.createTextNode(spoiler), false);
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
	if (currentFMode == 2) {
		// When user clicks toolbar button make sure it always targets its respective WYSIWYG
		var obj = document.getElementById("wysiwyg_" + n);
		if (!obj) return;
		obj.contentWindow.focus();
	}
	var fTA = document.getElementById('textArea_' + n);
	// When in Text Mode these execCommands are disabled
	var formatIDs = new Array("Bold","Italic","Underline","Strikethrough","InsertUnorderedList","InsertOrderedList","Spoiler","CreateLink");
	for (var i = 0; i <= formatIDs.length;) { // Check if button clicked is in disabled list
		if (formatIDs[i] == id) var disabled_id = 1; 
		i++;
	}
	if (viewTextMode == 1 && disabled_id == 1) { // Check if in Text Mode and disabled button was clicked
		alert ("You are in TEXT Mode. This feature has been disabled.");	
	} else {
		if (currentFMode == 2) {
			if (id == "CreateLink") { // CreateLink
				createLink(obj.contentWindow, fTA, null, false);
			} else if (id == "Spoiler") { // Spoiler
				createSpoiler(obj.contentWindow, fTA, null, false);
			} else if (id == "ViewSource") { // ViewSource
				viewMode('source',n);
			} else if (id == "ViewRte") { // ViewRte
				viewMode('text',n);
			} else if (id == "CleanSource") { // Clean Source
				cleanHTML(obj.contentWindow.document);
			} else if (id == "Preview") { // Preview document
				previewDoc(obj.contentWindow.document);
			} else { // Every other command
				obj.contentWindow.document.execCommand(id, false, null);
			}
		} else {
			if (id == "CreateLink") { // CreateLink
				createLink(fTA, fTA, null, false);
			} else if (id == "Preview") { // Preview document
				previewDoc(fTA);
			} else { // Every other command
				textFormatMagic(id,fTA);
			}
		}
	}
}

/* Function that takes care of assisted formating
 * @param id 
 * @param fTA TextArea
 */
function textFormatMagic(id,fTA) {
	var selection = getSelection(fTA);
	var action = id.toLowerCase();
	//if (selection && selection.length) 
	insertAtSelection(fTA, FunctionMap[action]['start']+selection+FunctionMap[action]['end'], true);
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
	var link = createLink(null,null,m1,m2,m3,true);
	return (link == null ? mstr : link);
}
function convertLinksOutput(mstr, m1, m2, m3, offset, s) {
	var href = att = type = "";
	var hrefIndex = mstr.indexOf('href');
	if (hrefIndex >= 0) {
		var hrefIndex0 = mstr.indexOf('"',hrefIndex)+1;
		var hrefIndex1 = mstr.indexOf('"',hrefIndex0+1);
		href = mstr.substring(hrefIndex0,hrefIndex1);
	}
	var attIndex = mstr.indexOf('att');
	if (attIndex >= 0) {
		var attIndex0 = mstr.indexOf('"',attIndex)+1;
		var attIndex1 = mstr.indexOf('"',attIndex0+1);
		att = mstr.substring(attIndex0,attIndex1);
	}
	var typeIndex = mstr.indexOf('type');
	if (typeIndex >= 0) {
		var typeIndex0 = mstr.indexOf('"',typeIndex)+1;
		var typeIndex1 = mstr.indexOf('"',typeIndex0+1);
		type = mstr.substring(typeIndex0,typeIndex1);
	}
	if (href.indexOf('anidb.net') < 0) return ('(link: ' + m2+' - '+href+')');
	return ("["+type+":"+att+":"+m2+"]");
}

function areTwoEqual(mstr,s1,s2) {
	if (s1 == s2) return ' ';
	else return mstr;
}

/* Function that cleans up html source
 * @param str String
 */
function cleanHTMLSource(str) {
	str = str.replace(/\n/mgi,'');
	
	str = str.replace(/\<[/]?(div|font|span|xml|del|ins|img|h\d|[ovwxp]:\w+)[^>]*?\>/mgi,'');
	str = str.replace(/\<\!\-\-\[.+?\]\-\-\>/mgi,'');

	str = str.replace(/\<(p|u|b|i|ul|ol|li|strike|br|pre) [^>]*?\>/mgi,'<$1>');
	str = str.replace(/\<\/(p|u|b|i|ul|ol|li|strike|br|pre) [^>]*?\>/mgi,'</$1>');

	str = str.replace(/\<p\>/mgi,'<br />');
	str = str.replace(/\<\/p\>/mgi,'');
	
	str = str.replace(/\[([a-z].+?)\:(\d+)\:([^\:\\\/\[\]].+?)\]/mgi,convertLinksInput);
	
	/* IE and opera support */
	str = str.replace(/\<([/])?strong ([^>]*)\>/mgi,'<$1b>');
	str = str.replace(/\<([/])?em ([^>]*)\>/mgi,'<$1i>');
	/* Extra stuff, mostly word paste */
	/* Remove empty tags */
	str = str.replace(/\<(p|u|b|i|ul|ol|li)\>\<\/(p|u|b|i|ul|ol|li)\>/mgi,areTwoEqual);
	return (str)
}

/* Replacement functions
 * @param str String to replace identifiers
 */
function convert_input(str) {
	str = str.replace(/\[(p|b|i|u|ul|ol|li)+?\]/mgi,'<$1>');
	str = str.replace(/\[\/(p|b|i|u|ul|ol|li)+?\]/mgi,'</$1>');
	str = str.replace(/\[([/])?s\]/mgi,'<$1strike>');
	str = str.replace(/\[br\]/mgi,'<br />');
	str = str.replace(/\n/mgi,'');
	/* IE and opera support */
	str = str.replace(/\[([a-z].+?)\:(\d+)\:([^\:\\\/\[\]].+?)\]/mgi,convertLinksInput);
	return (str);
}

/* Function that formats an output string
 * @param str String to format
 */
function convert_output(str) {
	/* Extra stuff, mostly word paste */
	str = str.replace(/\&nbsp\;/mgi,' ');
	str = str.replace(/\<[/]?(div|font|span|xml|del|ins|img|h\d|[ovwxp]:\w+)[^>]*?\>/mgi,'');
	str = str.replace(/\<\!\-\-\[.+?\]\-\-\>/mgi,'');

	str = str.replace(/\n/mgi,(currentFMode == 2 ? '' : '[br]'));
	str = str.replace(/\<br([^>]*)\>/mgi,'[br]');
	str = str.replace(/\<(p|u|b|i|ul|ol|li|strike|pre) [^>]*?\>/mgi,'[$1]');
	str = str.replace(/\<\/(p|u|b|i|ul|ol|li|strike|pre) [^>]*?\>/mgi,'[/$1]');
	str = str.replace(/\<(p|u|b|i|ul|ol|li|strike|pre)\>/mgi,'[$1]');
	str = str.replace(/\<\/(p|u|b|i|ul|ol|li|strike|pre)\>/mgi,'[/$1]');
	str = str.replace(/\[strike\]/mgi,'[s]');
	str = str.replace(/\[\/strike\]/mgi,'[/s]');
	str = str.replace(/\[pre\]/mgi,'[code]');
	str = str.replace(/\[\/pre\]/mgi,'[/code]');
	str = str.replace(/\<a(.+?)\>(.+?)\<\/a\>/mgi,convertLinksOutput);

	str = str.replace(/\[p\]/mgi,'[br]');
	str = str.replace(/\[\/p\]/mgi,'');

	/* Safari support */
/*
	str = str.replace(/\<span.+?font\-weight\: bold.+?\>(.+?)\<\/span\>/mgi,'[b]$1[/b]');
	str = str.replace(/\<span.+?text\-style\: italic.+?\>(.+?)\<\/span\>/mgi,'[i]$1[/i]');
	str = str.replace(/\<span.+?text\-decoration\: underline.+?\>(.+?)\<\/span\>/mgi,'[u]$1[/u]');
	str = str.replace(/\<span.+?text\-decoration\: line\-through.+?\>(.+?)\<\/span\>/mgi,'[s]$1[/s]');	
*/
	/* IE and opera support */
	str = str.replace(/\<([/])?strong([^>]*)\>/mgi,'[$1b]');
	str = str.replace(/\<([/])?em([^>]*)\>/mgi,'[$1i]');
	/* Remove empty tags */
	str = str.replace(/\[(p|u|b|i|ul|ol|li)\]\[\/(p|u|b|i|ul|ol|li)\]/mgi,areTwoEqual);
	return (str);
}

/* Function that formats outputs
 * @param n Number of the textArea
 */
function format_output(n) {
	var textArea = document.getElementById('textArea_'+n);
	if (!textArea) return;
	textArea.value = convert_output(textArea.value);
}

/* Function that cleans the html of an RTE window
 * @param myField Editor document
 */
function cleanHTML(myField) {
	if (!myField) return;
	myField.body.innerHTML = cleanHTMLSource(myField.body.innerHTML);
}

/* Function tha opens up a preview window
 * @param myField Editor document
 */
function previewDoc(myField) {
	if (!myField) return;
	var html = (currentFMode == 2 ? myField.body.innerHTML : myField.value);
	var previewWindow = window.open('','previewWin','height=250,width=500,resizable=yes,scrollbars=yes,toolbar=no,status=no');
	previewWindow.document.write('<!DOCTYPE html><html lang="en"><head><title>Text Preview</title>');
	previewWindow.document.write('<meta http-equiv="content-type" content="text/html; charset=UTF-8"/>');
	previewWindow.document.write('<link rel="icon" href="favicon.ico"/>');
	previewWindow.document.write('<link rel="stylesheet" href="'+document.styleSheets[0].href+'" type="text/css" title="Style"/>');
	previewWindow.document.write('</head><body id="anidb" class="nonav">');
	previewWindow.document.write('<div id="layout-main"><div class="g_content">');
	previewWindow.document.write(convert_input(convert_output(html)))
	previewWindow.document.write('</div></div>');
	previewWindow.document.write('</body></html>');
	previewWindow.document.close();
}

/* Initializes RTE's */
function init_formating() {
	// detection rar wants for his shinny firefox 1.0.7
	if (!document.getElementsByTagName) return; // Can't do a thing..
	if (!document.designMode) return;
	var textAreas = document.getElementsByTagName('textarea');
	if (!textAreas.length) return; // Still no nodes..

	FunctionMap = {'bold':{'id':"Bold",'desc':"Bold text: [b]text[/b] (alt+b)",'accesskey':"b",'text':"B",'start':"[b]",'end':"[/b]",'active':false},
	               'italic':{'id':"Italic",'desc':"Italic text: [i]text[/i] (alt+i)",'accesskey':"i",'text':"I",'start':"[i]",'end':"[/i]",'active':false},
	               'underline':{'id':"Underline",'desc':"Underline text: [u]text[/u] (alt+u)",'accesskey':"u",'text':"U",'start':"[u]",'end':"[/u]",'active':false},
	               'strikethrough':{'id':"Strikethrough",'desc':"Strike-through text: [s]text[/s] (alt+s)",'accesskey':"S",'text':"s",'start':"[s]",'end':"[/s]",'active':false},
	               'insertorderedlist':{'id':"InsertOrderedList",'desc':"Ordered list: [ol]text[/ol] (alt+o)",'accesskey':"o",'text':"oLi",'start':"[ol]",'end':"[/ol]",'active':false},
	               'insertunorderedlist':{'id':"InsertUnorderedList",'desc':"Unordered list: [ul]text[/ul] (alt+l)",'accesskey':"l",'text':"uLi",'start':"[ul]",'end':"[/ul]",'active':false},
				   'insertlistitem':{'id':"InsertListItem",'desc':"List Item: [li]text[/li] (alt+e)",'accesskey':"e",'text':"Li",'start':"[li]",'end':"[/li]",'active':false},
				   'spoiler':{'id':"Spoiler",'desc':"Spoiler: [spoiler]text[/spoiler]",'accesskey':"",'text':"spoiler",'start':'[spoiler]','end':'[/spoiler]','active':false},
				   'viewsource':{'id':"ViewSource",'desc':"Source Mode",'accesskey':"q",'text':"vSource",'start':null,'end':null,'active':false},
				   'viewrte':{'id':"ViewRte",'desc':"WYSIWYG Mode",'accesskey':"w",'text':"vWYSIWYG",'start':null,'end':null,'active':false},
				   'cleansource':{'id':"CleanSource",'desc':"Clean Source (alt+c)",'accesskey':"c",'text':"Clean",'start':null,'end':null,'active':false},
				   'preview':{'id':"Preview",'desc':"Preview",'accesskey':"",'text':"preview",'start':null,'end':null,'active':false},
	               'link':{'id':"CreateLink",'desc':"Link text: [type:attribute:text] (alt+h)",'accesskey':"h",'text':"LNK",'start':"[",'end':"]",'active':false}};
	OptionsMap = new Array('anime','ep','file','group','producer','producers','user','mylist','votes','creq','review','titles','genres',
							'cats','relations','groupcmts','reviews','tracker','wiki','forum.board','forum.topic','forum.post');
	
	// This will create the formating icons for each TextArea
	for (var i = 0; i < textAreas.length; i++) {
		var textArea = textAreas[i];
		if (textArea.className.indexOf('norte') >= 0) continue;
		textArea.id = "textArea_"+i;
		if (currentFMode == 2) textArea.style.display = "none";
		var fTA = textArea.form;
		var div = document.createElement('div');
		div.className = 'format-buttons f_controls';
		div.id = 'controls_'+ i;
		// yes, yes, i know, blame IE
		createLocalButton(div,'bold',FunctionMap['bold']);
		createLocalButton(div,'italic',FunctionMap['italic']);
		createLocalButton(div,'underline',FunctionMap['underline']);
		createLocalButton(div,'strikethrough',FunctionMap['strikethrough']);
		createLocalButton(div,'insertorderedlist',FunctionMap['insertorderedlist']);
		createLocalButton(div,'insertunorderedlist',FunctionMap['insertunorderedlist']);
		if (currentFMode == 1) createLocalButton(div,'insertlistitem',FunctionMap['insertlistitem']);
		createLocalButton(div,'spoiler',FunctionMap['spoiler']);
		if (currentFMode == 2) {
			createLocalButton(div,'viewsource',FunctionMap['viewsource']);
			createLocalButton(div,'viewrte',FunctionMap['viewrte']);
			createLocalButton(div,'cleansource',FunctionMap['cleansource']);
		}
		createLocalButton(div,'preview',FunctionMap['preview']);
		createLocalButton(div,'link',FunctionMap['link']);
		createSelect(div,OptionsMap,'f_links');
		var html = getElementsByClassName(div.getElementsByTagName('input'),'f_viewsource',true)[0];
		var text = getElementsByClassName(div.getElementsByTagName('input'),'f_viewrte',true)[0];
		var clean = getElementsByClassName(div.getElementsByTagName('input'),'f_cleansource',true)[0];
		var preview = getElementsByClassName(div.getElementsByTagName('input'),'f_preview',true)[0];
		if (html) html.style.display = ''; 
		if (text) text.style.display = 'none';
		if (clean) clean.style.display = '';
		if (preview) preview.style.display = '';
		var insertHere = document.getElementById('smiley-box');
		insertHere.parentNode.insertBefore(div,insertHere);
		//textArea.parentNode.insertBefore(div,textArea);
		if (currentFMode == 2) {
			// Create iframe which will be used for rich text editing
			var backgroundColor = getStyleInformation(textArea,'backgroundColor');
			var color = getStyleInformation(textArea,'color');
			var fontFamily = getStyleInformation(textArea,'fontFamily');
			var fontSize = getStyleInformation(textArea,'fontSize');
			var iframe = document.createElement('iframe');
			
			iframe.frameborder = 0;
			iframe.id = "wysiwyg_" + i;
			iframe.style.height = wysiwygHeight + "em";
			iframe.style.width = wysiwygWidth + "%";
			iframe.borderWidth = 0;
			textArea.parentNode.insertBefore(iframe,textArea);
			// Pass the textarea's existing text over to the content variable
			var content = convert_input(textArea.value);
			var doc = iframe.contentWindow.document;
			// Write the textarea's content into the iframe
			doc.open();
			doc.write(content);
			doc.close();
			// Make the iframe editable in both Mozilla and IE
			doc.body.contentEditable = true;
			if (color && color != '') doc.body.style.color = color;
			if (fontFamily && fontFamily != '') doc.body.style.fontFamily = fontFamily;
			if (!isIE && fontSize && fontSize != '') doc.body.style.fontSize = fontSize;
			if (backgroundColor && backgroundColor != '') doc.body.style.backgroundColor = backgroundColor;
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
		}
		var inputs = fTA.getElementsByTagName('input');
		for (var s = 0; s < inputs.length; s++) {
			var input = inputs[s];
			if (input.type != 'submit') continue;
			input.onclick = updateTextAreaCK;
			break;
		}
	}
}

function prepPage() {
	if (!currentFMode) return; // disabled
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
