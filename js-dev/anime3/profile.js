/* file profile page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * version 1.0 (19.05.2007) - Initial release
 * version 1.1 (29.07.2008) - Added Javascript Tab
 */
 
// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var cssurl;
var current_css = "";
var style_index = 0;
var base_url = "http://static.anidb.net/";
var styleList = new Array();   // Stylelist array
var style_thumbnail, style_title, style_creator, style_status, style_updated, style_description;
var style_apply, style_next, style_prev, style_chooser;
var audlangs = new Object();
var sublangs = new Object();

/* CSS STYLE STUFF */
function fetchData() {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') { base_url = ""; xhttpRequestFetch(req, "css/stylelist.xml", parseData); }
	else { base_url = "http://static.anidb.net/"; xhttpRequestFetch(req, "../css/stylelist.xml", parseData); }
}

function CStyle(node) {
	this.name = node.getAttribute('name');
	this.title = this.name; 
	this.screenshot = "";
	this.thumbnail = "";
	this.path = "";
	this.creator = "";
	this.update = "";
	this.status = "";
	this.description = "";
	for (var i = 0; i < node.childNodes.length; i++) {
		var sNode = node.childNodes.item(i);
		if (sNode.nodeType == 3) continue; // Text node, not interested
		switch (sNode.nodeName) {
			case 'title': this.title = nodeData(sNode); break;
			case 'screenshot': this.screenshot = nodeData(sNode); break;
			case 'thumbnail': this.thumbnail = nodeData(sNode); break;
			case 'path': this.path = nodeData(sNode); break;
			case 'creator': this.creator = nodeData(sNode); break;
			case 'update': this.update = nodeData(sNode); break;
			case 'status': this.status = nodeData(sNode); break;
			case 'description': this.description = nodeData(sNode); break;
			default: showAlert('Style entry for: '+this.name, node.nodeName, node.nodeName,sNode.nodeName);
		}
	}
}

function clearImg() {
	this.onerror = null;
	this.src = base_url+"css/nopic.png";
	return;
}

function updateCurrentStyle() {
	var curStyle = styleList[style_index];
	style_chooser.value = style_index;
	// Thumbnail
	if (!style_thumbnail) style_thumbnail = document.getElementById('style_thumbnail');
	style_thumbnail.title = "Style: "+curStyle.name;
	style_thumbnail.alt = curStyle.name;
	//style_thumbnail.onerror = clearImg;
	if (curStyle.thumbnail != "none") {
		if (curStyle.thumbnail == "") style_thumbnail.src = base_url+"css/"+curStyle.name+"/images/thumbnail.png";
		else style_thumbnail.src = base_url+"css/"+curStyle.thumbnail;
	} else style_thumbnail.src = base_url+"css/nopic.png";
	var i_enlarge = document.getElementById('style_enlarge');
	if (curStyle.screenshot != "none") {
		if (!i_enlarge) {
			i_enlarge = document.createElement('A');
			i_enlarge.className = "i_icon i_enlarge";
			i_enlarge.title = "Show screenshot";
			i_enlarge.alt = "show screenshot";
			i_enlarge.target = "blank";
			i_enlarge.id = 'style_enlarge';
			style_thumbnail.parentNode.appendChild(i_enlarge);
		}
		if (curStyle.screenshot == "") i_enlarge.href = base_url+"css/"+curStyle.name+"/images/screenshot.png";
		else i_enlarge.href = base_url+"css/"+curStyle.screenshot;    
	}
	// Title
	if (!style_title) style_title = document.getElementById('style_title');
	while (style_title.childNodes.length) { style_title.removeChild(style_title.childNodes[0]); }
	if (curStyle.title != "") style_title.appendChild(document.createTextNode(curStyle.title));
	else style_title.appendChild(document.createTextNode(curStyle.name));
	// Creator
	if (!style_creator) style_creator = document.getElementById('style_creator');
	while (style_creator.childNodes.length) { style_creator.removeChild(style_creator.childNodes[0]); }
	if (curStyle.creator != "") style_creator.appendChild(document.createTextNode(curStyle.creator));
	else style_creator.appendChild(document.createTextNode('N/A'));
	// Status
	if (!style_status) style_status = document.getElementById('style_status');
	while (style_status.childNodes.length) style_status.removeChild(style_status.childNodes[0]);
	if (curStyle.status != "") style_status.appendChild(document.createTextNode(curStyle.status));
	else style_status.appendChild(document.createTextNode('N/A'));
	// Update
	if (!style_updated) style_updated = document.getElementById('style_updated');
	while (style_updated.childNodes.length) style_updated.removeChild(style_updated.childNodes[0]);
	if (curStyle.update != "") style_updated.appendChild(document.createTextNode(curStyle.update));
	else style_updated.appendChild(document.createTextNode('N/A'));
	// Description
	if (!style_description) style_description = document.getElementById('style_description');
	while (style_description.childNodes.length) style_description.removeChild(style_description.childNodes[0]);
	if (curStyle.description != "") style_description.appendChild(document.createTextNode(curStyle.description));
	else style_description.appendChild(document.createTextNode('N/A'));
}

