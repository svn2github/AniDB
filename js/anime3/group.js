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

/* *
 * Updates the release list rows to allow more sorting options
 */
function updateReleaseListRows() {
	var table = gTable;
	if (!table) {
		var div = getElementsByClassName(document.getElementsByTagName('DIV'), 'group_released', true)[0];
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
		test = row.cells[6];		// State Cell
		if (test) {
			a = test.getElementsByTagName('A')[0];
			if (a) {
				var state = a.firstChild.nodeValue;
				test.setAttribute('anidb:sort',state);
			}
		}
		test = row.cells[8];	// Rating Cell
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
	var div = getElementsByClassName(document.getElementsByTagName('DIV'), 'group_released', true)[0];
  if (!div) return;
	gTable = div.getElementsByTagName('TABLE')[0];
	if (!gTable) return;
	headingList = gTable.getElementsByTagName('TH');
  // I know the headings i need so..
  headingList[0].className += ' c_date';			// First
  headingList[1].className += ' c_date';			// Last
  headingList[2].className += ' c_setlatin';	// Title
  headingList[3].className += ' c_number';		// EPS
  headingList[4].className += ' c_number';		// Last
  headingList[5].className += ' c_set';			// Done
	headingList[6].className += ' c_setlatin';	// State
	headingList[7].className += ' c_none';			// N
	headingList[8].className += ' c_set';			// Rating
	//headingList[9].className += ' c_none';			// Action - Only available for *special* ppl
	init_sorting(gTable.tBodies[0].rows[0],'title','down');
}

function prepPage() {
  uriObj = parseURI(); // update the uriObj
  if (!uriObj['show'] || uriObj['show'] != 'group') return; // badPage
	updateReleaseListRows();
	updateReleaseList();
}

//window.onload = prepPage;
addLoadEvent(prepPage);