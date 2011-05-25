/* Mass File scripts
 * @author fahrenheit (alka.setzer@gmail.com)
           some code derived from epoximator work at anidb
 * version 1.0 (xx.xx.2007) - Initial version
 * version 2.0 (23.05.2008) - Reworked version
 */
jsVersionArray.push({
	"file":"anime3/addfilem.js",
	"version":"2.0",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Reworked version"
});

// GLOBALS
var uriObj = new Array();		// Object that holds the URI
var aid;						// Anime Id
var form;						// The form container
var searchString = "";			// Group search string
var vidstrm, audstrm, substrm;
var vidTracks = 0;
var maxVidTracks = 1;
var audTracks = 0;
var maxAudTracks = 3;
var subTracks = 0;
var maxSubTracks = 3;
var templateType = 'manual';
var groupList = null;
var group = null;
var groupSearch = null;
var episodeList = null;
var audLangs = null;
var subLangs = null;
var crcStatus = null;
var qualSel = null;
var sourceSel = null;
var censorSel = null;
var vidSection = null;
var audSection = null;
var subSection = null;
var nextButton = null;
var my_dump = null;
var my_back = null;
var my_submit = null;

/* Function that fetches data */
function fetchData() {
	//alert(searchString);
	var req = xhttpRequest();
	if (isLocalHost()) xhttpRequestFetch(req, 'xml/groupsearch.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=groupsearch&search='+encodeURI(searchString), parseData);
}

/* Function that creates a Group object 
 * @param node The group node
 */
function CGroup(node) {
	this.id = Number(node.getAttribute('gid'));
	this.name = node.getAttribute('name');
	this.sname = node.getAttribute('sname');
}

/* Function that parses xml data
 * @param xmldoc The XML document that holds the data
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root')[0];
	var groupEntries = root.getElementsByTagName('group');
	var target = document.getElementById('addfilem.group.list');
	if (!target) { errorAlert('parseData','no group select present'); return; }
	var select = createBasicSelect('addf.group','addfilem.group.list');
	createSelectOption(select, 'ignore', '-1', true, null, false);
	createSelectOption(select, 'no or unknown group', '0', true, null, false);
	for (var i = 0; i < groupEntries.length; i++) {
		var groupEntry = new CGroup(groupEntries[i]);
		createSelectOption(select, '['+groupEntry.sname+'] '+groupEntry.name, groupEntry.id, false, null, false);
	}
	target.parentNode.replaceChild(select,target);
	groupSearch.disabled = false;
	groupSearch.value = 'Search';
}

/* Updates the search button to indicate search is ongoing */
function updateSearchString() { 
	this.value = 'searching...';
	searchString = group.value;
	this.disabled = true;
	fetchData(); 
}

/* [ ED2K PARSING ] */

