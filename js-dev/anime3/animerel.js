/* Add Relation Functions
 * @author lrudor (lrudor@gmail.com)
		   some code derived from work done by PetriW, Rar and fahrenheit at anidb
 * version 1 (14.04.2009)
 */
jsVersionArray.push({
	"file": "anime3/animerel.js",
	"version": "1",
	"revision":"$Revision$",
	"date":"$Date::						   $",
	"author":"$Author$"
});

var sortingCols = {
	'animerellist': {
		"date day": {"type": 'c_date'},
		"name anime next": {"type": 'c_setlatin',"isDefault": true},
		"type crel": {"type": 'c_latin'},
		"name anime": {"type": 'c_setlatin'},
		"name owner": {"type": 'c_setlatin'}
	}
}

var table;
var Anime;
var addARelInput
var addRelInput;
var aid;
var currentIds = new Array();
var table;

/* Function that fetches data */
function fetchData(searchString, searchType) {
	var req = xhttpRequest();
	var allowedTypes = ['characterdesc', 'creatordesc', 'animedesc', 'characterdescbyrel', 'characterdescbyid', 'creatordescbyid'];
	if (allowedTypes.indexOf(searchType) < 0) {
		errorAlert('fetchData', 'Invalid search type: ' + searchType);
		return;
	}

	if ('' + window.location.hostname == '') {
		xhttpRequestFetch(req, 'xml/'+ searchType +'_' + searchString + '.xml', parseData);
	} else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=' + searchType + '&search=' + encodeURI(searchString), parseData);
}

function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) {
		errorAlert('parseData', 'no root node');
		return;
	}
	var descNodes = root.getElementsByTagName('animedesc');
	if (!descNodes.length) {
		alert('Search for "' + searchString + '" resulted in no matches.');
		return;
	}
	aids = new Array();
	for (var d = 0; d < descNodes.length; d++) {
		var descNode = new CInfo(descNodes[d]);
		if (currentIds.indexOf(descNode.id) < 0) {
			var ids = aids;
			var descs = animeInfos;
			if (ids.indexOf(descNode.id) < 0) ids.push(descNode.id);
			if (!descs[descNode.id]) {
				descs[descNode.id] = descNode;
			} else continue; // a bit pointless, i know :P
		}
	}
	showResults();
	handleTables(new Object(), ['resultslist'], null, collapseThumbnails, false);
}
/* Function that expands all the titles for a given anime */
function expandTitles() {
	var node = this.parentNode;
	while (node.nodeName.toLowerCase() != 'td') node = node.parentNode;
	var altTitles = getElementsByClassName(node.getElementsByTagName('ul'), 'alt_titles', false)[0];
	if (!altTitles) {
		errorAlert('expandTitles', 'could not find alternative titles span');
		return;
	}
	if (this.className.indexOf('i_plus') >= 0) {
		this.className = this.className.replace('i_plus', 'i_minus');
		this.title = 'click to fold all titles matched';
		this.alt = this.firstChild.firstChild.nodeValue = '[-]';
		altTitles.style.display = '';
	} else {
		this.className = this.className.replace('i_minus', 'i_plus');
		this.title = 'click to expand all titles matched';
		this.alt = this.firstChild.firstChild.nodeValue = '[+]';
		altTitles.style.display = 'none';
	}
}

