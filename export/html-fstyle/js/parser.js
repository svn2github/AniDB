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
      case 'userinfo': userInfo = new CUserinfoEntry(childNode); break;
      default: showAlert('Options',node.nodeName,node.nodeName,childNode.nodeName);
    }
  }
  return true;
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
		animes[aid].groups.push(groupEntry.id);
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
	animes[aid].groups.push(groupEntry.id);
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
		animes[aid].eps.push(episodeEntry.id);
    epOrder.push(episodeEntry.id);
		parseEpisode(childNode,aid);
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
  var isAnimePage = false;
	if (uriObj['show']) {
		var pages = ['anime','ep','file'];
		for (var i in pages) { if (uriObj['show'].indexOf(pages[i]) >= 0) isAnimePage = true; }
	}
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
    if (isAnimePage) {
      anime = animes[animeEntry.id]; // assigning a shortcut
      var groupNodes = childNode.getElementsByTagName('groups');
      parseGroups(groupNodes,animeEntry.id); // Parsing Groups
      var epNodes = childNode.getElementsByTagName('eps');
      parseEpisodes(epNodes,animeEntry.id); // Parsing Episodes
			if (seeDebug) updateStatus('processed anime '+(i+1)+' of '+epNodes.length);
    }
    animeOrder.push(animeEntry.id); // This is need because Opera is a bad boy in for (elem in array)
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
}
