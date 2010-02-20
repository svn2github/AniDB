/* file add anime as a relation support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (09.12.2008) - Initial Version
 * version 1.5 (??.??.2008) - Added support for both types of relations
 * version 1.6 (04.02.2009) - Revised version to use shared scripts and customize tab
 */
jsVersionArray.push({
	"file":"anime3/addcharanimerel.js",
	"version":"1.6",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Revised version to use shared scripts and customize tab"
});

var addRelInput; // For anime addition to a given char
var addCRelInput; // For character addition to a given anime
var addRelSubmit;
var fieldset;
var table;
var seeDebug = false;
var searchString = "";
var aids = new Array();
var cids = new Array();

/* Function that fetches data */
function fetchData(searchString,charSearch) {
	var req = xhttpRequest();
	if (!charSearch) {
		if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/animedescsearch.xml', parseData);
		else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=animedesc&limit=200&search='+escape(searchString), parseData);
	} else {
		if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/charactersearch.xml', parseData);
		else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=characterdesc&limit=200&search='+escape(searchString), parseData);
	}
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','no root node'); return; }
	var descNodes = root.getElementsByTagName('animedesc');
	if (!descNodes) descNodes = root.getElementsByTagName('characterdesc');
	if (!descNodes) {
		alert('Search for "'+searchString+'" resulted in no matches.');
		return;
	}
	aids = new Array();
	cids = new Array();
	for (var d = 0; d < descNodes.length; d++) {
		var descNode = new CInfo(descNodes[d]);
		var ids = (descNode.type == 'anime' ? aids : cids);
		var descs = (descNode.type == 'anime' ? animeInfos : charInfos);
		if (ids.indexOf(descNode.id) < 0) ids.push(descNode.id);
		if (!descs[descNode.id])
			descs[descNode.id] = descNode;
		else if (descNode.type != 1)
			descs[descNode.id].addTitle(descNode.titletype,descNode.name,descNode.lang,descNode.verified);
		else
			continue; // a bit pointless, i know :P
	}
	//alert("Results:\ndescNodes: "+descNodes.length+" ["+aids.length+"|"+cids.length+"]");
	showResults();
	handleTables(new Object(),['resultslist'],null,collapseThumbnails,false);
}

/* Function that starts a search */
function doSearch() {
	searchString = addRelInput.value;
	if (searchString.length < 3) { alert('Error:\nSearch string has an insufficient number of characters.'); return; }
	fetchData(searchString);
}

