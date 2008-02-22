/* *
 * @file episode page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.0 (19.01.2008)
 */
 
// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var epTable = null;

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
function updateEpisodeListRows() {
	var table = epTable;
	if (!epTable) table = getElementsByClassName(document.getElementsByTagName('TABLE'), 'filelist', true)[0];
	if (!table) return;
	var tbody = table.getElementsByTagName('TBODY')[0];
	var extraCol = (uriObj['showcrc'] && uriObj['showcrc'] == 1) ? 1 : 0;
	for (var i = 0; i < tbody.rows.length; i++) { // update each row
		var row = tbody.rows[i];
		var test = row.cells[0];			// fid cell
		if (test) {
			var a = test.getElementsByTagName('A')[0];
			if (a) {
				var fid = a.href.substr(a.href.indexOf('fid=')+4,a.href.length);
				test.setAttribute('anidb:sort',fid);
			}
		}
		test = row.cells[1];		// Title Cell
		if (test) {
			var label = test.getElementsByTagName('LABEL')[0];
			if (!label || !label.childNodes.length) 
			var a = label.getElementsByTagName('A')[0];
			if (a) {
				var title = ''+a.firstChild.nodeValue;
				test.setAttribute('anidb:sort',title.toLowerCase());
			}
		}
		test = row.cells[2];		// Size cell
		if (test) test.setAttribute('anidb:sort',test.firstChild.nodeValue);
		test = row.cells[4+extraCol];		// codecs Cell
		if (test) {
			var codecSort = 'unk'; // default in case something borks
			var as = test.getElementsByTagName('A');
			for (var k = 0; k < as.length; k++) {
				var a = as[k];
				if (a.className.indexOf('i_video') < 0) continue;
				codecSort = a.className.substr(a.className.indexOf('i_video')+8,a.className.length);
				break;
			}
			test.setAttribute('anidb:sort',codecSort);
		}
		test = row.cells[6+extraCol];		// source Cell
		if (test) {
			var text = 'unknown'; // default in case something borks
			var label = test.getElementsByTagName('LABEL')[0];
			if (label && label.childNodes.length) text = label.firstChild.nodeValue;
			test.setAttribute('anidb:sort',text);
		}
		test = row.cells[7+extraCol];		// quality Cell
		if (test) {
			var span = test.getElementsByTagName('SPAN')[0];
			if (span) {
				var className = span.className.substr(span.className.indexOf('i_rate_')+7,span.className.length);
				test.setAttribute('anidb:sort',mapQuality(className));
			}
		}
		test = row.cells[9+extraCol];		// users Cell
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
 * Updates the release list table with sorting
 */
function updateEpisodeList() {
	var epTable = getElementsByClassName(document.getElementsByTagName('TABLE'), 'filelist', true)[0];
	if (!epTable) return;
	var tbody = epTable.getElementsByTagName('TBODY')[0];
	var thead = document.createElement('THEAD');
	thead.appendChild(tbody.rows[0]);
	epTable.insertBefore(thead,tbody);
	var tfoot = document.createElement('TFOOT');
	var generic = getElementsByClassName(tbody.getElementsByTagName('TR'), 'generic', true)[0];
	if (generic) tfoot.appendChild(generic);
	var showall = getElementsByClassName(tbody.getElementsByTagName('TR'), 'show_all', true)[0];
	if (showall) tfoot.appendChild(showall);
	epTable.appendChild(tfoot);
	headingList = thead.getElementsByTagName('TH');
	// I know the headings i need so.. i set the corresponding functions and add the names
	var i = 0;
	headingList[0].className += 'id c_set';				// File
	headingList[1].className += 'title c_setlatin';		// Groupname
	headingList[2].className += 'size c_setlatin';			// size
	if (uriObj['showcrc'] && uriObj['showcrc'] == 1) {
		headingList[3].className += 'crc c_latin';			// crc
		i++
	}
	headingList[4+i].className += 'codec c_setlatin';		// codec
	headingList[5+i].className += 'resolution c_latin';	// resolution
	headingList[6+i].className += 'source c_setlatin';		// source
	headingList[7+i].className += 'quality c_set';			// quality
	headingList[9+i].className += 'users c_set';			// users
	init_sorting(thead,'title','down');
}

function prepPage() {
  uriObj = parseURI(); // update the uriObj
  if (!uriObj['show'] || uriObj['show'] != 'ep') return; // badPage
	updateEpisodeList();
	updateEpisodeListRows();
}

//window.onload = prepPage;
addLoadEvent(prepPage);