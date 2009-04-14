/* Utilities
 * @author fahrenheit (alka.setzer@gmail.com)
           some code derived from work done by PetriW and Rar at anidb
 * version 2.2 (07.07.2007)
 */
jsVersionArray.push({
	"file":"anime3/utils.js",
	"version":"2.3",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$"
});
// TIME/DATE FUNCTIONS //

var globalStatus = new statusInformation(); // Global status information

function padLeft(text, padChar, count) {
  var result = '';
  text = new String(text);
  while (text.length + result.length < count) {
    result += result + padChar;
  }
  return (result + text);
}

function padRight(text, padChar, count) {
  var result = '';
  text = new String(text);
  while (text.length + result.length < count) {
    result += result + padChar;
  }
  return (text + result);
}

/* Function that converts UTC TIME/UNIXTIME date format to a string
 * @param data The date to convert
 * @return A string with the date
 */
function convertTime(data) {
  if (!data) return;
  if (data.indexOf('T') >= 0) 
    return (data.split('T').join(" "));
  else if (data.indexOf('-') >= 0 && data.indexOf(' ') < 0)
    return data;
  else {
    var datetime = new Date(data * 1000);
    return (datetime.getFullYear() + '-' + padLeft(datetime.getMonth()+1, '0', 2) + '-' + padLeft(datetime.getDate(), '0', 2));
  }
}

/* Function that converts given date to javascript a Date object
 * @param data The date to convert
 * @return A Date object
 */
function javascriptDate(data) {
	if (!data) return;
	if (data.indexOf('T') >= 0) { // UTC time
		var date = data.split('T')[0].split('-');
		var time = data.split('T')[1].split(':');
		if (date[0].length == 2) {
			if (Number(date[0][0]) < 4) date[0] = '20'+date[0];
			else date[0] = '19'+date[0];
		}
		return new Date(Number(date[0]),Number(date[1])-1,Number(date[2]),Number(time[0]),Number(time[1]),(time[2] ? Number(time[2]) : 0));
	} else if (data.indexOf('-') >= 0 && data.indexOf(' ') >= 0 && data.indexOf(':') >= 0) {
		var date = data.split(' ')[0].split('-');
		var time = data.split(' ')[1].split(':');
		if (date[0].length == 2) {
			if (Number(date[0][0]) < 4) date[0] = '20'+date[0];
			else date[0] = '19'+date[0];
		}
		return new Date(Number(date[0]),Number(date[1])-1,Number(date[2]),Number(time[0]),Number(time[1]),(time[2] ? Number(time[2]) : 0));
	} else if (data.indexOf('.') >= 0 && data.indexOf(' ') < 0 && data.indexOf(':') < 0) {
		var date = data.split('.');
		if (date[2].length == 2) {
			if (Number(date[2][0]) < 4) date[2] = '20'+date[2];
			else date[2] = '19'+date[2];
		}
		return new Date(Number(date[2]),Number(date[1])-1,Number(date[0]));
	} else if (data.indexOf('.') >= 0 && data.indexOf(' ') >= 0 && data.indexOf(':') >= 0) {
		var date = data.split(' ')[0].split('.');
		var time = data.split(' ')[1].split(':');
		if (date[2].length == 2) {
			if (Number(date[2][0]) < 4) date[2] = '20'+date[2];
			else date[2] = '19'+date[2];
		}
		return new Date(Number(date[2]),Number(date[1])-1,Number(date[0]),Number(time[0]),Number(time[1]),(time[2] ? Number(time[2]) : 0));
	} else
		return datetime = new Date(data * 1000); // UNIX time format
}

/* This function returns a Date of UTC time date
 * @param data The date
 * @return Date in the format of dd.mm.yyyy
 */
function cTimeDate(data) {
  if (!data) return;
  if (data.indexOf(' ') >= 0) {
    data = data.split(' ')[0];
    data = data.split('-')[2] + '.' + data.split('-')[1] + '.' + data.split('-')[0];
    return (data);
  } else if (data.indexOf('-') >= 0 && data.indexOf(' ') < 0) {
    data = data.split('-')[2] + '.' + data.split('-')[1] + '.' + data.split('-')[0];
    return (data);
  } else return(data);
}
/* This function returns a Hour of UTC time date
 * @param data The date
 * @return Date in the format of hh:mm:ss
 */
function cTimeHour(data) {
  if (!data) return;
  if (data.indexOf(' ') >= 0) return (data.split(' ')[1]);
  else if (data.indexOf(':') >= 0 && data.indexOf(' ') < 0) return(data);
  else return(data);
}

/* This function retuns a Date of UTC time in DATE HOUR style
 * @param data The date
 * @return Date in the format of dd.mm.yyyy hh:mm
 */
function cTimeDateHour(data) {
  if (!data) return;
  var date = cTimeDate(data);
  if (data.indexOf(' ') >= 0) {
	var hour = data.split(' ')[1] || '';
	if (hour != '') hour = ' ' + hour.split(':')[0] + ':' + hour.split(':')[1];
    return (date+hour);
  }
  else return(date);
}

// DOM NODE FUNCTIONS //

/* returns computed style information about a specific element
 * @param element where to get attributes
 * @param attribute what to get 
 */
function getStyleInformation(element,attribute,debug) {
	if (!element && attribute) return;
	var useCurrentStyle = (element.currentStyle) ? true : false;
	var useGetCumputedStyle = (window.getComputedStyle) ? true : false;

	if (debug) {
		var result = '';
		if (!attribute) {
			if (useGetCumputedStyle)
				result += '\n!attribute>useGetCumputedStyle: ' + window.getComputedStyle(element,null);
			if (useCurrentStyle)
				result += '\n!attribute>useCurrentStyle: ' + element.currentStyle;
		} else {
			if (useGetCumputedStyle)
				result += '\nattribute>useGetCumputedStyle: ' + window.getComputedStyle(element,null)[attribute];
			if (useCurrentStyle)
				result += '\nattribute>useCurrentStyle: ' + element.currentStyle[attribute];
		}
		alert('useCurrentStyle: '+useCurrentStyle+
			'\nuseGetCumputedStyle: '+useGetCumputedStyle+
			'\nelement: '+element+
			'\nattribute: '+attribute+
			'\nresult: '+result
		);
	}
	
	if (!attribute) {
		if (useGetCumputedStyle)
			return window.getComputedStyle(element,null);
		else if (useCurrentStyle)
			return element.currentStyle;
		else return '';
	} else {
		if (useGetCumputedStyle)
			return window.getComputedStyle(element,null)[attribute];
		else if (useCurrentStyle)
			return element.currentStyle[attribute];
		else return '';
	}
	return null;
}

/* Returns the nodeValue of a given node
 * @param node The node where to extract information
 * @return String containing node data
 */
function nodeData(node) { 
  try { return node.childNodes.item(0).nodeValue; } 
  catch(e) { return ''; }

}

// STUBS //
function doNothing() { return false; }
function notImplemented() { alert('Not implemented yet'); }

/* Replace a node with its children -- delete the item and move its children up one level in the hierarchy */
function replaceNodeWithChildren(theNode) {
	var theChildren = new Array();
	var theParent = theNode.parentNode;
	
	if (theParent != null) 	{
		for (var i = 0; i < theNode.childNodes.length; i++)
			theChildren.push(theNode.childNodes[i].cloneNode(true));
			
		for (var i = 0; i < theChildren.length; i++)
			theParent.insertBefore(theChildren[i], theNode);	
			
		theParent.removeChild(theNode);
		return theParent;
	}
	return true;
}

/* Debug function that shows the dom tree of a node
 * @param node The node to show the tree
 * @return void (shows an alert box)
 */
function goDeepDOMtree(node,level) {
  if (!node.childNodes.length) return (node.nodeValue);
  var spacing = '';
  for (var k = 0; k < level; k++) spacing += '..';
  var out = spacing +'<'+node.nodeName+'>';
  for (var i = 0; i < node.childNodes.length; i++){
    var sNode = node.childNodes[i];
    if (sNode.childNodes && sNode.childNodes.length) out += '\n';
    out += goDeepDOMtree(sNode,level+1);
    if (sNode.childNodes && sNode.childNodes.length) out += spacing;
  }
  out += '</'+node.nodeName+'>\n';
  return out;
} 

function showDOMtree(node) {
  var out = '<'+node.nodeName+'>\n';
  for (var i = 0; i < node.childNodes.length; i++){
    var sNode = node.childNodes[i];
    out += goDeepDOMtree(sNode,1);
  }
  out += '</'+node.nodeName+'>';
  alert(out);
}

/* Function that searches a HTMLCollection Object for an element
 * with a specific class name
 * @param nodes The nodes
 * @param name The name of the class to find
 * @param useIndexOf If true will use a indexOf(name) >= 0 to search
 * @return requested node(s)
 */
