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
var animeFileColsList = {
	'default':{'text':"Default"},
	'check-anime':{'text':"(Un)Check file"},
	'fid':{'text':"File details/FID"},
	'group':{'text':"Group"},
	'size':{'text':"Size"},
	'crc':{'text':"CRC (profile dependent)"},
	'langs':{'text':"Languages"},
	'cf':{'text':"Container Format"},
	'resolution':{'text':"Resolution"},
	'anime-source':{'text':"Source"},
	'quality':{'text':"Quality"},
	'anime-hashes':{'text':"Hashes"},
	'users':{'text':"Users"},
	'state-anime':{'text':"Mylist"},
	'actions-anime':{'text':"Actions"}
}
var animePage_defLayout = ['check-anime','fid','group','size','crc','langs','cf','resolution','anime-source','quality','anime-hashes','users','state-anime','actions-anime'];
var animePage_curLayout = ['check-anime','fid','group','size','crc','langs','cf','resolution','anime-source','quality','anime-hashes','users','state-anime','actions-anime'];
var animePage_sorts = ['default','fid','group','size','cf','resolution','anime-source','users'];
var animePage_sortsV = ['default','fid','group','size','codec','resolution','source','users'];
var animePage_curSort = 'default'; // whatever the db spits out 
var ed2k_pattern = (hashObj && hashObj.pattern ? hashObj.pattern : "%ant - %enr%ver - %ept - <[%grp]><(%crc)><(%cen)><(%lang)><(%raw)>");
var space_pattern = (hashObj && hashObj.spacesChar ? hashObj.spacesChar : "_");
var use_mylist_add = 0;
var mylist_add_viewed_state = 0;
var mylist_add_state = 0;
var mylist_add_fstate = 0;
var group_check_type = 0;
var group_langfilter = 0;
var collapseThumbnails = 0;
var def_search = 'none';
var get_info = 0; // bit var
var get_info_sz = '150';
var get_info_mw = '450';
var animePageLayout = null;
var virtualDiv = document.createElement('div');
var arePrefsShown = false;
var defPrefTab = 0;
var storedTab = '';
var currentFMode = 1;
var picbase = 'http://img5.anidb.net/pics/anime/';
var charInfos = new Array(); 		// Character information	(indexed by charid)
var creatorInfos = new Array(); 	// Creator information		(indexed by creatorid)
var animeInfos = new Array();		// Anime information		(indexed by aid)

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

function addSetting(ul,array) {
	var help_text = array["help-text"] ? array["help-text"] : 'Those who seek help shall find it.';
	var li = document.createElement('li');
	createLink(li, '[?]', array["help-link"], 'wiki', null, help_text, 'i_inline i_help');
	if (array["element"])
		li.appendChild(array["element"]);
	if (array["elements"]) {
		for (var i=0; i < array["elements"].length; i++) {
			li.appendChild(array["elements"][i]);
			if (i < array["elements"].length-1) 
				li.appendChild(document.createTextNode(" "));
		}
	}
	if (array["text"]) 
		li.appendChild(document.createTextNode(' '+array["text"]));
	ul.appendChild(li);	
}


/* Function that creates the preferences table
 * @param type Type can be either mylist or anime
 */
