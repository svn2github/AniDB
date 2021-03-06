/* *
 * @file Shared interfaces
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (25.07.2007)
 */

// GLOBALs //

var uid;												// userID
var userInfo;										// userInfo
var anime;											// anime Object (used in animePage)
var animes = new Array();				// stored by aid
var animeOrder = new Array();		// animes ordered in the db away (link to aid)
var groups = new Array();				// stored by gid
var aGroups = new Array();			// stored by agid
var awardtypes = new Array();		// stored by awardtypeid
var companys = new Array();			// stored by prid
var episodes = new Array();			// stored by eid
var epOrder = new Array();			// episodes ordered in the db away (link to eid)
var files = new Array();				// stored by fid
var uriObj = new Array();				// Object that holds the URI
var seeDebug = false;						// should we display debug info (can be very verbose)
var seeTimes = false;						// should we display timing info?
function curDataWrite() {};			// var that holds the dataWrite function to use

/* *
 * Fetches data
 * @param url URL of the data to fetch
 * @param func Parsing function
 */
function loadData(url,func) {
  var req = xhttpRequest();
  xhttpRequestFetch(req, url, func);
}


/* *
 * Removes rows from a given TBody
 */
function clearTableRows(tbody) {
	while (tbody.rows.length) tbody.removeChild(tbody.rows[0]);
}


function clearFontWeight(parentNode,type) {
	//alert('pType: '+parentNode.nodeName+'\nnType: '+type);
	var elems = parentNode.getElementsByTagName(type);
	for (var i = 0; i < elems.length; i++) { 
		var elem = elems[i];
		if (!elem.style) continue;
		elem.style.fontWeight = 'normal';
	}
}

/* *
 * Sets the number of EntriesPerPage and refreshes data
 */
function setEpp() {
	var epp = Number(this.firstChild.nodeValue);
	if (epp.isNaN) return;
	clearFontWeight(this.parentNode.parentNode,this.nodeName);
	this.style.fontWeigth = 'bold';
	entriesPerPage = epp;
	curDataWrite(); // force data write
	curpage = 1;
	showPage(curpage);
	updateStatus('');
}

/* *
 * Creates a search box
 */
function createSearchBox() {
  var searchboxes = document.getElementsByTagName('searchbox');
  for (var j = 0; j < searchboxes.length; j++) {
    var ul = document.createElement('UL');
    ul.className = 'searchBox';
    var li = document.createElement('LI');
    var input = createTextInput('searchbox',15,false,false);
    var button = document.createElement('INPUT');
    button.type = 'submit';
    button.onclick = filterByText;
    button.value = "search";
    li.appendChild(input);
    li.appendChild(button);
    ul.appendChild(li);
    searchboxes[j].parentNode.replaceChild(ul,searchboxes[j]);
  }
}

/* *
 * Replace Global's
 * @param node to search
 */
function replaceGlobals(node) {
  if (!userInfo) return;
  var elems = node.getElementsByTagName('USERNAME');
  while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(userInfo.name),elems[0]);
  elems = node.getElementsByTagName('USERID');
  while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(userInfo.id),elems[0]);
  elems = node.getElementsByTagName('THEMENAME');
  while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(theme['name']),elems[0]);
  elems = node.getElementsByTagName('THEMEVERSION');
  while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(theme['version']),elems[0]);
  elems = node.getElementsByTagName('THEMESOURCE');
  while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(theme['source']),elems[0]);
  elems = node.getElementsByTagName('THEMEDATE');
  while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(theme['date']),elems[0]);
  elems = node.getElementsByTagName('USERIDLINK');
  var a = createLink(null, 'u'+userInfo.id, 'http://anidb.net/u'+userInfo.id, null, null, null, 'short_link');
  while (elems.length) elems[0].parentNode.replaceChild(a,elems[0]);
	elems = document.getElementsByTagName('USERVLINK');
	a = createLink(null, 'v'+userInfo.id, 'http://anidb.net/v'+userInfo.id, null, null, null, 'short_link')
	while (elems.length) elems[0].parentNode.replaceChild(a,elems[0]);
  elems = node.getElementsByTagName('USERPAGELINK');
  a = createLink(null, 'userpage', 'http://anidb.net/up'+userInfo.id, null, null, null, 'short_link');
  while (elems.length) elems[0].parentNode.replaceChild(a,elems[0]);
  elems = node.getElementsByTagName('ANIDBMSGUSERLINK');
  a = createLink(null, 'send message', base_url+'?show=msg&do=new&msg.to='+userInfo.name, null, null, null, null);
  while (elems.length) elems[0].parentNode.replaceChild(a,elems[0]);
	elems = document.getElementsByTagName('VOTES.RESOURCES');
	while (elems.length) {
		var div = document.createElement('DIV');
		div.className = 'icons';
		createIcon(div, 'mylist', 'mylist.html?show=mylist', null, 'back to mylist', 'i_backtomylist');
		elems[0].parentNode.replaceChild(div,elems[0]);
	}
  createSearchBox();
}