function changeStyle() {
	// if (style_index == 0) cssurl.style.display = "";
	// else cssurl.style.display = "none";
	var curStyle = styleList[style_index];
	if (curStyle.path != "none") {
		if (curStyle.path == "") cssurl.value = base_url+"css/"+curStyle.name+"/"+curStyle.name+".css";
		else cssurl.value = curStyle.path;
	} else cssurl.value = "";
	updateCurrentStyle();
}

function stylePrev() {
	if (style_index > 0) style_index--;
	if (style_index == 0) style_prev.disabled = true;
	if (style_index < styleList.length-1) style_next.disabled = false;
	changeStyle();
}

function styleNext() {
	if (style_index < styleList.length-1) style_index++;
	if (style_index == styleList.length-1) style_next.disabled = true;
	if (style_index > 0) style_prev.disabled = false;
	changeStyle();
}

function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('css_styles').item(0);
	var t1 = new Date();
	var styleEntries = (root) ? root.getElementsByTagName('style') : new Array();;
	// the first entry is the custom node
	var optionArray = new Object();
	var scustom = new Object();
	scustom.name = "custom";
	scustom.title = "Custom/Default";
	scustom.path = "none";
	scustom.thumbnail = "none";
	scustom.screenshot = "none";
	scustom.creator = "N/A"
	scustom.update = "N/A";
	scustom.status = "N/A";
	scustom.description = "Provide a url to a custom style sheet or leave it blank if you wish to use the default style.";
	optionArray[styleList.length] = {'text':"custom" };
	styleList.push(scustom);
	for (var i = 0; i < styleEntries.length; i++) {
		var styleEntry = new CStyle(styleEntries[i]);
		if (styleEntry.path != "none") {
			if (styleEntry.path == "") {
				if (styleEntry.name.toLowerCase() == current_css.toLowerCase()) style_index = i+1;
			} else {
				if (styleEntry.name.toLowerCase().indexOf(current_css.toLowerCase()) >= 0) style_index = i+1;
			}
		} // we just got the style the user is using (at least i hope we got it)
		optionArray[styleList.length] = {'text':styleEntry.name.toLowerCase()};
		styleList.push(styleEntry);
	}
	// FUCK IE //
	if (!style_prev) style_prev = document.getElementById('style_prev');
	if (!style_next) style_next = document.getElementById('style_next');
	if (style_index > 0) style_prev.disabled = false;
	if (style_index < styleList.length-1) style_next.disabled = false;
	style_next.onclick = styleNext;
	style_prev.onclick = stylePrev;
	style_chooser = createSelectArray(null,null,"profile.style_chooser",selectStyle,0,optionArray);
	style_next.parentNode.insertBefore(style_chooser,style_next);
	style_next.parentNode.insertBefore(document.createTextNode(' '),style_next);
	updateCurrentStyle();
}

function selectStyle() {
	style_index = this.value;
	changeStyle();
}

/* LANGUAGE STUFF */

/* Adds a language
 * @param value Language to add
 */
function addLanguageToArray(value) { 
	if (this.array.indexOf(value) < 0) this.array.push(value); 
}
/* Removes a language
 * @param value Language to remove
 */
function remLanguageFromArray(value) { 
	if (this.array.indexOf(value) >= 0)
		this.array.splice(this.array.indexOf(value),1);
}

/* Converts a language array to text */
function convLanguagesToText() {
	this.text = '{'+this.array.join(',')+'}';
	this.input.value = this.text;
}

