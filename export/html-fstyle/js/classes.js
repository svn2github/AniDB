/* *
 * @file Classes interface
 * @author fahrenheit (alka.setzer@gmail.com)
 *         Based on code from PetriW's work at anidb
 * @version 2.1 (22.03.2007)
 */
 
/* *
 * Creates a new Mylist Entry from a given node
 * @param node Node from which to get the mylist information
 * @return mylistEntry
 */
function CMylistEntry(node) {
  this.id = Number(node.getAttribute('id'));
  this.fileId = Number(node.getAttribute('fid'));
  this.episodeId = Number(node.getAttribute('eid'));
  this.groupId = Number(node.getAttribute('gid'));
  this.fstate = 'unknown';
  this.status = 'unknown';
  this.seen = 0;
  this.seenDate = null;
  this.source = null;
  this.storage = null;
  this.other = null;
  this.fType = null;
  for (var i = 0; i < node.childNodes.length; i++) {
    var sNode = node.childNodes.item(i);
    if (sNode.nodeType == 3) continue; // Text node, not interested
    switch (sNode.nodeName) {
      case 'state': this.status = nodeData(sNode); break;
      case 'fstate': this.fstate = nodeData(sNode); break;
      case 'seen': this.seen = Number(nodeData(sNode)); this.seenDate = convertTime(sNode.getAttribute('date')); break;
      case 'source': this.source = nodeData(sNode); break;
      case 'storage': this.storage = nodeData(sNode); break;
      case 'other': this.other = nodeData(sNode); break;
      default: showAlert('mylistEntry for lid: '+this.id, node.nodeName, node.nodeName,sNode.nodeName);
    }
  }
  this.fType = mapFState(this.fstate);
}

/* *
 * Creates a new Group Entry from a given node
 * @param node Node from which to get the group information
 * @return groupEntry
 */