function createPreferencesTable(type) {
	var items = new Object();
	var titlePrefs = {'id':"title-prefs",'head':"Title",'title':"Title Preferences",'default':true};
	var ed2kPrefs = {'id':"ed2k-prefs",'head':"ED2K",'title':"ED2K Link Preferences"};
	var mylistPrefs = {'id':"mylist-prefs",'head':"Mylist",'title':"Mylist Quick-Add Preferences"};
	var global = {'id':"global",'head':"Global",'title':"Global Layout Preferences",'default':true};
	var groupPrefs = {'id':"group-prefs",'head':"Group",'title':"Group select Preferences"};
	var animeLayoutPrefs = {'id':"anime-layout",'head':"Layout",'title':"Anime page Layout Preferences"};
	var otherPrefs = {'id':"other-prefs",'head':"Other",'title':"Other Preferences"};
	items['mylist'] =	[titlePrefs, ed2kPrefs, global];
	items['anime'] =	[titlePrefs, ed2kPrefs, mylistPrefs, groupPrefs, animeLayoutPrefs, global];
	items['group'] =	[titlePrefs, ed2kPrefs, groupPrefs, global];
	items['episode'] =	[titlePrefs, ed2kPrefs, mylistPrefs, global];
	items['global'] =	[global];
	items['profile'] =	[titlePrefs, ed2kPrefs, mylistPrefs, groupPrefs, animeLayoutPrefs, global, otherPrefs];
	if (!items[type]) return;

	/* load settings from cookie */
	loadSettings();
	
	/* create preferences tabs */
	var main = panes = jsdiv = bodyDiv = null;
	var body = document.createElement('div');

	panes = document.createElement('div');
	panes.className = "tabbed_pane";
	var ul_tabs = document.createElement('ul');
	ul_tabs.className = 'tabs';
	for (var t = 0; t < items[type].length; t++) {
		var li = document.createElement('li');
		li.id = "pref"+(t+1);
		li.className = "tab" + (t < 1 ? ' selected' : '');
		li.appendChild(document.createTextNode(items[type][t]['head']));
		li.onclick = Magic.toggle_tabs;
		ul_tabs.appendChild(li);
		if (t < items[type].length) ul_tabs.appendChild(document.createTextNode(' '));
	}
	
	panes.appendChild(ul_tabs);
	
	if (type != 'profile') {
		main = document.createElement('div');
		main.className = "g_section preferences";
		main.style.display = 'none';
		main.id = 'layout-prefs';
		body.className = 'body';
	} else {
		var target = getElementsByClassName(document.getElementsByTagName('ul'), 'tabs', false)[0];
		if (!target) return;
		bodyDiv = target.parentNode.getElementsByTagName('div')[0];
		if (!bodyDiv) return;
		// Create the new tab
		var lis = target.getElementsByTagName('li');
		var jstab = document.createElement('li');
		var num = lis.length+2; // for some strange reason stuff now starts from 2
		jstab.id = 'tab_'+num;
		jstab.className = 'tab javascript';
		jstab.appendChild(document.createTextNode('Javascript'));
		jstab.onclick = Magic.toggle_tabs;
		target.appendChild(jstab);
		// now to actualy make the options
		jsdiv = document.createElement('div');
		jsdiv.id = 'tab_'+num+'_pane';
		jsdiv.className = 'pane hide javascript';
	
		body.className = 'body';
	}
	for (var t = 0; t < items[type].length; t++) {
		var item = items[type][t];
		var tab = document.createElement('div');
		tab.id = "pref"+(t+1)+"_pane";
		tab.className = "pane" + (t > 0 ? ' hide' : '');
		var h4 = document.createElement('h4');
		h4.appendChild(document.createTextNode(item['title']));
		tab.appendChild(h4);
		switch(item['id']) {
			case 'title-prefs':
				var ul = document.createElement('ul');
				addSetting(ul,{
					"element":createLanguageSelect(null,'animeAltTitleLang','animeAltTitleLang',function() { animeAltTitleLang = this.value; }, settings['title']['aATL']),
					"text":"Anime Alternative Title Language",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_TITLE',
					"help-text":"Set the alternate anime title language"});
				addSetting(ul,{
					"element":createLanguageSelect(null,'episodeAltTitleLang','episodeAltTitleLang',function() { episodeAltTitleLang = this.value; }, settings['title']['eATL']),
					"text":"Episode Alternative Title Language",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_TITLE',
					"help-text":"Sets the alternative episode title language"});
				addSetting(ul,{
					"element":createSelectArray(null,'episodeTitleDisplay','episodeTitleDisplay',function() { episodeTitleDisplay = this.value; }, settings['title']['eTD'], { 0 : {"text":'Title'} , 1 : {"text":'Title (Alt. Title)'}, 2 : {"text":'Title [tooltip: Alt. Title]'}, 3 : {"text":'Title [tooltip Alt.title / Ja title]'}, 4 : {"text":'Title (Alt. Title / Ja title)'}}),
					"text":"Episode Alternative Title Display",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_TITLE',
					"help-text":"How to display the alternate episode title"});
				tab.appendChild(ul);
				break;
			case 'ed2k-prefs':
				var ul = document.createElement('ul');
				var elements = new Array();
				var input = createTextInput('ed2k_pattern',80,false,false,255,ed2k_pattern);
				input.onchange = function() { 
					ed2k_pattern = this.value;
					hashObj.pattern = ed2k_pattern;
					hashObj.ed2k = "ed2k://|file|"+hashObj.pattern+".%ext|%flen|%ed2k|";
					hashObj.sfv = hashObj.pattern+".%ext %crc";
				};
				elements.push(input);
				var setDefault = createBasicButton('set_ed2k_default','default');
				setDefault.onclick = function sd() {
					var input = document.getElementById('ed2k_pattern');
					if (input) input.value = hashObj.defaultPattern;
					ed2k_pattern = hashObj.defaultPattern;
					hashObj.pattern = ed2k_pattern;
					hashObj.ed2k = "ed2k://|file|"+hashObj.pattern+".%ext|%flen|%ed2k|";
					hashObj.sfv = hashObj.pattern+".%ext %crc";
				};
				elements.push(setDefault);
				addSetting(ul,{
					"elements":elements,
					"text":"ED2K hash style",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_ED2K',
					"help-text":"You can customize the display of the ed2k links here, see the wiki page for more help."});
				
				elements = new Array();
				var setSpace = createTextInput('space_pattern',1,false,false,1,space_pattern);
				setSpace.onchange = function() { 
					space_pattern = this.value;
					hashObj.spacesChar = space_pattern;
				};
				elements.push(setSpace);
				var setSpaceDefault = createBasicButton('set_space_default','default');
				setSpaceDefault.onclick = function ssd() {
					var input = document.getElementById('space_pattern');
					if (input) input.value = hashObj.defaultSpacesChar;
					space_pattern = hashObj.defaultSpacesChar;
					hashObj.spacesChar = space_pattern;
				};
				elements.push(setSpaceDefault);				
				addSetting(ul,{
					"elements":elements,
					"text":"ED2K hash spaces convert character",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_ED2K',
					"help-text":"You can chose what to do with spaces in the ed2k links here."});

				addSetting(ul,{
					"element":createLabledCheckBox(null,'pad_epnums','pad_epnums',function() {
							pad_epnums = Number(this.checked);
							document.getElementById('pad_only_normal_epnums').disabled = !pad_epnums;
						},Number(pad_epnums),' Pad episode numbers',null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST',
					"help-text":"Pad episode numbers to the same greatness as the maximum normal episode count. ie: 25 eps -> S01, 251 eps -> S001."});
				
				addSetting(ul,{
					"element":createLabledCheckBox(null,'pad_only_normal_epnums','pad_only_normal_epnums',function() {
							pad_only_normal_epnums = Number(this.checked);
						},Number(pad_only_normal_epnums),' Pad only normal episodes',null,!pad_epnums),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST',
					"help-text":"Use the above padding only to pad \"Normal\" episode numbers."});
				tab.appendChild(ul);
				break;
			case 'mylist-prefs':
				var ul = document.createElement('ul');
				addSetting(ul,{
					"element":createLabledCheckBox(null,'use_mylist_add','use_mylist_add',function() {
							use_mylist_add = Number(this.checked);
							document.getElementById('mylist_add_state').disabled = (!this.checked);
							document.getElementById('mylist_add_gstate').disabled = (!this.checked);
							document.getElementById('mylist_add_fstate').disabled = (!this.checked);
							document.getElementById('mylist_add_viewed_state').disabled = (!this.checked);
						},Number(use_mylist_add),' Use quick-add instead of normal mylist add',null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST',
					"help-text":"Some pages allow for you to add files to mylist without reloads, enable it here."});
				var optionArray = {	0:{"text":'unknown'},1:{"text":'internal storage (hdd)'},
									2:{"text":'external storage (cd/dvd/...)'},3:{"text":'deleted'}};
				var stateSel = createSelectArray(null,"mylist_add_state","mylist_add_state",null,mylist_add_state,optionArray);
				if (!use_mylist_add) stateSel.disabled = true;
				stateSel.onchange = function() { mylist_add_state = this.value; };
				addSetting(ul,{
					"element":stateSel,
					"text":"Default quick-add state",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST',
					"help-text":"Default mylist quick-add state"});

				optionArray = {	0:{"text":' normal/original '},1:{"text":' corrupted version/invalid crc '},
								2:{"text":' self edited '},100:{"text":' other '}};
				var fstateSel = createSelectArray(null,"mylist_add_fstate","mylist_add_fstate",null,mylist_add_fstate,optionArray);
				if (!use_mylist_add) fstateSel.disabled = true;
				fstateSel.onchange = function() { mylist_add_fstate = this.value; };
				addSetting(ul,{
					"element":fstateSel,
					"text":"Default quick-add file state",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST',
					"help-text":"Default mylist quick-add file state"});

				optionArray = {0:{"text": ' page default '},100:{"text":' other '},10:{"text":' self ripped '},11:{"text":' on dvd '},
						 12:{"text":' on vhs '},13:{"text":' on tv '},14:{"text":' theater '},15:{"text":' streamed '}};
				var gstateSel = createSelectArray(null,"mylist_add_gstate","mylist_add_gstate",null,mylist_add_gstate,optionArray);
				if (!use_mylist_add) gstateSel.disabled = true;
				gstateSel.onchange = function() { mylist_add_gstate = this.value; };
				addSetting(ul,{
					"element":gstateSel,
					"text":"Default quick-add file state (generic)",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST',
					"help-text":"Default mylist quick-add file state for generic files"});
					
				var watchedSel = createSelectArray(null,"mylist_add_viewed_state","mylist_add_viewed_state",null,mylist_add_viewed_state,{0:{"text":'unwatched'},1:{"text":'watched'}});
				if (!use_mylist_add) watchedSel.disabled = true;
				watchedSel.onchange = function() { mylist_add_viewed_state = this.value; };
				addSetting(ul,{
					"element":watchedSel,
					"text":"Default quick-add watched state",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_MYLIST',
					"help-text":"Default mylist quick-add watched state"});
				tab.appendChild(ul);
				break;
			case 'group-prefs':
				var ul = document.createElement('ul');
				addSetting(ul,{
					"element":createLabledRadioBox(null,'group_check_type',null,function() { group_check_type = this.value; },5,(group_check_type == 5),'Check all files',null,null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP',
					"help-text":"Check all group files"});
				addSetting(ul,{
					"element":createLabledRadioBox(null,'group_check_type',null,function() { group_check_type = this.value; },0,(group_check_type == 0),'Check all non-deprecated files',null,null),
					"text":"",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP',
					"help-text":"Check all non-deprecated group files"});
				addSetting(ul,{
					"element":createLabledRadioBox(null,'group_check_type',null,function() { group_check_type = this.value; },1,(group_check_type == 1),'Check all non-deprecated mkv/ogm/mp4 files',null,null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP',
					"help-text":"Check all non-deprecated mkv/ogm/mp4 group files"});
				addSetting(ul,{
					"element":createLabledRadioBox(null,'group_check_type',null,function() { group_check_type = this.value; },2,(group_check_type == 2),'Check all non-deprecated avi files',null,null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP',
					"help-text":"Check all non-deprecated avi group files"});
				addSetting(ul,{
					"element":createLabledRadioBox(null,'group_check_type',null,function() { group_check_type = this.value; },3,(group_check_type == 3),'Check all non-deprecated High Definition files (video resolution height >= 720) files',null,null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP',
					"help-text":"Check all non-deprecated High Definition files (video resolution height >= 720) group files"});
				addSetting(ul,{
					"element":createLabledRadioBox(null,'group_check_type',null,function() { group_check_type = this.value; },4,(group_check_type == 4),'Check all non-deprecated Standard Definition files (video resolution height < 720) files',null,null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP',
					"help-text":"Check all non-deprecated Standard Definition files (video resolution height < 720) group files"});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'group_langfilter','group_langfilter',function() { group_langfilter = this.value; },Number(group_langfilter),' Filter groups in group table according to language preferences (bypasses profile option if enabled)',null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP',
					"help-text":"Filter groups in group table according to language preferences"});
				tab.appendChild(ul);
				break;
			case 'anime-layout':
				var ul = document.createElement('ul');
				var li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_ANIME_LAYOUT', 'wiki', null, 'You chose what columns, and in what order, are displayed in anime file tables.', 'i_inline i_help');
				li.appendChild(document.createTextNode('File table column layout'));
				li.appendChild(makeLayoutPreferencesTable());
				ul.appendChild(li);
				var elements = new Array();
				var defaultSort = createSelectArray(null,"animePage_curSort","animePage_curSort");
				for (var si = 0; si < animePage_sorts.length; si++) {
					var option = animePage_sorts[si];
					var value = animePage_sortsV[si];
					var text = animeFileColsList[option]['text'];
					createSelectOption(defaultSort, text, value, (value == animePage_curSort));
				}
				defaultSort.onchange = function() { animePage_curSort = this.value; };
				elements.push(defaultSort);
				var defaultSortOrder = createSelectArray(null,"animePage_curSortOrder","animePage_curSortOrder",null,animePage_curSortOrder,{'down':{'text':"descending"},'up':{'text':"ascending"}});
				defaultSortOrder.onchange = function() { animePage_curSortOrder = this.value; };
				elements.push(defaultSortOrder);
				addSetting(ul,{
					"elements":elements,
					"text":"Default sorted column",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_ANIME_LAYOUT',
					"help-text":"Chose the default column and the default ordering in file tables."});

				var szformatSel = createSelectArray(null,"format_size","format_size",null,format_size,{0:{"text":'no'},1:{"text":'yes'}});
				szformatSel.onchange = function() { format_size = config['settings']['FORMATFILESIZE'] = this.value; };
				addSetting(ul,{
					"element":szformatSel,
					"text":"Format file size",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_ANIME_LAYOUT',
					"help-text":"Chose if you should format the file size display from bytes to the nearest size order."});
				tab.appendChild(ul);
				break;
			case 'other-prefs': // only available on the profile page
				var ul = document.createElement('ul');
				var optionArray = {0:{"text":'Off',"desc":'All modes disabled, functions as if Javascript is disabled'},
					1:{"text":'Assisted',"desc":'Textarea with some assistance'},
					2:{"text":'Visual',"desc":'WYSIWYG - What You See Is What You (Usualy) Get'}};
				var fmodeSel = createSelectArray(null,"currentFMode","currentFMode",null,currentFMode,optionArray);
				fmodeSel.onchange = function() { currentFMode = this.value; };
				addSetting(ul,{
					"element":fmodeSel,
					"text":"Default Text Editor Mode",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
					"help-text":"Chose the default text editor mode from the available settings."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'usejspopups','usejspopups',function() { usejspopups = this.checked; },Number(usejspopups),' Use Javascript popups for anidb::popup',null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
					"help-text":"Select if you want to use Javascript popups where available."});
				var dfsearch = createBasicSelect('def_search','def_search',function() { def_search = this.value; if (def_search != 'none' && searchTypeSelect) searchTypeSelect.value = def_search;});
				createSelectOption(dfsearch, 'default', 'none', (def_search == 'none'));
				if (searchTypeSelect && searchTypeSelect.options) {
					for (var sto = 0; sto < searchTypeSelect.options.length; sto++) {
						var opt = searchTypeSelect.options[sto];
						createSelectOption(dfsearch, opt.text, opt.value, (def_search == opt.value));
					}
				}
				addSetting(ul,{
					"element":dfsearch,
					"text":"Default Search Type",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
					"help-text":"Select if you want to use Javascript popups where available."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'search_assist','search_assist',function() { searchTypeAssist = this.checked; },Number(search_assist),' Use anidb search assist',null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
					"help-text":"Select if you want to use search assistance on the anidb search box where available."});
				optionArray = {0:{"text":'default',"desc":'Default behaviour'},
					1:{"text":'always hidden',"desc":'Menus are always hidden by default'},
					2:{"text":'auto',"desc":'Menus will be hidden by default depending on the width of the window'}};
				var menuCollapseSel = createSelectArray(null,"menuCollapse","menuCollapse",null,settings['other']['menuCollapse'],optionArray);
				menuCollapseSel.onchange = function() { settings['other']['menuCollapse'] = this.value; };
				addSetting(ul,{
					"element":menuCollapseSel,
					"text":"Behaviour of collapsable menus",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
					"help-text":"You can alter the default behaviour of collapsable menus."});
				if (String(""+window.location).indexOf('anidb.net') < 0) { // Show extra options for the local developer
					addSetting(ul,{
						"element":createLabledCheckBox(null,'seeDebug','seeDebug',function() { seeDebug = this.checked; },Number(seeDebug),' See Debug information (can be very verbose and very alerty)',null),
						"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
						"help-text":"Check if you want to see debug information (lots of Alert boxes)."});
					addSetting(ul,{
						"element":createLabledCheckBox(null,'seeTimes','seeTimes',function() { seeTimes = this.checked; },Number(seeTimes),' See Timing information (can be very verbose and is very alerty)',null),
						"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
						"help-text":"Check if you want to see timing information (lots and lots of Alert boxes)."});
					addSetting(ul,{
						"element":createLabledCheckBox(null,'ignoreLocal','ignoreLocal',function() { ignoreLocal = this.checked; },Number(ignoreLocal),' Ignore the localhost check (for usage with your own anidb copy)',null),
						"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
						"help-text":"Are you one of those happy mods running their own copy of anidb running it on localhost and sick of the xml file not found alert? Check this!"});
				}
				tab.appendChild(ul);
				break;
			case 'global':
				var ul = document.createElement('ul');
				addSetting(ul,{
					"element":createLabledCheckBox(null,'collapseThumbnails','collapseThumbnails',function() { 
					collapseThumbnails = Number(this.checked); 
					},Number(collapseThumbnails),' Collapse thumbnails on rows',null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GLOBAL',
					"help-text":"Check if you want to collapse Thumbnail rows when not hovering on them."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'get_info','get_info',function() {
							document.getElementById('get_info_sz').disabled = !this.checked;
							document.getElementById('get_info_mw').disabled = !this.checked;
							document.getElementById('get_info_panime').disabled = !this.checked;
							document.getElementById('get_info_pmylist').disabled = !this.checked;
							document.getElementById('get_info_pepisode').disabled = !this.checked;
							document.getElementById('get_info_pgroup').disabled = !this.checked;
							document.getElementById('get_info_pcharacter').disabled = !this.checked;
							document.getElementById('get_info_pcreator').disabled = !this.checked;
							document.getElementById('get_info_pwishlist').disabled = !this.checked;
						},get_info,' Enable/disable information tooltips',null),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GLOBAL',
					"help-text":"Check if you want to enable information tooltips where available (chose where below)."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'get_info_panime','get_info_panime',null,(get_info & 1),' Use information tooltips on Anime pages',null,(!get_info)),
					"text":"",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GLOBAL',
					"help-text":"Check if you want to see information tooltips on anime pages."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'get_info_pmylist','get_info_pmylist',null,(get_info & 2),' Use information tooltips on Mylist',null,(!get_info)),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GLOBAL',
					"help-text":"Check if you want to see information tooltips on my list."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'get_info_pepisode','get_info_pepisode',null,(get_info & 4),' Use information tooltips on Episode pages',null,(!get_info)),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GLOBAL',
					"help-text":"Check if you want to see information tooltips on episode pages."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'get_info_pgroup','get_info_pgroup',null,(get_info & 8),' Use information tooltips on Group pages',null,(!get_info)),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GLOBAL',
					"help-text":"Check if you want to see information tooltips on group pages."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'get_info_pcharacter','get_info_pcharacter',null,(get_info & 16),' Use information tooltips on Character pages',null,(!get_info)),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GLOBAL',
					"help-text":"Check if you want to see information tooltips on character pages."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'get_info_pcreator','get_info_pcreator',null,(get_info & 32),' Use information tooltips on Creator pages',null,(!get_info)),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GLOBAL',
					"help-text":"Check if you want to see information tooltips on creator pages."});
				addSetting(ul,{
					"element":createLabledCheckBox(null,'get_info_pwishlist','get_info_pwishlist',null,(get_info & 64),' Use information tooltips on Wishlist',null,(!get_info)),
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_GLOBAL',
					"help-text":"Check if you want to see information tooltips on wishlist."});
				var ainfoSZ = createSelectArray(null,"get_info_sz","get_info_sz",null,get_info_sz,{'150':{"text":'large'},'50x65':{"text":'small'}});
				if (!get_info) ainfoSZ.disabled = true;
				ainfoSZ.onchange = function() { get_info_sz = this.value; };
				addSetting(ul,{
					"element":ainfoSZ,
					"text":"Default information tooltip size",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
					"help-text":"Set the default size for information tooltips."});
				var ainfoMW = createSelectArray(null,"get_info_mw","get_info_mw",null,get_info_mw,{'300':{"text":'small'},'450':{"text":'medium'},'600':{"text":'large'}});
				if (!get_info) ainfoMW.disabled = true;
				ainfoMW.onchange = function() { get_info_mw = this.value; };
				addSetting(ul,{
					"element":ainfoMW,
					"text":"Default information tooltip width",
					"help-link":'http://wiki.anidb.net/w/PAGE_PREFERENCES_OTHER',
					"help-text":"Set the default width for information tooltips."});
				tab.appendChild(ul);
				break;
			default:
		}
		body.appendChild(tab);
	}
	
	var p = document.createElement('p');
	var reloadInput = document.createElement('button');
	reloadInput.name = 'do.apply';
	reloadInput.value = "apply";
	reloadInput.appendChild(document.createTextNode((type != 'profile' ? 'Save' : 'Apply')));
	reloadInput.onclick = function reloadPage() {
		if (document.getElementById('animeAltTitleLang')) {
			settings['title']['aATL'] = document.getElementById('animeAltTitleLang').value;
			settings['title']['eATL'] = document.getElementById('episodeAltTitleLang').value;
			settings['title']['eTD'] = document.getElementById('episodeTitleDisplay').value;
			CookieSetFromArray('title', settings['title'], 3650);
		}
		if (document.getElementById('ed2k_pattern')) {
			settings['ed2k']['pattern'] = document.getElementById('ed2k_pattern').value;
			settings['ed2k']['space'] = document.getElementById('space_pattern').value;
			settings['ed2k']['pad'] = document.getElementById('pad_epnums').value;
			settings['ed2k']['padonormal'] = document.getElementById('pad_only_normal_epnums').value;
			CookieSetFromArray('ed2k', settings['ed2k'], 3650);
		}
		if (document.getElementById('use_mylist_add')) {
			settings['mylist']['use'] = Number(document.getElementById('use_mylist_add').checked)+'';
			settings['mylist']['state'] = document.getElementById('mylist_add_state').value;
			settings['mylist']['fstate'] = document.getElementById('mylist_add_fstate').value;
			settings['mylist']['viewed'] = document.getElementById('mylist_add_viewed_state').value;
			CookieSetFromArray('mylist', settings['mylist'], 3650);
		}
		if (document.getElementById('group_langfilter')) {
			var gradios = document.getElementsByName('group_check_type');
			for (var gi = 0; gi < gradios.length; gi++) {
				if (!gradios[gi].checked) continue;
				settings['group']['type'] = Number(gradios[gi].value)+'';
				break;
			}
			settings['group']['filter'] = Number(document.getElementById('group_langfilter').checked)+'';
			CookieSetFromArray('group', settings['group'], 3650);
		}
		if (document.getElementById('userAnimeLayoutSelect')) {
			var tempArray = new Array();
			var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
			animePage_curLayout = new Array();
			for (var oi = 0; oi < userAnimeLayoutSelect.options.length; oi++) {
				var value = userAnimeLayoutSelect.options[oi].value;
				tempArray.push(animePage_defLayout.indexOf(value));
				animePage_curLayout.push(value);
			}
			settings['aLayout']['aPL'] = tempArray.join(',');
			settings['aLayout']['aCS'] = document.getElementById('animePage_curSort').value;
			settings['aLayout']['aCSO'] = document.getElementById('animePage_curSortOrder').value;
			settings['aLayout']['fsize'] = document.getElementById('format_size').value;
			CookieSetFromArray('aLayout', settings['aLayout'], 3650);
		}
		if (document.getElementById('usejspopups')) {
			settings['other']['jsp'] = Number(document.getElementById('usejspopups').checked)+'';
			settings['other']['emode'] = document.getElementById('currentFMode').value;
			settings['other']['dsearch'] = document.getElementById('def_search').value;
			settings['other']['asearch'] = Number(document.getElementById('search_assist').checked)+'';
			settings['other']['menuCollapse'] = Number(document.getElementById('menuCollapse').value)+'';
			if (document.getElementById('seeDebug')) { settings['other']['seeDebug'] = Number(document.getElementById('seeDebug').checked)+''; }
			if (document.getElementById('seeTimes')) { settings['other']['seeTimes'] = Number(document.getElementById('seeTimes').checked)+''; }
			if (document.getElementById('ignoreLocal')) { settings['other']['ignoreLocal'] = Number(document.getElementById('ignoreLocal').checked)+''; }
			CookieSetFromArray('other', settings['other'], 3650);
		}
		if (document.getElementById('get_info_sz')) {
			get_info = 0;
			if (document.getElementById('get_info_panime').checked) get_info += 1;
			if (document.getElementById('get_info_pmylist').checked) get_info += 2;
			if (document.getElementById('get_info_pepisode').checked) get_info += 4;
			if (document.getElementById('get_info_pgroup').checked) get_info += 8;
			if (document.getElementById('get_info_pcharacter').checked) get_info += 16;
			if (document.getElementById('get_info_pcreator').checked) get_info += 32;
			if (document.getElementById('get_info_pwishlist').checked) get_info += 64;
			settings['global']['info'] = get_info+'';
			settings['global']['infosz'] = document.getElementById('get_info_sz').value;
			settings['global']['infomw'] = document.getElementById('get_info_mw').value;
			settings['global']['collapse'] = Number(document.getElementById('collapseThumbnails').checked)+'';
			CookieSetFromArray('global', settings['global'], 3650);
		}
		alert('Current preferences saved.');
	}
	p.appendChild(reloadInput);


	if (type != 'profile') {
		panes.appendChild(body);
		main.appendChild(panes);
		main.appendChild(p);

		// first find the main tabs
		var laytabs = document.getElementById('layout-tabs');
		var laycontent = document.getElementById('layout-content');
		if (!laytabs) {
			laytabs = document.createElement('div');
			laytabs.id = 'layout-tabs';
			var ultabs = document.createElement('ul');
			ultabs.className = 'main-tabs';
			var li = document.createElement('li');
			li.className = 'g_odd entry selected';
			createTextLink(li, 'entry', 'url');
			ultabs.appendChild(li);
			laytabs.appendChild(ultabs);
			if (laycontent) laycontent.insertBefore(laytabs,laycontent.firstChild);
		}
		var ul = laytabs.getElementsByTagName('ul')[0];
		if (ul) {
			var lis = ul.getElementsByTagName('li');
			defPrefTab = 0;
			for (var i = 0; i < lis.length; i++) if (lis[i].className.indexOf('selected') >= 0) defPrefTab = i;
			var li = document.createElement('li');
			li.className = 'customise' + (lis[lis.length-1].className.indexOf('g_odd') < 0 ? ' g_odd' : '');
			createTextLink(li, 'customise', 'removeme', null, toggleCustomizeBody, null, null);
			ul.appendChild(document.createTextNode(' '));
			ul.appendChild(li);
			if (lis[defPrefTab]) {
				var a = lis[defPrefTab].getElementsByTagName('a')[0];
				if (a) {
					a.removeAttribute('href');
					a.onclick = toggleCustomizeBody;
					a.style.cursor = 'pointer';
					a.setAttribute('role','link');
				}
			}
		}
		if (laycontent) laycontent.appendChild(main);
	} else {
		var header = document.createElement('h4');
		header.className = 'header';
		header.appendChild(document.createTextNode('Javascript'));
		jsdiv.appendChild(header);
		panes.appendChild(body);
		jsdiv.appendChild(panes);
		jsdiv.appendChild(p);		
		bodyDiv.appendChild(jsdiv);
	}
}

