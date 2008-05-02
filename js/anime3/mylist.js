/* file mylist interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (03.04.2008) - Initial version
 */

// GLOBALs //

var uid;						// userID
var ruid;						// for slave lists (needs testing)
var mod;						// isMod
var anime;						// last parsed anime
var animes = new Array();		// stored by aid
var animeOrder = new Array();	// animes ordered in db way (link to aid)
var groups = new Array();		// stored by gid
var aGroups = new Array();		// stored by agid (gid to link groups)
var mylist = new Array();		// stored by fid
var episodes = new Array();		// stored in eid
var epOrder = new Array();		// episodes ordered in db way (link to eid)
var files = new Array();		// stored by fid
var epQueue = new Array();		// episode process queue
var hiddenGroups = 0;
// settings
var animeTitleLang = '';
var animeAltTitleLang = 'en';
var episodeTitleLang = '';
var episodeAltTitleLang = 'en';
var episodeTitleDisplay = 2;
var entriesPerPage = 30;
var uriObj = new Array();		// Object that holds the URI
var LAY_FORMATFILESIZE = false;
var LAY_SHOWCRC = false;
var useLangFilters = true;
var filterAudLang = new Array();
var filterSubLang = new Array();
var seeDebug = false;
var seeTimes = false;
var base_url = 'http://static.anidb.net/';
// mylist settings (deducted from mylist links, though the xmls should take care of most of the stuff)
var mylist_settings = new Object();
mylist_settings['noeptb'] = false;
mylist_settings['showvotes'] = true;
mylist_settings['showheader'] = true;
mylist_settings['filemode'] = '1';
var expandedAID = null;
// cached stuff
var loading_row = null;
var epTableHead = null;
var epTableFoot = null;
var fileTableHead = null;
var deltimer = null;
var g_note = null;
// general column definitions
var fileCols = cloneArray(genFileCols);
removeColAttribute("check-anime",fileCols);
removeColAttribute("state-anime",fileCols);
removeColAttribute("actions-anime",fileCols);
removeColAttribute("anime-source",fileCols);
removeColAttribute("resolution",fileCols);
removeColAttribute("users",fileCols);
var fileSkips = null;
var epCols = cloneArray(genEpCols);
removeColAttribute("airdate",epCols);
removeColAttribute("users",epCols);
var epSkips = null;


/* This function parses mylist expand links and sets some settings
 * @param href The mylist expand link
 */
function parseMylistExpandLink(href,obj) {
	if (!obj) obj = new Object();
	if (href.indexOf('?') > -1) { // we have an href
		var elems;
		if (href.indexOf('#') >= 0) {
			elems = href.substring(href.indexOf('?')+1,href.indexOf('#')).split('&');
			obj['#'] = href.substring(href.indexOf('#')+1,href.length);
		} else elems = href.substring(href.indexOf('?')+1,href.length).split('&');
	}
	for (i in elems) {
		var splite = String(elems[i]).split('=');
		var efield = splite[0];
		var evalue = splite[1];
		if (evalue == '1' || evalue == 'true') evalue = true;
		if (evalue == '0' || evalue == 'false') evalue = false;
		obj[efield] = evalue;
	}
	return obj;
}