function showResults(single, filter) {
	if (single == null) single = useSingleMode;
	if (single != undefined && filter == null) filter = useFilterMode;
	if (!table) {
		errorAlert('showResults', 'no table');
		return;
	}
	var ids;
	var itype;
	var infos;
	if (aids.length > 0) {
		ids = aids;
		itype = 'anime';
		infos = animeInfos;
	} else return;
	table.id = "resultslist";
	var caption = table.getElementsByTagName('caption')[0];
	if (caption) caption.firstChild.nodeValue = itype.charAt(0).toUpperCase() + itype.substring(1) + "(s) matching \"" + searchString + "\"";
	var tbody = table.getElementsByTagName('tbody')[0];
	if (!tbody) {
		tbody = document.createElement('tbody');
		table.appendChild(tbody);
	}
	// clear table
	while (tbody.childNodes.length) tbody.removeChild(tbody.firstChild);
	// Create table head
	var thead = table.getElementsByTagName('thead')[0];
	if (!thead) {
		thead = document.createElement('thead');
		var row = document.createElement('tr');
		row.className = 'header';
		createHeader(row, 'check', 'X');
		if (!HIDETHUMBNAILS) createHeader(row, 'image', 'Image');
		createHeader(row, 'name', 'Name'); // Matched Name
		thead.appendChild(row);
		table.insertBefore(thead, tbody);
	}

	// now figure out the names of the checkboxes / radio buttons
	var ckNames = {
		'addseq': {
			'single': "adds.next",
			'multi': "adds.next[]",
			'func': null
		}
	}
	var oddRow = 1;
	// var create tbody rows
	for (var i = 0; i < ids.length; i++) {
		var desc = infos[ids[i]];
		// skip existing ids
		if (currentIds.indexOf(desc.id) >= 0) continue;
		var row = document.createElement('tr');
		row.className = (oddRow % 2 ? 'g_odd': '');
		// checkbox
		var ck;
		if (!single) {
			ck = createCheckbox(ckNames[curPageID]['multi'], false);
			ck.value = desc.id;
			ck.onclick = ckNames[curPageID]['func'];
		} else {
			ck = createButton(ckNames[curPageID]['single'], null, false, desc.id, 'radio', ckNames[curPageID]['func']);
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
		if (existTitles) {
			altTitleSpan = document.createElement('ul');
			altTitleSpan.className = 'alt_titles';
			altTitleSpan.style.display = "none";
			var types = (itype == 'anime' ? ['Official', 'Synonym', 'Short'] : ['Official', 'Alias', 'Short', 'Nick', 'Maiden', 'other']);
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
							span.className = 'title ' + type.toLowerCase();
							var b = document.createElement('b');
							var icons = document.createElement('div');
							icons.className = 'icons';
							for (var l = 0; l < groups[title][v].length; l++) {
								var lang = groups[title][v][l];
								icons.appendChild(createIcon(null, '[' + lang + ']', null, null, 'Language: ' + languageMap[lang]['name'], 'i_audio_' + lang));
							}
							if (v) icons.appendChild(createIcon(null, '[V]', null, null, 'these title are considered to be correct for the corresponding language(s)', 'i_verified'));
							span.appendChild(icons);
							var si = title.toLowerCase().indexOf(searchString.toLowerCase());
							var firstBlock = document.createTextNode(title.substring(0, si));
							var middleBlock = document.createTextNode(title.substr(si, searchString.length));
							var lastBlock = document.createTextNode(title.substring(si + searchString.length, title.length));
							span.appendChild(firstBlock);
							b.appendChild(middleBlock);
							span.appendChild(b);
							span.appendChild(lastBlock);
							span.appendChild(document.createTextNode(' (' + type + ')'));
							altTitleSpan.appendChild(span);
						}
					}
				}
			}
			if (altTitleSpan.childNodes && altTitleSpan.childNodes.length) div.appendChild(createIcon(null, '[+]', 'removeme', expandTitles, 'click to expand all titles matched', 'i_plus'));
		}
		if (get_info & 1) {
			var infoIcon = createIcon(null, 'Click to show ' + type + ' information', 'removeme', null, null, 'i_mylist_ainfo');
			//alert('['+(i+1)+'/'+ids.length+'] type: '+type+'/'+desc.type);
			infoIcon.id = 'ainfo_' + desc.id;
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
		a.href = 'animedb.pl?show=' + itype + '&aid=' + desc.id;
		var si = desc.title.toLowerCase().indexOf(searchString.toLowerCase());
		if (si >= 0) {
			var firstBlock = document.createTextNode(desc.title.substring(0, si));
			var middleBlock = document.createTextNode(desc.title.substr(si, searchString.length));
			var lastBlock = document.createTextNode(desc.title.substring(si + searchString.length, desc.title.length));
			a.appendChild(firstBlock);
			b.appendChild(middleBlock);
			a.appendChild(b);
			a.appendChild(lastBlock);
			a.appendChild(document.createTextNode(' (Main)'));
			existMatch = true;
		} else a.appendChild(document.createTextNode(desc.title + ' (Main)'));
		mainTitleSpan.appendChild(a);
		cell.appendChild(mainTitleSpan);
		// now i cycle other tiles matched for this anime
		if (existTitles && altTitleSpan.childNodes && altTitleSpan.childNodes.length) cell.appendChild(altTitleSpan);
		row.appendChild(cell);
		if (filter && !existMatch) continue;
		tbody.appendChild(row);
		oddRow++;
	}
	// create tfoot
	var tfoot = table.getElementsByTagName('tfoot')[0];
	if (!tfoot) {
		tfoot = document.createElement('tfoot');
		row = document.createElement('tr');
		row.className = 'action';
		var buttonName = 'adds.do.add';
		createCell(row, 'value', createButton(buttonName, null, false, 'Add relation', 'submit'), null, 3);
		tfoot.appendChild(row);
		table.appendChild(tfoot);
	}
}

