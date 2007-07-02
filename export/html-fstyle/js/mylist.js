/* *
 * @file Mylist interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (22.03.2007)
 */
 
// GLOBALs //

var uid;                       // userID
var userInfo;                  // userInfo
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
var animeTitleLang = 'x-jat';
var animeAltTitleLang = 'en';
var animeTitleDisplay = 2;
var episodeTitleLang = 'en';
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
var animeListTable; // the animelist table
var animeListRow; // the sample animeList row
var base_url = 'http://anidb.info/perl-bin/animedb.pl';

function loadData(url,func) {
  var req = xhttpRequest();
  xhttpRequestFetch(req, url, func);
}

function parseData(xmldoc) {
  updateStatus('parsing Custom node');
  var root = xmldoc.getElementsByTagName('root').item(0);
  base_url = root.getAttribute('anidb');
  updateStatus('Processing animes...');
  var t1 = new Date();
  parseAnimes(root.getElementsByTagName('animes'));
  var parseAnimeNode = (new Date()) - t1;
  updateStatus('Processing user information...');
  var t1 = new Date();
  parseCustom(root.getElementsByTagName('custom').item(0));
  var parseCustomNode = (new Date()) - t1;  
  if (seeTimes) { alert('Processing...'+
        '\n\tanime: '+parseAnimeNode+'ms'+
        '\n\tcustom: '+parseCustomNode+' ms'+
        '\nTotal: '+(parseAnimeNode+parseCustomNode)+' ms'); }
  updateStatus('');
  renderPage();
}

function updateAnimeRow(anime) {
  row = animeListRow.cloneNode(true);
  row.id = row.id.replace('%ANIMEID%',anime.id);
  if (anime.complete) row.className = "complete ";
  if (anime.watched) row.className = "all_watched ";
  // EXPAND
  // TITLE
  var cell = row.cells[1];
  var icons = getElementsByClassName(cell.getElementsByTagName('DIV'),'icons',false)[0];
  if (icons) {
    while (icons.childNodes.length) icons.removeChild(icons.firstChild);
    var state = anime.state;
    if (state.indexOf('hdd') >= 0) state = 'onhdd';
    if (state.indexOf('cd') >= 0) state = 'oncd';
    createIcon(icons, anime.state, null, null, 'state: '+anime.state,'i_state_'+state);
  }
  var ahref = cell.getElementsByTagName('A')[0];
  if (ahref) {
    ahref.href = ahref.href.replace('%ANIMEID%',anime.id);
    var title = anime.getTitle('main');
    if (animeTitleDisplay == 1) title += ' ('+anime.getAltTitle('official')+')';
    cell.innerHTML = cell.innerHTML.replace('%ANIMETITLE%',title);
    if (animeTitleDisplay == 2 && anime.getAltTitle('official').length) ahref.title = 'Alternative title: '+ anime.getAltTitle('official');
  }
  // EPS
  cell = row.cells[2];
  var owned = anime.eps['user'];
  var total = anime.eps['cnt']+(anime.seps['cnt'] ? '+' + anime.seps['cnt'] : '');
  cell.innerHTML = cell.innerHTML.replace('%EPS%',owned+'/'+total);
  // SEEN
  cell = row.cells[3];
  var seen = anime.eps['seen'];
  cell.innerHTML = cell.innerHTML.replace('%SEEN%',seen+'/'+total);
  // RATING
  cell = row.cells[4];
  cell.innerHTML = cell.innerHTML.replace('%RATING%',(anime.votes ? anime.rating : '-'));
  cell.innerHTML = cell.innerHTML.replace('%VOTES%',(anime.votes ? anime.votes : '0'));
  // MY VOTE
  cell = row.cells[5];
  cell.innerHTML = cell.innerHTML.replace('%MYVOTE%',(anime.myvote ? anime.myvote['vote'] : ''));
  // REVIEWS
  cell = row.cells[6];
  cell.innerHTML = cell.innerHTML.replace('%REVIEWRATING%',(anime.reviews ? anime.rrating : '-'));
  cell.innerHTML = cell.innerHTML.replace('%REVIEWCNT%',(anime.reviews ? anime.reviews : '0'));
  // ACTION
  cell = row.cells[7];
  cell.innerHTML = cell.innerHTML.replace('%ANIMEID%',anime.id);
  return row;
}

// Only use when actualy needed. Because the rows won't get expanded if you use this
function replaceAnimeGlobals(node,anime) {
  replaceGlobals(node);
  node.innerHTML = node.innerHTML.replace(/\%ANIMEID\%/mgi,anime.id);
  node.innerHTML = node.innerHTML.replace(/\%ANIMETITLE\%/mgi,anime.title);
  var owned = anime.eps['user'];
  var total = anime.eps['cnt']+(anime.seps['cnt'] ? '+' + anime.seps['cnt'] : '');
  node.innerHTML = node.innerHTML.replace(/\%EPS\%/mgi,owned+'/'+total);
  var seen = anime.eps['seen'];
  node.innerHTML = node.innerHTML.replace(/\%SEEN\%/mgi,seen+'/'+total);
  node.innerHTML = node.innerHTML.replace(/\%RATING\%/mgi,(anime.votes ? anime.rating : '-'));
  node.innerHTML = node.innerHTML.replace(/\%VOTES\%/mgi,(anime.votes ? anime.votes : '0'));
  node.innerHTML = node.innerHTML.replace(/\%MYVOTE\%/mgi,(anime.myvote ? anime.myvote['vote'] : ''));
  node.innerHTML = node.innerHTML.replace(/\%REVIEWRATING\%/mgi,(anime.reviews ? anime.rrating : '-'));
  node.innerHTML = node.innerHTML.replace(/\%REVIEWCNT\%/mgi,(anime.reviews ? anime.reviews : '0'));
}

function replaceGlobals(node) {
  if (!userInfo) return;
  node.innerHTML = node.innerHTML.replace(/\%ANIDBURL\%/mgi,base_url);
  node.innerHTML = node.innerHTML.replace(/\%USERNAME\%/mgi,userInfo.name);
  node.innerHTML = node.innerHTML.replace(/\%USERID\%/mgi,userInfo.id);
  node.innerHTML = node.innerHTML.replace(/\%THEMEVERSION\%/mgi,theme['version']);
  node.innerHTML = node.innerHTML.replace(/\%THEMESOURCE\%/mgi,theme['source']);
  node.innerHTML = node.innerHTML.replace(/\%THEMEDATE\%/mgi,theme['date']);
}



function renderPage() {
  // first see if we have animes
  if (!animes) { alert('Do you have animes?'); return; }
  replaceGlobals(document.body);
  animeListTable = getElementsByClassName(document.getElementsByTagName('TABLE'),'animelist',false)[0];
  if (!animeListTable) { alert('Your html structure isn\'t valid'); return; }
  var tbody = animeListTable.tBodies[0];
  animeListRow = tbody.rows[0].cloneNode(true);
  tbody.removeChild(tbody.rows[0]);
  for (var i = 0; i < animeOrder.length; i++) {
    var anime = animes[animeOrder[i]];
    var row = updateAnimeRow(anime);
    row.className += (i % 2) ? 'g_odd' : '';
    tbody.appendChild(row);
  }
}

function prepPage() {
  uriObj = parseURI();
  loadData('mylist.xml',parseData);
}

window.onload = prepPage;