/* This function prepares the mylist page for use with my scripts */
function prepPage() {
	// some other stuff, used only in dev 
	if (''+window.location.hostname != '') {
		var mylist3 = getElementsByClassName(document.getElementsByTagName('div'), 'mylist3', true)[0];
		if (mylist3) mylist3.className = mylist3.className.replace("mylist3","mylist"); // this will correct css used in dev
	} else base_url = '';
	uriObj = parseURI();
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	initTooltips();
	// find the mylist table and update the links
	var mylistTable = getElementsByClassName(document.getElementsByTagName('table'),'animelist',true)[0];
	if (!mylistTable) { errorAlert('prepPage','no mylist table found'); return; }
	var body = mylistTable.tBodies[0];
	for (var i = 1; i < body.rows.length-1; i++) {
		var row = body.rows[i];
		var cell = getElementsByClassName(row.getElementsByTagName('td'),'expand',true)[0];
		if (!cell) { errorAlert('prepPage','no expand cell found'); continue; }
		var a = cell.getElementsByTagName('a')[0];
		if (!a) { errorAlert('prepPage','no a link found'); continue; }
		parseMylistExpandLink(a.href,mylist_settings);
		ruid = mylist_settings['uid'];
		if (Number(ruid) == 1) return; // Ajax mylist is currently disabled for exp
		a.removeAttribute('href');
		if (a.className.indexOf('i_minus') >= 0) {
			a.onclick = foldAnime;
			expandedAID = Number(row.id.substring(1,row.id.length));
		}
		else a.onclick = expandAnime;
		a.style.cursor = "pointer";
	}
	var filters = getElementsByClassName(document.getElementsByTagName('div'),'filters',true)[0];
	if (filters) {
		var a = filters.getElementsByTagName('a')[0];
		if (a) {
			if (a.firstChild.nodeValue.indexOf('fileinfo') >= 0) {
				a.removeAttribute('href');
				a.onclick = toggleFileMode;
				a.style.cursor = "pointer";
			}
			mylist_settings['filemode'] = (a.firstChild.nodeValue.indexOf('show') >= 0) ? '0' : '1';
		}
	}
	mylist_settings['noeptb'] = true; // force this setting
	createPreferencesTable('mylist');
	cleanUpExpands();
}

/* Function that fetches anime data
 * @param aid Anime ID
 * @param uid User ID
 */
function fetchData(aid,uid) {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/aid'+aid+'_uid'+ruid+'.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=useranime&aid='+aid+'&uid='+Number(ruid), parseData);
}

/* Function that posts data
 * @param url URL to post
 */
function postData(url) {
	var req = xhttpRequest();
	var data = url.substr(url.indexOf('?')+1,url.length);
	if (''+window.location.hostname == '') xhttpRequestPost(req, 'msg_del.html', showSuccessBox, data);
	else xhttpRequestPost(req, 'animedb.pl', showSuccessBox, data);
}

/* Function that parses xml response
 * @param xmldoc The xml response
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { if (seeDebug) alert('Error: Could not get root node'); return; }
	updateStatus('Processing anime data...');
	t1 = new Date();
	parseAnimes(root.getElementsByTagName('animes'));
	var parseAnimeNode = (new Date()) - t1;
	var filedataNodes = root.getElementsByTagName('filedata');
	for (var k = 0; k < filedataNodes.length; k++) parseFiledata(filedataNodes[k], aid);
	updateStatus('Processing user data...');
	var t1 = new Date();
	parseCustom(root.getElementsByTagName('custom').item(0));
	var parseCustomNode = (new Date()) - t1;
	// do some triming of the definition cols if possible
	if ((!uriObj['showcrc'] || (uriObj['showcrc'] && uriObj['showcrc'] == '0')) && !LAY_SHOWCRC)
		removeColAttribute('crc',fileCols);
	if (!mylist_settings['noeptb']) removeColAttribute('epno',fileCols);
	fileSkips = buildSkipCols(fileCols);
	epSkips = buildSkipCols(epCols);
	if (seeTimes) alert('Processing...\n'+
						'\n\tanimes: '+parseAnimeNode+'ms'+
						'\n\tcustom: '+parseCustomNode+' ms'+
						'\nTotal: '+(parseAnimeNode+parseCustomNode)+' ms');
	updateStatus('');
	var aid = root.getElementsByTagName('anime')[0];
	if (!aid) { errorAlert('parseData','no anime node'); return; }
	aid = Number(aid.getAttribute('id'));
	createEpisodeTable(aid);
}

/* Shows a success box for a brief time */
function showSuccessBox(xmldoc) {
	if (!g_note) {
		g_note = document.createElement('div');
		g_note.className = 'g_section g_notebox';
		var h3 = document.createElement('h3');
		h3.appendChild(document.createTextNode('NOTE:'));
		g_note.appendChild(h3);
		var p = document.createElement('p');
		p.appendChild(document.createTextNode('Action done.'));
		g_note.appendChild(p);
	}
	var msg_all = getElementsByClassName(document.getElementsByTagName('div'), 'g_content msg_all', true)[0];
	if (!msg_all) return;
	msg_all.insertBefore(g_note,msg_all.firstChild);
	self.clearTimeout(deltimer);
	deltimer = self.setTimeout('removeNoteBox()', 5000);
}

