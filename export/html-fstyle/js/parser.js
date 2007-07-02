/* *
 * @file XMLParser for mylist, anime and episode xml data 
 * @author fahrenheit (alka.setzer@gmail.com)
 *         Some code derived from PetriW's work at anidb
 * @contents Core Parsing Functions
 * @version 2.1.2 (26.06.2007)
 */

// CORE Vars //

var episodeFileTableRenderList = new Array(); // stack of fileTables in queue for render (currently not used)
var fInt = 100; // time to wait between ep node parsing 
var eInt = 150; // time to wait between ep render
var groupGuess = false; // Should i be making guesses on files for group?

// CORE Classes //

/* *
 * Processes a node to extract Custom (user) information
 * @param node Node to process
 * @return boolean (true if processing succeful, false otherwise)
 */
function parseCustom(node) {
  if (!node) return false; // no nodes return;
  for (var nd = 0; nd < node.length; nd++) { // find the right custom entry
    if (node[nd].parentNode.nodeName == 'root') { node = node[nd]; break; }
  }
  if (node.length > 1 || node.parentNode.nodeName != 'root') return; 
  uid = Number(node.getAttribute('uid')) || 0;
  mod = Number(node.getAttribute('mod')) || 0;
  for (var i = 0; i < node.childNodes.length; i++) {
    childNode = node.childNodes.item(i);
    if (childNode.nodeType == 3) continue;
    switch (childNode.nodeName) {
      case 'mylist':
        var mylistNodes = childNode.getElementsByTagName('file');
        for (m = 0; m < mylistNodes.length; m++) {
          var mylistNode = mylistNodes[m];
      	  mylistEntry = new CMylistEntry(mylistNode);
          mylist[mylistEntry.fileId] = mylistEntry;
          var ep = episodes[mylistEntry.episodeId];
          if (mylistEntry.seenDate) ep.seenDate = mylistEntry.seenDate;
          //if (mylistEntry.groupId != 0) {
            var group = groups[mylistEntry.groupId];
            group.isInMylist = true;
            group.visible = true;
            group.epsInMyListArray.push(ep.epno);
          //}
        }
        break;
      case 'ratings': // taking care of both episode votes and group ratings
        var groupVotes = childNode.getElementsByTagName('group');
        var episodeVotes = childNode.getElementsByTagName('ep');
        for (var gv = 0; gv < groupVotes.length; gv++) { // Group rating entries
          var gvote = groupVotes[gv];
          var urating = nodeData(gvote);
          var agid = gvote.getAttribute('agid');
          var gid = aGroups[agid].gid;
          groups[gid].userRating = urating;
        }
        for (var ev = 0; ev < episodeVotes.length; ev++) {
          var evote = episodeVotes[ev];
          var vote = nodeData(evote);
          var eid = evote.getAttribute('id');
          episodes[eid].vote = vote;
        }
        break;
      case 'config': parseConfig(childNode);
      default: showAlert('Options',node.nodeName,node.nodeName,childNode.nodeName);
    }
  }
  // We set now the string for the eps of each group the user has
  //
  var noGroup = groups[0];
  for (var i in groups) {
    var group = groups[i];
    if (group.id == 0) continue;
    if (group.epsInMyListArray == null || !group.epsInMyListArray.length) continue;
    var workArray = new Array();
    if (groupGuess) { // doing some guess
      for (var ep in group.epsInMyListArray) workArray.push(group.epsInMyListArray[ep]);
      for (var ep in noGroup.epsInMyListArray) {
        var epNo = noGroup.epsInMyListArray[ep];
        if (workArray.indexOf(epNo) == -1) workArray.push(epNo);
      }
    } else workArray = group.epsInMyListArray;
    // We now produce the new string with the episodes
    var startNum = -1;
    var st = 0;
    workArray.sort(function sort(a,b) { return (a-b); });
    while (st < workArray.length) {
      if (startNum == -1) startNum = workArray[st];
      if ((workArray.length > st+1) && (workArray[st]+1 < workArray[st+1])) {
        if (group.epsInMyListStringGuess.length > 0) group.epsInMyListStringGuess += ', ';
        if (startNum == workArray[st]) group.epsInMyListStringGuess += epNoToString(startNum);
        else group.epsInMyListStringGuess += epNoToString(startNum) + '-' + epNoToString(workArray[st]);
        startNum = -1;
      } 
      st++;
    }
    if (startNum >= 0) {
      if (group.epsInMyListStringGuess.length > 0) group.epsInMyListStringGuess += ', ';
      if (startNum == workArray[workArray.length-1]) group.epsInMyListStringGuess += epNoToString(startNum);
      else group.epsInMyListStringGuess += epNoToString(startNum) + '-' + workArray[workArray.length-1];
    }
    if (groupGuess) { // also keep track of real group info
      var startNum = -1;
      var st = 0;
      group.epsInMyListArray.sort(function sort(a,b) { return (a-b); });
      while (st < group.epsInMyListArray.length) {
        if (startNum == -1) startNum = group.epsInMyListArray[st];
        if ((group.epsInMyListArray.length > st+1) && (group.epsInMyListArray[st]+1 < group.epsInMyListArray[st+1])) {
          if (group.epsInMyListString.length > 0) group.epsInMyListString += ', ';
          if (startNum == group.epsInMyListArray[st]) group.epsInMyListString += epNoToString(startNum);
          else group.epsInMyListString += epNoToString(startNum) + '-' + epNoToString(group.epsInMyListArray[st]);
          startNum = -1;
        } 
        st++;
      }
      if (startNum >= 0) {
        if (group.epsInMyListString.length > 0) group.epsInMyListString += ', ';
        if (startNum == group.epsInMyListArray[group.epsInMyListArray.length-1]) group.epsInMyListString += epNoToString(startNum);
        else group.epsInMyListString += epNoToString(startNum) + '-' + group.epsInMyListArray[group.epsInMyListArray.length-1];
      }
    }
    if (!groupGuess) group.epsInMyListString = group.epsInMyListStringGuess;
  }
  return true;
}

