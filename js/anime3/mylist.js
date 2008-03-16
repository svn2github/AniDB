/* *
 * @file mylist interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (01.03.2008)
 */

// GLOBALs //

var uid;						// userID
var ruid;
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
// cached stuff
var epTableHead = null;
var epTableFoot = null;
var fileTableHead = null;
var deltimer = null;
var g_note = null

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
		var mylist3 = getElementsByClassName(document.getElementsByTagName('DIV'), 'mylist3', true)[0];
		if (mylist3) mylist3.className = mylist3.className.replace("mylist3","mylist"); // this will correct css used in dev
	}
	initTooltips();
	uriObj = parseURI();
	// find the mylist table and update the links
	var mylistTable = getElementsByClassName(document.getElementsByTagName('TABLE'),'animelist',true)[0];
	if (!mylistTable) { errorAlert('prepPage','no mylist table found'); return; }
	var body = mylistTable.tBodies[0];
	for (var i = 1; i < body.rows.length; i++) {
		var row = body.rows[i];
		var cell = getElementsByClassName(row.getElementsByTagName('TD'),'expand',true)[0];
		if (!cell) { errorAlert('prepPage','no expand cell found'); continue; }
		var a = cell.getElementsByTagName('A')[0];
		if (!a) { errorAlert('prepPage','no a link found'); continue; }
		parseMylistExpandLink(a.href,mylist_settings);
		ruid = mylist_settings['uid'];
		a.removeAttribute('href');
		a.onclick = expandAnime;
	}
	mylist_settings['noeptb'] = false;
	/*
	var text = '';
	for (var key in mylist_settings) text += '['+key+'] : '+mylist_settings[key]+'\n';
	alert(text);
	*/
}

/* Function that fetches anime data
 * @param aid Anime ID
 * @param uid User ID
 */
function fetchData(aid,uid) {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/aid'+aid+'_uid'+ruid+'.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=useranime&aid='+aid+'&uid='+ruid, parseData);
}

/* Function that posts data
 * @param url URL to post
 */
function postData(url) {
  var req = xhttpRequest();
	var data = url.substr(url.indexOf('?')+1,url.length);
  if (''+window.location.hostname == '') xhttpRequestPost(req, 'msg_del.html', showSuccessBox, data);
  else xhttpRequestPost(req, 'animedb.pl', showSucessBox, data);
}

/* Shows a success box for a brief time */
function showSucessBox(xmldoc) {
	if (!g_note) {
		g_note = document.createElement('DIV');
		g_note.className = 'g_section g_notebox';
		var h3 = document.createElement('H3');
		h3.appendChild(document.createTextNode('NOTE:'));
		g_note.appendChild(h3);
		var p = document.createElement('P');
		p.appendChild(document.createTextNode('Action done.'));
		g_note.appendChild(p);
	}
	var msg_all = getElementsByClassName(document.getElementsByTagName('DIV'), 'g_content msg_all', true)[0];
	if (!msg_all) return;
	msg_all.insertBefore(g_note,msg_all.firstChild);
	self.clearTimeout(deltimer);
	deltimer = self.setTimeout('removeNoteBox()', 1000);
}

/* Removes the success box */
function removeDelBox() {
	g_note.parentNode.removeChild(g_note);
	g_note = null;
}

