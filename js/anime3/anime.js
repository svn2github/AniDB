/* file animePage interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (22.03.2007) - Initial version
 * version 1.2 (03.04.2008) - Some fixes and starting 2.0 conversion
 * version 2.0 (xx.04.2008) - Version 2.0 conversion
 */

// GLOBALs //

var uid;						// userID
var mod;						// isMod
var anime;						// anime Object (used in animePage)
var animes = new Array();		// stored by aid
var animeOrder = new Array();	// animes ordered in db way (link to aid)
//var groupOrder = new Array();	// ordered group list (filtering porpuses)
var groups = new Array();		// stored by gid
var aGroups = new Array();		// stored by agid (gid to link groups)
var mylist = new Array();		// stored by fid
var episodes = new Array();		// stored in eid
var epOrder = new Array();		// episodes ordered in db way (link to eid)
var files = new Array();		// stored by fid
var epQueue = new Array();		// episode process queue
var groupFilter = new Array();	// Filter with group filters
// settings
var animeTitleLang = '';
var animeAltTitleLang = 'x-jat';
var episodeTitleLang = '';
var episodeAltTitleLang = 'x-jat';
var episodeTitleDisplay = 2;
var entriesPerPage = 30;
var uriObj = new Array();      // Object that holds the URI
var LAY_HEADER = false;
var LAY_NOANIMEGROUPREL = false;
var LAY_HIDEFILES = false;
var LAY_HIDERAWS = false;
var LAY_HIDEGROUPRAWS = false;
var LAY_HIDEGENERICFILES = false;
var LAY_HIDEPARODYEPS = false;
var LAY_SHOWFID = false;
var LAY_SHOWCRC = false;
var LAY_FORMATFILESIZE = false;
var LAY_HIDEFILTEREDGROUPS = true;
var useLangFilters = true;
var filterAudLang = new Array();
var filterSubLang = new Array();
var seeDebug = false;
var seeTimes = false;
var epInterval = 250; // This one needs work
var epInt; // Interval ID
var processingEps = false; // are we processing episodes?
var hideNoGroup = false;
var hideAllGroups = false;
var hiddenGroups = 0;
var loadExpand = false;
var internalExpand = false;
var expandedGroups = 0;
var showNoGroup = true;
var expandNoGroup = false;
var expandAllG = false;
var base_url = 'http://static.anidb.net/';
var group_vis = {'complete':false,'finished':false,'ongoing':false,'stalled':false,'dropped':false,'specials':false,'all':true};
var simpleaddanimetomylistbox = null;
var addfilestomylistbox = null;
var addanimetomylistbox = null;
var g_note = null;
// general column definitions
// remove some cols from the default definitions
var fileCols = cloneArray(genFileCols);
removeColAttribute("check-mylist",fileCols);
removeColAttribute("mylist-storage",fileCols);
removeColAttribute("mylist-source",fileCols);
removeColAttribute("state-mylist",fileCols);
removeColAttribute("actions-mylist",fileCols);
removeColAttribute("epno",fileCols);
var fileSkips = null;
var groupCols = cloneArray(genGroupCols);
var groupSkips = buildSkipCols(groupCols);
//var animeCols = cloneArray(genAnimeCols);	

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
	aid = Number(uriObj['aid']);
	if (isNaN(aid)) return;
	fetchData(aid);
	createPreferencesTable('anime');
}

/* Function that fetches anime data
 * @param aid Anime ID
 * @param uid User ID
 */
