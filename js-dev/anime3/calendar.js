/* file Calendar interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (11.11.2008) - Initial version
 */

// GLOBALs //

var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var seeTimes = false;

/* Function that fetches anime data
 * @param aid Anime ID
 * @param uid User ID
 */
function fetchData(url) {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/buddies.xml', parseData);
	else xhttpRequestFetch(req, url, parseData);
}

/* Function that posts data
 * @param url URL to post
 */
function postData(url) {
	var req = xhttpRequest();
	var data = url.substr(url.indexOf('?')+1,url.length);
	if (''+window.location.hostname == '') xhttpRequestPost(req, 'msg_del.html', null, data);
	else xhttpRequestPost(req, 'animedb.pl', null, data);
}

function parseCalendarRows(row,wishlistState,type) {
	if (row && wishlistState.state != '0') return wishlistState;
	var pri = row.getElementsByTagName('img')[0];
	if (pri) {
		var pristate = pri.title.substr(10,pri.title.length);
		switch(pristate) {
			case 'low': pristate = 'setpri0'; break;
			case 'high': pristate = 'setpri2'; break;
			case 'medium': pristate = 'setpri2'; break;
			default: pristate = '0'; break;
		}
		wishlistState = {'state':type,'pri':pristate};
	}
	return wishlistState;
}

function parseCalendarEntry(entry) {
	if (!entry) return;
	var defList = getElementsByClassName(entry.getElementsByTagName('div'), 'g_definitionlist data', true)[0];
	if (!defList) return;
	// we now have the data, let's parse it
	var rows = defList.getElementsByTagName('tr');
	// wishlist parsing
	var wishlistState = {'state':'0','pri':'0'};
	var notificationState = {'state':'0','pri':'0'};
	// to watch
	wishlistState = parseCalendarRows(getElementsByClassName(rows, 'wishlist_towatch', true)[0],wishlistState,'moveto1');
	wishlistState = parseCalendarRows(getElementsByClassName(rows, 'wishlist_toget', true)[0],wishlistState,'moveto2');
	wishlistState = parseCalendarRows(getElementsByClassName(rows, 'wishlist_other', true)[0],wishlistState,'moveto3');
	alert('state: '+wishlistState.state+'\npri: '+wishlistState.pri);
}

function prepPage() {
	uriObj = parseURI();
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	var calendarEntries = getElementsByClassName(document.getElementsByTagName('div'), 'latestanimes_anime', true);
	for (var i = 0; i < calendarEntries.length; i++) {
		parseCalendarEntry(calendarEntries[i]);
	}
}

// hook up the window onload event
addLoadEvent(prepPage);