function doSearchAnime() {
	searchString = addARelInput.value;
	if (searchString.length < 3) {
		alert('Error:\nSearch string has an insufficient number of characters.');
		return;
	}
	fetchData(searchString, 'animedesc');
	useSingleMode = true;
	useFilterMode = false;
}

function prepPageAddAnimeAnimeRel() {
	var div = document.getElementById('addrelform');
	if (!div) {
		errorAlert('prepPageAddAnimeAnimeRel', 'no matching div');
		return;
	}
	fieldset = div.getElementsByTagName('fieldset')[0];
	var inputs = div.getElementsByTagName('input');
	addRelInput = getElementsByName(inputs, 'adds.aid', false)[0];
	aid = getElementsByName(inputs, 'aid', false)[0].value;
	currentIds.push(Number(aid));
	addARelInput = getElementsByName(inputs, 'search', false)[0];
	searchInput = getElementsByName(inputs, 'do.search', false)[0];
	var tables = new Array();
	var reldivs = getElementsByClassName(document.getElementsByTagName('div'), 'g_section addseq_entries', true);
	for (var i = 0; i < reldivs.length; i++) {
		var curdiv = reldivs[i];
		var tb = curdiv.getElementsByTagName('table')[0];
		if (tb) {
			tb.id = 'animerellist_' + i;
			tables.push(tb.id);
			//quickly check for header class in the first tr -- possibly get this hard codded
			var count = document.getElementById('animerellist_' + i).getElementsByTagName('tr');
			count[0].setAttribute("class", "header");
		}
	}
	//go grab added id's
	var tmp = document.getElementById('animerellist_0');
	var tmp_rows = tmp.getElementsByTagName('tr');
	//list of rows
	for (var i = 1; i < tmp_rows.length; i++) {
		var cell = getElementsByClassName(tmp_rows[i].getElementsByTagName('td'), 'name anime next', true)[0];
		var a = cell.getElementsByTagName('a')[0];
		if (!a) break;
		currentIds.push(Number(a.href.substring(a.href.indexOf('id=') + 3)));
	}
	// @TODO: when table cells have proper class names add sorting support
	handleTables(sortingCols, tables, null, collapseThumbnails, (get_info & 1));

	if (!searchInput) return;
	if (searchInput) {
		var searchInputClone = createButton(searchInput.name, searchInput.id, false, searchInput.value, 'button', doSearchAnime, null);
		searchInput.parentNode.replaceChild(searchInputClone, searchInput);
	}
	// append results table to the page
	table = document.createElement('table');
	table.id = 'resultslist';
	var caption = document.createElement('caption');
	caption.appendChild(document.createTextNode(''));
	table.appendChild(caption);
	fieldset.parentNode.appendChild(table);
}

function prepPage() {
	/* This function prepares the page for use with my scripts */
	createPreferencesTable('global');
	initTooltips();
	switch (curPageID) {
		case 'addseq': prepPageAddAnimeAnimeRel(); break;
		default:
			errorAlert('prepPage', "unknown page identifier (" + curPageID + "), will not continue.");
			return;
	}
}
// hook up the window onload event
addLoadEvent(prepPage);