/* Removes the success box */
function removeDelBox() {
	g_note.parentNode.removeChild(g_note);
	g_note = null;
}

/* Function that cleans up pages called with expanded animes */
function cleanUpExpands() {
	if (!uriObj['expand'] && uriObj['show']) return; // nothing to do
	var aRow = null;
	if (uriObj['expand']) {
		var aid = Number(uriObj['expand']);
		aRow = document.getElementById('a'+aid);
	} else if (expandedAID) { // this one is the tricky bit
		aRow = document.getElementById('a'+expandedAID);
	}
	if (!aRow) return;
	var rowIndex = aRow.rowIndex;
	var tbody = aRow.parentNode;
	if (!tbody.rows[rowIndex+1].id) tbody.removeChild(tbody.rows[rowIndex+1]); // the episode table row
	if (!tbody.rows[rowIndex+1].id) tbody.removeChild(tbody.rows[rowIndex+1]); // the other crapy row
	// now do my stuff and get this over with
	var a = aRow.getElementsByTagName('a')[0];
	if (a) {
		if (expandedAID) a.onclick = expandAnime;
		a.onclick();
		if (expandedAID) a.onclick = foldAnime;
	}
}

/* Function that expands one anime */
function expandAnime() {
	this.onclick = foldAnime;
	this.title = 'Fold entry';
	this.className = 'i_icon i_minus';
	var row = this.parentNode;
	while (row.nodeName.toLowerCase() != 'tr') row = row.parentNode;
	var aid = row.id.substr(1,row.id.length);
	if (!animes[aid]) {
		var nrow = createLoadingRow(row.cells.length);
		nrow.id = 'a'+aid+'_epsRow';
		row.parentNode.insertBefore(nrow,row.nextSibling);
		fetchData(aid,uid); // need to fetch data
	} else {	// show stuff
		var testRow = document.getElementById('a'+aid+'_epsRow');
		if (testRow) testRow.style.display = '';
	}
}

/* Function that folds one anime */
function foldAnime() {
	this.onclick = expandAnime;
	this.title = 'Expand entry';
	this.className = 'i_icon i_plus';
	var row = this.parentNode;
	while (row.nodeName.toLowerCase() != 'tr') row = row.parentNode;
	var aid = row.id.substr(1,row.id.length);
	var testRow = document.getElementById('a'+aid+'_epsRow');
	if (testRow) testRow.style.display = 'none';
}

/* Function that expands files of a given anime */
function expandFiles() {
	this.onclick = foldFiles;
	this.title = 'Fold this entry';
	this.className = 'i_icon i_minus';
	this.firstChild.firstChild.nodeValue = '[-]';
	var row = this.parentNode;
	while (row.nodeName.toLowerCase() != 'tr') row = row.parentNode;
	var eid = row.id.substr(1,row.id.length);
	var testRow = document.getElementById('e'+eid+'_filesRow');
	if (testRow) testRow.style.display = '';
}

/* Function that folds files of a given anime */
function foldFiles() {
	this.onclick = expandFiles;
	this.title = 'Expand this entry';
	this.className = 'i_icon i_plus';
	this.firstChild.firstChild.nodeValue = '[+]';
	var row = this.parentNode;
	while (row.nodeName.toLowerCase() != 'tr') row = row.parentNode;
	var eid = row.id.substr(1,row.id.length);
	var testRow = document.getElementById('e'+eid+'_filesRow');
	if (testRow) testRow.style.display = 'none';
}