function getElementsByClassName(nodes, name, useIndexOf) {
  var ret = new Array();
  for (var i = 0; i < nodes.length; i++) {
    if (!useIndexOf) { if (nodes[i].className == name) ret.push(nodes[i]); }
    else { if (nodes[i].className.indexOf(name) >= 0 ) ret.push(nodes[i]); }
  }
  return ret;
}

/* Function that searches a HTMLCollection Object for an element
 * with a specific name
 * @param nodes The nodes
 * @param sname The name of the class to find
 * @param useIndexOf If true will use a indexOf(name) >= 0 to search
 * @return requested node(s)
 */
function getElementsByName(nodes, sname, useIndexOf) {
  var ret = new Array();
  for (var i = 0; i < nodes.length; i++) {
    if (!useIndexOf) { if (nodes[i].name == sname) ret.push(nodes[i]); }
    else { if (nodes[i].name.indexOf(sname) >= 0 ) ret.push(nodes[i]); }
  }
  return ret;
}

/* Function that returns elements of given tag name for the given node only
 * @param node The node
 * @param tag The nodeName of the node.childNodes elements to find
 * @return requested node(s)
 */
function getNodeElementsByTagName(node, tag) {
  var ret = new Array();
  for (var i = 0; i < node.childNodes.length; i++) {
    if (node.childNodes[i].nodeName == tag) ret.push(node.childNodes[i]);
  }
  return ret;
}

/* Function that creates an header
 * @param parentNode The parent node (or null if you want to return the object)
 * @param className Class name to give
 * @param text Text to go in the node
 * @param abbr Text to go in the abbreviation title (or null if you won't use it)
 */
function createHeader(parentNode, className, text, abbr, anidbSort, colSpan) {
	var th = document.createElement('th');
	if (className != null) th.className = className;
	if (abbr != null) {
		var abbreviation = document.createElement('abbr');
		abbreviation.title = abbr;
		if (text != null) {
			if(typeof(text) == 'string') abbreviation.appendChild(document.createTextNode(text));
			else abbreviation.appendChild(text);
		}
		th.appendChild(abbreviation);
	} else if (text != null) {
		if(typeof(text) == 'string') th.appendChild(document.createTextNode(text));
		else th.appendChild(text);
	}
	if (colSpan != null && colSpan > 1) th.colSpan = colSpan;
	if (anidbSort != null) {
		if (th.className) th.className += ' '+anidbSort;
		else th.className = anidbSort;
	}
	if (parentNode != null) parentNode.appendChild(th);
	else return th;
}
function createDT(parentNode, className, text, abbr) {
	var dt = document.createElement('dt');
	if (className) dt.className = className;
	if (abbr != null) {
		var abbreviation = document.createElement('abbr');
		abbreviation.title = abbr;
		if (text != null) abbreviation.appendChild(document.createTextNode(text));
		dt.appendChild(abbreviation);
	} else if (text != null) dt.appendChild(document.createTextNode(text));
	if (parentNode) parentNode.appendChild(dt);
	else return dt;
}

/* This function creates a simple cell with an optional element
 * @param parentNode The parent node (or null if you want to return the object)
 * @param className Class name to give
 * @param someElement Some element to add to the cell, other elements have to be added manualy
 */
function createCell(parentNode, className, someElement, anidbSort, colSpan) {
	var td = document.createElement('td');
	if (className != null) td.className = className;
	if (someElement != null) {
		if (typeof(someElement) == 'string') td.appendChild(document.createTextNode(someElement));
		else td.appendChild(someElement);
	}
	if (anidbSort != null) td.setAttribute('anidb:sort',anidbSort);
	if (colSpan != null && colSpan > 1) td.colSpan = colSpan;
	if (parentNode != null) parentNode.appendChild(td);
	else return td;
}
function createDD(parentNode, className, someElement) {
	var dd = document.createElement('dd');
	if (className != null) dd.className = className;
	if (someElement != null) dd.appendChild(someElement);
	if (parentNode != null) parentNode.appendChild(dd);
	else return dd;
}

/* Creates icons
 * @param parentNode ParenteNode of the newly created icon or null if want return
 * @param text Text
 * @param url HREF
 * @param onclick Onclick action
 * @param title ALT and Title attribute
 * @param cname Class name
 */
function createIcon(parentNode, text, url, onclick, title, className) {
	var obj;
	if (url == '' || url == null) obj = document.createElement('span');
	else {
		obj = document.createElement('a');
		obj.href = url;
	}
	if (onclick != null && onclick != '') {
		obj.onclick = onclick;
		obj.style.cursor = 'pointer';
		obj.setAttribute('role','link');
	}
	if (url == 'removeme') {
		obj.style.cursor = 'pointer';
		obj.removeAttribute('href');
	}
	if (text != null) {
		var label = document.createElement('span');
		label.appendChild(document.createTextNode(text));
		obj.appendChild(label);
	}
	if (title != null && title != '') {
		obj.title = title;
		obj.alt = title;
	}
	if (className != null && className != '') obj.className = 'i_icon '+className;
	if (parentNode != null && parentNode != '') parentNode.appendChild(obj);
	else return(obj);
}

function createLink(parentNode, text, url, rel, onclick, title, className) {
	var obj = createIcon('', text, url, onclick, title, className);
	if (rel == '' || rel == null) {
		obj.rel = rel;
		if (rel == 'wiki') obj.target = '_blank';
	}
	if (className != null || className != '') obj.className = className;
	if (parentNode != null && parentNode != '') parentNode.appendChild(obj);
	else return(obj);
}

function createTextLink(parentNode, text, url, rel, onclick, title, className) {
	var obj = createLink(null, text, url, rel, onclick, title, className);
	if (className) obj.className = className;
	if (parentNode != null && parentNode != '') parentNode.appendChild(obj);
  else return(obj);
}

/* Creates a SELECT option element
 * @param parentNode ParenteNode of the newly created option or null if want return
 * @param text Text for the option
 * @param value Value of the option
 * @param isSelected Should this option be selected
 */
function createSelectOption(parentNode, text, value, isSelected, className, isDisabled) {
  var obj = document.createElement('option');
  if (text != null) obj.appendChild(document.createTextNode(text));
  if (value != null) obj.value = value;
  if (isSelected != null) obj.selected = isSelected;
  if (className != null) obj.className = className;
  if (isDisabled != null) obj.disabled = isDisabled;
  if (parentNode != null && parentNode != '') parentNode.appendChild(obj);
  else return(obj);
}

function createCheckbox(name,checked) {
  var ck = document.createElement('input');
  ck.type = 'checkbox';
  ck.name = name;
  if (checked != null) ck.checked = checked;
  return ck;
}

function createTextInput(name,size,disabled,hidden,maxlength,value) {
  var input = document.createElement('input');
  if (!hidden) input.type = 'text';
  else input.type = 'hidden';
  if (name != null) { input.name = name; input.id = name;/*.replace('.','_')*/; }
  if (size != null) input.size = size;
  if (disabled != null) input.disabled = disabled;
  if (maxlength != null) input.maxLength = maxlength;
  if (value != null) input.value = value;
  return input;
}

function createBasicButton(name,value,type) {
	var button = document.createElement('input');
	button.type = (type && type != '') ? type : 'button';
	if (name && name != '') {
		button.name = name;
		button.id = name;
	}
	if (value != null) button.value = value;
	return button;
}

function createButton(name,id,disabled,value,type,onclick, className) {
	var button = createBasicButton(name,value,type);
	if (id && id != '') button.id = id;
	if (disabled != null) button.disabled = (disabled);
	if (onclick != null) button.onclick = onclick;
	if (className != null) button.className = className;
	return button;
}

function createTextBox(name,id,cols,rows,onchange) {
	var textbox = document.createElement('textarea');
	if (name) textbox.name = name;
	if (id) textbox.id = id;
	if (cols) textbox.cols = cols;
	if (rows) textbox.rows = rows;
	if (onchange) textbox.onchange = onchange;
	return textbox;
}

function createBasicSelect(name,id,onchange) {
	var select = document.createElement('select');
	select.size = "1";
	if (name && name != '') select.name = name;
	if (id && id != '') select.id = id;
	if (onchange && onchange != '') select.onchange = onchange;
	return select;
}

function createLanguageSelect(parentNode,name,id,onchange,selected) {
	var select = createBasicSelect(name,id,onchange);
	for (var lang in languageMap) {
		var op = languageMap[lang];
		createSelectOption(select,op['name'],lang,(lang == selected), null, false);
	}
	if (parentNode && parentNode != '') parentNode.appendChild(select);
	else return select;
}

