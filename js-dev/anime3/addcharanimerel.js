/* file add anime as a relation support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (09.12.2008) - Initial Version
 */

var addRelInput;
var addRelSubmit;
var fieldset;
var table;
var seeDebug = true;
var searchString = "";
var AnimeInfos = new Array(); // indexed by aid
var descs = new Array();
var picbase = 'http://img5.anidb.net/pics/anime/';

/* Creates a new AnimeInfo node */
function CAnimeInfo(node) {
	this.aid = Number(node.getAttribute('id'));
	this.desc = node.getAttribute('desc');
	if (this.desc == '') this.desc = '<i>no description</i>'
	// @todo: parse bbstyles
	this.title = node.getAttribute('title');
	this.picurl = node.getAttribute('picurl');
	if (this.picurl != 'nopic.gif') this.picurl = picbase+'thumbs/150/'+this.picurl+'-thumb.jpg';
	else this.picurl = picbase+this.picurl;
	this.restricted = Number(node.getAttribute('restricted'));
	this.airdate = javascriptDate(node.getAttribute('airdate'));
	this.enddate = javascriptDate(node.getAttribute('enddate'));
	var airDateYear = this.airdate.getFullYear();
	var endDateYear = this.enddate.getFullYear();
	this.year = (airDateYear == endDateYear ? airDateYear : airDateYear + '-' +endDateYear);
}

/* AnimeDesc Node Parser */
function CDesc(node) {
	this.aid = Number(node.getAttribute('aid'));
	this.picurl = node.getAttribute('picurl');
	if (this.picurl != 'nopic.gif') this.picurl = picbase+'thumbs/50x65/'+this.picurl+'-thumb.jpg';
	else this.picurl = picbase+this.picurl;
	this.airdate = convertTime(node.getAttribute('airdate'));
	this.enddate = convertTime(node.getAttribute('enddate'));
	this.desc = node.getAttribute('desc');
	this.name = node.getAttribute('name');
	this.title = node.getAttribute('title');
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
	// now, build the anime info node
	if (!AnimeInfos[this.aid])
		AnimeInfos[this.aid] = new CAnimeInfo(node);
}

/* Function that fetches data */
function fetchData(searchString) {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/animesearch.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=animedescaid&search='+escape(searchString), parseData);
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','no root node'); return; }
	var descNodes = root.getElementsByTagName('animedesc');
	descs = new Array();
	for (var d = 0; d < descNodes.length; d++)
		descs.push(new CDesc(descNodes[d]));
	showResults();
}

/* Function that starts a search */
function doSearch() {
	searchString = addRelInput.value;
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
	var title = document.createElement('span');
	title.className = 'title';
	title.appendChild(document.createTextNode(info.title));
	var year = document.createElement('span');
	year.className = 'year';
	year.appendChild(document.createTextNode('('+info.year+')'));
	var cell = createHeader(null, (info.restricted ? 'restricted' : null), title, null, null, 2);
	cell.appendChild(document.createTextNode(' '));
	cell.appendChild(year);
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
	var aid = this._aid;
	if (isNaN(aid)) { errorAlert('showAnimeInfo','aid is not a number'); return; }
	var info = AnimeInfos[aid];
	if (!info) { // fetch data and display later
		errorAlert('showAnimeInfo','no AnimeInfos data for aid'+aid); 
		return;
	} else { // display the data
		showAnimeInfoWork(this,info);
	}
}

/* Function that shows the results of the search */
function showResults() {
	if (!table) { errorAlert('showResults','no table'); return; }
	var caption = table.getElementsByTagName('caption')[0];
	if (caption) caption.firstChild.nodeValue = "Characters matching \""+searchString+"\"";
	var tbody = table.tBodies[0];
	// clear table
	while (tbody.childNodes.length) tbody.removeChild(tbody.firstChild);
	// Create table head
	var thead = document.createElement('thead');
	var row = document.createElement('tr');
	createHeader(row, null, 'X');
	createHeader(row, null, 'Image');
	createHeader(row, null, 'Matched Name');
	createHeader(row, null, 'Name');
	thead.appendChild(row);
	table.insertBefore(thead,tbody);
	// var create tbody rows
	for (var i = 0; i < descs.length; i++) {
		var desc = descs[i];
		var row = document.createElement('tr');
		row.className = (i % 1 ? 'g_odd' : '');
		// checkbox
		var ck = createCheckbox('addcarel.aid[]',false);
		ck.value = desc.aid;
		createCell(row, null, ck);
		// image
		var img = document.createElement('img');
		img.src = desc.picurl;
		img.alt = "thumbnail";
		img.title = desc.name;
		createCell(row, null, img);
		// matched name
		var b = document.createElement('b');
		var span = document.createElement('span');
		var si = desc.name.toLowerCase().indexOf(searchString.toLowerCase());
		if (si >= 0) {
			var firstBlock = document.createTextNode(desc.name.substring(0,si));
			var middleBlock = document.createTextNode(desc.name.substr(si,searchString.length));
			var lastBlock = document.createTextNode(desc.name.substring(si+searchString.length,desc.name.length));
			span.appendChild(firstBlock);
			b.appendChild(middleBlock);
			span.appendChild(b);
			span.appendChild(lastBlock);
			span.appendChild(document.createTextNode(' ('+desc.type+')'));
		} else span.appendChild(document.createTextNode(desc.name+' ('+desc.type+')'));
		createCell(row, null, span);
		// Original name and anime link
		var div = document.createElement('div');
		div.className = 'icons';
		var infoIcon = createIcon(null, 'anime info', null, null, null, 'i_mylist_ainfo');
		infoIcon._aid = desc.aid;
		hookEvent(infoIcon,'mouseover',showAnimeInfo);
		hookEvent(infoIcon,'mouseout',hideTooltip);
		div.appendChild(infoIcon);
		var cell = createCell(null, null, div);
		cell.appendChild(createTextLink(null, desc.title, 'animedb.pl?show=anime&aid='+desc.aid));
		row.appendChild(cell);
		tbody.appendChild(row);
	}
	// create tfoot
	var tfoot = document.createElement('tfoot');
	row = document.createElement('tr');
	row.className = 'action';
	createCell(row, 'value', createButton('add.doadd',null,false,'Add relation','submit'), null, 4);
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
}

// hook up the window onload event
addLoadEvent(prepPage);