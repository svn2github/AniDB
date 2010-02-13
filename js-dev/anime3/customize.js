/* file customization support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *
 * version 1.0 (03.02.2009) - Initial release
 * version 1.5 (18.02.2009) - Merged information tooltips
 */
jsVersionArray.push({
	"file":"anime3/customize.js",
	"version":"1.5",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Merged information tooltips"
});

/* Function that sets options values */
function changeOptionValue(node) {
	if (!node) node = this;
	var name = "";
	if (node.name && node.id && node.name == node.id) name = node.name;
	else if (node.name && !node.id) name = node.name;
	else if (!node.name && node.id) name = node.id;
	else return;
	var value = (node.type == 'checkbox' ? Number(node.checked) : node.value);
	CookieSet(name,value,3650);
}

// Quick reference Table

//get_info_sz = null;
//get_info_mw = null;

/* Function that sets options values */
function changeOptionValue(node) {
	if (!node) node = this;
	var name = "";
	if (node.name && node.id && node.name == node.id) name = node.name;
	else if (node.name && !node.id) name = node.name;
	else if (!node.name && node.id) name = node.id;
	else return;
	var value = (node.type == 'checkbox' ? Number(node.checked) : node.value);
	CookieSet(name,value,3650);
}

/* Function that adds file columns */
function addColToBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var defsAnimeLayoutSelect = document.getElementById('defsAnimeLayoutSelect');
	var option;
	if (this.nodeName.toLowerCase() == 'input') // we are dealing with the button
		option = defsAnimeLayoutSelect.options[defsAnimeLayoutSelect.selectedIndex];
	else // we are dealing with an option
		option = this;
	option.ondblclick = remColFromBox;
	var selIndex = userAnimeLayoutSelect.selectedIndex;
	if (selIndex < 0)
		userAnimeLayoutSelect.appendChild(option);
	else
		userAnimeLayoutSelect.insertBefore(option,userAnimeLayoutSelect.options[selIndex]);
}

/* Function that removes file columns */
function remColFromBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var defsAnimeLayoutSelect = document.getElementById('defsAnimeLayoutSelect');
	var option;
	if (this.nodeName.toLowerCase() == 'input') { // we are dealing with the button
		var selIndex = userAnimeLayoutSelect.selectedIndex;
		option = userAnimeLayoutSelect.options[selIndex];
	} else // we are dealing with an option
		option = this;
	option.ondblclick = addColToBox;
	defsAnimeLayoutSelect.appendChild(option);
}

/* Moves a Column in the user selected cols up */
function moveColUpBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var selIndex = userAnimeLayoutSelect.selectedIndex;
	if (selIndex < 1) return; // either no selection or selection is the top option
	var curOption = userAnimeLayoutSelect.options[selIndex];
	var prevOption = userAnimeLayoutSelect.options[selIndex-1];
	userAnimeLayoutSelect.insertBefore(curOption,prevOption);
}

/* Moves a Column in the user selected cols down */
function moveColDownBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var selIndex = userAnimeLayoutSelect.selectedIndex;
	if (selIndex < 0 || selIndex > userAnimeLayoutSelect.options.length-1) return; // either no selection or selection is the bottom option
	var curOption = userAnimeLayoutSelect.options[selIndex];
	var nextOption = userAnimeLayoutSelect.options[selIndex+2];
	userAnimeLayoutSelect.insertBefore(curOption,nextOption);

}

/* Resets both cols */
function resetColsBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var defsAnimeLayoutSelect = document.getElementById('defsAnimeLayoutSelect');
	// first clear them
	while (userAnimeLayoutSelect.childNodes.length) userAnimeLayoutSelect.removeChild(userAnimeLayoutSelect.firstChild);
	while (defsAnimeLayoutSelect.childNodes.length) defsAnimeLayoutSelect.removeChild(defsAnimeLayoutSelect.firstChild);
	var tempArray = new cloneArray(animePage_defLayout);
	// Current User Layout
	//for (var lc = 0; lc < animePage_curLayout.length; lc++) {
	//	var option = animePage_curLayout[lc];
	for (var lc = 0; lc < tempArray.length; lc++) {
		var option = tempArray[lc];
		var newOption = createSelectOption(null, animeFileColsList[option].text, option, false, null, false);
		newOption.ondblclick = remColFromBox;
		userAnimeLayoutSelect.appendChild(newOption);
		//tempArray.splice(tempArray.indexOf(option),1);
	}