/* Function that parses xml response
 * @param xmldoc The xml response
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { if (seeDebug) alert('Error: Could not get root node'); return; }
	var t1 = new Date();
	var filedataNodes = root.getElementsByTagName('filedata');
	for (var k = 0; k < filedataNodes.length; k++) parseFiledata(filedataNodes[k], aid);
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
	var aid = root.getElementsByTagName('anime')[0];
	if (!aid) { errorAlert('parseData','no anime node'); return; }
	aid = Number(aid.getAttribute('id'));
	createEpisodeTable(aid);
}

/* Function that expands one anime */
function expandAnime() {
	this.onclick = foldAnime;
	this.title = 'Fold entry';
	this.className = 'i_icon i_minus';
	var row = this.parentNode;
	while (row.nodeName != 'TR') row = row.parentNode;
	var aid = row.id.substr(1,row.id.length);
	if (!animes[aid]) fetchData(aid,uid); // need to fetch data
	else {	// show stuff
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
	while (row.nodeName != 'TR') row = row.parentNode;
	var aid = row.id.substr(1,row.id.length);
	var testRow = document.getElementById('a'+aid+'_epsRow');
	if (testRow) testRow.style.display = 'none';
}

/* Function that expands files of a given anime */
function expandFiles() {
	this.onclick = foldFiles;
	this.title = 'Fold entry';
	this.className = 'i_icon i_minus';
	var row = this.parentNode;
	while (row.nodeName != 'TR') row = row.parentNode;
	var eid = row.id.substr(1,row.id.length);
	var testRow = document.getElementById('e'+eid+'_filesRow');
	if (testRow) testRow.style.display = '';
}

/* Function that folds files of a given anime */
function foldFiles() {
	this.onclick = expandFiles;
	this.title = 'Expand entry';
	this.className = 'i_icon i_plus';
	var row = this.parentNode;
	while (row.nodeName != 'TR') row = row.parentNode;
	var eid = row.id.substr(1,row.id.length);
	var testRow = document.getElementById('e'+eid+'_filesRow');
	if (testRow) testRow.style.display = 'none';
}

/* Function that toogles all the checkboxes for a given anime */
function cbToggle() {
	var node = this.parentNode;
	while (node.nodeName != 'TABLE') node = node.parentNode;
	var aid = Number(node.id.substring(1,node.id.indexOf('_')));
	var tbody = document.getElementById('a'+aid+'_episodesTable').tBodies[0];
	var checkboxes = tbody.getElementsByTagName('INPUT');
	for (var i = 0; i < checkboxes.length; i++) {
		var ck = checkboxes[i];
		if (ck.type != 'checkbox') continue;
		ck.checked = this.checked;
	}
}

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
	var stateCell = getElementsByClassName(row.getElementsByTagName('TD'),'icons state',true)[0];
	if (mylistEntry.seen) {
		url = 'animedb.pl?show=mylist&do=seen&seen=0&lid='+mylistEntry.id;
		this.title = 'mark unwatched';
		this.className = 'i_icon i_seen_no';
		this.getElementsByTagName('SPAN')[0].nodeValue = 'mylist.unwatch';
		// add seen icon
		if (stateCell) createIcon(stateCell, 'seen ', null, null, 'seen on: '+cTimeDate(mylistEntry.seenDate), 'i_seen');
	} else {
		url = 'animedb.pl?show=mylist&do=seen&seen=1&lid='+mylistEntry.id;
		this.title = 'mark watched';
		this.className = 'i_icon i_seen_yes';
		this.getElementsByTagName('SPAN')[0].nodeValue = 'mylist.watch';
		// remove seen icon
		if (stateCell) {
			var ico = getElementsByClassName(stateCell.getElementsByTagName('SPAN'),'i_seen',true)[0];
			if (ico) stateCell.removeChild(ico);
		}
	}	
	return true;
}
/* Function that removes a file from mylist */
function removeFromMylist() {
	var row = this.parentNode;
	while (row.nodeName != 'TR') row = row.parentNode;
	var fid = Number(row.id.substr(row.id.indexOf('f')+1,row.id.length));
	if (this.href) this.removeAttribute('href');
	var mylistEntry = mylist[fid];
	var url = 'animedb.pl?show=mylist&do=del&lid='+mylistEntry.id;
	mylist[fid] = null; // clear mylist entry
	// find fileTable
	var fileTable = row.parentNode;
	fileTable.removeChild(row);
	row = fileTable;
	if (!fileTable.rows.length) { // no more files for this episode, remove the row
		while (row.nodeName != 'TR') row = row.parentNode;
		var eid = Number(row.id.substr(1,row.id.indexOf('_')-1));
		var eidRow = document.getElementById('e'+eid);
		var fileTable = eidRow.parentNode;
		fileTable.removeChild(eidRow);
		fileTable.removeChild(row);
	}
	row = fileTable;
	if (!fileTable.rows.length) { // no more episodes for this anime, remove the row
		while (row.nodeName != 'TR') row = row.parentNode;
		var aid = Number(row.id.substr(1,row.id.indexOf('_')-1));
		var aidRow = document.getElementById('a'+aid);
		var fileTable = aidRow.parentNode;
		fileTable.removeChild(aidRow);
		fileTable.removeChild(row);
	}
	return true;
}


