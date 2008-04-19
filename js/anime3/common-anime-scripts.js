/* file common anime/episode/files interfaces and functions
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (05.03.2008) - Initial Version (createFileRow, createFileIcons, buildSkips, removeColAttribute, createLoadingRow)
 * version 1.1 (31.03.2008) - Group Rows added (createGroupRow, createGroupIcons)
 * version 1.2 (03.04.2008) - Episode Rows added (createEpisodeRow, createEpisodeIcons)
 * version 1.3 (15.04.2008) - Page Preferences added
 */

/* These are the default file, episode, group and anime cols */
var genFileCols = [ {'name':"check-mylist",'classname':"check",'header':"S",'abbr':"Check/uncheck files"},
					{'name':"check-anime",'classname':"check",'header':"S",'abbr':"Check/uncheck files"},
					{'name':"fid",'classname':"file id icons",'header':"F",'abbr':"File details/FID",'headclass':"file",'sort':"c_set"},
					{'name':"epno",'classname':"epno",'header':"Epno",'abbr':"Episode number",'sort':"c_setlatin"}, 
					{'name':"group",'classname':"name group",'header':"Group",'headclass':"group",'sort':"c_setlatin"},
					{'name':"size",'classname':"size",'nogenerics':true,'header':"Size",'sort':"c_set"}, 
					{'name':"crc",'classname':"crc",'nogenerics':true,'header':"CRC",'sort':"c_latin"},
					{'name':"langs",'classname':"lang icons",'nogenerics':true,'header':"Lang",'headclass':"lang"}, 
					{'name':"cf",'classname':"codec icons",'nogenerics':true,'header':"CF",'abbr':"Container Format",'headclass':"codec",'sort':"c_setlatin"},
					{'name':"resolution",'classname':"resolution",'nogenerics':true,'header':"Resolution",'sort':"c_set"},
					{'name':"anime-source",'classname':"source",'nogenerics':true,'header':"Source",'sort':"c_latin"},
					{'name':"quality",'classname':"quality",'nogenerics':true,'header':"Quality",'sort':"c_set"}, 
					{'name':"hashes",'classname':"hash icons ed2k",'nogenerics':true,'header':"Hash",'headclass':"hash"},
					{'name':"users",'classname':"users count",'header':"Users",'headclass':"users",'sort':"c_set"},
					{'name':"mylist-storage",'classname':"storage text",'header':"Storage",'headclass':"storage"}, 
					{'name':"mylist-source",'classname':"source text",'header':"Source",'headclass':"source"},
					{'name':"state-mylist",'classname':"icons state",'header':"State",'headclass':"state"}, 
					{'name':"state-anime",'classname':"icons state",'header':"State",'headclass':"state"}, 
					{'name':"actions-mylist",'classname':"icons action",'header':"Action",'headclass':"action"},
					{'name':"actions-anime",'classname':"icons action",'header':"Action",'headclass':"action"}	];
					
var genEpCols = [	{'name':"expand",'classname':"expand",'header':"X",'abbr':"Expand/fold"}, 
					{'name':"epno",'classname':"epno",'header':"EP"},
					{'name':"title",'classname':"title",'header':"Title"}, 
					{'name':"duration",'classname':"duration",'header':"Len",'abbr':"Play length"},
					{'name':"date",'classname':"date",'header':"Date"}, 
					{'name':"airdate",'classname':"date airdate",'header':"Air Date"}, 
					{'name':"users",'classname':"number epusers",'header':"Users"},
					{'name':"files",'classname':"count files",'header':"Files",'headclass':"files"},
					{'name':"actions",'classname':"action",'header':"Action"}	];
					
var genGroupCols = [{'name':"expand",'classname':"expand",'header':"X",'abbr':"Expand/fold"}, 
					{'name':"date",'classname':"date lastupdate",'nogenerics':true,'header':"Last Update"},
					{'name':"group",'classname':"name group",'header':"Name"}, 
					{'name':"state",'classname':"state",'nogenerics':true,'header':"State"},
					{'name':"note",'classname':"note",'nogenerics':true,'header':"N",'abbr':"Group Note"}, 
					{'name':"eps",'classname':"epbar",'nogenerics':true,'header':"Episodes",'headclass':"epbar",'sort':"c_set"},
					{'name':"lastep",'classname':"lastep epno",'nogenerics':true,'header':"HE",'headclass':"epno last",'abbr':"Highest episode number"}, 
					{'name':"specials",'classname':"specials number",'nogenerics':true,'header':"SP",'headclass':"specials",'abbr':"Number of specials"},
					{'name':"langs",'classname':"languages",'nogenerics':true,'header':"Languages"}, 
					{'name':"rating",'classname':"rating",'nogenerics':true,'header':"Rating"},
					{'name':"cmts",'classname':"cmts number",'nogenerics':true,'header':"Cmts",'headclass':"cmts",'abbr':"Comments"}, 
					{'name':"actions",'classname':"icons action",'nogenerics':true,'header':"Action",'headclass':"action"}	];
					
var genAnimeCols = [{'name':"expand",'classname':"expand",'header':"X",'abbr':"Expand/fold"},
					{'name':"check",'classname':"check",'header':"S",'abbr':"Select"},
					{'name':"title",'classname':"title",'header':"Title"},
					{'name':"eps",'classname':"stats eps",'header':"Eps",'headclass':"eps"},
					{'name':"seen",'classname':"stats seen",'header':"Seen",'headclass':"seen"},
					{'name':"rating",'classname':"rating anime mid",'header':"Rating",'headclass':"rating"},
					{'name':"mylist",'classname':"vote",'header':"My",'headclass':"my"},
					{'name':"reviews",'classname':"rating attavg mid",'header':"Reviews",'headclass':"attavg"}	];