function CGroupEntry(node) {
  hiddenGroups++;
  this.visible = true;
  this.defaultVisible = false;
  this.filtered = false;
  this.id = Number(node.getAttribute('id'));
  this.agid = Number(node.getAttribute('agid'));
  this.rating = '-';
  this.ratingCount = 0;
  this.commentCount = 0;
  this.userRating = -1; // set later
  //this.relatedGroups = new Array();
  this.sepCnt = 0;
  this.epCnt = 0;
  this.isInMylist = false;
  this.epsInMyListArray = new Array();
  this.epsInMyListStringGuess = '';
  this.epsInMyListString = '';
  this.audioLangs = new Array();
  this.subtitleLangs = new Array();
  this.state = 'unknown';
  this.stateId = 0;
  this.hasCherryBeenPoped = false; // just to know if we have the files related to this group on list
  for (var i = 0; i < node.childNodes.length; i++) {
    var sNode = node.childNodes.item(i);
    if (sNode.nodeType == 3) continue; // Text node, not interested
    switch (sNode.nodeName) {
      case 'name': this.name = nodeData(sNode); break;
      case 'sname': this.shortName = nodeData(sNode); break;
      case 'state': this.state = nodeData(sNode); this.stateId = Number(sNode.getAttribute('id')); break;
      case 'lastep': this.lastEp = epNoToString(nodeData(sNode)); break;
      case 'lastup': this.lastUp = convertTime(nodeData(sNode)); break;
      case 'rating': this.rating = nodeData(sNode); this.ratingCount = Number(sNode.getAttribute('cnt')); break;
      case 'cmts': this.commentCount = Number(sNode.getAttribute('cnt')); break;
      case 'epcnt': this.epCnt = Number(nodeData(sNode)); break;
      case 'sepcnt': this.sepCnt = Number(nodeData(sNode)); break;
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

/* *
 * Creates a new Anime Entry from a given node
 * @param node Node from which to get the anime information
 * @return animeEntry
 */
function CAnimeEntry(node) {
  this.id = Number(node.getAttribute('id'));
  this.type = node.getAttribute('type');
  this.titles = new Array();
  for (i = 0; i < node.childNodes.length; i++) {
    var sNode = node.childNodes.item(i);
    if (sNode.nodeType == 3) continue; // Text node, not interested
    switch (sNode.nodeName) {
      case 'neps': this.eps = Number(nodeData(sNode)); break;
      case 'epcnt': this.epCount = Number(nodeData(sNode)); break;
      case 'fcnt': this.fileCount = Number(nodeData(sNode)); break;
      case 'gcnt': this.groupCount = Number(nodeData(sNode)); break;
      case 'eps': 
      case 'groups': break; // Will be taken care elsewhere 
      case 'titles':
        for (var k = 0; k < sNode.childNodes.length; k++) {
          var tNode = sNode.childNodes.item(k);
          if (tNode.nodeType == 3) continue; // Text node, not interested
          this.titles[tNode.getAttribute('lang')] = { "type":tNode.getAttribute('type'),"title":nodeData(tNode)};
        }
        break;
      default: showAlert('animeEntry for aid: '+this.id, node.nodeName, node.nodeName,sNode.nodeName);
    }
  }
}

CAnimeEntry.prototype.getTitle = function() {
  var title = null;
  if (this.titles[animeTitleLang]) title = this.titles[animeTitleLang]['title'];
  if (!title) {
    for (var lang in this.titles) { // default
      if (this.titles[lang]['type'] == 'main') { title = this.titles[lang]['title']; break; } 
    }
  }
  if (!title) title = 'unknown';
  return (title);
}

CAnimeEntry.prototype.getAltTitle = function() {
  var title = null;
  if (this.titles[animeAltTitleLang]) title = this.titles[animeAltTitleLang]['title'];
  if (!title) title = '';
  return (title);
}

/* *
 * Creates a new Episode Entry from a given node
 * @param node Node from which to get the episode information
 * @return episodeEntry
 */
function CEpisodeEntry(node) {
  this.id = Number(node.getAttribute('id'));
  this.type = 'normal';
  this.typeFull = 'Normal Episode';
  this.typeChar = '';
  this.vote = -1; // set later
  this.isRecap = false;
  this.animeId = -1;
  this.hiddenFiles = 0;
  this.seenDate = 0;
  this.length = 0;
  this.relDate = 0;
  this.userCount = 0;
  this.fileCount = 0;
  this.other = '';
  this.newFiles = false;
  this.titles = new Array();
  this.files = new Array(); // File IDS of related files are stored in the files array
  this.pseudoFiles = new Array(); // pseudo files are a totaly new thing
  for (var i = 0; i < node.childNodes.length; i++) {
    var sNode = node.childNodes.item(i);
    if (sNode.nodeType == 3) continue; // Text node, not interested
    switch (sNode.nodeName) {
      case 'flags': this.flags = Number(nodeData(sNode)); break;
      case 'epno': this.epno = Number(nodeData(sNode)); break;
      case 'len': this.length = Number(nodeData(sNode)); break;
      case 'date': this.addDate = convertTime(nodeData(sNode)); this.relDate = convertTime(sNode.getAttribute('rel')) || 0; break;
      case 'ucnt': this.userCount = Number(nodeData(sNode)); break;
      case 'fcnt': this.fileCount = Number(nodeData(sNode)); break;
      case 'other': this.other = nodeData(sNode); break;
      case 'titles':
        for (var k = 0; k < sNode.childNodes.length; k++) {
          var tNode = sNode.childNodes.item(k);
          if (tNode.nodeType == 3) continue; // Text node, not interested
          this.titles[tNode.getAttribute('lang')] = nodeData(tNode);
        }
        break;
      default: showAlert('episodeEntry for gid: '+this.id, node.nodeName, node.nodeName,sNode.nodeName);
    }
  }
  if (this.relDate == 0) this.date = this.addDate;
  else this.date = this.relDate;
  if (this.flags & 1) { this.type = 'special'; this.typeFull = 'Special Episode'; this.typeChar = 'S'; }
  if (this.flags & 2) { this.isRecap = true; this.typeFull += ', Recap'; }
  if (this.flags & 4) { this.type = 'opening'; this.typeFull = 'Opening/Ending/Credits'; this.typeChar = 'C'; }
  if (this.flags & 32) { this.type = 'trailer'; this.typeFull = 'Trailer/Promo/Ads'; this.typeChar = 'T'; }
  if (this.flags & 64) { this.type = 'parody'; this.typeFull = 'Parody/Fandub'; this.typeChar = 'P'; }
  if (this.flags & 128) { this.type = 'other'; this.typeFull = 'Other Episodes'; this.typeChar = 'O'; }
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
  if (this.titles[episodeTitleLang]) title = this.titles[episodeTitleLang];
  if (!title && episodeTitleLang != 'en' && this.titles['en']) title = this.titles['en']; 
  if (!title && episodeTitleLang != 'x-jat' && this.titles['x-jat']) title = this.titles['x-jat'];
  if (!title && episodeTitleLang != 'ja' && this.titles['ja']) title = this.titles['ja']; 
  if (!title) { for (var i in this.titles) { title = this.titles[i]; break; } }
  if (!title) title = 'Episode '+this.typeChar+this.epno;
  return (title);
}

CEpisodeEntry.prototype.getAltTitle = function() {
  var title = this.titles[episodeAltTitleLang]; // first option
  if (!title && episodeAltTitleLang != 'en' && this.titles['en']) title = this.titles['en']; 
  if (!title && episodeAltTitleLang != 'x-jat' && this.titles['x-jat']) title = this.titles['x-jat'];
  if (!title && episodeAltTitleLang != 'ja' && this.titles['ja']) title = this.titles['ja']; 
  if (!title) { for (var i in this.titles) { title = this.titles[i]; break; } }
  if (!title) title = 'Episode '+this.typeChar+this.epno;
  return (title);
}

CEpisodeEntry.prototype.getTitles = function() {
  var ret = new Array();
  for (var tid in this.titles) ret.push(tid + ': ' +title);
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
  this.source = 'unknown';
  this.other = '';
  this.usersTotal = 0;
  this.usersUnknown = 0;
  this.usersDeleted = 0;
  this.vidCnt = 0;
  this.audCnt = 0;
  this.subCnt = 0;
  this.newFile = false;
  this.pseudoFile = false; // Got fed up with strange methods for checking if a file isn't pseudo
  this.videoTracks = new Array();
  this.audioTracks = new Array();
  this.subtitleTracks = new Array();
  // Actualy fill the data;
  for (var i = 0; i < node.childNodes.length; i++) {
    var sNode1 = node.childNodes.item(i);
    switch (sNode1.nodeName) {
      case 'size': this.size = Number(nodeData(sNode1)); break;
      case 'ed2k': this.ed2k = nodeData(sNode1); break;
      case 'crc': this.crc32 = nodeData(sNode1); break;
      case 'len': this.length = Number(nodeData(sNode1)); break;
      case 'ftype': this.fileType = nodeData(sNode1); break;
      case 'group': this.groupId = Number(sNode1.getAttribute('id')); this.relatedGroups.push(this.groupId); break; 
      case 'flags': this.flags = Number(nodeData(sNode1)); break;
      case 'date': 
        this.date = convertTime(nodeData(sNode1));
        if (Number(new Date()/1000 - javascriptDate(this.date)/1000) < 86400) this.newFile = true;
        this.relDate = convertTime(sNode1.getAttribute('rel'));
        break;
      case 'vid':
        this.vidCnt = Number(sNode1.getAttribute('cnt')) || 0;
        for (var j = 0; j < sNode1.childNodes.length; j++) {
          var dNode = sNode1.childNodes.item(j);
          switch (dNode.nodeName) {
            case 'stream':
              var stream = new Object;
              stream.resW = 0;
              stream.resH = 0;
              stream.ar = 'unknown';
              stream.codec = 'unknown';
              for (var k = 0; k < dNode.childNodes.length; k++) {
                var stNode = dNode.childNodes.item(k);
                switch (stNode.nodeName) {
                  case 'res': 
                    stream.resW = Number(stNode.getAttribute('w')) || 0; 
                    stream.resH = Number(stNode.getAttribute('h')) || 0;                     
                    if (stream.resW && stream.resH) this.resolution = stream.resW + 'x' + stream.resH;
                    break;
                  case 'ar': stream.ar = nodeData(stNode); break;
                  case 'codec': stream.codec = nodeData(stNode); break;
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
          switch (dNode.nodeName) {
            case 'stream':
              var stream = new Object;
              stream.chan = 'unknown';
              stream.codec = 'unknown';
              stream.type = 'normal';
              for (var k = 0; k < dNode.childNodes.length; k++) {
                var stNode = dNode.childNodes.item(k);
                switch (stNode.nodeName) {
                  case 'lang': stream.lang = nodeData(stNode); break;
                  case 'chan': stream.chan = nodeData(stNode); break;
                  case 'codec': stream.codec = nodeData(stNode); break;
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
          switch (dNode.nodeName) {
            case 'stream':
              var stream = new Object;
              stream.type = 'unknown';
              stream.flags = 0;
              for (var k = 0; k < dNode.childNodes.length; k++) {
                var stNode = dNode.childNodes.item(k);
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
          switch (sNode2.nodeName) {
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
}
