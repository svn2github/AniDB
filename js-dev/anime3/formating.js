/* file message page scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 * @contents Core Functions
 *           Formating
 * version 1.2 (18.06.2007) - Previous version
 * version 2.0 (20.04.2008) - Rather full support for every major browser Firefox, IE, Opera and Safari
 * version 2.1 (27.07.2008) - Introduction of Assisted Mode
 * version 2.2 (02.09.2008) - Rewrote somestuff
 * version 3.0 (22.09.2008) - Major rewrite
 */

// GLOBALS
var seeDebug = CookieGet('seeDebug') || 0;
var wysiwygWidth = "100%";
var wysiwygHeight = "20em";
var viewTextMode = 0;
var dS = (document.selection != undefined) ? true : false;
var wGS = (window.getSelection != undefined) ? true : false;
var isWK = (navigator.userAgent.toLowerCase().indexOf('webkit') >= 0);
var isIE = (navigator.userAgent.toLowerCase().indexOf('msie') >= 0);
var isOP = (navigator.userAgent.toLowerCase().indexOf('presto') >= 0 || navigator.userAgent.toLowerCase().indexOf('opera') >= 0);
var isFF = (!isWK && navigator.userAgent.toLowerCase().indexOf('gecko') >= 0 || navigator.userAgent.toLowerCase().indexOf('firefox') >= 0);
var currentFMode = CookieGet('currentFMode') || 2;
if (isWK && currentFMode == 2) currentFMode = 1;
var smileyList = ['very_happy','happy','sad','shock','confused','cool','laughing','razz','embarassed','crying',
				  'mad','twisted','rolleyes','wink','neutral','sweating','undecided','thinking','wtf','tehehe',
				  'sweatdrop','disgusted','surprised','dead','nosebleed','brickwall','zzz','mymaster','thumbup','angel',
				  'baka'];
var smileys = [
		{'id':"01",'name':"very_happy",'title':"very happy",'text':":grin:",'targets':'(?<=[^;]):-?D|:grin:'},
		{'id':"02",'name':"happy",'title':"happy",'text':':smile:','targets':'(?<=[^;]):-?\)|\^\^;?|:smile:'},
		{'id':"03",'name':"sad",'title':"sad",'text':':sad:','targets':'(?<=[^;]):-?\(|:sad:'},
		{'id':"04",'name':"shock",'title':"shocked",'text':':shock:','targets':'O_O|:shock:'},
		{'id':"05",'name':"confused",'title':"confused",'text':':S','targets':':-?\?|:s|:S'},
		{'id':"06",'name':"cool",'title':"cool",'text':':cool:','targets':'(?<=[ >a-zA-Z])8-?\)|^8-?\)|:cool:'},
		{'id':"07",'name':"laughing",'title':"laughing",'text':':lol:','targets':':lol:'},
		{'id':"08",'name':"razz",'title':"razz",'text':':razz:','targets':':-?[Pp]|:razz:'},
		{'id':"09",'name':"embarassed",'title':"embarassed",'text':':oops:','targets':':oops:'},
		{'id':"10",'name':"crying",'title':"crying or very sad",'text':':cry:','targets':';_;|:cry:'},
		{'id':"11",'name':"mad",'title':"evil or very mad",'text':':evil:','targets':'&gt;[:;]?\(|:evil:'},
		{'id':"12",'name':"twisted",'title':"twisted evil",'text':':twisted:','targets':'&gt;[;:]?:?D|:twisted:'},
		{'id':"13",'name':"rolleyes",'title':"rolling eyes",'text':':roll:','targets':':roll:|:rolleyes:'},
		{'id':"14",'name':"wink",'title':"wink",'text':':wink:','targets':';-?\)|:wink:'},
		{'id':"15",'name':"neutral",'title':"neutral",'text':':neutral:','targets':':-?\||:neutral:'},
		{'id':"16",'name':"sweating",'title':"upset, sighing",'text':'-_-','targets':'-_-;?'},
		{'id':"17",'name':"undecided",'title':"undecided",'text':':/','targets':'(?<=[^p]):-?\/'},
		{'id':"18",'name':"thinking",'title':"thinking",'text':'O_o','targets':'O_o'},
		{'id':"19",'name':"wtf",'title':"huh?/wtf?",'text':'o_O','targets':'o_O;?'},
		{'id':"20",'name':"tehehe",'title':"tehehe",'text':'-.-','targets':'-\.-'},
		{'id':"21",'name':"sweatdrop",'title':"sweat drop",'text':'^_^','targets':'\^_?\^;?'},
		{'id':"22",'name':"disgusted",'title':"in pain/frustration",'text':'>_<','targets':'&gt;_&lt;'},
		{'id':"23",'name':"surprised",'title':"surprised",'text':':O','targets':':-?[oO]'},
		{'id':"24",'name':"dead",'title':"dead",'text':'x_X','targets':'[xX]_[xX]'},
		{'id':"25",'name':"nosebleed",'title':"nosebleed",'text':':nosebleed:','targets':'&gt;,&lt;|:nosebleed:'},
		{'id':"26",'name':"brickwall",'title':"bullheaded",'text':':brickwall:','targets':':brickwall:|:bash:'},
		{'id':"27",'name':"zzz",'title':"sleepy",'text':':zZz:','targets':':z[zZ]z:'},
		{'id':"28",'name':"mymaster",'title':"my master",'text':':mymaster:','targets':':mymaster:'},
		{'id':"29",'name':"thumbup",'title':"thumb up",'text':':thumbup:','targets':':thumbup:'},
		{'id':"30",'name':"angel",'title':"innocent",'text':':angel:','targets':':angel:'},
		{'id':"31",'name':"baka",'title':"idiot",'text':':baka:','targets':':baka:'}
	];

