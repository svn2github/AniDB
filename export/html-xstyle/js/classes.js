/* file Classes interface
 * author fahrenheit (alka.setzer@gmail.com)
 *        Based on code from PetriW's work at anidb
 * version 2.3 (07.08.2008)
 */

var anime = new Object(); //stub

/* Creates a new Mylist Entry from a given node
 * @param node Node from which to get the mylist information
 * @return mylistEntry
 */
function CMylistEntry(node) {
	this.id = Number(node.getAttribute('id'));
	this.fileId = Number(node.getAttribute('fid'));
	this.episodeId = Number(node.getAttribute('eid'));
	this.groupId = Number(node.getAttribute('gid'));
	this.filetype = node.getAttribute('type');
	this.fstate = 'unknown';
	this.status = 'unknown';
	this.seen = 0;
	this.seenDate = null;
	this.addDate = '-';
	this.source = null;
	this.storage = null;
	this.other = null;
	this.fType = null;
	this.relatedEids = new Array();
	for (var i = 0; i < node.childNodes.length; i++) {
		var sNode = node.childNodes.item(i);
		if (sNode.nodeType == 3) continue; // Text node, not interested
		switch (sNode.nodeName.toLowerCase()) {
			case 'date': 
				this.seenDate = sNode.getAttribute('viewed');
				this.seen = (this.seenDate != '-') ? true : false;
				if (this.seen && episodes[this.episodeId]) episodes[this.episodeId].seenDate = this.seenDate;
				this.addDate = nodeData(sNode);
				break;
			case 'storage': this.storage = nodeData(sNode); break;
			case 'source': this.source = nodeData(sNode); break;
			case 'mystate': this.status = nodeData(sNode); break;
			case 'myfilestate': this.fstate = nodeData(sNode); break;
			case 'other': this.other = nodeData(sNode); break;
			case 'releids': this.relatedEids = nodeData(sNode).split(','); break;
			default: showAlert('mylistEntry for lid: '+this.id, node.nodeName, node.nodeName,sNode.nodeName);
		}
	}
	this.fType = mapFState(this.fstate);
}

/* Creates a new UserInfo Entry from a given node
 * @param node Node from which to get the userinfo
 * @return userinfoEntry
 */
function CUserinfoEntry(node) {
	this.id = uid;
	this.animes = new Object();
	this.eps = new Object();
	this.files = new Object();
	this.groups = new Object();
	this.size = new Object();
	for (var i = 0; i < node.childNodes.length; i++) {
		var snode = node.childNodes.item(i);
		if (snode.nodeType == 3) continue; // Text node, not interested
		switch (snode.nodeName) {
			case 'name': this.name = nodeData(snode); break;
			case 'animes':
				this.animes['added'] = Number(snode.getAttribute('added'));
				this.animes['user'] = Number(snode.getAttribute('user'));
				break;
			case 'eps':
				this.eps['added'] = Number(snode.getAttribute('added'));
				this.eps['dbviewedp'] = snode.getAttribute('dbviewedp');
				this.eps['dbownedp'] = snode.getAttribute('dbownedp');
				this.eps['viewedp'] = snode.getAttribute('viewedp');
				this.eps['viewed'] = Number(snode.getAttribute('viewed'));
				this.eps['user'] = Number(snode.getAttribute('user'));
				break;
			case 'files':
				this.files['added'] = Number(snode.getAttribute('added'));
				this.files['lamecnt'] = Number(snode.getAttribute('lamecnt'));
				this.files['user'] = Number(snode.getAttribute('user'));
				break;
			case 'groups':
				this.groups['added'] = Number(snode.getAttribute('added'));
				break;
			case 'size':
				this.size['bytes'] = snode.getAttribute('longn');
				this.size['short'] = snode.getAttribute('shortn');
				break;
			case 'stats':
				this.votes = Number(snode.getAttribute('votes'));
				this.reviews = Number(snode.getAttribute('reviews'));
				break;
			case 'date':
				this.date = convertTime(nodeData(snode));
				break;
			default: showAlert('userinfoEntry for uid: '+uid, node.nodeName, node.nodeName,snode.nodeName);
		}
	}
}

/* Creates a new Group Entry from a given node
 * @param node Node from which to get the group information
 * @return groupEntry
 */
