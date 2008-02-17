/* *
 * @file group page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.0 (22.11.2007)
 */
 
// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var gTable = null;
var released_div = null;
var ep_table = null;

/* *
 * Converts qualitys to a rate system
 * @param qual Quality
 * @return Quality className
 */
function mapQuality(qualClassName) {
  switch (qualClassName) {
		case 'veryhigh': return (8);
		case 'high': return (7);
		case 'med': return (6);
		case 'low': return (5);
		case 'verylow': return (4);
		case 'corrupted': return (3);
		case 'eyecancer': return (2);
		case 'unknown': return (1);
  }
  return (1);
}

/* *
 * Updates the release list rows to allow more sorting options
 */
function updateReleaseListRows() {
	var table = gTable;
	if (!table) {
		var div = released_div;
		if (!div) return;
		var table = div.getElementsByTagName('TABLE')[0];
	}
	if (!table) return;
	var tbody = table.tBodies[0];
	for (var i = 1; i < tbody.rows.length; i++) { // update each row
		var row = tbody.rows[i];
		var test = row.cells[2];		// Title Cell
		if (test) {
			var a = test.getElementsByTagName('A')[0];
			if (a) {
				var title = a.firstChild.nodeValue;
				test.setAttribute('anidb:sort',title);
			}
		}
		test = row.cells[5];		// Done Cell
		if (test) {
			var cnt = test.firstChild.nodeValue;
			var neps = cnt.split('+')[0];
			var seps = cnt.split('+')[1];
			cnt = neps+seps;
			test.setAttribute('anidb:sort',cnt);
		}
		test = row.cells[7];		// State Cell
		if (test) {
			a = test.getElementsByTagName('A')[0];
			if (a) {
				var state = a.firstChild.nodeValue;
				test.setAttribute('anidb:sort',state);
			}
		}
		test = row.cells[9];	// Rating Cell
		if (test) {
			var rating = test.firstChild.nodeValue;
			test.setAttribute('anidb:sort',((rating == 'N/A ') ? "0" : rating));
		}
	}
}

/* *
 * Updates the release list table with sorting
 */
function updateReleaseList() {
	var div = released_div;
	if (!div) return;
	gTable = div.getElementsByTagName('TABLE')[0];
	if (!gTable) return;
	var headingList = gTable.getElementsByTagName('TH');
	// I know the headings i need so..
	headingList[0].className += ' c_date';			// First
	headingList[1].className += ' c_date';			// Last
	headingList[2].className += ' c_setlatin';		// Title
	headingList[3].className += ' c_number';		// EPS
	headingList[4].className += ' c_number';		// Last
	headingList[5].className += ' c_set';			// Done
	headingList[6].className += ' c_number';		// Undumped
	headingList[7].className += ' c_setlatin';		// State
	headingList[8].className += ' c_none';			// N
	headingList[9].className += ' c_set';			// Rating
	init_sorting(gTable.tBodies[0].rows[0],'title','down');
}

/* *
 * Updates the episode list rows to allow more sorting options
 */
function updateEpTableRows() {
	var table = ep_table;
	if (!table) return;
	var tbody = table.tBodies[0];
	for (var i = 1; i < tbody.rows.length; i++) { // update each row
		var row = tbody.rows[i];
		test = row.cells[4];		// Group Cell
		if (test) {
			var a = test.getElementsByTagName('A')[0];
			if (a) {
				var group = a.firstChild.nodeValue;
				test.setAttribute('anidb:sort',group);
			}
		}
		test = row.cells[7];		// Quality Cell
		if (test) {
			var span = test.getElementsByTagName('SPAN')[0];
			if (span) {
				var className = span.className.substr(span.className.indexOf('i_rate_')+7,span.className.length);
				test.setAttribute('anidb:sort',mapQuality(className));
			}
		}
		test = row.cells[10];	// Users Cell
		if (test) {
			var a = test.getElementsByTagName('A')[0];
			if (a) {
				var cnt = a.firstChild.nodeValue;
				test.setAttribute('anidb:sort',cnt);
			}
		}
	}
}

/* *
 * Updates the filelist table with sorting
 */
function updateEpTable() {
	var table = ep_table;
	if (!table) return;
	var headingList = table.getElementsByTagName('TH');
	// I know the headings i need so..
	headingList[0].className += ' c_none';			// X
	headingList[1].className += ' c_date';			// Date
	headingList[2].className += ' c_set';			// ID
	headingList[3].className += ' c_set';			// EP
	headingList[4].className += ' c_setlatin';		// Group
	headingList[5].className += ' c_set';			// Size
	headingList[6].className += ' c_latin';		// CRC
	headingList[7].className += ' c_setlatin';		// Quality
	headingList[8].className += ' c_latin';		// Source
	headingList[9].className += ' c_latin';		// Resolution
	headingList[10].className += ' c_set';			// User count
	init_sorting(table.tBodies[0].rows[0],'group','down');
}

function prepPage() {
	uriObj = parseURI(); // update the uriObj
	if (!uriObj['show'] || uriObj['show'] != 'group') return; // badPage
	released_div = getElementsByClassName(document.getElementsByTagName('DIV'), 'group_released', true)[0];
	ep_table = getElementsByClassName(document.getElementsByTagName('TABLE'), 'filelist', true)[0];
	if (released_div) { // releases
		updateReleaseListRows();
		updateReleaseList();
	}
	if (ep_table) {
		updateEpTableRows();
		updateEpTable();
	}
}

//window.onload = prepPage;
addLoadEvent(prepPage);