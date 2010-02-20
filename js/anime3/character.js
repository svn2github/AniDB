/* file character page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *
 * version 1.0 (06.02.2008) - Merged Character related scripts
 */
jsVersionArray.push({
	"file":"anime3/character.js",
	"version":"1.0",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Merged Character related scripts"
});

var sortingCols = {
	'stafflist': 	{	"name":{"type":'c_setlatin',"isDefault":true},
						"type":{"type":'c_latin'},
						"eps":{"type":'c_number'},
						"year":{"type":'c_date'},
						"rating":{"type":'c_latin'},
						"credit":{"type":'c_latin'},
						"eprange":{"type":'c_latin'},
						"comment":{"type":'c_latin'}},
	'characterlist': {	"name":{"type":'c_setlatin',"isDefault":true},
						"bloodtype":{"type":'c_latin'},
						"age":{"type":'c_number'},
						"gender":{"type":'c_latin'},
						"entity":{"type":'c_setlatin'},
						"eps":{"type":'c_number'},
						"year":{"type":'c_date'},
						"comment":{"type":'c_latin'},
						"relation":{"type":'c_latin'},
						"ismainseiyuu":{"type":'c_latin'},
						"rating":{"type":'c_setlatin'}},
	'animelist': {		"name":{"type":'c_setlatin',"isDefault":true},
						"type":{"type":'c_latin'},
						"eps":{"type":'c_number'},
						"year":{"type":'c_date'},
						"rating":{"type":'c_setlatin'},
						"relation":{"type":'c_latin'},
						"stats eps":{"type":'c_setlatin'},
						"stats seen":{"type":'c_setlatin'}},
	'seiyuulist': 	{	"name":{"type":'c_setlatin',"isDefault":true},
						"entity":{"type":'c_setlatin'},
						"ismainseiyuu":{"type":'c_latin'},
						"comment":{"type":'c_latin'}},
	'relationslist': {	"name entity":{"type":'c_setlatin',"isDefault":true},
						"name anime":{"type":'c_setlatin'},
						"date added":{"type":'c_date'},
						"date deleted":{"type":'c_date'},
						"owner":{"type":'c_setlatin'},
						"name user added":{"type":'c_setlatin'},
						"name user deleted":{"type":'c_setlatin'},
						"name user":{"type":'c_setlatin'},
						"name seiyuu":{"type":'c_setlatin'},
						"ismainseiyuu":{"type":'c_latin'},
						"rating":{"type":'c_setlatin'},
						"type":{"type":'c_latin'},
						"stats eps":{"type":'c_setlatin'},
						"stats seen":{"type":'c_setlatin'}},
	'characterrellist':{"date day":{"type":'c_date'},
						"name character":{"type":'c_setlatin',"isDefault":true},
						"type crel":{"type":'c_latin'},
						"name character next":{"type":'c_setlatin'},
						"name owner":{"type":'c_setlatin'}},
	'creatorrellist': {	"date":{"type":'c_date',"isDefault":true},
						"name creator":{"type":'c_setlatin'},
						"type crel":{"type":'c_latin'},
						"name creator next":{"type":'c_setlatin'},
						"orderno":{"type":'c_number'},
						"has_ended":{"type":'c_latin'},
						"name user added":{"type":'c_setlatin'}}
};
var tableNames = new Array();
var skipTables = null;
var creatordivs = new Array();
var chardivs = new Array();
var selector = null;
var selector_next = null;
var selector_prev = null;
var addRelInput; // For anime addition to a given char
var addCRelInput; // For character addition to a given anime
var addRelSubmit;
var fieldset;
var table;
var tbody;
var isCreatorPage = false;
var searchString = "";
var aids = new Array();
var cids = new Array();
var crids = new Array();
var useSingleMode = false;
var useFilterMode = true;
var currentIds = new Array();
var guiseinput = null;
var newguiseinput = null;
var searchinput = null;
var magicRows = new Array();

/* ====[ CHARACTER/CREATOR ]====
 * Character page scripts
 */

/* Show a given Entity */
function showEntityDo(index,type) {
	var divs = (type == 'character' ? chardivs : creatordivs);
	for (var i = 0; i < divs.length; i++) {
		var div = divs[i];
		div.className = div.className.replace(/ hide|hide/ig,'');
		div.className += (i != index ? ' hide' : '');
	}
}