function CGroupEntry(node) {
	hiddenGroups++;
	this.visible = true;
	this.defaultVisible = false;
	this.filtered = false;
	this.langFiltered = false;
	this.id = Number(node.getAttribute('id'));
	this.agid = Number(node.getAttribute('agid'));
	this.rating = '-';
	this.ratingCount = 0;
	this.commentCount = 0;
	this.userRating = -1; // set later
	//this.relatedGroups = new Array();
	this.sepCnt = 0;
	this.epCnt = 0;
	this.isInMylistRange = '';
	this.epRange = '';
	this.audioLangs = new Array();
	this.subtitleLangs = new Array();
	this.state = 'unknown';
	this.stateId = 0;
	this.hasCherryBeenPoped = false; // just to know if we have the files related to this group on list
	for (var i = 0; i < node.childNodes.length; i++) {
		var sNode = node.childNodes.item(i);
		if (sNode.nodeType == 3) continue; // Text node, not interested
		switch (sNode.nodeName.toLowerCase()) {
			case 'name': this.name = nodeData(sNode); break;
			case 'sname': this.shortName = nodeData(sNode); break;
			case 'state': this.state = nodeData(sNode); this.stateId = Number(sNode.getAttribute('id')); break;
			case 'lastep': this.lastEp = epNoToString(nodeData(sNode)); break;
			case 'lastup': this.lastUp = convertTime(nodeData(sNode)); break;
			case 'rating': this.rating = nodeData(sNode); this.ratingCount = Number(sNode.getAttribute('cnt')); break;
			case 'cmts': this.commentCount = Number(sNode.getAttribute('cnt')); break;
			case 'epcnt': this.epCnt = Number(nodeData(sNode)); break;
			case 'sepcnt': this.sepCnt = Number(nodeData(sNode)); break;
			case 'eprange': this.epRange = nodeData(sNode); break;
			case 'aud': var langs = sNode.getElementsByTagName('lang');
				for (var k = 0; k < langs.length; k++) this.audioLangs.push(nodeData(langs[k]));
				break;
			case 'sub': var langs = sNode.getElementsByTagName('lang');
				for (var k = 0; k < langs.length; k++) this.subtitleLangs.push(nodeData(langs[k]));
				break;
			default: showAlert('groupEntry for gid: '+this.id, node.nodeName, node.nodeName,sNode.nodeName);
		}
	}
}

/* Creates a new Anime Entry from a given node
 * @param node Node from which to get the anime information
 * @return animeEntry
 */