function fetchData(aid) {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/aid'+aid+'.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=anime&aid='+aid, parseData);
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
	fileSkips = buildSkipCols(fileCols);
	updateStatus('');
	var aid = root.getElementsByTagName('anime')[0];
	if (!aid) { errorAlert('parseData','no anime node'); return; }
	aid = Number(aid.getAttribute('id'));
	t1 = new Date();
	var t2 = new Date();
	updateGroupTable();
	var uGT = new Date() - t2;
	t2 = new Date();
	updateEpisodeTable();
	var uET = new Date() - t2;
	t2 = new Date();
	updateAddToMylistBox();
	var uAB = new Date() - t2;
	var preparingPage = (new Date() - t1);
	if (seeTimes) alert('Processing...\n'+
						'\n\tanimes: '+parseAnimeNode+'ms'+
						'\n\tcustom: '+parseCustomNode+' ms'+
						'\n\tpreping: '+preparingPage+' ms'+
						'\n\t\tupdateGroupTable: '+uGT+' ms'+
						'\n\t\tupdateEpisodeTable: '+uET+' ms'+
						'\n\t\tupdateAddToMylistBox: '+uAB+' ms'+
						'\n\tTotal: '+(parseAnimeNode+parseCustomNode+preparingPage)+' ms');
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
	var msg_all = getElementsByClassName(document.getElementsByTagName('DIV'), 'g_content msg_all', true)[0];
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

/* -[GROUP]-------------------------
 * GROUP TABLE AND RELATED FUNCTIONS
 * ---------------------------------
 */

/* Function that paints stripes (depending on mode)
 * @param i Index of the row to check if it would be a odd or even row
 * @param row RowElement, the row to paint [optional]
 * @return ' g_odd'/'' if only i supplied, nothing otherwise
 */
function gODD(i,row) {
	var g_odd = (!(i % 2) ? ' g_odd' : '');
	if (!row) return g_odd;
	if (row.className.indexOf('g_odd') >= 0) row.className = row.className.replace("g_odd","");
	if (row.className.length > 0) g_odd = " " + g_odd;
	row.className += g_odd;
}

/* Function that actualy does the group table operations
 * @param op Operation to execute [expand,fold]
 */
function doGroupTableOperations(op) {
	var groupTable = document.getElementById('grouplist');
	if (!groupTable) return;
	var tbody = groupTable.tBodies[0];
	var tfoot = groupTable.tFoot;
	var visible = 0;
	for (var g = 0; g < tbody.rows.length; g++) {
		var row = tbody.rows[g];
		var gid = Number(row.id.substring(4,row.id.length));
		var group = groups[gid];
		if (op == 'expand' || (op == 'fold' && group.defaultVisible) ) {
			row.style.display = '';
			gODD(visible,row);
			visible++;
		}
		else row.style.display = 'none';
	}
	gODD(visible,tfoot.rows[0]);
	var cell = getElementsByClassName(tfoot.rows[0].getElementsByTagName('td'), 'action', false)[0];
	if (cell) {
		var ahref = cell.getElementsByTagName('a')[0];
		if (ahref) { // There are cases when we have nothing to expand
			if (op == 'expand') {
				ahref.onclick = foldGroupTable;
				ahref.firstChild.nodeValue = 'show default';
				expandAllG = true;
			} else {
				ahref.onclick = expandGroupTable;
				ahref.firstChild.nodeValue = 'show all';
				expandAllG = false;
			}
		}
	}
}


/* Function that expands the group table */
function expandGroupTable() { doGroupTableOperations('expand'); }
/* Function that folds the group table */
function foldGroupTable() { doGroupTableOperations('fold'); }

/* Function that forces an episode file table redraw
 * @param episode An Episode to force the redraw
 */
function forceFileTableRedraw(episode) {
	if (!episode) {
		for (var e in episodes) {
			var episode = episodes[e];
			if (!episode || !episode.files) continue;
			var row = document.getElementById('eid_'+episode.id+'_ftHolder');
			if (!row) continue;
			var fileTable = createFileTable(episode);
			row.cells[0].className = '';
			if (!fileTable.tBodies[0].rows.length && !fileTable.tFoot.rows.length) 
				fileTable.parentNode.removeChild(fileTable);
		}
	} else {
		var fileTable = createFileTable(episode);
		var row = document.getElementById('eid_'+episode.id+'_ftHolder');
		if (!row) return;
		row.cells[0].className = '';
		if (!fileTable.tBodies[0].rows.length && !fileTable.tFoot.rows.length) 
			fileTable.parentNode.removeChild(fileTable);
	}
}

/* Function that forces group visibility
 * @param vis Group visibility
 */
function forceGroupVisibilty(vis) {
	groupFilter = new Array();
	for (var g in groups) {
		var group = groups[g];
		if (!group) continue;
		var row = document.getElementById('gid_'+group.id);
		if (row) {
			var cell, icon;
			// hook up the expand function
			cell = getElementsByClassName(row.getElementsByTagName('td'), 'expand', true)[0];
			if (cell) {
				var ahref = cell.getElementsByTagName('a')[0];
				if (ahref.className.indexOf('minus') >= 0) { // group is expanded 
					if (!vis) {
						group.visible = true; // show group files
						groupFilter.push(group.id);
					}
					continue;
				}
			}
		}
		group.visible = vis;
	}
	if (!vis && expandNoGroup) {
		groups[0].visible = true;
		groupFilter.push(0);
	}
}

/* Function that toogles files for a given group */
function toggleFilesFromGroup() {
	var checked = this.checked;
	var gid = this.id.substring(4,this.id.length);
	var filesChecked = 0;
	var totalFiles = 0;
	for (var f in files) {
		var file = files[f];
		if (!file) continue;
		if (file.groupId != gid) continue;
		totalFiles++;
		if (checked) {
			if (Number(group_check_type) != 5 && !file.visible) continue;
			switch(Number(group_check_type)) {
				case 0: break;
				case 1: if (file.fileType != 'mkv' && file.fileType != 'ogm') continue; break;
				case 2: if (file.fileType != 'avi') continue; break;
				case 3: if ((!file.videoTracks.length || file.videoTracks[0].resH < 720)) continue; break;
				case 4: if ((!file.videoTracks.length || file.videoTracks[0].resH >= 720)) continue; break;
				case 5: break;
				default: continue;
			}
		}
		var row = document.getElementById('e'+file.episodeId+'f'+file.id);
		if (!row) continue;
		var ck = row.getElementsByTagName('input')[0];
		if (!ck) continue;
		ck.checked = checked;
		filesChecked++;
	}
	if (checked) 
		alert('Checked '+filesChecked+' of '+totalFiles+' files, please confirm if the selection is correct.'+
			'\nManual adding is not recommend. The best way to add files to your mylist is by using an AniDB Client. There is also a ready-to-run Java Applet.');
	showAddToMylistBox();
}

/* Function that expands files by group */
function expandFilesByGroup() { 
	// this will be cute :P
	loadExpand = true; // warn scripts we are doing a full blown ep work
	internalExpand = true;
	expandedGroups++;
	var expandCnt = 0;
	var row;
	var gid = this.parentNode.parentNode.id;
	gid = gid.substring(gid.indexOf('_')+1,gid.length);
	if (!gid) { gid = 0; showNoGroup = true; expandNoGroup = true; }
	var group = groups[gid];
	group.visible = true;
	var getXML = false;
	if (group && !group.hasCherryBeenPoped) { 
		getXML = true;
		group.hasCherryBeenPoped = true; // And assuming everything goes well, our group has had his cherry poped :P
	}
	// add the checkbox to the group
	if (group.id != 0) row = document.getElementById('gid_'+group.id);
	if (row) {
		group.defaultVisible = true;
		var cell = getElementsByClassName(row.getElementsByTagName('td'), 'action', true)[0];
		if (cell) createCheckBox(cell,'ck_g'+group.id,'ck_g'+group.id,toggleFilesFromGroup,false);
	}
	this.onclick = foldFilesByGroup;
	this.title = this.alt = 'Fold this release';
	this.firstChild.firstChild.nodeValue = '-';
	this.className = 'i_icon i_minus';
	forceGroupVisibilty(false);		// #1 Hide all groups that aren't expanded
	// check for page expands
	for (var e in episodes) {
		var episode = episodes[e];
		if (!episode) continue;
		var eid = episode.id;
		var row = document.getElementById('eid_'+eid);
		if (!row) continue;
		var tRow = document.getElementById(row.id+'_ftHolder');
		if (tRow) tRow.parentNode.removeChild(tRow);
		var nRow = createLoadingRow(10); //the new episode row
		nRow.id = 'eid_' + eid + '_ftHolder';		
		row.parentNode.insertBefore(nRow,row.nextSibling);
		var cell = getElementsByClassName(row.getElementsByTagName('td'), 'expand', true)[0];
		var ahref = cell.getElementsByTagName('a')[0];
		if (getXML) ahref.onclick = null; // will be set later
		else ahref.onclick = foldEp;
		ahref.title = ahref.alt = 'fold all entries';
		ahref.firstChild.firstChild.nodeValue = '-';
		ahref.className = 'i_icon i_minus';
		if (!getXML) // we already have the files for this group so we need to do the following:
			forceFileTableRedraw(episode);	// #2 Force a redraw of the episode file table
	}
	uriObj['expandall'] = 1;
	if (getXML) { // *Need* to fetch the xml so we need to do the following:
		var req = xhttpRequest(); // #2 Fetch data
		if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/aid'+aid+'_gid'+gid+'.xml', parseEpisodeData);
		else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=ep&aid='+uriObj['aid']+'&eid=all&gid='+gid, parseEpisodeData);
	}
}

/* Function that folds files by group */
function foldFilesByGroup() {
	// this will be cute :P
	expandedGroups--;
	var row;
	var gid = this.parentNode.parentNode.id;
	gid = gid.substring(gid.indexOf('_')+1,gid.length);
	if (!gid) { gid = 0; showNoGroup = false; expandNoGroup = false; }
	var group = groups[gid];
	group.visible = false;
	var eid = null;
	this.onclick = expandFilesByGroup;
	this.title = this.alt = 'Expand this release';
	this.firstChild.firstChild.nodeValue = '+';
	this.className = 'i_icon i_plus';
	if (expandedGroups <= 0) forceGroupVisibilty(true); // unhide all groups
	else forceGroupVisibilty(false);                    // #1 Hide all groups that aren't expanded
	var row = null;
	if (group.id != 0) row = document.getElementById('gid_'+group.id);
	if (row) {
		group.defaultVisible = true;
		var cell = getElementsByClassName(row.getElementsByTagName('td'), 'action', true)[0];
		if (cell && row.id.indexOf('gid') >= 0) {
			var cks = cell.getElementsByTagName('input');
			while (cks.length) cell.removeChild(cks[0]);
		}
	}
	for (var e in episodes) {
		var episode = episodes[e];
		if (!episode) continue;
		eid = episode.id;
		var row = document.getElementById('eid_'+eid);
		if (!row) continue;
		// we will remove the table on fold on this case
		var tb = document.getElementById('eid_'+episode.id+'_ftHolder');
		if (tb) tb.parentNode.removeChild(tb);
		if (expandedGroups <= 0) { // no more groups expanded
			// update the icon
			var cell = getElementsByClassName(row.getElementsByTagName('td'), 'action expand', true)[0];
			var ahref = cell.getElementsByTagName('a')[0];
			ahref.onclick = expandEp;
			ahref.title = this.alt = 'expand all entries';
			ahref.firstChild.firstChild.nodeValue = '+';
			ahref.className = 'i_icon i_plus';
		} else { // we still have expanded groups
			var nRow = createLoadingRow(10); //the new episode row
			nRow.id = 'eid_' + eid + '_ftHolder';		
			row.parentNode.insertBefore(nRow,row.nextSibling);
			forceFileTableRedraw(episode); // Force a redraw of the fileTable
		}
	}
	internalExpand = false;
}

/* Function that filters the group table according to a given filter */
function filterGroups() {
	var state = this.className;
	var active = (state.indexOf(' f_selected') >= 0);
	state = state.substring(state.indexOf('gstate_')+7,(active) ? state.indexOf(' f_selected') : state.length);
	for (var g in groups) {
		var group = groups[g];
		if (!group) continue;
		if (group.id == 0) continue;
		var gstate = group.state.substring(0,(group.state.indexOf(' ') >= 0) ? group.state.indexOf(' ') : group.state.length);
		//alert('state: '+state+'\nactive: '+active+'\ngstate: '+gstate+'\nequal: '+(gstate == state));
		if (state != 'all') {
			if (gstate == state) { 
				if (!active) group.filtered = true;
				else group.filtered = false;
			}
		} else {
			if (active) continue;
			group.filtered = false;
		}
		var row = document.getElementById('gid_'+group.id);
		if (!row) continue;
		else {
			if (expandAllG) row.style.display = (group.filtered) ? 'none' : '';
			else {
				if (group.defaultVisible) row.style.display = (group.filtered) ? 'none' : '';
				else row.style.display = 'none';
			}
		}
	}
	if (state == 'all' && !active) { // reset the icons
		var div = this.parentNode;
		var spans = div.getElementsByTagName('a');
		for (var i = 0; i < spans.length; i++) {
			var span = spans[i];
			span.className = span.className.replace(' f_selected','');
			span.title = span.title.replace('show','hide');
		}
	} else {
		if (active) {
			this.className = this.className.replace(' f_selected','');
			this.title = this.title.replace('show','hide');
		} else { 
			this.className += ' f_selected';
			this.title = this.title.replace('hide','show');
		}
	}
	forceFileTableRedraw();
}

/* Function that creates Group Table Filters
 * @param node Node where to put this
 */
function createGroupFilters(node) {
  var icons = new Array('complete','finished','ongoing','stalled','dropped','specials only','all');
  for (var i = 0; i < icons.length; i++) {
    var icon = icons[i];
    var desc = (icon != 'all') ? 'toggle group rows with state: '+icon : 'reset group filter';
    var ico = createIcon(null, icon, 'removeme', filterGroups, desc, 'i_gstate_'+icon.substring(0,(icon.indexOf(' ') >= 0) ? icon.indexOf(' ') : icon.length));
    node.appendChild(ico);
  }
}

/* Function that updates the group table */
function updateGroupTable() {
	var groupTable = document.getElementById('grouplist');
	if (!groupTable) return;
	var tbody = groupTable.tBodies[0];
	var thead = null;
	if (LAY_HEADER) {
		thead = document.createElement('thead');
		thead.appendChild(tbody.rows[0]);
		groupTable.insertBefore(thead,tbody);
	}
	var tfoot = document.createElement('tfoot');
	tfoot.appendChild(tbody.rows[tbody.rows.length-1]);
	groupTable.appendChild(tfoot);
	var groupCnt = tbody.rows.length;
	var newTbody = document.createElement('tbody');
	for (var g = 0; g < anime.groups.length; g++) {
		var group = groups[anime.groups[g]];
		if (!group || group && group.id == 0) continue; // not interested in non groups nor the no group
		// update existing rows
		var gid = group.id;
		if (!groups[gid]) continue; // not interested
		var row = document.getElementById('gid_'+gid);
		if (!row) {
			row = createGroupRow(group.id, groupCols, groupSkips);
			row.style.display = 'none';
			newTbody.appendChild(row);
		} else { // update stuff
			lastKnown = row.rowIndex;
			group.defaultVisible = true;
			var cells = row.getElementsByTagName('td');
			// hook up the expand function
			for (var c = 0; c < cells.length; c++) {
				var cell = cells[c];
				var className = cell.className;
				if (className.indexOf('expand') >= 0) {
					var ahref = cell.getElementsByTagName('a')[0];
					if (uriObj['gid'] && uriObj['gid'] == group.id) ahref.onclick = foldFilesByGroup;
					else ahref.onclick = expandFilesByGroup;
					ahref.removeAttribute('href');
					ahref.style.cursor = 'pointer';
				}
				if (className.indexOf('group') >= 0)
					cell.setAttribute('anidb:sort',(group.shortName ? group.shortName.toLowerCase() : 'no group'));
				if (className.indexOf('status') >= 0)
					cell.setAttribute('anidb:sort',(group.epsInMyListArray && group.epsInMyListArray.length) ? String(group.epsInMyListArray.length) : '0');
				if (className.indexOf('epbar') >= 0)
					cell.setAttribute('anidb:sort',String(group.epCnt));
				if (className.indexOf('lastep') >= 0)
					cell.setAttribute('anidb:sort',mapEpisodeNumber(group.lastEp));
				if (className.indexOf('rating') >= 0)
					cell.setAttribute('anidb:sort',(group.rating == '-') ? '0' : group.rating);
				if (className.indexOf('cmts') >= 0)
					cell.setAttribute('anidb:sort',group.commentCount);
				if (className.indexOf('action') >= 0) {
					if (uriObj['gid'] && uriObj['gid'] == gid && gid >= 0)
						createCheckBox(cell,'ck_g'+group.id,'ck_g'+group.id,toggleFilesFromGroup,false);
				}
			}
			newTbody.appendChild(row);
		}
	}
	groupTable.replaceChild(newTbody,tbody);
	if (LAY_HEADER && thead) {
		// update thead with sorting functions
		var headingList = thead.rows[0].getElementsByTagName('th');
		headingTest = getElementsByClassName(headingList,'group',true)[0];
		if (headingTest) headingTest.className += ' c_setlatin';
		headingTest = getElementsByClassName(headingList,'eps',true)[0];
		if (headingTest) headingTest.className = headingTest.className.replace('eps','epbar') + ' c_set';
		headingTest = getElementsByClassName(headingList,'specials',true)[0];
		if (headingTest) headingTest.className += ' c_latin';
		var head = headingTest = getElementsByClassName(headingList,'epno last',true)[0];
		if (headingTest) headingTest.className = headingTest.className.replace('epno last','epno lastep') + ' c_set';
		headingTest = getElementsByClassName(headingList,'state',true)[0];
		if (headingTest) headingTest.className += ' c_latin';
		headingTest = getElementsByClassName(headingList,'lastupdate',true)[0];
		if (headingTest) headingTest.className += ' c_date';
		headingTest = getElementsByClassName(headingList,'rating',true)[0];
		if (headingTest) headingTest.className += ' c_set';
		headingTest = getElementsByClassName(headingList,'cmts',true)[0];
		if (headingTest) headingTest.className += ' c_set';
		init_sorting(thead.rows[0],'epno','up');
	}
	// add filtering and stuff to tfoot
	var row = tfoot.rows[0];
	var cells = row.getElementsByTagName('td');
	var cell = getElementsByClassName(cells, 'expand', true)[0];
	if (cell) {
		var ahref = cell.getElementsByTagName('a')[0];
		if (uriObj['gid'] && uriObj['gid'] == '0') ahref.onclick = foldFilesByGroup;
		else ahref.onclick = expandFilesByGroup;
		ahref.removeAttribute('href');
		ahref.style.cursor = 'pointer';
	}
	// append some group status filters
	cell = row.cells[row.cells.length-2]; // second to last cell
	cell.className = 'action filter';
	var span = document.createElement('span');
	span.className = 'icons';
	createGroupFilters(span)
	cell.appendChild(span);
	cell = getElementsByClassName(cells, 'action', false)[0];
	var ahref = cell.getElementsByTagName('a')[0];
	if (ahref) { // There are cases when we have nothing to expand
		ahref.removeAttribute('href');
		ahref.style.cursor = 'pointer';
		if (uriObj['showallag'] && uriObj['showallag'] == 1) {
			ahref.onclick = foldGroupTable;
			ahref.firstChild.nodeValue = 'show default';
			doGroupTableOperations('expand'); // Repaint stripes
		} else {
			ahref.onclick = expandGroupTable;
			doGroupTableOperations('fold'); // Repaint stripes
		}
	} else { // but we should have something to see no?
		if (groupCnt != tbody.rows.length) { // we do! show the link
			var a = createTextLink(null,'show all','removeme', null,expandGroupTable,null,null);
			cell.appendChild(a);
			doGroupTableOperations('fold'); // Repaint stripes
		}
	}
}

/* -[EPISODE]-------------------------
 * EPISODE TABLE AND RELATED FUNCTIONS
 * -----------------------------------
 */

/* Function that folds files by episode */
function foldEp() {
	var row = this.parentNode.parentNode;
	var elem = document.getElementById(row.id+'_ftHolder');
	if (elem) elem.style.display = 'none';
	this.onclick = expandEp;
	this.alt = this.title = 'Expand this entry';
	this.firstChild.firstChild.nodeValue = '+';
	this.className = 'i_icon i_plus';
}

/* Function that expands files by episode */
function expandEp() {
	var row = this.parentNode.parentNode;
	var eid = row.id.split('_')[1];
	var getXML = false;
	if (!document.getElementById('eid_'+eid+'_ftHolder')) {
		var nRow = createLoadingRow(10); //the new episode row
		nRow.id = 'eid_' + eid + '_ftHolder';		
		row.parentNode.insertBefore(nRow,row.nextSibling);
		getXML = true;
		var req = xhttpRequest();
		if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/eid'+eid+'.xml', parseEpisodeData);
		else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=ep&aid='+uriObj['aid']+'&eid='+eid, parseEpisodeData);
	} else {
		document.getElementById('eid_'+eid+'_ftHolder').style.display = '';
	}
	if (!getXML) this.onclick = foldEp;
	else this.onclick = null;
	this.alt = this.title = 'Fold this entry';
	this.firstChild.firstChild.nodeValue = '-';
	this.className = 'i_icon i_minus';
	uriObj['expand'] = eid;
	uriObj['#'] = 'eid_'+eid;
}

/* Function that updates the episode table */
function updateEpisodeTable() {
	var episodeTable = document.getElementById('eplist');
	if (!episodeTable) return;
	var tbody = episodeTable.tBodies[0];
	if (LAY_HEADER) {
		var thead = document.createElement('thead');
		thead.appendChild(tbody.rows[0]);
		episodeTable.insertBefore(thead,tbody);
	}
	for (var e in episodes) {
		var episode = episodes[e];
		if (!episode) continue;
		var eid = episode.id;
		var row = document.getElementById('eid_'+eid);
		if (row) { // update row
			var cells = row.getElementsByTagName('td');
			for (var c = 0; c < cells.length; c++) {
				var cell = cells[c];
				var cname = cell.className;
				if (cname.indexOf('expand') >= 0) {
					var ahref = cell.getElementsByTagName('a')[0];
					if (ahref) {
						if (uriObj['eid'] && uriObj['eid'] == eid) ahref.onclick = foldEp;
						else ahref.onclick = expandEp;
						ahref.removeAttribute('href');
					} else // someone called expand gid, add the ep expand icon
						ahref = createIcon(cell, '+', 'removeme', foldEp, "Expand this entry", 'i_plus');
				}
				if (cname.indexOf('title') >= 0) {
					var mylistEpEntries = findMylistEpEntries(eid);
					var altTitle = jaTitle = curTitle = '';
					var titleSpan = cell.getElementsByTagName('label')[0];
					if (titleSpan) curTitle = nodeData(titleSpan);
					if (episodeTitleLang != episodeAltTitleLang && 
						episode.titles[episodeAltTitleLang] && 
						episode.titles[episodeAltTitleLang]['title'] != '' &&
						episode.titles[episodeAltTitleLang]['title'] != curTitle) altTitle = episode.titles[episodeAltTitleLang]['title'];
					if (episodeTitleLang != 'ja' &&
						episodeAltTitleLang != 'ja' &&
						episode.titles['ja'] && 
						episode.titles['ja']['title'] != '' && 
						episode.titles['ja']['title'] != altTitle) jaTitle = episode.titles['ja']['title'];
					if (altTitle != '' || jaTitle != '') {
						if (episodeTitleDisplay == 1 || (episodeTitleDisplay == 4 && jaTitle == ''))
							titleSpan.firstChild.nodeValue += ' ('+altTitle+')';
						if (episodeTitleDisplay == 2 || (episodeTitleDisplay == 3 && jaTitle == '')) 
							titleSpan.title = mapLanguage(episodeAltTitleLang) + ' title: '+ altTitle;
						if (episodeTitleDisplay == 3 && jaTitle != '') {
							var titleTag = '';
							if (altTitle != '') titleTag = mapLanguage(episodeAltTitleLang) + ' title: '+ altTitle + ' / ';
							titleSpan.title = titleTag + jaTitle;
						}
						if (episodeTitleDisplay == 4 && jaTitle != '') {
							var titleTag = ' (';
							if (altTitle != '') titleTag += altTitle+' / ';
							titleSpan.firstChild.nodeValue += titleTag +jaTitle+ ')';
						}
					}
					if (mylistEpEntries.length) { // A neat part now, state icons
						var icon;
						// Loop to see if an entry should get a status, and file state
						var span = cell.getElementsByTagName('span')[0];
						if (span) {
							while (span.childNodes.length) span.removeChild(span.firstChild);
							var icons = createEpisodeIcons(episode);
							if (icons['recap']) span.appendChild(icons['recap']);
							if (icons['comment']) span.appendChild(icons['comment']);
							if (icons['seen']) span.appendChild(icons['seen']);
							if (icons['state']) for (var st = 0; st < icons['state'].length; st++) span.appendChild(icons['state'][st]);
							if (icons['fstate']) for (var st = 0; st < icons['fstate'].length; st++) span.appendChild(icons['fstate'][st]);
						}
					}
				}
				if (cname.indexOf('date') >= 0) {
					if (episode.relDate) // This episode has a release date, so we add the the db add date
						cell.title = 'DB add date: '+ episode.addDate;
				}
			}
		}
	}
}

/* Function that gives some indicators to what is happening
 * @param eid Episode eid in which the loading row should be visible
 * @param msg Message to display
 * @param append If true msg will be appended to the end of the existing text, otherwise msg will replace the current text
 */
function updateLoadingStatus(eid, msg, append) {
	var loadingRow = document.getElementById('eid_'+eid+'_ftHolder');
	if (!loadingRow || (loadingRow && loadingRow.cells[0].className != 'loading')) return;
	var cell = loadingRow.cells[0];
	var currentText = cell.firstChild.nodeValue;
	if (append) cell.firstChild.nodeValue = currentText + msg;
	else cell.firstChild.nodeValue = 'please wait while loading data...' + msg;
}

/* -[FILE]-------------------------
 * FILE TABLE AND RELATED FUNCTIONS
 * --------------------------------
 */


/* Function that marks a file (un)watched */
function changeWatchedState() {
	var row = this.parentNode;
	var isWatched = (this.className.indexOf('i_seen_yes') >= 0);
	while (row.nodeName != 'TR') row = row.parentNode;
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
				var erow = document.getElementById('eid_'+eid);
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
						if (icons['recap']) span.appendChild(icons['recap']);
						if (icons['comment']) span.appendChild(icons['comment']);
						if (icons['seen']) span.appendChild(icons['seen']);
						if (icons['state']) for (var st = 0; st < icons['state'].length; st++) span.appendChild(icons['state'][st]);
						if (icons['fstate']) for (var st = 0; st < icons['fstate'].length; st++) span.appendChild(icons['fstate'][st]);
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
				if (mylist[episode.files[fe]] && mylist[episode.files[fe]].seenDate) epWatchedCount++;
			}
			if (episode.files.length == 1 || !epWatchedCount) { // there are now more watched files
				episode.seenDate = 0; // updated episode entry
				var erow = document.getElementById('eid_'+eid);
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
/* Function that changes the state of a mylist entry
 * @param action Action to perform (add/remove)
 * @param node The node element that triggered the action
 */
function changeMylistState(action,node) {
	var row = node.parentNode;
	while (row.nodeName.toLowerCase() != 'tr') row = row.parentNode;
	var fid = Number(row.id.substr(row.id.indexOf('f')+1,row.id.length));
	var url = 'animedb.pl?show=mylist&';
	if (action == 'add' && !use_mylist_add) { // make sure the user has the url and return this function
		node.href = url + 'do=add&fid=' + fid;
		return true;
	}
	if (node.href) node.removeAttribute('href');
	var mylistEntry = mylist[fid];
	var file = files[fid];
	if (action == 'remove') {
		url += 'do=del&lid='+mylistEntry.id;
		mylist[fid] = null; // clear mylist entry
	} else {
		var mylistNode = document.createElement('file');
		mylistNode.setAttribute('id','-1');
		mylistNode.setAttribute('eid',file.episodeId);
		mylistNode.setAttribute('fid',file.id);
		mylistNode.setAttribute('gid',file.groupId);
		mylistNode.appendChild(document.createElement('state').appendChild(document.createTextNode('unknown')));
		mylist[fid] = new CMylistEntry(mylistNode);
		if (mylist_add_viewed_state) {
			var now = new Date();
			var day = (now.getDate() > 9 ? now.getDate() : '0'+now.getDate());
			var month = (((now.getMonth() + 1) > 9) ? (now.getMonth()+1) : '0'+(now.getMonth()+1));
			now = day + '.' + month + '.' + now.getFullYear();
			mylist[fid].seenDate = now;
			mylist[fid].seen = 1;
		}
		mylist[fid].status = mapMylistState(mylist_add_state);
		mylistEntry = mylist[fid];
		url += 'addl.aid='+file.animeId+'&addl.eid='+file.episodeId+'&addl.fid='+file.id;
		url += '&addl.source=&addl.storage=';
		url += '&addl.viewed='+mylist_add_viewed_state;
		url += '&addl.state='+mylist_add_state;
		url += '&addl.filestate='+mylist_add_fstate;
		url += '&addl.other=&addl.doit=Add';
	}
	var file = files[fid];
	var eids = new Array();
	eids[file.episodeId] = 1;
	for (var eid in file.epRelations) eids[eid] = 1;
	for (var eid in eids) {
		if (!eids[eid]) continue;
		var row = document.getElementById('e'+eid+'f'+file.id);
		var eidFilesRow = document.getElementById('eid_'+eid+'_ftHolder');
		var eidRow = document.getElementById('eid_'+eid);
		var fileTable = row.parentNode;
		var cells = row.getElementsByTagName('td');
		var actionCell = getElementsByClassName(cells, 'icons action', true)[0];
		var as = actionCell.getElementsByTagName('a');
		var mylistCell = getElementsByClassName(cells, 'icons state', true)[0];
		var icons = createFileIcons(file);
		var episode = episodes[eid];
		if (action == 'remove') {
			while(mylistCell.childNodes.length) mylistCell.removeChild(mylistCell.firstChild);
			var aRemove			= getElementsByClassName(as, 'removemylist', true)[0];
			var aWatchedState	= getElementsByClassName(as, 'seen', true)[0];
			if (aWatchedState) actionCell.removeChild(aWatchedState);
			if (aRemove) actionCell.replaceChild(icons['mylist_add'],aRemove);
			var epWatchedCount = 0;
			for (var fe in episode.files) { // find how many watched files we have (the current file has already been marked unwatched)
				if (mylist[episode.files[fe]] && mylist[episode.files[fe]].seenDate) epWatchedCount++;
			}
			if (!epWatchedCount) episode.seenDate = 0;
		} else {
			var aAdd = getElementsByClassName(as, 'addmylist', true)[0];
			if (aAdd) {
				var removeWithoutAction = icons['mylist_remove'];
				removeWithoutAction.removeAttribute('href');
				removeWithoutAction.onclick = function showWarning() { alert('Action not available to quick-add files.\nPlease reload the page to act upon the file'); }
				actionCell.replaceChild(removeWithoutAction,aAdd);
			}
			var watchWithoutAction = icons['mylist_watch'];
			watchWithoutAction.removeAttribute('href');
			watchWithoutAction.onclick = function showWarning() { alert('Action not available to quick-add files.\nPlease reload the page to act upon the file'); }
			actionCell.appendChild(watchWithoutAction);
			// mylist cell
			if (mylistCell) {
				while (mylistCell.childNodes.length) mylistCell.removeChild(mylistCell.firstChild);
				if (icons['mylist']) mylistCell.appendChild(icons['mylist']);
				if (icons['mylist_status']) mylistCell.appendChild(icons['mylist_status']);
				if (icons['mylist_fstate']) mylistCell.appendChild(icons['mylist_fstate']);
				if (icons['mylist_seen']) mylistCell.appendChild(icons['mylist_seen']);
				if (icons['mylist_cmt']) mylistCell.appendChild(icons['mylist_cmt']);
			}
		}
		var cell = getElementsByClassName(eidRow.getElementsByTagName('td'), 'title', true)[0];
		if (cell) {
			var span = cell.getElementsByTagName('span')[0];
			if (span) { // first remove all the icons and then add all the needed icons
				while (span.childNodes.length) span.removeChild(span.firstChild);
				var icons = createEpisodeIcons(episode);
				if (icons['seen']) span.appendChild(icons['seen']);
				if (icons['state']) for (var st = 0; st < icons['state'].length; st++) span.appendChild(icons['state'][st]);
				if (icons['fstate']) for (var st = 0; st < icons['fstate'].length; st++) span.appendChild(icons['fstate'][st]);
			}
		}
	}
	/* TODO:
	 * #1 Update mylist info box episode counters/mylist tab episode counters
	 * #2 Update group epbars with mylist episode information
	 */
	// submit data to the server
	postData(url);
	return true;
}
/* Function that removes a file from mylist */
function removeFromMylist() { return changeMylistState('remove',this); }
/* Function that adds a file to Mylist */
function addToMylist() { return changeMylistState('add',this); }

/* Function that prepares the file table for sorting */
function prepareForSort() {
	var node = this;
	while (node.nodeName.toLowerCase() != 'table') node = node.parentNode;
	var eid = Number(node.id.split("episode")[1].split("files")[0]);
	var episode = episodes[eid];
	if (!episode) return;
	var tBody = node.tBodies[0];
	for (var i = 0; i < tBody.rows.length; i++) {
		var row = tBody.rows[i];
		if (row.id.indexOf('_relftHolder') < 0) continue; //not interested
		var fid = Number(row.id.split('_')[1]);
		var parentRow = document.getElementById('fid_'+fid);
		if (!parentRow) { if (seeDebug) alert('Error while preparing for sort at fid: '+fid); continue; }
		row.parentNode.removeChild(row);
		var cell = getElementsByClassName(parentRow.getElementsByTagName('TD'),'file expand',true)[0];
		if (cell) {
			var span = cell.getElementsByTagName('SPAN')[0];
			if (span && span.className.indexOf('i_minus') >= 0) span.className = span.className.replace('i_minus','i_plus'); 
		}
	}
	sortcol(this);
}

/* Function that shows hidden files 
 * @param hide Hide files if true
 * @param node Node which called the hideFiles action
 */
function showFiles(hide,passedNode) {
	var node = this;
	if (passedNode) node = passedNode;
	var cell = null; 
	while (node.nodeName.toLowerCase() != 'table') {
		if (!cell && node.nodeName.toLowerCase() == 'td') cell = node;
		node = node.parentNode;
	}
	var eid = Number(node.id.substring(7,node.id.indexOf("files"))); 
	var tBody = node.tBodies[0];
	for (var i = 0; i < tBody.rows.length; i++) {
		var row = tBody.rows[i];
		if (row.id.indexOf('_relftHolder') >= 0) continue; //not interested
		var file = null;
		var eLen = String('e'+eid).length;
		var fid = Number(row.id.substring(row.id.indexOf('f')+1, row.id.length));
		switch (row.id.substr(eLen,1)) {
			case 'f': file = files[fid]; break;
			case 'r': file = pseudoFiles.list[fid]; break;
		}
		if (!file) continue;
		if (!passedNode) row.style.display = ''; 
		else if (!file.visible) row.style.display = 'none'; // hide
	}
	while (cell.childNodes.length) cell.removeChild(cell.firstChild);
	var i = document.createElement('i');
	var ahref;
	if (!passedNode) {
		i.appendChild(document.createTextNode('All files shown - '));
		ahref = createLink(null,'hide files', 'removeme', null, hideFiles, null, null);
	} else {
		i.appendChild(document.createTextNode(episodes[eid].hiddenFiles + ' file'+((episodes[eid].hiddenFiles > 1) ? 's' : '')+ ' not shown - '));
		ahref = createLink(null,'show all files', 'removeme', null, showFiles, null, null);			
	}
	i.appendChild(ahref);
	cell.appendChild(i);
}

/* Function that hides filtered out files */
function hideFiles() { showFiles(true,this); }

/* Expand file relations */
function expandFiles() {
	var row = this.parentNode.parentNode;
	var eid = null;
	if (!document.getElementById(row.id+'_relftHolder')) {
		var rowSibling = row.nextSibling;
		var rowParent = row.parentNode;
		var nRow = document.createElement('tr'); //the new episode row
		var nCell = document.createElement('td');
		nRow.id = row.id + '_relftHolder';
		var rfid = row.id.split('f')[1];
		eid = row.id.split((row.id.indexOf('rf') < 0) ? 'f' : 'rf')[0];
		var file = null;
		if (row.id.indexOf('rf') < 0) file = files[rfid];
		else file = pseudoFiles.list[rfid];
		nCell.colSpan = row.cells.length;
		// Relation Table
		var table = document.createElement('table');
		table.className = 'filelist';
		table.id = 'file'+rfid+'relations';
		if (LAY_HEADER)
			table.appendChild(createTableHead(fileCols));
		var tfoot = document.createElement('tfoot');
		var tbody = document.createElement('tbody');
		// TableBody
		for (var f = 0; f < file.relatedFiles.length; f++) {
			var drow = document.getElementById('e'+eid+'f'+file.relatedFiles[f]);
			var dnode;
			if (!drow) {
				var rfile = files[file.relatedFiles[f]];
				var episode = episodes[rfile.episodeId];
				drow = createFileRow(eid,rfile.id,fileCols,fileSkips);
				dnode = drow;
			} else {
				dnode = drow.cloneNode(true);
				dnode.id = 'e'+eid+'rf'+file.relatedFiles[f];
			}
			dnode.style.display = '';
			// clean up the extra node
			var ccell = getElementsByClassName(dnode.getElementsByTagName('td'),'id icons',true)[0];
			if (ccell) {
				var spans = dnode.getElementsByTagName('span');
				var ico = getElementsByClassName(spans,'i_icon i_plus',true)[0];
				if (ico) ccell.removeChild(ico);
				else {
					ico = getElementsByClassName(spans,'i_icon i_minus',true)[0];
					if (ico) ccell.removeChild(ico);
				}
			}
			tbody.appendChild(dnode);
		}
		table.appendChild(tbody);
		// END here
		nCell.appendChild(table);
		nRow.appendChild(nCell);
		rowParent.insertBefore(nRow,rowSibling);
	} else {
		document.getElementById(row.id+'_relftHolder').style.display = '';
	}
	this.onclick = foldFiles;
	this.title = 'fold this entry';
	this.alt = '[-]';
	this.className = 'i_icon i_minus';
}

/* Fold file relations */
function foldFiles() {
	var row = this.parentNode.parentNode;
	document.getElementById(row.id+'_relftHolder').style.display = 'none';
	this.onclick = expandFiles;
	this.title = 'expand this entry';
	this.alt = '[+]';
	this.className = 'i_icon i_plus';
}

/* Function that creates a file table for a given episode
 * @param episode The episode data
 */
function createFileTable(episode) {
	if (!episode) return;
	var eid = episode.id;
	episode.hiddenFiles = 0;
	var table = document.createElement('table');
	table.className = 'filelist';
	table.id = 'episode'+eid+'files';
	if (LAY_HEADER)
		table.appendChild(createTableHead(fileCols));
	var tfoot = document.createElement('tfoot');
	var tbody = document.createElement('tbody');
	for (var f = 0; f < episode.files.length; f++) {
		var file = files[episode.files[f]];
		if (!file) continue;
		if (expandedGroups && groupFilter.indexOf(file.groupId) < 0) continue;
		var row = createFileRow(eid,episode.files[f],fileCols,fileSkips);
		// Filtering stuff
		if (!file.pseudoFile || file.type != 'stub') {
			filterObj.markDeprecated(file);
			filterObj.markUnfiltered(file);
			if (LAY_HIDEFILES && file.isDeprecated) file.visible = false;
			filterObj.markVisible(file);
			filterObj.markHidden(file);
			if (!file.visible) episode.hiddenFiles++;
		} else file.visible = false; // STUB files should be very hidden, very hidden indeed..
		if (!expandedGroups) {
			if (groups[file.groupId] && !groups[file.groupId].visible) row.style.display = 'none';
			if (!file.visible || (file.type == 'generic' && LAY_HIDEGENERICFILES)) row.style.display = 'none';		
		}
		if (file.type == 'generic') row.className = "generic no_sort";
		else {
			if (file.crcStatus == 'valid') row.className = 'good';
			if (file.crcStatus == 'invalid') row.className = 'bad';
			if (file.isDeprecated) row.className = 'deprecated';
			if (!file.avdumped) row.className += ' undumped';
		}
		if (row.className.indexOf('generic') < 0) tbody.appendChild(row);
		else tfoot.appendChild(row);
	}
	repaintStripes(tbody);
	if (!expandedGroups && episode.hiddenFiles) {
		var row = document.createElement('tr');
		var i = document.createElement('i');
		i.appendChild(document.createTextNode(episode.hiddenFiles + ' file'+((episode.hiddenFiles > 1) ? 's' : '')+ ' not shown - '));
		var ahref = createLink(null,'show all files', 'removeme', null, showFiles, null, null);
		i.appendChild(ahref);
		createCell(row, null, i, null, fileCols.length);
		tfoot.appendChild(row);
	}
	table.appendChild(tbody);
	table.appendChild(tfoot);
	var epHolder = document.getElementById('eid_'+eid+'_ftHolder');
	var cell = epHolder.cells[0];
	cell.className = '';
	cell.replaceChild(table,cell.firstChild);
	// fix the sorting function
	if (LAY_HEADER) {
		init_sorting(table,'title','down');
		var ths = table.tHead.getElementsByTagName('th');
		for (var i = 0; i < ths.length; i++) ths[i].onclick = prepareForSort;
	}
	return table;
}

/* Function that parses Episode data
 * @param xmldoc The xml document to parse
 */
function parseEpisodeData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) return;
	updateStatus('Processing anime episode(s)...');
	var animesNode = root.getElementsByTagName('animes')[0];
	if (!animesNode) return;
	var animeNodes = animesNode.getElementsByTagName('anime');
	for (var i = 0; i < animeNodes.length; i++) {
		if (animeNodes[i].parentNode.nodeName != 'animes') continue; // wrong node
		var aid = Number(animeNodes[i].getAttribute('id'));
		var epNodes = animeNodes[i].getElementsByTagName('ep');
		var filedataNodes = animeNodes[i].getElementsByTagName('filedata');
		var t1 = new Date();
		for (var k = 0; k < filedataNodes.length; k++)
			parseFiledata(filedataNodes[k], aid);
		var epNodestime = (new Date()) - t1;
		t1 = new Date();
		processingEps = true; // make the table creation wait
		for (var k = 0; k < epNodes.length; k++) {
			// if we are handling just one episode do stuff now otherwise defer execution
			var eid = Number(epNodes[k].getAttribute('id'));
			updateLoadingStatus(eid, 'parsing episode data', false);
			parseEpisode(epNodes[k], aid);
			var episode = episodes[eid];
			if (!episode) continue;
			var eprow = document.getElementById('eid_'+episode.id);
			var a = eprow.getElementsByTagName('a')[0];
			if (a) a.onclick = foldEp;
			if (loadExpand) { // Normal behaviour
				var eprowid = eprow.rowIndex + 1;
				var tbRow = document.getElementById('eid_'+eid+'_ftHolder');
				if (tbRow) {
					var rmTable = tbRow.getElementsByTagName('table')[0];
					if (rmTable) tbRow.cells[0].removeChild(rmTable);
				}
			}
			if (!internalExpand) {
				updateLoadingStatus(eid, 'creating file table', false);
				createFileTable(episode);
				document.getElementById('eid_'+episode.id+'_ftHolder').cells[0].className = '';
			}
		}
		processingEps = false;
		pseudoFilesCreator(); // create pseudo files
		if (loadExpand) loadExpand = false;
		if (seeTimes) alert('Processing...\n\tepNodes: '+((new Date()) - t1)+' ms\n\tfiledataNodes: '+epNodestime+' ms');
	}
	// Little Hack for the expand all by group
	if (internalExpand) {
		// find file tables without files
		for (var e in episodes) {
			var episode = episodes[e];
			if (!episode) continue;
			forceFileTableRedraw(episode);
		}
	}
	updateStatus('');
}