/*
	// Layout Columns
	for (var lc = 0; lc < tempArray.length; lc++) {
		var option = tempArray[lc];
		var newOption = createSelectOption(null, animeFileColsList[option].text, option, false, null, false);
		newOption.ondblclick = addColToBox;
		defsAnimeLayoutSelect.appendChild(newOption);
	}
*/
}

/* Function that makes the FileTable Column layout preferences */
function makeLayoutPreferencesTable() {
	var table = document.createElement('table');
	var tbody = document.createElement('tbody');
	var row = document.createElement('tr');
	createHeader(row, null, "Available columns");
	createHeader(row);
	createHeader(row, null, "Selected columns");
	createHeader(row);
	tbody.appendChild(row);
	row = document.createElement('tr');
	// first build users personal layout table
	// reference: animeFileColsList, animePage_defLayout, animePage_curLayout
	var userAnimeLayoutSelect = createBasicSelect('userAnimeLayoutSelect','userAnimeLayoutSelect',null);
	var defsAnimeLayoutSelect = createBasicSelect('defsAnimeLayoutSelect','defsAnimeLayoutSelect',null);
	var tempArray = new cloneArray(animePage_defLayout);
	// Current User Layout
	for (var lc = 0; lc < animePage_curLayout.length; lc++) {
		var option = animePage_curLayout[lc];
		var newOption = createSelectOption(null, animeFileColsList[option].text, option, false, null, false);
		newOption.ondblclick = remColFromBox;
		userAnimeLayoutSelect.appendChild(newOption);
		tempArray.splice(tempArray.indexOf(option),1);
	}
	// Layout Columns
	for (var lc = 0; lc < tempArray.length; lc++) {
		var option = tempArray[lc];
		var newOption = createSelectOption(null, animeFileColsList[option].text, option, false, null, false);
		newOption.ondblclick = addColToBox;
		defsAnimeLayoutSelect.appendChild(newOption);
	}
	userAnimeLayoutSelect.size = defsAnimeLayoutSelect.size = 14;
	// Defaults Cell
	createCell(row, null, defsAnimeLayoutSelect);
	// Switch Boxes Cell
	var cell = document.createElement('td');
	var addCol = createButton('addCol','addCol',false,'>>','button');
	addCol.onclick = addColToBox;
	cell.appendChild(addCol);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	var remCol = createButton('remCol','remCol',false,'<<','button');
	remCol.onclick = remColFromBox;
	cell.appendChild(remCol);
	row.appendChild(cell);
	// User Selects Cell
	createCell(row, null, userAnimeLayoutSelect);
	// Up and Down Cell
	cell = document.createElement('td');
	var upCol = createButton('upCol','upCol',false,'\u2191','button');
	upCol.onclick = moveColUpBox;
	cell.appendChild(upCol);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	var resetCol = createButton('resetCol','resetCol',false,'x','button');
	resetCol.onclick = resetColsBox;
	cell.appendChild(resetCol);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	var downCol = createButton('downCol','downCol',false,'\u2193','button');
	downCol.onclick = moveColDownBox;
	cell.appendChild(downCol);
	row.appendChild(cell);
	tbody.appendChild(row);
	table.appendChild(tbody);
	return table;
}

/* Creates Profile options groups for the customize tab
 * @param parentNode If supplied will append resulting div to given node
 * @param headText The heading text
 * @param options A list with the options
 */