/* Function that starts a char search */
function doSearchChar() {
	searchString = addCRelInput.value;
	if (searchString.length < 3) { alert('Error:\nSearch string has an insufficient number of characters.'); return; }
	fetchData(searchString,true);
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

/* Function that shows the results of the search */
function showResults() {
	if (!table) { errorAlert('showResults','no table'); return; }
	table.id = "resultslist";
	var caption = table.getElementsByTagName('caption')[0];
	if (caption) caption.firstChild.nodeValue = (aids.length ? "Animes" : "Character")+" matching \""+searchString+"\"";
	var tbody = table.tBodies[0];
	// clear table
	while (tbody.childNodes.length) tbody.removeChild(tbody.firstChild);
	// Create table head
	var thead = document.createElement('thead');
	var row = document.createElement('tr');
	createHeader(row, 'check', 'X');
	if (!HIDETHUMBNAILS)
		createHeader(row, 'image', 'Image');
	createHeader(row, 'name', 'Name'); // Matched Name
	//createHeader(row, null, 'Name');
	thead.appendChild(row);
	table.insertBefore(thead,tbody);
	// var create tbody rows
	var ids = (aids.length ? aids : cids);
	var isChar = (cids.length > 0);
	for (var i = 0; i < ids.length; i++) {
		var desc = (!isChar ? animeInfos[ids[i]] : charInfos[ids[i]]);
		var row = document.createElement('tr');
		row.className = ((i+1) % 2 ? 'g_odd' : '');
		// checkbox
		var ck = createCheckbox((!isChar ? 'addcarel.aid[]' : 'addcarel.charid[]'),false);
		ck.value = (!isChar ? desc.aid : desc.charid);
		createCell(row, 'check', ck);
		if (!HIDETHUMBNAILS) {
			// image
			var img = document.createElement('img');
			img.src = desc.thumbnail;
			img.alt = "thumbnail";
			img.title = desc.name;
			createCell(row, 'image', img);
		}
		// matched name
		// first, let's create the icons i want
		var div = document.createElement('div');
		div.className = 'icons';
		var existTitles = (!isChar ?
							(desc.titles['Official'] || desc.titles['Short'] || desc.titles['Synonym'])
							:
							(desc.titles['Official'] || desc.titles['Short'] || desc.titles['Maiden'] || desc.titles['other'] || desc.titles['Alias'] || desc.titles['Nick'])
						);
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
		if (get_info & 16) {
			var infoIcon = createIcon(null, 'Click to show '+(!isChar ? 'anime' : 'character')+' information', 'removeme', null, null, 'i_mylist_ainfo');
			infoIcon.id = (!isChar ? 'ainfo_'+desc.id : 'cinfo_ch'+desc.id);
			infoIcon.onmouseover = showInfo;
			infoIcon.onmouseout = hideTooltip;
			div.appendChild(infoIcon);
		}
		var cell = createCell(null, 'name', div);
		// now, create the titles and what not.
		// first, main title, always present, so default showing
		var mainTitleSpan = document.createElement('span');
		mainTitleSpan.className = 'title main';
		var b = document.createElement('b');
		var a = document.createElement('a');
		a.href = 'animedb.pl?show='+(!isChar ? 'anime&aid='+desc.id : 'character&charid='+desc.id);
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
	// create tfoot
	var tfoot = document.createElement('tfoot');
	row = document.createElement('tr');
	row.className = 'action';
	createCell(row, 'value', createButton('add.doadd',null,false,'Add relation','submit'), null, 3);
	tfoot.appendChild(row);
	table.appendChild(tfoot);
}

/* Prepares the page for my scripts */
function prepPage() {
	var uriObj = parseURI();
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	var div = document.getElementById('addrelform');
	if (!div) { errorAlert('prepPage','no matching div'); return; }
	fieldset = div.getElementsByTagName('fieldset')[0];
	table = div.getElementsByTagName('table')[0];
	var inputs = div.getElementsByTagName('input');
	addRelInput = getElementsByName(inputs, 'addcarel.aname', false)[0];
	addCRelInput = getElementsByName(inputs, 'addcarel.charname', false)[0];
	addRelSubmit = getElementsByName(inputs, 'add.doadd', false)[0];
	if (!addRelSubmit || (!addRelInput && !addCRelInput)) { errorAlert('prepPage','no valid inputs'); return; }
	var newSubmitClone = createButton(addRelSubmit.name,addRelSubmit.id,false,addRelSubmit.value,'button',(addRelInput ? doSearch : doSearchChar), null);
	addRelSubmit.parentNode.replaceChild(newSubmitClone,addRelSubmit);
	addRelSubmit = newSubmitClone;
	initTooltips();
	// update curtable
	var tables = new Array();
	var reldivs = getElementsByClassName(document.getElementsByTagName('div'),'g_section reltable',true);
	for (var i = 0; i < reldivs.length; i++) {
		var curdiv = reldivs[i];
		var tb = curdiv.getElementsByTagName('table')[0];
		if (tb) {
			tb.id = 'relationslist_'+i;
			tables.push(tb.id);
		}
	}
	var sortingCols = {
		'relationslist': {	"name entity":{"type":'c_setlatin',"isDefault":true},
							"name anime":{"type":'c_setlatin'},
							"date added":{"type":'c_date'},
							"owner":{"type":'c_setlatin'},
							"rating":{"type":'c_setlatin'},
							"type":{"type":'c_latin'},
							"stats eps":{"type":'c_setlatin'},
							"stats seen":{"type":'c_setlatin'}}
	};
	// @TODO: when table cells have proper class names add sorting support
	handleTables(new Object(),tables,null,collapseThumbnails,(get_info & 16));
}

// hook up the window onload event
addLoadEvent(prepPage);
