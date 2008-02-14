/* *
 * @file producer page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.0 (08.01.2008)
 */
 
// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;

/* *
 * Updates the release list rows to allow more sorting options
 */
function updateReleaseListRows() {
	var div = getElementsByClassName(document.getElementsByTagName('DIV'), 'producer_related_anime', true)[0];
  if (!div) return;
	var table = div.getElementsByTagName('TABLE')[0];
	if (!table) return;
	var tbody = table.tBodies[0];
	for (var i = 1; i < tbody.rows.length; i++) { // update each row
		var row = tbody.rows[i];
		var titleCell = row.cells[0];		// Title Cell
		if (!titleCell) continue;
		var a = titleCell.getElementsByTagName('A')[0];
		if (!a) continue;
		var title = a.firstChild.nodeValue;
		titleCell.setAttribute('anidb:sort',title);
	}
}

/* *
 * Updates the release list table with sorting
 */
function updateReleaseList() {
	var div = getElementsByClassName(document.getElementsByTagName('DIV'), 'producer_related_anime', true)[0];
  if (!div) return;
	var table = div.getElementsByTagName('TABLE')[0];
	if (!table) return;
	headingList = table.getElementsByTagName('TH');
  // I know the headings i need so..
  headingList[0].className += ' c_setlatin';	// Anime
  headingList[1].className += ' c_latin';		// Relation
  headingList[2].className += ' c_date';			// Year
  headingList[3].className += ' c_number';		// EPS
  headingList[4].className += ' c_latin';		// TYPE
	init_sorting(table.tBodies[0].rows[0],'year','down');
}

function prepPage() {
  uriObj = parseURI(); // update the uriObj
  if (!uriObj['show'] || uriObj['show'] != 'producer') return; // badPage
	updateReleaseList();
	updateReleaseListRows();
}

//window.onload = prepPage;
addLoadEvent(prepPage);