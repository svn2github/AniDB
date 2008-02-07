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
  // Preload function, taken from anidbscript.js (thanks Rar :P)
  
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
}

function expandBox(object, node) {
  if (!node) node = this;
  alert(node);
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
  var icons = getElementsByClassName(cell.getElementsByTagName('DIV'),'icons',true)[0];
  if (!icons) {
    icons = document.createElement('DIV');
    icons.className = 'icons';
  }
  var icon;
  // Loop to see if an entry should get a status, and file state
  if (cell) icon = getElementsByClassName(icons.getElementsByTagName('SPAN'), 'mixed', true)[0];
  if (icon) icons.removeChild(icon); // remove the mixed state icon
  if (cell) icon = getElementsByClassName(icons.getElementsByTagName('SPAN'), 'unknown', true)[0];
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
    var icon = getElementsByClassName(icons.getElementsByTagName('SPAN'), stClass, true)[0];
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
    var icon = getElementsByClassName(icons.getElementsByTagName('SPAN'), stClass, true)[0];
    if (icon) // we already have this icon, so we just update it's title
      icon.title = txt;
    else // we don't have the icon, we add one
      createIcon(icons, txt, null, null, txt, stClass);
  }
}

function checkEpExpand() {
  if (uriObj['expand']) { // If we have this object we are loading a ep
    loadExpand = true;
    var eid = uriObj['expand'];
    var aid = uriObj['aid'];
    var req = xhttpRequest();
    if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/eid'+eid+'.xml', parseEpisodeData);
    else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=ep&dev=1&aid='+aid+'&eid='+eid, parseEpisodeData);
  }
  if (uriObj['expandall']) { // Shucks Batman, we are loading all episodes
    loadExpand = true;
    var aid = uriObj['aid'];
    var gid = uriObj['gid'];
    var req = xhttpRequest();
    if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/aid'+aid+'_gid566.xml', parseEpisodeData);
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
    var ahref = cell.getElementsByTagName('A')[0];
    if (uriObj['expand'] && uriObj['expand'] == episode.id) ahref.onclick = foldEp;
    else ahref.onclick = expandEp;
    ahref.removeAttribute('href');
    cell = getElementsByClassName(row.getElementsByTagName('TD'), 'title', true)[0];
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
    // check to see if we have episode comments
    if (episode.other.length) {
      cell = getElementsByClassName(row.getElementsByTagName('TD'), 'title', true)[0];
      var icons = cell.getElementsByTagName('DIV')[0]; 
      createIcon(icons, 'ep cmt', null, null, 'episode comment: '+episode.other, 'i_comment');
    }
    if (mylistEpEntries.length) { // A neat part now, state icons
      var icon;
      // Loop to see if an entry should get a status, and file state
      var cell = getElementsByClassName(row.getElementsByTagName('TD'), 'title', true)[0];
      var icons = cell.getElementsByTagName('DIV')[0];
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
        var icon = getElementsByClassName(icons.getElementsByTagName('SPAN'), stClass, true)[0];
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
        var icon = getElementsByClassName(icons.getElementsByTagName('SPAN'), stClass, true)[0];
        if (icon) // we already have this icon, so we just update it's title
          icon.title = txt;
        else // we don't have the icon, we add one
          createIcon(icons, txt, null, null, txt, stClass);
      }
      if (cell) icon = getElementsByClassName(icons.getElementsByTagName('SPAN'), 'seen', true)[0];
      if (icon) icon.title = 'seen on: ' + cTimeDate(episode.seenDate);
    }
  }
  // fix the expand all thingie
  if (uriObj['expandall']) {
    var epTb = getElementsByClassName(document.getElementsByTagName('TABLE'), 'eplist', true)[0];
    var epTbRow = epTb.tBodies[0].rows[0];
    var a = epTbRow.getElementsByTagName('A')[0];
    if (a) {
      // update the thing
      a.removeAttribute('href');
      a.onclick = foldAllEps;
      a.onclick = expandAllEps;
    }
  }
}