/* Function that does some changes to the way anidb handles languages */
function prepLanguages() {
	var form = audlangs.input.form;
	// remove the boxes things
	var audioDiv = getElementsByClassName(form.getElementsByTagName('div'), 'audio', false)[0];
	var subtitlesDiv = getElementsByClassName(form.getElementsByTagName('div'), 'subtitles', false)[0];
	if (audioDiv) {
		var baseSelect = audioDiv.getElementsByTagName('select')[0];
		audlangs.select = new Array();
		audlangs.select[0] = createBasicSelect('audioLangSelect0','audioLangSelect0',null);
		audlangs.select[1] = createBasicSelect('audioLangSelect1','audioLangSelect1',null);
		if (baseSelect) {
			for (var i = 0; i < baseSelect.options.length; i++) {
				var option = baseSelect.options[i];
				if (audlangs.array.indexOf(option.value) >= 0) {
					var newOption = createSelectOption(null, option.text, option.value, false, null, false);
					newOption.ondblclick = remLangFromBox; 
					audlangs.select[1].appendChild(newOption);
				} else {
					var newOption = createSelectOption(null, option.text, option.value, false, null, false);
					newOption.ondblclick = addLangToBox;
					audlangs.select[0].appendChild(newOption);
				}
			}
		}
		audlangs.select[0].size = audlangs.select[1].size = 8;
		audioDiv.parentNode.removeChild(audioDiv);
	}
	if (subtitlesDiv) {
		var baseSelect = subtitlesDiv.getElementsByTagName('select')[0];
		sublangs.select = new Array();
		sublangs.select[0] = createBasicSelect('subtitleLangSelect0','subtitleLangSelect0',null);
		sublangs.select[1] = createBasicSelect('subtitleLangSelect1','subtitleLangSelect1',null);
		if (baseSelect) {
			for (var i = 0; i < baseSelect.options.length; i++) {
				var option = baseSelect.options[i];
				if (sublangs.array.indexOf(option.value) >= 0) {
					var newOption = createSelectOption(null, option.text, option.value, false, null, false);
					newOption.ondblclick = remLangFromBox; 
					sublangs.select[1].appendChild(newOption);
				} else {
					var newOption = createSelectOption(null, option.text, option.value, false, null, false);
					newOption.ondblclick = addLangToBox;
					sublangs.select[0].appendChild(newOption);
				}
			}
		}
		sublangs.select[0].size = sublangs.select[1].size = 8;
		subtitlesDiv.parentNode.removeChild(subtitlesDiv);
	}
	var langsDiv = document.createElement('div');
	var h4 = document.createElement('h4')
	h4.appendChild(document.createTextNode('Languages'));
	langsDiv.appendChild(h4);
	var table = document.createElement('table');
	var tbody = document.createElement('tbody');
	var row = document.createElement('tr');
	var optionArray = { 0 : {"text":'Audio Languages'} , 1 : {"text":'Subtitle Languages'}};
	createCell(row, null, createSelectArray(null,'languagesTypeSelect','languagesTypeSelect',changeLangType,0,optionArray), null, 3);
	tbody.appendChild(row);
	row = document.createElement('tr');
	audlangs.row = row;
	createCell(row, null, audlangs.select[0]);
	var cell = document.createElement('td');
	var addLang = createButton('addAudLang','addAudLang',false,'>>','button');
	addLang.onclick = addLangToBox;
	cell.appendChild(addLang);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	var remLang = createButton('remAudLang','remAudLang',false,'<<','button');
	remLang.onclick = remLangFromBox;
	cell.appendChild(remLang);
	row.appendChild(cell);
	createCell(row, null, audlangs.select[1]);
	tbody.appendChild(row);
	row = document.createElement('tr');
	row.style.display = 'none';
	sublangs.row = row;
	createCell(row, null, sublangs.select[0], null, 1);
	cell = document.createElement('td');
	addLang = createButton('addSubLang','addSubLang',false,'>>','button');
	addLang.onclick = addLangToBox;
	cell.appendChild(addLang);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	remLang = createButton('remSubLang','remSubLang',false,'<<','button');
	remLang.onclick = remLangFromBox;
	cell.appendChild(remLang);
	row.appendChild(cell);
	createCell(row, null, sublangs.select[1]);
	tbody.appendChild(row);
	table.appendChild(tbody);
	langsDiv.appendChild(table);
	form.insertBefore(langsDiv,form.getElementsByTagName('p')[0]);
}

