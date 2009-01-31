/* @file addcharacter page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.0 (17.05.2007)
 */
jsVersionArray.push({
	"file":"anime3/addcharacter.js",
	"version":"1.0",
	"revision":"$Revision: 1996 $",
	"date":"$Date:: 2009-01-03 23:18:53 +0000#$",
	"author":"$Author: fahrenheit $",
	"changelog":"Initial version"
});

var seeDebug = false;
var magicRows = new Array();
var tbody = null;
var resulttable = null;
var guiseinput = null;
var newguiseinput = null;
var searchinput = null;
var searchString = "";
var CharInfos = new Array(); // indexed by Charid
var picbase = 'http://img5.anidb.net/pics/anime/';
var charInfo = new Array();
var cids = new Array();

/* Replacement functions
 * @param str String to replace identifiers
 * @source anime3.formating::convert_input
 */
function convert_input(str) {
	str = str.replace(/\[(p|b|i|u|ul|ol|li)\]/mgi,'<$1>');
	str = str.replace(/\[\/(p|b|i|u|ul|ol|li)\]/mgi,'</$1>');
	str = str.replace(/\[([/])?s\]/mgi,'<$1strike>');
	str = str.replace(/\[([/])?code\]/mgi,'<$1pre>');
	str = str.replace(/\<p\>/mgi,'');
	str = str.replace(/\<\/p\>/mgi,'<br />');
	str = str.replace(/\[br\]/mgi,'<br />');
	str = str.replace(/\n/mgi,'<br />');
	str = str.replace(/\<\/li\>\<br \/\>/mgi,'</li>');
	str = str.replace(/\<\/ul\>\<br \/\>\<br \/\>/mgi,'</ul></br>');
	str = str.replace(/\<\/ol\>\<br \/\>\<br \/\>/mgi,'</ol></br>');
	str = str.replace(/\[url=([^\[\]].+?)\]([^\:\\\/\[\]].+?)\[\/url\]/mgi,'<a href="$1">$2</a>');
	str = str.replace(/\[url\]([^\:\\\/\[\]].+?)\[\/url\]/mgi,'<a href="$1">$1</a>');
	str = str.replace(/\[img\]([^\[\]].+?)\[\/img\]/mgi,'<img src="$1" alt="" />');
	return (str);
}

/* Function that changes the layout of the page */
function changeLayout() {
	var type = Number(this.value);
	switch(type) {
		case 1: // character
			magicRows['gender'].style.display = '';
			magicRows['bloodtype'].style.display = '';
			magicRows['height'].style.display = '';
			magicRows['weight'].style.display = '';
			magicRows['bust'].style.display = '';
			magicRows['waist'].style.display = '';
			magicRows['hips'].style.display = '';
			break;
		case 2: // mecha
			magicRows['gender'].style.display = 'none';
			magicRows['bloodtype'].style.display = 'none';
			magicRows['height'].style.display = '';
			magicRows['weight'].style.display = '';
			magicRows['bust'].style.display = 'none';
			magicRows['waist'].style.display = 'none';
			magicRows['hips'].style.display = 'none';
			break;
		default: // organization and future types?
			magicRows['gender'].style.display = 'none';
			magicRows['bloodtype'].style.display = 'none';
			magicRows['height'].style.display = 'none';
			magicRows['weight'].style.display = 'none';
			magicRows['bust'].style.display = 'none';
			magicRows['waist'].style.display = 'none';
			magicRows['hips'].style.display = 'none';
			break;
	}
	// repaint stripes (i guess someone will complain about this, so i better do it..
	if (!tbody) { errorAlert('changeLayout',"tbody is missing"); return; }
	var rows = tbody.rows;
	var k = 0;
	for (var i = 0; i < rows.length; i++) {
		var row = rows[i];
		if (row.style.display == 'none') continue;
		row.className = row.className.replace('g_odd ','');
		row.className = (k % 2 ? '' : 'g_odd ') + row.className;
		k++;
	}
}

