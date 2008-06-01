/* *
 * @file addproducern page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.0 (02.01.2008)
 */
 
// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var searchbox = null;
var inputbutton = null;
var searchString = "";
var globalSelect = null;
var typeSelect = null;
var newsearchbox = null;
var editMode = 'none';
var relidinput = null;
var reltbinput = null;
var titleinput = null;
var titleselect = null;
var entries = new Array();
var titleentries = new Array();
var companyTitles = new Array();
var personTitles = new Array();
var collaborationTitles = new Array();
var titles = new Array();
var titlesBody = null;
var titlesCell = null;
var targets = new Array('producern','mangaka','artist','seiyuu');
var target = targets[0];

function fetchData(id,type) {
	var req = xhttpRequest();
	if (!type) type = document.getElementById('addp.type').value;
	if (!type) { errorAlert('fetchData','wrong or inexistent type'); return; }
	if (''+window.location.hostname == '') {
		if (id) xhttpRequestFetch(req, 'xml/'+type+'search.xml', parseData);
		else xhttpRequestFetch(req, 'xml/producernsearch.xml', parseData);
	} else {
		if (id) xhttpRequestFetch(req, 'animedb.pl?show=xml&t='+type+'search&id='+id, parseData);
		else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=producernsearch&search='+escape(searchString), parseData);
	}
}

function CEntity(node) {
	this.id = 0;
	if (node.getAttribute('coid')) this.id = Number(node.getAttribute('coid'));
	if (node.getAttribute('pid')) this.id = Number(node.getAttribute('pid'));
	if (node.getAttribute('cid')) this.id = Number(node.getAttribute('cid'));
	this.name = node.getAttribute('name');
}

function CTitle(node) {
	this.id = Number(node.getAttribute('id'));
	this.lang = node.getAttribute('lang');
	this.type = node.getAttribute('type');
	this.name = nodeData(node);
}

function CCompany(node) {
	this.id = Number(node.getAttribute('coid'));
	this.name = "";
	this.titles = new Array();
	for (var i = 0; i < node.childNodes.length; i++) {
		var childNode = node.childNodes[i];
		if (childNode.nodeType == 3) continue;
		switch(childNode.nodeName) {
			case 'titles':
				var ctitles = childNode.getElementsByTagName('title');
				for (var k = 0; k < ctitles.length; k++) {
					var entry = new CTitle(ctitles[k]);
					companyTitles[entry.id] = entry;
					this.titles.push(entry.id);
					if (this.name == "" && entry.type == 'main') this.name = entry.name;
				}
				break;
			default:
				errorAlert('CCompany','unknown node with name: '+childNode.nodeName)
		}
	}
}

function CPerson(node) {
	this.id = Number(node.getAttribute('pid'));
	this.name = "";
	this.titles = new Array();
	for (var i = 0; i < node.childNodes.length; i++) {
		var childNode = node.childNodes[i];
		if (childNode.nodeType == 3) continue;
		switch(childNode.nodeName) {
			case 'titles':
				var ctitles = childNode.getElementsByTagName('title');
				for (var k = 0; k < ctitles.length; k++) {
					var entry = new CTitle(ctitles[k]);
					personTitles[entry.id] = entry;
					this.titles.push(entry.id);
					if (this.name == "" && entry.type == 'main') this.name = entry.name;
				}
				break;
			default:
				errorAlert('CPerson','unknown node with name: '+childNode.nodeName)
		}
	}
}

function CCollaboration(node) {
	this.id = Number(node.getAttribute('cid'));
	this.name = "";
	this.titles = new Array();
	for (var i = 0; i < node.childNodes.length; i++) {
		var childNode = node.childNodes[i];
		if (childNode.nodeType == 3) continue;
		switch(childNode.nodeName) {
			case 'titles':
				var ctitles = childNode.getElementsByTagName('title');
				for (var k = 0; k < ctitles.length; k++) {
					var entry = new CTitle(ctitles[k]);
					collaborationTitles[entry.id] = entry;
					this.titles.push(entry.id);
					if (this.name == "" && entry.type == 'main') this.name = entry.name;
				}
				break;
			default:
				errorAlert('CCollaboration','unknown node with name: '+childNode.nodeName)
		}
	}
}

function CProducer(data,type) {
	this.id = 0;
	this.data = data;
	this.type = type;
}