function changeLangType() {
	if (this.value == 0) {
		audlangs.row.style.display = '';
		sublangs.row.style.display = 'none';
	} else {
		audlangs.row.style.display = 'none';
		sublangs.row.style.display = '';
	}
}

function addLangToBox() {
	var type = 'aud';
	if (this.nodeName.toLowerCase() == 'input') type = (this.id.indexOf('Aud') > 0) ? 'aud' : 'sub';
	else type = (this.parentNode.id.indexOf('audio') >= 0) ? 'aud' : 'sub';
	if (type == 'aud') {
		if (audlangs.select[0].value == '') return;
		var selectedId = audlangs.select[0].selectedIndex;
		if (audlangs.select[0].value != '0') {
			audlangs.add(audlangs.select[0].value);
			if (audlangs.array.indexOf('0') >= 0) {
				audlangs.rem('0');
				for (var i = 0; i < audlangs.select[1].options.length; i++) {
					if (audlangs.select[1].options[i].value != 0) continue;
					var newOption = audlangs.select[1].options[i];
					newOption.ondblclick = addLangToBox;
					audlangs.select[0].appendChild(newOption);
					break;
				}
			}
			var newOption = audlangs.select[0].options[selectedId];
			newOption.ondblclick = remLangFromBox;
			audlangs.select[1].appendChild(newOption);
		} else { // we are adding the no language lang, we need to remove all other langs
			while(audlangs.select[1].options.length) {
				var newOption = audlangs.select[1].options[0];
				newOption.ondblclick = addLangToBox;
				audlangs.select[0].appendChild(newOption);
			}
			var newOption = audlangs.select[0].options[selectedId];
			newOption.ondblclick = remLangFromBox;
			audlangs.select[1].appendChild(newOption);
			audlangs.array = [ '0' ];
		}
	} else {
		if (sublangs.select[0].value == '') return;
		var selectedId = sublangs.select[0].selectedIndex;
		if (sublangs.select[0].value != '0') {
			sublangs.add(sublangs.select[0].value);
			if (sublangs.array.indexOf('0') >= 0) {
				sublangs.rem('0');
				for (var i = 0; i < sublangs.select[1].options.length; i++) {
					if (sublangs.select[1].options[i].value != 0) continue;
					var newOption = sublangs.select[1].options[i];
					newOption.ondblclick = addLangToBox;
					sublangs.select[0].appendChild(newOption);
					break;
				}
			}
			var newOption = sublangs.select[0].options[selectedId];
			newOption.ondblclick = remLangFromBox;
			sublangs.select[1].appendChild(newOption);
		} else { // we are adding the no language lang, we need to remove all other langs
			var selectedId = sublangs.select[0].selectedIndex;
			while(sublangs.select[1].options.length) {
				var newOption = sublangs.select[1].options[0];
				newOption.ondblclick = addLangToBox;
				sublangs.select[0].appendChild(newOption);
			}
			var newOption = sublangs.select[0].options[selectedId];
			newOption.ondblclick = remLangFromBox;
			sublangs.select[1].appendChild(newOption);
			sublangs.array = [ '0' ];
		}
	}
	audlangs.toString();
	sublangs.toString();
}

function remLangFromBox() {
	var type = 'aud';
	if (this.nodeName.toLowerCase() == 'input') type = (this.id.indexOf('Aud') > 0) ? 'aud' : 'sub';
	else type = (this.parentNode.id.indexOf('audio') >= 0) ? 'aud' : 'sub';
	if (type == 'aud') {
		if (audlangs.select[1].value == '') return;
		if (audlangs.array.length == 1 && audlangs.array[0] == '0') return;
		audlangs.rem(audlangs.select[1].value);
		var newOption = audlangs.select[1].options[audlangs.select[1].selectedIndex];
		newOption.ondblclick = addLangToBox;
		audlangs.select[0].appendChild(newOption);
		if (!audlangs.select[1].options.length) { // clean
			for (var i = 0; i < audlangs.select[0].options.length; i++) {
				if (audlangs.select[0].options[i].value != 0) continue;
				var newOption = audlangs.select[0].options[i];
				newOption.ondblclick = remLangFromBox;
				audlangs.select[1].appendChild(newOption);
				break;
			}
			audlangs.array = [ '0' ];
		}
	} else {
		if (sublangs.select[1].value == '') return;
		if (sublangs.array.length == 1 && sublangs.array[0] == '0') return;
		sublangs.rem(sublangs.select[1].value);
		var newOption = sublangs.select[1].options[sublangs.select[1].selectedIndex];
		newOption.ondblclick = addLangToBox;
		sublangs.select[0].appendChild(newOption);
		if (!sublangs.select[1].options.length) { // clean
			for (var i = 0; i < sublangs.select[0].options.length; i++) {
				if (sublangs.select[0].options[i].value != 0) continue;
				var newOption = sublangs.select[0].options[i];
				newOption.ondblclick = remLangFromBox;
				sublangs.select[1].appendChild(newOption);
				break;
			}
			sublangs.array = [ '0' ];
		}
	}
	audlangs.toString();
	sublangs.toString();
}

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