/* *
 * Function to parse configuration options
 * @param node Config node
 * @return void Options will be set
 */
function parseConfig(node) {
  for (var i = 0; i < node.childNodes.length; i++) {
    var sNode = node.childNodes.item(i);
    if (sNode.nodeType == 3) continue;
    switch (sNode.nodeName) {
      case 'epp': preferredEntriesPerPage = parseInt(nodeData(sNode)) || 25; break;
      case 'lay':
        var lay = Number(nodeData(sNode)) || 0;
        LAY_HEADER = (lay & 1) ? true : false;
        LAY_NOANIMEGROUPREL = (lay & 32) ? true : false;
        LAY_HIDEFILES = (lay & 512) ? true : false;
        LAY_HIDERAWS = (lay & 1024) ? true : false;
        LAY_HIDEGENERICFILES = (lay & 4096) ? true : false;
        LAY_HIDEPARODYEPS = (lay & 131072) ? true : false;
        break;
      case 'animelang':
        for (var j = 0; j < sNode.childNodes.length; j++) {
          var dNode = sNode.childNodes.item(j);
          switch (dNode.nodeName) {
            case 'lang': animeTitleLang = nodeData(dNode); animeAltTitleLang = dNode.getAttribute('alt') || 'en'; break;
            default: showAlert('Options',sNode.nodeName,sNode.nodeName,dNode.nodeName);
          }
        }
        break;
      case 'eplang':
        for (var j = 0; j < sNode.childNodes.length; j++) {
          var dNode = sNode.childNodes.item(j);
          switch (dNode.nodeName) {
            case 'lang': animeTitleLang = nodeData(dNode); animeAltTitleLang = dNode.getAttribute('alt') || 'en'; break;
            default: showAlert('Options',sNode.nodeName,sNode.nodeName,dNode.nodeName);
          }
        }
        break;
      case 'filealang':
      	for (var j = 0; j < sNode.childNodes.length; j++) {
      	  var dNode = sNode.childNodes.item(j);
      	  switch (dNode.nodeName) {
      	  	case 'lang': filterAudLang.push(nodeData(dNode)); break;
      	  	default: showAlert('Options',sNode.nodeName,sNode.nodeName,dNode.nodeName);
      	  }
      	}
      	break;
      case 'fileslang':
      	for (var j = 0; j < sNode.childNodes.length; j++) {
      	  var dNode = sNode.childNodes.item(j);
      	  switch (dNode.nodeName) {
      	  	case 'lang': filterSubLang.push(nodeData(dNode)); break;
      	  	default: showAlert('Options',sNode.nodeName,sNode.nodeName,dNode.nodeName);
      	  }
      	}
      	break;
      case 'ed2k': break;
      default: showAlert('Options','config',sNode.nodeName,dNode.nodeName);
    }
  }
}