/* Function that toogles all the checkboxes for a given anime */
function cbToggle() {
	var selGroup = this.form.getElementsByTagName('select')[0];
	if (selGroup) selGroup = selGroup.value;
	var node = this.parentNode;
	while (node.nodeName.toLowerCase() != 'table') node = node.parentNode;
	var aid = Number(node.id.substring(1,node.id.indexOf('_')));
	var table = document.getElementById('a'+aid+'_episodesTable');
	if (!table) table = document.getElementById('a'+aid+'_filesTable');
	if (!table) return; // can't make this work :P
	var tbody = table.tBodies[0];
	var checkboxes = tbody.getElementsByTagName('input');
	for (var i = 0; i < checkboxes.length; i++) {
		var ck = checkboxes[i];
		if (ck.type != 'checkbox') continue;
		// now comes the nice bit;
		if (selGroup == 'all') ck.checked = this.checked;
		else { // filtering by group
			var row = ck.parentNode;
			while(row.nodeName.toLowerCase() != 'tr') row = row.parentNode;
			var fid = row.id.substring(row.id.indexOf('f')+1,row.id.length);
			var file = files[fid];
			if (!file) continue;
			if (file.groupId == selGroup) ck.checked = this.checked;
		}
	}
}

/* Function that toogles the current fileinfo mode */
function toggleFileMode() {
	var showFiles = (this.firstChild.nodeValue.indexOf('show') >= 0);
	if (showFiles) { // hidden files, show them!
		mylist_settings['filemode'] = '1';
		this.firstChild.nodeValue = 'hide fileinfo';
	} else { // shown files, hide them!
		mylist_settings['filemode'] = '0';
		this.firstChild.nodeValue = 'show fileinfo';
	}
	for (var a in animes) {
		var anime = animes[a];
		if (!anime) continue;
		createEpisodeTable(anime.id);
	}
}

