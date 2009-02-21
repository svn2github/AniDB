/* file common tags interfaces and functions
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (27.05.2008) - Initial Version
 * version 1.2 (18.02.2009) - Update to reflect current tag system changes
 */
jsVersionArray.push({
	"file":"anime3/tags.js",
	"version":"1.2",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Update to reflect current tag system changes"
});

var tags = new Array();
var lastSearch = "";
var searchString = "";
var inactiveSearch = true;
var override = false;
var inputBox = null;
var TagIsSpoiler = null;
var TagIsASpoiler = null;
var TagIsVerified = null;
var TagDescription = null;
var resultsDiv = null;

/* Function that fetches data */
function fetchData(searchString) {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/tagsearch.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=tag&search='+escape(searchString), parseData);
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','Could not get root node'); return; }
	var tagNodes = root.getElementsByTagName('tag');
	tags = new Array();
	for (var t = 0; t < tagNodes.length; t++)
		tags.push(new CTag(tagNodes[t]));
	showResults();
}

function showResults() {
	resultsDiv = document.getElementById('resultsDiv');
	if (resultsDiv) resultsDiv.parentNode.removeChild(resultsDiv);
	var div = document.createElement('ul');
	div.id = 'resultsDiv';
	div.className = 'acdropdown';
	div.style.display = "block";
	div.style.position = "absolute";
	div.style.width = inputBox.offsetWidth - 3 + "px";
	for (var i = 0; i < tags.length; i++) {
		var tag = tags[i];
		var b = document.createElement('b');
		var span = document.createElement('li');
		var si = tag.name.toLowerCase().indexOf(searchString.toLowerCase());
		if (si >= 0) {
			var firstBlock = document.createTextNode(tag.name.substring(0,si));
			var middleBlock = document.createTextNode(tag.name.substr(si,searchString.length));
			var lastBlock = document.createTextNode(tag.name.substring(si+searchString.length,tag.name.length));
			span.appendChild(firstBlock);
			b.appendChild(middleBlock);
			span.appendChild(b);
			span.appendChild(lastBlock);
		} else continue;
		span.id = 'tag_'+i;
		span.onclick = function onclick(event) {
			var id = Number(this.id.substr(4,this.id.length));
			var tag = tags[id];
			/*
			alert('tag.name: '+tag.name+
				'\ntag.is_spoiler: '+tag.is_spoiler+
				'\ntag.is_verified: '+tag.is_verified+
				'\ntag.desc: '+tag.desc+
				'\ntag.id: '+tag.id);
			*/
			inputBox.value = tag.name;
			TagIsSpoiler.checked = tag.is_spoiler;
			TagIsVerified.checked = tag.is_verified;
			TagDescription.value = tag.desc;
			resultsDiv.style.display = 'none';
		}
		span.ondblclick = function() { this.onclick(); };
		span.onmousedown = function() { this.onclick(); };
		div.appendChild(span);
	}
	if(tags.length >= 8) {
		height = div.firstChild.offsetHeight * 8;
		if (height > 0) div.style.height = height + "px";
	} else div.style.height = "auto";
	
	inputBox.parentNode.appendChild(div);
	resultsDiv = div;
}

function checkSearchString() {
	if (this.value.length < 2 && !override) {
		resultsDiv = document.getElementById('resultsDiv');
		if (resultsDiv) resultsDiv.parentNode.removeChild(resultsDiv);
		return;
	}
	var ll = lastSearch.length;
	var cl = this.value.length;
	var c1 = lastSearch.substr(0,Math.min(ll,cl));
	var c2 = this.value.substr(0,Math.min(ll,cl));
	var doSearch = false;
	if (c1.toLowerCase() == c2.toLowerCase() && ll && cl) doSearch = false;
	else doSearch = true;
	lastSearch = this.value;
	searchString = this.value;
	if (doSearch) fetchData(this.value);
	else showResults();
}

/* Tag Node Parser */
function CTag(node) {
	this.id = Number(node.getAttribute('id'));
	this.is_spoiler = Number(node.getAttribute('is_spoiler')) || 0;
	this.name = node.getAttribute('name');
	this.desc = node.getAttribute('description') || "";
	this.is_verified = Number(node.getAttribute('is_verified')) || 0;
}

/* Function that prepares the page */
function prepPage() {
	var divs = document.getElementsByTagName('div');
	var divAddtag = getElementsByClassName(divs, 'animetag_all', true)[0];
	if (divAddtag) {
		var inputs = divAddtag.getElementsByTagName('input');
		inputBox = getElementsByName(inputs, 'tag.name', true)[0];
		TagDescription = getElementsByName(divAddtag.getElementsByTagName('textarea'), 'tag.description', true)[0];
		TagIsSpoiler = getElementsByName(inputs, 'tag.is_spoiler', true)[0];
		TagIsASpoiler = getElementsByName(inputs, 'atag.is_spoiler', true)[0];
		TagIsVerified = getElementsByName(inputs, 'tag.is_verified', true)[0];
		inputBox.onkeyup = checkSearchString;
	}
	// more stuff for me to worry, sorting
	var divTagList = getElementsByClassName(divs, 'animetag_entries', true)[0];
	if (divTagList) {
		var tables = divTagList.getElementsByTagName('table');
		for (var t = 0; t < tables.length; t++) {
			var table = tables[t];
			var tbody = table.tBodies[0];
			var thead = document.createElement('thead');
			thead.appendChild(tbody.rows[0]);
			table.insertBefore(thead,tbody);
			// now add the sorting things
			var ths = thead.getElementsByTagName('th');
			ths[0].className += ' c_latin'; // name
			ths[1].className += ' c_latin'; // desc
			ths[2].className += ' c_number'; // approval
			ths[3].className += ' c_number'; // global cnt
			ths[4].className += ' c_setlatin'; // verified
			//ths[6].className += ' c_date'; // date
			init_sorting(thead,'name','down')
		}
	}
}

// hook up the window onload event
addLoadEvent(prepPage);
