/* file common anime/episode/files interfaces and functions
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (05.03.2008) - Initial Version (createFileRow, createFileIcons, buildSkips, removeColAttribute, createLoadingRow)
 * version 1.1 (31.03.2008) - Group Rows added (createGroupRow, createGroupIcons)
 * version 1.2 (03.04.2008) - Episode Rows added (createEpisodeRow, createEpisodeIcons)
 * version 1.3 (15.04.2008) - Page Preferences added
 * version 1.4 (26.05.2008)	- Moved stuff from utils to here
 * version 1.4a(14.07.2008) - Fixed some textual errors
 * version 1.5 (07.12.2008) - AnimePage file table column layout
 * version 1.6 (03.02.2009) - Moved costumize stuff out this to enable more sharing and less code replication
 */
jsVersionArray.push({
	"file":"anime3/common-anime-scripts.js",
	"version":"1.6",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Moved costumize stuff out this to enable more sharing and less code replication"
});

/* These are the default file, episode, group and anime cols */
var genFileCols = [ {'name':"check-mylist",'classname':"check",'header':"S",'abbr':"Check/uncheck files"},
					{'name':"check-anime",'classname':"check",'header':"S",'abbr':"Check/uncheck files"},
					{'name':"fid",'classname':"file id icons",'header':"F",'abbr':"File details/FID",'headclass':"file",'sort':"c_set"},
					{'name':"epno",'classname':"epno",'header':"Epno",'abbr':"Episode number",'sort':"c_set"}, 
					{'name':"group",'classname':"name group",'header':"Group",'headclass':"group",'sort':"c_setlatin"},
					{'name':"size",'classname':"size",'nogenerics':true,'header':"Size",'sort':"c_set"}, 
					{'name':"crc",'classname':"crc",'nogenerics':true,'header':"CRC",'sort':"c_latin"},
					{'name':"langs",'classname':"lang icons",'nogenerics':true,'header':"Lang",'headclass':"lang"}, 
					{'name':"cf",'classname':"codec icons",'nogenerics':true,'header':"CF",'abbr':"Container Format",'headclass':"codec",'sort':"c_setlatin"},
					{'name':"resolution",'classname':"resolution",'nogenerics':true,'header':"Resolution",'sort':"c_set"},
					{'name':"anime-source",'classname':"source",'nogenerics':true,'header':"Source",'sort':"c_latin"},
					{'name':"quality",'classname':"quality",'nogenerics':true,'header':"Quality",'sort':"c_set"}, 
					{'name':"mylist-hashes",'classname':"misc icons",'nogenerics':true,'header':"Misc",'headclass':"hash"},
					{'name':"anime-hashes",'classname':"hash icons ed2k",'nogenerics':true,'header':"Hash",'headclass':"hash"},
					{'name':"users",'classname':"users count",'header':"Users",'headclass':"users",'sort':"c_set"},
					{'name':"mylist-storage",'classname':"storage text",'header':"Storage",'headclass':"storage"}, 
					{'name':"mylist-source",'classname':"source text",'header':"Source",'headclass':"source"},
					{'name':"state-mylist",'classname':"icons state",'header':"State",'headclass':"state"}, 
					{'name':"state-anime",'classname':"icons state",'header':"Mylist",'headclass':"state"}, 
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
					{'name':"cmts",'classname':"threads number",'nogenerics':true,'header':"Cmts",'headclass':"threads",'abbr':"Comments"}, 
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

/* This is an auxiliar function that adds/alters a new definition to a given attribute from the cols
 * @param name Name of the column
 * @param cols Cols var
 * @param defName Definition name
 * @param defValue Definition value
 */
function addColDefinition(name,cols,defName,defValue) {
	var index = -1;
	for (var i = 0; i < cols.length; i++) {
		if (cols[i]['name'] != name) continue;
		index = i;
		break;
	}
	if (index > -1)
		cols[index][defName] = defValue;
}
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

/* This is an auxiliar function that adds a given attribute from the cols to another Array
 * @param name Name of The column to add
 * @param cols Cols var
 * @param array Array which will get the new col
 * @return The given array with the added col
 */
function addColAttribute(name,cols,array) {
	var index = -1;
	for (var i = 0; i < cols.length; i++) {
		if (cols[i]['name'] != name) continue;
		index = i;
		break;
	}
	if (index > -1) array.push(cols[index]);
	return array;
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

/* Dummy function, this should be overwritten by specific functions */
function changeEpWatchedState() {}

/* This function creates and returns an Object with all the possible icons for a given episode
 * @param group Group to build icon object
 * @return an Object with the icons
 * ids: expand, seen, state, fstate
 */
function createEpisodeIcons(episode) {
	var icons = new Object();
	if (mylist_settings && mylist_settings['filemode'] == '1') {
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
			if (mylistEntry.filetype != 'generic') {
				if (isNaN(statusFiles[mylistEntry.status])) statusFiles[mylistEntry.status] = 1;
				else statusFiles[mylistEntry.status]++;
			}
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
	} else if (mylist.length) { // for the case where the episode is related to some file
		for (var fe = 0; fe < episode.files.length; fe++) {
			var fid = episode.files[fe];
			if (!mylist[fid]) continue;
			if (mylist[fid].seenDate) {
				if (!episode.seenDate || mylist[fid].seenDate < episode.seenDate) episode.seenDate = mylist[fid].seenDate;
				icons['seen'] = createIcon(null, 'seen ', null, null, 'seen on: '+cTimeDateHour(episode.seenDate), 'i_seen');
				break;
			}
		}	
	}
	if (episode.seenDate) {
		icons['seen'] = createIcon(null, 'seen ', null, null, 'seen on: '+cTimeDateHour(episode.seenDate), 'i_seen');
		if (mylistEpEntries.length && mylistEpEntries.length == 1) icons['mylist'] = createIcon(null, 'MU', 'removeme', changeEpWatchedState, 'Mark as unwatched', 'i_seen_no');
	} else {
		if (mylistEpEntries.length && mylistEpEntries.length == 1) icons['mylist'] = createIcon(null, 'MW', 'removeme', changeEpWatchedState, 'Mark as watched', 'i_seen_yes');
	}
	if (episode.isRecap) icons['recap'] = createIcon(null, '[recap] ', null, null, 'This episode is a recap (summary).', 'i_recap');
	if (episode.other) icons['comment'] = createIcon(null, '[cmt] ',null, null, 'Comment: '+episode.other, 'i_comment');
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
				var eptitleText = "";
				if (episode.seenDate == 0 && config && config['lay'] && config['lay']['LAY_NOSPOILER']) {
					eptitleText = (anime.type != 'movie' ? 'Episode' : 'Part') + ' ' + episode.epno;
					eptitle = createTextLink(null, eptitleText, eptitleURL, null, null, null, null);
				} else {
					var altTitle = jaTitle = curTitle = '';
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
							eptitle = createTextLink(null, episode.getTitle()+' ('+altTitle+')', eptitleURL, null, null, null, null);
						if (episodeTitleDisplay == 2 || (episodeTitleDisplay == 3 && jaTitle == '')) 
							eptitle = createTextLink(null, episode.getTitle(), eptitleURL, null, null, mapLanguage(episodeAltTitleLang) + ' title: '+ altTitle, null);
						if (episodeTitleDisplay == 3 && jaTitle != '') {
							var titleTag = '';
							if (altTitle != '') titleTag = mapLanguage(episodeAltTitleLang) + ' title: '+ altTitle + ' / ';
							eptitle = createTextLink(null, episode.getTitle(), eptitleURL, null, null, titleTag += jaTitle, null);
						}
						if (episodeTitleDisplay == 4 && jaTitle != '') {
							var titleTag = ' (';
							if (altTitle != '') titleTag += altTitle+' / ';
							eptitle = createTextLink(null, episode.getTitle()+ titleTag +jaTitle+ ')', eptitleURL, null, null, null, null);
						}
					} else eptitle = createTextLink(null, episode.getTitle(), eptitleURL, null, null, null, null);
				}
				if (true) {
					var watchedState = document.createElement('span');
					watchedState.className = 'icons';
					if (icons['state'].length) for (var s=0; s < icons['state'].length; s++) watchedState.appendChild(icons['state'][s]);
					if (icons['fstate'].length) for (var s=0; s < icons['fstate'].length; s++) watchedState.appendChild(icons['fstate'][s]);
					if (icons['recap']) watchedState.appendChild(icons['recap']);
					if (icons['comment']) watchedState.appendChild(icons['comment']);
					if (icons['seen']) watchedState.appendChild(icons['seen']);
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
				createCell(row, col['classname'], (icons['mylist'] ? icons['mylist'] : document.createTextNode(' ')), null, colSpan);
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
	icons['comment'] = createLink(null,'cmt','animedb.pl?show=threads&do=animegroup&cmt=1&id='+group.agid,'anidb::popup',null,'Comment on this release','i_icon i_group_comment 600.500.1.1.agcmts');
	icons['forum'] = createLink(null,'forum','animedb.pl?show=threads&do=animegroup&id='+group.agid,'anidb::popup',null,'View the forum for this release','i_icon i_forum_link 600.500.1.1.agcmts');
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
				createCell(row, col['classname'], createLink(null,clean_input(group.shortName),'animedb.pl?show=group&gid=' + group.id, null, null, clean_input(group.name), null), group.shortName.toLowerCase(), colSpan);
				break;
			case 'state':
				createCell(row, col['classname']+ ' '+group.state, createLink(null,group.state,'animedb.pl?show=group&gid='+group.id+'&aid='+anime.id,null,null,null,null), null, colSpan);
				break;
			case 'note': 
				createCell(row, col['classname'], icons['note'], null, colSpan);
				break;
			case 'eps':
				var maps = {'0' : {'use':true, 'type': 0,'desc':"",'img':"blue"}, 
							'1' : {'use':false,'type': 1,'desc':"Done: "+group.epRange,'img':"darkblue"}, 
							'2' : {'use':false,'type': 2,'desc':"in mylist: "+convertRangeToText(group.isInMylistRange),'img':"lime"}, 
							'3' : {'use':false,'type': 3,'desc':"Done by: ",'img':"lightblue"}};
				var totalEps = (anime.eps ? anime.eps : anime.highestEp);
				var range = expandRange(null, totalEps, maps[0], null);
				if (group.relatedGroups.length) {
					maps[3]['use'] = true;
					for(var i = 0; i < group.relatedGroups.length; i++) {
						var relGroup = groups[group.relatedGroups[i]]; // ID existance check needed? 
						if (!relGroup) continue;
						maps['desc'] += relGroup.Name + ' ';
						range = expandRange(relGroup.epRange, totalEps, maps[3], range);
					}
				}
				if (group.epRange != '') { maps[1]['use'] = true; range = expandRange(group.epRange, totalEps, maps[1], range);}
				if (group.isInMylistRange != '') { maps[2]['use'] = true; range = expandRange(group.isInMylistRange, totalEps, maps[2], range);}
				var cell = createCell(null, col['classname'], makeCompletionBar(null,range,maps),String(group.epCnt),colSpan);
				// @TODO remove this to CSS
				cell.style.minWidth = (screen.width < 1280 ? '205px' : '305px');
				row.appendChild(cell);
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
				var a = createLink(null,(group.rating == '-' ? 'N/A' : group.rating),'animedb.pl?show=animegroupvotes&agid='+group.agid,'anidb::popup',null,null,'350.400.1.1.agvotes');
				a.appendChild(document.createTextNode(' '));
				var cnt = document.createElement('span');
				cnt.className = 'count';
				cnt.appendChild(document.createTextNode('('+group.ratingCount+')'));
				a.appendChild(cnt);
				createCell(row, col['classname'],a,(group.rating == '-' ? '0' : group.rating), colSpan);
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
					cell.appendChild(icons['forum']);
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
			icons['mylist_seen'] = createIcon(null, 'seen ', null, null, 'seen on: '+cTimeDateHour(mylistEntry.seenDate), 'i_seen');
		// mylist comment
		if ((mylistEntry.other) && (mylistEntry.other != '') && (mylistEntry.other != undefined)) 
			icons['mylist_cmt'] = createIcon(null, 'mylist comment ', null, null, 'mylist comment: '+mylistEntry.other, 'i_comment');
		// mylist action
		icons['mylist_remove'] = createIcon(null, 'mylist.remove ', 'animedb.pl?show=mylist&do=del&lid='+mylistEntry.id+'&expand=' + file.animeId + '&showfiles=1&char='+animeFL+'#a'+file.animeId, removeFromMylist, 'remove from mylist', 'i_file_removemylist');
		if (mylistEntry.seen) 
			icons['mylist_watch'] = createIcon(null, 'mylist.unwatch ', 'animedb.pl?show=mylist&do=seen&seen=0&lid='+mylistEntry.id+'&expand='+ file.animeId+'&showfiles=1&char='+animeFL+'#a'+file.animeId, changeWatchedState, 'mark unwatched', 'i_seen_no');
		else 
			icons['mylist_watch'] = createIcon(null, 'mylist.watch ', 'animedb.pl?show=mylist&do=seen&seen=1&lid='+mylistEntry.id+'&expand='+file.animeId+'&showfiles=1&char='+animeFL+'#a'+file.animeId, changeWatchedState, 'mark watched', 'i_seen_yes');
		icons['mylist_edit'] = createIcon(null, 'mylist.edit ', 'animedb.pl?show=mylist&do=add&lid='+mylistEntry.id+'&fid='+mylistEntry.fileId, null, 'edit mylist entry', 'i_file_edit');
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
function createFileRow(eid,fid,cols,skips,rfid) {
	if (!cols) { errorAlert('createFileRow','no cols given'); return; }
	var row = document.createElement('tr');
	var mylistEntry = mylist[fid];
	var file = files[fid];
	if (rfid != null) file = pseudoFiles[rfid];
	var episode = episodes[eid];
	row.id = 'e'+eid+(file.pseudoFile ? 'r' : '')+'f'+fid;
	classNameAtts = new Array();
	if (file.type != 'generic') {
		if (file.crcStatus == 'valid' && !file.isDeprecated) classNameAtts.push('good');
		if (file.crcStatus == 'invalid') classNameAtts.push('invalid');
		if (!file.avdumped) classNameAtts.push('undumped');
		else classNameAtts.push('verified');
		if (file.pseudoFile || file.isVirtual(eid)) classNameAtts.push('virtual');
		if (file.isDeprecated) classNameAtts.push('deprecated');
	} else classNameAtts.push('generic no_sort');
	row.className = classNameAtts.join(' ');
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
				var ck = ((uid != ruid) ? document.createTextNode(' ') : createCheckBox(null,'mylmod.f.'+mylistEntry.id,'mylmod.f.'+mylistEntry.id,null,false));
				createCell(row, col['classname'], ck, null, colSpan);
				break;
			case 'check-anime':
				createCell(row, col['classname'], createCheckBox(null,'madd.f.'+file.id,'madd.f.'+file.id,showAddToMylistBox,false), null, colSpan);
				break;
			case 'fid':
				if (!icons['expand'])
					createCell(row, col['classname'], icons['fid'], file.id, colSpan);
				else {
					var cell = createCell(null, col['classname'], icons['fid'], file.id, colSpan);
					// now do some checking before posting the expand icon
					var showExpandIcon = true;
					if (mylist_settings) {
						var mycnt = 0;
						for (var fr = 0; fr < file.relatedFiles.length; fr++) {
							if (!files[file.relatedFiles[fr]]) continue;
							if (mylist[file.relatedFiles[fr]]) mycnt++;
						}
						if (!mycnt) showExpandIcon = false;
					}
					if (showExpandIcon) cell.appendChild(icons['expand']);
					row.appendChild(cell);
				}
				break;
			case 'epno':
				var epno = createTextLink(null, episode.typeChar+episode.epno, 'animedb.pl?show=ep&eid='+eid, null, null, null, null);
				createCell(row, col['classname'], epno, mapEpisodeNumber(episode.typeChar+episode.epno), colSpan);
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
							label.appendChild(createLink(null, clean_input(groupEntry.shortName), 'animedb.pl?show=group&gid=' + file.groupId, 'extern anidb', null, 'group name: ' + clean_input(groupEntry.name), null));
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
				createCell(row, col['classname'], document.createTextNode((mylistEntry.storage ? mylistEntry.storage : "")), null, colSpan);
				break;
			case 'mylist-source':
				var text = (mylistEntry.source ? mylistEntry.source : "");
				if (IRC_SHOWFILESOURCE) text = file.source;
				createCell(row, col['classname'], document.createTextNode(text), null, colSpan);
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
				if (uid == ruid) {
					if (icons['mylist_watch']) cell.appendChild(icons['mylist_watch']);
					if (icons['mylist_edit']) cell.appendChild(icons['mylist_edit']);
					if (icons['mylist_remove']) cell.appendChild(icons['mylist_remove']);
				} else {
					if (icons['mylist_add']) cell.appendChild(icons['mylist_add']);
				}
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
			case 'anime-hashes':
				cell = createCell(null, col['classname'], null, null, colSpan);
				if (uid != undefined && file.ed2k != '') cell.onmouseover = createHashLink;
				if (icons['ed2k']) cell.appendChild(icons['ed2k']);
				if (icons['crc']) cell.appendChild(icons['crc']);
				if (icons['avdump']) cell.appendChild(icons['avdump']);
				row.appendChild(cell);
				break;
			case 'mylist-hashes':
				cell = createCell(null, col['classname'], null, null, colSpan);
				if (uid != undefined && file.ed2k != '') cell.onmouseover = createHashLink;
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
	row.className = 'header';
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