// Failsafe vars;
var mylist_settings = null;
var ed2k_pattern = (hashObj && hashObj.pattern ? hashObj.pattern : "%ant - %enr%ver - %ept - <[%grp]><(%crc)><(%cen)><(%lang)><(%raw)>");
var space_pattern = (hashObj && hashObj.spacesChar ? hashObj.spacesChar : "_");
var use_mylist_add = 0;
var mylist_add_viewed_state = 0;
var mylist_add_state = 0;
var mylist_add_fstate = 0;
var group_check_type = 0;

/* This is an auxiliar function that removes a given attribute from the cols
 * @param name Name of the column to remove
 * @param cols Cols var
 * @return A copy of the cols var without the given attribute
 */
function removeColAttribute(name,cols) {
	var index = -1;
	for (var i = 0; i < cols.length; i++) {
		if (cols[i]['name'] != name) continue;
		index = i;
		break;
	}
	if (index > -1) cols.splice(index,1);
	return cols;
}

/* This function will build a col skip array
 * @param cols The original cols
 * @return array with the col skips
 */
function buildSkipCols(cols) {
	var skips = new Array();
	var startIndex = -1;
	var backlogged = 0;
	var i = 0;
	for (var i = 0; i < cols.length; i++) {
		if (!cols[i]['nogenerics']) {
			startIndex = i;
			skips[i] = (backlogged ? backlogged : 1);
			backlogged = 0;
			continue;
		}
		if (startIndex > -1) skips[startIndex]++;
		else backlogged++;
	}
	return skips;
}

/* This function creates and returns an Object with all the possible icons for a given episode
 * @param group Group to build icon object
 * @return an Object with the icons
 * ids: expand, seen, state, fstate
 */
function createEpisodeIcons(episode) {
	var icons = new Object();
	if (mylist_settings && mylist_settings['filemode'] == '2') {
		var a = createIcon(null, '[-]', null, (mylist_settings ? foldFiles : foldEp), 'Fold this entry', 'i_minus');
		a.style.cursor = 'pointer';
		icons['expand'] = a;
	} else {
		var a = createIcon(null, '[+]', null, (mylist_settings ? expandFiles : expandEp), 'Expand this entry', 'i_plus');
		a.style.cursor = 'pointer';
		icons['expand'] = a;
	}
	var mylistEpEntries = findMylistEpEntries(episode.id);
	if (mylistEpEntries.length) {
		// Loop to see if an entry should get a status, and file state
		var stateFiles = new Object();
		var statusFiles = new Object();
		icons['state'] = new Array();
		icons['fstate'] = new Array();
		for (var me = 0; me < mylistEpEntries.length; me++) {
			var mylistEntry = mylistEpEntries[me];
			if (isNaN(statusFiles[mylistEntry.status])) statusFiles[mylistEntry.status] = 1;
			else statusFiles[mylistEntry.status]++;
			if (isNaN(stateFiles[mylistEntry.fstate])) stateFiles[mylistEntry.fstate] = 1;
			else stateFiles[mylistEntry.fstate]++;
		}
		for (var st in statusFiles) {
			var status = statusFiles[st];
			if (isNaN(status)) continue;
			//if (status == null || !status) continue;
			var stClass = 'i_icon i_state_'+mapMEStatusName(st);
			var txt = status + ' file' + (status > 1 ? 's' : '') + ' with status: '+st;
			icons['state'].push(createLink(null, txt, 'http://wiki.anidb.net/w/Filetype', 'anidb::wiki', null, txt, stClass));
		}
		for (var st in stateFiles) {
			if (st == 'indexOf' || st == 'unknown') continue;
			var state = stateFiles[st];
			if (isNaN(state)) continue;
			//if (state == null || !state) continue;
			var stClass = 'i_icon '+mapFState(st);
			var txt = state + ' file' + (state > 1 ? 's' : '') + ' with state: '+st;
			icons['fstate'].push(createLink(null, txt, 'http://wiki.anidb.net/w/Filetype', 'anidb::wiki', null, txt, stClass));
		}
		if (episode.seenDate)
			icons['seen'] = createIcon(null, 'seen ', null, null, 'seen on: '+cTimeDate(episode.seenDate), 'i_seen');
	}
	return icons;
}
/* Function that creates an episode table row
 * @param aid Anime ID
 * @param eid Episode ID
 * @param cols Array of columns to show, should have at least attribute 'name' defined
		Attributes are the following:
			- name 		: Column name (for identifiying how the data should be handeled)
			- classname : Classname to use for the column
			- nogenerics: Ignore this cell on generic files
 * @param skips Array with colspans for generic classes (optional)
 * @return episode row
 */