FunctionMap = {'bold':{'id':"Bold",'desc':"Bold text: [b]text[/b] (alt+b)",'accesskey':"b",'text':"b",'start':"[b]",'end':"[/b]",'active':false},
               'italic':{'id':"Italic",'desc':"Italic text: [i]text[/i] (alt+i)",'accesskey':"i",'text':"i",'start':"[i]",'end':"[/i]",'active':false},
               'underline':{'id':"Underline",'desc':"Underline text: [u]text[/u] (alt+u)",'accesskey':"u",'text':"u",'start':"[u]",'end':"[/u]",'active':false},
               'strikethrough':{'id':"Strikethrough",'desc':"Strike-through text: [s]text[/s] (alt+s)",'accesskey':"S",'text':"s",'start':"[s]",'end':"[/s]",'active':false},
               'insertorderedlist':{'id':"InsertOrderedList",'desc':"Ordered list: [ol]text[/ol] (alt+o)",'accesskey':"o",'text':"ol",'start':"[ol]",'end':"[/ol]",'active':false},
               'insertunorderedlist':{'id':"InsertUnorderedList",'desc':"Unordered list: [ul]text[/ul] (alt+l)",'accesskey':"l",'text':"ul",'start':"[ul]",'end':"[/ul]",'active':false},
			   'insertlistitem':{'id':"InsertListItem",'desc':"List Item: [li]text[/li] (alt+e)",'accesskey':"e",'text':"li",'start':"[li]",'end':"[/li]",'active':false},
			   'spoiler':{'id':"Spoiler",'desc':"Spoiler: [spoiler]text[/spoiler]",'accesskey':"",'text':"spoiler",'start':'[spoiler]','end':'[/spoiler]','active':false},
               'link':{'id':"CreateLink",'desc':"Link text: [type:attribute:text] (alt+h)",'accesskey':"h",'text':"href",'start':"[",'end':"]",'active':false},
			   'unlink':{'id':"RemoveLink",'desc':"Unlink",'accesskey':null,'text':"href",'start':"[",'end':"]",'active':false}
			   };
ModeMap = new Array('Mode: Off','Mode: Assisted','Mode: Visual');

/* Function that gets the current selection
 * @param field Where the cursor/selection is located
 * @param myValue text that will replace the current
 * @param isText if true will insert text instead of nodes
 */
function selectionMagic(field, myValue, isText) {
	var selectionObject;
	if (field.nodeName && field.nodeName.toLowerCase() == 'textarea') { // our nice textArea
		if (wGS) { // window.getSelection does not work for textareas (why? i have no fucking clue)
			var startPos = field.selectionStart;
			var endPos = field.selectionEnd;
			if (!myValue) return field.value.substring(startPos, endPos);
			field.value = field.value.substring(0, startPos) + myValue + field.value.substring(endPos, field.value.length);
			// this clears the current selection
			field.selectionStart = startPos + myValue.length;
			field.selectionEnd = startPos + myValue.length;
			return myValue;
		} else { // Microsoft Model (this has 3 hacks in it, find them!)
			var range = field.document.selection.createRange();
			if (!myValue) return range.text;
			var startPos = field.selectionStart();
			var endPos = field.selectionEnd();
			field.value = field.value.substring(0, startPos) + myValue + field.value.substring(endPos, field.value.length);
			// this clears the current selection
			range.move("character", startPos+myValue.length); 
			range.select();
			return myValue;
		}
	} else { // field should be a window, if not, it's hell
		try {
			if (wGS) selectionObject = field.getSelection();
			else if (dS) {
				//field.document.body.focus();
				var sel = field.document.selection.createRange();
				if (!myValue) return sel.text;
				alert(sel.text);
				if (isText) sel.text = myValue;
				else sel.pasteHTML(myValue);
				return myValue;
			}
			else { errorAlert('selectionMagic','unknown selection method'); return; }
			if (selectionObject.getRangeAt) {
				range = selectionObject.getRangeAt(0);
			} else { // very old webkit support
				range = field.document.selection.createRange();
				range.setStart(selectionObject.anchorNode,selectionObject.anchorOffset);
				range.setEnd(selectionObject.focusNode,selectionObject.focusOffset);
			}
			if (myValue) {
				range.deleteContents();
				var doc = field.document;
				if (isText) range.insertNode(doc.createTextNode(myValue));
				else range.insertNode(myValue);
			}
			return range.toString();
		} catch(error) {
			errorAlert('selectionMagic','trying to do window work on something that probably is not a window...');
			return '';
		}
	}
}