/* Function that marks a file (un)watched */
function changeWatchedState() {
	var row = this.parentNode;
	var isWatched = (this.className.indexOf('i_seen_yes') >= 0);
	while (row.nodeName.toLowerCase() != 'tr') row = row.parentNode;
	var fid = Number(row.id.substr(row.id.indexOf('f')+1,row.id.length));
	if (this.href) this.removeAttribute('href');
	var mylistEntry = mylist[fid];
	mylistEntry.seen = isWatched;
	var now = new Date();
	var day = (now.getDate() > 9 ? now.getDate() : '0'+now.getDate());
	var month = (((now.getMonth() + 1) > 9) ? (now.getMonth()+1) : '0'+(now.getMonth()+1));
	now = day + '.' + month + '.' + now.getFullYear();
	mylistEntry.seenDate = (isWatched ? now : 0);
	var url;
	var file = files[fid];
	var eids = new Array();
	eids[file.episodeId] = 1;
	for (var eid in file.epRelations) eids[eid] = 1;
	for (var eid in eids) {
		if (!eids[eid]) continue;
		var episode = episodes[eid];
		var row = document.getElementById('e'+eid+'f'+file.id);
		var stateCell = getElementsByClassName(row.getElementsByTagName('td'),'icons state',true)[0];
		var actionCell = getElementsByClassName(row.getElementsByTagName('td'),'icons action',true)[0];
		var ico = getElementsByClassName(actionCell.getElementsByTagName('a'),'i_seen_',true)[0];
		var arow = document.getElementById('a'+episode.animeId);
		var cellSeen = (arow ? getElementsByClassName(arow.getElementsByTagName('td'),'stats seen',true)[0] : null);
		if (mylistEntry.seen) {
			url = 'animedb.pl?show=mylist&do=seen&seen=1&lid='+mylistEntry.id;
			ico.title = 'mark unwatched';
			ico.className = 'i_icon i_seen_no';
			ico.getElementsByTagName('span')[0].nodeValue = 'mylist.unwatch';
			// add seen icon
			if (stateCell) createIcon(stateCell, 'seen ', null, null, 'seen on: '+cTimeDate(mylistEntry.seenDate), 'i_seen');
			// if episode is unwatched, mark it watched
			if (!episode.seenDate) {
				episode.seenDate = now; // updated episode entry
				var erow = document.getElementById('e'+eid);
				if (erow) { // we got the episode row, add the span
					var cell = getElementsByClassName(erow.getElementsByTagName('td'), 'title', true)[0];
					if (cell) {
						var icons = createEpisodeIcons(episode);
						var span = getElementsByClassName(cell.getElementsByTagName('span'),'icons', true)[0];
						if (!span) { // no span, add one to the dom and add the icon
							span = document.createElement('span');
							span.className = 'icons';
							cell.insertBefore(span,cell.firstChild);
						} else {
							while (span.childNodes.length) span.removeChild(span.firstChild);
						}
						if (icons['seen']) span.appendChild(icons['seen']);
						if (icons['state']) for (var st = 0; st < icons['state'].length; st++) span.appendChild(icons['state'][st]);
						if (icons['fstate']) for (var st = 0; st < icons['fstate'].length; st++) span.appendChild(icons['fstate'][st]);
						// update anime row
						if (cellSeen) {
							if (episode.typeChar == '') { // episode is one of the normal episodes
								var text = cellSeen.firstChild.nodeValue;
								var valueSeen =  Number(text.split('/')[0]);
								cellSeen.firstChild.nodeValue = text.replace(valueSeen+'/',(valueSeen+1)+'/');
								if ((valueSeen+1) == animes[episode.animeId].eps && (arow.className.indexOf('complete') >= 0))
									arow.className = arow.className.replace('complete','all_watched');
							} else if (episode.typeChar == 'S') { // episode is special
								var text = cellSeen.firstChild.nodeValue;
								var value = Number(text.split('+')[1]);
								if (!value) cellSeen.firstChild.nodeValue += '+1';
								else cellSeen.firstChild.nodeValue = text.replace('+'+value,'+'+(value+1));
							}
						}
					}
				}
			}
		} else {
			url = 'animedb.pl?show=mylist&do=seen&seen=0&lid='+mylistEntry.id;
			ico.title = 'mark watched';
			ico.className = 'i_icon i_seen_yes';
			ico.getElementsByTagName('span')[0].nodeValue = 'mylist.watch';
			// remove seen icon
			if (stateCell) {
				var ico = getElementsByClassName(stateCell.getElementsByTagName('span'),'i_seen',true)[0];
				if (ico) stateCell.removeChild(ico);
			}
			// this one is a bit more trick, if this is the only file or all other files are unwatched, remove the ep watched icon
			var epWatchedCount = 0;
			for (var fe in episode.files) { // find how many watched files we have (the current file has already been marked unwatched)
				if (mylist[episode.files[fe]].seenDate) epWatchedCount++;
			}
			if (episode.files.length == 1 || !epWatchedCount) { // there are now more watched files
				episode.seenDate = 0; // updated episode entry
				var erow = document.getElementById('e'+eid);
				if (erow) { // we got the episode row, add the span
					var cell = getElementsByClassName(erow.getElementsByTagName('td'), 'title', true)[0];
					if (cell) {
						var spans = cell.getElementsByTagName('span');
						for (var sp = 0; sp < spans.length; sp++) {
							var span = spans[sp];
							var ico = getElementsByClassName(span.getElementsByTagName('span'),'i_seen',true)[0];
							if (ico) { // we found our target, delete icon and span if unused.
								span.removeChild(ico);
								if (!span.childNodes.length) span.parentNode.removeChild(span);
								// update anime row
								if (cellSeen) {
									if (episode.typeChar == '') { // episode is one of the normal episodes
										var text = cellSeen.firstChild.nodeValue;
										var valueSeen =  Number(text.split('/')[0]);
										var newVal = (valueSeen-1);
										if (newVal >= 0) cellSeen.firstChild.nodeValue = text.replace(valueSeen+'/',newVal+'/');
										if (arow.className.indexOf('all_watched') >= 0) arow.className = arow.className.replace('all_watched','complete');
									} else if (episode.typeChar == 'S') { // episode is special
										var text = cellSeen.firstChild.nodeValue;
										var value = Number(text.split('+')[1]);
										if (!value) cellSeen.firstChild.nodeValue += '+1';
										else cellSeen.firstChild.nodeValue = text.replace('+'+value,'+'+(value+1));
									}
								}
								break;
							}
						}
					}
				}
			}
		}
	}
	postData(url);
	return true;
}

/* Function that adds a file to mylist */
function addToMylist() {
	alert('This action shouldn\'t be possible.\nPlease inform an anidb moderator what you have done to get this message.');
	return true;
}