/* Show previous Entity in sequence */
function showPrevEntity() {
	var index = selector.selectedIndex;
	if (index == 0) index = selector.options.length; // can't show any more Entities as there are any other Entities to show
	index--;
	selector.selectedIndex = index;
	showEntityDo(index,curPageID);
}

/* Show next Entity in sequence */
function showNextEntity() {
	var index = selector.selectedIndex;
	if (index+1 == selector.options.length) index = -1; // can't show any more Entities as there are any other Entities to show
	index++;
	selector.selectedIndex = index;
	showEntityDo(index,curPageID);
}

/* Show a specific Entity based on it's index */
function showEntity() {
	showEntityDo(this.value,curPageID);
}

/* This function prepares the show=character page for use with my scripts */
function prepPageEntity() {
	var charNames = new Array();
	var divs = null;
	if (curPageID == 'character') {
		chardivs = getElementsByClassName(document.getElementsByTagName('div'), 'g_section characters', true);
		var tbNames = ['characterlist','animelist','seiyuulist'];
		divs = chardivs;
	} else {
		creatordivs = getElementsByClassName(document.getElementsByTagName('div'), 'g_section creators', true);
		var tbNames = ['characterlist','stafflist'];
		divs = creatordivs;
	}
	var ident = 0;
	for (var i = 0; i < divs.length; i++) {
		var div = divs[i];
		var tables = div.getElementsByTagName('table');
		for (var t = 0; t < tables.length; t++) {
			if (tbNames.indexOf(tables[t].id) < 0) continue;
			tables[t].id += '_'+ident;
			tableNames.push(tables[t].id);
			ident++;
		}
		div.className += (i > 0 ? ' hide' : '');
		div.id = 'section_'+i;
		var nameTR = getElementsByClassName(div.getElementsByTagName('tr'),'mainname',true)[0];
		if (nameTR) {
			var text = dig_text(nameTR.getElementsByTagName('td')[0]);
			text = text.substring(0,text.length-1);
			charNames.push(text);
		}
	}
	// clear horizontal rule nodes
	var hrNodes = document.getElementsByTagName('hr');
	while(hrNodes.length) hrNodes[0].parentNode.removeChild(hrNodes[0]);
	// add the selector if we have more than one char
	if (divs && divs.length > 1) {
		var selectorDiv = document.createElement('div');
		selectorDiv.className = 'g_section selector';
		selector_prev = createButton(null,'prev',false,'Previous','button',showPrevEntity, 'prev');
		selector_next = createButton(null,'next',false,'Next','button',showNextEntity, 'next');
		selector = createBasicSelect(null,'char_sel',showEntity);
		for (var i = 0; i < charNames.length; i++)
			createSelectOption(selector, charNames[i], i, 0, null, false);
		selectorDiv.appendChild(selector_prev);
		selectorDiv.appendChild(document.createTextNode(' '));
		selectorDiv.appendChild(selector);
		selectorDiv.appendChild(document.createTextNode(' '));
		selectorDiv.appendChild(selector_next);
		divs[0].parentNode.insertBefore(selectorDiv,divs[0]);
	}
	// add sorting and other info tooltips
	handleTables(sortingCols,tableNames,skipTables,collapseThumbnails,(get_info & 16));
}

/* =====[ ADDCHARANIMEREL ]=====
 * show=addcharanimerel page scripts
 */

/* Function that fetches data
 * @param searchString Usualy the search string unless we are dealing with a search by id search in that case it's the ID
 * @param searchType Type of search
 * @param extraSearchString When searchString is an ID this can be used to pass a filter down Search String (not an ID)
 */