function createSelectArray(parentNode,name,id,onchange,selected,optionArray) {
	var select = createBasicSelect(name,id,onchange);
	for (var opt in optionArray) {
		var op = optionArray[opt];
		createSelectOption(select,op['text'], opt, (opt == selected), (op['class'] ? op['class'] : null), (op['disabled'] ? op['disabled'] : null));
	}
	if (parentNode && parentNode != '') parentNode.appendChild(select);
	else return select;
}
function createSelectArrayN(parentNode,name,id,onchange,selected,optionArray) {
	var select = createBasicSelect(name,id,onchange);
	for (var i = 0; i < optionArray.length; i++) {
		var op = optionArray[i];
		if (typeof(op) == 'object')
			createSelectOption(select, op['text'], (op['value'] ? op['value'] : op['text']), (op['selected'] || op['value'] == selected), (op['class'] ? op['class'] : null), (op['disabled'] ? op['disabled'] : null));
		else
			createSelectOption(select, op, op, (op == selected), null, null);
	}
	if (parentNode && parentNode != '') parentNode.appendChild(select);
	else return select;
}

function createMultiSelectArray(parentNode,name,id,onchange,selected,optionArray,size) {
	var select = createSelectArray(null,name,id,onchange,selected,optionArray);
	select.multiple = true;
	select.size = (size) ? size : 5;
	if (parentNode && parentNode != '') parentNode.appendChild(select);
	else return select;
}

function createCheckBox(parentNode,name,id,onchange,checked,disabled) {
	var ck = document.createElement('input');
	ck.type = 'checkbox';
	if (name && name != '') ck.name = name;
	if (id && id != '') ck.id = id;
	if (onchange && onchange != '') ck.onchange = onchange;
	if (checked) ck.checked = true;
	if (disabled) ck.disabled = true;
	if (parentNode && parentNode != '') parentNode.appendChild(ck);
	else return ck;
}
function createLabledCheckBox(parentNode,name,id,onchange,checked,text,className,disabled) {
	var ck = createCheckBox(null,name,id,onchange,checked,disabled);
	var label = document.createElement('label');
	label.appendChild(ck);
	if (className) label.className = className;
	if (text) {
		if (typeof(text) == 'string') label.appendChild(document.createTextNode(text));
		else label.appendChild(text);
	}
	if (parentNode && parentNode != '') parentNode.appendChild(label);
	else return label;
}

// GROUP BAR FUNCTIONS //
function makeBar(parentNode,start,end,total,map) {
	var MAX_BAR_WIDTH = (screen.width < 1280 ? 200 : 300);
	var mult = 1;
	if ( total > 0 && MAX_BAR_WIDTH / total >= 1) mult = Math.floor(MAX_BAR_WIDTH / total);
	var max_width = (total > MAX_BAR_WIDTH ? MAX_BAR_WIDTH : total);
	var img = document.createElement('img');
	img.src = base_url + 'pics/anidb_bar_h_'+map['img']+'.gif';
	//img.width = ( width * mult );
	img.width = Math.round(max_width * ((1+end - start) / total))*mult;
	img.height = 10;
	img.title = img.alt = '';
	if (parentNode != null || parentNode != '') parentNode.appendChild(img);
	else return img;
}

function makeCompletionBar(parentNode, range, maps) {
	var len = range.length;
	var span = document.createElement('span');
	span.className = 'completion';
	if (maps[1]['use'] || maps[2]['use']) {
		//span.setAttribute('anidb:data',maps);
		span.onmouseout = hideTooltip;
		span.onmouseover = function onmouseover(event) {
			var node = document.createElement('div');
			if (maps[1]['use']) node.appendChild(document.createTextNode(maps[1]['desc']));
			if (maps[1]['use'] && maps[2]['use']) node.appendChild(document.createTextNode(', ')); 
			if (maps[2]['use']) node.appendChild(document.createTextNode(maps[2]['desc']));
			setTooltip(node,true,'auto');
		}
	}

	for (var i=0; i < range.length; ) {
		var v = range[i];
		var k = i+1;
		while ( k < range.length && range[k] == v ) k++;
		if (!v) v=0;
		makeBar(span, i+1, k, range.length, maps[v]);
		i = k;
	}
	if (parentNode != null && parentNode != '') parentNode.appendChild(span);
	return span;
}

function expandRange(range,limit,map,array) {
	if (!range && !array) return (new Array(limit));
	var rangeGroups = range.split(',');
	for (var r = 0; r < rangeGroups.length; r++) {
		var rangeGroup = rangeGroups[r];
		var rg = rangeGroup.split('-');
		if (Number(rg[0]) >= 1000) 
			continue; //return (new Array(limit)); // don't do this for specials and crap.
		if ( rg.length == 1 ) 
			array[Number(rg[0])-1] = map['type'];
		else {
			for( var i = Number(rg[0]); i <= Number(rg[1]); i++) 
				array[ i-1 ] = map['type']; 
		}
	}
	return array;
}
function convertRangeToText(range) {
	//javascript:alert(convertRangeToText("1-3,1001-1002,10001"));
	var text = '';
	if (!range) return(text);
	var rangeGroups = range.split(',');
	var newRangeGroup = new Array();
	for (var r = 0; r < rangeGroups.length; r++) {
		var rangeText = "";
		var rangeGroup = rangeGroups[r];
		var rg = rangeGroup.split('-');
		if (rg.length == 1) rangeText += mapReverseEpisodeNumber(rg[0]);
		else {
			var rgGroup = new Array();
			for (var i = 0; i < rg.length; i++)
				rgGroup.push(mapReverseEpisodeNumber(rg[i]));
			rangeText += rgGroup.join('-');
		}
		newRangeGroup.push(rangeText);
	}
	return newRangeGroup.join(',');
}

// GENERAL FUNCTIONS //

/* Function that alerts the user for errors
 * @param func Name of the function
 * @param process 
 * @param pNode Parent node
 * @param cNode Child node (or currentNode)
 * @return void
 */
function showAlert(func, process, pNode, cNode) {
  if (cNode != '#text' && seeDebug)
  alert(func+
	      '\nProcessing: '+process+
	      '\nUnrecognized '+pNode+' node: '+cNode+
        '\nPlease warn your favorite moderator with this text message');
}

/* Function that alerts the user for errors
 * @param funcName Name of the function
 * @param whereIs some message to indicate where the error is
 * @return void
 */
function errorAlert(funcName,whereIs) {
	if (seeDebug) {
		alert('There was an error while processing '+funcName+'.'+
			'\nDetails: '+whereIs+
			'\nLocation: '+window.location+
			'\nPlease inform an AniDB mod about this error.');
	}
	return;
}

/* Function that parses the URI
 * @return Object holding URI Data
 */
function parseURI() {
  var uri = "" + window.location;
  var obj = new Object();
  if (uri.indexOf('?') > -1) { // we have an URI
    var elems;
    if (uri.indexOf('#') >= 0) {
      elems = uri.substring(uri.indexOf('?')+1,uri.indexOf('#')).split('&');
      obj['#'] = uri.substring(uri.indexOf('#')+1,uri.length);
    } else elems = uri.substring(uri.indexOf('?')+1,uri.length).split('&');
  }
  for (i in elems) {
    var efield = String(elems[i]).split('=')[0];
    var evalue = String(elems[i]).split('=')[1];
    obj[efield] = evalue;
  }
  return obj;
}

/* Function that updates the URI
 * @param obj URI object
 * @return void (set's the URI)
 */
function updateURI(obj) {
  var uri = "" + window.location;
  var currentURI = uri.substring(0,uri.indexOf('?'));
  var initial = true;
  for (var field in obj) {
    if (field == '#') continue;
    if (!initial) currentURI += '&';
    else currentURI += '?';
    currentURI += field + '=' + obj[field];
  }
  if (obj['#']) currentURI += '#' + obj['#'];
  return (currentURI);
}

/* Function that clones an object
 * @param what The object to clone
 * @return usage: var x = new cloneObject(y);
 */
function cloneObject(what) {
  for (var i in what)
    this[i] = what[i];
}

function cloneArray(what) {
	var array = new Array();
	for (var i = 0; i < what.length; i++)
		array[i] = what[i];
	return array;
}

/* Adds array push prototype to arrays if not defined */
function Array_push() {
  var A_p = 0;
  for (A_p = 0; A_p < arguments.length; A_p++) {
    this[this.length] = arguments[A_p];
  }
  return(this.length);
}

if (typeof Array.prototype.push == "undefined") {
  Array.prototype.push = Array_push;
}


/* Adds array shift prototype to arrays if not defined */
function Array_shift() {
  var A_s = 0;
  var response = this[0];
  for (A_s = 0; A_s < this.length-1; A_s++) {
    this[A_s] = this[A_s + 1];
  }
  this.length--;
  return(response);
}