/* -[ADDTOMYLIST]---------------------
 * ADD ANIME/FILES TO MYLIST FUNCTIONS
 * -----------------------------------
 */

/* Creates a field Row
 * @param parentNode parentNode where to append this row
 * @param className row this row classname
 * @param field header text
 * @param valueElement the element to add as data
 */
function createFieldValueRow(parentNode, className, field, valueElement) {
	var row = document.createElement('tr');
	row.className = className;
	createHeader(row, 'field', field, null);
	createCell(row, 'value', valueElement, null);
	if (parentNode) parentNode.appendChild(row);
	else return row;
}

/* Function that changes the generic add type */
function changeGenAddType () {
	var select = getElementsByName(addanimetomylistbox.getElementsByTagName('select'),'addl.genaddtype',false)[0];
	if (!select) return;
	var input = getElementsByName(addanimetomylistbox.getElementsByTagName('input'),'addl.upto',false)[0];
	if (!input) return;
	if (select.value == 0) input.disabled = false;
	if (select.value == 1) input.disabled = true;
}
/* Shows the Add Anime to Mylist Box */
function showAddAnimeToMylistBox() {
	this.className = 'i_inline i_minus';
	this.title = 'show simple Add Anime to Mylist box';
	var parentNode = simpleaddanimetomylistbox.parentNode;
	parentNode.replaceChild(createMylistAddBox(null,'anime'),simpleaddanimetomylistbox);
	simpleaddanimetomylistbox = null;
}