function createEpisodeRow(aid,eid,cols,skips) {
	var row = document.createElement('tr');
	row.id = 'e'+eid;
	var episode = episodes[eid];
	var icons = createEpisodeIcons(episode); // get icons
	for (var c = 0; c < cols.length; c++) {
		var col = cols[c];
		var colSpan = 1;
		if (episode.id == 0 && skips) {
			if (col['nogenerics']) continue;
			colSpan = skips[c];
		}
		switch(col['name']) {
			case 'expand':
				createCell(row, col['classname'], icons['expand'], null, colSpan);
				break;
			case 'epno':
				var epno = createTextLink(null, episode.typeChar+episode.epno, 'animedb.pl?show=ep&eid='+eid, null, null, null, null);
				createCell(row, col['classname'], epno, null, colSpan);
				break;
			case 'title':
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
				if (episode.seenDate != 0) {
					var watchedState = document.createElement('span');
					watchedState.className = 'icons';
					watchedState.appendChild(icons['seen']);
					var cell = createCell(null, col['classname'], watchedState, null, colSpan);
					cell.appendChild(eptitle);
					row.appendChild(cell);
				} else createCell(row, col['classname'], eptitle, null, colSpan);
				break;
			case 'duration':
				createCell(row, col['classname'], document.createTextNode(episode.length), null, colSpan);
				break;
			case 'date':
				var cell = createCell(null, col['classname'], document.createTextNode(cTimeDate(episode.date)), null, colSpan);
				if (episode.addDate) cell.title = 'DB add date: '+ episode.addDate;
				row.appendChild(cell);
				break;
			case 'files':
				createCell(row, col['classname'], document.createTextNode(episode.files.length), null, colSpan);
				break;
			case 'actions':
				createCell(row, col['classname'], document.createTextNode(' '), null, colSpan);
				break;
			default:
				errorAlert('createEpisodeRow','unknown column with name: '+col['name']);
		}
	}
	return row;
}

/* This function creates and returns an Object with all the possible icons for a given group
 * @param group Group to build icon object
 * @return an Object with the icons
 * ids: audlangs, sublangs, note, comment, vote, edit
 */
function createGroupIcons(group) {
	var icons = new Object();
	var audLangs = new Array();
	var subLangs = new Array();
	if (group.audioLangs) {
		for (var i = 0; i < group.audioLangs.length; i++)
			audLangs.push(createIcon(null, mapLanguage(group.audioLangs[i]), '', '', 'audio lang: '+mapLanguage(group.audioLangs[i]), 'i_audio_'+group.audioLangs[i]));
	}
	if (group.subtitleLangs) {
		for (var i = 0; i < group.subtitleLangs.length; i++)
			subLangs.push(createIcon(null, mapLanguage(group.subtitleLangs[i]), '', '', 'sub lang: '+mapLanguage(group.subtitleLangs[i]), 'i_sub_'+group.subtitleLangs[i]));
	}
	icons['audlangs'] = audLangs;
	icons['sublangs'] = subLangs;
	var a = createIcon(null, 'Expand this release', 'removeme', expandFilesByGroup, 'Expand this release', 'i_plus');
	a.removeAttribute('href');
	a.style.cursor = 'pointer';
	icons['expand'] = a;
	icons['note'] = document.createTextNode(' ');
	icons['comment'] = createLink(null,'cmt','animedb.pl?show=cmt&do=animegroup&id='+group.agid,'anidb::popup',null,'Comment on this release','i_icon i_group_comment 600.500.1.1.agcmts');
	icons['vote'] = createLink(null,
				   (group.userRating > -1) ? '['+group.userRating+']' : 'rate it','animedb.pl?show=agvote&id='+group.agid,
				   'anidb::popup',
				   null,
				   (group.userRating > -1) ? 'Your vote: '+group.userRating : 'Rate this release',
				   'i_icon ' +((group.userRating > -1) ? 'i_revote' : 'i_vote' )+' 380.140.0.0.agvote');
	icons['edit'] = createLink(null,'edit','animedb.pl?show=animegroup&agid='+group.agid,'anidb::popup',null,'Request edit of the state','i_icon i_file_edit 400.400.0.0.agstate');
	return icons;
}

/* Function that creates a group table row
 * @param gid Group ID
 * @param cols Array of columns to show, should have at least attribute 'name' defined
		Attributes are the following:
			- name 		: Column name (for identifiying how the data should be handeled)
			- classname : Classname to use for the column
			- nogenerics: Ignore this cell on generic files
 * @param skips Array with colspans for generic classes (optional)
 * @return group row
 */