/* Get the caret position in a textarea (black magic IE hack) */
function getCaretPosition() {
	var caretPos = null;
	var range = this.document.selection.createRange(); // get current selection
	var range_all = this.document.body.createTextRange(); // a new selection of the whole textarea
	range_all.moveToElementText(this); 	// calculate selection start point by moving beginning of range_all to beginning of range
	var sel_start;
	for (sel_start = 0; range_all.compareEndPoints('StartToStart', range) < 0; sel_start++)
		range_all.moveStart('character', 1);
	return sel_start;
}

/* Get the start of a selection (black magic IE hack) */
function textAreaSelectionStart() { return this.getCaretPosition(); }
/* Get the end of a selection (black magic IE hack) */
function textAreaSelectionEnd() {
	var selection = this.document.selection.createRange().text;
	return this.selectionStart() + selection.length;
}

/* Function that formats text
 * @param id 
 * @param n Number of textArea
 * @param selected Is select
 */
function formatText(id, n, selected, element) {
	if (n == null) n = Number(this.parentNode.id.substring(iframe.id.indexOf('_')+1));
	var rte = document.getElementById("wysiwyg_" + n);
	var fta = document.getElementById('textArea_' + n);
	if (!rte || !fta) { errorAlert('formatText','no rte or no fta'); return; }
	var doc = rte.contentWindow.document;
	if (currentFMode == 2) rte.contentWindow.focus();
	else fta.focus();
	switch(id.toLowerCase()) {
		case 'createlink':
			var addLinkWidget = document.getElementById('widget_addlink');
			if (!addLinkWidget) { errorAlert('formatText','no addlink widget'); return; }
			addLinkWidget.style.display = '';
			addLinkWidget.style.left = element.offsetLeft + "px";
			addLinkWidget.style.top = element.offsetTop + element.offsetHeight + "px";
			var field = (currentFMode == 2 ? rte.contentWindow : fta);
			//if (currentFMode == 2) rte.contentWindow.focus(); else fta.focus();
			var selectionText = selectionMagic(field);
			var textField = document.getElementById('f_links_text');
			if (textField) textField.value = selectionText;
			var confirmButton = document.getElementById('f_links_ok');
			confirmButton.onclick = function addLink() {
				var addLinkWidget = document.getElementById('widget_addlink');
				if (!addLinkWidget) { errorAlert('formatText','no addlink widget'); return; }
				var rte = document.getElementById('wysiwyg_'+n);
				var fta = document.getElementById('textArea_'+n);
				var field = (currentFMode == 2 ? rte.contentWindow : fta);
				var textField = document.getElementById('f_links_text').value || '';
				var hrefField = document.getElementById('f_links_href').value || '';
				if (textField == '') textField = hrefField;
				var hyperLink = '[url='+hrefField+']'+textField+'[/url]';
				if (currentFMode != 2) {
					selectionMagic(field, hyperLink, true);
					addLinkWidget.style.display = 'none';
					return;
				}
				if (wGS) {
					hyperLink = field.document.createElement('a');
					hyperLink.href = hrefField;
					hyperLink.appendChild(document.createTextNode(textField));
					selectionMagic(field, hyperLink, false);
				} else {
					hyperLink = '<a href="'+hrefField+'">'+textField+'</a>';
					selectionMagic(field, hyperLink, false);
				}
				addLinkWidget.style.display = 'none';
				return;
			}
			break;
		case 'spoiler':
			if (currentFMode == 2) {
				var selection = selectionMagic(rte.contentWindow);
				if (selection && selection.length)
					selectionMagic(rte.contentWindow, FunctionMap['spoiler']['start']+selection+FunctionMap['spoiler']['end'], true);
			} else fta.execCommand(id);
			break;
		default:
			if (currentFMode == 2) doc.execCommand(id, false, null);
			else fta.execCommand(id);
			break;
	}
}

/* Function that inserts smileys */
function insertSmiley() {
	var start = this.className.indexOf('i_smiley_')+9;
	var name = this.className.substring(start);
	var id = Number(this.parentNode.id.substring(11));
	var text = smileys[smileyList.indexOf(name)]['text'];
	var fta = document.getElementById('textArea_'+id);
	var rte = document.getElementById("wysiwyg_"+id);
	var field = (currentFMode == 2 ? rte.contentWindow : fta);
	var selection = getSelection(field);
	selectionMagic(field, text, true);
}

/* This function should be added to a textArea */
function textAreaExecCommand(id) {
	var selection = selectionMagic(this);
	var n = Number(this.id.substring(this.id.indexOf('_')+1));
	var controls = document.getElementById('controls_'+n);
	var action = id.toLowerCase();
	if (selection && selection.length) {
		selectionMagic(this, FunctionMap[action]['start']+selection+FunctionMap[action]['end'], true);
		FunctionMap[action]['active'] = false;
		setButtonState(controls, id.toLowerCase(), false);
	} else if (!selection || !selection.length) {
		if (FunctionMap[action]['active']) {
			selectionMagic(this, FunctionMap[action]['end'], true);
			FunctionMap[action]['active'] = false;
			setButtonState(controls, id.toLowerCase(), false);
		} else {
			selectionMagic(this, FunctionMap[action]['start'], true);
			FunctionMap[action]['active'] = true;
			setButtonState(controls, id.toLowerCase(), true);
		}
	}
}