/* Javascript Options Tab Work */
function tabWork() {
	var target = getElementsByClassName(document.getElementsByTagName('ul'), 'tabs', false)[0];
	if (!target) return;
	var bodyDiv = target.parentNode.getElementsByTagName('div')[0];
	if (!bodyDiv) return;
	// Create the new tab
	var lis = target.getElementsByTagName('li');
	var jstab = document.createElement('li');
	var num = lis.length+1;
	jstab.id = 'tab'+num;
	jstab.className = 'tab';
	jstab.appendChild(document.createTextNode('Javascript'));
	jstab.onclick = Magic.toggle_tabs;
	target.appendChild(jstab);
	// now to actualy make the options
	var jsdiv = document.createElement('div');
	jsdiv.id = 'tab'+num+'_pane';
	jsdiv.className = 'pane hide';
	// add all possible js options here, i don't care!
	
	var type = 'all';
	var items = new Object();
	var titlePrefs = {'id':"title-prefs",'head':"Title",'title':"Title Preferences",'default':true};
	var ed2kPrefs = {'id':"ed2k-prefs",'head':"ED2K",'title':"ED2K Link Preferences"};
	var mylistPrefs = {'id':"mylist-prefs",'head':"Mylist",'title':"Mylist Quick-Add Preferences"};
	var groupPrefs = {'id':"group-prefs",'head':"Group",'title':"Group select Preferences"};
	var otherPrefs = {'id':"other-prefs",'head':"Other",'title':"Other Preferences"};
	items['all'] =	[titlePrefs, ed2kPrefs, mylistPrefs, groupPrefs, otherPrefs];
	
	/* load settings from cookie */
	var animeAltTitleLang = CookieGet('animeAltTitleLang') || "x-jat";
	var episodeAltTitleLang = CookieGet('episodeAltTitleLang') || "x-jat";
	var episodeTitleDisplay = CookieGet('episodeTitleDisplay') || 2;
	var ed2k_pattern = CookieGet('ed2k_pattern') || "%ant - %enr%ver - %ept - <[%grp]><(%crc)><(%cen)><(%lang)><(%raw)>";
	var space_pattern = CookieGet('space_pattern') || "_";
	var use_mylist_add = CookieGet('use_mylist_add') || 0;
	var mylist_add_viewed_state = CookieGet('mylist_add_viewed_state') || 0;
	var mylist_add_state = CookieGet('mylist_add_state') || 0;
	var mylist_add_fstate = CookieGet('mylist_add_fstate') || 0;
	var group_check_type = CookieGet('group_check_type') || 0;
	var currentFMode = CookieGet('currentFMode') || 2;
	var storedTab = CookieGet('tab') || '';
	
	/* create preferences tabs */
	
	var body = document.createElement('div');
	body.className = 'prefs';
	for (var t = 0; t < items[type].length; t++) {
		var item = items[type][t];
		var tab = document.createElement('div');
		tab.id = "pref"+(t+1)+"_pane";
		var h4 = document.createElement('h4');
		h4.appendChild(document.createTextNode(item['title']));
		tab.appendChild(h4);
		switch(item['id']) {
			case 'title-prefs':
				var ul = document.createElement('ul');
				var li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_TITLE', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				createLanguageSelect(li,'animeAltTitleLang','animeAltTitleLang',function() { changeOptionValue(this); animeAltTitleLang = this.value; },animeAltTitleLang);
				li.appendChild(document.createTextNode(' Anime Alternative Title Language'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_TITLE', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				createLanguageSelect(li,'episodeAltTitleLang','episodeAltTitleLang',function() { changeOptionValue(this); episodeAltTitleLang = this.value; },episodeAltTitleLang);
				li.appendChild(document.createTextNode(' Episode Alternative Title Language'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_TITLE', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var optionArray = { 0 : {"text":'Title'} , 1 : {"text":'Title (Alt. Title)'}, 2 : {"text":'Title [tooltip: Alt. Title]'}, 3 : {"text":'Title [tooltip Alt.title / Ja title]'}, 4 : {"text":'Title (Alt. Title / Ja title)'}};
				createSelectArray(li,'episodeTitleDisplay','episodeTitleDisplay',function() { changeOptionValue(this); episodeTitleDisplay = this.value; },episodeTitleDisplay,optionArray);
				li.appendChild(document.createTextNode(' Episode Alternative Title Display'));
				ul.appendChild(li);
				tab.appendChild(ul);
				break;
			case 'ed2k-prefs':
				var ul = document.createElement('ul');
				var li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_ED2K', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var input = createTextInput('ed2k_pattern',80,false,false,255,ed2k_pattern);
				input.onchange = function() { 
					changeOptionValue(this); 
					ed2k_pattern = this.value;
				};
				li.appendChild(input);
				var setDefault = createBasicButton('set_ed2k_default','default');
				setDefault.onclick = function sd() {
					var input = document.getElementById('ed2k_pattern');
					if (input) input.value = "%ant - %enr%ver - %ept - <[%grp]><(%crc)><(%cen)><(%lang)><(%raw)>";
					ed2k_pattern = "%ant - %enr%ver - %ept - <[%grp]><(%crc)><(%cen)><(%lang)><(%raw)>";
				}
				li.appendChild(document.createTextNode(' '));
				li.appendChild(setDefault);
				li.appendChild(document.createTextNode(' ED2K hash style'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_ED2K', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var setSpace = createTextInput('space_pattern',1,false,false,1,space_pattern);
				setSpace.onchange = function() { 
					changeOptionValue(this); 
					space_pattern = this.value;
				};
				li.appendChild(setSpace);
				var setSpaceDefault = createBasicButton('set_space_default','default');
				setSpaceDefault.onclick = function ssd() {
					var input = document.getElementById('space_pattern');
					if (input) input.value = '_';
					space_pattern = '_';
				}
				li.appendChild(document.createTextNode(' '));
				li.appendChild(setSpaceDefault);
				li.appendChild(document.createTextNode(' ED2K hash spaces convert character'));
				ul.appendChild(li);
				tab.appendChild(ul);
				break;
			case 'mylist-prefs':
				var ul = document.createElement('ul');
				var li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var ck = createCheckbox('use_mylist_add',use_mylist_add);
				ck.onchange = function() {
					changeOptionValue(this);
					use_mylist_add = Number(this.checked);
					document.getElementById('mylist_add_state').disabled = (!this.checked);
					document.getElementById('mylist_add_fstate').disabled = (!this.checked);
					document.getElementById('mylist_add_viewed_state').disabled = (!this.checked);
				}
				li.appendChild(ck);
				li.appendChild(document.createTextNode(' Use quick-add instead of normal mylist add'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var optionArray = {	0:{"text":'unknown'},1:{"text":'internal storage (hdd)'},
									2:{"text":'external storage (cd/dvd/...)'},3:{"text":'deleted'}};
				var stateSel = createSelectArray(null,"mylist_add_state","mylist_add_state",null,mylist_add_state,optionArray);
				if (!use_mylist_add) stateSel.disabled = true;
				stateSel.onchange = function() { changeOptionValue(this); mylist_add_state = this.value; };
				li.appendChild(stateSel);
				li.appendChild(document.createTextNode(' Default quick-add state'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				optionArray = {	0:{"text":' normal/original '},1:{"text":' corrupted version/invalid crc '},
								2:{"text":' self edited '},100:{"text":' other '}};
				var fstateSel = createSelectArray(null,"mylist_add_fstate","mylist_add_fstate",null,mylist_add_fstate,optionArray);
				if (!use_mylist_add) fstateSel.disabled = true;
				fstateSel.onchange = function() { changeOptionValue(this); mylist_add_fstate = this.value; };
				li.appendChild(fstateSel);
				li.appendChild(document.createTextNode(' Default quick-add file state'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var watchedSel = createSelectArray(null,"mylist_add_viewed_state","mylist_add_viewed_state",null,mylist_add_viewed_state,{0:{"text":'unwatched'},1:{"text":'watched'}});
				if (!use_mylist_add) watchedSel.disabled = true;
				watchedSel.onchange = function() { changeOptionValue(this); mylist_add_viewed_state = this.value; };
				li.appendChild(watchedSel);
				li.appendChild(document.createTextNode(' Default quick-add watched state'));
				ul.appendChild(li);
				tab.appendChild(ul);
				break;
			case 'group-prefs':
				var ul = document.createElement('ul');
				var li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var rb = createBasicButton('group_check_type',5,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 5);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all files'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				rb = createBasicButton('group_check_type',0,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 0);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprecated files'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				rb = createBasicButton('group_check_type',1,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 1);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprecated mkv/ogm/mp4 files'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				rb = createBasicButton('group_check_type',2,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 2);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprecated avi files'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				rb = createBasicButton('group_check_type',3,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 3);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprecated High Definition files (video resolution height >= 720)'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				rb = createBasicButton('group_check_type',4,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 4);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprecated Standard Definition files (video resolution height < 720)'));
				ul.appendChild(li);
				tab.appendChild(ul);
				break;
			case 'other-prefs':
				var ul = document.createElement('ul');
				var li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var optionArray = {0:{"text":'Off',"desc":'All modes disabled, functions as if Javascript is disabled'},
					1:{"text":'Assisted',"desc":'Textarea with some assistance'},
					2:{"text":'Visual',"desc":'WYSIWYG - What You See Is What You (Usualy) Get'}};
				var fmodeSel = createSelectArray(null,"currentFMode","currentFMode",null,currentFMode,optionArray);
				fmodeSel.onchange = function() { changeOptionValue(this); currentFMode = this.value; };
				li.appendChild(fmodeSel);
				li.appendChild(document.createTextNode(' Default Text Editor Mode'));
				ul.appendChild(li);
				tab.appendChild(ul);
				break;
			default:
		}
		body.appendChild(tab);
	}
	jsdiv.appendChild(body);
	
	bodyDiv.appendChild(jsdiv);
}

function prepPage() {
	uriObj = parseURI(); // update the uriObj
	tabWork();
	fetchData(); // Fetch Data, we update the page in the meanwhile
	cssurl = document.getElementById('style_url');
	if (!cssurl) return;
	current_css = cssurl.value.substring(cssurl.value.lastIndexOf('/')+1,cssurl.value.indexOf('.css'));
	// cssurl.style.display = "none";
	// more stuff
	style_thumbnail = document.getElementById('style_thumbnail');
	style_title = document.getElementById('style_title');
	style_creator = document.getElementById('style_creator');
	style_status = document.getElementById('style_status');
	style_updated = document.getElementById('style_updated');
	style_description = document.getElementById('style_description');
	style_apply = document.getElementById('style_apply');
	style_next = document.getElementById('style_next');
	style_prev = document.getElementById('style_prev');
	var inputs = document.getElementsByTagName('input');
	audlangs.input = getElementsByName(inputs, 'lang.audin', false)[0];
	if (audlangs.input) {
		audlangs.text = audlangs.input.value;
		audlangs.array = audlangs.text.replace(/[{}]/mgi,'').split(',');
		if (audlangs.array.length == 1 && !audlangs.array[0].length) audlangs.array = new Array();
		if (!audlangs.array.length) audlangs.array.push('0');
		audlangs.add = addLanguageToArray;
		audlangs.rem = remLanguageFromArray;
		audlangs.toString = convLanguagesToText;
		audlangs.input.name = 'lang.filealang';
	}
	sublangs.input = getElementsByName(inputs, 'lang.subin', false)[0];
	if (sublangs.input) {
		sublangs.text = sublangs.input.value;
		sublangs.array = sublangs.text.replace(/[{}]/mgi,'').split(',');
		if (sublangs.array.length == 1 && !sublangs.array[0].length) sublangs.array = new Array();
		if (!sublangs.array.length) sublangs.array.push('0');
		sublangs.add = addLanguageToArray;
		sublangs.rem = remLanguageFromArray;
		sublangs.toString = convLanguagesToText;
		sublangs.input.name = 'lang.fileslang';
	}
	if (audlangs.input && sublangs.input) prepLanguages();
}
 
//window.onload = prePage;
addLoadEvent(prepPage);