/* Creates an episode files table thead */
function createFilesTableHead() {
	var thead = document.createElement('THEAD');
	var row = document.createElement('TR');
	row.className = 'head';
	createHeader(row, 'check', 'X', 'Check/uncheck files');
	createHeader(row, 'file', 'F', 'File details/FID');
	if (mylist_settings['noeptb']) createHeader(row, 'epno', 'Epno', 'Episode number');
	createHeader(row, 'group', 'Group');
	createHeader(row, 'size', 'Size');
	if (uriObj['showcrc'] && uriObj['showcrc'] == 1) 
		createHeader(row, 'crc','CRC');
	createHeader(row, 'lang', 'Lang');
	createHeader(row, 'codec', 'CF', 'Container Format and Codecs');
	createHeader(row, 'quality', 'Quality');
	createHeader(row, 'hash', 'Hash');
	createHeader(row, 'storage', 'Storage');
	createHeader(row, 'source', 'Source');
	createHeader(row, 'state', 'State');
	createHeader(row, 'action', 'Action');
	thead.appendChild(row);
	fileTableHead = thead;
	return(fileTableHead.cloneNode(true));
}

/* This function creates and returns a Object with all the possible icons for a given file
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
	if (!LAY_SHOWFID) {
		if (!file.pseudoFile)
			icons['fid'] = createIcon(null, file.id, 'animedb.pl?show=file&fid='+file.id, null, 'show file details - FID: '+file.id, 'i_file_details');
		else
			icons['fid'] = createIcon(null, 'P'+file.id, 'http://wiki.anidb.net/w/Files:Pseudo_files', null, 'Pseudo File: P'+file.id, 'i_file_details');
	} else {
		if (!file.pseudoFile)
			icons['fid'] = createLink(null, file.id, 'animedb.pl?show=file&fid='+file.id, null, null ,null, null);
		else
			icons['fid'] = createLink(null, 'P'+file.id, 'http://wiki.anidb.net/w/Files:Pseudo_files', null, null ,null, null);
	}
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
		icons['mylist_action'] = createIcon(null, 'mylist.remove ', 'animedb.pl?show=mylist&do=del&lid='+mylistEntry.id+'&expand=' + file.animeId + '&showfiles=1&char='+animeFL+'#a'+file.animeId, removeFromMylist, 'remove from mylist', 'i_file_removemylist');
		if (mylistEntry.seen) 
			icons['mylist_watch'] = createIcon(null, 'mylist.unwatch ', 'animedb.pl?show=mylist&do=seen&seen=0&lid='+mylistEntry.id+'&expand='+ file.animeId+'&showfiles=1&char='+animeFL+'#a'+file.animeId, changeWatchedState, 'mark unwatched', 'i_seen_no');
		else 
			icons['mylist_watch'] = createIcon(null, 'mylist.watch ', 'animedb.pl?show=mylist&do=seen&seen=1&lid='+mylistEntry.id+'&expand='+file.animeId+'&showfiles=1&char='+animeFL+'#a'+file.animeId, changeWatchedState, 'mark watched', 'i_seen_yes');
		icons['mylist_edit'] = createIcon(null, 'mylist.edit ', 'animedb.pl?show=mylist&do=add&lid='+mylistEntry.id, null, 'edit mylist entry', 'i_file_edit');
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

/* Function that creates an episode files table row
 * @param eid Episode ID
 * @param fid File ID
 * @return file row
 */
