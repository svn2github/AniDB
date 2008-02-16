/* *
 * @file animePage interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (22.03.2007)
 */

// GLOBALs //

var uid;                       // userID
var mod;                       // isMod
var anime;                     // anime Object (used in animePage)
var animes = new Array();      // stored by aid
var animeOrder = new Array();  // episodes ordered in db way (link to eid)
//var groupOrder = new Array();   // ordered group list (filtering porpuses)
var groups = new Array();      // stored by gid
var aGroups = new Array();     // stored by agid (gid to link groups)
var mylist = new Array();      // stored by fid
var episodes = new Array()     // stored in eid
var epOrder = new Array()      // episodes ordered in db way (link to eid)
var files = new Array();       // stored by fid
var epQueue = new Array();     // episode process queue
// settings
var animeTitleLang = '';
var animeAltTitleLang = 'en';
var episodeTitleLang = '';
var episodeAltTitleLang = 'en';
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
//var LAY_HIDEEPTITLES = false;
//var LAY_FILEHIGHLIGHT = true;
//var LAY_PSEUDOFILES = true;
var LAY_FORMATFILESIZE = false;
//var LAY_FILTERFILES = false;
var useLangFilters = true;
var filterAudLang = new Array();
var filterSubLang = new Array();
//var useFancyStyles = false;
var seeDebug = false;
var seeTimes = false;
var epInterval = 250; // This one needs work
var epInt; // Interval ID
var processingEps = false; // are we processing episodes?
var hideNoGroup = false;
var hideAllGroups = false;
var hiddenGroups = 0;
var tFileHeadRow = null;
var tFileRow = null;
var loadExpand = false;
var internalExpand = false;
var expandedGroups = 0;
var showNoGroup = true;
var expandAllG = false;
var base_url = 'http://static.anidb.net/';
var group_vis = {'complete':false,'finished':false,'ongoing':false,'stalled':false,'dropped':false,'specials':false,'all':true};
var addfilestomylistbox = null;
var addanimetomylistbox = null;

function changeValueOfOption() {
  switch (this.id) {
    case 'seeDebug' : seeDebug = this.checked; break;
    case 'seeTimes' : seeTimes = this.checked; break;
    case 'LAY_HIDEFILES' : LAY_HIDEFILES = this.checked; break;
    case 'pseudoFiles.create' : pseudoFiles.create = this.checked; break;
    case 'pseudoFiles.noHide' : pseudoFiles.noHide = this.checked; break;
  }
}

function preLoad() {
  // some other stuff, used only in dev 
  if (''+window.location.hostname != '') {
    var anime3 = getElementsByClassName(document.getElementsByTagName('DIV'), 'anime3', true)[0];
    if (anime3) anime3.className = anime3.className.replace("anime3","anime"); // this will correct css used in dev
  }
  loadData();
  return;
}

function loadData() {
  uriObj = parseURI(); // update the uriObj
  if (uriObj['aid'] && uriObj['show'] && uriObj['show'].indexOf('anime') >= 0) aid = uriObj['aid'];
  else return;
  var req = xhttpRequest();
  if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/aid'+aid+'.xml', parseData);
  else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=anime&dev=1&aid='+aid, parseData);
}

function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { if (seeDebug) alert('Error: Could not get root node'); return; }
	var t1 = new Date();
	parseAnimes(root.getElementsByTagName('animes'));
	var parseAnimeNode = (new Date()) - t1;
	updateStatus('Processing user data...');
	var t1 = new Date();
	parseCustom(root.getElementsByTagName('custom').item(0));
	var parseCustomNode = (new Date()) - t1;
	if (seeTimes) alert('Processing...\n'+
						'\n\tanimes: '+parseAnimeNode+'ms'+
						'\n\tcustom: '+parseCustomNode+' ms'+
						'\nTotal: '+(parseAnimeNode+parseCustomNode)+' ms');
	updateStatus('');
	if (uriObj['expandall']) loadExpand = true;
	initTooltips();
	updateEpisodeTable(); // Update the episode table
	loadExpand = false;
	updateGroupTable(); // Update the group table
	checkEpExpand(); // Check to see if the user requested the page with ep expanding
	updateMylistBox(); // update mylist box
	if (!getElementsByClassName(document.getElementsByTagName('DIV'),'g_definitionlist mylist_add',false).length) {
		// we don't have the mylist add to thingie, as i allways show checkboxes i need that
		var eplist = document.getElementById('eplist');
		if (!eplist) return;
		createMylistAddBox(null,'files');
	}
	// replace the existing box with my box :P (box in a box, funny? well i guessed not)
	var p = getElementsByClassName(document.getElementsByTagName('P'),'mylist_add',false)[0];
	if (p) {
		var parentNode = p.parentNode;
		parentNode.replaceChild(createMylistAddBox(null,'anime'),p);
	}
}

function expandBox(object, node) {
  if (!node) node = this;
  //alert(node);
  if (node.parentNode.parentNode.tBodies[0].style.display == 'none') {
    node.parentNode.parentNode.tBodies[0].style.display = '';
    var span = node.parentNode.parentNode.caption.getElementsByTagName('SPAN')[0];
    while (span.childNodes.length) span.removeChild(span.childNodes[0]);
    span.appendChild(document.createTextNode(' [hide]'));
  } else {
    node.parentNode.parentNode.tBodies[0].style.display = 'none';
    var span = node.parentNode.parentNode.caption.getElementsByTagName('SPAN')[0];
    while (span.childNodes.length) span.removeChild(span.childNodes[0]);
    span.appendChild(document.createTextNode(' [show]'));
  }
}

function updateMylistBox() {
  var mylistBox = getElementsByClassName(document.getElementsByTagName('DIV'),'g_section mylist',true)[0];
  if (!mylistBox) return; // not logged or no files on mylist
  var cell = getElementsByClassName(mylistBox.getElementsByTagName('TD'),'title',true)[0];
  if (!cell) return;
  var icons = getElementsByClassName(cell.getElementsByTagName('SPAN'),'icons',true)[0];
  if (!icons) {
    icons = document.createElement('SPAN');
    icons.className = 'icons';
  }
  var icon;
  // Loop to see if an entry should get a status, and file state
  if (cell) icon = getElementsByClassName(icons.getElementsByTagName('A'), 'mixed', true)[0];
  if (icon) icons.removeChild(icon); // remove the mixed state icon
  if (cell) icon = getElementsByClassName(icons.getElementsByTagName('A'), 'unknown', true)[0];
  if (icon) icons.removeChild(icon); // remove the uknown state icon
  var stateFiles = new Array();
  var statusFiles = new Array();
  for (var me in mylist) {
    var mylistEntry = mylist[me];
    if (statusFiles[mylistEntry.status] == null) statusFiles[mylistEntry.status] = 1;
    else statusFiles[mylistEntry.status]++;
    if (stateFiles[mylistEntry.fstate] == null) stateFiles[mylistEntry.fstate] = 1;
    else stateFiles[mylistEntry.fstate]++;
  }
  for (var st in statusFiles) {
    if (st == 'indexOf') continue;
    var status = statusFiles[st];
    if (status == null || !status) continue;
    var stClass = 'i_state_'+mapMEStatusName(st);
    var txt = status + ' file';
    if (status > 1) txt += 's';
    txt += ' with status: '+st;
    var icon = getElementsByClassName(icons.getElementsByTagName('A'), stClass, true)[0];
    if (icon) // we already have this icon, so we just update it's title
      icon.title = txt;
    else // we don't have the icon, we add one
      createIcon(icons, txt, null, null, txt, stClass);        
  }
  for (var st in stateFiles) {
    if (st == 'indexOf' || st == 'unknown') continue;
    var state = stateFiles[st];
    if (state == null || !state) continue;
    var stClass = mapFState(st);
    var txt = state + ' file';
    if (state > 1) txt += 's';
    txt += ' with state: '+st;
    var icon = getElementsByClassName(icons.getElementsByTagName('A'), stClass, true)[0];
    if (icon) // we already have this icon, so we just update it's title
      icon.title = txt;
    else // we don't have the icon, we add one
      createIcon(icons, txt, null, null, txt, stClass);
  }
}

function checkEpExpand() {
  if (uriObj['eid']) { // If we have this object we are loading a ep
    loadExpand = true;
    var eid = uriObj['eid'];
    var aid = uriObj['aid'];
    var req = xhttpRequest();
    if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/eid'+eid+'.xml', parseEpisodeData);
    else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=ep&dev=1&aid='+aid+'&eid='+eid, parseEpisodeData);
  }
  if (uriObj['gid']) { // Shucks Batman, we are loading all episodes for a group
    loadExpand = true;
    var aid = uriObj['aid'];
    var gid = uriObj['gid'];
    var req = xhttpRequest();
    if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/aid'+aid+'_gid'+gid+'.xml', parseEpisodeData);
    else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=ep&dev=1&aid='+aid+'&eid=all&gid='+gid, parseEpisodeData);
  }
}

