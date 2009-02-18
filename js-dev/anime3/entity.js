/* @file character/creator page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.0 (02.02.2009)
 */
jsVersionArray.push({
	"file":"anime3/entity.js",
	"version":"1.0",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Initial version"
});

var seeDebug = false;
var isCreatorPage = false;

/* Function that prepares the page */
function prepPage() {
	var uriObj = parseURI(); // update the uriObj
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	var idDiv = document.getElementById('layout-nav');
	isCreatorPage = (idDiv.className.indexOf('creator') >= 0);
	if (isCreatorPage) {
		loadSettings();
		initTooltips();
		var tableNames = ['characterlist','stafflist'];
		var sortingCols = {
			'characterlist': {	"name":{"type":'c_setlatin',"isDefault":true},
								"entity":{"type":'c_setlatin'},
								"type":{"type":'c_latin'},
								"eps":{"type":'c_number'},
								"year":{"type":'c_date'},
								"rating":{"type":'c_latin'},
								"ismainseiyuu":{"type":'c_latin'},
								"comment":{"type":'c_latin'}	},
			'stafflist': 	{	"name":{"type":'c_setlatin',"isDefault":true},
								"type":{"type":'c_latin'},
								"eps":{"type":'c_number'},
								"year":{"type":'c_date'},
								"rating":{"type":'c_latin'},
								"credit":{"type":'c_latin'},
								"eprange":{"type":'c_latin'},
								"comment":{"type":'c_latin'}}
		};
		var skipTables = null;
		handleTables(sortingCols,tableNames,skipTables,collapseThumbnails,(get_info & 32));
	}
}

addLoadEvent(prepPage);