/* Function that parses an ed2k link */
function MyFile(name, size, ed2k){
	this.find_crc = function(text){
		var regx = /[\[\(]([a-f0-9]{8})[\]\)]/i;
		if(regx.test(text)) return (RegExp.$1).toLowerCase();
		else return('');
	}
	this.find_version = function(text){
		var regx = /(\d)v(\d)/;
		if(regx.test(text)) return (RegExp.$2)*1
		else if(/v(\d)/.test(text)) return (RegExp.$1)*1
		else return(1);
	}
	this.find_filetype = function(text){
		var regx = /\.([a-z0-9]{2,4})$/i;
		if(regx.test(text)) return RegExp.$1.toLowerCase();
	}
	this.find_group = function(text){
		var regx = /(\[|\{)(.+?)(\]|\})(\[|\(|\.|_).+$/i;
		if(regx.test(text)) return (RegExp.$2)
		else return('');
	}
	this.find_epno = function(text){
		var regx = /\u7B2C(\d+)(\u8A71|\u5DFB)/;
		if(regx.test(text)) return (RegExp.$1)*1;
		else if (/EP[\s\_|\.]?([0-9]{0,4})/i.test(text) && RegExp.$1) return (RegExp.$1)*1;
		else if (/EP([0-9]{0,4})/i.test(text) && RegExp.$1) return (RegExp.$1)*1;
		else if (/\[([0-9]{0,4})\]/.test(text) && RegExp.$1) return (RegExp.$1)*1;
		else {
			regx = /(\-[\s\_|\.]?|[\s\_|\.])\[*([SCTPO]?\d+)\]*(v\d)?[\s\_|\.\-\(\[\.]/;
			if(regx.test(text)) return (RegExp.$2)*1 || RegExp.$2;
		}
	}
	name = decodeURI(name);
	this.name = name;
	this.ed2k = ed2k.toLowerCase();
	this.size = size;
	this.crc = this.find_crc(name);
	this.version = this.find_version(name);
	this.filetype = this.find_filetype(name);
	this.group = this.find_group(name);
	this.epno = this.find_epno(name);
}

// Function that submits data
function do_submit(){
	if( my_data.length<1 ){
		alert('Nothing to submit!');
		return false;
	}
	var my_result = document.getElementById('addfilem.result');

	my_fieldset = form.getElementsByTagName('fieldset')[0];
	if (my_fieldset) my_fieldset = document.createElement('fieldset');
	for (var i = 0, file = null; (file = my_data[i]); i++){
		my_fieldset.appendChild(createButton('addfilem.data.'+i,null,false,file.size+"."+file.ed2k+"."+file.filetype+"."+(file.crc||''),'hidden'));
	}
	my_fieldset.appendChild(createButton('addfilem.action','results_hidden',false,'1','hidden'));
	form.appendChild(my_fieldset);
	return true;
}

/* Function that parses the dump text */
function parseDump(text) {
	if( text.length<1 ){
		alert('No data in dump.');
		return;
	}
	nextButton.disabled = true;
	var re_ed2k_link = /ed2k\:\/\/\|file\|(.+)\|(\d+)\|([a-f0-9]{32})\|.*/i;	
	var lines = text.split("\n");
	var index = 0;
	var skipped = '';
	my_data = new Array();
	for (i in lines){
		if(re_ed2k_link.test(lines[i])){
			var file = new MyFile(RegExp.$1, RegExp.$2, RegExp.$3);
			my_data[index++] = file;
		}
		else if(/ed2k\:/.test(lines[i])) skipped += "\n"+lines[i];
	}
	if(my_data.length>26){
		alert('Too many links. Max 26 is allowed.');
		nextButton.disabled = false;
		return;
	}
	if( my_data.length<1 ){
		alert('No ed2k links found.');
		nextButton.disabled = false;
		return;
	}
	if( skipped.length>0 ) alert('Some lines were skipped (invalid): '+skipped);
	my_dump = document.getElementById('addfilem.dump');
	
	var div = document.createElement('div');
	div.className = 'g_section result';
	div.id = 'addfilem.result';
	var h4 = document.createElement('h4');
	h4.appendChild(createText('Result'));
	div.appendChild(h4);
	var table = document.createElement('table');
	table.id = table.className = 'filelist';
	var thead = document.createElement('thead');
	var tbody = document.createElement('tbody');
	var headNodes = [{'className':"id",'text':"#"},{'className':"name",'text':"Name"},{'className':"crc",'text':"CRC"},{'className':"ext",'text':"Ftyp",'abbr':"Filetype"},
					 {'className':"eid",'text':"Epno",'abbr':"Episode number"},{'className':"version",'text':"Version"},{'className':"released",'text':"Rel. Date",'abbr':"Release date"}];
	var row = document.createElement('tr');
	for (var i = 0; i < headNodes.length; i++) {
		var head = headNodes[i];
		createHeader(row, head['className'], head['text'], head['abbr']);
	}
	thead.appendChild(row);
	table.appendChild(thead);
	for (var i = 0, file = null; (file = my_data[i]); i++) {
		var row = document.createElement('tr');
		row.className = (i % 2) ? '' : 'g_odd';
		createCell(row, headNodes[0]['className'], createText(i));
		createCell(row, headNodes[1]['className'], createText(file.name));
		createCell(row, headNodes[2]['className'], createText(file.crc.toUpperCase()||'N/A'));
		createCell(row, headNodes[3]['className'], createText(file.filetype));
		var select = episodeList.cloneNode(true);
		select.name = 'addfilem.eid.'+i;
		for (var k = 0; k < select.options.length; k++)
			if (select.options[k].text == file.epno) { select.value = select.options[k].value; break; }
		createCell(row, headNodes[4]['className'], select);
		select = createBasicSelect('addfilem.version.'+i);
		for (var k = 1; k < 6; k++)
			createSelectOption(select, 'v'+k, k, false, null, false);
		select.value = file.version || '1';
		createCell(row, headNodes[5]['className'], select);
		var input = createTextInput('addfilem.released.'+i,10,false,false,10,'');
		createCell(row, headNodes[6]['className'], input);
		tbody.appendChild(row);
	}
	table.appendChild(tbody);
	div.appendChild(table);
	// create Back/Add Files buttons
	var p = document.createElement('p');
	my_back = createButton(null,'results_back',false,'Back','button',restore_dump, null);
	p.appendChild(my_back);
	p.appendChild(document.createTextNode(' '));
	my_submit = createButton('addf.mass','results_submit',false,'Add files','submit',do_submit);
	p.appendChild(my_submit);
	div.appendChild(p);
	form.replaceChild(div,my_dump);
}

/* function that restores the dump textarea */
function restore_dump() {
	if (!my_dump) return;
	var my_result = document.getElementById('addfilem.result');
	if (!my_result) return;
	my_result.parentNode.replaceChild(my_dump,my_result);
	nextButton.disabled = false;
}

/* [ INTERFACE ] */

/* Function that creates and appends a row to a given table
 * @param table The table node
 * @param field A field Node (usually a TH)
 * @param value A value node (usually a TD)
 */
function createRow(table, field, value, classname) {
	if (!field && !value) { errorAlert('createRow','no cells to add'); return; }
	var row = document.createElement('tr');
	if (field) row.appendChild(field);
	if (value) row.appendChild(value);
	if (table) table.appendChild(row);
	if (classname && classname != '') row.className = classname;
	return row;
}

function createBoldText(text) {
	var b = document.createElement('b');
	b.appendChild(document.createTextNode(text));
	return b;
}
function createText(text) { return document.createTextNode(text); }

function createLabelCheckbox(name,id,checked,title) {
	var ck = createCheckBox(null,name,id,null,checked);
	ck.title = title;
	return ck;
}

function vidStreams() {
	this.numStreams = getElementsByName(document.getElementsByTagName('input'),'addf.vid.streams',false)[0];
	this.streams = new Array();
}
vidStreams.prototype.add = function() {
	if (vidTracks >= maxVidTracks) return;
	var i = vidTracks;
	this.numStreams.value = vidTracks+1;
	this.streams[i] = new Object();
	var ck = createLabelCheckbox('addf.vid.none',null,false,'check to disable video track (check for subtitle files)');
	ck.onchange = function toggle() { vidstrm.disable(this.checked,i); }
	ck.style.display = 'none';
	this.streams[i].enabled = ck;

	var optionArray = {"10":{"text":'4:3'},"20":{"text":'16:9'},"30":{"text":'1.66:1'},"40":{"text":'1.85:1'},
						"50":{"text":'2.00:1'},"60":{"text":'2.21:1'},"70":{"text":'2.35:1'},
						"100":{"text":'other'},"200":{"text":'unknown'}};
	var select = createSelectArray(null,'addstrm.vid.ar','addstrm.vid.ar',null,"200",optionArray);
	select.title = 'Video track aspect ratio';
	select.style.display = 'none';
	this.streams[i].ar = select;
	this.streams[i].anamorphic = createLabelCheckbox('addstrm.vid.flag.FLAG_ANAMORPHIC',null,false,'anamorphic');
	this.streams[i].cleanvideo = createLabelCheckbox('addstrm.vid.flag.FLAG_CLEAN',null,false,'clean video');
	this.streams[i].vfr = createLabelCheckbox('addstrm.vid.flag.FLAG_VFR',null,false,'vfr');
	this.streams[i].wrongar = createLabelCheckbox('addstrm.vid.flag.FLAG_WRONGAR',null,false,'wrong aspect ratio');
	var cell = createCell(null,'value',this.streams[i].enabled);
	//cell.appendChild(createText(' '));
	cell.appendChild(this.streams[i].ar);
	//cell.appendChild(document.createElement('br'));
	//cell.appendChild(createText('Flags: '));
	//cell.appendChild(this.streams[i].anamorphic);
	//cell.appendChild(createText(' anamorphic '));
	cell.appendChild(this.streams[i].cleanvideo);
	cell.appendChild(createText(' clean video '));
	//cell.appendChild(this.streams[i].vfr);
	//cell.appendChild(createText(' vfr '));
	cell.appendChild(this.streams[i].wrongar);
	cell.appendChild(createText(' wrong ar'));
	var row = createRow(null,createCell(null,'field',createText('Video Track:')),cell);
	audSection.parentNode.insertBefore(row,audSection);
	vidTracks++;
}
vidStreams.prototype.disable = function(val,id) {
	if (!id) id = 0;
	for (var elem in this.streams[id]) {
		if (elem == 'enabled') continue;
		this.streams[id][elem].disabled = val;
	}
	this.streams[id]['enabled'].checked = val;
	if (vidTracks == 1 && val) {
		this.numStreams.value = 0;
	} else {
		this.numStreams.value = vidTracks;
	}
}
vidStreams.prototype.remove = function(id) {
	if (vidTracks-1 < 1) {
		this.disable(true,0);
		this.numStreams.value = 0;
		return;
	}
	if (!id) id = (vidTracks-1);
	var target = this.streams[id].enabled;
	var row = target.parentNode.parentNode;
	row.parentNode.removeChild(row);
	this.numStreams.value = vidTracks;
	this.streams[id] = null;
	vidTracks--;
}

function audStreams() {
	this.numStreams = getElementsByName(document.getElementsByTagName('input'),'addf.aud.streams',false)[0];
	this.streams = new Array();
}
audStreams.prototype.add = function() {
	if (audTracks >= maxAudTracks) return;
	// if we are adding a new stream all other streams must be enabled
	for (var i = 0; i < audTracks; i++)
		this.disable(false,i);
	var i = audTracks;
	this.numStreams.value = audTracks+1;
	var id = (i > 0) ? '_'+(i+1) : '';
	this.streams[i] = new Object();
	var ck = createLabelCheckbox('addf.aud'+id+'.none',null,false,'check to disable audio track');
	ck.onchange = function toggle() { audstrm.disable(this.checked,i); }
	ck.style.display = 'none';
	this.streams[i].enabled = ck;
	var optionArray = {"10":{'text':"normal audio"},"20":{'text':"commentary"},"30":{'text':"fandub"},
						"40":{'text':"alternative voiceover"},"100":{'text':"other"},"200":{'text':"unknown"}};
	var select = createSelectArray(null,'addstrm.aud'+id+'.type','addstrm.aud'+id+'.type',null,"10",optionArray);
	select.title = 'Audio track type';
	this.streams[i].type = select;
	var langsel = audLangs.cloneNode(true);
	langsel.name = 'addstrm.aud'+id+'.lang';
	langsel.id = 'addfilem.aud'+id+'.lang';
	langsel.value = 2;
	langsel.title = 'Audio track language';
	this.streams[i].lang = langsel;
	var cell = createCell(null,'value',this.streams[i].enabled);
	cell.appendChild(createText(' '));
	cell.appendChild(this.streams[i].type);
	cell.appendChild(createText(' '));
	cell.appendChild(this.streams[i].lang);
	var row = createRow(null,createCell(null,'field',createText('Audio Track '+(i+1)+':')),cell);
	subSection.parentNode.insertBefore(row,subSection);
	audTracks++;
}
audStreams.prototype.disable = function(val,id) {
	if (!id) id = 0;
	for (var elem in this.streams[id]) {
		if (elem == 'enabled') continue;
		this.streams[id][elem].disabled = val;
	}
	this.streams[id]['enabled'].checked = val;
	if (audTracks == 1 && val) {
		this.numStreams.value = 0;
	} else {
		this.numStreams.value = audTracks;
	}
}
audStreams.prototype.remove = function(id) {
	if (audTracks-1 < 1) {
		this.disable(true,0);
		this.numStreams.value = 0;
		return;
	}
	if (!id) id = (audTracks-1);
	var target = this.streams[id].enabled;
	var row = target.parentNode.parentNode;
	row.parentNode.removeChild(row);
	audTracks--;
	this.numStreams.value = audTracks;
	this.streams[id] = null;
}
function subStreams() {
	this.numStreams = getElementsByName(document.getElementsByTagName('input'),'addf.sub.streams',false)[0];
	this.streams = new Array();
}
subStreams.prototype.add = function() {
	if (subTracks >= maxSubTracks) return;
	for (var i = 0; i < subTracks; i++)
		this.disable(false,i);
	var i = subTracks;
	this.numStreams.value = subTracks+1;
	var id = (i > 0) ? '_'+(i+1) : '';
	this.streams[i] = new Object();
	var ck = createLabelCheckbox('addf.sub'+id+'.none',null,false,'check to disable subtitle track (no internal subtitles / RAW)');
	ck.onchange = function toggle() { substrm.disable(this.checked,i); }
	ck.style.display = 'none';
	this.streams[i].enabled = ck;
	var optionArray = {"10":{'text':"hard sub"},"20":{'text':"soft sub"},"30":{'text':"supplementary soft sub"},
						"100":{'text':"other"},"200":{'text':"unknown"}};
	var select = createSelectArray(null,'addstrm.sub'+id+'.type','addstrm.sub'+id+'.type',null,"200",optionArray);
	select.title = 'Subtitle track type';
	this.streams[i].type = select;
	var langsel = subLangs.cloneNode(true);
	langsel.name = 'addstrm.sub'+id+'.lang';
	langsel.id = 'addfilem.sub'+id+'.lang';
	langsel.value = 4;
	langsel.title = 'Subtitle track language';
	this.streams[i].lang = langsel;
	this.streams[i].dubsub = createLabelCheckbox('addstrm.sub'+id+'.flag.FLAG_DUBSUBBED',null,false,'dubsubbed');
	this.streams[i].hearing = createLabelCheckbox('addstrm.sub'+id+'.flag.FLAG_HEARINGIMP',null,false,'hearing impaired subs');
	this.streams[i].comment = createLabelCheckbox('addstrm.sub'+id+'.flag.FLAG_FORCOMMENTARY',null,false,'subs for commentary audio stream');
	var cell = createCell(null,'value',this.streams[i].enabled);
	cell.appendChild(createText(' '));
	cell.appendChild(this.streams[i].type);
	cell.appendChild(createText(' '));
	cell.appendChild(this.streams[i].lang);
	//cell.appendChild(createText('Flags: '));
	cell.appendChild(document.createElement('br'));
	cell.appendChild(this.streams[i].dubsub);
	cell.appendChild(createText(' dubsubbed '));
	cell.appendChild(this.streams[i].hearing);
	cell.appendChild(createText(' hearing imp. '));
	cell.appendChild(this.streams[i].comment);
	cell.appendChild(createText(' commentary audio'));
	var row = createRow(null,createCell(null,'field',createText('Subtitle Track '+(i+1)+':')),cell);
	subSection.parentNode.appendChild(row);
	subTracks++;
}
subStreams.prototype.disable = function(val,id) {
	if (!id) id = 0;
	for (var elem in this.streams[id]) {
		if (elem == 'enabled') continue;
		this.streams[id][elem].disabled = val;
	}
	this.streams[id]['enabled'].checked = val;
	if (subTracks == 1 && val) {
		this.numStreams.value = 0;
	} else {
		this.numStreams.value = subTracks;
	}
}
subStreams.prototype.remove = function(id) {
	if (subTracks-1 < 1) {
		this.disable(true,0);
		this.numStreams.value = 0;
		return;
	}
	if (!id) id = (subTracks-1);
	var target = this.streams[id].enabled;
	var row = target.parentNode.parentNode;
	row.parentNode.removeChild(row);
	this.numStreams.value = subTracks;
	this.streams[id] = null;
	subTracks--;
}

/* Function that does the template work */
function doTemplateWork() {
	qualSel.value = 3;
	sourceSel.value = 5;
	vidstrm.disable(false);
	audstrm.disable(false);
	substrm.disable(false);
	// reset track count to default values
	while(audTracks > 1) audstrm.remove();
	while(subTracks > 1) substrm.remove();
	templateType = this.value;
	switch(this.value) {
		case 'manual':
			break;
		case 'rawsub':
			substrm.disable(true,0);
			audstrm.streams[0].lang.value = 2;
			if (audstrm.streams[1]) audstrm.remove(1);
			if (substrm.streams[1]) substrm.remove(1);
			break;
		case 'fansub':
			audstrm.streams[0].lang.value = 2;
			if (audstrm.streams[1]) audstrm.remove(1);
			if (substrm.streams[1]) substrm.remove(1);
			break;
		case 'dual':
			qualSel.value = 2;
			sourceSel.value = 7;
			audstrm.streams[0].lang.value = 2;
			substrm.streams[0].lang.value = 4;
			substrm.streams[0].type.value = 20;
			if (!audstrm.streams[1]) { 
				audstrm.add();
				audstrm.streams[1].lang.value = 4;
				audstrm.streams[1].type.value = 10;
			} else
				audstrm.disable(false,1);
			if (substrm.streams[1]) substrm.remove(1);
			break;
		case 'dualsubs':
			qualSel.value = 2;
			sourceSel.value = 7;
			audstrm.streams[0].lang.value = 2;
			substrm.streams[0].lang.value = 2;
			substrm.streams[0].type.value = 20;
			if (!audstrm.streams[1]) { 
				audstrm.add();
				audstrm.streams[1].lang.value = 4;
				audstrm.streams[1].type.value = 10;
			} else
				audstrm.disable(false,1);
			if (!substrm.streams[1]) { 
				substrm.add();
				substrm.streams[1].lang.value = 2;
				substrm.streams[1].type.value = 20;
			} else
				substrm.disable(false,1);
			break;
		case 'dub':
			if (audstrm.streams[1]) audstrm.remove(1);
			if (substrm.streams[1]) substrm.remove(1);
			substrm.disable(true,0);
			break;
		case 'extdub':
			vidstrm.disable(true,0);
			substrm.disable(true,0);
			if (audstrm.streams[1]) audstrm.remove(1);
			if (substrm.streams[1]) substrm.remove(1);
			break;
		case 'extsub':
			vidstrm.disable(true,0);
			audstrm.disable(true,0);
			if (substrm.streams[1]) substrm.remove(1);
			if (audstrm.streams[1]) audstrm.remove(1);
			break;
		case 'other':
			vidstrm.disable(true,0);
			qualSel.value = 1;
			if (audstrm.streams[1]) audstrm.remove(1);
			sourceSel.value = 1;
			break;
		default: return;
	}
}

function doAudStreamsAdd() { audstrm.add(); }
function doAudStreamsRemove() { audstrm.remove(); }
function doSubStreamsAdd() { substrm.add(); }
function doSubStreamsRemove() { substrm.remove(); }

/* Function that builds the mass-file table */
function buildTable() {
	var table = document.createElement('table');
	table.className = 'g_section options';
	table.id = 'addfilem.options';
	var tbody = document.createElement('tbody');
	createRow(tbody,createCell(null,'field',createBoldText('General'),null,2));
	var cell = createCell(null,'value',groupList);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(group);
	cell.appendChild(document.createTextNode(' '));
	var newinput = createButton(groupSearch.name, groupSearch.id, groupSearch.disabled, groupSearch.value, groupSearch.type, updateSearchString, groupSearch.className);
	cell.appendChild(newinput);
	groupSearch = newinput;
	createRow(tbody,createCell(null,'field',createText('Group:')),cell,'important');
	var optionArray = {
		"manual":{"text":'manual input'},
		"rawsub":{"text":'raw (japanese audio, no subtitles)'},
		"fansub":{"text":'fansub (japanese audio, ? subtitles)'},
		"dual":{"text":'dual (japanese audio, ? audio, ? subtitles)'},
		"dub":{"text":'dub (audio ?, no subtitles)'},
		"extdub":{"text":'external dub file (? audio)'},
		"extsub":{"text":'external sub file (? subtitles)'},
		"other":{"text":'other (do NOT use this for dubs)'}
	};
	var select = createSelectArray(null,null,null,doTemplateWork,null,optionArray);
	createRow(tbody,createCell(null,'field',createText('Template:')),createCell(null,'value',select),'important');
	createRow(tbody,createCell(null,'field',createBoldText('File Settings'),null,2));
	cell = createCell(null,'value',crcStatus);
	cell.appendChild(createText(' (Needs CRC in filename)'));
	createRow(tbody,createCell(null,'field',createText('CRC Status:')),cell);
	if (censorSel.nodeName.toLowerCase() != 'input')
		createRow(tbody,createCell(null,'field',createText('Censored:')),createCell(null,'value',censorSel));
	else cell.appendChild(censorSel);
	cell = createCell(null,'value',qualSel);
	cell.appendChild(createText(' / '));
	cell.appendChild(sourceSel);
	createRow(tbody,createCell(null,'field',createText('Quality / Source:')),cell);
	vidSection = createRow(tbody,createCell(null,'field',createBoldText('Video Track Info'),null,2));
	cell = createCell(null,'field',createBoldText('Audio Track Info '),null,2);
	createTextLink(cell, '[+]', 'removeme', null, doAudStreamsAdd, 'Add audio track', null);
	cell.appendChild(document.createTextNode(' '));
	createTextLink(cell, '[-]', 'removeme', null, doAudStreamsRemove, 'Remove audio track', null);
	audSection = createRow(tbody,cell);
	cell = createCell(null,'field',createBoldText('Subtitle Track Info '),null,2);
	createTextLink(cell, '[+]', 'removeme', null, doSubStreamsAdd, 'Add subtitle track', null);
	cell.appendChild(document.createTextNode(' '));
	createTextLink(cell, '[-]', 'removeme', null, doSubStreamsRemove, 'Remove subtitle track', null);
	subSection = createRow(tbody,cell);
	vidstrm.add();
	audstrm.add();
	substrm.add();
	table.appendChild(tbody);
	return table;
}

/* Function that prepares the page for the script */
function prepPage() {
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	vidstrm = 		new vidStreams();
	audstrm = 		new audStreams();
	substrm = 		new subStreams();
	aid =			document.getElementById('addfilem.aid') ? Number(document.getElementById('addfilem.aid').value) : 0;
	form =			document.getElementById('addfilem.form');
	groupList =     document.getElementById('addfilem.group.list');
	group =         document.getElementById('addfilem.group');
	groupSearch =   document.getElementById('addfilem.search');
	episodeList =	document.getElementById('addfilem.episodes');
	audLangs =		document.getElementById('addfilem.aud.lang').cloneNode(true);
	subLangs =		document.getElementById('addfilem.sub.lang').cloneNode(true);
	qualSel =		document.getElementById('addfilem.quality').cloneNode(true);
	sourceSel =		document.getElementById('addfilem.type').cloneNode(true);
	var selects = 	form.getElementsByTagName('select');
	crcStatus =		getElementsByName(selects,'addf.crcstatus',false)[0].cloneNode(true);
	censorSel = 	getElementsByName(selects,'addf.censored',false)[0]
	if (censorSel)	censorSel = censorSel.cloneNode(true);
	else censorSel =getElementsByName(form.getElementsByTagName('input'),'addf.censored',false)[0].cloneNode(true);
	nextButton =	document.getElementById('addfilem.next');
	nextButton.onclick = function do_process_dump() { parseDump(document.getElementById('addfilem.ed2k').value); };
	var p =         document.createElement('p');
	nextButton.parentNode.insertBefore(p,nextButton);
	p.appendChild(nextButton);
	var dl =		document.getElementById('addfilem.options');
	dl.parentNode.replaceChild(buildTable(),dl);
}

// hook up the window onload event
addLoadEvent(prepPage);