// GROUP FUNCTIONS //

function createGroupRow(group) {
  var row = document.createElement('TR');
  row.id = 'gid_'+group.id;
  row.className = group.state; 
  if (group.filtered) row.style.display = 'none';
  var cell = document.createElement('TD');
  // Expand
  cell.className = 'expand';
  cell.setAttribute('anidb:sort','0');
  var a = document.createElement('A');
  var img = document.createElement('IMG');
  img.src = base_url+'pics/plus.gif';
  img.title = 'expand all entries';
  img.alt = '(+)';
  img.width = 15;
  img.height = 13;
  a.appendChild(img);
  a.onclick = expandFilesByGroup;
  cell.appendChild(a);
  row.appendChild(cell);
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
  cell.appendChild(document.createTextNode(group.state));
  row.appendChild(cell);
  // N
  cell = document.createElement('TD');
  cell.className = 'note';
  cell.appendChild(document.createTextNode('-'));
  row.appendChild(cell);
  // Eps
  cell = document.createElement('TD');
  cell.className = 'eps';
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
  cell.className = 'votes';
  cell.setAttribute('anidb:sort',(group.rating == '-') ? '0' : group.rating);
  createLink(cell,((group.rating == '-') ? 'N/A' : group.rating) + ' ('+group.ratingCount+')','animedb.pl?show=animegroupvotes&agid='+group.agid,'anidb::popup',null,null,'350.400.1.1.agvotes');
  row.appendChild(cell);
  // Cmts
  cell = document.createElement('TD');
  cell.className = 'cmts';
  cell.setAttribute('anidb:sort',group.commentCount);
  createLink(cell,group.commentCount,'animedb.pl?show=agcmts&agid='+group.agid,'anidb::popup',null,null,'600.500.1.1.agcmts action');
  row.appendChild(cell);
  // Action
  cell = document.createElement('TD');
  cell.className = 'action icons';
  createLink(cell,'edit','animedb.pl?show=animegroup&agid='+group.agid,'anidb::popup',null,null,'i_icon i_file_edit 400.400.0.0.agstate','');
  createLink(cell,(group.userRating > -1) ? '['+group.userRating+']' : 'rate it','animedb.pl?show=pop&pop=agvote&id='+group.id,'anidb::popup',null,null,'i_icon i_ep_vote 380.140.0.0.agvote','');
  row.appendChild(cell);
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
  var cell, icon;
  // hook up the expand function
  cell = getElementsByClassName(row.getElementsByTagName('TD'), 'expand', true)[0];
  if (cell) {
    var ahref = cell.getElementsByTagName('A')[0];
    if (uriObj['expand'] && uriObj['expand'] == group.id) ahref.onclick = foldFilesByGroup;
    else ahref.onclick = expandFilesByGroup;
    ahref.removeAttribute('href');
  }
  cell = getElementsByClassName(row.getElementsByTagName('TD'), 'status', true)[0];
  if (cell) cell.setAttribute('anidb:sort',(group.epsInMyListArray && group.epsInMyListArray.length) ? String(group.epsInMyListArray.length) : '0');
  cell = getElementsByClassName(row.getElementsByTagName('TD'), 'eps', true)[0];
  if (cell) cell.setAttribute('anidb:sort',String(group.epCnt));
  cell = getElementsByClassName(row.getElementsByTagName('TD'), 'votes', true)[0];
  if (cell) cell.setAttribute('anidb:sort',(group.rating == '-') ? '0' : group.rating);
  cell = getElementsByClassName(row.getElementsByTagName('TD'), 'cmts', true)[0];
  if (cell) cell.setAttribute('anidb:sort',group.commentCount);
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
  for (var i in groups) {
    var group = groups[i];
    if (!group) continue;
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
  // Move the heading and the footer rows to their table counterparts
  var thead = document.createElement('THEAD');
  var tfoot = document.createElement('TFOOT');
  groupTable.insertBefore(thead,groupBody);
  groupTable.insertBefore(tfoot,groupBody.nextSibling);
  thead.appendChild(groupBody.rows[0]);
  // append some group status filters
  var tr = document.createElement('TR');
  var cell = document.createElement('TD');
  cell.colSpan = 12;
  cell.className = 'action filter';
  var div = document.createElement('DIV');
  div.className = 'icons';
  createGroupFilters(div);
  cell.appendChild(div);
  tr.appendChild(cell);
  thead.insertBefore(tr,thead.rows[0]);
  
  var row = groupBody.rows[groupBody.rows.length-1];
  if (!row) return;  

  cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action', false)[0];
  var ahref = cell.getElementsByTagName('A')[0];
  if (ahref) { // There are cases when we have nothing to expand
    ahref.removeAttribute('href');
    if (uriObj['showallag'] && uriObj['showallag'] == 1) {
      ahref.onclick = foldGroupTable;
      ahref.firstChild.nodeValue = 'show default';
    } else ahref.onclick = expandGroupTable;
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
  headingTest = getElementsByClassName(headingList,'votes',true)[0];
  if (headingTest) headingTest.className += ' c_none'; //waiting for adbs_anime.pm
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
    var view = false;
    for (var st in group_vis) {
      if (gstate == st && group_vis[st]) { view = true; break; }
    }
  }
/*
  for (var g in groups) {
    var group = groups[g];
    if (!group) continue;
    if (group.id == 0) continue;
    var gstate = group.state.substring(0,(group.state.indexOf(' ') >= 0) ? group.state.indexOf(' ') : group.state.length);
    if (gstate == state) {
      if (!active) group.filtered = true;
      else group.filtered = false;
    }
    if (active) {
      this.className = this.className.replace(' f_selected','');
      this.title = this.title.replace('show','hide');
    } else { 
      this.className += ' f_selected';
      this.title = this.title.replace('hide','show');
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
*/
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
        var img = ahref.getElementsByTagName('IMG')[0];
        if (img.src.indexOf('minus') >= 0) { // group is expanded 
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
  var img = this.getElementsByTagName('IMG')[0];
  if (!img) return;
  img.title = 'fold all entries';
  img.alt = '(-)';
  img.src = base_url+'pics/minus.gif';
  uriObj['expandall'] = 1;
  for (var i in episodes) { // Update each row of the episodeList 
    var episode = episodes[i];
    var row = document.getElementById('eid_'+episode.id);
    if (!row) continue;
    var cell = getElementsByClassName(row.getElementsByTagName('TD'), 'action expand', true)[0];
    var ahref = cell.getElementsByTagName('A')[0];
    ahref.onclick = foldEp;
    var imgC = ahref.getElementsByTagName('IMG')[0];
    if (!imgC) return;
    imgC.title = 'fold all entries';
    imgC.alt = '(-)';
    imgC.src = base_url+'pics/minus.gif';
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
  var img = this.getElementsByTagName('IMG')[0];
  if (!img) return;
  img.title = 'expand all entries';
  img.alt = '(+)';
  img.src = base_url+'pics/plus.gif';
  if (expandedGroups <= 0) forceGroupVisibilty(true); // unhide all groups
  else forceGroupVisibilty(false);                    // #1 Hide all groups that aren't expanded
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
      var imgC = ahref.getElementsByTagName('IMG')[0];
      if (!imgC) return;
      imgC.title = 'expand all entries';
      imgC.alt = '(+)';
      imgC.src = base_url+'pics/plus.gif';
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
  if (!document.getElementById(row.id+'_ftHolder')) {
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
    var req = xhttpRequest();
    if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/eid'+eid+'.xml', parseEpisodeData);
    else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=ep&dev=1&aid='+uriObj['aid']+'&eid='+eid, parseEpisodeData);
  } else {
    document.getElementById(row.id+'_ftHolder').style.display = '';
  }
  this.onclick = foldEp;
  var img = this.getElementsByTagName('IMG')[0];
  if (!img) return;
  img.title = 'fold this entry';
  img.alt = '(-)';
  img.src = base_url+'pics/minus.gif';
  uriObj['expand'] = eid;
  uriObj['#'] = 'eid_'+eid;
}

function foldEp() {
  var row = this.parentNode.parentNode;
  if (!document.getElementById(row.id+'_ftHolder')) return; // what the fuck am i doing here anyway?
  document.getElementById(row.id+'_ftHolder').style.display = 'none';
  this.onclick = expandEp;
  var img = this.getElementsByTagName('IMG')[0];
  if (!img) return;
  img.title = 'expand this entry';
  img.alt = '(+)';
  img.src = base_url+'pics/plus.gif';
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
      var ccell = getElementsByClassName(dnode.getElementsByTagName('TD'),'file expand',true)[0];
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

function createFileTableHead() {
  if (tFileHeadRow) return (tFileHeadRow.cloneNode(tFileHeadRow,true));
  var fTHr0 = document.createElement('TR');
  fTHr0.className = 'heading';
  var fTHr0c0 = document.createElement('TH');
    fTHr0c0.className = 'expand c_none';
    fTHr0c0.appendChild(document.createTextNode(' '));
    fTHr0.appendChild(fTHr0c0);
  var fTHr0c1 = document.createElement('TH');
    fTHr0c1.className = 'check c_none';
    fTHr0c1.appendChild(document.createTextNode('x'));
    fTHr0.appendChild(fTHr0c1);
  var fTHr0c2 = document.createElement('TH');
    fTHr0c2.className = 'id c_set';
    fTHr0c2.appendChild(document.createTextNode('FID'));
    fTHr0.appendChild(fTHr0c2);
  var fTHr0c4 = document.createElement('TH');
    fTHr0c4.className = 'size c_set s_forward';
    fTHr0c4.appendChild(document.createTextNode('Size'));
    fTHr0.appendChild(fTHr0c4);
  var fTHr0c5 = document.createElement('TH');
    fTHr0c5.className = 'status file c_none';
    fTHr0c5.appendChild(document.createTextNode('Status'));
    fTHr0.appendChild(fTHr0c5);
  var fTHr0c6 = document.createElement('TH');
    fTHr0c6.className = 'title c_latin';
    fTHr0c6.appendChild(document.createTextNode('Group'));
    fTHr0.appendChild(fTHr0c6);
  var fTHr0c7 = document.createElement('TH');
    fTHr0c7.className = 'quality c_set';
    fTHr0c7.appendChild(document.createTextNode('Quality'));
    fTHr0.appendChild(fTHr0c7);
  var fTHr0c8 = document.createElement('TH');
    fTHr0c8.className = 'resolution c_latin';
    fTHr0c8.appendChild(document.createTextNode('Resolution'));
    fTHr0.appendChild(fTHr0c8);
  var fTHr0c9 = document.createElement('TH');
    fTHr0c9.className = 'source c_latin';
    fTHr0c9.appendChild(document.createTextNode('Source'));
    fTHr0.appendChild(fTHr0c9);
  var fTHr0c10 = document.createElement('TH');
    fTHr0c10.className = 'stats c_set';
    fTHr0c10.appendChild(document.createTextNode('Users'));
    fTHr0.appendChild(fTHr0c10);
  var fTHr0c11 = document.createElement('TH');
    fTHr0c11.className = 'icon ed2k';
    fTHr0c11.appendChild(document.createTextNode('ed2k'));
    fTHr0.appendChild(fTHr0c11);
  var fTHr0c12 = document.createElement('TH');
    fTHr0c12.className = 'action file';
    fTHr0c12.appendChild(document.createTextNode('Action'));
    fTHr0.appendChild(fTHr0c12);
  tFileHeadRow = fTHr0;
  return (fTHr0.cloneNode(fTHr0,true));
}

function createFileIcons(file,icons) {
  if (file.type != 'generic') {
    var tooltip = 'type: '+file.type+' | added: '+cTimeDate(file.date);
    if (file.relDate > 0) tooltip += ' | released: '+cTimeDate(file.relDate);
    if (file.fileType != '') tooltip += ' | extension: '+file.fileType;
    createIcon(icons, file.type, null, null, tooltip, 'i_ftype_'+file.type);
  }
  // /TEST //
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
        createIcon(icons, 'vid0 ', null, null, tooltip, 'i_videostream_'+vstyle);
      }
    }
  }
  if (file.type == 'video' || file.type == 'audio') { // AUDIO STREAM
    for (var st in file.audioTracks) {
      var stream = file.audioTracks[st];
      if (stream && stream.lang) {
        var tooltip = 'audio: '+mapLanguage(stream.lang);
        if (st < 2) { // EXP only exports info other than lang for the first 2 streams
          if (stream.codec != 'unknown') tooltip += ' | codec: '+stream.codec;
          if (stream.chan != 'other') tooltip += ' | channels: '+mapAudioChannels(stream.chan);
          if (stream.type) tooltip += ' | type: '+mapAudioType(stream.type);
          if (file.length) tooltip += ' | duration: '+formatFileLength(file.length,'long');
        }
        createIcon(icons, 'aud'+st+'.'+stream.lang+' ', null, null, tooltip, 'i_audio_'+stream.lang);
      }
    }
  }
  if (file.type == 'video' || file.type == 'subtitle' || file.type == 'other') { // SUBTITLE STREAM
    for (var st in file.subtitleTracks) {
      var stream = file.subtitleTracks[st];
      if (stream && stream.lang) {
        var tooltip = 'subtitle: '+mapLanguage(stream.lang);
        if (st < 2) {
         if (stream.type && stream.type != 'unknown') tooltip += ' | type: '+mapSubTypeData(stream.type);
         if (stream.flags) tooltip += ' | flags: '+mapSubFlagData(stream.flags);
        }
        createIcon(icons, 'sub'+st+'.'+stream.lang+' ', null, null, tooltip, 'i_sub_'+stream.lang);
      }
    }
  }
  // FILE CRC STATUS
  if (file.crcStatus == 'valid') createIcon(icons, 'crc.ok ', null, null, 'crc *matches* official crc ('+file.crc32.toUpperCase()+')', 'i_crc_yes');
  else if (file.crcStatus == 'invalid') createIcon(icons, 'crc.bad ', null, null, 'crc *does not match* official crc', 'i_crc_no');
  else if (file.type != 'generic') createIcon(icons, 'crc.unv ', null, null, 'crc *not* compared to official crc', 'i_crc_unv');
  // FILE VERSION
  if (file.version != 'v1') createIcon(icons, file.version+' ', null, null, 'version *'+file.version.charAt(1)+'* (error corrections)', 'i_vrs_'+file.version.charAt(1));
  // FILE CENSOR STATUS
  if (file.isCensored) createIcon(icons, 'cen ', null, null, '*censored*', 'i_censored');
  if (file.isUncensored) createIcon(icons, 'uncen ', null, null, '*uncensored*', 'i_uncensored');
  // FILE<->EP RELATIONS
  if (file.epRelations && file.epRelations[file.episodeId]) { // Do some work
    var rel = file.epRelations[file.episodeId];
    var tooltip = 'File covers: [';
    for (var bar = 0; bar < rel['startp']/10; bar++) tooltip += '-';
    for (var bar = rel['startp']/10; bar < rel['endp']/10; bar++) tooltip += '=';
    for (var bar = rel['endp']/10; bar < 10; bar++) tooltip += '-';
    createIcon(icons, '['+rel['startp']+'-'+rel['endp']+']', null, null, tooltip+']', 'i_file2ep_rel');
  }
  // FILE<->FILE RELATIONS
  if (file.fileRelations && file.fileRelations.length) { // Do some work
    for (var r in file.fileRelations) {
      var rel = file.fileRelations[r];
      if (rel['relfile'] == undefined) continue;
      var tooltip = 'File relation of type \"'+rel['type']+'\" \"'+rel['dir']+'\" with file id \"'+rel['relfile']+'\"';
      createIcon(icons,'[rel: '+ rel['relfile'] + ']', null, null, tooltip, 'i_file2file_rel');
    }
  }
  // FILE COMMENT
  if ((file.other) && (file.other != '') && (file.other != undefined)) createIcon(icons, 'comment ', null, null, 'comment: '+file.other, 'i_comment');
  // NEW FILE
  if (file.newFile) createIcon(icons, 'new.file ', null, null, '*new file* (last 24h)', 'i_new_icon');
  // MYLIST
  var temp = new Array();
  var mylistEntry = mylist[file.id];
  if (mylistEntry) {
    createIcon(icons, 'in.mylist ', null, null, '*in mylist*', 'i_mylist');
    // Mylist status icon
    var tooltip = 'mylist status: '+mylistEntry.status;
    if (mylistEntry.storage && mylistEntry.storage != '') tooltip += ' | storage: '+mylistEntry.storage;
    var className = mapMEStatusName(mylistEntry.status);
    createIcon(icons, mylistEntry.status+' ', null, null, tooltip, 'i_state_'+className);
    // Mylist FileState
    if (mylistEntry.fstate && mylistEntry.fstate != 'unknown') {
      var className = mapFState(mylistEntry.fstate);
      var tooltip = 'mylist state: '+mylistEntry.fstate;
      createIcon(icons, mylistEntry.fstate+' ', null, null, tooltip, className);
    }
    // Seen status
    if (mylistEntry.seen) createIcon(icons, 'seen ', null, null, 'seen on: '+cTimeDate(mylistEntry.seenDate), 'i_seen');
    // mylist comment
    if ((mylistEntry.other) && (mylistEntry.other != '') && (mylistEntry.other != undefined)) createIcon(icons, 'mylist comment ', null, null, 'mylist comment: '+mylistEntry.other, 'i_comment');
	}
}

function createFileTableRow(episode,file) {
  // if (tFileRow) return (tFileRow.cloneNode(tFileRow,true));
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
  if (file.type == 'generic') fTBr0.className = "genr no_sort";
  else {
    if (file.crcStatus == 'valid') fTBr0.className = 'good';
    if (file.crcStatus == 'invalid') fTBr0.className = 'bad';
    if (file.isDeprecated) fTBr0.className = 'deprecated';
  }
  var fTBr0c0 = document.createElement('TD'); // EXPAND
  fTBr0c0.className = 'file expand'; 
  if (file.pseudoFile || (file.relatedFiles && file.relatedFiles.length)) 
    createIcon(fTBr0c0, '[+]', null, expandFiles, 'expand this entry', 'i_plus');
  fTBr0.appendChild(fTBr0c0);
  var fTBr0c1 = document.createElement('TD'); // CHECKBOX
  fTBr0c1.className = 'check'; 
  var ck = document.createElement('INPUT');
  ck.id = 'madd.f.'+file.id;
  ck.name = 'madd.f.'+file.id;
  ck.type = 'checkbox';
  if (file.pseudoFile) ck.disabled = true;
  fTBr0c1.appendChild(ck);
  fTBr0.appendChild(fTBr0c1);
  var fTBr0c2 = document.createElement('TD'); // FID
  fTBr0c2.className = 'id';
  if (file.type == 'generic') {
    fTBr0c2.colSpan = 2;
    createLink(fTBr0c2, 'x', 'animedb.pl?show=file&aid=' + file.animeId + '&eid=' + file.episodeId + '&fid=' + file.id + '&nonav=1', 'extern anidb', null, null, null);
    fTBr0.appendChild(fTBr0c2);
    fTBr0c2.appendChild(document.createTextNode(' generic file ['));
    createLink(fTBr0c2, '?', 'http://wiki.anidb.info/w/Generic_files', 'extern wiki', null, null, null);
    fTBr0c2.appendChild(document.createTextNode(']'));  
  } else {
    if (!file.pseudoFile)
      createLink(fTBr0c2, file.id, 'animedb.pl?show=file&aid=' + file.animeId + '&eid=' + file.episodeId + '&fid=' + file.id + '&nonav=1', 'extern anidb', null, null, null);
    else
      fTBr0c2.appendChild(document.createTextNode('P'+file.id));
    fTBr0c2.setAttribute('anidb:sort',file.id);
    fTBr0.appendChild(fTBr0c2);
    var fTBr0c4 = document.createElement('TD'); // SIZE
    fTBr0c4.className = 'size';
    fTBr0c4.setAttribute('anidb:sort',file.size);
    fTBr0c4.appendChild(document.createElement('NOBR').appendChild(document.createTextNode(formatFileSize(file.size))));
    fTBr0.appendChild(fTBr0c4);
  }
  var fTBr0c5 = document.createElement('TD'); // STAT
  fTBr0c5.className = 'status file';
  var icons = document.createElement('DIV');
  icons.className = 'icons';
  createFileIcons(file,icons);
	fTBr0c5.appendChild(icons);
  fTBr0.appendChild(fTBr0c5);
  var fTBr0c6 = document.createElement('TD');
  if (file.type == 'generic') { fTBr0c6.colSpan = 4; fTBr0.appendChild(fTBr0c6); }
  else {
    var groupEntry = groups[file.groupId]; // GROUP
    if (!file.groupId) groupEntry = null;
    fTBr0c6.className = 'title';
    if (!file.pseudoFile) {
      if (groupEntry) createLink(fTBr0c6, groupEntry.shortName, 'animedb.pl?show=group&gid=' + file.groupId + '&nonav=1', 'extern anidb', null, 'group name: ' + groupEntry.name, null);
      else fTBr0c6.appendChild(document.createTextNode('no group'));
    } else {
      for (var gr = 0; gr < file.relatedGroups.length; gr++) {
        var groupEntry = groups[file.relatedGroups[gr]]
        if (groupEntry) createLink(fTBr0c6, groupEntry.shortName, 'animedb.pl?show=group&gid=' + groupEntry.id + '&nonav=1', 'extern anidb', null, 'group name: ' + groupEntry.name, null);
        else fTBr0c6.appendChild(document.createTextNode('no group'));
        if (gr < 2 && file.relatedGroups.length > gr+1) {
          fTBr0c6.appendChild(document.createTextNode('&'));
        } else if (gr == 1) {
          fTBr0c6.appendChild(document.createTextNode('&...'));
        } else break; // will not add more information even if it exists
      }
    }
    fTBr0.appendChild(fTBr0c6);
    var fTBr0c7 = document.createElement('TD'); // QUALITY
    fTBr0c7.className = 'quality '+file.quality;
    fTBr0c7.setAttribute('anidb:sort',mapQuality(file.quality));
    buildQualityIcon(fTBr0c7,file.quality);
    fTBr0.appendChild(fTBr0c7);
    var fTBr0c8 = document.createElement('TD'); // RESOLUTION
    fTBr0c8.className = 'resolution';
    fTBr0c8.appendChild((file.type == 'video') ? document.createTextNode(file.resolution) : document.createTextNode('n/a'));
    fTBr0.appendChild(fTBr0c8);
    var fTBr0c9 = document.createElement('TD'); // SOURCE
    fTBr0c9.className = 'source';
    fTBr0c9.appendChild(document.createTextNode(file.source));
    fTBr0.appendChild(fTBr0c9);
  }
  var fTBr0c10 = document.createElement('TD'); // USERS
  fTBr0c10.className = "stats"
  fTBr0c10.setAttribute('anidb:sort',file.usersTotal);
  createLink(fTBr0c10, file.usersTotal, 'animedb.pl?show=userlist&fid=' + file.id, null, null, null, null);
  fTBr0c10.appendChild(document.createTextNode('/'+file.usersDeleted+'/'));
  var inode = document.createElement('I');
  inode.appendChild(document.createTextNode(file.usersUnknown));
  fTBr0c10.appendChild(inode);
  fTBr0.appendChild(fTBr0c10);
  var fTBr0c11 = document.createElement('TD'); // ED2K
  fTBr0c11.className = 'icon ed2k'
  if (uid != undefined && file.ed2k != '') {
    fTBr0c11.onmouseover = createHashLink;
    if (file.crcStatus != 'invalid') createIcon(fTBr0c11, 'ed2k', "!fillme!", null, 'ed2k hash', 'i_file_ed2k');
    else createIcon(fTBr0c11, 'ed2k.crc.bad', "!fillme!", null, 'ed2k hash (invalid CRC file)', 'i_file_ed2k_corrupt');
  } else fTBr0c11.appendChild(document.createTextNode('-'));
  fTBr0.appendChild(fTBr0c11);
  var fTBr0c12 = document.createElement('TD'); // ACTION
  fTBr0c12.className = "action file"
  var mylistEntry = mylist[file.id];
  if (uid != undefined && !file.pseudoFile) {
    if (mylistEntry == null || !mylistEntry) createIcon(fTBr0c12, 'mylist.add ', 'animedb.pl?show=mylist&do=add&aid=' + file.animeId + '&eid=' + file.episodeId + '&fid=' + file.id, null, 'add to mylist', 'i_file_addmylist');
    else {
      createIcon(fTBr0c12, 'mylist.remove ', 'animedb.pl?show=mylist&do=del&aid=' + file.animeId + '&eid=' + file.episodeId + '&fid=' + file.id, null, 'remove from mylist', 'i_file_removemylist');
      if (mylistEntry.seen) createIcon(fTBr0c12, 'mylist.unwatch ', 'animedb.pl?show=mylist&do=seen&seen=0&lid='+mylistEntry.id+'&expand=1&showfiles=1&char='+anime.getTitle().charAt(0).toLowerCase()+'#a'+file.animeId, null, 'mark unwatched', 'i_seen_no');
      else createIcon(fTBr0c12, 'mylist.watch ', 'animedb.pl?show=mylist&do=seen&seen=1&lid='+mylistEntry.id+'&expand=1&showfiles=1&char='+anime.getTitle().charAt(0).toLowerCase()+'#a'+file.animeId, null, 'mark watched', 'i_seen_yes');
    }
    createIcon(fTBr0c12, 'edit', 'animedb.pl?show=addfile&aid='+file.animeId+'&eid='+file.episodeId+'&edit='+file.id, null, 'edit', 'i_file_edit');
  }
  fTBr0.appendChild(fTBr0c12);
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
    td.colSpan = 12;
    var i = document.createElement('I');
    i.appendChild(document.createTextNode(episode.hiddenFiles + ' file'+((episode.hiddenFiles > 1) ? 's' : '')+ ' not shown - '));
    var ahref = createLink(null,'show all files', 'removeme', null, showFiles, null, null);
    ahref.removeAttribute('href');
    i.appendChild(ahref);
    td.appendChild(i);
    fTF.appendChild(td);
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
  init_sorting(fT,'size','down');
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
      var ck = document.createElement('INPUT');
      ck.id = 'madd.f.'+file.id;
      ck.name = 'madd.f.'+file.id;
      ck.type = 'checkbox';
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
    var icons = getElementsByClassName(statusrow.getElementsByTagName('DIV'), 'icons', true)[0];
    while (icons.childNodes.length)
      icons.removeChild(icons.childNodes[0]);
    createFileIcons(file,icons);
    //row.style.display = '';
  }
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
        var eprowid = document.getElementById('eid_'+episode.id).rowIndex + 1;
        var tbRow = document.getElementById('eid_'+episode.id).parentNode.rows[eprowid];
        tbRow.id = 'eid_'+eid+'_ftHolder';
        var rmTable = tbRow.getElementsByTagName('TABLE')[0];
        if (rmTable) tbRow.cells[0].removeChild(rmTable);
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