function updateEpisodeTable() {
	for (var i in episodes) { // Update each row of the episodeList 
		var episode = episodes[i];
		var row = document.getElementById('eid_'+episode.id);
		if (!row) continue;
		if (loadExpand) { // i have to fix some data tables
		  var eprowid = row.rowIndex + 1;
		  var tbRow = row.parentNode.rows[eprowid];
		  tbRow.id = 'eid_'+episode.id+'_ftHolder';
		}
		var cell, icon;
		var mylistEpEntries = findMylistEpEntries(episode.id);
		if (episode.relDate) { // This episode has a release date, so we add the the db add date
		  cell = getElementsByClassName(row.getElementsByTagName('TD'), 'date', true)[0];
		  if (cell) cell.title = 'DB add date: '+ episode.addDate;
		}
		// hook up the expand function
		cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action expand', true)[0];
		if (cell) {
			var ahref = cell.getElementsByTagName('A')[0];
			if (ahref) {
				if (uriObj['eid'] && uriObj['eid'] == episode.id) ahref.onclick = foldEp;
				else ahref.onclick = expandEp;
			} else { // someone called expand gid, add the ep expand icon
				var ahref = createIcon(null, "", "removeme", foldEp, "", "");
				var img = document.createElement('IMG');
				img.width = "15";
				img.height = "13";
				img.alt = "(-)";
				img.title = "fold all entries";
				img.src = base_url + "pics/minus.gif";
				ahref.appendChild(img);
				ahref.removeAttribute('href');
				cell.appendChild(ahref);
			}
			ahref.removeAttribute('href');
		}
		cell = getElementsByClassName(row.getElementsByTagName('TD'), 'title', true)[0];
		if (cell) {
			var altTitle = curTitle = '';
			var titleSpan = getNodeElementsByTagName(cell, 'SPAN')[0];
			if (titleSpan) curTitle = nodeData(titleSpan);
			if (episodeTitleLang != episodeAltTitleLang && 
				episode.titles[episodeAltTitleLang] && 
				episode.titles[episodeAltTitleLang] != '' &&
				episode.titles[episodeAltTitleLang] != curTitle) altTitle = episode.titles[episodeAltTitleLang];
			if (altTitle != '') {
				if (episodeTitleDisplay == 1) titleSpan.firstChild.nodeValue += ' ('+altTitle+')';
				if (episodeTitleDisplay == 2) titleSpan.title = mapLanguage(episodeAltTitleLang) + ' title: '+ altTitle;
			}
			if (mylistEpEntries.length) { // A neat part now, state icons
				var icon;
				// Loop to see if an entry should get a status, and file state
				var cell = getElementsByClassName(row.getElementsByTagName('TD'), 'title', true)[0];
				if (cell) {
					var icons = cell.getElementsByTagName('SPAN')[0];
					if (cell) icon = getElementsByClassName(icons.getElementsByTagName('SPAN'), 'mixed', true)[0];
					if (icon) icons.removeChild(icon); // remove the mixed state icon
					var stateFiles = new Array();
					var statusFiles = new Array();
					for (var me = 0; me < mylistEpEntries.length; me++) {
						var mylistEntry = mylistEpEntries[me];
						if (statusFiles[mylistEntry.status] == null) statusFiles[mylistEntry.status] = 1;
						else statusFiles[mylistEntry.status]++;
						if (stateFiles[mylistEntry.fstate] == null) stateFiles[mylistEntry.fstate] = 1;
						else stateFiles[mylistEntry.fstate]++;
					}
					for (var st in statusFiles) {
						if (st == 'indexOf') continue;
						var status = statusFiles[st];
						if (status == null || !status) continue;
						var stClass = 'i_state_'+mapMEStatusName(st);
						var txt = status + ' file';
						if (status > 1) txt += 's';
						txt += ' with status: '+st;
						var icon = getElementsByClassName(icons.getElementsByTagName('A'), stClass, true)[0];
						if (icon) // we already have this icon, so we just update it's title
							icon.title = txt;
						else // we don't have the icon, we add one
							createIcon(icons, txt, null, null, txt, stClass);        
					}
					for (var st in stateFiles) {
						if (st == 'indexOf' || st == 'unknown') continue;
						var state = stateFiles[st];
						if (state == null || !state) continue;
						var stClass = mapFState(st);
						var txt = state + ' file';
						if (state > 1) txt += 's';
						txt += ' with state: '+st;
						var icon = getElementsByClassName(icons.getElementsByTagName('A'), stClass, true)[0];
						if (icon) // we already have this icon, so we just update it's title
							icon.title = txt;
						else // we don't have the icon, we add one
							createIcon(icons, txt, null, null, txt, stClass);
					}
					icon = getElementsByClassName(icons.getElementsByTagName('A'), 'seen', true)[0];
					if (icon) icon.title = 'seen on: ' + cTimeDate(episode.seenDate);
				}
			}
		}
	}
	// Remove the collapse icon as i don't need it
	if (uriObj['gid']) {
		var table = document.getElementById('eplist');
		if (!table) return;
		var cell = table.tBodies[0].rows[0].cells[0];
		while (cell.childNodes.length) cell.removeChild(cell.firstChild);
	}
}

// GROUP FUNCTIONS //

function showAddToMylistBox() {
	if (!getElementsByClassName(document.getElementsByTagName('DIV'),'g_definitionlist mylist_add',false).length || addanimetomylistbox) {
		// we don't have the mylist add to thingie, as i allways show checkboxes i need that
		var eplist = document.getElementById('eplist');
		if (!eplist) return;
		if (!addanimetomylistbox) createMylistAddBox(eplist.parentNode,'files');
		else {
			eplist.parentNode.replaceChild(addfilestomylistbox,addanimetomylistbox);
			addanimetomylistbox = null;
		}
	}
	addfilestomylistbox.style.display = '';
	if (addanimetomylistbox && addanimetomylistbox.parentNode) addanimetomylistbox.parentNode.removeChild(addanimetomylistbox);
}

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
		if (checked && !file.visible) continue;
		var row = document.getElementById('fid_'+file.id);
		if (!row) continue;
		var ck = row.getElementsByTagName('INPUT')[0];
		if (!ck) continue;
		ck.checked = checked;
		filesChecked++;
	}
	if (checked) alert('Checked '+filesChecked+' of '+totalFiles+' files, please check to see if the selection is correct.'+
											'\nManual adding is not recommend. The best way to add files to your mylist is by using an AniDB Client. There is also a ready-to-run Java Applet.');
	showAddToMylistBox();
}

function createGroupRow(group) {
  var row = document.createElement('TR');
  row.id = 'gid_'+group.id;
  row.className = group.state; 
  if (group.filtered) row.style.display = 'none';
  var cell;
	if (uid) {
		cell = document.createElement('TD');
		// Expand
		cell.className = 'expand';
		cell.setAttribute('anidb:sort','0');
		var a = document.createElement('A');
		a.className = 'i_icon i_plus';
		a.title = 'expand all entries';
		a.alt = '(+)';
		var span = document.createElement('SPAN');
		span.appendChild(document.createTextNode('expand all entries'));
		a.appendChild(span);
		a.onclick = expandFilesByGroup;
		cell.appendChild(a);
		row.appendChild(cell);
	}
  // Last update
  cell = document.createElement('TD');
  cell.className = 'date lastupdate';
  if (Number(new Date())/1000 - Number(javascriptDate(cTimeDate(group.lastUp)))/1000 < 86400) cell.className += ' new';
  cell.appendChild(document.createTextNode(cTimeDate(group.lastUp)));
  row.appendChild(cell);
  // Name
  cell = document.createElement('TD');
  cell.className = 'name group';
  createLink(cell,group.shortName,'animedb.pl?show=group&gid=' + group.id, null, null, group.name, null);
  row.appendChild(cell);
  // State
  cell = document.createElement('TD');
  cell.className = 'state ' + group.state;
	createLink(cell,group.state,'animedb.pl?show=group&gid='+group.id+'&aid='+anime.id,null,null,null,null);
  //cell.appendChild(document.createTextNode(group.state));
  row.appendChild(cell);
  // N
  cell = document.createElement('TD');
  cell.className = 'note';
  cell.appendChild(document.createTextNode(' '));
  row.appendChild(cell);
  // Eps
  cell = document.createElement('TD');
  cell.className = 'eps number';
  cell.setAttribute('anidb:sort',String(group.epCnt));
  //createLink(cell,group.epCnt + '+' + group.sepCnt,'animedb.pl?show=group&gid='+group.id+'&aid='+anime.id,null,null,null,null);
  var maps = {'0' : {'use':true,'type': 0,'desc':"",'img':"blue"},
              '1' : {'use':false,'type': 1,'desc':"done: "+group.epRange,'img':"darkblue"},
              '2' : {'use':false,'type': 2,'desc':"in mylist: "+group.inMylistRange,'img':"lime"}};
  var range = expandRange(null,anime.eps,maps[0],null);
  if (group.epRange != '') { maps[1]['use'] = true; range = expandRange(group.epRange,anime.eps,maps[1],range); }
  if (group.isInMylistRange != '') { maps[2]['use'] = true; range = expandRange(group.isInMylistRange,anime.eps,maps[2],range); }
  makeCompletionBar(cell,range,maps);
  row.appendChild(cell);
  // Last
  cell = document.createElement('TD');
  cell.className = 'epno lastep';
  cell.appendChild(document.createTextNode(group.lastEp));
  row.appendChild(cell);
  // Specials
  cell = document.createElement('TD');
  cell.className = 'number specials';
  cell.appendChild(document.createTextNode(group.sepCnt));
  row.appendChild(cell);
  // Languages
  cell = document.createElement('TD');
  cell.className = 'languages';
  if (group.audioLangs) {
    for (var i = 0; i < group.audioLangs.length; i++)
      createIcon(cell, mapLanguage(group.audioLangs[i]), '', '', 'audio lang: '+mapLanguage(group.audioLangs[i]), 'i_audio_'+group.audioLangs[i]);
  }
  if (group.subtitleLangs) {
    for (var i = 0; i < group.subtitleLangs.length; i++)
      createIcon(cell, mapLanguage(group.subtitleLangs[i]), '', '', 'sub lang: '+mapLanguage(group.subtitleLangs[i]), 'i_sub_'+group.subtitleLangs[i]);  
  }
  row.appendChild(cell);
  // Votes
  cell = document.createElement('TD');
  cell.className = 'rating';
  cell.setAttribute('anidb:sort',(group.rating == '-') ? '0' : group.rating);
  createLink(cell,((group.rating == '-') ? 'N/A' : group.rating) + ' ('+group.ratingCount+')','animedb.pl?show=animegroupvotes&agid='+group.agid,'anidb::popup',null,null,'350.400.1.1.agvotes');
  row.appendChild(cell);
  // Cmts
  cell = document.createElement('TD');
  cell.className = 'number cmts';
  cell.setAttribute('anidb:sort',group.commentCount);
  createLink(cell,group.commentCount,'animedb.pl?show=agcmts&agid='+group.agid,'anidb::popup',null,null,'600.500.1.1.agcmts action');
  row.appendChild(cell);
  // Action
	if (uid) {
		cell = document.createElement('TD');
		cell.className = 'action icons';
		createLink(cell,'cmt','animedb.pl?show=agcmts&agid='+group.agid,'anidb::popup',null,'Comment on this release','i_icon i_group_comment 600.500.1.1.agcmts');
		createLink(cell,(group.userRating > -1) ? '['+group.userRating+']' : 'rate it','animedb.pl?show=pop&pop=agvote&id='+group.id,'anidb::popup',null,'Rate this release','i_icon i_vote 380.140.0.0.agvote');
		createLink(cell,'edit','animedb.pl?show=animegroup&agid='+group.agid,'anidb::popup',null,'Request edit of the state','i_icon i_file_edit 400.400.0.0.agstate');
		//createCheckBox(cell,'ck_g'+group.id,'ck_g'+group.id,toggleFilesFromGroup,false);
		row.appendChild(cell);
	}
  return (row);
}