function CAnimeEntry(node) {
	this.id = Number(node.getAttribute('id'));
	this.type = node.getAttribute('type');
	this.year = node.getAttribute('year');
	this.neps = new Object();
	this.seps = new Object();
	this.title = "Anime "+this.id;
	this.titles = new Object();
	this.tags = new Object();
	this.rating = new Object();
	this.state = 'unknown';
	this.filtered = false;
	this.info = new Object();
	this.eps = new Array();
	this.groups = new Array();
	this.temp; // temp var
	// info stuff
	this.other = '';
	this.resources = new Object();
	this.genres = new Array();
	this.cats = new Array();
	this.companys = new Array();
	this.awards = new Array();
	this.episodes = new Array();
	for (i = 0; i < node.childNodes.length; i++) {
		var sNode = node.childNodes.item(i);
		if (sNode.nodeType == 3) continue; // Text node, not interested
		switch (sNode.nodeName) {
			case 'status':
				this.complete = Number(sNode.getAttribute('complete'));
				this.watched = Number(sNode.getAttribute('watched'));
				this.hasawards = Number(sNode.getAttribute('hasawards'));
				this.restricted = Number(sNode.getAttribute('restricted'));
				break;
			case 'neps': 
				this.neps['cnt'] = Number(sNode.getAttribute('cnt'));
				this.neps['user'] = Number(sNode.getAttribute('user'));
				this.neps['seen'] = Number(sNode.getAttribute('seen'));
				break;
			case 'seps':
				this.seps['cnt'] = Number(sNode.getAttribute('cnt'));
				this.seps['user'] = Number(sNode.getAttribute('user'));
				this.seps['seen'] = Number(sNode.getAttribute('seen'));
				break;
			case 'titles':
				for (var k = 0; k < sNode.childNodes.length; k++) {
					var tNode = sNode.childNodes.item(k);
					if (tNode.nodeType == 3) continue; // Text node, not interested
					var ttype = tNode.getAttribute('type');
					var tlang = tNode.getAttribute('lang');
					if (ttype == 'main') this.title = nodeData(tNode);
					if (!this.titles[ttype]) this.titles[ttype] = new Object();
					this.titles[ttype][tlang] = nodeData(tNode);
				}
				break;
			case 'tags':
				for (var k = 0; k < sNode.childNodes.length; k++) {
					var tNode = sNode.childNodes.item(k);
						if (tNode.nodeType == 3) continue; // Text node, not interested
						var tid = tNode.getAttribute('id');
						this.tags[tid] = new Object();
						this.tags[tid]['date'] = tNode.getAttribute('date')
						this.tags[tid]['desc'] = nodeData(tNode);
					}
				break;
			case 'state': this.state = nodeData(sNode); break;
			case 'size': this.sizeBytes = sNode.getAttribute('longn'); this.size = sNode.getAttribute('cnt'); break;
			case 'rating': this.rating['type'] = sNode.getAttribute('type'); this.rating['votes'] = sNode.getAttribute('votes'); this.rating['rating'] = sNode.getAttribute('rating'); break;
			case 'reviews': this.reviews = sNode.getAttribute('cnt'); this.rrating = sNode.getAttribute('rating'); break;
			case 'wishlist': this.wishlist = {'type':sNode.getAttribute('type'),'pri':sNode.getAttribute('pri'),'comment':nodeData(sNode)}; break;
			case 'myvote': this.myvote = {'type':sNode.getAttribute('type'),'date':sNode.getAttribute('date'),'vote':sNode.getAttribute('vote')}; break;
			case 'dates': this.dates = {'added':sNode.getAttribute('added'),'update':sNode.getAttribute('update'),'start':sNode.getAttribute('start'),'end':sNode.getAttribute('end')}; break;
			case 'info': // very long anime info parser
				for (var k = 0; k < sNode.childNodes.length; k++) {
					var tNode = sNode.childNodes.item(k);
					if (tNode.nodeType == 3) continue; // Text node, not interested
					switch (tNode.nodeName) {
						case 'desc': this.other = nodeData(tNode); break;
						case 'resources':
							for (var r = 0; r < tNode.childNodes.length; r++) {
								var rNode = tNode.childNodes.item(r);
								if (rNode.nodeType == 3) continue; // Text node, not interested
								this.resources[rNode.nodeName] = new Object();
								if (rNode.getAttribute('id')) this.resources[rNode.nodeName]['id'] = rNode.getAttribute('id');
								this.resources[rNode.nodeName]['link'] = nodeData(rNode);
							}
							break;
						case 'genres':
							for (var r = 0; r < tNode.childNodes.length; r++) {
								var rNode = tNode.childNodes.item(r);
								if (rNode.nodeType == 3) continue; // Text node, not interested
								var genre = new Object();
								genre['id'] = rNode.getAttribute('id');
								genre['name'] = nodeData(rNode);
								this.genres.push(genre);
							}
							break;
						case 'cats':
							for (var r = 0; r < tNode.childNodes.length; r++) {
								var rNode = tNode.childNodes.item(r);
								if (rNode.nodeType == 3) continue; // Text node, not interested
								var cat = new Object();
								cat['id'] = rNode.getAttribute('id');
								cat['pid'] = rNode.getAttribute('pid');
								cat['restricted'] = rNode.getAttribute('restricted');
								cat['name'] = nodeData(rNode);
								this.cats.push(cat);
							}
							break;
						case 'companys':
							for (var r = 0; r < tNode.childNodes.length; r++) {
								var rNode = tNode.childNodes.item(r);
								if (rNode.nodeType == 3) continue; // Text node, not interested
								var id = rNode.getAttribute('id');
								var company = new Object();
								company['id'] = id;
								company['type'] = rNode.getAttribute('atype');
								this.companys.push(company);
								if (!companys[id]) {
									companys[id] = new Object();
									companys[id]['type'] = rNode.getAttribute('type');
									for (var c = 0; c < rNode.childNodes.length; c++) {
										var cNode = rNode.childNodes.item(c);
										if (cNode.nodeType == 3) continue; // Text node, not interested
										companys[id][cNode.nodeName] = nodeData(cNode);
									}
								}
							}
							break;
						case 'awards_type':
							for (var r = 0; r < tNode.childNodes.length; r++) {
								var rNode = tNode.childNodes.item(r);
								if (rNode.nodeType == 3) continue; // Text node, not interested
								var id = rNode.getAttribute('id');
								if (awardtypes[id]) continue;
								awardtypes[id] = nodeData(rNode);
							}
							break;
						case 'awards':
							for (var r = 0; r < tNode.childNodes.length; r++) {
								var rNode = tNode.childNodes.item(r);
								if (rNode.nodeType == 3) continue; // Text node, not interested
								var award = new Object();
								award['id'] = rNode.getAttribute('id');
								award['type'] = rNode.getAttribute('type');
								award['url'] = rNode.getAttribute('url');
								award['award'] = nodeData(rNode);
								this.awards.push(award);
							}
							break;
						default: showAlert('infoEntry for aid: '+this.id, sNode.nodeName, sNode.nodeName,tNode.nodeName);
					}
				}
				break;
			case 'eps':
			case 'groups': break; // handled else where
			default: showAlert('animeEntry for aid: '+this.id, node.nodeName, node.nodeName,sNode.nodeName);
		}
	}
}