if (typeof Array.prototype.shift == "undefined") {
  Array.prototype.shift = Array_shift;
}

/* Adds array indexOf prototype to arrays if not defined */
function Array_indexOf(what) {
	var index = -1;
	for (var i = 0; i < this.length; i++) {
		if (this[i] == what) return i;
	}
	return index;
}

if (typeof Array.prototype.indexOf == "undefined") {
	Array.prototype.indexOf = Array_indexOf;
}

/* Overriding an override */
if (typeof Array.prototype.sum != "undefined") {
	Array.prototype.sum = null;
}

/* This function is used to update the status of the Request
 * @param text Text to show
 * @return void
 */
function updateStatus(text,add,targetName) {
	try {
		if (!targetName) targetName = 'statusBox';
		if (document.getElementById(targetName)) {
			var statusBox = document.getElementById(targetName);
			if (!add) { 
				if (statusBox.firstChild) statusBox.removeChild(statusBox.firstChild);
				statusBox.appendChild(document.createTextNode(text)); 
			} else {
				statusBox.firstChild.nodeValue += text;
			}
		}
		else window.status = text;
	} catch (e) { }
}

/* statusInformation Object */
function statusInformation() {
	this.container = null;
	this.text = '';
	this.loadingbar_color = 'green';
	this.loadingbar_initial = -120;
	this.loadingbar_imageWidth = 240;
	this.loadingbar_eachPercent = (this.loadingbar_imageWidth/2)/100;
	this.loadingbar_percentage = 0;
	this.loadingbar_tooltip = '';
	this.barElement = null;
	this.textElement = null;
	this.brElement = document.createElement('br');
	this.clearContainer = function() {
		if (!this.container) { updateStatus(''); return; }
		this.container.nodeValue = '';
		while (this.container.childNodes.length) this.container.removeChild(this.container.firstChild);
		this.barElement = null;
		this.textElement = null;
		this.brElement = null;
	}
	this.init = function() {
		if (!this.container) return;
		this.clearContainer();
		this.container.appendChild(this.createText());
		this.container.appendChild(this.createBr());
		this.container.appendChild(this.createBar());
	}
	this.createText = function() {
		if (!this.container) return;
		var span = document.createElement('span');
		span.style.dispaly = 'none';
		span.style.align = 'center';
		span.appendChild(document.createTextNode(' '));
		this.textElement = span;
		return span;
	}
	this.createBar = function() {
		if (!this.container) return;
		var div = document.createElement('div');
		div.className = 'loadingbar loadingbar_'+this.loadingbar_color;
		div.style.display = 'none';
		this.barElement = div;
		return div;
	}
	this.createBr = function() { 
		if (!this.container) return;
		this.brElement = document.createElement('br'); 
		this.brElement.style.display = 'none';
		return this.brElement;
	}
	this.updateText = function(text, doAppend) {
		if (!this.container) { updateStatus(text,doAppend); return }
		if (!this.textElement) this.init();
		this.text = text;
		if (!doAppend) this.textElement.firstChild.nodeValue = text;
		else this.textElement.firstChild.nodeValue += text;
		this.textElement.style.display = '';
		this.brElement.style.display = 'none';
		this.barElement.style.display = 'none';
	}
	this.updateBar = function(percentage, tooltip) {
		if (!this.container) { updateStatus(tooltip+percentage); return }
		if (!this.barElement) this.init();
		this.loadingbar_tooltip = tooltip;
		this.loadingbar_percentage = percentage;
		var percentageWidth = this.loadingbar_eachPercent * percentage;
		var newProgress = (this.loadingbar_initial+percentageWidth)+'px';
		this.barElement.style.backgroundPosition = newProgress+' 0';
		this.barElement.title = tooltip + percentage + '%';
		this.textElement.style.display = 'none';
		this.brElement.style.display = 'none';
		this.barElement.style.display = '';
	}
	this.updateBarWithText = function(text, percentage, tooltip) {
		if (!this.container) { updateStatus(text+' - '+tooltip+percentage); return }
		this.updateText(text);
		this.updateBar(percentage,tooltip);
		this.textElement.style.display = '';
		this.brElement.style.display = '';
		this.barElement.style.display = '';
	}
	this.clearAfterTimeout = function(obj,timeout) {
		setTimeout(obj+".clearContainer()", timeout);
	}
}

/* Function that initializes the status div */
function initStatusBox() {
	if (!document.getElementById('statusBox')){
		var statusBox = document.createElement('div');
		statusBox.id = "statusBox"
		document.body.appendChild(statusBox);
		globalStatus.container = statusBox;
	}
}

/* Converts episode numbers from exp notation to interface notation
 * @param epno Episode Number to convert
 * @return converted episode number
 */
function epNoToString(epno) {

  var ret = Number(epno);
  if (isNaN(ret)) return epno;
  if (ret >= 10000) return 'O' + Number(ret - 10000);
  if (ret >= 4000 && ret < 10000) return 'P' + Number(ret - 4000);
  if (ret >= 3000 && ret < 4000) return 'T' + Number(ret - 3000);
  if (ret >= 2000 && ret < 3000) return 'C' + Number(ret - 2000);
  if (ret >= 1000 && ret < 2000) return 'S' + Number(ret - 1000);
  return ret;

}

/* This function formats the file size
 * @param size Size in bytes
 * @param force Should force LAY_FORMATFILESIZE?
 * @return Converted file size
 */
function formatFileSize(size,force) {
  var format = false;
  if (LAY_FORMATFILESIZE) format = LAY_FORMATFILESIZE;
  if (force) format = force;
  if (!format) {
    var aux = new String(size);
    var sz = new Array();
    for (var i = 0; i < aux.length; i++) sz.push(aux.charAt(i));
    aux = ''; 
    var i = sz.length - 1;
    while (i - 2 > 0) { i -= 2; sz.splice(i, 0, '.'); i--; }
    for (i = 0; i < sz.length; i++) aux += sz[i];
    return (aux);
  }
  var tsize = parseInt(size);
  if (tsize < 1000) return(tsize + ' B'); // Byte
  else {
    tsize = tsize / 1024;
    if (tsize < 1000) return(tsize.toFixed(2) + ' KB'); // kilobyte
    else { // megabyte
      tsize = tsize / 1024;
      return(tsize.toFixed(2) + ' MB');
    }
  }
}

/* This function formats a length to a given format
 * @param length Length in seconds
 * @param format The output format ['long'|'rounded']
 * @return Formated length
 */
function formatFileLength(length, format) {
  var tsec = length % 60;
  var tmin = Math.floor(length/60);
  tmin = tmin % 60;
  var thr = Math.floor(length/3600);
  thr = thr % 60;
  var tday = Math.floor(length/86400);
  var output = '';
  if (format == 'long') {
    if (tday) output += tday + ':';
    if (thr) output += thr + ':';
    if (tmin) output += tmin + ':';
    output += tsec;
  } else { // format is 'rounded'
    var minutes = 0;
    if (tday) minutes = tday * 1440;
    if (thr) minutes += thr * 60;
    if (tmin) minutes += tmin;
    if (tsec > 29) minutes += 1;
    output = minutes+'m';
  }
  return output;
}

// QUALITY FUNCTIONS //

function buildQualityIcon(node,quality) {
  var qual = quality.replace(' ','');
  createIcon(node, quality, null, null, 'quality: '+quality, 'i_rate_'+qual);
}

// EPISODE Functions //

/* Finds and returns every mylist entry associated with a given eid
 * @param eid Episode id of the episode to find entries
 * @return Array with found entries
 */
function findMylistEpEntries(eid) {
	var ret = new Array();
	var episode = episodes[eid];
	if (!episode) return ret;
	for (var sd in mylist) {
		var mylistEntry = mylist[sd];
		if (!mylistEntry) continue;
		if (mylistEntry.episodeId != eid && mylistEntry.relatedEids && mylistEntry.relatedEids.indexOf(String(eid)) < 0) continue;
		ret.push(mylistEntry);
	}
	return ret;
}

// SORTING Functions //

