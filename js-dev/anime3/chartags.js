/* Char Tags
 * @author fahrenheit (alka.setzer@gmail.com)
           some code derived from work done by PetriW and Rar at anidb
 * version 1.0 (13.12.2008)
 */
jsVersionArray.push({
	"file":"anime3/chartags.js",
	"version":"1.0",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$"
});

var seeDebug = false;
var groups = new Array();
var tags = new Array();
var level0groups = new Array(); // top level tag groups
var textArea;
var insertButton;

/* TagGroupClass */
function CCTagGroup(node) {
	this.gid = Number(node.getAttribute('id'));
	this.name = node.getAttribute('name');
	this.pid = Number(node.getAttribute('pid'));
	this.childGroups = new Array();
	this.tags = new Array();
}

/* TagClass */
function CCTag(node) {
	this.id = Number(node.getAttribute('id'));
	this.gid = 0;
	try {
		this.gid = Number(node.getAttribute('gid'));
	} catch(e) {
		this.gid = 0;
	}
	this.name = node.getAttribute('name');
	this.verified = Number(node.getAttribute('verified'));
	this.cnt = Number(node.getAttribute('cnt'));
}

/* Function that fetches data */
function fetchData(charid) {
	var req = xhttpRequest();
	var extra = "";
	if (charid && !isNaN(charid) && charid > 0) extra = "&charid="+charid;
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/'+(extra != '' ? 'char'+charid+'_tags' : 'chartags')+'.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=chartags'+extra, parseData);
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','no root node'); return; }
	var groupNodes = root.getElementsByTagName('group');
	var tagNodes = root.getElementsByTagName('tag');
	// Add the general tag group
	groups[0] = {'gid':0,'name':'ungroupped','pid':0,'tags':new Array(),'childGroups':new Array()};
	level0groups.push(0);
	for (var d = 0; d < groupNodes.length; d++) {
		var taggroup = new CCTagGroup(groupNodes[d]);
		groups[taggroup.gid] = taggroup;
		if (!taggroup.pid) level0groups.push(taggroup.gid);
	}
	// now go over the tag groups again and set relations
	for (var g in groups) {
		var group = groups[g];
		if (!group) continue;
		if (group.pid) groups[group.pid].childGroups.push(group.gid);
	}
	// go over tags
	for (var d = 0; d < tagNodes.length; d++) {
		var tag = new CCTag(tagNodes[d]);
		groups[tag.gid].tags.push(tag.id);
		tags[tag.id] = tag;
	}
	writeTagGroups();
}

function writeTag() {
	var node = this;
	var tagid = Number(node.id.substring(1));
	var tag = tags[tagid]
	if (!tag) { errorAlert('writeTag','bad tag with id: '+tagid); return; }
	textArea.value += (textArea.value.length > 0 ? ';' : '')+ tag.name;
	node.parentNode.removeChild(node);
}

function writeGroupTags(group,isSelf) {
	if (!group) return null;
	var tagCols = new Array();
	var step = 9; // as in, number of items you want to show - 1;
	var start = 0;
	var end = (group.tags.length > step ? step : group.tags.length);
	while (start != group.tags.length) {
		var tagCol = document.createElement('div');
		tagCol.className = 'tag-column';
		if(isSelf) {
			var h5 = document.createElement('h5');
			h5.appendChild(document.createTextNode(group.name));
			tagCol.appendChild(h5);
		}
		for (var t = start; t < end; t++) {
			var tag = tags[group.tags[t]];
			if (!tag) continue;
			var span = createTextLink(null, tag.name, null, null, writeTag, null, 'link')
			span.id = 't'+tag.id;
			tagCol.appendChild(span);
		}
		start = end;
		end = (group.tags.length > start+step ? start+step : group.tags.length);
		tagCols.push(tagCol);
	}
	return tagCols;
}

/* Recursive hell */
function writeDeepTags(group,tagsDiv) {
	for (var g = 0; g < group.childGroups.length; g++) {
		var childGroup = groups[group.childGroups[g]];
		if (!childGroup) continue;
		if (childGroup.childGroups.length) {
			var tagCols = writeGroupTags(childGroup,true);
			for (var i = 0; i < tagCols.length; i++) 
				tagsDiv.appendChild(tagCols[i]);
			writeDeepTags(childGroup,tagsDiv);
		} else {
			var tagCols = writeGroupTags(childGroup,true);
			for (var i = 0; i < tagCols.length; i++) 
				tagsDiv.appendChild(tagCols[i]);
		}
	}
}

function showTagsForGroup() {
	var node = this;
	var gid = Number(node.id.substring(1));
	var group = groups[gid];
	if (!group) { errorAlert('showTagsForGroup','bad group with id: '+gid); return; }
	var tagHolder = document.getElementById('tagsholder');
	while (tagHolder.childNodes.length) tagHolder.removeChild(tagHolder.firstChild);
	var innerDiv = document.createElement('div');
	innerDiv.className = 'inner';
	var h3 = document.createElement('h3');
	h3.appendChild(document.createTextNode(group.name));
	innerDiv.appendChild(h3);
	var tagsDiv = document.createElement('div');
	tagsDiv.className = 'tag-list';
	if (group.childGroups.length) {
		var tagCols = writeGroupTags(group,true);
			for (var i = 0; i < tagCols.length; i++) 
				tagsDiv.appendChild(tagCols[i]);
		writeDeepTags(group,tagsDiv);
	} else {
		var tagCols = writeGroupTags(group,false);
			for (var i = 0; i < tagCols.length; i++) 
				tagsDiv.appendChild(tagCols[i]);
	}
	innerDiv.appendChild(tagsDiv);
	tagHolder.appendChild(innerDiv);
}

function writeTagGroups() {
	var div = document.createElement('div');
	div.className = "taggrouplist";
	for (var g = 0; g < level0groups.length; g++) {
		var group = groups[level0groups[g]];
		if (!group) continue;
		var span = createTextLink(null, group.name, null, null, showTagsForGroup, null, 'link');
		span.id = 'g'+group.gid;
		div.appendChild(span);
		if (g < level0groups.length - 1) div.appendChild(document.createTextNode(' | '));
	}
	textArea.parentNode.insertBefore(div,textArea.nextSibling);
	var tagdiv = document.createElement('div');
	tagdiv.className = 'tag-group';
	tagdiv.id = 'tagsholder';
	textArea.parentNode.appendChild(tagdiv);
}

/* Prepares the page for my scripts */
function prepPage() {
	var uriObj = parseURI();
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	var div = getElementsByClassName(document.getElementsByTagName('div'),'addbox',false)[0];
	if (!div) { errorAlert('prepPage','no matching div'); return; }
	fieldset = div.getElementsByTagName('fieldset')[0];
	var inputs = fieldset.getElementsByTagName('input');
	var charidInput = getElementsByName(inputs, 'ctag.charid', false)[0];
	var charid = (!charidInput ? null : charidInput.value);
	textArea = div.getElementsByTagName('textarea')[0];
	insertButton = getElementsByName(div.getElementsByTagName('input'), 'do.add', false)[0];
	fetchData(Number(charid));
}

// hook up the window onload event
addLoadEvent(prepPage);