function fetchData(searchString,searchType,extraSearchString) {
	var req = xhttpRequest();
	var allowedTypes = ['characterdesc','creatordesc','animedesc','characterdescbyrel','characterdescbyid','creatordescbyid'];
	if (allowedTypes.indexOf(searchType) < 0) { errorAlert('fetchData','Invalid search type: '+searchType); return; }

	if (searchType != 'characterdescbyrel' && searchType != 'characterdescbyid' && searchType != 'creatordescbyid') {
		if (isLocalHost()) xhttpRequestFetch(req, 'xml/'+searchType+'_'+encodeURI(searchString)+'.xml', parseData);
		else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type='+searchType+'&limit=500&search='+encodeURI(searchString), parseData);
	} else {
		if (isLocalHost()) {
			xhttpRequestFetch(req, 'xml/'+searchType+'_'+encodeURI(searchString)+'.xml', parseData);
		} else
			xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type='+searchType+'&limit=500&id='+encodeURI(searchString)+(extraSearchString ? '&search='+encodeURI(extraSearchString) : ''), parseData);
	}
}

/* XMLHTTP RESPONSE parser
 * @param xmldoc Response document
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { errorAlert('parseData','no root node'); return; }
	var allowCurIds = false;
	var descNodes = root.getElementsByTagName('animedesc');
	if (!descNodes.length) descNodes = root.getElementsByTagName('characterdesc');
	if (!descNodes.length) descNodes = root.getElementsByTagName('creatordesc');
	if (!descNodes.length) descNodes = root.getElementsByTagName('characterdescbyrel');
	if (!descNodes.length) { allowCurIds = true; descNodes = root.getElementsByTagName('characterdescbyid'); }
	if (!descNodes.length) { allowCurIds = true; descNodes = root.getElementsByTagName('creatordescbyid'); }
	if (!descNodes.length) {
		alert('Search for "'+searchString+'" resulted in no matches.');
		if (curPageID == 'addcreator' || curPageID == 'addcharacter') {
			searchinput.value = "search";
			searchinput.onclick = doEntitySearch;
		}
		return;
	}
	var filtered = 0;
	aids = new Array();
	cids = new Array();
	crids = new Array();
	for (var d = 0; d < descNodes.length; d++) {
		var descNode = new CInfo(descNodes[d]);
		var ids = (descNode.type == 'anime' ? aids : (descNode.type == 'character' ? cids : crids));
		var descs = (descNode.type == 'anime' ? animeInfos : (descNode.type == 'character' ? charInfos : creatorInfos));
		if (!allowCurIds && currentIds.indexOf(descNode.id) >= 0) { filtered++; continue; }
		if (ids.indexOf(descNode.id) < 0) ids.push(descNode.id);
		if (!descs[descNode.id]) {
			descs[descNode.id] = descNode;
			if ((curPageID == 'addcreator' && descNodes[d].nodeName.toLowerCase() == 'creatordescbyid') ||
				(curPageID == 'addcharacter' && descNodes[d].nodeName.toLowerCase() == 'characterdescbyid'))
				guiseinput.value = descNode.title;
		} else
			continue; // a bit pointless, i know :P
	}
	if (!aids.length && !cids.length && !crids.length) {
		alert('Search for "'+searchString+'" resulted in no matches.'+(filtered ? '\n('+filtered+' filtered matches)' : ''));
		if (curPageID == 'addcreator' || curPageID == 'addcharacter') {
			searchinput.value = "search";
			searchinput.onclick = doEntitySearch;
		}
		return;
	}
	//alert("Results:\ndescNodes: "+descNodes.length+" ["+aids.length+"|"+cids.length+"|"+crids.length+"]");
	if (!(curPageID == 'addcreator' && descNodes[0].nodeName == 'creatordescbyid') &&
				!(curPageID == 'addcharacter' && descNodes[0].nodeName == 'characterdescbyid'))
	{
		showResults();
		handleTables(new Object(),['resultslist'],null,collapseThumbnails,false);
	}
	if (curPageID == 'addcreator' || curPageID == 'addcharacter') {
		searchinput.value = "search";
		searchinput.onclick = doEntitySearch;
	}
}

/* Function that starts a search */
function doSearch(type) {
	if (!type || type == 'anime') {
		searchString = addRelInput.value;
		if (searchString.length < 1) { alert('Error:\nSearch string has an insufficient number of characters.'); return; }
		fetchData(searchString,'animedesc');
		useFilterMode = false;
	} else {
		searchString = addCRelInput.value;
		if (searchString.length < 1) { alert('Error:\nSearch string has an insufficient number of characters.'); return; }
		fetchData(searchString,type);
		useSingleMode = (curPageID.indexOf('add') >= 0 && curPageID.indexOf('rel') >=0);
		useFilterMode = false;
	}
}