CAnimeEntry.prototype.getOfficialTitle = function() {
	var title = null;
	for (var type in this.titles){ // loop title types and see if we have a match
		if (type != 'main' || type != 'official') continue;
		if (this.titles[type][animeTitleLang]) { title = this.titles[type][animeTitleLang]; break; }
	}
	if (!title) // just return the main title
	for (var lang in this.titles['main']) { title = this.titles['main'][lang]; break; }
	if (!title) title = 'unknown';
	return (title);
}

CAnimeEntry.prototype.getTitle = function() {
	var title = null;
	for (var type in this.titles){ // loop title types and see if we have a match
		if (this.titles[type][animeTitleLang]) { title = this.titles[type][animeTitleLang]; break; }
	}
	if (!title) // just return the main title
	for (var lang in this.titles['main']) { title = this.titles['main'][lang]; break; }
	if (!title) title = 'unknown';
	return (title);
}

CAnimeEntry.prototype.getAltTitle = function() {
	var title = null;
	if (this.titles['main'][animeAltTitleLang]) return(this.titles['main'][animeAltTitleLang]);
	if (this.titles['official'][animeAltTitleLang]) return(this.titles['official'][animeAltTitleLang]);
	if (this.titles['official']['x-jat']) return(this.titles['official']['x-jat']);
	if (this.titles['official']['en']) return(this.titles['official']['en']);
	for (var i in this.titles['official']) return(this.titles['official'][i]);
}

/* Creates a new Episode Entry from a given node
 * @param node Node from which to get the episode information
 * @return episodeEntry
 */