/* Function that removes a file from mylist */
function removeFromMylist() {
	var row = this.parentNode;
	while (row.nodeName.toLowerCase() != 'tr') row = row.parentNode;
	var fid = Number(row.id.substr(row.id.indexOf('f')+1,row.id.length));
	if (this.href) this.removeAttribute('href');
	var mylistEntry = mylist[fid];
	var url = 'animedb.pl?show=mylist&do=del&lid='+mylistEntry.id;
	mylist[fid] = null; // clear mylist entry
	var file = files[fid];
	var eids = new Array();
	eids[file.episodeId] = 1;
	for (var eid in file.epRelations) eids[eid] = 1;
	for (var eid in eids) {
		if (!eids[eid]) continue;
		var row = document.getElementById('e'+eid+'f'+file.id);
		var eidFilesRow = document.getElementById('e'+eid+'_filesRow');
		var eidRow = document.getElementById('e'+eid);
		var fileTable = row.parentNode;
		fileTable.removeChild(row);
		if (!fileTable.rows.length) { // no more files for this episode, remove the row
			if (eidFilesRow) fileTable = eidFilesRow.parentNode;
			if (eidFilesRow) fileTable.removeChild(eidFilesRow);
			if (eidRow) fileTable.removeChild(eidRow);
			row = fileTable;
			// now we need to update the anime episode count
			// NOTE: i quicked pathed to episodes only belonging to one anime, should i do this?
			var episode = episodes[eid];
			var arow = document.getElementById('a'+episode.animeId);
			if (arow) {
				var cell1 = getElementsByClassName(arow.getElementsByTagName('td'),'stats eps',true)[0];
				var cell2 = getElementsByClassName(arow.getElementsByTagName('td'),'stats seen',true)[0];
				if (cell1 || cell2) {
					if (episode.typeChar == '') { // episode is one of the normal episodes
						if (cell1) {
							var text = cell1.firstChild.nodeValue;
							var value = Number(text.split('/')[0]);
							cell1.firstChild.nodeValue = text.replace(value+'/',(value-1)+'/');
						}
						if (cell2) {
							var text = cell2.firstChild.nodeValue;
							var valueSeen  = text.split('/')[0];
							var valueTotal = text.split('/')[1];
							if (valueTotal.indexOf('+') < 0) valueTotal = Number(valueTotal);
							else valueTotal = Number(valueTotal.split('+')[0]);
							cell2.firstChild.nodeValue = text.replace(valueSeen+'/'+valueTotal,((valueSeen && valueSeen > 0) ? (valueSeen-1) : '0') +'/'+(valueTotal-1));
						}
					} else if (episode.typeChar == 'S') { // episode is special
						if (cell1) {
							var text = cell1.firstChild.nodeValue;
							var value = Number(text.split('+')[1]);
							var newVal = value-1;
							cell1.firstChild.nodeValue = text.replace('+'+value,(newVal ? '+'+newVal : ''));
						}
						if (cell2) {
							var text = cell2.firstChild.nodeValue;
							var value = Number(text.split('+')[1]);
							var newVal = value-1;
							cell2.firstChild.nodeValue = text.replace('+'+value,(newVal ? '+'+newVal : ''));
						}
					}
					if (arow.className.indexOf('complete') >= 0) arow.className = arow.className.replace('complete','');
					if (arow.className.indexOf('all_watched') >= 0) arow.className = arow.className.replace('all_watched','');
				}
			}
		} else { // need to update the file count
			if (eidRow) {
				var countCell = getElementsByClassName(eidRow.getElementsByTagName('td'),'count files',true)[0];
				if (countCell) {
					var fileCount = Number(countCell.firstChild.nodeValue);
					fileCount--;
					countCell.firstChild.nodeValue = fileCount;
				}
			}
		}
		if (!fileTable.rows.length) { // no more episodes for this anime, remove the row
			while (row.nodeName != 'TR') row = row.parentNode;
			var aid = Number(row.id.substr(1,row.id.indexOf('_')-1));
			var aidRow = document.getElementById('a'+aid);
			var fileTable = aidRow.parentNode;
			fileTable.removeChild(aidRow);
			fileTable.removeChild(row);
		}
	}
	postData(url);
	return true;
}