function c_undefined_simp(a, b) {
	if (a[1] < b[1]) return -1;
	if (a[1] > b[1]) return 1;
	return 0;
}
function c_undefined(a, b) {
	return a[1] - b[1];
}
function c_undefined_r(a, b) {
	return c_undefined(b, a);
}
function c_string(a, b) {
	var aN = parseFloat(a[1]);
	var bN = parseFloat(b[1]);
	if (!isNaN(aN) && !isNaN(bN)) return aN - bN;
	if (a[1] < b[1]) return -1;
	if (a[1] > b[1]) return 1;
	return a[0] - b[0];
}
function c_string_r(a, b) {
	return c_string(b, a);
}
function c_number(a, b) {
	var aN = Number(a[1]);
	var bN = Number(b[1]);
	if (isNaN(aN)) aN = 0;
	if (isNaN(bN)) bN = 0;
	return aN - bN;
}
function c_number_r(a, b) {
  return c_number(b, a);
}
function dig_text(node) {
	if (!node) {
		//alert('mode -1: '+node.nodeValue);
		return ("0");
	}
	if (node.childNodes.length == 1 && node.firstChild.nodeValue) {
		//alert('mode 0a: '+node.firstChild.nodeValue);
		return node.firstChild.nodeValue;
	} else if (!node.childNodes.length) {
		//alert('mode 0b: '+node.nodeValue);
		return (node.nodeValue ? node.nodeValue : "0");
	}
	for (var i = 0; i < node.childNodes.length; i++) {
		var childNode = node.childNodes[i];
		if (childNode.nodeType == 3 && childNode.nodeValue.replace(/\s/ig,'') == '') { continue; } // #text node }
		//alert('mode 1: '+ childNode.childNodes.length+'\nnodeValue: <'+childNode.nodeValue+'>');
		return dig_text(childNode); // recursive mode, the first node has to have everything!
	}
	//alert('mode 2: 0');
	return ("0");
}
function dig_text_lower(node) {
	return dig_text(node).toLowerCase();
}
function get_blank(node) {
	return "";
}
function get_anidbsort(node) {
	// Should be using node.getAttributeNS("http://anidb.info/markup","sort");
	var ret = node.getAttribute("anidb:sort");
	if (ret == null) ret = dig_text(node); // backup
	return (ret ? ret : 0);
}

function get_date(node) {
	if (node && !node.childNodes.length) string = '?';
	else {
		while (node && !node.nodeValue) { node = node.firstChild; }
		var string = node.nodeValue;
	}
	var findPrecision = function getPrecision(pstring) {
		if (pstring == '?') return Number(new Date(2099,11,31)); // Totaly future date
		if (pstring.indexOf('.') >= 0) {
			var highPrecision = (pstring.indexOf(':') >= 0);
			var datePart = hourPart = "";
			if (highPrecision) {
				var splitChar = String.fromCharCode(160);
				if (pstring.indexOf(splitChar) >= 0) {
					datePart = pstring.substr(0,pstring.indexOf(splitChar));
					hourPart = pstring.substr(pstring.indexOf(splitChar)+1,pstring.length);
				} else {
					if (pstring.indexOf('T') >= 0) { // UTC time
						var datePart = pstring.split('T')[0].split('-').join('.');
						var hourPart = pstring.split('T')[1].split(':').join(':');
					} else if (pstring.indexOf('-') >= 0 && pstring.indexOf(' ') >= 0 && pstring.indexOf(':') >= 0) {
						var datePart = pstring.split(' ')[0].split('-').join('.');
						var hourPart = pstring.split(' ')[1].split(':').join(':');
					} else if (pstring.indexOf('.') >= 0 && pstring.indexOf(' ') >= 0 && pstring.indexOf(':') >= 0) {
						var datePart = pstring.split(' ')[0].split('.').join('.');
						var hourPart = pstring.split(' ')[1].split(':').join(':');
					}
				}
			} else datePart = pstring;
			var edate = datePart.split(".");
			var ehour = hour = minute = 0;
			var now = new Date();
			var year;
			// more date stuff
			if (edate[2] && !isNaN(Number(edate[2]))) {
				year = edate[2];
				if (year.length == 2) {
					if (Number(year[0]) < 4) year = '20'+year;
					else year = '19'+year;
				}
			} else {
				if ((Number(edate[1]) - 1) > now.getMonth()) year = (now.getFullYear()-1);
				else year = now.getFullYear();
			}
			var month = (edate[1] != '?' ? Number(edate[1]) - 1 : 11);
			var day;
			if (edate[0] == '?') {
				var aux = 31;
				var tdate = new Date(year,month,aux);
				while (aux != tdate.getDate()) { aux--; tdate = new Date(year,month,aux); }
				day = aux;
			} else day = Number(edate[0]);
			if (highPrecision) {
				ehour = hourPart.split(":");
				hour = Number(ehour[0]);
				minute = Number(ehour[1]);
			}
			return Number((highPrecision ? new Date(year,month,day,hour,minute) : new Date(year,month,day)));
		} else { // very low precision mode
			var pyear = parseInt(pstring);
			if (isNaN(pyear)) return 0;// not even a low precision date
			return Number(new Date(pyear,00,01));
		}
	}
	var isRange = (string.indexOf('till') >= 0 || string.indexOf(' - ') >= 0);
	if (isRange) { 
		var split = (string.indexOf('till') >= 0 ? string.split('till') : string.split(' - '));
		return findPrecision(split[0]); // Ranged date mode, just return the first date
	} else return findPrecision(string); // Single date mode
}

/* This function attaches the sorting function to th's
 * @param node If specified tells the root node [node]
 * @param ident If specified tells to make a special case for the th identified by ident [identifier]
 * @param sortico If specified the th referenced by ident will get this sort icon or the i_down1 icon [up|down] 
 */ 
function init_sorting(node,ident,sortico) {
	if (!node) node = document;
	var headinglist;
	if (!document.getElementsByTagName) return;
	var table = (node.nodeName.toLowerCase() == 'table' ? node : null);
	if (!table) {
		var MAX_RECURSE = 3;
		var table = node.parentNode;
		var depth = 0;
		while (table.nodeName.toLowerCase() != 'table' && depth < MAX_RECURSE) {
			table = table.parentNode;
			depth++
		}
		if (table.nodeName.toLowerCase() != 'table') { errorAlert("init_sorting","either could not find table node or reached MAX_RECURSE ("+depth+")."); return; }
	}
	var tbody = table.tBodies[0];
	// The Sorting functions, see FunctionMap for the actual function names
	var sortFuncs = new Array('c_latin','c_number','c_date','c_set','c_setlatin');
	// We find out if our header is in the tBody of the Table
	// If it's not create a Table Head and move the node there, stuff like this is the reason THEAD exists
	var thead = table.getElementsByTagName('thead')[0];
	var headRow = (tbody.rows[0] && tbody.rows[0].className.indexOf('header') >= 0);
	if (!thead && headRow) {
		thead = document.createElement('thead');
		thead.appendChild(tbody.rows[0]);
		table.insertBefore(thead,tbody);
	}
	headinglist = thead.getElementsByTagName('th');
	for (var i = 0; i < headinglist.length; i++) {
		var header = headinglist[i];
		// Add some accelerators to this
		// Parent Table
		header._parentTable = table;
		// Sorting Function
		var sortfunc = header.className.substring(header.className.indexOf(" c_")+1,(header.className.indexOf(" ",header.className.indexOf(" c_")+1)+1 || header.className.length+1)-1);
		header._sortFunction = (sortfunc.indexOf('c_') == -1 || sortfunc == 'c_none' ? 'c_none' : sortfunc);
		// Cell identifier
		header._identifier = (header.className && header.className.length > 0 ? header.className.substring(0,header.className.indexOf(" ")) || header.className : null);
		if (header._sortFunction == 'c_none') continue;
		header.onclick = sortcol; // This applies the actual sorting behaviour
		// And the following adds the icons (Optional, it's just a visual input)
		if (typeof(ident) == 'string') {
			if (ident && ident.length) {
				if (header._identifier.indexOf(ident) >= 0) {
					if (sortico && sortico.length > 0) {
						if (sortico == 'up')	header.className += ' s_forward';
						if (sortico == 'down')	header.className += ' s_reverse';
					} else header.className += ' s_reverse';
				}
			}
		} else { // passed an actual th node
			if (header == ident) {
				if (sortico && sortico.length > 0) {
					if (sortico == 'up')	header.className += ' s_forward';
					if (sortico == 'down')	header.className += ' s_reverse';
				} else header.className += ' s_reverse';
			}
		}
	}
	FunctionMap = {'c_latin':{'sortf':c_string, 'sortr':c_string_r, 'getval':dig_text_lower},
		'c_number':{'sortf':c_number, 'sortr':c_number_r, 'getval':dig_text},
		'c_date':{'sortf':c_number, 'sortr':c_number_r, 'getval':get_date},
		'c_set':{'sortf':c_number, 'sortr':c_number_r, 'getval':get_anidbsort},
		'c_setlatin':{'sortf':c_string, 'sortr':c_string_r, 'getval':get_anidbsort},
		'c_none':{'sortf':c_undefined, 'sortr':c_undefined_r, 'getval':get_blank} }  
}

/* Finds the active sort col and return it's identifier
 * @param node If specified tells the root node [node]
 */