/* Creates a new CharInfo node */
function CCharInfo(node) {
	this.charid = Number(node.getAttribute('charid'));
	this.desc = convert_input(node.getAttribute('desc'));
	if (this.desc == '') this.desc = '<i>no description</i>';
	this.title = node.getAttribute('title');
	this.picurl = node.getAttribute('picurl');
	this.restricted = false; // override
	if (this.picurl != 'nopic.gif' && this.picurl != '') this.picurl = picbase+'thumbs/150/'+this.picurl+'-thumb.jpg';
	else this.picurl = 'http://static.anidb.net/pics/nopic_150.gif';
	this.lang = node.getAttribute('mainlang');
}

/* CharDesc Node Parser */
function CCDesc(node) {
	this.charid = Number(node.getAttribute('charid'));
	this.picurl = node.getAttribute('picurl');
	if (this.picurl != 'nopic.gif' && this.picurl != '') this.picurl = picbase+'thumbs/50x65/'+this.picurl+'-thumb.jpg';
	else this.picurl = 'http://static.anidb.net/pics/nopic_50x65.gif';
	this.desc = convert_input(node.getAttribute('desc'));
	this.name = node.getAttribute('name');
	this.verified = (Number(node.getAttribute('verifydate')) > 0);
	this.lang = node.getAttribute('lang');
	this.title = node.getAttribute('title');
	this.mainlang = node.getAttribute('mainlang');
	this.type = Number(node.getAttribute('type'));
	var type = "Main";
	switch(this.type) {
		case 1: type = "Main"; break;
		case 2: type = "Official"; break;
		case 3: type = "Alias"; break;
		case 4: type = "Maiden"; break;
		case 5: type = "Nick"; break;
		case 6: type = "Short"; break;
		case 7: type = "other"; break;
		default: type = "unknown"; break;
	}
	this.type = type;
	this.titles = new Array();
	if (this.type != 'Main') {
		this.titles[this.type] = new Array();
		this.titles[this.type].push({'title':this.name,'lang':this.lang,'verified':this.verified});
	}
	// now, build the anime info node
	if (!CharInfos[this.charid])
		CharInfos[this.charid] = new CCharInfo(node);
}

/* Function that fetches char data
 * @param type Type of data to fetch
 * @param search Search string
 */