/* Creates an episode files table thead */
function createFilesTableHead() {
	var thead = createTableHead(fileCols);
	fileTableHead = thead;
	return(fileTableHead.cloneNode(true));
}

/* Function that creates an episode files table
 * @param eid Episode ID
 * @return Returns an episode file table inside the row
 */
function createFilesTable(eid) {
	var insertRow = document.createElement('tr');
	insertRow.id = 'e'+eid+'_filesRow';
	insertRow.style.display = (mylist_settings['filemode'] == '1' ? '' : 'none');
	var insertCell = document.createElement('td');
	//insertCell.colSpan = (mylist_settings['noeptb']) ? '7' : '6';
	insertCell.colSpan = '7';
	// Create the files table
	var table = document.createElement('table');
	table.className = 'filelist';
	table.id = 'e'+eid+'_filesTable';
	var thead = (fileTableHead ? fileTableHead.cloneNode(true) : createFilesTableHead());
	table.appendChild(thead);
	var tbody = document.createElement('tbody');
	// Add files
	var episode = episodes[eid];
	for (var i = 0; i < episode.files.length; i++) {
		var fid = episode.files[i];
		if (!mylist[fid]) continue;
		var row = createFileRow(eid,fid,fileCols,fileSkips);
		row.className = ((i % 2) ? '' : 'g_odd ') + 'files';
		tbody.appendChild(row);
	}
	// Piece it all together
	table.appendChild(tbody);
	init_sorting(table);
	insertCell.appendChild(table);
	insertRow.appendChild(insertCell);
	return insertRow;
}

/* Creates an episode table thead */
function createEpisodeTableHead() {
	var thead = createTableHead(epCols);
	epTableHead = thead;
	return(epTableHead.cloneNode(true));
}

/* Function that creates the episode table tfoot */
function createEpisodeTableFoot(colSpan,aid) {
	var tfoot = document.createElement('tfoot');
	var row = document.createElement('tr');
	var cell = document.createElement('td');
	cell.colSpan = colSpan;
	createCheckBox(cell,'ck',null,cbToggle,false);
	cell.appendChild(document.createTextNode(' select '));
	var optionArray = [ {"value":'all',"text":'all'} ];
	for (var g in animes[aid].groups) {
		var group = groups[animes[aid].groups[g]];
		if (!group) continue;
		optionArray.push({"value":group.id,"text":group.shortName});
	}
	createSelectArrayN(cell,"mylmod.groupsel","mylmod.groupsel",null,0,optionArray);
	cell.appendChild(document.createTextNode(' files '));
	optionArray = [	{"value":'0',"text":'Select action',"class":'section',"selected":true},{"value":'seen',"text":'mark watched'},{"value":'unseen',"text":'mark not watched'},
					{"value":'update',"text":'edit (more options)'},{"value":'del',"text":'remove'},
					{"value":'0',"text":'Change state to',"class":'section',"disabled":'disabled'},
					{"value":'state:0',"text":'unknown'},{"value":'state:1',"text":'internal storage (hdd)'},{"value":'state:2',"text":'external storage (cd/dvd/...)'},{"value":'state:3',"text":'deleted'},
					{"value":'0',"text":'Change file state to',"class":'section',"disabled":'disabled'},
					{"value":'fstate:0',"text":'normal/original'},{"value":'fstate:1',"text":'corrupted version/invalid crc'},{"value":'fstate:2',"text":'self edited'},{"value":'fstate:100',"text":'other'},
					{"value":'0',"text":'Change generic state to',"class":'section',"disabled":'disabled'},
					{"value":'gstate:10',"text":'self ripped'},{"value":'gstate:11',"text":'on dvd'},{"value":'gstate:12',"text":'on vhs'},
					{"value":'gstate:13',"text":'on tv'},{"value":'gstate:14',"text":'theater'},{"value":'gstate:20',"text":'filler ep'},{"value":'gstate:100',"text":'other'}];
	createSelectArrayN(cell,"mylmod.type","mylmod.type",null,0,optionArray);
	cell.appendChild(document.createTextNode(' '));
	var button = createBasicButton('mylmod.doit','Update selected files','submit');
	cell.appendChild(button)
	row.appendChild(cell);
	tfoot.appendChild(row);
	fileTableFoot = tfoot;
	return(fileTableFoot.cloneNode(true));
}