/* Shows the Add (files) To Mylist Box */
function showAddToMylistBox() {
	if (!getElementsByClassName(document.getElementsByTagName('div'),'g_definitionlist mylist_add',false).length || addanimetomylistbox || simpleaddanimetomylistbox) {
		// we don't have the mylist add to thingie, as i allways show checkboxes i need that
		var eplist = document.getElementById('eplist');
		if (!eplist) return;
		if (!addanimetomylistbox) createMylistAddBox(eplist.parentNode,'files');
		else {
			if (simpleaddanimetomylistbox) eplist.parentNode.replaceChild(addfilestomylistbox,simpleaddanimetomylistbox);
			else eplist.parentNode.replaceChild(addfilestomylistbox,addanimetomylistbox);
			addanimetomylistbox = null;
		}
	}
	addfilestomylistbox.style.display = '';
	if (addanimetomylistbox && addanimetomylistbox.parentNode) addanimetomylistbox.parentNode.removeChild(addanimetomylistbox);
	if (simpleaddanimetomylistbox && simpleaddanimetomylistbox.parentNode) {
		simpleaddanimetomylistbox.parentNode.removeChild(simpleaddanimetomylistbox);
		simpleaddanimetomylistbox = null;
	}
}

/* Creates the extended Mylist Add Box
 * @param parentNode Parent Node to append this table
 * @param type Type of mylist box - anime, files
 */