function createProfileGroup(parentNode, headText, options) {
	var div = document.createElement('div');
	var h4 = document.createElement('h3');
	if (headText) {
		if (typeof(headText) == 'string') h4.appendChild(document.createTextNode(headText));
		else h4.appendChild(headText);
		div.appendChild(h4);
	}
	var ul = document.createElement('ul');
	for (var e in options) {
		var option = options[e];
		var li = document.createElement('li');
		if (option['url']) {
			createLink(li, '?', option['url'], 'anidb::wiki', null, null, 'i_inline i_help');
			li.appendChild(document.createTextNode(' '));
		}
		var ckname = option['type']+'.'+option['var'];
		var ckchecked = false;
		if (config && config[option['type']] && config[option['type']][option['var']]) ckchecked = config[option['type']][option['var']];
		createLabledCheckBox(li,ckname,null,null,ckchecked, option['text'], null);
		ul.appendChild(li);
	}
	div.appendChild(ul);
	if (parentNode) parentNode.appendChild(div);
	else return div;
}

/* -[ENTITIES]----------------------
 * ENTITY RELATED FUNCTIONS
 * ---------------------------------
 */

/* Creates a new Info node */
function CInfo(node) {
	var MAX_CHAR_LENGTH = 750;
	if (!get_info_sz) get_info_sz = "150";
	var type = 'character';
	var idatt = 'charid';
	switch (node.nodeName.toLowerCase()) {
		case 'charcaterdescbyrel':
		case 'characterdesc':
		case 'characterdescbyid': type = 'character'; idatt = 'charid'; break;
		case 'creatordescbyrel':
		case 'creatordesc':
		case 'creatordescbyid': type = 'creator'; idatt = 'creatorid'; break;
		case 'animedesc':
		case 'animedescbyid': type = 'anime'; idatt = 'aid'; break;
	}
	this.type = type;
	this.id = Number(node.getAttribute(idatt));
	this.desc = convert_input(node.getAttribute('desc'));
	if (this.desc && this.desc.length > MAX_CHAR_LENGTH) {
		this.desc = this.desc.substr(0,MAX_CHAR_LENGTH);
		this.desc += ' [...]';
	}
	if (!this.desc || this.desc == '') this.desc = '<i>no description</i>';

	this.restricted = (type != 'anime' ? false : Number(node.getAttribute('restricted'))); // override
	this.year = null;
	this.airdate = null;
	this.enddate = null;
	var year = node.getAttribute('year');
	if (year) {
		this.year = year;
	} else {
		var airdate = node.getAttribute('airdate');
		var enddate = node.getAttribute('enddate');
		var airDateYear = endDateYear = '?';
		if (airdate && airdate != '0') {
			this.airdate = javascriptDate(airdate);
			airDateYear = this.airdate.getFullYear();
		}
		if (enddate && enddate != '0') {
			this.enddate = javascriptDate(enddate);
			endDateYear = this.enddate.getFullYear();
		}
		this.year = (airDateYear == endDateYear ? airDateYear : airDateYear + '-' +endDateYear);
	}
	this.picurl = node.getAttribute('picurl');
	this.thumbnail = null;
	if (this.picurl != null) {
		if (this.picurl != 'nopic.gif' && this.picurl != '') {
			this.thumbnail = picbase+'thumbs/50x65/'+this.picurl+'-thumb.jpg';
			this.picurl = picbase+'thumbs/'+get_info_sz+'/'+this.picurl+'-thumb.jpg';
		} else {
			this.thumbnail = 'http://static.anidb.net/pics/nopic_50x65.gif';
			this.picurl = 'http://static.anidb.net/pics/nopic_'+get_info_sz+'.gif';
		}
		HIDETHUMBNAILS = false;
	} else
		HIDETHUMBNAILS = true;

	this.title = node.getAttribute('title');
	this.mainlang = node.getAttribute('mainlang');

	this.titles = new Array();
	var dataNodes = node.getElementsByTagName('data');
	for (var i = 0; i < dataNodes.length; i++) {
		var child = dataNodes[i];
		var lang = child.getAttribute('lang')
		var name = child.getAttribute('name');
		var titletype = Number(child.getAttribute('type'));
		var verified = (child.getAttribute('verifydate') ? (Number(child.getAttribute('verifydate')) > 0) : false);
		if (titletype != 1 && lang && name)
			this.addTitle(titletype,name,lang,verified);
	}
}
CInfo.prototype.addTitle = function(type,title,lang,verified) {
	//alert('adding title: '+'\ntype: '+type+'\ntitle: '+title+'\nlang: '+lang+'\nverified: '+verified);
	var titletype = 'Main';
	if (this.type == 'anime') {
		switch(this.titletype) {
			case 1: titletype = "Main"; break;
			case 2: titletype = "Synonym"; break;
			case 3: titletype = "Short"; break;
			case 4: titletype = "Official"; break;
			default: titletype = "unknown"; break;
		}
	} else if (this.type == 'character' || this.type == 'creator') {
		switch(this.titletype) {
			case 1: titletype = "Main"; break;
			case 2: titletype = "Official"; break;
			case 3: titletype = "Alias"; break;
			case 4: titletype = "Maiden"; break;
			case 5: titletype = "Nick"; break;
			case 6: titletype = "Short"; break;
			case 7: titletype = "Other"; break;
			default: titletype = "unknown"; break;
		}
	}
	if (!this.titles[type]) this.titles[type] = new Array();
	this.titles[type].push({'type':titletype,'title':title,'lang':lang,'verified':verified});
}
CInfo.prototype.getTitles = function(type) {
	if (typeof(type) == 'string') {
		if (this.type == 'anime') {
			switch(type.toLowerCase()) {
				case "main": type = 1; break;
				case "synonym": type = 2; break;
				case "short": type = 3; break;
				case "official": type = 4; break;
				default: type = 0; break;
			}
		} else if (this.type == 'character' || this.type == 'creator') {
			switch(type.toLowerCase()) {
				case "main": type = 1; break;
				case "official": type = 2; break;
				case "alias": type = 3; break;
				case "maiden": type = 4; break;
				case "nick": type = 5; break;
				case "short": type = 6; break;
				case "other": type = 7; break;
				default: type = 0; break;
			}
		}
	}
	if (!this.titles[type]) return null;
	return this.titles[type];
}


