/* file Admin enanchements interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (19.11.2008) - Initial version
 */
jsVersionArray.push({
	"file":"anime3/admin.js",
	"version":"1.0",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Initial version"
});

var adminScriptVar = new Object();

/* Tag Node Parser */
function CTag(node) {
	this.id = Number(node.getAttribute('id'));
	this.is_spoiler = Number(node.getAttribute('is_spoiler')) || 0;
	this.name = node.getAttribute('name');
	this.desc = node.getAttribute('description') || "";
	this.is_verified = Number(node.getAttribute('is_verified')) || 0;
}

/* Function that fetches data */
function fetchTagData() {
	searchString = adminScriptVar.tagSearch.value;
	var req = xhttpRequest();
	if (isLocalHost()) xhttpRequestFetch(req, 'xml/tagsearch.xml', parseTagData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=tag&search='+escape(searchString), parseTagData);
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function parseTagData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','Could not get root node'); return; }
	var tagNodes = root.getElementsByTagName('tag');
	tags = new Array();
	for (var t = 0; t < tagNodes.length; t++)
		tags.push(new CTag(tagNodes[t]));
	showTagResults();
}

function showTagResults() {
	// first clear the results div
	var dd = adminScriptVar.slaveResults.getElementsByTagName('dd')[0];
	while (dd.childNodes.length) 
		dd.removeChild(dd.firstChild);
	// now recreate the results div
	// DerIdiot liked selects, but i think a table with results fits this better
	var table = document.createElement('table');
	table.id = 'results';
	table.className = 'results';
	var thead = document.createElement('thead');
	var tr = document.createElement('tr');
	tr.className = 'header';
	createHeader(tr, 'sel', 'X');
	createHeader(tr, 'name', 'Name');
	createHeader(tr, 'desc', 'Desc?','Description');
	createHeader(tr, 'spoiler', 'Spoiler?');
	createHeader(tr, 'verified', 'Verified?');
	thead.appendChild(tr);
	table.appendChild(thead);
	var tbody = document.createElement('tbody');
	for (var i = 0; i < tags.length; i++) {
		var tag = tags[i];
		var row = document.createElement('tr');
		// select radio cell
		createCell(row, 'sel', createButton('tag.slaveid',null,false,tag.id,'radio'));
		// Name cell
		var nameCell = createCell(null, 'name');
		var b = document.createElement('b');
		var si = tag.name.toLowerCase().indexOf(searchString.toLowerCase());
		if (si >= 0) {
			var firstBlock = document.createTextNode(tag.name.substring(0,si));
			var middleBlock = document.createTextNode(tag.name.substr(si,searchString.length));
			var lastBlock = document.createTextNode(tag.name.substring(si+searchString.length,tag.name.length));
			nameCell.appendChild(firstBlock);
			b.appendChild(middleBlock);
			nameCell.appendChild(b);
			nameCell.appendChild(lastBlock);
		} else continue;
		row.appendChild(nameCell);
		// description
		createCell(row, 'desc', (tag.desc != "" ? createIcon(null, '[d] ',null, null, 'Description: '+clean_input(tag.desc), 'i_comment') : document.createTextNode(' ')));
		// spoiler
		createCell(row, 'spoiler', document.createTextNode(tag.is_spoiler?'yes':'no'));
		// verified
		createCell(row, 'verified', document.createTextNode(tag.is_verified?'yes':'no'));
		tbody.appendChild(row);
	}
	table.appendChild(tbody);
	dd.appendChild(table);
}

function prepPageModtag() {
	var dls = document.getElementsByTagName('dl');
	adminScriptVar.slaveSearch = getElementsByClassName(dls, 'slavesearch', false)[0];
	adminScriptVar.slaveResults = getElementsByClassName(dls, 'slaveresult', false)[0];
	var inputs = adminScriptVar.slaveSearch.getElementsByTagName('input');
	adminScriptVar.tagSearch = getElementsByName(inputs, 'tagsearch', false)[0];
	adminScriptVar.searchButton = getElementsByName(inputs, 'do.search', false)[0];
	// replace the search button
	var newSearch = createButton('do.search',null,false,'Search','button',fetchTagData, null);
	adminScriptVar.searchButton.parentNode.replaceChild(newSearch,adminScriptVar.searchButton);
	adminScriptVar.searchButton = newSearch;
}

function prepPage() {
	if (!document.getElementsByName('tag.merge').length) return; // nothing to do currently
	else prepPageModtag();
}

addLoadEvent(prepPage);