function createMylistAddBox(parentNode,type) {
	var div = document.createElement('div');
	div.className = "g_definitionlist mylist_add";
	if (type == 'files') div.style.display = 'none';
	var table = document.createElement('table');
	var caption = document.createElement('caption');
	if (type == 'files')
		caption.appendChild(document.createTextNode('Add selected files to mylist'));
	if (type == 'anime') {
		caption.appendChild(document.createTextNode('Add anime to mylist'));
		var input = createTextInput('addl.fullanime',null,false,true);
		input.value = '1';
		div.appendChild(input);
	}
	table.appendChild(caption);
	var i = 0;
	var tbody = document.createElement('tbody');
	var select; var row; var input;
	if (type == 'anime') {
		select = createSelectArray(null,"addl.genaddtype","addl.genaddtype",changeGenAddType,0,{0:{"text":'episodes up to'},1:{"text":"all episodes+specials"}});
		row = createFieldValueRow(null,'genaddtype'+gODD(i),'Add',select);
		row.getElementsByTagName('td')[0].appendChild(document.createTextNode(' '));
		input = createTextInput('addl.upto',3,false,false,3);
		input.value = anime.eps;
		row.getElementsByTagName('td')[0].appendChild(input);
		tbody.appendChild(row); i++;
	}
	select = createSelectArray(null,"addl.viewed","addl.viewed",null,((type == 'anime') ? 1 : 0),{0:{"text":'unwatched'},1:{"text":'watched'}});
	createFieldValueRow(tbody,'watched'+gODD(i),'Watched',select); i++;
	var optionArray = {0:{"text":'unknown'},1:{"text":'internal storage (hdd)'},
						2:{"text":'external storage (cd/dvd/...)'},3:{"text":'deleted'}};
	select = createSelectArray(null,"addl.state","addl.state",null,0,optionArray);
	var row = createFieldValueRow(null,'state'+gODD(i),'State',select); i++;
	var inlineHelp = document.createElement('a');
	inlineHelp.className = 'i_inline i_help';
	inlineHelp.href = 'http://wiki.anidb.net/w/myliststate';
	inlineHelp.rel = 'popup 300-450-1-1-helppopup';
	var span = document.createElement('span');
	span.appendChild(document.createTextNode('help'));
	inlineHelp.appendChild(span);
	row.getElementsByTagName('td')[0].appendChild(inlineHelp);
	tbody.appendChild(row);
	if (type == 'files') {
		optionArray = {0:{"text":' normal/original '},1:{"text":' corrupted version/invalid crc '},
						2:{"text":' self edited '},100:{"text":' other '}};
		select = createSelectArray(null,"addl.filestate","addl.filestate",null,0,optionArray);
		createFieldValueRow(tbody,'type'+gODD(i),'Type',select); i++;
	}
	if (type == 'anime') {
		optionArray = {100:{"text":' other '},10:{"text":' self ripped '},11:{"text":' on dvd '},
						 12:{"text":' on vhs '},13:{"text":' on tv '},14:{"text":' theater '}};
		select = createSelectArray(null,"addl.genericstate","addl.genericstate",null,((type == 'anime') ? 13 : 100),optionArray);
		createFieldValueRow(tbody,'type'+gODD(i),'Type Gen',select); i++;
	}
	createFieldValueRow(tbody,'source'+gODD(i),'Source',createTextInput("addl.source",30,false,false,100)); i++;
	createFieldValueRow(tbody,'storage'+gODD(i),'Storage',createTextInput("addl.storage",30,false,false,100)); i++;
	createFieldValueRow(tbody,'other'+gODD(i),'Other',createTextBox('addl.other',null,25,4,null)); i++;
	row = createFieldValueRow(null,'note'+gODD(i),'Note'); i++;
	var cell = row.getElementsByTagName('td')[0];
	if (type == 'anime') {
		cell.appendChild(document.createTextNode('You can add this anime to your mylist with the form above using generic files. Expand per group if you wish to add specific files.'));
		cell.appendChild(document.createElement('br'));
	}
	cell.appendChild(document.createTextNode('Manual adding is '));
	var b = document.createElement('b');
	b.appendChild(document.createTextNode('not recommend'));
	cell.appendChild(b);
	cell.appendChild(document.createTextNode('. The best way to add files to your mylist is by using an '));
	cell.appendChild(createTextLink(null,'AniDB Client',"http://wiki.anidb.net/w/AniDB_Clients","anidb::wiki",null, null,null));
	cell.appendChild(document.createTextNode('. There is also a ready-to-run '));
	cell.appendChild(createTextLink(null,'Java Applet',"http://static.anidb.net/client/webaom.htm","anidb::popup",null, null,null));
	cell.appendChild(document.createTextNode('.'));
	tbody.appendChild(row);
	row = document.createElement('tr');
	row.className = 'action';
	input = document.createElement('input');
	input.type = 'submit';
	input.name = 'addl.doadd';
	input.value = 'Add';
	createCell(row, null, input, null, 2);
	tbody.appendChild(row);
	table.appendChild(tbody);
	div.appendChild(table);
	if (type == 'files') addfilestomylistbox = div;
	if (type == 'anime') addanimetomylistbox = div;
	if (parentNode) parentNode.appendChild(div);
	else return div;
}

/* This function updates the add to mylist box */
function updateAddToMylistBox() {
	if (!getElementsByClassName(document.getElementsByTagName('div'),'g_definitionlist mylist_add',false).length) {
		// we don't have the mylist add to thingie, as i allways show checkboxes i need that
		var eplist = document.getElementById('eplist');
		if (!eplist) return;
		createMylistAddBox(null,'files');
	}
	// replace the existing box with my box :P (box in a box, funny? well, i guessed not)
	simpleaddanimetomylistbox = getElementsByClassName(document.getElementsByTagName('p'),'mylistadd',false)[0];
	if (simpleaddanimetomylistbox) {
		// Add an expand inline icon to allow expanding to a full version of the generic add thing
		var span = simpleaddanimetomylistbox.getElementsByTagName('span')[0];
		if (span) {
			var expand = document.createElement('span');
			expand.className = 'i_inline i_plus';
			expand.title = 'Show full Add Anime to Mylist box';
			expand.onclick = showAddAnimeToMylistBox;
		}
		simpleaddanimetomylistbox.insertBefore(expand,span);
	}
}

// hook up the window onload event
addLoadEvent(prepPage);
