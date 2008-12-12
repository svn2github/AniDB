/* file Calendar interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (11.11.2008) - Initial version
 */
jsVersionArray.push({
	"file":"anime3/calendar.js",
	"version":"1.0",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Initial version"
});

// GLOBALs //

var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var seeTimes = false;
var submitQueue = new Array();	// This is kind of fake, but oh well...

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
	if (''+window.location.hostname == '') xhttpRequestPost(req, 'msg_del.html', acceptChanges, data);
	else xhttpRequestPost(req, 'animedb.pl', acceptChanges, data);
}

function acceptChanges() {
	var input =submitQueue.shift();
	input.value = 'Update';
	input.disabled = false;
}

function createSubmitRequest() {
	var cell = this.parentNode;
	var url = 'animedb.pl?';
	var rows = cell.parentNode.parentNode.getElementsByTagName('tr');
	var cells = new Array();
	cells.push(cell);
	cells.push(getElementsByClassName(rows, 'wishlist', true)[0]);
	cells.push(getElementsByClassName(rows, 'notification', true)[0]);
	for (var c = 0; c < cells.length; c++) {
		var cell = cells[c];
		var inputs = cell.getElementsByTagName('input');
		var selects = cell.getElementsByTagName('select');
		for (var i = 0; i < inputs.length; i++) {
			var input = inputs[i];
			if (input.id == 'adba.update' || input.value == '-1') continue;
			url += input.name + '=' + input.value + '&';
		}
		for (var i = 0; i < selects.length; i++) {
			var select = selects[i];
			if (select.value == '-1') continue;
			url += select.name + '=' + select.value + '&';
		}
	}
	if (url[url.length-1] == '&') url = url.substr(0,url.length-1);
	this.value = 'Submiting...';
	this.disabled = true;
	submitQueue.push(this);
	//alert(url);
	postData(url);
}

function togglePrioritySelect() {
	var select = this.parentNode.getElementsByTagName('select')[1];
	if (!select) return;
	select.disabled = (this.value == '-1');
	select.value = (this.value == '-1') ? '-1' : '1';
	var stateInput = this.parentNode.getElementsByTagName('input')[0];
	stateInput.value = (this.value == '-1') ? 'off' : 'on';
}

function parseCalendarRows(row,wishlistState,type) {
	if (row && wishlistState.type != '-1') return wishlistState;
	var pri = row.getElementsByTagName('img')[0];
	if (pri) {
		var pristate = pri.title.substr(10,pri.title.length);
		switch(pristate) {
			case 'low': pristate = '0'; break;
			case 'medium': pristate = '1'; break;
			case 'high': pristate = '2'; break;
			default: pristate = '-1'; break;
		}
		wishlistState = {'aid':wishlistState.aid,'type':type,'pri':pristate,'state':'on'};
	}
	return wishlistState;
}

function parseWishlistOther(row,wishlistState) {
	if (row && wishlistState.type != '-1') return wishlistState;
	wishlistState = parseCalendarRows(row,wishlistState,'0');
	if (wishlistState.state != '0') return wishlistState; //all done, otherwise need to see if it's blacklisted
	var as = row.getElementsByTagName('a');
	for (var i = 0; i < as.length; i++) {
		if (as[i].href.indexOf('adba.wishlisttype=3') >= 0) return {'aid':wishlistState.aid,'type':'3','pri':'1','state':'on'};
	}
	return wishlistState;
}

function createWishlistRow(parentNode, wishlistState, className) {
	//alert('aid: '+wishlistState.aid+'\nstate: '+wishlistState.state+'\ntype: '+wishlistState.type+'\npri: '+wishlistState.pri);
	if (!wishlistState) return;
	var row = document.createElement('tr');
	row.className = ((className) ? className + ' ' : '') + 'wishlist';
	var head = createHeader(null, 'field');
	createTextLink(head, 'Wishlist', 'animedb.pl?show=mywishlist#a'+wishlistState.aid);
	row.appendChild(head);
	var cell = createCell(null, 'value',createTextInput('adba.wishlist',10,false,true,null,wishlistState.state));
	var optionArray = {	'-1':{"text":'type: not set'},'0':{"text":'type: undefined'},
						'1':{"text":'type: to watch'},'2':{"text":'type: to get'},'3':{"text":'type: blacklisted'}};
	var stateSel = createSelectArray(cell,"adba.wishlisttype","adba.wishlisttype",togglePrioritySelect,wishlistState.type,optionArray);
	cell.appendChild(document.createTextNode(' '));
	optionArray = {	'-1':{"text":'priority: not set'},'0':{"text":'priority: low'},
						'1':{"text":'priority: medium'},'2':{"text":'priority: high'}};
	var priSel = createSelectArray(null,"adba.wishlistpri","adba.wishlistpri",null,wishlistState.pri,optionArray);
	if (wishlistState.pri == '-1') priSel.disabled = true;
	cell.appendChild(priSel);
	row.appendChild(cell);
	if (parentNode) parentNode.appendChild(row);
	else return row;
}