/* Function that crates an episode table
 * @param aid Anime id to create the episode table
 */
function createEpisodeTable(aid) {
	var pRow = document.getElementById('a'+aid);
	// Create the insert row for later insertion
	var insertRow = document.createElement('tr');
	insertRow.id = 'a'+aid+'_epsRow';
	var insertCell = document.createElement('td');
	insertCell.colSpan = pRow.cells.length;
	// create the form
	var form = document.createElement('form');
	form.method = 'post';
	form.action = 'animedb.pl';
	var fieldset = document.createElement('fieldset');
	fieldset.appendChild(createTextInput('show',null,false,true,null,'mylist'));
	fieldset.appendChild(createTextInput('expand',null,false,true,null,aid));
	fieldset.appendChild(createTextInput('uid',null,false,true,null,uid));
	fieldset.appendChild(createTextInput('filemode',null,false,true,null,mylist_settings['filemode']));
	fieldset.appendChild(createTextInput('noeptb',null,false,true,null,mylist_settings['noeptb']));
	fieldset.appendChild(createTextInput('showheader',null,false,true,null,mylist_settings['showheader']));
	form.appendChild(fieldset);
	// now for the tables
	var table = document.createElement('table');
	var colSpan;
	if (mylist_settings['filemode'] == 0) {
		table.className = 'eplist';
		table.id = 'a'+aid+'_episodesTable';
		colSpan = 7;
		var thead = (epTableHead ? epTableHead.cloneNode(true) : createEpisodeTableHead(colSpan));
		table.appendChild(thead);
		var tbody = document.createElement('tbody');
		var anime = animes[aid];
		for (var i = 0; i < anime.episodes.length; i++) {
			var eid = anime.episodes[i];
			var row = createEpisodeRow(aid,eid,epCols,epSkips);
			row.className = ((i % 2) ? '' : 'g_odd ') + 'files';
			tbody.appendChild(row);
			row = createFilesTable(eid);
			tbody.appendChild(row);
		}
		table.appendChild(tbody);
	} else { // only show files
		table.className = 'eplist';
		table.id = 'a'+aid+'_filesTable';
		colSpan = 12;
		if (mylist_settings['noeptb']) colSpan++;
		if ((uriObj['showcrc'] && uriObj['showcrc'] == '1') || LAY_SHOWCRC) colSpan++;
		var thead = (fileTableHead ? fileTableHead.cloneNode(true) : createFilesTableHead());
		table.appendChild(thead);
		var tbody = document.createElement('tbody');
		var anime = animes[aid];
		// Add files		
		for (var e = 0; e < anime.episodes.length; e++) {
			var eid = anime.episodes[e];
			var episode = episodes[eid];
			for (var f = 0; f < episode.files.length; f++) {
				var fid = episode.files[f];
				if (!mylist[fid]) continue;
				var row = createFileRow(eid,fid,fileCols,fileSkips);
				tbody.appendChild(row);
			}
		}
		for (var i = 0 ; i < tbody.rows.length; i++)
			tbody.rows[i].className = ((i % 2) ? '' : 'g_odd ') + 'files';
		table.appendChild(tbody);
	}
	if (uid == ruid) {
		var tfoot = createEpisodeTableFoot(colSpan,aid);
		var inputs = tfoot.getElementsByTagName('input');
		for (var i = 0; i < inputs.length; i++) {
			var input = inputs[i];
			if (input.type != 'checkbox') continue;
			input.onchange = cbToggle;
			break;
		}
		table.appendChild(tfoot);
	}
	form.appendChild(table);
	// Now piece it all together
	insertCell.appendChild(form);
	insertRow.appendChild(insertCell);
	// add the new row to the mylist table
	pRow.parentNode.replaceChild(insertRow,document.getElementById('a'+aid+'_epsRow'));
}

// hook up the window onload event
addLoadEvent(prepPage);
