/**
 * @file Mylist interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (22.03.2007)
 */

// GLOBALs //

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

function loadData() {
  var req = xhttpRequest();
  xhttpRequestFetch(req, 'mylist.xml', parseData);
}

function parseData(xmldoc) {
  updateStatus('parsing Custom node');
  var root = xmldoc.getElementsByTagName('root').item(0);
  updateStatus('Processing groups...');
  var t1 = new Date();
  parseGroups(root.getElementsByTagName('groups'));
  var parseGroupsNode = (new Date()) - t1;
  updateStatus('Processing animes...');
  var t1 = new Date();
  parseAnimes(root.getElementsByTagName('animes'));
  var parseAnimeNode = (new Date()) - t1;
  updateStatus('Processing user information...');
  var t1 = new Date();
  parseCustom(root.getElementsByTagName('custom').item(0));
  var parseCustomNode = (new Date()) - t1;  
  alert('Processing...\n\tgroups: '+parseGroupsNode+' ms'+
        '\n\tanime: '+parseAnimeNode+'ms'+
        '\n\tcustom: '+parseCustomNode+' ms'+
        '\nTotal: '+(parseGroupsNode+parseAnimeNode+parseCustomNode)+' ms');
  updateStatus('');
}