/* Exectutes a related search */
function doSearchRelated(type,id) {
	if (!(type && id)) { errorAlert('doSearchRelated','no type set'); return; }
	searchString = addCRelInput.value;
	//if (searchString.length < 3) { alert('Error:\nSearch string has an insufficient number of characters.'); return; }
	useFilterMode = true;
	//if (searchString.length < 3)
		useSingleMode = false;
	//else
	//	useSingleMode = true;
	fetchData(id,type,searchString);
}

/* Function that starts a char search */
function doSearchChar() { doSearch('characterdesc'); }
/* Function that starts a char search */
function doSearchCreator() { doSearch('creatordesc'); }

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

/* Function that shows the results of the search
 * @param single If true, will use radio buttons instead of checkboxes
 */
function showResults(single,filter) {
	if (single == null) single = useSingleMode;
	if (single != undefined && filter == null) filter = useFilterMode;
	if (!table) { errorAlert('showResults','no table'); return; }
	var ids;
	var itype;
	var infos;
	if (aids.length > 0) { ids = aids; itype = 'anime'; infos = animeInfos;}
	else if (cids.length > 0) { ids = cids; itype = 'character'; infos = charInfos;}
	else if (crids.length > 0) { ids = crids; itype = 'creator'; infos = creatorInfos;}
	else return; // bastards
	table.id = "resultslist";
	var caption = table.getElementsByTagName('caption')[0];
	if (caption) caption.firstChild.nodeValue = itype.charAt(0).toUpperCase()+itype.substring(1)+"(s) matching \""+searchString+"\"";
	var tbody = table.getElementsByTagName('tbody')[0];
	if (!tbody) {
		tbody = document.createElement('tbody');
		table.appendChild(tbody);
	}
	// clear table
	while (tbody.childNodes.length)
		tbody.removeChild(tbody.firstChild);
	// Create table head
	var thead = table.getElementsByTagName('thead')[0];
	if (!thead) {
		thead = document.createElement('thead');
		var row = document.createElement('tr');
		row.className = 'header';
		createHeader(row, 'check', 'X');
		if (!HIDETHUMBNAILS)
			createHeader(row, 'image', 'Image');
		createHeader(row, 'name', 'Name'); // Matched Name
		thead.appendChild(row);
		table.insertBefore(thead,tbody);
	}

	// now figure out the names of the checkboxes / radio buttons
	var ckNames = {
		'addcharcharrel':{'single':"addccrel.next",'multi':"addccrel.nextchar[]",'func':null},
		'addcreatorcreatorrel':{'single':"addccrel.next",'multi':"addccrel.nextcreator[]",'func':null},
		'addcharanimerel':{'single':"addcarel.charid[]",'multi':"addcarel.charid[]",'func':null},
		'addcreator':{'single':"creator",'multi':"creator[]",'func':function setCreatorId() {
				newguiseinput.value = this.value;
				guiseinput.value = creatorInfos[this.value].title;
				table.style.display = "none";
			}},
		'addcharacter':{'single':"character",'multi':"character[]",'func':function setCharId() {
				newguiseinput.value = this.value;
				guiseinput.value = charInfos[this.value].title;
				table.style.display = "none";
			}
		}
	};
	var oddRow = 1;
	// var create tbody rows
	for (var i = 0; i < ids.length; i++) {
		var desc = infos[ids[i]];
		// skip existing ids
		if (currentIds.indexOf(desc.id) >= 0) continue;
		var row = document.createElement('tr');
		row.className = (oddRow % 2 ? 'g_odd' : '');
		// checkbox
		var ck;
		if (!single) {
			ck = createCheckbox(ckNames[curPageID]['multi'],false);
			ck.value = desc.id;
			ck.onclick = ckNames[curPageID]['func'];
		} else {
			ck = createButton(ckNames[curPageID]['single'],null,false,desc.id,'radio',ckNames[curPageID]['func']);
		}
		if (ids.length == 1) {
			ck.checked = true;
			if (curPageID == 'addcreator' || curPageID == 'addcharacter') {
				newguiseinput.value = desc.id;
				guiseinput.value = desc.title;
			}
		}
		createCell(row, 'check', ck);
		if (!HIDETHUMBNAILS) {
			// image
			var img = document.createElement('img');
			img.src = desc.thumbnail;
			img.alt = "thumbnail";
			img.title = desc.title;
			createCell(row, 'image', img);
		}
		// matched name
		// first, let's create the icons i want
		var div = document.createElement('div');
		div.className = 'icons';
		var existTitles = desc.titles.length;
		var existMatch = false;
		var altTitleSpan = null;
		// hack for ' converted to ` conversion
		if (searchString.indexOf("'") >= 0) searchString = searchString.replace(/\'/mgi,"`");
		if (existTitles) {
			altTitleSpan = document.createElement('ul');
			altTitleSpan.className = 'alt_titles';
			altTitleSpan.style.display = "none";
			var types = (itype == 'anime' ? ['Official','Synonym','Short'] : ['Official','Alias','Short','Nick','Maiden','other']);
			for (var tt = 0; tt < types.length; tt++) {
				var type = types[tt];
				var titles = desc.getTitles(type);
				if (!titles) continue;
				// Group titles
				var groups = new Array();
				for (var t = 0; t < titles.length; t++) {
					var title = titles[t]['title'];
					var lang = titles[t]['lang'];
					var verified = titles[t]['verified'];
					var si = title.toLowerCase().indexOf(searchString.toLowerCase());
					if (si < 0) continue; // title provides no match, so ignore it
					if (!groups[title]) groups[title] = new Array();
					if (!groups[title][Number(verified)]) groups[title][Number(verified)] = new Array();
					groups[title][Number(verified)].push(lang);
					existMatch = true;
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
			if (altTitleSpan.childNodes && altTitleSpan.childNodes.length)
				div.appendChild(createIcon(null, '[+]', 'removeme', expandTitles, 'click to expand all titles matched', 'i_plus'));
		}
		if (get_info & 16) {
			var infoIcon = createIcon(null, 'Click to show '+type+' information', 'removeme', null, null, 'i_mylist_ainfo');
			//alert('['+(i+1)+'/'+ids.length+'] type: '+type+'/'+desc.type);
			if (itype == 'creator' || itype == 'character') infoIcon.id = 'cinfo_c'+(itype == 'character' ? 'h' : 'r')+desc.id;
			else infoIcon.id = 'ainfo_'+desc.id;
			infoIcon.onmouseover = showInfo;
			infoIcon.onmouseout = hideTooltip;
			div.appendChild(infoIcon);
		}
		var cell = createCell(null, 'name', div);
		// now, create the titles and what not.
		// first, main title, always present, so default showing
		var mainTitleSpan = document.createElement('span');
		mainTitleSpan.className = 'title main';
		var b = document.createElement('b');
		var a = document.createElement('a');
		var hreflink = "";
		switch (itype) {
			case 'anime': hreflink = itype+'&aid='; break;
			case 'character': hreflink = itype+'&charid='; break;
			case 'creator': hreflink = itype+'&creatorid='; break;
		}
		a.href = 'animedb.pl?show='+hreflink+desc.id;
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
			existMatch = true;
		} else a.appendChild(document.createTextNode(desc.title+' (Main)'));
		mainTitleSpan.appendChild(a);
		cell.appendChild(mainTitleSpan);
		// now i cycle other tiles matched for this anime
		if (existTitles && altTitleSpan.childNodes && altTitleSpan.childNodes.length)
			cell.appendChild(altTitleSpan);
		row.appendChild(cell);
		if (filter && !existMatch) continue;
		tbody.appendChild(row);
		oddRow++;
	}
	if (curPageID != 'addcharacter' && curPageID != 'addcreator') {
		// create tfoot
		var tfoot = table.getElementsByTagName('tfoot')[0];
		if (!tfoot) {
			tfoot = document.createElement('tfoot');
			row = document.createElement('tr');
			row.className = 'action';
			var buttonName = 'add.doadd';
			if (curPageID == 'addcharcharrel' || curPageID == 'addcreatorcreatorrel') buttonName = 'addccrel.do.add';
			createCell(row, 'value', createButton(buttonName,null,false,'Add relation','submit'), null, 3);
			tfoot.appendChild(row);
			table.appendChild(tfoot);
		}
	}
}

/* Prepares the page for my scripts */
function prepPageAddCharAnimeRel() {
	var div = document.getElementById('addrelform');
	if (!div) { errorAlert('prepPageAddCharAnimeRel','no matching div'); return; }
	fieldset = div.getElementsByTagName('fieldset')[0];
	table = div.getElementsByTagName('table')[0];
	var inputs = div.getElementsByTagName('input');
	addRelInput = getElementsByName(inputs, 'addcarel.aname', false)[0];
	addCRelInput = getElementsByName(inputs, 'addcarel.charname', false)[0];
	addRelSubmit = getElementsByName(inputs, 'add.doadd', false)[0];
	// update curtable
	var tables = new Array();
	var reldivs = getElementsByClassName(document.getElementsByTagName('div'),'g_section reltable',true);
	for (var i = 0; i < reldivs.length; i++) {
		var curdiv = reldivs[i];
		var tb = curdiv.getElementsByTagName('table')[0];
		if (tb) {
			tb.id = 'relationslist_'+i;
			tables.push(tb.id);
		}
	}
	// @TODO: when table cells have proper class names add sorting support
	handleTables(sortingCols,tables,null,collapseThumbnails,(get_info & 16));
	if (!addRelSubmit || (!addRelInput && !addCRelInput)) { errorAlert('prepPage','no valid inputs'); return; }
	var newSubmitClone = createButton(addRelSubmit.name,addRelSubmit.id,false,addRelSubmit.value,'button',(addRelInput ? doSearch : doSearchChar), null);
	addRelSubmit.parentNode.replaceChild(newSubmitClone,addRelSubmit);
	addRelSubmit = newSubmitClone;
}

/* =====[ ADDENTITYENTITYREL ]=====
 * show=addcharcharrel page scripts
 * show=addcreatorcreatorrel page scripts
 */

/* Function that starts a related char search */
function doSearchRelatedChar() { doSearchRelated('characterdescbyrel',charid); }
/* Function that starts a related creator search */
function doSearchRelatedCreator() { doSearchRelated('creatordescbyrel',charid); }

/* Prepares the page for my scripts */
function prepPageAddEntityEntityRel() {
	var div = document.getElementById('addrelform');
	if (!div) { errorAlert('prepPageAddEntityEntityRel','no matching div'); return; }
	fieldset = div.getElementsByTagName('fieldset')[0];
	var inputs = div.getElementsByTagName('input');
	var relinputid,charinputid, searchFunc, relatedSearchFunc, ginfo, tableid, reldivs;
	switch(curPageID) {
		case 'addcharcharrel':
			relinputid = 'addccrel.charid';
			charinputid = 'charid';
			searchFunc = doSearchChar;
			relatedSearchFunc = doSearchRelatedChar;
			ginfo = 16;
			tableid = 'characterrellist_';
			reldivs = 'g_section addcharcharrel_entries';
			break;
		case 'addcreatorcreatorrel':
			relinputid = 'addccrel.creatorid';
			charinputid = 'creatorid';
			searchFunc = doSearchCreator;
			relatedSearchFunc = doSearchRelatedCreator;
			ginfo = 32;
			tableid = 'creatorrellist_';
			reldivs = 'g_section addcreatorcreatorrel_entries';
			break;
		default:
			errorAlert('prepPageAddEntityEntityRel','no matching page');
			return;
	}
	addRelInput = getElementsByName(inputs, relinputid, false)[0];
	charid = getElementsByName(inputs, charinputid, false)[0].value;
	currentIds.push(Number(charid));
	addCRelInput = getElementsByName(inputs, 'search', false)[0];
	showRelatedInput = getElementsByName(inputs, 'do.showrelated', false)[0];
	searchInput = getElementsByName(inputs, 'do.search', false)[0];
	var tables = new Array();
	var reldivs = getElementsByClassName(document.getElementsByTagName('div'),reldivs,true);
	for (var i = 0; i < reldivs.length; i++) {
		var curdiv = reldivs[i];
		var tb = curdiv.getElementsByTagName('table')[0];
		if (tb) {
			tb.id = tableid+i;
			tables.push(tb.id);
		}
	}
	// @TODO: when table cells have proper class names add sorting support
	handleTables(sortingCols,tables,null,collapseThumbnails,(get_info & ginfo));
	if (!showRelatedInput && !searchInput) return;
	if (showRelatedInput) {
		var showRelatedClone = createButton(showRelatedInput.name,showRelatedInput.id,false,showRelatedInput.value,'button',relatedSearchFunc,null);
		showRelatedInput.parentNode.replaceChild(showRelatedClone,showRelatedInput);
	}
	if (searchInput) {
		var searchInputClone = createButton(searchInput.name,searchInput.id,false,searchInput.value,'button',searchFunc,null);
		searchInput.parentNode.replaceChild(searchInputClone,searchInput);
	}
	// append results table to the page
	table = document.createElement('table');
	table.id = 'resultslist';
	var caption = document.createElement('caption');
	caption.appendChild(document.createTextNode(''));
	table.appendChild(caption);
	fieldset.parentNode.appendChild(table);
}

/* =====[ ADDCHARACTER/ADDCREATOR ]=====
 * show=addcharacter/addcreator page scripts
 */

/* Function that handles search */
function doEntitySearch() {
	if (guiseinput.value != "" && guiseinput.value == searchString) { table.style.display = ""; return; }
	searchString = guiseinput.value;
	if (searchString.length < 1) { alert('Error:\nSearch string has an insufficient number of characters.'); return; }
	searchinput.value = "wait....";
	searchinput.onclick = null;
	fetchData(searchString,!isCreatorPage ? "characterdesc" : "creatordesc");
	//useFilterMode = false;
	useSingleMode = true;
}

/* Function that changes the layout of the page */
function changeLayout() {
	var type = Number(this.value);
	if (!isCreatorPage) { // character page
		switch(type) {
			case 1: // character
				magicRows['gender'].style.display = '';
				magicRows['bloodtype'].style.display = '';
				magicRows['height'].style.display = '';
				magicRows['weight'].style.display = '';
				magicRows['bust'].style.display = '';
				magicRows['waist'].style.display = '';
				magicRows['hips'].style.display = '';
				break;
			case 2: // mecha
				magicRows['gender'].style.display = 'none';
				magicRows['bloodtype'].style.display = 'none';
				magicRows['height'].style.display = '';
				magicRows['weight'].style.display = '';
				magicRows['bust'].style.display = 'none';
				magicRows['waist'].style.display = 'none';
				magicRows['hips'].style.display = 'none';
				break;
			case 4: // vessel
				magicRows['gender'].style.display = 'none';
				magicRows['bloodtype'].style.display = 'none';
				magicRows['height'].style.display = '';
				magicRows['weight'].style.display = '';
				magicRows['bust'].style.display = 'none';
				magicRows['waist'].style.display = 'none';
				magicRows['hips'].style.display = 'none';
				break;
			default: // organization and future types?
				magicRows['gender'].style.display = 'none';
				magicRows['bloodtype'].style.display = 'none';
				magicRows['height'].style.display = 'none';
				magicRows['weight'].style.display = 'none';
				magicRows['bust'].style.display = 'none';
				magicRows['waist'].style.display = 'none';
				magicRows['hips'].style.display = 'none';
				break;
		}
	} else { // creator page
		switch(type) {
			case 1: // person
				magicRows['gender'].style.display = '';
				break;
			default: // company, colaboration
				magicRows['gender'].style.display = 'none';
				break;
		}
	}
	// repaint stripes (i guess someone will complain about this, so i better do it..
	if (!tbody) { errorAlert('changeLayout',"tbody is missing"); return; }
	var rows = tbody.rows;
	var k = 0;
	for (var i = 0; i < rows.length; i++) {
		var row = rows[i];
		if (row.style.display == 'none') continue;
		row.className = row.className.replace('g_odd ','');
		row.className = (k % 2 ? '' : 'g_odd ') + row.className;
		k++;
	}
}

/* Prepares the page for my scripts */
function prepPageAddEntity() {
	isCreatorPage = (curPageID == 'addcreator');
	var div = getElementsByClassName(document.getElementsByTagName('div'), 'g_definitionlist g_section '+(!isCreatorPage ? 'character' : 'creator') +'_form', false)[0];
	if (!div) { errorAlert('prepPage',"no div found, this might not be an error"); return; }
	var ttable = div.getElementsByTagName('table')[0];
	if (!ttable) { errorAlert('prepPage',"no table found"); return; }
	// find current id of the entity to ignore it
	var fieldset = getElementsByClassName(document.getElementsByTagName('div'), 'g_section '+(!isCreatorPage ? 'character' : 'creator') +'_add', false)[0];
	if (fieldset) {
		fieldset = fieldset.getElementsByTagName('fieldset')[0];
		if (fieldset) {
			var cidinput = getElementsByName(fieldset.getElementsByTagName('input'), (!isCreatorPage ? 'charid' : 'creatorid'), false)[0];
			if (cidinput) currentIds.push(Number(cidinput.value));
		}
	}
	tbody = ttable.tBodies[0];
	var rows = tbody.rows;
	// create a pretty row array with indexes and all
	for (var i = 0; i < rows.length; i++) {
		var row = rows[i];
		var name = row.className;
		name = name.replace('g_odd ','');
		name = name.split(' ')[0];
		magicRows[name] = row; // magic rows will have rows indexed by the first item in the classname
	}
	// okay, now that i have done that i can do some magic
	var select = magicRows['type'].getElementsByTagName('select')[0];
	if (!select) { errorAlert('prepPage',"no select found"); return; }
	select.onchange = changeLayout;
	select.onchange(); // run the script to change the layout in case it's needed
	// okay, next bit, the guise thingie
	guiseinput = magicRows['parent'].getElementsByTagName('input')[0];
	if (!guiseinput) { errorAlert('prepPage',"no guise input"); return; }
	// create a copy of this input and hide it (this is an IE hack btw)
	newguiseinput = createTextInput(guiseinput.name,5,false,true,guiseinput.maxlength,guiseinput.value);
	guiseinput.parentNode.insertBefore(newguiseinput,guiseinput);
	guiseinput.name = "";
	guiseinput.value = "";
	guiseinput.size = guiseinput.size;
	// this will make sure that if the name thingie is cleared also will the actual place where the id goes
	guiseinput.onkeyup = function checkBlank() { if (this.value == "") newguiseinput.value = ""; };
	guiseinput.onchange = function checkBlank() { if (this.value == "") newguiseinput.value = ""; };
	if (newguiseinput.value != "" && !isNaN(Number(newguiseinput.value))) {
		var curid = Number(newguiseinput.value);
		currentIds.push(curid);
		if (!isCreatorPage) fetchData(curid,"characterdescbyid");
		else fetchData(curid,"creatordescbyid");
	}
	searchinput = createButton('do.search','do.search',false,"search","button",doEntitySearch,null);
	guiseinput.parentNode.insertBefore(document.createTextNode(' '),guiseinput.nextSibling);
	guiseinput.parentNode.insertBefore(searchinput,guiseinput.nextSibling);
	guiseinput.parentNode.insertBefore(document.createTextNode(' '),searchinput);
	// append results table to the page
	table = document.createElement('table');
	table.id = 'resultslist';
	var caption = document.createElement('caption');
	caption.appendChild(document.createTextNode(''));
	table.appendChild(caption);
	guiseinput.parentNode.appendChild(table);
}

/* =====[ ALL PAGES ]=====
 * Common scripts
 */

function prepPage() {
	var uriObj = parseURI();
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	initTooltips();

	switch(curPageID) {
		case 'creator':
		case 'creatortext':
		case 'character': prepPageEntity(); break;
		case 'addcreator':
		case 'addcharacter': prepPageAddEntity(); break;
		case 'addcharanimerel': prepPageAddCharAnimeRel(); break;
		case 'addcharcharrel':
		case 'addcreatorcreatorrel': prepPageAddEntityEntityRel(); break;
		default:
			errorAlert('prepPage',"unknown page identifier ("+curPageID+"), will not continue.");
			return;
	}
}

// hook up the window onload event
addLoadEvent(prepPage);