/* Function that fetches char/creator data
 * @param type Type of data to fetch
 * @param search Search string
 */
function fetchInfoData(type,searchString) {
	var req = xhttpRequest();
	var allowedTypes = ['characterdescbyid','creatordescbyid','animedescbyid'];
	if (allowedTypes.indexOf(type) < 0) { errorAlert('fetchInfoData','Invalid search type: '+type); return; }
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/'+type.replace('descbyid','')+searchString+'_desc.xml', parseEntityData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type='+type+'&id='+searchString, parseEntityData);
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function parseEntityData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','no root node'); return; }
	var type = 'character';
	var cdescNodes = root.getElementsByTagName('characterdescbyid');
	if (!cdescNodes.length) { type = 'creator'; cdescNodes = root.getElementsByTagName('creatordescbyid'); }
	if (!cdescNodes.length) { type = 'anime'; cdescNodes = root.getElementsByTagName('animedescbyid'); }
	if (!cdescNodes.length) { type = 'character'; cdescNodes = root.getElementsByTagName('characterdescbyrel'); }
	for (var d = 0; d < cdescNodes.length; d++) {
		var infoNode = new CInfo(cdescNodes[d]);
		var acid = "";
		switch (type) {
			case 'character': charInfos[infoNode.id] = infoNode; acid = 'cinfo_ch' + infoNode.id; break;
			case 'creator': creatorInfos[infoNode.id] = infoNode; acid = 'cinfo_cr' + infoNode.id;  break;
			case 'anime': animeInfos[infoNode.id] = infoNode; acid = 'ainfo_' + infoNode.id;  break;
		}
		var targets = findAllInfoIcons(acid);
		for (var i = 0; i < targets.length; i++) {
			var a = targets[i];
			if (!a) continue;
			a.className = a.className.replace(/i_mylist_ainfo_loading|i_mylist_ainfo_greyed/gi,'i_mylist_ainfo');
		}
	}
}