/* Function that inserts RTE commands */
function make () {
	var cname = this.className;
	if (cname.indexOf('selected') >= 0) cname = cname.replace(' selected','');
	var start = cname.indexOf('f_button f_')+11;
	var formatingFunc = cname.substring(start);
	if (currentFMode == 2) {
		if (this.className.indexOf('selected') < 0) this.className += ' selected';
		else this.className = this.className.replace(' selected','');
	} else {
		if (this.className.indexOf('selected') >= 0) this.className = this.className.replace(' selected','');
	}
	var fMap = FunctionMap[formatingFunc];
	var execID = fMap.id;
	var iframeId = this.parentNode.id.replace('controls_','');
	formatText(execID, iframeId, null, this);
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
	//if (curItem['text']) button.value = curItem['text'];
	if (node) node.appendChild(button);
	else return(button);
}

/* Change button mode
 * @params controls The element where the buttons are located
 * @params button Identifier of the button to locate
 * @params state True if button should be set to activated, false otherwise
 */
function setButtonState(controls, button, state) {
	if (!controls || !button) return;
	var inputs = controls.getElementsByTagName('input');
	var targetButton = getElementsByClassName(inputs, button, true)[0];
	if (!targetButton) return;
	if (state && targetButton.className.indexOf('selected') < 0) 
		targetButton.className += ' selected';
	else if (!state && targetButton.className.indexOf('selected') >= 0) 
		targetButton.className = targetButton.className.replace(' selected','');
	else return;
}

/* Check the nesting of the current cursor position/selection
 * @params iframe Iframe which holds the RTE
 * @params resubmit Allow browser to update selection before using the selection
 */
function checkButtonState(iframe, resubmit) {
	if (!iframe) return;
	if (!resubmit)
		setTimeout(function(){checkButtonState(iframe, true); return true;}, 500);

	var theSelection = null;
	var theRange = null;
	var theParentNode = null;
	var theLevel = 0;
	
	// Turn off all the buttons
	var id = Number(iframe.id.substring(iframe.id.indexOf('_')+1));
	var controls = document.getElementById('controls_'+id);
	var buttons = controls.getElementsByTagName('input');
	for (var i = 0; i < buttons.length; i++)
		buttons[i].className = buttons[i].className.replace(' selected','');
	
	/* IE selections */
	if (iframe.contentWindow.document.selection) {
		theSelection = iframe.document.selection;
		theRange = theSelection.createRange();
		try {
			theParentNode = theRange.parentElement();
		} catch (e) {
			return false;
		}
	} else { /* Mozilla selections */
		try {
			theSelection = iframe.contentWindow.getSelection();
		} catch (e) {
			return false;
		}
		theRange = theSelection.getRangeAt(0);
		theParentNode = theRange.commonAncestorContainer;
	}
	
	while (theParentNode.nodeType == 3)
		theParentNode = theParentNode.parentNode;
	
	while (theParentNode.nodeName.toLowerCase() != "body") {
		switch (theParentNode.nodeName.toLowerCase()) {
			case "a":
				setButtonState(controls, 'link', true);
				break;
			case "i":
			case "em":
				setButtonState(controls, 'italic', true);
				break;
			case "b":
			case "strong":
				setButtonState(controls, 'bold', true);
				break;
			case "u":
				setButtonState(controls, 'underline', true);
				break;
			case "strike":
				setButtonState(controls, 'strikethrough', true);
				break;
			case "li":
				setButtonState(controls, 'insertlistitem', false);
				break;
			case "ul":
				setButtonState(controls, 'insertorderedlist', false);
				setButtonState(controls, 'insertunorderedlist', true);
				break;
			case "ol":
				setButtonState(controls, 'insertorderedlist', true);
				setButtonState(controls, 'insertunorderedlist', false);
				break;
			case "span":
				var style = theParentNode.getAttribute("style").toLowerCase();
				if (style.indexOf('font-weight') >= 0 && style.indexOf('bold') >= 0)
					setButtonState(controls, 'bold', true);
				if (style.indexOf('font-style') >= 0 && style.indexOf('italic') >= 0)
					setButtonState(controls, 'italic', true);
				if (style.indexOf('text-decoration') >= 0 && style.indexOf('underline') >= 0)
					setButtonState(controls, 'underline', true);
				if (style.indexOf('text-decoration') >= 0 && style.indexOf('line-through') >= 0)
					setButtonState(controls, 'strikethrough', true);
				break;
			default:
				//alert('other/unknown: '+theParentNode.nodeName.toLowerCase());
				break;
		}
		theParentNode = theParentNode.parentNode;
		theLevel++;
	}
	return true;			
}

// -------------------------------------------
// CONVERTING FUNCTIONS
// -------------------------------------------

/* Make "valid" tags
 * @param str String to parse
 */