function expandGroupTable() {
  var groupTable = getElementsByClassName(document.getElementsByTagName('TABLE'), 'grouplist', true)[0];
  if (!groupTable) return;
  expandAllG = true;
  var groupBody = groupTable.tBodies[0];
  var groupHead = groupTable.tHead;
  var groupFoot = groupTable.tFoot;
  var lastIndex = 0;
  var firstRun = false;
  for (var i in groups) {
    var group = groups[i];
    var row = document.getElementById('gid_'+group.id);
    if (row) {
      lastIndex = row.rowIndex;
      if (!group.filtered) row.style.display = '';
    } else {
      if (group.id == undefined || group.id == 0) continue;
      row = createGroupRow(group);
      firstRun = true;
      if (lastIndex >= groupBody.rows.length) lastIndex = groupBody.rows.length-1;
      if (row && groupBody.rows[lastIndex]) groupBody.insertBefore(row, groupBody.rows[lastIndex]);
    }
  }
  if (firstRun) { // rebuild the group list to prevent some rendering issues
    groupBody.style.display = "none"; // hide for now
    var cloneTB = document.createElement('TBODY');
    while (groupBody.childNodes.length) {
      var child = groupBody.firstChild;
      if (child.nodeName == 'TR') cloneTB.appendChild(child);
      else groupBody.removeChild(child); // this is like infanticide :P
    }
    while (cloneTB.childNodes.length) groupBody.appendChild(cloneTB.firstChild);
    var ident = findSortCol(groupHead.rows[0]);
    if (ident) {
      var head = getElementsByClassName(groupHead.getElementsByTagName('TH'),ident,true)[0];
      if (head) { // we have an head, need to switch the sorting functions first though
        if (head.className.indexOf("s_forward") >= 0) head.className = head.className.replace("s_forward", "s_reverse");
        else head.className = head.className.replace(" s_reverse","") + " s_forward";
        sortcol(head);
      }
    }
    groupBody.style.display = ""; // finished
  }
  for (var i = 0; i < groupBody.rows.length; i++) {
    var className = (i % 2) ? 'g_odd' : 'g_even';
    var row = groupBody.rows[i];
    var gid = row.id.substring(row.id.indexOf('_')+1,row.id.length);
    var group = groups[Number(gid)];
    if (group)
      row.className = className + ' ' + group.state;
  }
  // Update the "show all" link to point to my expand groups function
  var row = groupFoot.rows[groupFoot.rows.length-1];
  if (!row) return;
  var cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action', false)[0];
  var ahref = cell.getElementsByTagName('A')[0];
  ahref.onclick = foldGroupTable;
  ahref.removeAttribute('href');
  ahref.firstChild.nodeValue = 'show default';
  uriObj['showallag'] = 1;
}

function foldGroupTable() {
  var groupTable = getElementsByClassName(document.getElementsByTagName('TABLE'), 'grouplist', true)[0];
  if (!groupTable) return;
  expandAllG = false;
  var groupBody = groupTable.tBodies[0];
  var groupFoot = groupTable.tFoot;
  var displayed = 1;
  for (var i = 0; i < groupBody.rows.length; i++) {
    var row = groupBody.rows[i];
    var gid = row.id.substring(row.id.indexOf('_')+1,row.id.length);
    var group = groups[Number(gid)];
    if (group) {
      if (group.defaultVisible) {
        if (!group.filtered) row.style.display = '';
        row.className = ((displayed % 2) ? 'g_odd' : 'g_even') + ' ' + groups[group.id].state;
        displayed++;
      } else row.style.display = 'none'
    } else continue;
  }
  var row = groupFoot.rows[groupFoot.rows.length-1];
  if (!row) return;
  var cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action', false)[0];
  var ahref = cell.getElementsByTagName('A')[0];
  ahref.onclick = expandGroupTable;
  ahref.firstChild.nodeValue = 'show all';
  //uriObj.slice('showallag',1);
}

function updateGroupRow(group) {
  var row;
  if (group.id != 0) row = document.getElementById('gid_'+group.id);
  else {
    var tb = getElementsByClassName(document.getElementsByTagName('TABLE'),'grouplist',true)[0];
    var row = tb.tBodies[0].rows[tb.tBodies[0].rows.length - 1];
  }
  if (!row) return;
  group.defaultVisible = true;
	var cells = row.getElementsByTagName('TD');
  var cell, icon;
  // hook up the expand function
  cell = getElementsByClassName(cells, 'expand', true)[0];
  if (cell) {
    var ahref = cell.getElementsByTagName('A')[0];
    if (uriObj['gid'] && uriObj['gid'] == group.id) ahref.onclick = foldFilesByGroup;
    else ahref.onclick = expandFilesByGroup;
    ahref.removeAttribute('href');
  }
  cell = getElementsByClassName(cells, 'status', true)[0];
  if (cell) cell.setAttribute('anidb:sort',(group.epsInMyListArray && group.epsInMyListArray.length) ? String(group.epsInMyListArray.length) : '0');
  cell = getElementsByClassName(cells, 'eps', true)[0];
  if (cell) cell.setAttribute('anidb:sort',String(group.epCnt));
  cell = getElementsByClassName(cells, 'rating', true)[0];
  if (cell) cell.setAttribute('anidb:sort',(group.rating == '-') ? '0' : group.rating);
  cell = getElementsByClassName(cells, 'cmts', true)[0];
  if (cell) cell.setAttribute('anidb:sort',group.commentCount);
	if (uriObj['gid'] && uriObj['gid'] == group.id) { // add a checkbox if we got this expanded group
		cell = getElementsByClassName(cells, 'action', true)[0];
		if (cell && row.id.indexOf('gid') >= 0) createCheckBox(cell,'ck_g'+group.id,'ck_g'+group.id,toggleFilesFromGroup,false);
	}
}

function createGroupFilters(node) {
  var icons = new Array('complete','finished','ongoing','stalled','dropped','specials only','all');
  for (var i = 0; i < icons.length; i++) {
    var icon = icons[i];
    var desc = (icon != 'all') ? 'toggle group rows with state: '+icon : 'reset group filter';
    var ico = createIcon(null, icon, 'removeme', filterGroups, desc, 'i_gstate_'+icon.substring(0,(icon.indexOf(' ') >= 0) ? icon.indexOf(' ') : icon.length));
    ico.removeAttribute('href');
    node.appendChild(ico);
  }
}

function updateGroupTable() {
  // Update each row of the groupList
	var groupCnt = 0;
  for (var i in groups) {
    var group = groups[i];
    if (!group) continue;
		groupCnt++;
    updateGroupRow(group);
  }
  // update the no group row
  var group = new Object();
  group.id = 0;
  updateGroupRow(group);
  // Update the "show all" link to point to my expand groups function
  var groupTable = getElementsByClassName(document.getElementsByTagName('TABLE'), 'grouplist', true)[0];
  if (!groupTable) return;
  groupBody = groupTable.tBodies[0];
	var groupRowCnt = groupBody.rows.length - 1;
  // Move the heading and the footer rows to their table counterparts
  var thead = document.createElement('THEAD');
  var tfoot = document.createElement('TFOOT');
  groupTable.insertBefore(thead,groupBody);
  groupTable.insertBefore(tfoot,groupBody.nextSibling);
  thead.appendChild(groupBody.rows[0]);
  var row = groupBody.rows[groupBody.rows.length-1];
  if (!row) return;
  // append some group status filters
  var cell = row.cells[row.cells.length-2]; // second to last cell
  cell.className = 'action filter';
  var div = document.createElement('SPAN');
  div.className = 'icons';
  createGroupFilters(div);
  cell.appendChild(div);
  cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action', false)[0];
  var ahref = cell.getElementsByTagName('A')[0];
  if (ahref) { // There are cases when we have nothing to expand
    ahref.removeAttribute('href');
    if (uriObj['showallag'] && uriObj['showallag'] == 1) {
      ahref.onclick = foldGroupTable;
      ahref.firstChild.nodeValue = 'show default';
    } else ahref.onclick = expandGroupTable;
  } else { // but we should have something to see no?
		if (groupRowCnt != groupCnt) { // we do! show the link
			var a = createTextLink(null,'show all','removeme', null,expandGroupTable,null,null);
			a.removeAttribute('href');
			cell.appendChild(a);
		}
	}
  tfoot.appendChild(groupBody.rows[groupBody.rows.length-1]);
  //groupBody.removeChild(groupBody.rows[groupBody.rows.length-1]);
  // apply filtering support for the group list
  var headingList = thead.rows[0].getElementsByTagName('TH');
  headingTest = getElementsByClassName(headingList,'name group',true)[0];
  if (headingTest) headingTest.className += ' c_latin';
  headingTest = getElementsByClassName(headingList,'eps',true)[0];
  if (headingTest) headingTest.className += ' c_set';
  headingTest = getElementsByClassName(headingList,'specials',true)[0];
  if (headingTest) headingTest.className += ' c_number';
  headingTest = getElementsByClassName(headingList,'epno last',true)[0];
  if (headingTest) headingTest.className += ' c_number s_reverse';
  headingTest = getElementsByClassName(headingList,'state',true)[0];
  if (headingTest) headingTest.className += ' c_latin';
  headingTest = getElementsByClassName(headingList,'date lastupdate',true)[0];
  if (headingTest) headingTest.className += ' c_date';
  headingTest = getElementsByClassName(headingList,'rating',true)[0];
  if (headingTest) headingTest.className += ' c_set'; //waiting for adbs_anime.pm
  headingTest = getElementsByClassName(headingList,'cmts',true)[0];
  if (headingTest) headingTest.className += ' c_set';
  init_sorting(thead.rows[0],'epno','down');
}