function createFilesTableRow(eid,fid) {
	var row = document.createElement('TR');
	row.id = 'e'+eid+'f'+fid;
	var mylistEntry = mylist[fid];
	var file = files[fid];
	var episode = episodes[eid];
	var icons = createFileIcons(file); // get file icons
	createCell(row, 'check', createCheckBox(null,'mylmod.f.'+mylistEntry.id,'mylmod.f.'+mylistEntry.id,null,false), null);
	if (!icons['expand'])
		createCell(row, 'file id icons', icons['fid'], file.id); // FID
	else {
		var cell = createCell(null, 'file id icons',icons['fid'],file.id);
		cell.appendChild(icons['expand']);
		row.appendChild(cell);
	}
	if (mylist_settings['noeptb']) {
		var epno = createTextLink(null, episode.typeChar+episode.epno, 'animedb.pl?show=ep&eid='+eid, null, null, null, null);
		createCell(row, 'epno', epno, null);
	}
	if (file.type == 'generic') {
		createCell(row, 'name group',createLink(null, 'generic file', 'http://wiki.anidb.info/w/Files:Generic_files', 'extern wiki', null, null, null)); // GROUP
	} else { // group
		var cell = createCell(null, 'name group');
		if (icons['date'] || icons['cmt'] || icons['ver']) {
			var divIcons = document.createElement('SPAN');
			divIcons.className = 'icons';
			if (icons['date']) divIcons.appendChild(icons['date']);
			if (icons['cmt']) divIcons.appendChild(icons['cmt']);
			if (icons['ver']) divIcons.appendChild(icons['ver']);
			cell.appendChild(divIcons);
		}
		var groupEntry = groups[file.groupId];
		cell.setAttribute('anidb:sort',groupEntry.shortName.toLowerCase());
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
		row.appendChild(cell);
	}
	if (file.type == 'generic') {
		var cell = document.createElement('TD');
		cell.colSpan = ((uriObj['showcrc'] && uriObj['showcrc'] == '1') ? 6 : 5);
		row.appendChild(cell);
	} else {
		createCell(row, 'size', document.createElement('NOBR').appendChild(document.createTextNode(formatFileSize(file.size))), file.size); // SIZE
		if (uriObj['showcrc'] && uriObj['showcrc'] == '1') createCell(row, 'crc', document.createTextNode(file.crc32.toUpperCase()));
		var cell = createCell(null, 'lang icons');
		if (icons['audlangs']) { var audlangs = icons['audlangs']; for (var i = 0; i < audlangs.length; i++) cell.appendChild(audlangs[i]); }
		if (icons['sublangs']) { var sublangs = icons['sublangs']; for (var i = 0; i < sublangs.length; i++) cell.appendChild(sublangs[i]); }
		row.appendChild(cell);
		cell = createCell(null, 'codec icons');
		var codecSort = 'unknown';
		if (file.type == 'video' && file.videoTracks[0] && file.videoTracks[0].codec) codecSort = file.videoTracks[0].codec;
		cell.setAttribute('anidb:sort',codecSort);
		if (icons['ext']) cell.appendChild(icons['ext']);
		if (icons['vid']) cell.appendChild(icons['vid']);
		if (icons['audcodecs']) { var audcodecs = icons['audcodecs']; for (var i = 0; i < audcodecs.length; i++) cell.appendChild(audcodecs[i]); }
		row.appendChild(cell);
		createCell(row, 'quality '+file.quality, icons['quality'],mapQuality(file.quality));
		cell = createCell(null, 'hash icons ed2k');
		if (uid != undefined && file.ed2k != '') cell.onmouseover = createHashLink;
		if (icons['ed2k']) cell.appendChild(icons['ed2k']);
		if (icons['crc']) cell.appendChild(icons['crc']);
		if (icons['avdump']) cell.appendChild(icons['avdump']);
		row.appendChild(cell);
	}
	// Storage and Source
	createCell(row, 'storage text', document.createTextNode(mylistEntry.storage));
	createCell(row, 'source text', document.createTextNode(mylistEntry.source));
	var cell = createCell(null,'icons state');
	if (icons['mylist_status']) cell.appendChild(icons['mylist_status']);
	if (icons['mylist_fstate']) cell.appendChild(icons['mylist_fstate']);
	if (icons['mylist_seen']) cell.appendChild(icons['mylist_seen']);
	if (icons['mylist_cmt']) cell.appendChild(icons['mylist_cmt']);
	row.appendChild(cell);
	cell = createCell(null,'icons action');
	if (icons['mylist_watch']) cell.appendChild(icons['mylist_watch']);
	if (icons['mylist_edit']) cell.appendChild(icons['mylist_edit']);
	if (icons['mylist_action']) cell.appendChild(icons['mylist_action']);
	row.appendChild(cell);
	return row;
}

