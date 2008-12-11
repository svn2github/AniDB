/* file add anime as a relation support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (09.12.2008) - Initial Version
 */
jsVersionArray.push({
	"file":"anime3/addcharanimerel.js",
	"version":"1.0",
	"revision":"$Revision$",
	"date":"$Date$",
	"author":"$Author$",
	"changelog":"Initial Version"
});

var addRelInput;
var addRelSubmit;
var fieldset;
var table;
var seeDebug = true;
var searchString = "";
var AnimeInfos = new Array(); // indexed by aid
var descs = new Array();
var picbase = 'http://img5.anidb.net/pics/anime/';
var descInfo = new Array();
var aids = new Array();

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

/* Creates a new AnimeInfo node */
function CAnimeInfo(node) {
	this.aid = Number(node.getAttribute('id'));
	this.desc = convert_input(node.getAttribute('desc'));
	if (this.desc == '') this.desc = '<i>no description</i>'
	// @todo: parse bbstyles
	this.title = node.getAttribute('title');
	this.picurl = node.getAttribute('picurl');
	if (this.picurl != 'nopic.gif' && this.picurl != '') this.picurl = picbase+'thumbs/150/'+this.picurl+'-thumb.jpg';
	else this.picurl = 'http://static.anidb.net/pics/nopic_150.gif';
	this.restricted = Number(node.getAttribute('restricted'));
	this.airdate = javascriptDate(node.getAttribute('airdate'));
	this.enddate = javascriptDate(node.getAttribute('enddate'));
	var airDateYear = this.airdate.getFullYear();
	var endDateYear = this.enddate.getFullYear();
	this.year = (airDateYear == endDateYear ? airDateYear : airDateYear + '-' +endDateYear);
	this.lang = node.getAttribute('mainlang');
}

/* AnimeDesc Node Parser */
function CDesc(node) {
	this.aid = Number(node.getAttribute('aid'));
	this.picurl = node.getAttribute('picurl');
	if (this.picurl != 'nopic.gif' && this.picurl != '') this.picurl = picbase+'thumbs/50x65/'+this.picurl+'-thumb.jpg';
	else this.picurl = 'http://static.anidb.net/pics/nopic_50x65.gif';
	this.airdate = convertTime(node.getAttribute('airdate'));
	this.enddate = convertTime(node.getAttribute('enddate'));
	this.desc = convert_input(node.getAttribute('desc'));
	this.name = node.getAttribute('name');
	this.verified = (Number(node.getAttribute('verifydate')) > 0);
	this.lang = node.getAttribute('lang');
	this.title = node.getAttribute('title');
	this.mainlang = node.getAttribute('mainlang');
	this.restricted = Number(node.getAttribute('restricted'));
	this.type = Number(node.getAttribute('type'));
	var type = "main";
	switch(this.type) {
		case 1: type = "Main"; break;
		case 2: type = "Synonym"; break;
		case 3: type = "Short"; break;
		case 4: type = "Official"; break;
		default: type = "unknown"; break;
	}
	this.type = type;
	this.titles = new Array();
	if (this.type != 'Main') {
		this.titles[this.type] = new Array();
		this.titles[this.type].push({'title':this.name,'lang':this.lang,'verified':this.verified});
	}
	// now, build the anime info node
	if (!AnimeInfos[this.aid])
		AnimeInfos[this.aid] = new CAnimeInfo(node);
}

/* Function that fetches data */
function fetchData(searchString) {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/animesearch.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=animedesc&limit=100&search='+escape(searchString), parseData);
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','no root node'); return; }
	var descNodes = root.getElementsByTagName('animedesc');
	descs = new Array();
	aids = new Array();
	if (!descNodes.length) { // no matches found
		alert('Search for "'+searchString+'" resulted in no matches.');
		return;
	}
	for (var d = 0; d < descNodes.length; d++) {
		var descNode = new CDesc(descNodes[d]);
		descs.push(descNode);
		if (aids.indexOf(descNode.aid) < 0) aids.push(descNode.aid);
		if (!descInfo[descNode.aid]) descInfo[descNode.aid] = descNode;
		else if (descNode.type != 'Main') {
			if (!descInfo[descNode.aid].titles[descNode.type]) descInfo[descNode.aid].titles[descNode.type] = new Array();
			descInfo[descNode.aid].titles[descNode.type].push({'title':descNode.name,'lang':descNode.lang,'verified':descNode.verified});
		} else
			continue; // a bit pointless, i know :P
	}
	showResults();
}

/* Function that starts a search */
function doSearch() {
	searchString = addRelInput.value;
	if (searchString.length < 3) { alert('Error:\nSearch string has an insufficient number of characters.'); return; }
	fetchData(searchString);
}

/* Function that actualy shows the tooltip
 * @param obj The object that will be base for the tooltip position
 * @param info The AnimeInfo object
 */