function makeValidTags(str) {
	/* Replace uppercase element names with lowercase */
	str = str.replace(/<[^> ]*/g, function(match){return match.toLowerCase();});
	
	/* Replace uppercase attribute names with lowercase */
	str = str.replace(/<[^>]*>/g, function(match) {
			match = match.replace(/ [^=]+=/g, function(match2){return match2.toLowerCase();});
			return match;
		});
		
	/* Put quotes around unquoted attributes */
	str = str.replace(/<[^>]*>/g, function(match) {
			match = match.replace(/( [^=]+=)([^"][^ >]*)/g, "$1\"$2\"");
			return match;
		});

	return str;
}

/* Function that converts spans to equivalents
 * @param id, this works at DOM level
 */
function convertSPANs(id) {
	/* Replace styled spans with their semantic equivalent */
	var iframe = document.getElementById('wysiwyg_'+id);
	if (!iframe) return;
	
	var theSPANs = iframe.contentWindow.document.getElementsByTagName("span");

	while(theSPANs.length > 0) {
		var theChildren = new Array();
		var theReplacementElement = null;
		var theParentElement = null;
		
		for (var j = 0; j < theSPANs[0].childNodes.length; j++)
			theChildren.push(theSPANs[0].childNodes[j].cloneNode(true));
		
		/* Detect type of span style */
		switch (theSPANs[0].getAttribute("style")) {
			case "font-weight: bold;":
				theReplacementElement = iframe.contentWindow.document.createElement("b");
				theParentElement = theReplacementElement;
				break;
			case "font-style: italic;":
				theReplacementElement = iframe.contentWindow.document.createElement("i");
				theParentElement = theReplacementElement;
				break;
			case "text-decoration: underline;":
				theReplacementElement = iframe.contentWindow.document.createElement("u");
				theParentElement = theReplacementElement;
				break;
			case "text-decoration: line-through;":
				theReplacementElement = iframe.contentWindow.document.createElement("strike");
				theParentElement = theReplacementElement;
				break;
			case "font-weight: bold; font-style: italic;":
				theParentElement = iframe.contentWindow.document.createElement("i");
				theReplacementElement = iframe.contentWindow.document.createElement("b");
				theReplacementElement.appendChild(theParentElement);
				break;
			case "font-style: italic; font-weight: bold;":
				theParentElement = iframe.contentWindow.document.createElement("b");
				theReplacementElement = iframe.contentWindow.document.createElement("i");
				theReplacementElement.appendChild(theParentElement);
				break;
			default:
				replaceNodeWithChildren(theSPANs[0]);
				break;
		}
		
		if (theReplacementElement != null) {
			for (var j = 0; j < theChildren.length; j++)
				theParentElement.appendChild(theChildren[j]);
			theSPANs[0].parentNode.replaceChild(theReplacementElement, theSPANs[0]);
		}
		
		theSPANs = iframe.contentWindow.document.getElementsByTagName("span");
	}
}

/* Convert links */
function convertLinksOutput(mstr, inner, text, offset, s) {
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
	return ("[url="+href+"]"+text+"[/url]");
}

/* Function that formats an output string
 * @param str String to format
 */
function convert_output(str) {
	str = makeValidTags(str);
	/* Extra stuff, mostly word paste */
	str = str.replace(/\&nbsp\;/mgi,' ');
	/* convert stupid webkit div's to br's */
	str = str.replace(/\<div\>/mgi,'');
	str = str.replace(/\<\/div\>/mgi,'<br />');
	// Word crap
	str = str.replace(/\<[/]?(div|font|span|xml|del|ins|img|h\d|[ovwxp]:\w+)[^>]*?\>/mgi,'');
	str = str.replace(/\<\!\-\-\[.+?\]\-\-\>/mgi,'');
	str = str.replace(/\<\?xml[^>]*\>/g, "");
	str = str.replace(/\<[^ >]+:[^>]*\>/g, "");
	str = str.replace(/\<\/[^ >]+:[^>]*\>/g, "");
	/* Remove leading and trailing whitespace */
	str = str.replace(/^\s+/, "");
	str = str.replace(/\s+$/, "");
	/* Remove style attribute inside any tag */
	str = str.replace(/ style="[^"]*"/g, "");
	/* Replace improper BRs */
	str = str.replace(/\<br\>/g, "<br />");
	/* Remove BRs right before the end of blocks */
	str = str.replace(/\<br \/\>\s*\<\/(h1|h2|h3|h4|h5|h6|li|p)/g, "</$1");
	/* convert stupid webkit div's to br's */
	str = str.replace(/\<div\>/mgi,'');
	str = str.replace(/\<\/div\>/mgi,'<br />');

	str = str.replace(/\<br([^>]*)\>/mgi,'\n');
	str = str.replace(/\<(p|u|b|i|ul|ol|li|strike|pre) [^>]*?\>/mgi,'[$1]');
	str = str.replace(/\<\/(p|u|b|i|ul|ol|li|strike|pre) [^>]*?\>/mgi,'[/$1]');
	str = str.replace(/\<(p|u|b|i|ul|ol|li|strike|pre)\>/mgi,'[$1]');
	str = str.replace(/\<\/(p|u|b|i|ul|ol|li|strike|pre)\>/mgi,'[/$1]');
	
	str = str.replace(/\<([/])?strong([^>]*)\>/mgi,'[$1b]');
	str = str.replace(/\<([/])?em([^>]*)\>/mgi,'[$1i]');

	str = str.replace(/\[strike\]/mgi,'[s]');
	str = str.replace(/\[\/strike\]/mgi,'[/s]');
	str = str.replace(/\[pre\]/mgi,'[code]');
	str = str.replace(/\[\/pre\]/mgi,'[/code]');
	str = str.replace(/\<a(.+?)\>(.+?)\<\/a\>/mgi,convertLinksOutput);
	
	/* Just a bit of beautifier */
	str = str.replace(/\[\/li\]\[li\]/mgi,'[/li]\n[li]');
	str = str.replace(/\[\/li\]\[\/ul\]/mgi,'[/li]\n[/ul]');
	str = str.replace(/\[\/li\]\[\/ol\]/mgi,'[/li]\n[/ol]');
	str = str.replace(/\[\/ul\]\[(u|b|i|s|code|quote)\]/mgi,'[/ul]\n[$1]');
	str = str.replace(/\[\/ol\]\[(u|b|i|s|code|quote)\]/mgi,'[/ol]\n[$1]');

	str = str.replace(/\[p\]/mgi,'');
	str = str.replace(/\[\/p\]/mgi,'\n');
	/* Remove empty tags */
	str = str.replace(/\[(p|u|b|i|ul|ol|li)\]\[\/(p|u|b|i|ul|ol|li)\]/mgi,	function areTwoEqual(mstr,s1,s2) { 
																				if (s1 == s2) return ' ';
																				else return mstr;
																			}
																		);
	return (str);
}

/* Replacement functions
 * @param str String to replace identifiers
 */
function convert_input(str) {
	str = str.replace(/\[(p|b|i|u|ul|ol|li)\]/mgi,'<$1>');
	str = str.replace(/\[\/(p|b|i|u|ul|ol|li)\]/mgi,'</$1>');
	str = str.replace(/\[([/])?s\]/mgi,'<$1strike>');
	str = str.replace(/\<p\>/mgi,'');
	str = str.replace(/\<\/p\>/mgi,'<br />');
	str = str.replace(/\[br\]/mgi,'<br />');
	str = str.replace(/\n/mgi,'<br />');
	str = str.replace(/\<\/li\>\<br \/\>/mgi,'</li>');
	str = str.replace(/\<\/ul\>\<br \/\>\<br \/\>/mgi,'</ul></br>');
	str = str.replace(/\<\/ol\>\<br \/\>\<br \/\>/mgi,'</ol></br>');
	str = str.replace(/\[url=([^\[\]].+?)\]([^\:\\\/\[\]].+?)\[\/url\]/mgi,'<a href="$1">$2</a>');
	str = str.replace(/\[url\]([^\:\\\/\[\]].+?)\[\/url\]/mgi,'<a href="$1">$1</a>');
	return (str);
}

/* Converts text from the RTE to the TEXTAREA and vice versa
 * @param mode If true/defined will convert from the textArea to the RTE, otherwise from RTE to textArea
 */
function convertText(id, mode) {
	if (id == null && mode == null) return;
	var rte = document.getElementById('wysiwyg_'+id);
	var fTA = document.getElementById('textArea_'+id);
	if (!rte || !fTA) return;
	rte = rte.contentWindow.document;
	if (mode) { // 
		var content = convert_input(fTA.value);
		//if (!content.length) content = '&#xA0;';
		rte.body.innerHTML = content;
	} else {
		convertSPANs(id);
		fTA.value = convert_output(rte.body.innerHTML);
	}
}

function showRTEinPRE(id) {
	var rte = document.getElementById('wysiwyg_'+id);
	var codeview = document.getElementById("codeview");
	var innerdoc = rte.contentDocument;
	if (typeof(document.documentElement.textContent) == "undefined" || !window.XMLSerializer || !document.addEventListener)
	{
		var display_code = (function _display_code_ie()
			{
				codeview.value = innerdoc.body.outerHTML;
			});
		innerdoc.onkeyup = display_code;
	}
	else
	{
		var stringer = new XMLSerializer();
		var display_code = (function _display_code_moz()
			{
				codeview.value = stringer.serializeToString(innerdoc.body); // innerdoc.body.innerHTML;
			});
		innerdoc.addEventListener("keyup", display_code, true);
	}
	display_code();
}

function showConvertedRTEinPRE(id) {
	var rte = document.getElementById('wysiwyg_'+id);
	var codeview = document.getElementById("codeview");
	var innerdoc = rte.contentDocument;
	var display_code = (function _display_code_ie() {
			convertSPANs(0);
			codeview.value = convert_output(innerdoc.body.innerHTML);
		});
	innerdoc.onkeyup = display_code;
	display_code();
}

function changeFMode() {
	var controls = this.parentNode;
	var id = Number(controls.id.substring(controls.id.indexOf('_')+1));
	var mode = this.selectedIndex;
	if (mode == currentFMode) return;
	var rte = document.getElementById("wysiwyg_" +id);
	var fta = document.getElementById("textArea_"+id);
	if (currentFMode == 2)
		convertText(id, false);
	else
		convertText(id, true);
	if (controls) {
		var newControls = createControls(null, id, mode);
		newControls.style.display = '';
		controls.parentNode.replaceChild(newControls,controls);
	}
	if (rte) rte.style.display = (mode != 2 ? 'none' : '');
	if (fta) fta.style.display = (mode != 2 ? '' : 'none');
	currentFMode = mode;
}

// -----------------------------------------------
// Creator functions
// -----------------------------------------------

/* Updates the attribute note text */
function updateAttributeHelpText() {
	var option = this.value;
	var attrHelp = document.getElementById('f_links_attr');
	attrHelp.title = OptionsMap[option]['attribute'];
	var attr = document.getElementById('f_links_attr');
	if (option == 'wiki') { attr.disabled = true; attr.value = ''; }
	else attr.disabled = false;
}

/* Function that creates the link window */
function createLinkWindow(parentNode) {
	var div = document.createElement('div');
	div.id = 'widget_addlink';
	div.className = 'dialogBox';
	div.style.display = 'none';
	var table = document.createElement('table');
	var tbody = document.createElement('tbody');
	var row = document.createElement('tr');
	createHeader(row, null, 'URL');
	createCell(row, null, createTextInput('f_links_href',null,false,false,null,null));
	tbody.appendChild(row);
	row = document.createElement('tr');
	createHeader(row, null, 'Text');
	createCell(row, null, createTextInput('f_links_text',null,false,false,null,null));
	tbody.appendChild(row);
	table.appendChild(tbody);
	div.appendChild(table);
	// Create buttons for the drag window //
	var buttons = new Array();
	buttons.push(createButton('f_links_cancel','f_links_cancel',false,'Cancel','button',function hideBox() { document.getElementById('widget_addlink').style.display = 'none'; }, 'button dialogButton dialogCancel'));
	buttons.push(createButton('f_links_ok','f_links_ok',false,'Add','button',null,'confirmButton dialogButton dialogConfirm'));
	// add drag and drop behaviour
	dragDrop.initElement(div,'Create link', null, buttons);
	if (parentNode) parentNode.appendChild(div);
	else return div;
}

/* Function that creates the controls */
function createControls(parentNode, id, mode) {
	if (mode == null) mode = currentFMode;
	buttonList = {'0':[],
				  '1':['bold','italic','underline','strikethrough','insertorderedlist','insertunorderedlist','insertlistitem','spoiler','link'],
				  '2':['bold','italic','underline','strikethrough','insertorderedlist','insertunorderedlist','spoiler','link']};

	var div = document.createElement('div');
	div.className = 'format-buttons f_controls';
	div.id = 'controls_'+ id;
	//if (mode == 0) div.style.display = 'none';
	var buttons = buttonList[mode];
	for (var b = 0; b < buttons.length; b++) {
		var button = createLocalButton(null,buttons[b],FunctionMap[buttons[b]]);
		if (mode == 0) button.disabled = true;
		div.appendChild(button);
	}
	createSelectArrayN(div,'f_mode','f_mode',changeFMode,ModeMap[mode],ModeMap);
	if (parentNode) parentNode.appendChild(div);
	else return div;
}

/* function that adds RTE abilites to a given iframe */
function createRTE(iframe, textArea) {
	if (!iframe) return;
	
	var backgroundColor = getStyleInformation(textArea,'backgroundColor');
	var color = getStyleInformation(textArea,'color');
	var fontFamily = getStyleInformation(textArea,'fontFamily');
	var fontSize = getStyleInformation(textArea,'fontSize');
	var border = getStyleInformation(textArea,'border');
	var margin =	getStyleInformation(textArea,'marginTop') + ' ' + getStyleInformation(textArea,'marginRight') + ' ' +
					getStyleInformation(textArea,'marginBottom') + ' ' + getStyleInformation(textArea,'marginLeft');
	var padding = 	getStyleInformation(textArea,'paddingTop') + ' ' + getStyleInformation(textArea,'paddingRight') + ' ' +
					getStyleInformation(textArea,'paddingBottom') + ' ' + getStyleInformation(textArea,'paddingLeft');

	var doc = iframe.contentWindow.document;

	var documentContent = '';
	if (!isIE) {
		documentContent  = '\x3C!DOCTYPE html>\x3Chtml>\x3Chead>\x3Cstyle>body {';
		documentContent += 'margin: '+margin+';'
		documentContent += 'padding: '+padding+';';
		documentContent += 'background-image: none;';
		if (backgroundColor) documentContent += 'background-color: '+backgroundColor+';\n';
		if (fontFamily) documentContent += 'font-family: '+fontFamily+';\n';
		if (fontSize) documentContent += 'font-size: '+fontSize+';\n';
		if (color) documentContent += 'color: '+color+';\n';
		documentContent += '}\x3C/style>\x3C/head>\x3Cbody>'+convert_input(textArea.value)+'\x3C/body>\x3C/html>';
	} else documentContent = convert_input(textArea.value);

	doc.open();
	doc.write(documentContent);
	doc.close();
	doc.designMode = "on";
	/* Mozilla event capturing */
	if (typeof document.addEventListener == "function") {
		doc.addEventListener("mouseup", function(){checkButtonState(iframe); return true;}, false);
		doc.addEventListener("keyup", function(){checkButtonState(iframe); return true;}, false);
	} else { /* IE event capturing */
		doc.attachEvent("onmouseup", function(){checkButtonState(iframe); return true;});
		doc.attachEvent("onkeyup", function(){checkButtonState(iframe); return true;});
	}
	try { if (document.queryCommandSupported('styleWithCSS')) true; } 
	catch(e) { 
		try { doc.execCommand('styleWithCSS', null, false); } // FF 1.5+
		catch (e) {  } // FF 1.5-
	}
	try { if (document.queryCommandSupported('useCSS')) true; } 
	catch(e) { 
		try { doc.execCommand('useCSS', null, true); }
		catch (e) {  } // FF 1.5-
	}
	if (currentFMode != 2) iframe.style.display = 'none';
}

/* Function that creates the RTE iframe */
function createIframe(parentNode, id, textArea) {
	var backgroundColor = getStyleInformation(textArea,'backgroundColor');
	var color = getStyleInformation(textArea,'color');
	var fontFamily = getStyleInformation(textArea,'fontFamily');
	var fontSize = getStyleInformation(textArea,'fontSize');
	var border = getStyleInformation(textArea,'border');
	var margin =	getStyleInformation(textArea,'marginTop') + ' ' + getStyleInformation(textArea,'marginRight') + ' ' +
					getStyleInformation(textArea,'marginBottom') + ' ' + getStyleInformation(textArea,'marginLeft');
	var padding = 	getStyleInformation(textArea,'paddingTop') + ' ' + getStyleInformation(textArea,'paddingRight') + ' ' +
					getStyleInformation(textArea,'paddingBottom') + ' ' + getStyleInformation(textArea,'paddingLeft');
	
	var iframe = document.createElement('iframe');
	iframe.frameBorder = 0;
	iframe.id = "wysiwyg_" + id;
	iframe.style.height = wysiwygHeight;
	iframe.style.width = wysiwygWidth; //(isIE ? wysiwygWidth : (parseFloat(wysiwygWidth) - 14 + (isFF ? 1 : 0))+'px');
	iframe.src = 'about:blank';
	iframe.style.margin = margin;
	iframe.style.padding = padding;
	
	if (border) iframe.style.border = border;
	else {
		iframe.style.borderColor = getStyleInformation(textArea,'borderTopColor') || 'black';
		iframe.style.borderWidth = getStyleInformation(textArea,'borderTopWidth') || '1px';
		iframe.style.borderStyle = getStyleInformation(textArea,'borderTopStyle') || 'solid';
	}
	if (backgroundColor) iframe.style.backgroundColor = backgroundColor;
	return iframe;	
}

/* Function that creates the smiley list with my nice and current actions */
function createSmileyBox(parentNode, id) {
	var smileyBox = document.createElement('div');
	smileyBox.className = 'smiley-box icons';
	smileyBox.id = 'smiley-box_'+id;
	var h3 = document.createElement('h3');
	h3.appendChild(document.createTextNode('Smilies'));
	smileyBox.appendChild(h3);
	for (var i = 0; i < smileys.length; i++)
		createIcon(smileyBox, smileys[i]['text'], 'removeme', insertSmiley, smileys[i]['title'], 'i_smiley_'+smileys[i]['name']);
	if (parentNode) parentNode.appendChild(smileyBox);
	else return smileyBox;
}

/* Initiates Formating */
function initFormating() {
	var errorMsg = '';
	if (!document.getElementsByTagName) errorMsg = 'document.getElementsByTagName: fail\n';
	if (!document.designMode) errorMsg += 'document.designMode: fail\n';
	var textAreas = document.getElementsByTagName('textarea');
	if (!textAreas.length) errorMsg += 'textAreas: fail'; // Still no nodes..
	if (errorMsg != '') { errorAlert('init_formating',errorMsg); return; }

	// This will create the formating icons for each TextArea
	for (var i = 0; i < textAreas.length; i++) {
		var textArea = textAreas[i];
		if (textArea.className.indexOf('norte') >= 0 || textArea.name.indexOf('norte') >= 0) continue;
		textArea.id = "textArea_"+i;
		textArea.value = textArea.value.replace(/\[br\]/mgi,'\n');
		textArea.execCommand = textAreaExecCommand;

		if (dS) {
			textArea.getCaretPosition = getCaretPosition;
			textArea.selectionStart = textAreaSelectionStart;
			textArea.selectionEnd = textAreaSelectionEnd;
		}

		var smileyBox = getElementsByClassName(textArea.parentNode.parentNode.getElementsByTagName('div'),'smiley-box', true)[0];
		if (smileyBox) {
			var newSmileyBox = createSmileyBox(null,i);
			smileyBox.parentNode.replaceChild(newSmileyBox,smileyBox);
			smileyBox = newSmileyBox;
		}
		
		wysiwygHeight = getStyleInformation(textArea,'height');
		wysiwygWidth = getStyleInformation(textArea,'width');

		var controls = createControls(null, i);
		var iframe = createIframe(null, i, textArea);
		textArea.parentNode.insertBefore(iframe,textArea);
		if (currentFMode == 2) textArea.style.display = 'none';
		textArea.parentNode.insertBefore(controls,iframe);
		createRTE(iframe,textArea);
		
		// Update inputs
		if (textArea.form) {
			var inputs = textArea.form.getElementsByTagName('input');
			for (var s = 0; s < inputs.length; s++) {
				var input = inputs[s];
				if (input.type.toLowerCase() != 'submit') continue;
				input.onclick = function updateText() {
					var tx = this.form.getElementsByTagName('textarea');
					for (var i = 0; i < tx.length; i++) {
						var fta = tx[i];
						if (fta.id.indexOf('textArea_') < 0) continue;
						var id = Number(fta.id.replace('textArea_',''));
						if (currentFMode == 2) convertText(id, false);
					}
				};
				break;
			}
		}
	}
}

function prepPage() {
	initFormating();
	createLinkWindow(document.getElementById('layout-content'));
}

//window.onload = prepPage;
addLoadEvent(prepPage);