function CEpisodeEntry(node) {
	this.id = Number(node.getAttribute('id'));
	this.epno = '';
	this.type = 'normal';
	this.typeFull = 'Normal Episode';
	this.typeChar = '';
	this.vote = -1; // set later
	this.isRecap = false;
	this.animeId = -1;
	this.hiddenFiles = 0;
	this.seenDate = 0;
	this.length = 0;
	this.playLength = 0;
	this.relDate = 0;
	this.userCount = 0;
	this.fileCount = 0;
	this.other = '';
	this.rating = '-';
	this.ratingCount = 0;
	this.newFiles = false;
	this.titles = new Array();
	this.files = new Array(); // File IDS of related files are stored in the files array
	this.pseudoFiles = new Array(); // pseudo files are a totaly new thing
	this.update = (node.getAttribute('update') ? Number(node.getAttribute('update')) : 0);
	for (var i = 0; i < node.childNodes.length; i++) {
		var sNode = node.childNodes.item(i);
		if (sNode.nodeType == 3) continue; // Text node, not interested
		switch (sNode.nodeName.toLowerCase()) {
			case 'flags': this.flags = Number(nodeData(sNode)); break;
			case 'epno': this.epno = nodeData(sNode); break;
			case 'len': this.length = Number(nodeData(sNode)); break;
			case 'date': this.addDate = nodeData(sNode); this.relDate = sNode.getAttribute('rel') || 0; break;
			case 'ucnt': this.userCount = Number(nodeData(sNode)); break;
			case 'fcnt': this.fileCount = Number(nodeData(sNode)); break;
			case 'other': this.other = nodeData(sNode); break;
			case 'rating': this.rating = nodeData(sNode); this.ratingCount = Number(sNode.getAttribute('cnt')); break;
			case 'titles':
				for (var k = 0; k < sNode.childNodes.length; k++) {
					var tNode = sNode.childNodes.item(k);
					if (tNode.nodeType == 3) continue; // Text node, not interested
					this.titles[tNode.getAttribute('lang')] = new Object();
					this.titles[tNode.getAttribute('lang')]['title'] = nodeData(tNode);
					this.titles[tNode.getAttribute('lang')]['update'] = (tNode.getAttribute('update')) ? tNode.getAttribute('update') : 0;
					this.titles[tNode.getAttribute('lang')]['verify'] = (tNode.getAttribute('verify')) ? tNode.getAttribute('verify') : 0;
				}
				break;
			case 'files': break;
			default: showAlert('episodeEntry for gid: '+this.id, node.nodeName, node.nodeName,sNode.nodeName);
		}
	}
	if (this.relDate == 0 || this.relDate == '') this.date = this.addDate;
	else this.date = this.relDate;
	if (this.flags & 1) { this.type = 'special'; this.typeFull = 'Special Episode'; this.typeChar = 'S'; }
	if (this.flags & 2) { this.isRecap = true; this.typeFull += ', Recap'; }
	if (this.flags & 4) { this.type = 'opening'; this.typeFull = 'Opening/Ending/Credits'; this.typeChar = 'C'; }
	if (this.flags & 32) { this.type = 'trailer'; this.typeFull = 'Trailer/Promo/Ads'; this.typeChar = 'T'; }
	if (this.flags & 64) { this.type = 'parody'; this.typeFull = 'Parody/Fandub'; this.typeChar = 'P'; }
	if (this.flags & 128) { this.type = 'other'; this.typeFull = 'Other Episodes'; this.typeChar = 'O'; }
	this.playLength = this.length;
	// Format length
	var h, m;
	h = Math.floor(this.length / 60);
	m = this.length % 60;
	if (h > 0) {
		if (m > 0) this.length = h + 'h ' + m + 'm';
		else this.length = h + 'h';
	} else this.length = m + 'm';
}

CEpisodeEntry.prototype.getTitle = function() {
	var title = null;
	if (this.titles[episodeTitleLang]) title = this.titles[episodeTitleLang]['title'];
	if (!title && episodeTitleLang != 'en' && this.titles['en']) title = this.titles['en']['title']; 
	if (!title && episodeTitleLang != 'x-jat' && this.titles['x-jat']) title = this.titles['x-jat']['title'];
	if (!title && episodeTitleLang != 'ja' && this.titles['ja']) title = this.titles['ja']['title']; 
	if (!title) { for (var i in this.titles) { title = this.titles[i]['title']; break; } }
	if (!title) title = 'Episode '+this.typeChar+this.epno;
	return (title);
}

CEpisodeEntry.prototype.getAltTitle = function() {
	var title = this.titles[episodeAltTitleLang]; // first option
	if (!title && episodeAltTitleLang != 'en' && this.titles['en']) title = this.titles['en']['title']; 
	if (!title && episodeAltTitleLang != 'x-jat' && this.titles['x-jat']) title = this.titles['x-jat']['title'];
	if (!title && episodeAltTitleLang != 'ja' && this.titles['ja']) title = this.titles['ja']['title']; 
	if (!title) { for (var i in this.titles) { title = this.titles[i]['title']; break; } }
	if (!title) title = 'Episode '+this.typeChar+this.epno;
	return (title);
}

CEpisodeEntry.prototype.getTitles = function() {
	var ret = new Array();
	for (var tid in this.titles) ret.push(tid + ': ' +this.titles[tid]['title']);
	return (ret.join(', '));
}

/* *
 * Creates a new File Entry from a given node
 * @param node Node from which to get the file information
 * @return fileEntry
 */
