/* file wishlist page support scripts
 * author fahrenheit (alka.setzer@gmail.com)
 *         
 * version 1.5 (07.07.2007)
 * version 1.6 (03.08.2008) - fixed this again
 */
jsVersionArray.push({
	"file":"anime3/aproduceradd.js",
	"version":"1.6",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Fixed this again"
});
 
// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;

function createTooltips(node) {
	if (!node) node = document;
	var infos = getElementsByClassName(node.getElementsByTagName('div'),'releaseinfotooltip',false);
	for (var i = 0; i < infos.length; i++) {
		var div = infos[i];
		var parentNode = div.parentNode;
		var span = parentNode.getElementsByTagName('span')[0];
		if (!span) continue;
		if (span.className.indexOf('i_gstate_none') >= 0) continue;
		span.title = "";
		parentNode.onmouseout = hideTooltip;
		parentNode.onmouseover = function onmouseover(event) { 
			var div = getElementsByClassName(this.getElementsByTagName('div'),'releaseinfotooltip',false)[0];
			if (div) { var ndiv = div.cloneNode(true); ndiv.style.display = ''; setTooltip(ndiv,true); }
		};
	}
}

function prepPage() {
	uriObj = parseURI(); // update the uriObj
	initTooltips();
	var table = getElementsByClassName(document.getElementsByTagName('table'),'wishlist',false)[0];
	if (!table) return;
	var tbody = table.tBodies[0];
	var thead = document.createElement('thead');
	var tfoot = document.createElement('tfoot');
	thead.appendChild(tbody.rows[0]);
	table.insertBefore(thead,tbody);
	var actionRow = getElementsByClassName(tbody.getElementsByTagName('tr'),'action',false)[0];
	if (actionRow) { // let's do some stuff
		tfoot.appendChild(actionRow);
		table.appendChild(tfoot);
		var inputs = actionRow.getElementsByTagName('input');
		for (var i = 0 ; i < inputs.length; i++) {
			if (inputs[i].type.toLowerCase() != 'checkbox') continue;
			var inp = inputs[i];
			inp.onclick = function SelectAllCKs(event) {
				var table = this.parentNode;
				while (table.nodeName.toLowerCase() != 'table') table = table.parentNode;
				var tbody = table.tBodies[0];
				var ckInputs = tbody.getElementsByTagName('input');
				for (var c = 0; c < ckInputs.length; c++) {
					if (ckInputs[c].type.toLowerCase() != 'checkbox') continue;
					ckInputs[c].checked = this.checked;
				}
			}
		}
	}
	createTooltips(table);
}

addLoadEvent(prepPage);