function parseData(xmldoc) {
  var root = xmldoc.getElementsByTagName('root').item(0);
  var t1 = new Date();
	entries = new Array();
  var companyEntries = root.getElementsByTagName('company');
	var personEntries = root.getElementsByTagName('person');
	var collaborationEntries = root.getElementsByTagName('collaboration');
	if (target == 'producern') {
		if (companyEntries.length) {
			for (var i = 0; i < companyEntries.length; i++) {
				var centry = new CCompany(companyEntries[i]);
				var entry = new CProducer(centry,'company');
				entry.id = (entries.length ? entries.length - 1: 0);
				entries.push(entry);
			}
		}
	}
	if (personEntries.length) {
		for (var i = 0; i < personEntries.length; i++) {
			var centry = new CPerson(personEntries[i]);
			var entry = new CProducer(centry,'person');
			entry.id = (entries.length ? entries.length - 1: 0);
			entries.push(entry);
		}
	}
	if (target != 'seiyuu') {
		if (collaborationEntries.length) {
			for (var i = 0; i < collaborationEntries.length; i++) {
				var centry = new CCollaboration(collaborationEntries[i]);
				var entry = new CProducer(centry,'collaboration');
				entry.id = (entries.length ? entries.length - 1: 0);
				entries.push(entry);
			}
		}
	}
	selectedValues = null;
	if (!globalSelect) selectedValues = titles;
	createSelectBoxes(entries,selectedValues);
}

function updateTypeBox() {
	var id = this.value;
	var type = entries[id].type;
	relidinput.value = entries[id].data.id;
	typeSelect.value = type;
	reltbselect.value = 1
	if (type == 'person') reltbselect.value = 2;
	if (type == 'collaboration') reltbselect.value = 3;
	titles = new Array();
	setTitleIDS();
	while (titlesCell.childNodes.length) titlesCell.removeChild(titlesCell.firstChild);
	titlesCell.appendChild(createTitlesTable(null,titles));
}

function setTitleIDS() {
	titleinput.value = "{"+titles.join(',')+"}";
}

function createSelectBoxes(Entries,selectedValues) {
	globalSelect = createBasicSelect('addp.selectbox');
	globalSelect.onchange = updateTypeBox;
	for (var i = 0; i < Entries.length; i++) {
		var entry = Entries[i];
		var value = entry.data.name;
		createSelectOption(globalSelect, value, i, false);
		if (i == 0) {
			typeSelect.value = entry.type;
			reltbselect.value = 1
			if (entry.type == 'person') reltbselect.value = 2;
			if (entry.type == 'collaboration') reltbselect.value = 3;
			relidinput.value = entry.data.id;
		}
	}
	replaceCell.replaceChild(globalSelect,((newsearchbox) ? newsearchbox : searchbox));
	if (!uriObj['edit']) {
		var newinputbutton = createSearchAgainButton(null,'addp.dosearch');
		replaceCell.replaceChild(newinputbutton,inputbutton);
		inputbutton = newinputbutton;
	}
	while (titlesCell.childNodes.length) titlesCell.removeChild(titlesCell.firstChild);
	titlesCell.appendChild(createTitlesTable(null,titles));
}

/* Add an id to the titles array */
function removeTitle() {
	var id = Number(this.className.replace('remove_title_',''));
	if (!id) return;
	var index = titles.indexOf(id); 
	if (index < 0) return; 
	titles.splice(index,1);
	// find row
	var row = this.parentNode;
	while (row.nodeName != 'TR') row = row.parentNode;
	row.parentNode.removeChild(row);
	setTitleIDS();
	var newtitleselect = createTitleSelect(titles);
	titleselect.parentNode.replaceChild(newtitleselect,titleselect);
	titleselect = newtitleselect;
}
/* Remove an id from the titles array */
function addTitle() { 
	var id = Number(titleselect.value);
	if (!id) return;
	var index = titles.indexOf(id);
	if (index >= 0) return; 
	titles.push(id);
	var title;
	if (typeSelect.value == 'company') title = companyTitles[id];
	if (typeSelect.value == 'person') title = personTitles[id];
	if (typeSelect.value == 'collaboration') title = collaborationTitles[id];
	createTitleRow(title);
	setTitleIDS();
	var newtitleselect = createTitleSelect(titles);
	titleselect.parentNode.replaceChild(newtitleselect,titleselect);
	titleselect = newtitleselect;
}