/* function that shows or hides the customize body */
function toggleCustomizeBody() {
	var laycontent = document.getElementById('layout-content');
	var laytabs = document.getElementById('layout-tabs');
	var laymain = document.getElementById('layout-main');
	var layprefs = document.getElementById('layout-prefs');
	var bodyContent = getElementsByClassName(laymain.getElementsByTagName('div'), 'g_content', true)[0];
	var lis = laytabs.getElementsByTagName('li');
	if (!laymain || !layprefs) return;
	if (!arePrefsShown) { // flushOut the elem
		while(bodyContent.childNodes.length) virtualDiv.appendChild(bodyContent.firstChild);
		bodyContent.appendChild(layprefs);
		layprefs.style.display = '';
		lis[defPrefTab].className = lis[defPrefTab].className.replace(' selected','');
		lis[lis.length - 1].className += ' selected';
		arePrefsShown = true;
	} else {
		layprefs.style.display = 'none';
		while(bodyContent.childNodes.length) laycontent.appendChild(bodyContent.firstChild);
		while(virtualDiv.childNodes.length) bodyContent.appendChild(virtualDiv.firstChild);
		lis[lis.length - 1].className = lis[lis.length - 1].className.replace(' selected','');
		lis[defPrefTab].className += ' selected';
		arePrefsShown = false;
	}
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

/* Function that actualy shows the tooltip
 * @param obj The object that will be base for the tooltip position
 * @param info The AnimeInfo object
 * @param type Type of object we are using
 */
function showInfoWork(obj,info,type) {
	if (!get_info_mw) get_info_mw = 450;
	var minWidth = Number(get_info_mw);
	if (isNaN(minWidth)) minWidth = 450;
	var table = document.createElement('table');
	table.className = type+'Info';
	table.style.minWidth = minWidth + 'px';
	var thead = document.createElement('thead');
	var row = document.createElement('tr');
	var title = document.createElement('span');
	title.className = 'title';
	title.appendChild(document.createTextNode(info.title));
	var cell = createHeader(null, (info.restricted ? 'restricted' : null), title, null, null, 2);
	if (type == 'anime' && info.year) {
		var year = document.createElement('span');
		year.className = 'year';
		year.appendChild(document.createTextNode('('+info.year+')'));
		cell.appendChild(document.createTextNode(' '));
		cell.appendChild(year);
	}
	row.appendChild(cell);
	thead.appendChild(row);
	table.appendChild(thead);
	var tbody = document.createElement('tbody');
	row = document.createElement('tr');
	var img = document.createElement('img');
	img.src = info.picurl;
	img.alt = '['+info.id+']';
	createCell(row, type+'InfoThumb', img);
	var span = document.createElement('span');
	span.innerHTML = info.desc;
	createCell(row, type+'InfoDesc', span);
	tbody.appendChild(row);
	table.appendChild(tbody);
	setTooltip(table, true, minWidth);
}

/* function that finds all icons */
function findAllInfoIcons(acid) {
	var targets = document.getElementsByTagName('a');
	var validTargets = new Array();
	for (var i = 0; i < targets.length; i++) {
		if (targets[i].id == acid) 
			validTargets.push(targets[i]);
	}
	return validTargets;
}

/* Function that shows info (or not) */
function showInfo() {
	var type = 'character';
	if (this.id.indexOf('cinfo_ch') >= 0) type = 'character';
	else if (this.id.indexOf('cinfo_cr') >= 0) type = 'creator';
	else if (this.id.indexOf('ainfo_') >= 0) type = 'anime';
	else { errorAlert('showInfo','info type is unknown ['+this.id+']'); return; }
	var id = Number(this.id.substring((type != 'anime' ? 8 : 6)));
	if (isNaN(id)) { errorAlert('showInfo','id is not a number ['+id+']'); return; }
	var info = null;
	switch (type) {
		case 'character': info = charInfos[id]; break;
		case 'creator': info = creatorInfos[id]; break;
		case 'anime': info = animeInfos[id]; break;
	}
	if (!info) { // fetch data and display later
		setTooltip('please wait while loading data...');
		var targets = findAllInfoIcons(this.id);
		for (var i = 0; i < targets.length; i++) {
			var target = targets[i];
			target.className = target.className.replace('i_mylist_ainfo_greyed','i_mylist_ainfo_loading');
			target.title = '';
			target.onmouseover = showInfo;
			target.onmouseout = hideTooltip;
		}
		switch (type) {
			case 'character': fetchInfoData("characterdescbyid",id); break;
			case 'creator': fetchInfoData("creatordescbyid",id); break;
			case 'anime': fetchInfoData("animedescbyid",id); break;
		}
	} else { // display the data
		showInfoWork(this,info,type);
	}
}

/* Function that takes care of tabs and adds sorting and other stuff to the tabs
 * @param sortingCols Sorting definitions
 * @param tableNames Ids of tables that we are going to process
 * @param skipTables Ids of tables to skip adding info icons (optional, but needs to be set as null if not used)
 * @param collapseThumbnails Should we collapse thumbnails (optional, defaults to false)
 * @param get_info Should we create the information icon (optional, defaults to true)
 */
function handleTables(sortingCols,tableNames,skipTables,collapseThumbnails,get_info) {
	if (!sortingCols || !tableNames) return;
	if (collapseThumbnails == null) collapseThumbnails = false;
	if (get_info == null) get_info = true;
	var tables = new Array();
	for (var t = 0; t < tableNames.length; t++) {
		var tester = document.getElementById(tableNames[t]);
		if (tester) tables.push(tester);
	}
	for (var t = 0; t < tables.length; t++) {
		globalStatus.updateBarWithText('Preparing tabs',parseInt(t+1/tables.length*100),'Total progress: ');
		var table = tables[t];
		var tbody = table.tBodies[0];
		// let's assume that either we have a thead node
		// or if we don't, if the first row of the tbody
		// has "header" in the classname that row is the
		// thead
		var thead = table.getElementsByTagName('thead')[0];
		var headRow = (tbody.rows[0] && tbody.rows[0].className.indexOf('header') >= 0);
		if (!thead && headRow) {
			thead = document.createElement('thead');
			thead.appendChild(tbody.rows[0]);
			table.insertBefore(thead,tbody);
		}
		if (thead) { // apply sorting only if we have a table head (which in turns means we have headers)
			var sortingTable = sortingCols[table.id];
			if (sortingTable == undefined && table.id.indexOf('_') > 0) // first and only fail back i'm trying
				sortingTable = sortingCols[table.id.substring(0,table.id.indexOf('_'))];
			if (sortingTable != undefined) {
				var ths = thead.getElementsByTagName('th');
				var defaultTh = null;
				var defaultSort = null;
				for (var i = 0; i < ths.length; i++) {
					var colDef = sortingTable[ths[i].className];
					if (!colDef) continue;
					ths[i].className += ' '+colDef['type'];
					if (colDef['isDefault']) {
						defaultTh = ths[i];
						defaultSort = 'down';
					}
				}
				init_sorting(table,defaultTh,defaultSort);
			}
		}
		if (skipTables && skipTables.indexOf(table.id) >= 0) continue;
		if (!Number(collapseThumbnails) && !Number(get_info)) continue; // don't do the rest of the stuff
		for (var r = 0; r < tbody.rows.length; r++) {
			var row = tbody.rows[r];
			if (Number(collapseThumbnails)) {
				// add onmouseover/onmouseout effects
				addEventSimple(row, "mouseover", function showImages(event) {
					var images = getElementsByClassName(this.getElementsByTagName('td'), 'image', true);
					for (var i = 0; i < images.length; i++) {
						var imageCell = images[i];
						var img = imageCell.getElementsByTagName('img')[0]; // i'll just get the first img
						if (img) img.style.display = '';
					}
				});
				addEventSimple(row, "mouseout", function showImages(event) {
					var images = getElementsByClassName(this.getElementsByTagName('td'), 'image', true);
					for (var i = 0; i < images.length; i++) {
						var imageCell = images[i];
						var img = imageCell.getElementsByTagName('img')[0]; // i'll just get the first img
						if (img) img.style.display = 'none';
					}
				});
				// collapse images
				var images = getElementsByClassName(row.getElementsByTagName('td'), 'image', true);
				for (var i = 0; i < images.length; i++) {
					var imageCell = images[i];
					var img = imageCell.getElementsByTagName('img')[0]; // i'll just get the first img
					if (img) img.style.display = 'none';
				}
			}
			if (Number(get_info) && divHTMLTOOLTIP != null) {
				var names = getElementsByClassName(row.getElementsByTagName('td'), 'name', true);
				for (var n = 0; n < names.length; n++) {
					var nameCell = names[n];
					var label = nameCell.getElementsByTagName('label')[0];
					var a = (!label ? nameCell.getElementsByTagName('a')[0] : label.getElementsByTagName('a')[0]);
					if (!a) continue;
					nameCell.setAttribute('anidb:sort',a.firstChild.nodeValue);
					var type = null;
					if (a.href.indexOf('creator') >= 0) type = 'creator';
					else if (a.href.indexOf('character') >= 0) type = 'character';
					else if (a.href.indexOf('=anime') >= 0) type = 'anime';
					else continue;
					if (!type) continue;
					var id = a.href.substring(a.href.indexOf('id=')+3);
					var infoIcon = createIcon(null, 'cinfo', 'removeme', showInfo, 'Click to show '+type+' information', 'i_mylist_ainfo_greyed');
					if (type == 'creator' || type == 'character') 	infoIcon.id = 'cinfo_c'+(type == 'character' ? 'h' : 'r')+id;
					else infoIcon.id = 'ainfo_'+id;
					var icons = getElementsByClassName(nameCell.getElementsByTagName('span'),'icons',false)[0];
					if (!icons) {
						icons = document.createElement('span');
						icons.className = 'icons';
					}
					icons.appendChild(infoIcon);
					if (!label) {
						label = document.createElement('label');
						label.appendChild(a);
					}
					nameCell.appendChild(label);
					nameCell.insertBefore(icons,label);
				}
			}
		}
	}
}