function fetchData(type,searchString) {
	var req = xhttpRequest();
	if (type == "charbyid") {
		if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/char'+searchString+'_desc.xml', prepAjaxSearch);
		else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=characterdescbyid&id='+searchString, prepAjaxSearch);
	} else {
		if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/characterdescsearch.xml', parseData);
		else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=characterdesc&search='+escape(searchString), parseData);
	}
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function prepAjaxSearch(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','no root node'); return; }
	var cdescNodes = root.getElementsByTagName('characterdescbyid');
	cids = new Array();
	for (var d = 0; d < cdescNodes.length; d++) {
		var descNode = new CCDesc(cdescNodes[d]);
		if (cids.indexOf(descNode.charid) < 0) cids.push(descNode.charid);
		if (!charInfo[descNode.charid]) charInfo[descNode.charid] = descNode;
		else if (descNode.type != 'Main') {
			if (!charInfo[descNode.charid].titles[descNode.type]) charInfo[descNode.charid].titles[descNode.type] = new Array();
			charInfo[descNode.charid].titles[descNode.type].push({'title':descNode.name,'lang':descNode.lang,'verified':descNode.verified});
		} else
			continue; // a bit pointless, i know :P
	}
	// now work up some magic
	guiseinput.value = charInfo[cids[0]].title;
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','no root node'); return; }
	var cdescNodes = root.getElementsByTagName('characterdesc');
	cids = new Array();
	for (var d = 0; d < cdescNodes.length; d++) {
		var descNode = new CCDesc(cdescNodes[d]);
		if (cids.indexOf(descNode.charid) < 0) cids.push(descNode.charid);
		if (!charInfo[descNode.charid]) charInfo[descNode.charid] = descNode;
		else if (descNode.type != 'Main') {
			if (!charInfo[descNode.charid].titles[descNode.type]) charInfo[descNode.charid].titles[descNode.type] = new Array();
			charInfo[descNode.charid].titles[descNode.type].push({'title':descNode.name,'lang':descNode.lang,'verified':descNode.verified});
		} else
			continue; // a bit pointless, i know :P
	}
	if (!cdescNodes.length) { // no matches found
		alert('Search for "'+searchString+'" resulted in no matches.');
		return;
	}
	showResults();
	searchinput.value = "search";
	searchinput.onclick = doSearch;
}

/* Function that actualy shows the tooltip
 * @param obj The object that will be base for the tooltip position
 * @param info The AnimeInfo object
 * @param isChar If set to true we are displaying character info
 */
function showInfoWork(obj,info,isChar) {
	// i don't have the profile value options so, just use defaults
	var minWidth = 450;
	var table = document.createElement('table');
	table.className = 'characterInfo';
	table.style.minWidth = minWidth + 'px';
	var thead = document.createElement('thead');
	var row = document.createElement('tr');
	var title = document.createElement('span');
	title.className = 'title';
	title.appendChild(document.createTextNode(info.title));
	createHeader(row, (info.restricted ? 'restricted' : null), title, null, null, 2);
	thead.appendChild(row);
	table.appendChild(thead);
	var tbody = document.createElement('tbody');
	row = document.createElement('tr');
	var img = document.createElement('img');
	img.src = info.picurl;
	img.alt = '['+info.charid+']';
	createCell(row, 'characterInfoThumb', img);
	var span = document.createElement('span');
	span.innerHTML = info.desc;
	createCell(row, 'characterInfoDesc', span);
	tbody.appendChild(row);
	table.appendChild(tbody);
	setTooltip(table, true, minWidth);
}

/* Function that shows char info (or not) */
function showCharacterInfo() {
	var charid = Number(this.id.substring(7));
	if (isNaN(charid)) { errorAlert('showCharacterInfo','charid is not a number ['+charid+']'); return; }
	var info = CharInfos[charid];
	if (!info) { // fetch data and display later
		errorAlert('showCharacterInfo','no CharInfos data for charid: '+charid); 
		return;
	} else { // display the data
		showInfoWork(this,info,true);
	}
}

/* Function that expands all the titles for a given anime */
function expandTitles() {
	var node = this.parentNode;
	while (node.nodeName.toLowerCase() != 'td') node = node.parentNode;
	var altTitles = getElementsByClassName(node.getElementsByTagName('ul'), 'alt_titles', false)[0];
	if (!altTitles)  { errorAlert('expandTitles','could not find alternative titles span'); return; }
	if (this.className.indexOf('i_plus') >= 0) {
		this.className = this.className.replace('i_plus','i_minus');
		this.title = 'click to fold all titles matched';
		this.alt = this.firstChild.firstChild.nodeValue = '[-]';
		altTitles.style.display = '';
	} else {
		this.className = this.className.replace('i_minus','i_plus');
		this.title = 'click to expand all titles matched';
		this.alt = this.firstChild.firstChild.nodeValue = '[+]';
		altTitles.style.display = 'none';
	}
}

function setCharId() { 
	newguiseinput.value = this.value; 
	guiseinput.value = charInfo[this.value].title; 
	resulttable.style.display = "none";
}

/* Function that shows the results of the search */
function showResults() {
	var table = resulttable;
	if (!table) { errorAlert('showResults','no table'); return; }
	var caption = table.getElementsByTagName('caption')[0];
	if (caption) caption.firstChild.nodeValue = "Characters matching \""+searchString+"\"";
	var tbody = table.tBodies[0];
	// clear table
	while (tbody.childNodes.length) tbody.removeChild(tbody.firstChild);
	// var create tbody rows
	var ids = cids;
	var isChar = true;
	for (var i = 0; i < ids.length; i++) {
		var desc = charInfo[ids[i]];
		var row = document.createElement('tr');
		row.className = (i % 2 ? 'g_odd' : '');
		// radio button
		var ck = createButton("char",null,false,desc.charid,"radio",setCharId, null);
		createCell(row, 'check', ck);
		// image
		var img = document.createElement('img');
		img.src = desc.picurl;
		img.alt = "thumbnail";
		img.title = desc.name;
		createCell(row, 'thumbs', img);
		// matched name
		// first, let's create the icons i want
		var div = document.createElement('div');
		div.className = 'icons';
		var existTitles = (desc.titles['Official'] || desc.titles['Short'] || desc.titles['Maiden'] || desc.titles['other'] || desc.titles['Alias'] || desc.titles['Nick']);
		var altTitleSpan = null;
		if (existTitles) {
			altTitleSpan = document.createElement('ul');
			altTitleSpan.className = 'alt_titles';
			altTitleSpan.style.display = "none";
			var types = (!isChar ? ['Official','Synonym','Short'] : ['Official','Alias','Short','Nick','Maiden','other']);
			for (var tt = 0; tt < types.length; tt++) {
				var type = types[tt];
				if (!desc.titles[type]) continue;
				// Group titles
				var groups = new Array();
				for (var t = 0; t < desc.titles[type].length; t++) {
					var title = desc.titles[type][t]['title'];
					var lang = desc.titles[type][t]['lang'];
					var verified = desc.titles[type][t]['verified'];
					var si = title.toLowerCase().indexOf(searchString.toLowerCase());
					if (si < 0) continue; // title provides no match, so ignore it
					if (!groups[title]) groups[title] = new Array();
					if (!groups[title][Number(verified)]) groups[title][Number(verified)] = new Array();
					groups[title][Number(verified)].push(lang);
				}
				// display said groups of titles that provide matches
				for (var title in groups) {
					for (var v = 1; v >= 0; v--) {
						if (groups[title][v]) { // verified titles first
							var span = document.createElement('li'); // i know it's not a span!
							span.className = 'title '+type.toLowerCase();
							var b = document.createElement('b');
							var icons = document.createElement('div');
							icons.className = 'icons';
							for (var l = 0; l < groups[title][v].length; l++) {
								var lang = groups[title][v][l];
								icons.appendChild(createIcon(null, '['+lang+']', null, null, 'Language: '+languageMap[lang]['name'], 'i_audio_'+lang));
							}
							if (v) icons.appendChild(createIcon(null, '[V]', null, null, 'these title are considered to be correct for the corresponding language(s)', 'i_verified'));
							span.appendChild(icons);
							var si = title.toLowerCase().indexOf(searchString.toLowerCase());
							var firstBlock = document.createTextNode(title.substring(0,si));
							var middleBlock = document.createTextNode(title.substr(si,searchString.length));
							var lastBlock = document.createTextNode(title.substring(si+searchString.length,title.length));
							span.appendChild(firstBlock);
							b.appendChild(middleBlock);
							span.appendChild(b);
							span.appendChild(lastBlock);
							span.appendChild(document.createTextNode(' ('+type+')'));
							altTitleSpan.appendChild(span);
						}
					}
				}
			}
			if (altTitleSpan.childNodes && altTitleSpan.childNodes.length)
				div.appendChild(createIcon(null, '[+]', 'removeme', expandTitles, 'click to expand all titles matched', 'i_plus'));
		}
		var infoIcon = createIcon(null, 'character  info', null, null, null, 'i_mylist_ainfo');
		infoIcon.id = 'cinfo_c'+desc.charid;
		infoIcon.onmouseover = showCharacterInfo;
		infoIcon.onmouseout = hideTooltip;
		div.appendChild(infoIcon);
		var cell = createCell(null, 'titles', div);
		// now, create the titles and what not.
		// first, main title, always present, so default showing
		var mainTitleSpan = document.createElement('span');
		mainTitleSpan.className = 'title main';
		var b = document.createElement('b');
		var a = document.createElement('a');
		a.href = 'animedb.pl?show='+(!isChar ? 'anime&aid='+desc.aid : 'character&charid='+desc.charid);
		var si = desc.title.toLowerCase().indexOf(searchString.toLowerCase());
		if (si >= 0) {
			var firstBlock = document.createTextNode(desc.title.substring(0,si));
			var middleBlock = document.createTextNode(desc.title.substr(si,searchString.length));
			var lastBlock = document.createTextNode(desc.title.substring(si+searchString.length,desc.title.length));
			a.appendChild(firstBlock);
			b.appendChild(middleBlock);
			a.appendChild(b);
			a.appendChild(lastBlock);
			a.appendChild(document.createTextNode(' (Main)'));
		} else a.appendChild(document.createTextNode(desc.title+' (Main)'));
		mainTitleSpan.appendChild(a);
		cell.appendChild(mainTitleSpan);
		// now i cycle other tiles matched for this anime
		if (existTitles && altTitleSpan.childNodes && altTitleSpan.childNodes.length)
			cell.appendChild(altTitleSpan);
		row.appendChild(cell);
		tbody.appendChild(row);
	}
	table.style.display = "";
}


/* Function that handles search */
function doSearch() {
	if (guiseinput.value != "" && guiseinput.value == searchString) { resulttable.style.display = ""; return; }
	searchString = guiseinput.value;
	if (searchString.length < 3) { alert('Error:\nSearch string has an insufficient number of characters.'); return; }
	searchinput.value = "wait....";
	searchinput.onclick = null;
	fetchData("chardesc",searchString);
}

/* Function that prepares the page */
function prepPage() {
	var uriObj = parseURI(); // update the uriObj
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	var div = getElementsByClassName(document.getElementsByTagName('div'), 'g_definitionlist g_section character_form', false)[0];
	if (!div) { errorAlert('prepPage',"no div found, this might not be an error"); return; }
	var table = div.getElementsByTagName('table')[0];
	if (!table) { errorAlert('prepPage',"no table found"); return; }
	tbody = table.tBodies[0];
	var rows = tbody.rows;
	// create a pretty row array with indexes and all
	for (var i = 0; i < rows.length; i++) {
		var row = rows[i];
		var name = row.className;
		name = name.replace('g_odd ','');
		name = name.split(' ')[0];
		magicRows[name] = row; // magic rows will have rows indexed by the first item in the classname
	}
	// okay, now that i have done that i can do some magic
	var select = magicRows['type'].getElementsByTagName('select')[0];
	if (!select) { errorAlert('prepPage',"no select found"); return; }
	select.onchange = changeLayout;
	select.onchange(); // run the script to change the layout in case it's needed
	// okay, next bit, the guise thingie
	guiseinput = magicRows['parent'].getElementsByTagName('input')[0];
	if (!guiseinput) { errorAlert('prepPage',"no guise input"); return; }
	// create a copy of this input and hide it (this is an IE hack btw)
	newguiseinput = createTextInput(guiseinput.name,5,false,true,guiseinput.maxlength,guiseinput.value);
	guiseinput.parentNode.insertBefore(newguiseinput,guiseinput);
	guiseinput.name = "";
	guiseinput.value = "";
	guiseinput.size = guiseinput.size;
	// this will make sure that if the name thingie is cleared also will the actual place where the id goes
	guiseinput.onkeyup = function checkBlank() { if (this.value == "") newguiseinput.value = ""; };
	guiseinput.onchange = function checkBlank() { if (this.value == "") newguiseinput.value = ""; };
	if (newguiseinput.value != "" && !isNaN(Number(newguiseinput.value)))
		fetchData("charbyid",Number(newguiseinput.value));
	searchinput = createButton('do.search','do.search',false,"search","button",doSearch,null);
	guiseinput.parentNode.insertBefore(document.createTextNode(' '),guiseinput.nextSibling);
	guiseinput.parentNode.insertBefore(searchinput,guiseinput.nextSibling);
	guiseinput.parentNode.insertBefore(document.createTextNode(' '),searchinput);
	// create a basic table for results
	resulttable = document.createElement('table');
	resulttable.style.display = "none";
	var caption = document.createElement('caption');
	caption.appendChild(document.createTextNode("Characters"));
	resulttable.appendChild(caption);
	var thead = document.createElement('thead');
	var row = document.createElement('tr');
	createHeader(row, null, 'X');
	createHeader(row, null, 'Image');
	createHeader(row, null, 'Name'); // Matched Name
	thead.appendChild(row);
	resulttable.appendChild(thead);
	resulttable.appendChild(document.createElement('tbody'));
	guiseinput.parentNode.appendChild(resulttable);
	initTooltips();
}

addLoadEvent(prepPage);