function createTitleSelect(selectedValues) {
	var id = (globalSelect) ? globalSelect.value : 0;
	var entry = entries[id].data;
	var titleIDS = entry.titles;
	var newtitleselect = createBasicSelect('addp.titles');
	// find out selected *producer* id
	for (var k = 0; k < titleIDS.length; k++) {
		var title;
		if (typeSelect.value == 'company') title = companyTitles[titleIDS[k]];
		if (typeSelect.value == 'person') title = personTitles[titleIDS[k]];
		if (typeSelect.value == 'collaboration') title = collaborationTitles[titleIDS[k]];
		var isSelected = false;
		if (selectedValues) isSelected = (selectedValues.indexOf(title.id) >= 0);
		if (isSelected) continue; // won't add selected titles
		createSelectOption(newtitleselect, title.name, title.id, isSelected);
	}
	return(newtitleselect);
}

function createTitleRow(title) {
	var row = document.createElement('TR');
	createCell(row, 'title', document.createTextNode(title.name), null);
	var remlink = createTextLink(null, 'remove', 'removeme', null, removeTitle, null, 'remove_title_'+title.id);
	remlink.removeAttribute('href');
	createCell(row, 'action', remlink, null);
	titlesBody.appendChild(row);
}

function createTitlesTable(parentNode,selectedValues) {
	var id = (globalSelect) ? globalSelect.value : 0;
	var entry = entries[id].data;
	var titleIDS = entry.titles;
	var table = document.createElement('TABLE');
	table.class = "titles";

	var thead = document.createElement('THEAD');
	var row = document.createElement('TR');
	createHeader(row, 'title', 'Title', null);
	createHeader(row, 'action', 'Action', null);
	thead.appendChild(row);
	table.appendChild(thead);

	titlesBody = document.createElement('TBODY');
	if (selectedValues) { // add the selected titles to the table
		for (var k = 0; k < titleIDS.length; k++) {
			var title;
			if (typeSelect.value == 'company') title = companyTitles[titleIDS[k]];
			if (typeSelect.value == 'person') title = personTitles[titleIDS[k]];
			if (typeSelect.value == 'collaboration') title = collaborationTitles[titleIDS[k]];
			if (!titles.length && title.type == 'main') {
				selectedValues.push(title.id);
				if (titles.indexOf(title.id) < 0) titles.push(title.id);
				setTitleIDS();
			}
			if (selectedValues.indexOf(title.id) < 0) continue;
			createTitleRow(title)
		}
	}
	table.appendChild(titlesBody);

	var tfoot = document.createElement('TFOOT');
	row = document.createElement('TR');
	titleselect = createTitleSelect(selectedValues);
	createCell(row,'title',titleselect,null);
	var addlink = createTextLink(null, 'add', 'removeme', null, addTitle, null,null);
	addlink.removeAttribute('href');
	createCell(row, 'action', addlink, null);
	tfoot.appendChild(row);
	table.appendChild(tfoot);

	if (parentNode) parentNode.appendChild(table);
	return table;
}

function createSearchButton(parentNode,name) {
	var button = createBasicButton(name,' Search ');
	button.onclick = function updateSearchString() { searchString = searchbox.value; if (!searchString.length) return; searchbox.value = 'please wait while searching...'; fetchData(); };
	if (parentNode && parentNode != '') parentNode.appendChild(button);
	else return button;
}

function createSearchAgainButton(parentNode,name) {
	var button = createBasicButton(name,' Search again ');
	button.onclick = function replaceBox() {	searchbox.value = "";
												newsearchbox = searchbox.cloneNode(true);
												replaceCell.replaceChild(newsearchbox,globalSelect); 
												inputbutton.value = " Search ";
												inputbutton.onclick = function updateSearchString() {	searchString = newsearchbox.value; newsearchbox.value = 'please wait while searching...'; fetchData(); }
												while (titlesCell.childNodes.length) titlesCell.removeChild(titlesCell.firstChild);
												titlesCell.appendChild(document.createTextNode('- search again -'));
											}
	if (parentNode && parentNode != '') parentNode.appendChild(button);
	else return button;
}


function cleanUp() {
	reltbselect.value = 1;
	if (typeSelect.value == 'person') reltbselect.value = 2;
	if (typeSelect.value == 'collaboration') reltbselect.value = 3;
	setTitleIDS();
}