function filterGroups() {
  var state = this.className;
  var active = (state.indexOf(' f_selected') >= 0);
  state = state.substring(state.indexOf('gstate_')+7,(active) ? state.indexOf(' f_selected') : state.length);
  for (var g in groups) {
    var group = groups[g];
    if (!group) continue;
    if (group.id == 0) continue;
    var gstate = group.state.substring(0,(group.state.indexOf(' ') >= 0) ? group.state.indexOf(' ') : group.state.length);
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
		var spans = div.getElementsByTagName('A');
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
}

function forceGroupVisibilty(vis) {
  for (var g in groups) {
    var group = groups[g];
    if (!group) continue;
    var row = document.getElementById('gid_'+group.id);
    if (row) {
      var cell, icon;
      // hook up the expand function
      cell = getElementsByClassName(row.getElementsByTagName('TD'), 'expand', true)[0];
      if (cell) {
        var ahref = cell.getElementsByTagName('A')[0];
        if (ahref.className.indexOf('minus') >= 0) { // group is expanded 
          if (!vis) group.visible = true; // show group files
          continue;
        }
      }
    }
    group.visible = vis;
  }
  if (!vis && showNoGroup) groups[0].visible = true;
}

function forceFileTableRedraw(episode) {
  if (!episode) {
    for (var e in episodes) {
      var episode = episodes[e];
      if (!episode) continue;
      var fileTable = createFileTable(episode);
      document.getElementById('eid_'+episode.id+'_ftHolder').cells[0].className = '';
    }
  } else {
    var fileTable = createFileTable(episode);
    var row = document.getElementById('eid_'+episode.id+'_ftHolder');
    if (!row) return;
    row.cells[0].className = '';
  }
}

function expandFilesByGroup() {
  // this will be cute :P
  loadExpand = true; // warn scripts we are doing a full blown ep work
  internalExpand = true;
  expandedGroups++;
  var expandCnt = 0;
  var row;
  var gid = this.parentNode.parentNode.id;
  gid = gid.substring(gid.indexOf('_')+1,gid.length);
  if (!gid) { gid = 0; showNoGroup = true; }
  var group = groups[gid];
  group.visible = true;
  var getXML = false;
  if (group && !group.hasCherryBeenPoped) { 
    getXML = true;
    // And assuming everything goes well, our group has had it cherry poped :P
    group.hasCherryBeenPoped = true;
  }
	if (true) { // add the checkbox to the group
		var row;
		if (group.id != 0) row = document.getElementById('gid_'+group.id);
		else {
			var tb = getElementsByClassName(document.getElementsByTagName('TABLE'),'grouplist',true)[0];
			var row = tb.tBodies[0].rows[tb.tBodies[0].rows.length - 1];
		}
		if (row) {
			group.defaultVisible = true;
			var cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action', true)[0];
			if (cell && row.id.indexOf('gid') >= 0) createCheckBox(cell,'ck_g'+group.id,'ck_g'+group.id,toggleFilesFromGroup,false);
		}
	}
  var eid = null;
  // check for page expands
  for (var e in episodes) {
    var episode = episodes[e];
    if (!episode) continue;
    eid = episode.id;
    row = document.getElementById('eid_'+eid);
    if (!row) continue;
    var tRow = document.getElementById(row.id+'_ftHolder');
    if (tRow) tRow.parentNode.removeChild(tRow);
    var rowSibling = row.nextSibling;
    var rowParent = row.parentNode;
    var nRow = document.createElement('TR'); //the new episode row
    var nCell = document.createElement('TD');
    nRow.id = row.id + '_ftHolder';
    eid = row.id.split('_')[1];
    nCell.colSpan = row.cells.length;
    nCell.className = 'eidFileTable_load';
    nCell.appendChild(document.createTextNode('loading data...'));
    nRow.appendChild(nCell);
    rowParent.insertBefore(nRow,rowSibling);
  }
  this.onclick = foldFilesByGroup;
  this.title = 'fold all entries';
  this.alt = '(-)';
  this.className = 'i_icon i_minus';
  uriObj['expandall'] = 1;
  for (var i in episodes) { // Update each row of the episodeList 
    var episode = episodes[i];
    var row = document.getElementById('eid_'+episode.id);
    if (!row) continue;
    var cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action expand', true)[0];
    var ahref = cell.getElementsByTagName('A')[0];
    if (getXML) ahref.onclick = null; // will be set later
		else ahref.onclick = foldEp;
    ahref.title = 'fold all entries';
    ahref.alt = '(-)';
    ahref.className = 'i_icon i_minus';
  }
  if (getXML) { // *Need* to fetch the xml
    // we allready have the files for this group so we need to do the following:
    forceGroupVisibilty(false); // #1 Hide all groups that aren't expanded
    var req = xhttpRequest();
    if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/aid'+aid+'_gid'+gid+'.xml', parseEpisodeData);
    else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=ep&dev=1&aid='+uriObj['aid']+'&eid=all&gid='+gid, parseEpisodeData);
  } else {
    // we allready have the files for this group so we need to do the following:
    forceGroupVisibilty(false); // #1 Hide all groups that aren't expanded
    for (var e in episodes) {
      var episode = episodes[e];
      if (!episode) continue;
      forceFileTableRedraw(episode);     // #2 Force a redraw of the fileTables
    }
  }
}

function foldFilesByGroup() {
  // this will be cute :P
  expandedGroups--;
  var row;
  var gid = this.parentNode.parentNode.id;
  gid = gid.substring(gid.indexOf('_')+1,gid.length);
  if (!gid) { gid = 0; showNoGroup = false }
  var group = groups[gid];
  group.visible = false;
  var eid = null;
  this.onclick = expandFilesByGroup;
  this.title = 'expand all entries';
  this.alt = '(+)';
  this.className = 'i_icon i_plus';
  if (expandedGroups <= 0) forceGroupVisibilty(true); // unhide all groups
  else forceGroupVisibilty(false);                    // #1 Hide all groups that aren't expanded
	if (true) { // remove the checkbox from the group
		var row;
		if (group.id != 0) row = document.getElementById('gid_'+group.id);
		else {
			var tb = getElementsByClassName(document.getElementsByTagName('TABLE'),'grouplist',true)[0];
			var row = tb.tBodies[0].rows[tb.tBodies[0].rows.length - 1];
		}
		if (row) {
			group.defaultVisible = true;
			var cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action', true)[0];
			if (cell && row.id.indexOf('gid') >= 0) {
				var cks = cell.getElementsByTagName('INPUT');
				while (cks.length) cell.removeChild(cks[0]);
			}
		}
	}
  for (var e in episodes) {
    var episode = episodes[e];
    if (!episode) continue;
    eid = episode.id;
    row = document.getElementById('eid_'+eid);
    if (!row) continue;
    // we will remove the table on fold on this case
    var tb = document.getElementById('eid_'+episode.id+'_ftHolder');
    if (tb) tb.parentNode.removeChild(tb);
    if (expandedGroups <= 0) { // no more groups expanded
      // update the icon
      var cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action expand', true)[0];
      var ahref = cell.getElementsByTagName('A')[0];
      ahref.onclick = expandEp;
      ahref.title = 'expand all entries';
      ahref.alt = '(+)';
      ahref.className = 'i_icon i_plus';
    } else { // we still have expanded groups
      var rowSibling = row.nextSibling;
      var rowParent = row.parentNode;
      var nRow = document.createElement('TR'); //the new episode row
      var nCell = document.createElement('TD');
      nRow.id = row.id + '_ftHolder';
      nCell.colSpan = row.cells.length;
      nCell.className = 'eidFileTable_load';
      nCell.appendChild(document.createTextNode('loading data...'));
      nRow.appendChild(nCell);
      rowParent.insertBefore(nRow,rowSibling);
      forceFileTableRedraw(episode); // Force a redraw of the fileTable
    }
  }
}

function expandEp() {
  var row = this.parentNode.parentNode;
  var eid = null;
	var getXML = false;
  if (!document.getElementById(row.id+'_ftHolder')) {
    var rowSibling = row.nextSibling;
    var rowParent = row.parentNode;
    var nRow = document.createElement('TR'); //the new episode row
    var nCell = document.createElement('TD');
    nRow.id = row.id + '_ftHolder';
    eid = row.id.split('_')[1];
    nCell.colSpan = 10;//row.cells.length;
    nCell.className = 'eidFileTable_load';
    nCell.appendChild(document.createTextNode('loading data...'));
    nRow.appendChild(nCell);
    rowParent.insertBefore(nRow,rowSibling);
		getXML = true;
    var req = xhttpRequest();
    if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/eid'+eid+'.xml', parseEpisodeData);
    else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=ep&dev=1&aid='+uriObj['aid']+'&eid='+eid, parseEpisodeData);
  } else {
    document.getElementById(row.id+'_ftHolder').style.display = '';
  }
  if (!getXML) this.onclick = foldEp;
	else this.onclick = null;
  this.title = 'fold this entry';
  this.alt = '(-)';
  this.className = 'i_icon i_minus';
  uriObj['expand'] = eid;
  uriObj['#'] = 'eid_'+eid;
}

function foldEp() {
  var row = this.parentNode.parentNode;
  if (!document.getElementById(row.id+'_ftHolder')) return; // what the fuck am i doing here anyway?
  document.getElementById(row.id+'_ftHolder').style.display = 'none';
  this.onclick = expandEp;
  this.title = 'fold this entry';
  this.alt = '(-)';
  this.className = 'i_icon i_plus';
  //uriObj.slice('expand',1);
  //uriObj.slice('#',1);
}

function expandFiles() {
  var row = this.parentNode.parentNode;
  var eid = null;
  if (!document.getElementById(row.id+'_relftHolder')) {
    var rowSibling = row.nextSibling;
    var rowParent = row.parentNode;
    var nRow = document.createElement('TR'); //the new episode row
    var nCell = document.createElement('TD');
    nRow.id = row.id + '_relftHolder';
    rfid = row.id.split('_')[1];
    var file = null;
    if (row.id.substr(0,4) == 'fid_') file = files[rfid];
    else file = pseudoFiles.list[rfid];
    nCell.colSpan = row.cells.length;
		// Relation Table
    var fT = document.createElement('TABLE');
    fT.className = 'filelist';
    fT.id = 'file'+rfid+'relations';
    // TableHead
    if (LAY_HEADER) {
      var fTH = document.createElement('THEAD');
      fTH.appendChild(createFileTableHead());
      fT.appendChild(fTH);
    }
    // TableBody
    var fTB = document.createElement('TBODY');
    for (var f = 0; f < file.relatedFiles.length; f++) {
      var drow = document.getElementById('fid_'+file.relatedFiles[f]);
      var dnode;
      if (!drow) {
        var rfile = files[file.relatedFiles[f]];
        var episode = episodes[rfile.episodeId];
        drow = createFileTableRow(episode,rfile);
      }
      dnode = drow.cloneNode(true);
      dnode.id = 'r'+dnode.id;
      dnode.style.display = '';
      // clean up the extra node
      var ccell = getElementsByClassName(dnode.getElementsByTagName('TD'),'id icons',true)[0];
      if (ccell) {
        var spans = dnode.getElementsByTagName('SPAN');
        var ico = getElementsByClassName(spans,'i_icon i_plus',true)[0];
        if (ico) ccell.removeChild(ico);
        else {
          ico = getElementsByClassName(spans,'i_icon i_minus',true)[0];
          if (ico) ccell.removeChild(ico);
        }
      }
      fTB.appendChild(dnode);
    }
    fT.appendChild(fTB);
		// END here
    nCell.appendChild(fT);
    nRow.appendChild(nCell);
    rowParent.insertBefore(nRow,rowSibling);
  } else {
    document.getElementById(row.id+'_relftHolder').style.display = '';
  }
  this.onclick = foldFiles;
  this.title = 'fold this entry';
  this.alt = '(-)';
  this.className = 'i_icon i_minus';
}

function foldFiles() {
  var row = this.parentNode.parentNode;
  document.getElementById(row.id+'_relftHolder').style.display = 'none';
  this.onclick = expandFiles;
  this.title = 'expand this entry';
  this.alt = '(+)';
  this.className = 'i_icon i_plus';
}

// FILE TABLE FUNCTIONS //

/* * Creates the file table heading row */
function createFileTableHead() {
  if (tFileHeadRow) return (tFileHeadRow.cloneNode(tFileHeadRow,true));
  var row = document.createElement('TR');
	row.className = 'heading';
	//createHeader(row,'expand c_none',' ');															// expand
	createHeader(row,'check c_none','x');																	// checkbox
	createHeader(row,'id c_set','F','File Details/FID');									// fid
	createHeader(row,'title c_setlatin','Group');													// group
	createHeader(row,'size c_set','Size');																// size
	if (uriObj['showcrc'] && uriObj['showcrc'] == 1) 
		createHeader(row,'crc c_latin','CRC');															// crc
	createHeader(row,'lang c_none','Lang');																// lang
	createHeader(row,'codec c_setlatin','CF','Container Format and Codecs');	// codecs
	createHeader(row,'resolution c_latin','Resolution');									// resolution
	createHeader(row,'source c_latin','Source');													// source
	createHeader(row,'quality c_set','Quality');													// quality
	createHeader(row,'hash c_none','Hash');																// hash
	createHeader(row,'users c_set','Users');															// users
	createHeader(row,'mylist c_none','Mylist','Mylist State');						// mylist
	createHeader(row,'action c_none','Action');														// action
  tFileHeadRow = row;
  return (row.cloneNode(row,true));
}

/* *
 * This function creates and returns a Object with all the possible icons for a given file
 * @param file File to build icon object
 * @return an Object with the icons
 * ids: fid, ext, vid, audlangs, audcodecs, sublangs, crc, ver, cenfile_ep_rel, file_file_rel,
 *      cmt, date, mylist, mylist_status, mylist_fstate, mylist_seen, mylist_cmt, mylist_action,
 *      mylist_watch, ed2k, edit, expand, quality, avdump
 */
function createFileIcons(file) {
	var icons = new Object();
	// fid
	if (file.pseudoFile || (file.relatedFiles && file.relatedFiles.length)) 
		icons['expand'] = createIcon(null, '[+]', null, expandFiles, 'expand this entry', 'i_plus');
	if (!file.pseudoFile)
		icons['fid'] = createIcon(null, file.id, 'animedb.pl?show=file&fid='+file.id, null, 'show file details - FID: '+file.id, 'i_file_details');
	else
		icons['fid'] = createIcon(null, 'P'+file.id, 'http://wiki.anidb.net/w/Files:Pseudo_files', null, 'Pseudo File: P'+file.id, 'i_file_details');
	// extension
  if (file.type != 'generic') {
    var tooltip = 'type: '+file.type+' | added: '+cTimeDate(file.date);
    if (file.relDate > 0) tooltip += ' | released: '+cTimeDate(file.relDate);
    if (file.fileType != '') tooltip += ' | extension: '+file.fileType;
    icons['ext'] = createIcon(null, file.fileType, null, null, tooltip, 'i_ext '+file.fileType);
  }
	// avdumped
	if (file.avdumped)
		icons['avdump'] = createIcon(null,' [avdumped]','http://wiki.anidb.net/w/Avdump',null,'File was verified by an AniDB Client','i_av_yes');
	// vid
  if (file.type == 'video') { // VIDEO STREAM
    for (var st in file.videoTracks) {
      var stream = file.videoTracks[st];
      if (stream && stream.codec) {
        var res;
        var vc_RXP = /[^\w]/ig;
        var vcodec = 'unknown';
        if (stream.codec) vcodec = stream.codec.toLowerCase();
        var vstyle = vcodec.replace(vc_RXP,'_');
        var tooltip = 'video';
        if (file.resolution != 'unknown') tooltip += ' | resolution: '+file.resolution;
        if (stream.codec != 'unknown') tooltip += ' | codec: '+stream.codec;
        if (stream.ar != 'unknown')tooltip += ' | ar: '+stream.ar;
        if (file.length) tooltip += ' | duration: '+formatFileLength(file.length,'long');
        //createIcon(icons, 'vid0 ', null, null, tooltip, 'i_videostream_'+vstyle);
				icons['vid'] = createIcon(null, 'vid0 ', null, null, tooltip, 'i_video '+stream.codec_sname);
      }
    }
  }
	// audio languages
	if (file.type == 'video' || file.type == 'audio') { // AUDIO STREAM
		var audioLangs = new Array();
		var audioCodecs = new Array();
		for (var st in file.audioTracks) {
      var stream = file.audioTracks[st];
      if (stream && stream.lang) {
        var tooltip = 'audio: '+mapLanguage(stream.lang);
				if (st < 2) { // EXP only exports info other than lang for the first 2 streams
          if (stream.codec != 'unknown') tooltip += ' | codec: '+stream.codec;
          if (stream.chan != 'other') tooltip += ' | channels: '+mapAudioChannels(stream.chan);
          if (stream.type) tooltip += ' | type: '+mapAudioType(stream.type);
          if (file.length) tooltip += ' | duration: '+formatFileLength(file.length,'long');
					audioCodecs.push(createIcon(null, stream.codec, 'http://wiki.anidb.info/w/Codecs', null, 'Audio codec: '+stream.codec,'i_audio '+stream.codec_sname));
        }
				audioLangs.push(createIcon(null, 'aud'+st+'.'+stream.lang+' ', null, null, tooltip, 'i_audio_'+stream.lang));
      }
    }
		icons['audlangs'] = audioLangs;
		icons['audcodecs'] = audioCodecs;
	}
	// subtitle languages
  if (file.type == 'video' || file.type == 'subtitle' || file.type == 'other') { // SUBTITLE STREAM
		var subtitleLangs = new Array();
    for (var st in file.subtitleTracks) {
      var stream = file.subtitleTracks[st];
      if (stream && stream.lang) {
        var tooltip = 'subtitle: '+mapLanguage(stream.lang);
		    if (st < 2) {
         if (stream.type && stream.type != 'unknown') tooltip += ' | type: '+mapSubTypeData(stream.type);
         if (stream.flags) tooltip += ' | flags: '+mapSubFlagData(stream.flags);
        }
        subtitleLangs.push(createIcon(null, 'sub'+st+'.'+stream.lang+' ', null, null, tooltip, 'i_sub_'+stream.lang));
      }
    }
		icons['sublangs'] = subtitleLangs;
  }
  // crc status
  if (file.crcStatus == 'valid') 
		icons['crc'] = createIcon(null, 'crc.ok ', null, null, 'crc *matches* official crc ('+file.crc32.toUpperCase()+')', 'i_crc_yes');
  else if (file.crcStatus == 'invalid') 
		icons['crc'] = createIcon(null, 'crc.bad ', null, null, 'crc *does not match* official crc', 'i_crc_no');
  else if (file.type != 'generic') 
		icons['crc'] = createIcon(null, 'crc.unv ', null, null, 'crc *not* compared to official crc', 'i_crc_unv');
  // file version
  if (file.version != 'v1') 
		icons['ver'] = createIcon(null, file.version+' ', null, null, 'version *'+file.version.charAt(1)+'* (error corrections)', 'i_vrs_'+file.version.charAt(1));
	// censor
  if (file.isCensored) 
		icons['cen'] = createIcon(null, 'cen ', null, null, '*censored*', 'i_censored');
  if (file.isUncensored) 
		icons['cen'] = createIcon(null, 'uncen ', null, null, '*uncensored*', 'i_uncensored');
  // FILE<->EP RELATIONS
  if (file.epRelations && file.epRelations[file.episodeId]) { // Do some work
    var rel = file.epRelations[file.episodeId];
    var tooltip = 'File covers: [';
    for (var bar = 0; bar < rel['startp']/10; bar++) tooltip += '-';
    for (var bar = rel['startp']/10; bar < rel['endp']/10; bar++) tooltip += '=';
    for (var bar = rel['endp']/10; bar < 10; bar++) tooltip += '-';
    icons['file_ep_rel'] = createIcon(null, '['+rel['startp']+'-'+rel['endp']+']', null, null, tooltip+']', 'i_file2ep_rel');
  }
  // FILE<->FILE RELATIONS
  if (file.fileRelations && file.fileRelations.length) { // Do some work
    for (var r in file.fileRelations) {
      var rel = file.fileRelations[r];
      if (rel['relfile'] == undefined) continue;
      var tooltip = 'File relation of type \"'+rel['type']+'\" \"'+rel['dir']+'\" with file id \"'+rel['relfile']+'\"';
      icons['file_file_rel'] = createIcon(null,'[rel: '+ rel['relfile'] + ']', null, null, tooltip, 'i_file2file_rel');
    }
  }
  // FILE COMMENT
  if ((file.other) && (file.other != '') && (file.other != undefined)) 
		icons['cmt'] = createIcon(null, 'comment ', null, null, 'comment: '+file.other, 'i_comment');
  // NEW FILE
  if (file.newFile)
		icons['date'] = createIcon(null, 'new.file ', null, null, '*new file* (last 24h)', 'i_new_icon');
  // MYLIST
  var temp = new Array();
  var mylistEntry = mylist[file.id];
  if (mylistEntry) {
    icons['mylist'] = createIcon(null, 'in.mylist ', null, null, '*in mylist*', 'i_mylist');
    // Mylist status icon
    var tooltip = 'mylist status: '+mylistEntry.status;
    if (mylistEntry.storage && mylistEntry.storage != '') tooltip += ' | storage: '+mylistEntry.storage;
    var className = mapMEStatusName(mylistEntry.status);
    icons['mylist_status'] = createIcon(null, mylistEntry.status+' ', null, null, tooltip, 'i_state_'+className);
    // Mylist FileState
    if (mylistEntry.fstate && mylistEntry.fstate != 'unknown') {
      var className = mapFState(mylistEntry.fstate);
      var tooltip = 'mylist state: '+mylistEntry.fstate;
      icons['mylist_fstate'] = createIcon(null, mylistEntry.fstate+' ', null, null, tooltip, className);
    }
		var animeFL = anime.getTitle().charAt(0).toLowerCase();
    // Seen status
    if (mylistEntry.seen) 
			icons['mylist_seen'] = createIcon(null, 'seen ', null, null, 'seen on: '+cTimeDate(mylistEntry.seenDate), 'i_seen');
    // mylist comment
    if ((mylistEntry.other) && (mylistEntry.other != '') && (mylistEntry.other != undefined)) 
			icons['mylist_cmt'] = createIcon(null, 'mylist comment ', null, null, 'mylist comment: '+mylistEntry.other, 'i_comment');
		// mylist action
    icons['mylist_action'] = createIcon(null, 'mylist.remove ', 'animedb.pl?show=mylist&do=del&lid='+mylistEntry.id+'&expand=' + file.animeId + '&showfiles=1&char='+animeFL+'#a'+file.animeId, null, 'remove from mylist', 'i_file_removemylist');
    if (mylistEntry.seen) 
			icons['mylist_watch'] = createIcon(null, 'mylist.unwatch ', 'animedb.pl?show=mylist&do=seen&seen=0&lid='+mylistEntry.id+'&expand='+ file.animeId+'&showfiles=1&char='+animeFL+'#a'+file.animeId, null, 'mark unwatched', 'i_seen_no');
    else 
			icons['mylist_watch'] = createIcon(null, 'mylist.watch ', 'animedb.pl?show=mylist&do=seen&seen=1&lid='+mylistEntry.id+'&expand='+file.animeId+'&showfiles=1&char='+animeFL+'#a'+file.animeId, null, 'mark watched', 'i_seen_yes');
	} else {
		// mylist action
		icons['mylist_action'] = createIcon(null, 'mylist.add ', 'animedb.pl?show=mylist&do=add&fid=' + file.id, null, 'add to mylist', 'i_file_addmylist');
	}
	// ed2k
	if (uid != undefined && file.ed2k != '') {
    if (file.crcStatus != 'invalid') 
			icons['ed2k'] = createIcon(null, 'ed2k', "!fillme!", null, 'ed2k hash', 'i_file_ed2k');
    else 
			icons['ed2k'] = createIcon(null, 'ed2k.crc.bad', "!fillme!", null, 'ed2k hash (invalid CRC file)', 'i_file_ed2k_corrupt');
  }
	// Action icons
	if (file.type != 'generic') {
		icons['edit'] = createIcon(null, 'edit', 'animedb.pl?show=addfile&aid='+file.animeId+'&eid='+file.episodeId+'&edit='+file.id, null, 'edit', 'i_file_edit');
		var qual = file.quality.replace(' ','');
		icons['quality'] = createIcon(null, file.quality, null, null, 'quality: '+file.quality, 'i_rate_'+qual);
	}
	return icons;
}

function createFileTableRow(episode,file) {
	// if (tFileRow) return (tFileRow.cloneNode(tFileRow,true));
	// ROW Definition
	var fTBr0 = document.createElement('TR');
	if (!file.pseudoFile) {
		var test = document.getElementById('fid_'+file.id);
		if (test) {
			var t2 = test.parentNode.parentNode.id;
			if (Number(t2.substring(7,t2.indexOf('files'))) != episode.id) fTBr0.id = 'fid_'+file.id+'_eid_'+episode.id;
			else return; // The row was allready created and we have it
		} else fTBr0.id = 'fid_'+file.id;
	} else fTBr0.id = 'rfid_'+file.id+'_eid_'+episode.id; // This is safer
	if (!file.pseudoFile || file.type != 'stub') {
		filterObj.markDeprecated(file);
		filterObj.markUnfiltered(file);
		if (LAY_HIDEFILES && file.isDeprecated) file.visible = false;
		filterObj.markVisible(file);
		filterObj.markHidden(file);
		if (!file.visible) episode.hiddenFiles++;
	} else file.visible = false; // STUB files should be very hidden, very hidden indeed..
	if (!file.visible || (file.type == 'generic' && LAY_HIDEGENERICFILES)) fTBr0.style.display = 'none';
	if (groups[file.groupId] && !groups[file.groupId].visible) fTBr0.style.display = 'none';
	if (file.type == 'generic') fTBr0.className = "generic no_sort";
	else {
		if (file.crcStatus == 'valid') fTBr0.className = 'good';
		if (file.crcStatus == 'invalid') fTBr0.className = 'bad';
		if (file.isDeprecated) fTBr0.className = 'deprecated';
	}
	// Start of cells
	var icons = createFileIcons(file); // get file icons
	//createCell(fTBr0, 'file expand', icons['expand']); // EXPAND  
	var ck = createCheckBox(null,'madd.f.'+file.id,null,showAddToMylistBox,false);
	if (file.pseudoFile) ck.disabled = true;
	createCell(fTBr0, 'check', ck); // CHECKBOX
	if (!icons['expand'])
		createCell(fTBr0, 'id icons', icons['fid'], file.id); // FID
	else {
		var cell = createCell(null, 'id icons',icons['fid'],file.id);
		cell.appendChild(icons['expand']);
		fTBr0.appendChild(cell);
	}
	// Group
	if (file.type == 'generic') {
		createCell(fTBr0, 'title',createLink(null, 'generic file', 'http://wiki.anidb.info/w/Files:Generic_files', 'extern wiki', null, null, null)); // GROUP
	} else { // group
		var cell = createCell(null, 'title');
		if (icons['date'] || icons['cmt'] || icons['ver']) {
			var divIcons = document.createElement('SPAN');
			divIcons.className = 'icons';
			if (icons['date']) divIcons.appendChild(icons['date']);
			if (icons['cmt']) divIcons.appendChild(icons['cmt']);
			if (icons['ver']) divIcons.appendChild(icons['ver']);
			cell.appendChild(divIcons);
		}
		var groupEntry = groups[file.groupId];
		cell.setAttribute('anidb:sort',groupEntry.shortName);
		if (!file.groupId) groupEntry = null;
		var label = document.createElement('LABEL');
		if (!file.pseudoFile) {
			if (groupEntry) 
				label.appendChild(createLink(null, groupEntry.shortName, 'animedb.pl?show=group&gid=' + file.groupId, 'extern anidb', null, 'group name: ' + groupEntry.name, null));
			else 
				label.appendChild(document.createTextNode('no group'));
		} else {
			var shortNames = "";
			var Names = "";
			for (var gr = 0; gr < file.relatedGroups.length; gr++) {
				var groupEntry = groups[file.relatedGroups[gr]];
				shortNames += groupEntry.shortName;
				Names += groupEntry.name;
				if (gr < 2 && file.relatedGroups.length > gr+1) {
					shortNames += '&'; Names += '&';
				} else if (gr == 1) {
					shortNames += '&...'; Names += '&...';
				} else break; // will not add more information even if it exists
			}
			label.appendChild(createLink(null, shortNames, 'http://wiki.anidb.info/w/Files:Pseudo_files', 'extern wiki', null, 'group name: ' + Names, null));
		}
		cell.appendChild(label);
		fTBr0.appendChild(cell);
	}
	if (file.type == 'generic') {
		var cell = document.createElement('TD');
		cell.colSpan = ((uriObj['showcrc'] && uriObj['showcrc'] == '1') ? 8 : 7);
		fTBr0.appendChild(cell);
	} else {
		createCell(fTBr0, 'size', document.createElement('NOBR').appendChild(document.createTextNode(formatFileSize(file.size))), file.size); // SIZE
		if (uriObj['showcrc'] && uriObj['showcrc'] == '1') createCell(fTBr0, 'crc', document.createTextNode(file.crc32.toUpperCase()));
		var cell = createCell(null, 'lang icons');
		if (icons['audlangs']) {
			var audlangs = icons['audlangs'];
			for (var i = 0; i < audlangs.length; i++) cell.appendChild(audlangs[i]);
		}
		if (icons['sublangs']) {
			var sublangs = icons['sublangs'];
			for (var i = 0; i < sublangs.length; i++) cell.appendChild(sublangs[i]);
		}
		fTBr0.appendChild(cell);
		cell = createCell(null, 'codec icons');
		var codecSort = 'unknown';
		if (file.type == 'video' && file.videoTracks[0] && file.videoTracks[0].codec) codecSort = file.videoTracks[0].codec;
		cell.setAttribute('anidb:sort',codecSort);
		if (icons['ext']) cell.appendChild(icons['ext']);
		if (icons['vid']) cell.appendChild(icons['vid']);
		if (icons['audcodecs']) {
			var audcodecs = icons['audcodecs'];
			for (var i = 0; i < audcodecs.length; i++) cell.appendChild(audcodecs[i]);
		}
		fTBr0.appendChild(cell);
		createCell(fTBr0, 'resolution', ((file.type == 'video') ? document.createTextNode(file.resolution) : document.createTextNode('n/a')));
		var cell = createCell(null, 'source');
		if (icons['cen']) {
			var divIcons = document.createElement('SPAN');
			divIcons.className = 'icons';
			if (icons['cen']) divIcons.appendChild(icons['cen']);
			cell.appendChild(divIcons);
		}
		var label = document.createElement('LABEL');
		label.appendChild(document.createTextNode(file.source));
		cell.appendChild(label);
		fTBr0.appendChild(cell);
		createCell(fTBr0, 'quality '+file.quality, icons['quality'],mapQuality(file.quality));
		cell = createCell(null, 'hash icons ed2k');
		if (uid != undefined && file.ed2k != '') cell.onmouseover = createHashLink;
		if (icons['ed2k']) cell.appendChild(icons['ed2k']);
		if (icons['crc']) cell.appendChild(icons['crc']);
		if (icons['avdump']) cell.appendChild(icons['avdump']);
		fTBr0.appendChild(cell);
	}
	cell = createCell(null, 'users count',createLink(null, file.usersTotal, 'animedb.pl?show=userlist&fid=' + file.id, null, null, 'total users', null),file.usersTotal);
	cell.appendChild(document.createTextNode('/'));
	var abbr = document.createElement('ABBR');
	abbr.title = 'deleted';
	abbr.appendChild(document.createTextNode(file.usersDeleted));
	cell.appendChild(abbr);
	cell.appendChild(document.createTextNode('/'));
	abbr = document.createElement('ABBR');
	abbr.title = 'unknown status';
	abbr.appendChild(document.createTextNode(file.usersUnknown));
	cell.appendChild(abbr);
	fTBr0.appendChild(cell);
	cell = createCell(null, 'mylist icons');
	if (icons['mylist']) cell.appendChild(icons['mylist']);
	if (icons['mylist_status']) cell.appendChild(icons['mylist_status']);
	if (icons['mylist_fstate']) cell.appendChild(icons['mylist_fstate']);
	if (icons['mylist_seen']) cell.appendChild(icons['mylist_seen']);
	if (icons['mylist_cmt']) cell.appendChild(icons['mylist_cmt']);
	fTBr0.appendChild(cell);
	cell = createCell(null, 'action file icons');
	if (icons['edit']) cell.appendChild(icons['edit']);
	if (icons['mylist_action']) cell.appendChild(icons['mylist_action']);
	if (icons['mylist_watch']) cell.appendChild(icons['mylist_watch']);
	fTBr0.appendChild(cell);
	return (fTBr0);
}

function prepareForSort() {
  var node = this;
  while (node.nodeName != 'TABLE') node = node.parentNode;
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

function showFiles(hide,passedNode) {
  var node = this;
  if (passedNode) node = passedNode;
  var cell = null; 
  while (node.nodeName != 'TABLE') {
    if (!cell && node.nodeName == 'TD') cell = node;
    node = node.parentNode;
  }
  var eid = Number(node.id.split("episode")[1].split("files")[0]); 
  var episode = episodes[eid];
  if (!episode) return;
  //alert(episode.files);
  var tBody = node.tBodies[0];
  for (var i = 0; i < tBody.rows.length; i++) {
    var row = tBody.rows[i];
    if (row.id.indexOf('_relftHolder') >= 0) continue; //not interested
    var file = null;
    switch (row.id.substr(0,4)) {
      case 'fid_': file = files[Number(row.id.split('_')[1])]; break;
      case 'rfid': file = pseudoFiles.list[Number(row.id.split('_')[1])]; break;
    }
    if (!file) continue;
    //alert('show?: '+!passedNode+'\nfid: '+file.id+' is visible? '+file.visible+'\nrow: '+row);
    if (!passedNode) row.style.display = ''; 
    else { // hide
      if (!file.visible) row.style.display = 'none';
    }
  }
  while (cell.childNodes.length) cell.removeChild(cell.firstChild);
  var i = document.createElement('I');
  if (!passedNode) {
    i.appendChild(document.createTextNode('All files shown - '));
    var ahref = createLink(null,'hide files', 'removeme', null, hideFiles, null, null);
    ahref.removeAttribute('href');
    i.appendChild(ahref);     
  } else {
    i.appendChild(document.createTextNode(episode.hiddenFiles + ' file'+((episode.hiddenFiles > 1) ? 's' : '')+ ' not shown - '));
    var ahref = createLink(null,'show all files', 'removeme', null, showFiles, null, null);
    ahref.removeAttribute('href');
    i.appendChild(ahref);   
  }
  cell.appendChild(i);
}

function hideFiles() {
  showFiles(true,this);
}

function createFileTable(episode) {
  if (!episode) return;
  var fT = document.createElement('TABLE');
  fT.className = 'filelist';
  fT.id = 'episode'+episode.id+'files';
  // TableHead
  if (LAY_HEADER) {
    var fTH = document.createElement('THEAD');
    fTH.appendChild(createFileTableHead());
    fT.appendChild(fTH);
  }
  var fTF = document.createElement('TFOOT');
  // TableBody
  var fTB = document.createElement('TBODY');
  if (episode.files && episode.files.length) {
    for (var f = 0; f < episode.files.length; f++) {
      var file = files[episode.files[f]];
      if (file.type != 'generic') fTB.appendChild(createFileTableRow(episode,file));
      else fTF.appendChild(createFileTableRow(episode,file));
    }
  }
  fT.appendChild(fTB);
  if (episode.hiddenFiles) {
    var tr = document.createElement('TR');
    var td = document.createElement('TD');
    td.colSpan = ((uriObj['showcrc'] && uriObj['showcrc'] == '1') ? 14 : 13);
    var i = document.createElement('I');
    i.appendChild(document.createTextNode(episode.hiddenFiles + ' file'+((episode.hiddenFiles > 1) ? 's' : '')+ ' not shown - '));
    var ahref = createLink(null,'show all files', 'removeme', null, showFiles, null, null);
    ahref.removeAttribute('href');
    i.appendChild(ahref);
    td.appendChild(i);
		tr.appendChild(td);
    fTF.appendChild(tr);
  }
  fT.appendChild(fTF);
  // TableFoot
  var ftHolder = document.getElementById('eid_'+episode.id+'_ftHolder');
  if (ftHolder) {
    var cell = ftHolder.cells[0];
    if (cell && cell.firstChild) {
      cell.removeChild(cell.firstChild);
      cell.appendChild(fT);
    }
  }
  var old_ftHolder = document.getElementById('eid_'+episode.id+'_old_ftHolder');
  if (old_ftHolder) old_ftHolder.parentNode.removeChild(old_ftHolder); // Remove the old ep table
  init_sorting(fT,'title','down');
  // fix the sorting function
  if (LAY_HEADER) {
    var ths = fTH.getElementsByTagName('TH');
    for (var i = 0; i < ths.length; i++) ths[i].onclick = prepareForSort;
  }
}

function updateFileTable(episode) {
  var eprowid = document.getElementById('eid_'+episode.id).rowIndex + 1;
  var tbRow = document.getElementById('eid_'+episode.id).parentNode.rows[eprowid];
  var tbody = tbRow.getElementsByTagName('TBODY')[0];
  if (!tbody) return;
  var table = tbody.parentNode;
  var thead = document.createElement('THEAD');
  var tfoot = document.createElement('TFOOT');
  table.insertBefore(thead,tbody);
  table.insertBefore(thead,tbody.nextSibling);
  var ckrow = getElementsByClassName(tbody.rows[0].getElementsByTagName('TD'), 'check', false)[0];
  if (!ckrow) {
      var c1 = document.createElement('TH'); // CHECKBOX
      c1.className = 'check';
      c1.appendChild(document.createTextNode('x'));
      tbody.rows[0].insertBefore(c1,tbody.rows[0].cells[0]);
    }
  var c0 = document.createElement('TH');
  c0.className = 'file expand c_none';
  tbody.rows[0].insertBefore(c0,tbody.rows[0].cells[0]);
  thead.appendChild(tbody.rows[0]);
  for (var i = 0; i < tbody.rows.length; i++) {
    var row = tbody.rows[i];
    if (!row) continue;
    //row.style.display = 'none';
    var idrow = getElementsByClassName(row.getElementsByTagName('TD'), 'id', false)[0];
    if (!idrow) continue;
    var ahref = idrow.getElementsByTagName('A')[0];
    if (!ahref) continue;
    var fid = Number(ahref.href.substring(ahref.href.indexOf('fid=')+4,ahref.href.length));
    var file = files[fid];
    if (!file) continue;
    var ckrow = getElementsByClassName(row.getElementsByTagName('TD'), 'check', false)[0];
    if (!ckrow) {
      var c1 = document.createElement('TD'); // CHECKBOX
      c1.className = 'check'; 
      var ck = createCheckbox('madd.f.'+file.id,false)
      if (file.pseudoFile) ck.disabled = true;
      c1.appendChild(ck);
      row.insertBefore(c1,row.cells[0]);
    } else {
      var ck = ckrow.getElementsByTagName('INPUT')[0];
      if (file.pseudoFile) ck.disabled = true;
    }
    c0 = document.createElement('TD');
    c0.className = 'file expand';
    if (file.pseudoFile || (file.relatedFiles && file.relatedFiles.length)) 
      createIcon(c0, '[+]', null, expandFiles, 'expand this entry', 'i_plus');
    row.insertBefore(c0,row.cells[0]);
    var statusrow = getElementsByClassName(row.getElementsByTagName('TD'), 'status', true)[0];
    var icons = getElementsByClassName(statusrow.getElementsByTagName('SPAN'), 'icons', true)[0];
    while (icons.childNodes.length)
      icons.removeChild(icons.childNodes[0]);
    createFileIcons(file,icons);
    //row.style.display = '';
  }
}

function createFieldValueRow(parentNode, className, field, valueElement) {
	var row = document.createElement('TR');
	row.className = className;
	var th = document.createElement('TH');
	th.className = 'field';
	if (field) th.appendChild(document.createTextNode(field));
	row.appendChild(th);
	var td = document.createElement('TD');
	td.className = 'value';
	if (valueElement) td.appendChild(valueElement);
	row.appendChild(td);
	if (parentNode) parentNode.appendChild(row);
	else return row;
}

function changeGenAddType () {
	var select = getElementsByName(addanimetomylistbox.getElementsByTagName('SELECT'),'addl.genaddtype',false)[0];
	if (!select) return;
	var input = getElementsByName(addanimetomylistbox.getElementsByTagName('INPUT'),'addl.upto',false)[0];
	if (!input) return;
	if (select.value == 0) input.disabled = false;
	if (select.value == 1) input.disabled = true;
}

function gODD(i) {
	return (!(i % 2) ? ' g_odd' : '');
}

function createMylistAddBox(parentNode,type) {
	var div = document.createElement('DIV');
	div.className = "g_definitionlist mylist_add";
	if (type == 'files') div.style.display = 'none';
	var table = document.createElement('TABLE');
	var caption = document.createElement('CAPTION');
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
	var tbody = document.createElement('TBODY');
	var select; var row; var input;
	if (type == 'anime') {
		//,2:{"text":'all known generics'}}
		select = createSelectArray(null,"addl.genaddtype","addl.genaddtype",changeGenAddType,0,{0:{"text":'episodes up to'},1:{"text":"all episodes+specials"}});
		//select = createSelectArray(null,"addl.genaddtype","addl.genaddtype",changeGenAddType,0,{0:{"text":'episodes up to'}});
		row = createFieldValueRow(null,'genaddtype'+gODD(i),'Add',select);
		row.getElementsByTagName('TD')[0].appendChild(document.createTextNode(' '));
		input = createTextInput('addl.upto',3,false,false,3);
		input.value = anime.eps;
		row.getElementsByTagName('TD')[0].appendChild(input);
		tbody.appendChild(row); i++;
	}
	select = createSelectArray(null,"addl.viewed","addl.viewed",null,0,{0:{"text":'unwatched'},1:{"text":'watched'}});
	createFieldValueRow(tbody,'watched'+gODD(i),'Watched',select); i++;
	var optionArray = {0:{"text":'unknown'},1:{"text":'internal storage (hdd)'},
										 2:{"text":'external storage (cd/dvd/...)'},3:{"text":'deleted'}};
	select = createSelectArray(null,"addl.state","addl.state",null,0,optionArray);
	var row = createFieldValueRow(null,'state'+gODD(i),'State',select); i++;
	var inlineHelp = document.createElement('A');
	inlineHelp.className = 'i_inline i_help';
	inlineHelp.href = 'http://wiki.anidb.net/w/myliststate';
	inlineHelp.rel = 'popup 300-450-1-1-helppopup';
	var span = document.createElement('SPAN');
	span.appendChild(document.createTextNode('help'));
	inlineHelp.appendChild(span);
	row.getElementsByTagName('TD')[0].appendChild(inlineHelp);
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
		select = createSelectArray(null,"addl.genericstate","addl.genericstate",null,100,optionArray);
		createFieldValueRow(tbody,'type'+gODD(i),'Type Gen',select); i++;
	}
	createFieldValueRow(tbody,'source'+gODD(i),'Source',createTextInput("addl.source",30,false,false,100)); i++;
	createFieldValueRow(tbody,'storage'+gODD(i),'Storage',createTextInput("addl.storage",30,false,false,100)); i++;
	createFieldValueRow(tbody,'other'+gODD(i),'Other',createTextBox('addl.other',null,25,4,null)); i++;
	row = createFieldValueRow(null,'note'+gODD(i),'Note'); i++;
	var cell = row.getElementsByTagName('TD')[0];
	if (type == 'anime') {
		cell.appendChild(document.createTextNode('You can add this anime to your mylist with the form above using generic files. Expand per group if you wish to add specific files.'));
		cell.appendChild(document.createElement('BR'));
	}
	cell.appendChild(document.createTextNode('Manual adding is '));
	var b = document.createElement('B');
	b.appendChild(document.createTextNode('not recommend'));
	cell.appendChild(b);
	cell.appendChild(document.createTextNode('. The best way to add files to your mylist is by using an '));
	cell.appendChild(createTextLink(null,'AniDB Client',"http://wiki.anidb.net/w/AniDB_Clients","anidb::wiki",null, null,null));
	cell.appendChild(document.createTextNode('. There is also a ready-to-run '));
	cell.appendChild(createTextLink(null,'Java Applet',"http://static.anidb.net/client/webaom.htm","anidb::popup",null, null,null));
	cell.appendChild(document.createTextNode('.'));
	tbody.appendChild(row);
	row = document.createElement('TR');
	row.className = 'action';
	cell = document.createElement('TD');
	cell.colSpan = 2;
	input = document.createElement('INPUT');
	input.type = 'submit';
	input.name = 'addl.doadd';
	input.value = 'Add';
	cell.appendChild(input);
	row.appendChild(cell);
	tbody.appendChild(row);
	table.appendChild(tbody);
	div.appendChild(table);
	if (type == 'files') addfilestomylistbox = div;
	if (type == 'anime') addanimetomylistbox = div;
	if (parentNode) parentNode.appendChild(div);
	else return div;
}

var processedNode = 0;
function createPageExpandAll() {
  // Fetch next queued item
  if (processingEps) return; // We wait till processing is done before creating another table
  var epNode = epQueue.shift();
  processingEps = true;

  // Episode processing
  var eid = Number(epNode.getAttribute('id'));
  parseEpisode(epNode, aid);
  var episode = episodes[eid];
  if (!episode) return;
  
  // clear old table if we are expanding all eps
  if (internalExpand) {
    var eprowid = document.getElementById('eid_'+episode.id).rowIndex + 1;
    var tbRow = document.getElementById('eid_'+episode.id).parentNode.rows[eprowid];
    tbRow.id = 'eid_'+eid+'_ftHolder';
    var rmTable = tbRow.getElementsByTagName('TABLE')[0];
    if (rmTable) tbRow.cells[0].removeChild(rmTable);
  }
  
  // create table
  var fileTable = createFileTable(episode);
  document.getElementById('eid_'+episode.id+'_ftHolder').cells[0].className = '';

  processingEps = false;
  if (!epQueue.length) { 
    self.clearInterval(epInt);
    internalExpand = false;
  }
}

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
    //if (loadExpand) epInt = self.setInterval('createPageExpandAll()', epInterval);
    for (var k = 0; k < epNodes.length; k++) {
      // if we are handling just one episode do stuff now otherwise defer execution
      var eid = Number(epNodes[k].getAttribute('id'));
      parseEpisode(epNodes[k], aid);
      var episode = episodes[eid];
      if (!episode) continue;
      if (loadExpand) { // Normal behaviour
        //epQueue.push(epNodes[k]);
				var eprow = document.getElementById('eid_'+episode.id);
				// find the expand thingie (quick way)
				var a = eprow.getElementsByTagName('A')[0];
				if (a) a.onclick = foldEp;
        var eprowid = eprow.rowIndex + 1;
        var tbRow = document.getElementById('eid_'+episode.id).parentNode.rows[eprowid];
        tbRow.id = 'eid_'+eid+'_ftHolder';
        var rmTable = tbRow.getElementsByTagName('TABLE')[0];
        if (rmTable) tbRow.cells[0].removeChild(rmTable);
      } else {
				var eprow = document.getElementById('eid_'+episode.id);
				var a = eprow.getElementsByTagName('A')[0];
				if (a) a.onclick = foldEp;
			}
      if (!internalExpand) {
        var fileTable = createFileTable(episode);
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
      var row = document.getElementById('eid_'+episode.id+'_ftHolder');
      if (!row) continue;
      var tb = row.getElementsByTagName('TABLE')[0];
      if (!tb) continue;
      if (!tb.tBodies[0].rows.length) row.parentNode.removeChild(row);
    }
  }
  updateStatus('');
}

// hook up the window onload event
//window.onload = preLoad;
addLoadEvent(preLoad);