function createNotificationRow(parentNode, notificationState, className) {
	//alert('aid: '+notificationState.aid+'\nstate: '+notificationState.state+'\ntype: '+notificationState.type+'\npri: '+notificationState.pri);
	if (!notificationState) return;
	var row = document.createElement('tr');
	if (className) row.className = className;
	var head = createHeader(null, 'field');
	createTextLink(head, 'Notification', 'animedb.pl?show=mynotifies#a'+notificationState.aid);
	row.appendChild(head);
	var cell = createCell(null, 'value',createTextInput('adba.notify',10,false,true,null,notificationState.state));
	var optionArray = {	'-1':{"text":'type: not set'},'0':{"text":'type: all'},
						'1':{"text":'type: new'},'2':{"text":'type: group'},'3':{"text":'type: complete'}};
	var stateSel = createSelectArray(cell,"adba.notifytype","adba.notifytype",togglePrioritySelect,notificationState.type,optionArray);
	cell.appendChild(document.createTextNode(' '));
	optionArray = {	'-1':{"text":'priority: not set'},'0':{"text":'priority: low'},
						'1':{"text":'priority: medium'},'2':{"text":'priority: high'}};
	var priSel = createSelectArray(null,"adba.notifypri","adba.notifypri",null,notificationState.pri,optionArray);
	if (notificationState.pri == '-1') priSel.disabled = true;
	cell.appendChild(priSel);
	row.appendChild(cell);
	if (parentNode) parentNode.appendChild(row);
	else return row;
}

function createActionRow(parentNode, aid, className) {
	var row = document.createElement('tr');
	row.className = ((className) ? className + ' ' : '') + 'action';
	var head = createHeader(null, 'field');
	createTextLink(head, 'Action', 'removeme');
	row.appendChild(head);
	var cell = createCell(null, 'value',createTextInput('aid',10,false,true,null,aid));
	cell.appendChild(createTextInput('show',10,false,true,null,'latestanimes'));
	cell.appendChild(createButton('adba.update','adba.update',false,'Update','button',createSubmitRequest,null));
	row.appendChild(cell);
	if (parentNode) parentNode.appendChild(row);
	else return row;
}

function parseNotificationRow(row,notificationState) {
	if (!row) return notificationState;
	notificationState = parseCalendarRows(row,notificationState,'0');
	if (notificationState.pri != '-1') { // type is set, need to figure out what type is on
		var as = row.getElementsByTagName('a');
		var types = [0,0,0,0];
		for (var i = 0; i < as.length; i++) {
			var a = as[i];
			if (a.href.indexOf('adba.notifytype') < 0) continue;
			var start = a.href.indexOf('adba.notifytype=')+16;
			var end = a.href.indexOf('&',start);
			if (end < 0) end = a.href.length;
			var index = a.href.substr(start,end);
			types[Number(index)] = 1;
		}
		notificationState.type = types.indexOf(0);
	} else notificationState.type = '-1';
	return notificationState;
}

function parseCalendarEntry(entry) {
	if (!entry) return;
	var defList = getElementsByClassName(entry.getElementsByTagName('div'), 'g_definitionlist data', true)[0];
	if (!defList) return;
	var aid = entry.getElementsByTagName('h3')[0];
	if (aid) {
		aid = aid.getElementsByTagName('a')[0];
		var start = aid.href.indexOf('aid=')+4;
		var end = aid.href.indexOf('&',start);
		if (end < 0) end = aid.href.length;
		aid = aid.href.substr(start,end);
		aid = Number(aid);
	}
	// we now have the data, let's parse it
	var rows = defList.getElementsByTagName('tr');
	// wishlist parsing
	var wishlistState = {'aid':aid,'type':'-1','pri':'-1','state':'off'};
	var toWatchRow = getElementsByClassName(rows, 'wishlist_towatch', true)[0];
	var toGetRow = getElementsByClassName(rows, 'wishlist_toget', true)[0];
	var toOtherRow = getElementsByClassName(rows, 'wishlist_other', true)[0];
	wishlistState = parseCalendarRows(toWatchRow,wishlistState,'1');
	wishlistState = parseCalendarRows(toGetRow,wishlistState,'2');
	wishlistSTate = parseWishlistOther(toOtherRow,wishlistState);
	toWatchRow.parentNode.removeChild(toWatchRow);
	toGetRow.parentNode.removeChild(toGetRow);
	toOtherRow.parentNode.replaceChild(createWishlistRow(null, wishlistState, toOtherRow.className.indexOf('g_odd') >= 0 ? 'g_odd' : ''),toOtherRow);
	// Notification parsing
	var notificationState = {'aid':aid,'type':'-1','pri':'-1','state':'off'};
	var notificationRow = getElementsByClassName(rows, 'notification', true)[0];
	notificationState = parseNotificationRow(notificationRow,notificationState);
	notificationRow.parentNode.replaceChild(createNotificationRow(null, notificationState, notificationRow.className),notificationRow);
	// Recomendations
	var recomendationRow = getElementsByClassName(rows, 'recommend', true)[0];
	if (recomendationRow) {
		var a = recomendationRow.cells[1].getElementsByTagName('a')[0];
		if (!a) return;
		a.firstChild.nodeValue = 'to everyone';
		a.parentNode.appendChild(document.createTextNode(' | '));
		createTextLink(a.parentNode, 'to buddy', 'animedb.pl?show=buddyrec&aid='+aid, 'anidb::popup',BasicPopupSelf, null, '560.340.1.1.atbuddyrec');
	}
	// Action row
	createActionRow(recomendationRow.parentNode, aid, recomendationRow.className.indexOf('g_odd') >= 0 ? '' : 'g_odd');
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