function findSortCol(node) {
  if (!node) node = document;
  var headinglist;
	if (document.getElementsByTagName)
		headinglist = node.getElementsByTagName('th');
	else 	
	  return;
	for (var i = 0; i < headinglist.length; i++) {
    var heading = headinglist[i];
    if (heading.className.indexOf('s_forward') >= 0 || heading.className.indexOf('s_reverse') >= 0) {
      return heading.className.substring(0,heading.className.indexOf(" ")) || heading.className;
    } else continue;
  }
  return null;
}
/* Function that actualy sorts a column */
function sortcol(node) {
	var start = new Date();
	var MAX_RECURSE = 3;
	if (!node || !node.nodeName || (node.nodeName && node.nodeName.toLowerCase() != 'th')) node = this;
	if (node.nodeName && node.nodeName.toLowerCase() != 'th') return; //only allow sorting for THs
	// Okay, now for some assumptions (i can live without each, but they are nice to have):
	// #1 - init_sorting should have moved every th row to thead
	// #2 - init_sorting should have assigned each TH it's sort function
	// #3 - init_sorting should have assigned each TH it's parent Table node
	// #4 - init_sorting should have assigned each TH it's identifier
	var table = this._parentTable;
	if (!table || (table && table.nodeName.toLowerCase() != 'table')) {
		table = node.parentNode;
		// find the table node up to a given recursive depth and jumpstart this
		var depth = 0;
		while (table.nodeName.toLowerCase() != 'table' && depth < MAX_RECURSE) {
			table = table.parentNode;
			depth++
		}
		if (table.nodeName.toLowerCase() != 'table') { errorAlert("sortcol","either could not find table node or reached MAX_RECURSE ("+depth+")."); return; }
		node._parentTable = table;
	}
	var tbody = table.tBodies[0];
	if (!tbody) { errorAlert("sortcol","could not find tbody node."); return; }
	// if this th is not in thead there is probably a reason so just ignore it and go with the flow
	var startIndex = 0;
	if (node.parentNode.parentNode.nodeName.toLowerCase() == 'tbody')
		startIndex = Number(node.parentNode.rowIndex)+1; 
	var sortfunc = this._sortFunction;
	if (!sortfunc) {
		//  We now find out which sort function to apply to the column or none
		sortfunc = node.className.substring(node.className.indexOf(" c_")+1,(node.className.indexOf(" ",node.className.indexOf(" c_")+1)+1 || node.className.length+1)-1);
		sortfunc = (sortfunc.indexOf('c_') < 0 || sortfunc == 'c_none' ? 'c_none' : sortfunc);
		node._sortFunction = sortfunc;
	}
	if (sortfunc == 'c_none') return;
	var funcmap = FunctionMap[sortfunc];
	// sorting mode
	var sortingMode = (node.className.indexOf("s_reverse") >= 0 ? "sortr" : "sortf");
	// reset headers
	var headingNodes = node.parentNode.getElementsByTagName('th');
	for (var i = 0; i < headingNodes.length; i++) {
		var cell = headingNodes[i];
		cell.className = cell.className.replace(/ s_forward|s_forward|s_reverse| s_reverse/ig,"");
	}
	// reset our node sorting classname
	node.className += (sortingMode == 'sortr' ? ' s_forward' : ' s_reverse');

	// Now actualy do some stuff
	var sortMap = new Array();
	var rows = tbody.getElementsByTagName('tr');
	var cellIndex = node.cellIndex; // by default use this, faster
	var identifier = node._identifier; // use this if a row's cells length is lesser than the heading row cells length
	if (!identifier) {
		identifier = node.className.substring(0,node.className.indexOf(" ")) || node.className;
		node._identifier = identifier;
	}

	tbody.style.display = 'none';

	var rowIndex = -1;
	var tmpRows = new Array();
	var cellValue = "";
	var lastRowSpan = false;
	var auxRows = new Array();
	while (rows.length > startIndex) {
		var row = rows[startIndex];
		tbody.removeChild(row);
		var rowSpan = (row.className.indexOf('rowspan') >= 0);
		if (!rowSpan) rowIndex++;
		var cells = row.getElementsByTagName('td');
		var cell = null;
		if (cells.length == headingNodes.length) // do this the easy way
			cell = cells[cellIndex];
		else { // hard way, find the cell by identifier
			for (var k = 0; k < cells.length; k++) {
				if (cells[k].className.indexOf(identifier) < 0) continue;
				cell = cells[k];
				break;
			}
		}
		if (!auxRows[rowIndex]) auxRows[rowIndex] = new Array();
		auxRows[rowIndex].push([auxRows[rowIndex].length,(cell ? funcmap['getval'](cell) : '0'),row]);
		auxRows[rowIndex].sort(funcmap[sortingMode]);
		if (!rowSpan) {
			sortMap.push([rowIndex,auxRows[rowIndex][0][1]]);
			cellValue = "";
			lastRowSpan = rowSpan;
		}
	}
	if (seeDebug) {
		var text = new Array();
		for (var i = 0; i < sortMap.length; i++) text.push(sortMap[i][0]+'|'+sortMap[i][1]+'\t');
	}
	// now i have to make sure the inner rows of a row span are correct (this is insanity)
	for (var i = 0; i < auxRows.length; i++) {
		var rowMap = auxRows[i];
		if (rowMap.length == 1) continue; //no need for special handling
		// i'll assume the row with index 0 is allways the parent row, if this is not right, it will blow in your face
		if (rowMap[0][0] != 0) { // special handling
			var parentRow = null;
			var pRowIndex = 0;
			for (var k = 0; k < rowMap.length; k++) {
				var map = rowMap[k];
				if (map[0] != 0) continue;
				parentRow = map[2];
				pRowIndex = k;
				break; // no need to continue;
			}
			// okay found the fucking parent row
			// how to do this the sane way, which is wrong, because there is no sane way to do this, if i was sane i would drop this
			var fakeRow = document.createElement('tr');
			fakeRow.id = parentRow.id;
			fakeRow.className = parentRow.className.replace(/rowspan| rowspan|rowspan /ig,'');
			var newPRow = document.createElement('tr');
			newPRow.className = parentRow.className + ' rowspan';
			var fakeCell = 0;
			// What am i doing? (besides being insane)
			// For every cell in the original Row, if it's rowSpan is > 1 (meaning it's a shared cell)
			// 		move the contents to the new fakeRow (which will be row 0, in rowMap[0])
			// otherwise,
			//		copy the contents of the given cell in rowMap[0][2].cells[cellIndex] to the fakeRow
			//		also move the contents of the cell to the newPRow (otherwise known as the row that used to be the original row)
			// Because javascript does its thing by copy by reference this magic can work otherwise it would be hell
			var cellValue = sortMap[i][1];
			//alert('original cell value for row ['+i+']: '+cellValue);
			while(parentRow.cells.length) {
				var cell = parentRow.cells[0];
				if (cell.rowSpan > 1) {
					fakeRow.appendChild(cell);
					if (cell.className.indexOf(identifier) >= 0) {
						cellValue = funcmap['getval'](cell);
						//alert('for row ['+i+'] (not rowspan) i found this cellValue: '+cellValue);
					}
				} else {
					fakeRow.appendChild(rowMap[0][2].cells[fakeCell].cloneNode(true));
					if (fakeRow.cells[fakeRow.cells.length-1].className.indexOf(identifier) >= 0) {
						cellValue = funcmap['getval'](fakeRow.cells[fakeRow.cells.length-1]);/*
						alert('for row ['+i+'] (rowspan) i found this cellValue: '+cellValue+
							'\nidentifier: '+identifier+
							'\nclassName: '+fakeRow.cells[fakeRow.cells.length-1].className);*/
					}
					newPRow.appendChild(cell);
					fakeCell++;
				}
			}
			rowMap[0][2] = fakeRow;
			rowMap[pRowIndex][2] = newPRow;
			sortMap[i][1] = cellValue;
		}
	}
	// now sort the map
	if (seeDebug) for (var i = 0; i < sortMap.length; i++) text[i]+=sortMap[i][0]+'|'+sortMap[i][1]+'\t';
	sortMap.sort(funcmap[sortingMode]);
	if (seeDebug) {
		for (var i = 0; i < sortMap.length; i++) text[i]+=sortMap[i][0]+'|'+sortMap[i][1];
		alert('[in]\t[ms]\t[out]:\n'+text.join('\n'));
	}
	// and re-add the sorted rows and repaint the rows
	for (var i = 0; i < sortMap.length; i++) {
		var map = sortMap[i];
		var tmpRows = auxRows[map[0]];
		var rl = tmpRows.length;
		var ik = i;
		for (var k = 0; k < rl; k++) {
			var row = tmpRows[k][2];
			if (row.style.display != 'none') {
				if (rl < 3) // single row case or just one extra rowspan
					row.className = ((i % 2) ? "" : "g_odd ")+row.className.replace(/ g_odd|g_odd/ig,"");
				else {	// multi row case, not so easy one
					row.className = ((ik % 2) ? "" : "g_odd ")+row.className.replace(/ g_odd|g_odd/ig,"");
					ik++;
				}
			}
			tbody.appendChild(row);
		}
	}
	
	// this should do something
	if (table.normalize) table.normalize();
	
	// DO NOT USE repaintStripes, takes too long to execute and locks up
	tbody.style.display = '';

	if (seeDebug) {
		alert("sorted in "+(new Date() - start)+" ms"+
			"\nsorting function to apply: "+sortfunc+"\nsorting order: "+sortingMode+"\nidentifier: "+identifier+"\ntable.id: "+table.id);
	}
}