function prepPage() {
  uriObj = parseURI(); // update the uriObj
  if (!uriObj['show']) return; // badPage
	var obj = uriObj['show'].substr(3,uriObj['show'].length);
	if (targets.indexOf(obj) < 0) return; // also bad page
	target = targets[targets.indexOf(obj)];
	// get the ids for the stuff we want
	var div = getElementsByClassName(document.getElementsByTagName('DIV'),target+'_add',true)[0];
	if (!div) errorAlert('prepPage','noDIV');
	var tbody = div.getElementsByTagName('TBODY')[0];
	if (!tbody) errorAlert('prepPage','noTBODY');
	var inputs = tbody.getElementsByTagName('INPUT');
	relidinput = getElementsByName(inputs,'addp.relid',false)[0];
	reltbselect = getElementsByName(tbody.getElementsByTagName('SELECT'),'addp.reltb',false)[0];
	titleinput = getElementsByName(inputs,'addp.titleids',false)[0];
	var doadd = getElementsByName(inputs,'add.doadd',false)[0];
	doadd.onclick = cleanUp;
	if (!relidinput || !reltbselect || !titleinput) { errorAlert('prepPage','noInputs'); return; }
	// Delete old rows
	var drow = relidinput.parentNode;
	while (drow.nodeName != 'TR') drow = drow.parentNode;
	relidinput = relidinput.cloneNode(true);
	drow.parentNode.removeChild(drow);
	drow = reltbselect.parentNode;
	while (drow.nodeName != 'TR') drow = drow.parentNode;
	reltbselect = reltbselect.cloneNode(true);
	drow.parentNode.removeChild(drow);
	drow = titleinput.parentNode;
	while (drow.nodeName != 'TR') drow = drow.parentNode;
	titleinput = titleinput.cloneNode(true);
	drow.parentNode.removeChild(drow);
	// move some stuff to the fieldset as hidden fields
	var fieldset = div.getElementsByTagName('FIELDSET')[0];
	if (!fieldset) errorAlert('prepPage','noFIELDSET');
	var tinput = createTextInput('addp.reltb',null,false,true,null);
	tinput.value = reltbselect.value;
	fieldset.appendChild(tinput);
	reltbselect = tinput;
	tinput = createTextInput('addp.titleids',null,false,true,null);
	tinput.value = titleinput.value;
	fieldset.appendChild(tinput);
	titleinput = tinput;
	tinput = createTextInput('addp.relid',null,false,true,null);
	tinput.value = relidinput.value;
	fieldset.appendChild(tinput);
	relidinput = tinput;
	// update the titles
	if (uriObj['edit'] && titleinput.value.indexOf('{') >= 0) { // we are doing an edit and we have titles
		titles = titleinput.value.replace('{','');
		titles = titles.replace('}','');
		titles = titles.split(',');
		// convert everything to numbers
		var aux = new Array();
		for (var k = 0; k < titles.length; k++) aux.push(Number(titles[k]));
		titles = aux;
	}
	// Add the new row
	var row = document.createElement('TR');
	var cellTitle = target.charAt(0).toUpperCase()+target.substr(1,target.length);
	createCell(row, 'field', document.createTextNode(cellTitle+':'), null);
	searchbox = createTextInput('addp.search','20',false,false,null);
	replaceCell = createCell(null, 'value', searchbox, null);
	replaceCell.appendChild(document.createTextNode(' '));
	replaceCell.appendChild(document.createTextNode(' of type '));
	var optionArray = {'none':{"text":' search first '},'company':{"text":' company '},'person':{"text":' person '},'collaboration':{"text":' collaboration '}};
	typeSelect = createSelectArray(null,"addp.type","addp.type",null,editMode,optionArray);
	typeSelect.disabled = true;
	if (uriObj['edit']) {
		typeSelect.value = 'company';
		if (reltbselect.value == 2) typeSelect.value = 'person';
		if (reltbselect.value == 3) typeSelect.value = 'collaboration';
		fetchData(Number(relidinput.value),typeSelect.value);
	}
	replaceCell.appendChild(typeSelect);
	replaceCell.appendChild(document.createTextNode(' '));
	if (!uriObj['edit']) {	//inputbutton = createSearchAgainButton(null,'addp.dosearch');
		inputbutton = createSearchButton(null,'addp.dosearch');
		replaceCell.appendChild(inputbutton);
	}
	row.appendChild(replaceCell);
	tbody.insertBefore(row,tbody.rows[0]);
	// add the select box for titles
	row = document.createElement('TR');
	createCell(row, 'field', document.createTextNode('Titles:'), null);
	titlesCell = createCell(null, 'value', document.createTextNode('- search first -'), null);
	row.appendChild(titlesCell);
	tbody.insertBefore(row,tbody.rows[1]);
}

//window.onload = prepPage;
addLoadEvent(prepPage);