function createGroupRow(gid,cols,skips) {
	if (!cols) { errorAlert('createGroupRow','no cols given'); return; }
	var row = document.createElement('tr');
	row.id = 'gid_'+gid;
	var group = groups[gid];
	row.className = group.state;
	if (group.filtered) row.style.display = 'none';
	var icons = createGroupIcons(group); // get icons
	for (var c = 0; c < cols.length; c++) {
		var col = cols[c];
		var colSpan = 1;
		if (group.id == 0 && skips) {
			if (col['nogenerics']) continue;
			colSpan = skips[c];
		}
		switch(col['name']) {
			case 'expand':
				createCell(row, col['classname'], (uid ? icons['expand'] : document.createTextNode(' ')), null, colSpan);
				break;
			case 'date':
				var newFile = (Number(new Date())/1000 - Number(javascriptDate(cTimeDate(group.lastUp)))/1000 < 86400) ? ' new' : '';
				createCell(row, col['classname']+newFile, document.createTextNode(cTimeDate(group.lastUp)), null, colSpan);
				break;
			case 'group':
				createCell(row, col['classname'], createLink(null,group.shortName,'animedb.pl?show=group&gid=' + group.id, null, null, group.name, null), group.shortName.toLowerCase(), colSpan);
				break;
			case 'state':
				createCell(row, col['classname']+ ' '+group.state, createLink(null,group.state,'animedb.pl?show=group&gid='+group.id+'&aid='+anime.id,null,null,null,null), null, colSpan);
				break;
			case 'note': 
				createCell(row, col['classname'], icons['note'], null, colSpan);
				break;
			case 'eps':
				var maps = {'0' : {'use':true,'type': 0,'desc':"",'img':"blue"},
							'1' : {'use':false,'type': 1,'desc':"done: "+group.epRange,'img':"darkblue"},
							'2' : {'use':false,'type': 2,'desc':"in mylist: "+group.inMylistRange,'img':"lime"}};
				var range = expandRange(null,(anime.eps ? anime.eps : anime.epCount),maps[0],null);
				if (group.epRange != '') { maps[1]['use'] = true; range = expandRange(group.epRange,(anime.eps ? anime.eps : anime.epCount),maps[1],range); }
				if (group.isInMylistRange != '') { maps[2]['use'] = true; range = expandRange(group.isInMylistRange,(anime.eps ? anime.eps : anime.epCount),maps[2],range); }
				createCell(row, col['classname'], makeCompletionBar(null,range,maps),String(group.epCnt),colSpan);
				break;
			case 'lastep':
				createCell(row, col['classname'], document.createTextNode(group.lastEp), mapEpisodeNumber(group.lastEp), colSpan);
				break;
			case 'specials':
				createCell(row, col['classname'], document.createTextNode(group.sepCnt), group.sepCnt, colSpan);
				break;
			case 'langs':
				var cell = createCell(null, col['classname'],null,null,colSpan);
				if (icons['audlangs']) { var audlangs = icons['audlangs']; for (var i = 0; i < audlangs.length; i++) cell.appendChild(audlangs[i]); }
				if (icons['sublangs']) { var sublangs = icons['sublangs']; for (var i = 0; i < sublangs.length; i++) cell.appendChild(sublangs[i]); }
				row.appendChild(cell);
				break;
			case 'rating':
				createCell(row, col['classname'],
							createLink(null,((group.rating == '-') ? 'N/A' : group.rating) + ' ('+group.ratingCount+')','animedb.pl?show=animegroupvotes&agid='+group.agid,'anidb::popup',null,null,'350.400.1.1.agvotes'),
							((group.rating == '-') ? '0' : group.rating), colSpan);
				break;
			case 'cmts':
				createCell(row, col['classname'],
							createLink(null,group.commentCount,'animedb.pl?show=cmt&do=animegroup&id='+group.agid,'anidb::popup',null,null,'600.500.1.1.agcmts action'),
							group.commentCount, colSpan);
				break;
			case 'actions':
				var cell = createCell(null, col['classname'], null, null, colSpan);
				if (uid) {
					cell.appendChild(icons['comment']);
					cell.appendChild(icons['vote']);
					cell.appendChild(icons['edit']);
					row.appendChild(cell);
				}
				break;
			default:
				errorAlert('createGroupRow','unknown column with name: '+col['name']);
		}
	}
	return row;
}

/* This function creates and returns an Object with all the possible icons for a given file
 * @param file File to build icon object
 * @return an Object with the icons
 * ids: fid, ext, vid, audlangs, audcodecs, sublangs, crc, ver, cenfile_ep_rel, file_file_rel,
 *      cmt, date, mylist, mylist_status, mylist_fstate, mylist_seen, mylist_cmt, mylist_action,
 *      mylist_watch, ed2k, edit, expand, quality, avdump
 */