/* Function that repaints the stripes of a table
 * Note: this function for some odd reason can lock up rendering for a while
 * @param table Table (or tbody) to paint stripes 
 * @param startAt optional index where to start painting stripes
 */
function repaintStripes(table, startAt) {
	if (!table || (table && (table.nodeName.toLowerCase() != 'table' && table.nodeName.toLowerCase() != 'tbody'))) return;
	if (!startAt) startAt = 0;
	var tbody = (table.nodeName.toLowerCase() == 'table') ? table.tBodies[0] : table;
	if (!tbody) return;
	var rows = tbody.getElementsByTagName('tr');
	for (var i = startAt; i < rows.length; i++) {
		var row = rows[i];
		row.className = ((i % 2) ? "g_odd " : "")+row.className.replace(/ g_odd|g_odd/ig,"");
	}
}

// The hash Object holds hashing defaults
var hashObj = new Object();
hashObj.usePatterns = true;
hashObj.convertSpaces = true;
hashObj.defaultSpacesChar = '_';
hashObj.spacesChar = hashObj.defaultSpacesChar;
hashObj.defaultPattern = "%ant - %enr%ver - %ept - <[%grp]><(%crc)><(%cen)><(%lang)><(%raw)>";
hashObj.pattern = hashObj.defaultPattern;
hashObj.ed2k = "ed2k://|file|"+hashObj.pattern+".%ext|%flen|%ed2k|";
hashObj.sfv = hashObj.pattern+".%ext %crc";
hashObj.validHashes = [ "ed2k", "sfv" ];

var validIdentifiers = ["%ant","%anat","%ept","%epat","%enr","%pn","%fpn","%raw",
                        "%crc","%CRC","%ver","%cen","%dub","%sub","%lang","%flang",
                        "%grp","%grn","%qual","%src","%res","%vcodec","%eps","%atype",
                        "%fid","%aid","%eid","%gid","%dlen","%ext","%ed2k","%uncen",
                        "%acodec","%achans","%hlen","%flen"]
/* Function that tests if a given identifier is valid
 * @param identifier The identifier to test
 * @return true|false
 */
function checkIdentifiers(identifier) {
  for (var i = 0; i < validIdentifiers.length; i++) {
    if (identifier.indexOf(validIdentifiers[i]) >= 0) return true;
  }
  return false;
}
/* Function that creates the link for a given hash
 * @return void (sets the hash href) 
 */
function applyFormat(identifier, file, episode, anime, group) {
  var originalIdentifier = identifier;
  var dropIfNull = false;
  if (identifier.indexOf('<') >= 0) {  
    originalIdentifier = originalIdentifier.substr(originalIdentifier.indexOf('<')+1,originalIdentifier.indexOf('>')-1);
    identifier = identifier.match(/(\%[A-Z]+)/i)[0];
    originalIdentifier = originalIdentifier.replace(identifier,"%replaceme");
    dropIfNull = true;
  }
  //alert('identifier: '+identifier+' ('+originalIdentifier+') exists? '+checkIdentifiers(identifier));
  if (!checkIdentifiers(identifier)) return ("");
  identifier = identifier.replace("%ant",anime.getTitle());
  identifier = identifier.replace("%anat",anime.getAltTitle());
  identifier = identifier.replace("%ept",episode.getTitle());
  identifier = identifier.replace("%epat",episode.getAltTitle());
  if (identifier.indexOf("%enr") >= 0) {
    var epLen = String((anime.eps) ? anime.eps : anime.epCount);
    var epFmt = '0000'+episode.epno;
    epFmt = epFmt.slice(epFmt.length-epLen.length);
    identifier = identifier.replace("%enr",episode.typeChar+epFmt); 
  }
  identifier = identifier.replace("%pn",(anime.type == 'movie') ? "PA" : "EP");
  identifier = identifier.replace("%fpn",(anime.type == 'movie') ? "Part" : "Episode");
  if (identifier.indexOf("%raw") >= 0) {
    if (file.type == 'video' && file.subtitleTracks.length == 0)
      identifier = identifier.replace("%raw",(file.audioTracks.length == 1 && file.audioTracks[0].lang == 'ja') ? "RAW" : "");
    else identifier = identifier.replace("%raw","");
  }
  identifier = identifier.replace("%crc",(file.crcStatus == 'invalid') ? "INVALID" : file.crc32);
  identifier = identifier.replace("%CRC",(file.crcStatus == 'invalid') ? "INVALID" : file.crc32.toUpperCase());
  identifier = identifier.replace("%ver",(file.version != 'v1') ? file.version : "");
  identifier = identifier.replace("%cen",(file.isCensored) ? "cen" : "");
  identifier = identifier.replace("%uncen",(file.isUncensored) ? "uncen" : "");
  if (identifier.indexOf("%dub") >= 0) {
    var dub = new Array();
    for (var i = 0; i < file.audioTracks.length; i++) dub.push(file.audioTracks[i].lang);
    identifier = identifier.replace("%dub",(dub.length) ? dub.join(',') : "");
  }
  if (identifier.indexOf("%sub") >= 0) {
    var sub = new Array();
    for (var i = 0; i < file.subtitleTracks.length; i++) sub.push(file.subtitleTracks[i].lang);
    identifier = identifier.replace("%sub",(sub.length) ? sub.join(',') : "");
  }
  if (identifier.indexOf("%lang") >= 0 || identifier.indexOf("%flang") >= 0) {
    var dub = new Array();
    for (var i = 0; i < file.audioTracks.length; i++) {
      if (file.audioTracks[i].lang == "ja") continue;
      if (identifier.indexOf("%lang") >= 0 && dub.length > 1) { dub.push("+"); break; }
      dub.push(file.audioTracks[i].lang);
    }
    var sub = new Array();
    for (var i = 0; i < file.subtitleTracks.length; i++) {
      if (file.subtitleTracks[i].lang == "en") continue;
      if (identifier.indexOf("%lang") >= 0 && sub.length > 1) { sub.push("+"); break; }
      sub.push(file.subtitleTracks[i].lang);
    }
    var langs = "";
    if (dub.length) langs += 'dub';
    if (dub.length && sub.length) langs += '.';
    if (sub.length) langs += 'sub';
    if (dub.length || sub.length) langs += '_';
    langs += dub.join();
    if (dub.length && sub.length) langs += '.';
    langs += sub.join();
    if (langs == 'dub.sub_ja.en') langs = "";
    if (identifier.indexOf("%lang") >= 0) identifier = identifier.replace("%lang",langs);
    if (identifier.indexOf("%flang") >= 0) identifier = identifier.replace("%flang",langs);
  }
  identifier = identifier.replace("%grp",(group) ? group.shortName : '');
  identifier = identifier.replace("%grn",(group) ? group.name : '');
  identifier = identifier.replace("%qual",(file.quality != 'unknown') ? file.quality : "");
  identifier = identifier.replace("%src",file.source);
  identifier = identifier.replace("%vcodec",(file.type == 'video' && file.videoTracks.length) ? file.videoTracks[0].codec : "");
  identifier = identifier.replace("%acodec",((file.type == 'video' || file.type == 'audio') && file.audioTracks.length) ? file.audioTracks[0].codec : "");
  identifier = identifier.replace("%achans",((file.type == 'video' || file.type == 'audio') && file.audioTracks.length && file.audioTracks[0].chan != 'unknown') ? mapAudioChannels(file.audioTracks[0].chan) : "");
  identifier = identifier.replace("%res",(file.type == 'video' && file.resolution != 'unknown') ? file.resolution : "");
  identifier = identifier.replace("%eps",anime.eps);
  identifier = identifier.replace("%atype",(anime.type != 'unknown') ? mapAnimeType(anime.type) : "");
  identifier = identifier.replace("%fid",file.id);
  identifier = identifier.replace("%gid",file.groupId);
  identifier = identifier.replace("%eid",file.episodeId);
  identifier = identifier.replace("%aid",file.animeId);
  identifier = identifier.replace("%flen",file.size);
  identifier = identifier.replace("%dlen",formatFileSize(file.size,false));
  identifier = identifier.replace("%hlen",formatFileSize(file.size,true));
  identifier = identifier.replace("%ext",file.fileType);
  identifier = identifier.replace("%ed2k",file.ed2k);
  if (dropIfNull) {
    if (identifier != '') identifier = originalIdentifier.replace("%replaceme",identifier);
    else identifier = "";
  }
  return (identifier);
}