function CFileEntry(node) {
	this.id = Number(node.getAttribute('id'));
	this.animeId = -1; // Assigned by the caller
	this.episodeId = -1; // Properly assigned by the caller
	this.type = node.getAttribute('type');
	this.epRelations = new Array(); // holds file<->ep relations for a file
	this.fileRelations = new Array(); // holds file<->file relations for a file
	this.relatedFiles = new Array(); // if (this.length) this is a pseudoFile
	this.relatedPS = new Array(); // used to store related pseudoFiles ids
	this.relatedGroups = new Array(); // used to hold related group information
	//this.relatedEpisodes = new Array(); // used only if external file
	// defaults
	this.flags = 0;
	this.visible = true; // should the file be displayed (default: yes)
	this.md5 = '';
	this.sha1 = '';
	this.tth = '';
	this.crc32 = '';
	this.crcStatus = '';
	this.isDeprecated = false;
	this.ed2k = '';
	this.ed2klink = '';
	this.size = 0;
	this.date = 0;
	this.relDate = 0;
	this.length = 0;
	this.fileType = '';
	this.groupId = 0;
	//this.groupName = ''; // only for pseudo files
	//this.groupShortName = '';  // only for pseudo files
	this.version = 'v1';
	this.isCensored = 0;
	this.isUncensored = 0;
	this.quality = 'unknown';
	this.resolution = 'unknown';
	this.pixelarea = 0;
	this.source = 'unknown';
	this.other = '';
	this.usersTotal = 0;
	this.usersUnknown = 0;
	this.usersDeleted = 0;
	this.vidCnt = 0;
	this.audCnt = 0;
	this.subCnt = 0;
	this.avdumped = 0;
	this.newFile = false;
	this.pseudoFile = false; // Got fed up with strange methods for checking if a file isn't pseudo
	this.videoTracks = new Array();
	this.audioTracks = new Array();
	this.subtitleTracks = new Array();
	this.isRaw = false;
	this.mylist = new Object(); // substitute for mylist entries
	// Actualy fill the data;
	for (var i = 0; i < node.childNodes.length; i++) {
		var sNode1 = node.childNodes.item(i);
		switch (sNode1.nodeName.toLowerCase()) {
			case 'size': this.size = Number(nodeData(sNode1)); break;
			case 'ed2k': this.ed2k = nodeData(sNode1); break;
			case 'md5': this.md5 = nodeData(sNode1); break;
			case 'sha1': this.sha1 = nodeData(sNode1); break;
			case 'tth': this.tth = nodeData(sNode1); break;
			case 'avdumped': this.avdumped = Number(nodeData(sNode1)); break;
			case 'crc': this.crc32 = nodeData(sNode1); break;
			case 'len': this.length = Number(nodeData(sNode1)); break;
			case 'ftype': this.fileType = nodeData(sNode1); break;
			case 'group': this.groupId = Number(sNode1.getAttribute('id')); this.relatedGroups.push(this.groupId); break; 
			case 'flags': this.flags = Number(nodeData(sNode1)); break;
			case 'date': 
				this.date = convertTime(nodeData(sNode1));
				if (Number(new Date()/1000 - javascriptDate(this.date)/1000) < 86400) this.newFile = true;
				this.relDate = convertTime(sNode1.getAttribute('rel'));
				this.update = convertTime(sNode1.getAttribute('update'));
				break;
			case 'mylist':
				var mylistEntry = new CMylistEntry(sNode1);
				mylist[this.id] = mylistEntry;
				this.mylist = mylistEntry;
				break;
			case 'vid':
				this.vidCnt = Number(sNode1.getAttribute('cnt')) || 0;
				for (var j = 0; j < sNode1.childNodes.length; j++) {
					var dNode = sNode1.childNodes.item(j);
					if (dNode.nodeType == 3) continue;
					switch (dNode.nodeName.toLowerCase()) {
						case 'stream':
							var stream = new Object;
							stream.resW = 0;
							stream.resH = 0;
							stream.ar = 'unknown';
							stream.codec = 'unknown';
							stream.codec_sname = 'unk';
							for (var k = 0; k < dNode.childNodes.length; k++) {
								var stNode = dNode.childNodes.item(k);
								if (stNode.nodeType == 3) continue;
								switch (stNode.nodeName.toLowerCase()) {
									case 'res': 
										stream.resW = Number(stNode.getAttribute('w')) || 0; 
										stream.resH = Number(stNode.getAttribute('h')) || 0;                     
										if (stream.resW && stream.resH) {
											this.resolution = stream.resW + 'x' + stream.resH;
											this.pixelarea = Number(stream.resW)*Number(stream.resH);
										}
									break;
									case 'ar': stream.ar = nodeData(stNode); break;
									case 'codec': stream.codec_sname = stNode.getAttribute('sname'); stream.codec = nodeData(stNode); break;
									default: showAlert('fileEntry for fid: '+this.id+' (type: '+this.type+')', 'videoStream['+k+']', dNode.nodeName,stNode.nodeName);
								}
							} 
							this.videoTracks.push(stream);
							break;
						default: showAlert('fileEntry for fid: '+this.id+' (type: '+this.type+')', 'videoStreams', dNode.nodeName,stNode.nodeName);
					}
				}
				break;
			case 'aud':
				this.audCnt = Number(sNode1.getAttribute('cnt')) || 0;
				for (var j = 0; j < sNode1.childNodes.length; j++) {
					var dNode = sNode1.childNodes.item(j);
					if (dNode.nodeType == 3) continue;
					switch (dNode.nodeName.toLowerCase()) {
						case 'stream':
							var stream = new Object;
							stream.chan = 'unknown';
							stream.codec = 'unknown';
							stream.codec_sname = 'unknown';
							stream.type = 'normal';
							for (var k = 0; k < dNode.childNodes.length; k++) {
								var stNode = dNode.childNodes.item(k);
								if (stNode.nodeType == 3) continue;
								switch (stNode.nodeName.toLowerCase()) {
									case 'chan': stream.chan = nodeData(stNode); break;
									case 'lang': stream.lang = nodeData(stNode); break;
									case 'codec': stream.codec_sname = stNode.getAttribute('sname'); stream.codec = nodeData(stNode); break;
									case 'type': stream.type = nodeData(stNode); break;
									default: showAlert('fileEntry for fid: '+this.id+' (type: '+this.type+')', 'audioStream['+k+']', dNode.nodeName,stNode.nodeName);
								}
							}
							this.audioTracks.push(stream);
							break;
						default:  showAlert('fileEntry for fid: '+this.id+' (type: '+this.type+')', 'audioStreams', dNode.nodeName,stNode.nodeName);
					}
				}
				break;
			case 'sub':
				this.subCnt = Number(sNode1.getAttribute('cnt')) || 0;
				for (var j = 0; j < sNode1.childNodes.length; j++) {
					var dNode = sNode1.childNodes.item(j);
					if (dNode.nodeType == 3) continue;
					switch (dNode.nodeName.toLowerCase()) {
						case 'stream':
							var stream = new Object;
							stream.type = 'unknown';
							stream.flags = 0;
							for (var k = 0; k < dNode.childNodes.length; k++) {
								var stNode = dNode.childNodes.item(k);
								if (stNode.nodeType == 3) continue;
								switch (stNode.nodeName) {
									case 'lang': stream.lang = nodeData(stNode); break;
									case 'type': stream.type = nodeData(stNode); break;
									case 'flags': stream.flags = Number(nodeData(stNode)); break;
									default: showAlert('fileEntry for fid: '+this.id+' (type: '+this.type+')', 'subtitleStream['+k+']', dNode.nodeName,stNode.nodeName);
								}
							}
							this.subtitleTracks.push(stream);
							break;
						default: showAlert('fileEntry for fid: '+this.id+' (type: '+this.type+')', 'subtitleStreams', sNode1.nodeName,dNode.nodeName);
					}
				}
				break;
			case 'qual': this.quality = nodeData(sNode1); break;
			case 'source': this.source = nodeData(sNode1); break;
			case 'other': this.other = nodeData(sNode1); break;
			case 'users':
				for (var j = 0; j < sNode1.childNodes.length; j++) {
					var sNode2 = sNode1.childNodes.item(j);
					switch (sNode2.nodeName.toLowerCase()) {
						case 'all': this.usersTotal = Number(nodeData(sNode2)); break;
						case 'ukn': this.usersUnknown = Number(nodeData(sNode2)); break;
						case 'del': this.usersDeleted = Number(nodeData(sNode2)); break;
					}
				}
				break;
			default: showAlert('fileEntry for fid: '+this.id, node.nodeName, node.nodeName,sNode1.nodeName);
		}
	}
	// do some clean up
	if (this.relDate == '') this.relDate = 0;
	if (this.flags & 1) { this.crcStatus = 'valid'; }
	if (this.flags & 2) { this.crcStatus = 'invalid'; this.isDeprecated = true; }
	if (this.flags & 4) { this.version = 'v2'; }
	if (this.flags & 8) { this.version = 'v3'; }
	if (this.flags & 16) { this.version = 'v4'; }
	if (this.flags & 32) { this.version = 'v5'; }
	if (this.flags & 64) { this.isUncensored = 1; }
	if (this.flags & 128) { this.isCensored = 1; }
	if (this.vidCnt && !this.subCnt) { this.isRaw = true; }
}