/* *
 * Processes a node to extract group information
 * @param node Node to process
 * @param aid Anime ID of group data
 * @return void (sets groups)
 */
function parseGroups(node,aid) {
  if (!node) return false; // no nodes return;
  for (var nd = 0; nd < node.length; nd++) { // find the right groups entry
    if (node[nd].parentNode.nodeName == 'anime') { node = node[nd]; break; }
  }
  if (node.length > 1 || node.parentNode.nodeName != 'anime') return;
  var groupNodes = node.getElementsByTagName('group');
  for (var i = 0; i < groupNodes.length; i++) {
    var childNode = groupNodes[i];
    var groupEntry = new CGroupEntry(childNode);
    var aGroupEntry = {'id': groupEntry.agid, 'gid': groupEntry.id};
    groups[groupEntry.id] = groupEntry;
    aGroups[aGroupEntry.id] = aGroupEntry;
    if (seeDebug) updateStatus('processed group '+(i+1)+' of '+groupNodes.length);
  }
  // create the "no group" group entry
  var groupEntry = new Object();
  groupEntry.id = groupEntry.agid = 0;
  groupEntry.name = "no group";
  groupEntry.shortName = "no group";
  groupEntry.visible = true;
  groupEntry.rating = '-';
  groupEntry.ratingCount = groupEntry.commentCount = 0;
  groupEntry.userRating = -1;
  groupEntry.sepCnt = groupEntry.epCnt = 0;
  groupEntry.isInMylist = false;
  groupEntry.epsInMyListArray = new Array();
  groupEntry.epsInMyListString = '';
  groupEntry.audioLangs = groupEntry.subtitleLangs = new Array();
  groupEntry.state = 'unknown';
  groupEntry.stateId = 0;
  groupEntry.hasCherryBeenPoped = false;
  groups[groupEntry.id] = groupEntry;
}

/* *
 * Processes a node to extract episode information
 * @param node Node to process
 * @param aid Anime ID of episode data
 * @return void (sets episodes)
 */
function parseEpisodes(node,aid) {
  if (!node) return false; // no nodes return;
  for (var nd = 0; nd < node.length; nd++) { // find the right episode entry
    if (node[nd].parentNode.nodeName == 'anime') { node = node[nd]; break; }
  }
  if (node.length > 1 || node.parentNode.nodeName != 'anime') return; 
  var epNodes = node.getElementsByTagName('ep');
  for (var i = 0; i < epNodes.length; i++) {
    var childNode = epNodes[i];
    var episodeEntry = new CEpisodeEntry(childNode);
    episodeEntry.animeId = aid;
    episodes[episodeEntry.id] = episodeEntry;
    epOrder.push(episodeEntry.id);
    if (seeDebug) updateStatus('processed episode '+(i+1)+' of '+epNodes.length);
  }
}