/* Function that creates an episode files table
 * @param eid Episode ID
 * @return Returns an episode file table inside the row
 */
function createFilesTable(eid) {
	var insertRow = document.createElement('TR');
	insertRow.id = 'e'+eid+'_filesRow';
	if (mylist_settings['filemode'] == '2') insertRow.style.display = 'none';
	var insertCell = document.createElement('TD');
	//insertCell.colSpan = (mylist_settings['noeptb']) ? '7' : '6';
	insertCell.colSpan = '7';
	// Create the files table
	var table = document.createElement('TABLE');
	table.className = 'filelist';
	table.id = 'e'+eid+'_filesTable';
	var thead = (fileTableHead ? fileTableHead.cloneNode(true) : createFilesTableHead());
	table.appendChild(thead);
	var tbody = document.createElement('TBODY');
	// Add files
	var episode = episodes[eid];
	for (var i = 0; i < episode.files.length; i++) {
		var fid = episode.files[i];
		var row = createFilesTableRow(eid,fid);
		row.className = ((i % 2) ? '' : 'g_odd ') + 'files';
		tbody.appendChild(row);
	}
	// Piece it all together
	table.appendChild(tbody);
	insertCell.appendChild(table);
	insertRow.appendChild(insertCell);
	return insertRow;
}

/* Creates an episode table thead */
function createEpisodeTableHead() {
	var thead = document.createElement('THEAD');
	var row = document.createElement('TR');
	row.className = 'head';
	createHeader(row, 'expand', 'X', 'Expand/fold files');
	createHeader(row, 'epno', 'EP', null);
	createHeader(row, 'title', 'Title', null);
	createHeader(row, 'duration', 'Len', 'Play length');
	createHeader(row, 'date', 'Date', null);
	createHeader(row, 'count files', 'Files', null);
	createHeader(row, 'action', 'Action', null);
	thead.appendChild(row);
	epTableHead = thead;
	return(epTableHead.cloneNode(true));
}

/* Stub function */
function notImplemented() { alert('Not implemented yet'); }