function showAnimeInfoWork(obj,info) {
	// i don't have the profile value options so, just use defaults
	var minWidth = 450;
	var table = document.createElement('table');
	table.className = 'animeInfo';
	table.style.minWidth = minWidth + 'px';
	var thead = document.createElement('thead');
	var row = document.createElement('tr');
/*	
	var icons = document.createElement('div');
	icons.className = 'icons';
	icons.appendChild(createIcon(null, '['+info.lang+']', null, null, 'Language: '+languageMap[info.lang]['name'], 'i_audio_'+info.lang));
*/
	var title = document.createElement('span');
	title.className = 'title';
	title.appendChild(document.createTextNode(info.title));
	var year = document.createElement('span');
	year.className = 'year';
	year.appendChild(document.createTextNode('('+info.year+')'));
	var cell = createHeader(null, (info.restricted ? 'restricted' : null), title, null, null, 2);
	cell.appendChild(document.createTextNode(' '));
	cell.appendChild(year);
	//cell.appendChild(icons);
	row.appendChild(cell);
	thead.appendChild(row);
	table.appendChild(thead);
	var tbody = document.createElement('tbody');
	row = document.createElement('tr');
	var img = document.createElement('img');
	img.src = info.picurl;
	img.alt = '['+info.aid+']';
	createCell(row, 'animeInfoThumb', img);
	var span = document.createElement('span');
	span.innerHTML = info.desc;
	createCell(row, 'animeInfoDesc', span);
	tbody.appendChild(row);
	table.appendChild(tbody);
	setTooltip(table, true, minWidth);
}

/* Function that shows anime info (or not) */
function showAnimeInfo() {
	var aid = Number(this.id.substring(7));
	if (isNaN(aid)) { errorAlert('showAnimeInfo','aid is not a number ['+aid+']'); return; }
	var info = AnimeInfos[aid];
	if (!info) { // fetch data and display later
		errorAlert('showAnimeInfo','no AnimeInfos data for aid: '+aid); 
		return;
	} else { // display the data
		showAnimeInfoWork(this,info);
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

/* Function that shows the results of the search */
function showResults() {
	if (!table) { errorAlert('showResults','no table'); return; }
	var caption = table.getElementsByTagName('caption')[0];
	if (caption) caption.firstChild.nodeValue = "Animes matching \""+searchString+"\"";
	var tbody = table.tBodies[0];
	// clear table
	while (tbody.childNodes.length) tbody.removeChild(tbody.firstChild);
	// Create table head
	var thead = document.createElement('thead');
	var row = document.createElement('tr');
	createHeader(row, null, 'X');
	createHeader(row, null, 'Image');
	createHeader(row, null, 'Name'); // Matched Name
	//createHeader(row, null, 'Name');
	thead.appendChild(row);
	table.insertBefore(thead,tbody);
	// var create tbody rows
	for (var i = 0; i < aids.length; i++) {
		var desc = descInfo[aids[i]];
		var row = document.createElement('tr');
		row.className = (i % 2 ? 'g_odd' : '');
		// checkbox
		var ck = createCheckbox('addcarel.aid[]',false);
		ck.value = desc.aid;
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
		var existTitles = (desc.titles['Official'] || desc.titles['Short'] || desc.titles['Synonym']);
		if (existTitles) 
			div.appendChild(createIcon(null, '[+]', 'removeme', expandTitles, 'click to expand all titles matched', 'i_plus'));
		var infoIcon = createIcon(null, 'anime info', null, null, null, 'i_mylist_ainfo');
		infoIcon.id = 'ainfo_a'+desc.aid;
		hookEvent(infoIcon,'mouseover',showAnimeInfo);
		hookEvent(infoIcon,'mouseout',hideTooltip);
		div.appendChild(infoIcon);
		var cell = createCell(null, 'titles', div);
		// now, create the titles and what not.
		// first, main title, always present, so default showing
		var mainTitleSpan = document.createElement('span');
		mainTitleSpan.className = 'title main';
		var b = document.createElement('b');
		var a = document.createElement('a');
		a.href = 'animedb.pl?show=anime&aid='+desc.aid;
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
		if (existTitles) {
			var altTitleSpan = document.createElement('ul');
			altTitleSpan.className = 'alt_titles';
			altTitleSpan.style.display = "none";
			var types = ['Official','Synonym','Short'];
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
			cell.appendChild(altTitleSpan);
		}
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
	var div = document.getElementById('addrelform');
	if (!div) { errorAlert('prepPage','no matching div'); return; }
	fieldset = div.getElementsByTagName('fieldset')[0];
	table = div.getElementsByTagName('table')[0];
	var inputs = div.getElementsByTagName('input');
	addRelInput = getElementsByName(inputs, 'addcarel.aname', false)[0];
	addRelSubmit = getElementsByName(inputs, 'add.doadd', false)[0];
	if (!addRelInput || !addRelSubmit) { errorAlert('prepPage','no valid inputs'); return; }
	var newSubmitClone = createButton(addRelSubmit.name,addRelSubmit.id,false,addRelSubmit.value,'button',doSearch, null);
	addRelSubmit.parentNode.replaceChild(newSubmitClone,addRelSubmit);
	addRelSubmit = newSubmitClone;
	initTooltips();
	//addInfoToFooter(getJsFileVersionArrayInfo(jsVersionArray.length - 1).join(','), true);
}

// hook up the window onload event
addLoadEvent(prepPage);