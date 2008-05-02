/* buddylist page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * version 1.0 (02.04.2008) - Initial release
 */

// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var buddyListTable = null;

/* Function that updates buddylist rows */
function updateBuddyListRows() {
	if (!buddyListTable) return;
	var tbody = buddyListTable.tBodies[0];
	for (var i = 0; i < tbody.rows.length; i++) {
		var row = tbody.rows[i];
		if (row.cells[0]) {
			var a = row.cells[0].getElementsByTagName('a')[0];
			if (a) row.cells[0].setAttribute('anidb:sort',a.firstChild.nodeValue);
		}
		if (row.cells[2]) {
			var a = row.cells[2].getElementsByTagName('a')[0];
			if (a) row.cells[2].setAttribute('anidb:sort',a.firstChild.nodeValue);
		}
		/*
		if (row.cells[3]) {
			if (row.cells[3].childNodes.length) {
				var date = row.cells[3].firstChild.nodeValue;
				row.cells[3].setAttribute('anidb:sort',javascriptDate(date));
			} else row.cells[3].setAttribute('anidb:sort',0);
		}
		if (row.cells[4]) {
			if (row.cells[4].childNodes.length) {
				var date = row.cells[4].firstChild.nodeValue;
				row.cells[4].setAttribute('anidb:sort',javascriptDate(date));
			} else row.cells[4].setAttribute('anidb:sort',0);
		}
		*/
	}
}

/* Function that updates the buddylist */
function updateBuddyList() {
	if (!buddyListTable) return;
	var tbody = buddyListTable.tBodies[0];
	var thead = document.createElement('thead');
	thead.appendChild(tbody.rows[0]);
	buddyListTable.insertBefore(thead,tbody);
	var ths = thead.getElementsByTagName('th');
	ths[0].className = 'name c_setlatin';	// user
	ths[1].className = 'state c_latin';		// state
	ths[2].className = 'taste c_setlatin';	// taste
	ths[3].className = 'added c_date';		// added
	ths[4].className = 'online c_date';		// last online
	ths[5].className = 'action c_none';		// action
	init_sorting(thead,'name','down');
}

function prepPage() {
	uriObj = parseURI(); // update the uriObj
	var buddyListDiv = getElementsByClassName(document.getElementsByTagName('div'), 'g_section buddy_list', false)[0];
	if (!buddyListDiv) return;
	buddyListTable = buddyListDiv.getElementsByTagName('table')[0];
	updateBuddyList();
	updateBuddyListRows();
}

//window.onload = prepPage;
addLoadEvent(prepPage);