/* Function that creates the episode table tfoot */
function createEpisodeTableFoot() {
	var tfoot = document.createElement('TFOOT');
	var row = document.createElement('TR');
	var cell = document.createElement('TD');
	var colspan = 12;
	if (mylist_settings['noeptb']) colspan++;
	if (uriObj['showcrc'] && uriObj['showcrc'] == 1)  colspan++;
	cell.colSpan = colspan;
	createCheckBox(cell,'ck',null,cbToggle,false);
	cell.appendChild(document.createTextNode(' select all '));
	var optionArray = [{"value":'0',"text":'Select action',"class":'section',"selected":true},{"value":'seen',"text":'mark watched'},{"value":'unseen',"text":'mark not watched'},
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
	var button = createBasicButton('mylmod.doit','Update selected files','button');
	cell.appendChild(button)
	row.appendChild(cell);
	tfoot.appendChild(row);
	fileTableFoot = tfoot;
	return(fileTableFoot.cloneNode(true));
}

/* Function that creates an episode entry in the episodes table
 * @param aid Anime ID
 * @param eid Episode ID
 */
function createEpisodeTableRow(aid,eid) {
	var row = document.createElement('TR');
	row.id = 'e'+eid;
	var episode = episodes[eid];
	var expandIcon;
	if (mylist_settings['filemode'] == '2')
		expandIcon = createIcon(null, '[+]', null, expandFiles, 'Expand entry', 'i_plus');
	else
		expandIcon = createIcon(null, '[-]', null, foldFiles, 'Expand entry', 'i_minus');
	createCell(row, 'expand', expandIcon, null);
	var epno = createTextLink(null, episode.typeChar+episode.epno, 'animedb.pl?show=ep&eid='+eid, null, null, null, null);
	createCell(row, 'epno', epno, null);
	var eptitle;
	var eptitleURL = 'animedb.pl?show=ep&aid='+aid+'&eid='+eid;
	var altTitle = curTitle = '';
	if (episodeTitleLang != episodeAltTitleLang && 
		episode.titles[episodeAltTitleLang] && 
		episode.titles[episodeAltTitleLang] != '' &&
		episode.titles[episodeAltTitleLang] != curTitle) altTitle = episode.titles[episodeAltTitleLang];
	if (altTitle != '') {
		if (episodeTitleDisplay == 1) eptitle = createTextLink(null, episode.getTitle()+' ('+altTitle+')', eptitleURL, null, null, null, null);
		if (episodeTitleDisplay == 2) eptitle = createTextLink(null, episode.getTitle(), eptitleURL, null, null, mapLanguage(episodeAltTitleLang) + ' title: '+ altTitle, null);
	} else eptitle = createTextLink(null, episode.getTitle(), eptitleURL, null, null, null, null);
	createCell(row, 'title', eptitle, null);
	createCell(row, 'duration', document.createTextNode(episode.length), null);
	var cell = createCell(null, 'date', document.createTextNode(cTimeDate(episode.date)), null);
	if (episode.addDate) cell.title = 'DB add date: '+ episode.addDate;
	row.appendChild(cell);
	createCell(row, 'count files', document.createTextNode(episode.files.length), null);
	createCell(row, 'action', document.createTextNode(' '), null);
	return row;
}

/* Function that crates an episode table
 * @param aid Anime id to create the episode table
 */
function createEpisodeTable(aid) {
	var pRow = document.getElementById('a'+aid);
	// Create the insert row for later insertion
	var insertRow = document.createElement('TR');
	insertRow.id = 'a'+aid+'_epsRow';
	var insertCell = document.createElement('TD');
	insertCell.colSpan = pRow.cells.length;
	var table = document.createElement('TABLE');
	table.className = 'eplist';
	table.id = 'a'+aid+'_episodesTable';
	var thead = (epTableHead ? epTableHead.cloneNode(true) : createEpisodeTableHead());
	table.appendChild(thead);
	var tfoot = (epTableFoot ? epTableFoot.cloneNode(true) : createEpisodeTableFoot());
	var inputs = tfoot.getElementsByTagName('INPUT');
	for (var i = 0; i < inputs.length; i++) {
		var input = inputs[i];
		if (input.type == 'checkbox') input.onchange = cbToggle;
		if (input.type == 'button') input.onclick = notImplemented;
	}
	table.appendChild(tfoot);
	var tbody = document.createElement('TBODY');
	var anime = animes[aid];
	for (var i = 0; i < anime.episodes.length; i++) {
		var eid = anime.episodes[i];
		var row = createEpisodeTableRow(aid,eid);
		row.className = ((i % 2) ? '' : 'g_odd ') + 'files';
		tbody.appendChild(row);
		row = createFilesTable(eid);
		tbody.appendChild(row);
	}
	table.appendChild(tbody);
	// Now piece it all together
	insertCell.appendChild(table);
	insertRow.appendChild(insertCell);
	// add the new row to the mylist table
	pRow.parentNode.insertBefore(insertRow,pRow.nextSibling);
}

// hook up the window onload event
addLoadEvent(prepPage);