function createHashLink() {
  var ahref = this.getElementsByTagName('a')[0];
  //if (ahref.href.indexOf("!fillme!") < 0) return; // we allready have the hash
  var parentid = this.parentNode.id;
  var fid = Number((parentid.indexOf('fid_') >= 0) ? this.parentNode.id.split('fid_')[1] : this.parentNode.id.split('f')[1]);
  var file = files[fid];
  if (!file) return;
  
  var episode = episodes[file.episodeId];
  var curAnime = animes[file.animeId];
  var group = (file.groupId != 0) ? groups[file.groupId] : null;
  var possibleHashTypes = this.className.split(' ');
  var i;
  var found = false;
  for (i = 0; i < possibleHashTypes.length; i++) {
    if (hashObj.validHashes.indexOf(possibleHashTypes[i]) >= 0) { found = true; break; }
  }
  if (!found) return;
  var hashType = possibleHashTypes[i];

  if (!hashObj.usePatterns) hashObj.pattern = hashObj.defaultPattern;
  var pattern = hashObj[hashType]; 
  //alert('pattern.in: '+pattern); 
  var lt = 0; var gt = 0; // Find case '<' and '>' matches
  for (var i = 0; i < pattern.length; i++) {
    if (pattern.charAt(i) == '<') lt++;
    if (pattern.charAt(i) == '>') gt++;
  }
  if ((lt == gt) && (lt > 0)) { // only continues if lt == gt
    while (pattern.lastIndexOf("<") != -1) { // first get rid of the conditional patterns
      var i = pattern.lastIndexOf("<");
      var fI = 0;
      while (pattern.indexOf(">",fI) != -1) {
        var k = pattern.indexOf(">",fI);
        if (k < i) {
          fI = k + 1;
          continue;
        } // we have found a pair of thingies
        var splitstr = pattern.slice(i,k+1);
        pattern = pattern.replace(splitstr,function(str, offset, s) { return applyFormat(str,file,episode,anime,group); });
        break; // continue to next match thingie
      }
    }
  }
  pattern = applyFormat(pattern,file,episode,anime,group);
  if (hashObj.convertSpaces) pattern = pattern.replace(/ /mgi,hashObj.spacesChar);
  //alert('pattern.out: '+pattern);
  ahref.href = pattern+'/';
  file.ed2klink = pattern+'/';
}

// TOOLTIP FUNCTIONS //

var offsetfromcursorX=12;       //Customize x offset of tooltip
var offsetfromcursorY=10;       //Customize y offset of tooltip
var offsetdivfrompointerX=0;   //Customize x offset of tooltip div relative to pointer image
var offsetdivfrompointerY=0;   //Customize y offset of tooltip div relative to pointer image. Tip: Set it to (height_of_pointer_image-1).

var divHTMLTOOLTIP; //tooltip

var ie = document.all;
var ns6 = document.getElementById && !document.all;
var enabletip = false;

//var pointerobj = document.all ? document.all["dhtmlpointer"] : document.getElementById? document.getElementById("dhtmlpointer") : "";

function ietruebody(){
	return (document.compatMode && document.compatMode!="BackCompat") ? document.documentElement : document.body;
}

/* Sets the tooltip */
function setTooltip(thetext, dom, thewidth, thecolor, minWidth){
	while (divHTMLTOOLTIP.childNodes.length) divHTMLTOOLTIP.removeChild(divHTMLTOOLTIP.firstChild);
	if (!thetext) return;
	if (typeof thewidth != "undefined") {
		if (thewidth != 'auto') divHTMLTOOLTIP.style.width = thewidth + 'px';
		else divHTMLTOOLTIP.style.width = thewidth;
	}
	if (typeof thecolor != "undefined" && thecolor != "") divHTMLTOOLTIP.style.backgroundColor = thecolor;
	if (typeof dom != "undefined") {
		if (dom) divHTMLTOOLTIP.appendChild(thetext);
		else divHTMLTOOLTIP.innerHTML = thetext;
	}
	if (typeof minWidth != "undefined") {
		divHTMLTOOLTIP.style.minWidth = minWidth + 'px';
	}
	enabletip = true;
	divHTMLTOOLTIP.style.visibility = "visible";
	return false;
}

function positionTooltip(e){
	if (!enabletip) return;
	divHTMLTOOLTIP.style.position = 'absolute';
	divHTMLTOOLTIP.style.visibility = "visible";
	var nondefaultpos = false;
	var curX = 0;
	var curY = 0;
	if (!e) e = window.event;
	if (e.pageX || e.pageY) { curX = e.pageX; curY = e.pageY; }
	else if (e.clientX || e.clientY) { curX = e.clientX + document.documentElement.scrollLeft; curY = e.clientY + document.documentElement.scrollTop; }
	//Find out how close the mouse is to the corner of the window
	var winwidth = ie && !window.opera ? ietruebody().clientWidth : window.innerWidth - 20;
	var winheight = ie && !window.opera ? ietruebody().clientHeight : window.innerHeight - 20;
	var rightedge = ie && !window.opera ? winwidth - event.clientX - offsetfromcursorX : winwidth - e.clientX - offsetfromcursorX;
	var bottomedge = ie && !window.opera ? winheight - event.clientY - offsetfromcursorY : winheight - e.clientY - offsetfromcursorY;
	var leftedge = (offsetfromcursorX < 0) ? offsetfromcursorX * (-1) : - 5000;
	//if the horizontal distance isn't enough to accomodate the width of the context menu
	if (rightedge < divHTMLTOOLTIP.offsetWidth) { //move the horizontal position of the menu to the left by it's width
		divHTMLTOOLTIP.style.left = (curX - divHTMLTOOLTIP.offsetWidth) + 'px';
		nondefaultpos = true;
	}
	else if (curX < leftedge) divHTMLTOOLTIP.style.left = "5px";
	else { divHTMLTOOLTIP.style.left = (curX + offsetfromcursorX - offsetdivfrompointerX) + 'px'; 	} //position the horizontal position of the menu where the mouse is positioned
	if (bottomedge < divHTMLTOOLTIP.offsetHeight) { divHTMLTOOLTIP.style.top = (curY - divHTMLTOOLTIP.offsetHeight - offsetfromcursorY) + 'px'; nondefaultpos = true; } //same concept with the vertical position
	else { divHTMLTOOLTIP.style.top = (curY + offsetfromcursorY + offsetdivfrompointerY) + 'px';	}
}

function hideTooltip(){
	enabletip = false;
	divHTMLTOOLTIP.style.visibility = "hidden";
	while (divHTMLTOOLTIP.childNodes.length) divHTMLTOOLTIP.removeChild(divHTMLTOOLTIP.firstChild);
	divHTMLTOOLTIP.style.left = "-5000px";
	divHTMLTOOLTIP.style.backgroundColor = '';
	divHTMLTOOLTIP.style.width = '';
}

function initTooltips(nostyle) {
	if (!document.getElementById('obj-tooltip')){
		divHTMLTOOLTIP = document.createElement('div');
		if (!nostyle) divHTMLTOOLTIP.id = "obj-tooltip"
		document.body.appendChild(divHTMLTOOLTIP);
		document.onmousemove = positionTooltip;
	}
}

// Something i didn't want to put on anidbscript because, well, i don't want it there :P
/* a function to confirm revokes for reviews */
function confirmRevokes() {
	var divs = ['reviewmenu']; // list of div targets
	for (var i = 0; i < divs.length; i++) {
		var div = getElementsByClassName(document.getElementsByTagName('div'), divs[i], false)[0];
		if (!div) return;
		var as = div.getElementsByTagName('a');
		for (var k = 0; k < as.length; k++) {
			var a = as[k];
			var text = a.firstChild.nodeValue;
			if (text.indexOf('revoke') >= 0) a.onclick = function confirmRevoke() { if (!confirm("Are you sure you wish to revoke your vote?")) return false; }
			else if (text.indexOf('delete') >= 0) a.onclick = function confirmDelete() { if (!confirm("Are you sure you wish to delete this review?")) return false; }
			else continue;
		}
	}
}

// hook up the window onload event
addLoadEvent(confirmRevokes);