function createFileIcons(file) {
	var icons = new Object();
	// fid
	if (file.pseudoFile || (file.relatedFiles && file.relatedFiles.length)) {
		var a = createIcon(null, '[+]', null, expandFiles, 'expand this entry', 'i_plus');
		a.style.cursor = 'pointer';
		icons['expand'] = a;
	}
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
	if (file.type == 'video') { // VIDEO StrEAM
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
	if (file.type == 'video' || file.type == 'audio') { // AUDIO StrEAM
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
	if (file.type == 'video' || file.type == 'subtitle' || file.type == 'other') { // SUBTITLE StrEAM
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
		icons['mylist_status'] = createLink(null, mylistEntry.status+' ', 'http://wiki.anidb.net/w/Filetype', 'anidb::wiki', null, tooltip, 'i_icon i_state_'+className);
		//createIcon(null, mylistEntry.status+' ', null, null, tooltip, 'i_state_'+className);
		// Mylist FileState
		if (mylistEntry.fstate && mylistEntry.fstate != 'unknown') {
			var className = mapFState(mylistEntry.fstate);
			var tooltip = 'mylist state: '+mylistEntry.fstate;
			icons['mylist_fstate'] = createLink(null, mylistEntry.fstate+' ', 'http://wiki.anidb.net/w/Filetype', 'anidb::wiki', null, tooltip, 'i_icon '+className);
			//createIcon(null, mylistEntry.fstate+' ', null, null, tooltip, className);
		}
		var animeFL = anime.getTitle().charAt(0).toLowerCase();
		// Seen status
		if (mylistEntry.seen) 
			icons['mylist_seen'] = createIcon(null, 'seen ', null, null, 'seen on: '+cTimeDate(mylistEntry.seenDate), 'i_seen');
		// mylist comment
		if ((mylistEntry.other) && (mylistEntry.other != '') && (mylistEntry.other != undefined)) 
			icons['mylist_cmt'] = createIcon(null, 'mylist comment ', null, null, 'mylist comment: '+mylistEntry.other, 'i_comment');
		// mylist action
		icons['mylist_remove'] = createIcon(null, 'mylist.remove ', 'animedb.pl?show=mylist&do=del&lid='+mylistEntry.id+'&expand=' + file.animeId + '&showfiles=1&char='+animeFL+'#a'+file.animeId, removeFromMylist, 'remove from mylist', 'i_file_removemylist');
		if (mylistEntry.seen) 
			icons['mylist_watch'] = createIcon(null, 'mylist.unwatch ', 'animedb.pl?show=mylist&do=seen&seen=0&lid='+mylistEntry.id+'&expand='+ file.animeId+'&showfiles=1&char='+animeFL+'#a'+file.animeId, changeWatchedState, 'mark unwatched', 'i_seen_no');
		else 
			icons['mylist_watch'] = createIcon(null, 'mylist.watch ', 'animedb.pl?show=mylist&do=seen&seen=1&lid='+mylistEntry.id+'&expand='+file.animeId+'&showfiles=1&char='+animeFL+'#a'+file.animeId, changeWatchedState, 'mark watched', 'i_seen_yes');
		icons['mylist_edit'] = createIcon(null, 'mylist.edit ', 'animedb.pl?show=mylist&do=add&lid='+mylistEntry.id, null, 'edit mylist entry', 'i_file_edit');
	} else {
		// mylist action
		icons['mylist_add'] = createIcon(null, 'mylist.add ', 'animedb.pl?show=mylist&do=add&fid=' + file.id, addToMylist, 'add to mylist', 'i_file_addmylist');
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
 * @param cols Array of columns to show, should have at least attribute 'name' defined
		Attributes are the following:
			- name 		: Column name (for identifiying how the data should be handeled)
			- classname : Classname to use for the column
			- nogenerics: Ignore this cell on generic files
 * @param skips Array with colspans for generic classes (optional)
 * @return file row
 */
function createFileRow(eid,fid,cols,skips) {
if (!cols) { errorAlert('createFileRow','no cols given'); return; }
	var row = document.createElement('tr');
	var mylistEntry = mylist[fid];
	var file = files[fid];
	var episode = episodes[eid];
	row.id = 'e'+eid+(file.pseudoFile ? 'r' : '')+'f'+fid;
	if (file.type == 'generic') row.className = "generic no_sort";
	var icons = createFileIcons(file); // get file icons
	for (var c = 0; c < cols.length; c++) {
		var col = cols[c];
		var colSpan = 1;
		if (file.type == 'generic' && skips) {
			if (col['nogenerics']) continue;
			colSpan = skips[c];
		}
		switch(col['name']) {
			case 'check-mylist':
				createCell(row, col['classname'], createCheckBox(null,'mylmod.f.'+mylistEntry.id,'mylmod.f.'+mylistEntry.id,null,false), null, colSpan);
				break;
			case 'check-anime':
				createCell(row, col['classname'], createCheckBox(null,'madd.f.'+file.id,'madd.f.'+file.id,null,false), null, colSpan);
				break;
			case 'fid':
				if (!icons['expand'])
					createCell(row, col['classname'], icons['fid'], file.id, colSpan);
				else {
					var cell = createCell(null, col['classname'], icons['fid'], file.id, colSpan);
					cell.appendChild(icons['expand']);
					row.appendChild(cell);
				}
				break;
			case 'epno':
				var epno = createTextLink(null, episode.typeChar+episode.epno, 'animedb.pl?show=ep&eid='+eid, null, null, null, null);
				createCell(row, col['classname'], epno, null, colSpan);
				break;
			case 'group':
				if (file.type == 'generic') {
					createCell(row, col['classname'], createLink(null, 'generic file', 'http://wiki.anidb.info/w/Files:Generic_files', 'extern wiki', null, null, null), null, colSpan); // GROUP
				} else { // group
					var cell = createCell(null, col['classname']);
					if (icons['date'] || icons['cmt'] || icons['ver']) {
						var divIcons = document.createElement('span');
						divIcons.className = 'icons';
						if (icons['date']) divIcons.appendChild(icons['date']);
						if (icons['cmt']) divIcons.appendChild(icons['cmt']);
						if (icons['ver']) divIcons.appendChild(icons['ver']);
						cell.appendChild(divIcons);
					}
					var groupEntry = groups[file.groupId];
					cell.setAttribute('anidb:sort',groupEntry.shortName.toLowerCase());
					if (!file.groupId) groupEntry = null;
					var label = document.createElement('label');
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
				break;
			case 'mylist-storage':
				createCell(row, col['classname'], document.createTextNode(mylistEntry.storage), null, colSpan);
				break;
			case 'mylist-source':
				createCell(row, col['classname'], document.createTextNode(mylistEntry.source), null, colSpan);
				break;
			case 'users':
				cell = createCell(null, col['classname'],createLink(null, file.usersTotal, 'animedb.pl?show=userlist&fid=' + file.id, null, null, 'total users', null),file.usersTotal, colSpan);
				cell.appendChild(document.createTextNode('/'));
				var abbr = document.createElement('abbr');
				abbr.title = 'deleted';
				abbr.appendChild(document.createTextNode(file.usersDeleted));
				cell.appendChild(abbr);
				cell.appendChild(document.createTextNode('/'));
				abbr = document.createElement('abbr');
				abbr.title = 'unknown status';
				abbr.appendChild(document.createTextNode(file.usersUnknown));
				cell.appendChild(abbr);
				row.appendChild(cell);
				break;
			case 'state-mylist':
			case 'state-anime':
				cell = createCell(null, col['classname'], null, null, colSpan);
				if (col['name'] == 'state-anime' && icons['mylist']) cell.appendChild(icons['mylist']);
				if (icons['mylist_status']) cell.appendChild(icons['mylist_status']);
				if (icons['mylist_fstate']) cell.appendChild(icons['mylist_fstate']);
				if (icons['mylist_seen']) cell.appendChild(icons['mylist_seen']);
				if (icons['mylist_cmt']) cell.appendChild(icons['mylist_cmt']);
				row.appendChild(cell);
				break;
			case 'actions-mylist':
				cell = createCell(null, col['classname'], null, null, colSpan);
				if (icons['mylist_watch']) cell.appendChild(icons['mylist_watch']);
				if (icons['mylist_edit']) cell.appendChild(icons['mylist_edit']);
				if (icons['mylist_remove']) cell.appendChild(icons['mylist_remove']);
				row.appendChild(cell);
				break;
			case 'actions-anime':
				cell = createCell(null, col['classname'], null, null, colSpan);
				if (icons['edit']) cell.appendChild(icons['edit']);
				if (icons['mylist_add']) cell.appendChild(icons['mylist_add']);
				if (icons['mylist_remove']) cell.appendChild(icons['mylist_remove']);
				if (icons['mylist_watch']) cell.appendChild(icons['mylist_watch']);
				row.appendChild(cell);
				break;
			case 'size':
				if (file.type != 'generic') createCell(row, col['classname'], document.createElement('nobr').appendChild(document.createTextNode(formatFileSize(file.size))), file.size, colSpan);
				else createCell(row, col['classname'], document.createTextNode(' '), null, colSpan);
				break;
			case 'crc':
				createCell(row, col['classname'], document.createTextNode(file.crc32.toUpperCase()), null, colSpan);
				break;
			case 'resolution':
				createCell(row, col['classname'], ((file.type == 'video') ? document.createTextNode(file.resolution) : document.createTextNode('n/a')), file.pixelarea, colSpan);
				break;
			case 'langs':
				cell = createCell(null, col['classname'], null, null, colSpan);
				if (icons['audlangs']) { var audlangs = icons['audlangs']; for (var i = 0; i < audlangs.length; i++) cell.appendChild(audlangs[i]); }
				if (icons['sublangs']) { var sublangs = icons['sublangs']; for (var i = 0; i < sublangs.length; i++) cell.appendChild(sublangs[i]); }
				row.appendChild(cell);
				break;
			case 'cf':
				cell = createCell(null, col['classname'], null, null, colSpan);
				var codecSort = 'unknown';
				if (file.type == 'video' && file.videoTracks[0] && file.videoTracks[0].codec) codecSort = file.videoTracks[0].codec;
				cell.setAttribute('anidb:sort',codecSort);
				if (icons['ext']) cell.appendChild(icons['ext']);
				if (icons['vid']) cell.appendChild(icons['vid']);
				if (icons['audcodecs']) { var audcodecs = icons['audcodecs']; for (var i = 0; i < audcodecs.length; i++) cell.appendChild(audcodecs[i]); }
				row.appendChild(cell);
				break;
			case 'quality':
				createCell(row, 'quality '+file.quality, icons['quality'], mapQuality(file.quality), null, colSpan);
				break;
			case 'hashes':
				cell = createCell(null, col['classname'], null, null, colSpan);
				if (uid != undefined && file.ed2k != '') cell.onmouseover = createHashLink;
				if (icons['ed2k']) cell.appendChild(icons['ed2k']);
				if (icons['crc']) cell.appendChild(icons['crc']);
				if (icons['avdump']) cell.appendChild(icons['avdump']);
				row.appendChild(cell);
				break;
			case 'anime-source':
				var cell = createCell(null, col['classname'], null, null, colSpan);
				if (icons['cen']) {
					var divIcons = document.createElement('span');
					divIcons.className = 'icons';
					if (icons['cen']) divIcons.appendChild(icons['cen']);
					cell.appendChild(divIcons);
				}
				var label = document.createElement('label');
				label.appendChild(document.createTextNode(file.source));
				cell.appendChild(label);
				row.appendChild(cell);
				break;
			default:
				errorAlert('createFileRow','unknown column with name: '+col['name']);
		}
	}
	return row;
}

/* Function that creates a table head from a cols definition
 * @param cols Columns definition, needs 'header' and at least 'classname' if 'headclass' is duplicated, 'abbr' is optional
 * @return Returns THEAD element
 */
function createTableHead(cols) {
	var thead = document.createElement('thead');
	var row = document.createElement('tr');
	for (var i = 0; i < cols.length; i++) {
		var className = cols[i]['headclass'] ? cols[i]['headclass'] : cols[i]['classname'];
		if (cols[i]['sort']) className += ' '+cols[i]['sort'];
		createHeader(row, className, cols[i]['header'], cols[i]['abbr']);
	}
	thead.appendChild(row);
	return thead;
}

/* Function that creates an auxiliar loading row
 * @param cols Number of cols
 * @return loading row
 */
function createLoadingRow(colSpan) {
	var row = document.createElement('tr');
	createCell(row, 'loading', document.createTextNode('please wait while loading data...'), null, colSpan);
	return row;
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
	CookieSet(name,value);
}

/* Function that creates the preferences table
 * @param type Type can be either mylist or anime
 */
function createPreferencesTable(type) {
	var mylistItems =	[	{'id':"title-prefs",'head':"Title",'title':"Title Preferences",'default':true},
							{'id':"ed2k-prefs",'head':"ED2K",'title':"ED2K Link Preferences"}	];
	var animeItems =	[	{'id':"title-prefs",'head':"Title",'title':"Title Preferences",'default':true},
							{'id':"ed2k-prefs",'head':"ED2K",'title':"ED2K Link Preferences"}	,
							{'id':"mylist-prefs",'head':"Mylist",'title':"Mylist Quick-Add Preferences"},
							{'id':"group-prefs",'head':"Group",'title':"Group select Preferences"}	];
	var items;
	if (type == 'anime') items = animeItems;
	else items = mylistItems;

	/* load settings from cookie */

	episodeAltTitleLang = CookieGet('episodeAltTitleLang') || "x-jat";
	episodeTitleDisplay = CookieGet('episodeTitleDisplay') || 2;
	ed2k_pattern = CookieGet('ed2k_pattern') || "%ant - %enr%ver - %ept - <[%grp]><(%crc)><(%cen)><(%lang)><(%raw)>";
	hashObj.pattern = ed2k_pattern;
	space_pattern = CookieGet('space_pattern') || "_";
	hashObj.spacesChar = space_pattern;
	use_mylist_add = CookieGet('use_mylist_add') || 0;
	mylist_add_viewed_state = CookieGet('mylist_add_viewed_state') || 0;
	mylist_add_state = CookieGet('mylist_add_state') || 0;
	mylist_add_fstate = CookieGet('mylist_add_fstate') || 0;
	group_check_type = CookieGet('group_check_type') || 0;
	
	/* create preferences tabs */

	var main = document.createElement('div');
	main.className = "g_section preferences";
	main.style.display = 'none';
	main.id = 'page_prefs';
	var panes = document.createElement('div');
	panes.className = "tabbed_pane";
	var ul_tabs = document.createElement('ul');
	ul_tabs.className = 'tabs';
	for (var t = 0; t < items.length; t++) {
		var li = document.createElement('li');
		li.id = "pref"+(t+1);
		li.className = "tab" + (items[t]['default'] ? ' selected' : '');
		li.appendChild(document.createTextNode(items[t]['head']));
		li.onclick = Magic.toggle_tabs;
		ul_tabs.appendChild(li);
	}
	
	panes.appendChild(ul_tabs);
	var body = document.createElement('div');
	body.className = 'body';
	for (var t = 0; t < items.length; t++) {
		var item = items[t];
		var tab = document.createElement('div');
		tab.id = "pref"+(t+1)+"_pane";
		tab.className = "pane" + (item['default'] ? '' : ' hide');
		var h4 = document.createElement('h4');
		h4.appendChild(document.createTextNode(item['title']));
		tab.appendChild(h4);
		switch(item['id']) {
			case 'title-prefs':
				var ul = document.createElement('ul');
				var li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_TITLE', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				createLanguageSelect(li,'episodeAltTitleLang','episodeAltTitleLang',function() { changeOptionValue(this); episodeAltTitleLang = this.value; },episodeAltTitleLang);
				li.appendChild(document.createTextNode(' Episode Alternative Title Language'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_TITLE', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var optionArray = { 0 : {"text":'Title'} , 1 : {"text":'Title (Alt. Title)'}, 2 : {"text":'Title [tooltip: Alt. Title]'}};
				createSelectArray(li,'episodeTitleDisplay','episodeTitleDisplay',function() { changeOptionValue(this); episodeTitleDisplay = this.value; },episodeTitleDisplay,optionArray);
				li.appendChild(document.createTextNode(' Episode Alternative Title Display'));
				ul.appendChild(li);
				var actionLI = document.createElement('li');
				actionLI.className = 'action';
				actionLI.appendChild(document.createTextNode('Actions: '));
				var reloadInput = createBasicButton('do.reload','reload page');
				reloadInput.onclick = function reloadPage() { document.location.href = document.location.href; }
				actionLI.appendChild(reloadInput);
				actionLI.appendChild(document.createTextNode(' '));
				var saveInput = createBasicButton('do.save','save preferences');
				saveInput.onclick = function saveSettings() {
					CookieSet('episodeAltTitleLang',episodeAltTitleLang);
					CookieSet('episodeTitleDisplay',episodeTitleDisplay);
					alert('Current Title preferences saved.');
				}
				actionLI.appendChild(saveInput);
				ul.appendChild(actionLI);
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
					hashObj.pattern = ed2k_pattern;
					hashObj.ed2k = "ed2k://|file|"+hashObj.pattern+".%ext|%flen|%ed2k|";
				};
				li.appendChild(input);
				var setDefault = createBasicButton('set_ed2k_default','default');
				setDefault.onclick = function sd() {
					var input = document.getElementById('ed2k_pattern');
					if (input) input.value = hashObj.defaultPattern;
					ed2k_pattern = hashObj.defaultPattern;
					hashObj.pattern = ed2k_pattern;
					hashObj.ed2k = "ed2k://|file|"+hashObj.pattern+".%ext|%flen|%ed2k|";
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
					hashObj.spacesChar = space_pattern;
				};
				li.appendChild(setSpace);
				var setSpaceDefault = createBasicButton('set_space_default','default');
				setSpaceDefault.onclick = function ssd() {
					var input = document.getElementById('space_pattern');
					if (input) input.value = hashObj.defaultSpacesChar;
					space_pattern = hashObj.defaultSpacesChar;
					hashObj.spacesChar = space_pattern;
				}
				li.appendChild(document.createTextNode(' '));
				li.appendChild(setSpaceDefault);
				li.appendChild(document.createTextNode(' ED2K hash spaces convert character'));
				ul.appendChild(li);
				var actionLI = document.createElement('li');
				actionLI.className = 'action';
				actionLI.appendChild(document.createTextNode('Actions: '));
				var reloadInput = createBasicButton('do.reload','reload page');
				reloadInput.onclick = function reloadPage() { document.location.href = document.location.href; }
				actionLI.appendChild(reloadInput);
				actionLI.appendChild(document.createTextNode(' '));
				var applyInput = createBasicButton('do.apply','apply changes');
				applyInput.onclick = function reloadPage() { alert('Settings applied\nYou may need to reload page before seeing results.'); }
				actionLI.appendChild(applyInput);
				actionLI.appendChild(document.createTextNode(' '));
				var saveInput = createBasicButton('do.save','save preferences');
				saveInput.onclick = function saveSettings() {
					CookieSet('ed2k_pattern',ed2k_pattern);
					CookieSet('space_pattern',space_pattern);
					alert('Current ED2K preferences saved.');
				}
				actionLI.appendChild(saveInput);
				ul.appendChild(actionLI);
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
				var actionLI = document.createElement('li');
				actionLI.className = 'action';
				actionLI.appendChild(document.createTextNode('Actions: '));
				var reloadInput = createBasicButton('do.apply','apply changes');
				reloadInput.onclick = function reloadPage() { alert('Settings applied.'); }
				actionLI.appendChild(reloadInput);
				actionLI.appendChild(document.createTextNode(' '));
				var saveInput = createBasicButton('do.save','save preferences');
				saveInput.onclick = function saveSettings() {
					CookieSet('use_mylist_add',use_mylist_add);
					CookieSet('mylist_add_state',mylist_add_state);
					CookieSet('mylist_add_fstate',mylist_add_fstate);
					CookieSet('mylist_add_viewed_state',mylist_add_viewed_state);
					alert('Current Mylist preferences saved.');
				}
				actionLI.appendChild(saveInput);
				ul.appendChild(actionLI);
				tab.appendChild(ul);
				break;
			case 'group-prefs':
				var ul = document.createElement('ul');
				var li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				var rb = createBasicButton('group_check_type',0,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 0);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprectated files'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				rb = createBasicButton('group_check_type',1,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 1);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprectated mkv/ogm files'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				rb = createBasicButton('group_check_type',2,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 2);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprectated avi files'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				rb = createBasicButton('group_check_type',3,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 3);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprectated High Definition files (video resolution height >= 720)'));
				ul.appendChild(li);
				li = document.createElement('li');
				createLink(li, '[?]', 'http://wiki.anidb.net/w/PAGE_PREFERENCES_GROUP', 'wiki', null, 'Those who seek help shall find it.', 'i_inline i_help');
				rb = createBasicButton('group_check_type',4,'radio');
				rb.onchange = function() { changeOptionValue(this); group_check_type = this.value; };
				rb.checked = (group_check_type == 4);
				li.appendChild(rb);
				li.appendChild(document.createTextNode(' Check all non-deprectated Standard Definition files (video resolution height < 720)'));
				ul.appendChild(li);
				var actionLI = document.createElement('li');
				actionLI.className = 'action';
				actionLI.appendChild(document.createTextNode('Actions: '));
				var reloadInput = createBasicButton('do.apply','apply changes');
				reloadInput.onclick = function reloadPage() { alert('Settings applied.'); }
				actionLI.appendChild(reloadInput);
				actionLI.appendChild(document.createTextNode(' '));
				var saveInput = createBasicButton('do.save','save preferences');
				saveInput.onclick = function saveSettings() {
					CookieSet('group_check_type',group_check_type);
					alert('Current Group preferences saved.');
				}
				actionLI.appendChild(saveInput);
				ul.appendChild(actionLI);
				tab.appendChild(ul);
				break;
			default:
		}
		body.appendChild(tab);
	}
	panes.appendChild(body);
	main.appendChild(panes);

	// set this before the links
	if (type == 'anime') {
		var links = getElementsByClassName(document.getElementsByTagName('ul'),'g_list links', true)[1];
		if (links) {
			var lastLi = links.getElementsByTagName('li');
			lastLi = lastLi[lastLi.length-1];
			var li = document.createElement('li');
			li.className = (lastLi.className.indexOf('g_odd') < 0 ? 'g_odd' : '');
			createLink(li, 'Page Preferences', 'removeme', null, function () { var pagePrefs = document.getElementById('page_prefs'); if(!pagePrefs) return; pagePrefs.style.display = (pagePrefs.style.display == 'none' ? '' : 'none'); }, null, null);
			links.appendChild(li);
			links.parentNode.insertBefore(main,links);
		}
	}
	if (type == 'mylist') {
		var links = getElementsByClassName(document.getElementsByTagName('ul'),'g_list jump', true)[1];
		var userLinks = getElementsByClassName(document.getElementsByTagName('div'),'user', false)[0];
		if (userLinks) {
			var ul = userLinks.getElementsByTagName('ul')[0];
			var lastLi = ul.getElementsByTagName('li');
			lastLi = lastLi[lastLi.length-1];
			var li = document.createElement('li');
			li.className = (lastLi.className.indexOf('g_odd') < 0 ? 'g_odd' : '');
			createLink(li, 'Page Prefs', 'removeme', null, function () { var pagePrefs = document.getElementById('page_prefs'); if(!pagePrefs) return; pagePrefs.style.display = (pagePrefs.style.display == 'none' ? '' : 'none'); }, null, null);
			ul.appendChild(li);			
		}
		if (links) links.parentNode.insertBefore(main,links);
	}
}