/* *
 * Processes a node to extract anime information
 * @param node Node to process
 * @return void (sets anime)
 */
function parseAnimes(node) {
  if (!node) return false; // no nodes return;
  var isAnimePage = (uriObj['show'].indexOf('anime') >= 0) ? true : false;
  for (var nd = 0; nd < node.length; nd++) { // find the right animes entry
    if (node[nd].parentNode.nodeName == 'root') { node = node[nd]; break; }
  }
  if (node.length > 1 || node.parentNode.nodeName != 'root') return; 
  var animeNodes = node.getElementsByTagName('anime');
  for (var i = 0; i < animeNodes.length; i++) {
    if (animeNodes[i].parentNode.nodeName != 'animes') continue; // there could be other anime nodes
    var childNode = animeNodes[i];
    var animeEntry = new CAnimeEntry(childNode);
    animes[animeEntry.id] = animeEntry;
    if (isAnimePage) anime = animes[animeEntry.id]; // assigning a shortcut
    var groupNodes = childNode.getElementsByTagName('groups');
    parseGroups(groupNodes,animeEntry.id); // Parsing Groups
    var epNodes = childNode.getElementsByTagName('eps');
    parseEpisodes(epNodes,animeEntry.id); // Parsing Episodes
    animeOrder.push(animeEntry.id); // This is need because Opera is a bad boy in for (elem in array)
    if (seeDebug) updateStatus('processed anime '+(i+1)+' of '+epNodes.length);
  }
}

/* *
 * Builds a fileEntry
 * @param node The node to parse
 * @param aid Anime Id
 * @param eid Episode Id
 * @return void (adds file to "files" and adds the file id to the ep.files list
 */
function buildFileEntry(node, aid, eid) {
  if (!node || !aid || !eid) return; // invalid call
  if (node.nodeName != 'file') return; // invalid node
  var fileEntry = new CFileEntry(node);
  var episode = episodes[eid];
  fileEntry.episodeId = eid;
  fileEntry.animeId = aid;
  if (files[fileEntry.id] && files[fileEntry.id].type == 'stub') { // In case we had a stub file, copy relevant entries
    var efile = files[fileEntry.id];
    for (var r in efile.epRelations)
      fileEntry.epRelations[r] = efile.epRelations[r];
    for (var r in efile.fileRelations)
      fileEntry.fileRelations[r] = efile.fileRelations[r];
    for (var r in efile.relatedFiles)
      fileEntry.relatedFiles[r] = efile.relatedFiles[r];
    for (var r in efile.relatedPS)
      fileEntry.relatedPS[r] = efile.relatedPS[r];
    for (var r in efile.relatedGroups)
      fileEntry.relatedGroups[r] = efile.relatedGroups[r];
  }
  files[fileEntry.id] = fileEntry;
  if (!episode) return; // This only happens in case of an external file
  if (fileEntry.newFile) episode.newFiles = true;
  if (episode.files.indexOf(fileEntry.id) < 0) episode.files.push(fileEntry.id)
}

/* *
 * Processes a node to extract file information for a given episode
 * @param node Node to process
 * @param aid Anime ID of episode data
 * @return void (sets files)
 */
function parseEpisode(node, aid) {
  if (!node || node.parentNode.nodeName != 'eps') return;
  var eid = Number(node.getAttribute('id'));
  var fileNodes = node.getElementsByTagName('file');
  var episode = episodes[eid];
  var nodeTime = new Date();
  for (var i = 0; i < fileNodes.length; i++)
    buildFileEntry(fileNodes[i],aid, eid);
  if (seeTimes) alert('Processing files for eid.'+eid+' took: '+(new Date() - nodeTime)+' ms');
  //var fileTable = createFileTable(episode);
  //document.getElementById('eid_'+episode.id+'_ftHolder').cells[0].className = '';
}
