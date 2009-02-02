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
// overrides
var get_info_pentity = 1;
var collapseThumbnails = 0;

/* Function that prepares the page */
function prepPage() {
	var uriObj = parseURI(); // update the uriObj
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	var idDiv = document.getElementById('layout-nav');
	isCreatorPage = (idDiv.className.indexOf('creator') >= 0);
	if (isCreatorPage) {
		initTooltips();
		var tableNames = ['characterlist','stafflist'];
		var sortingCols = {
			'characterlist': [	{"type":'c_none',"isDefault":false},	// image
								{"type":'c_setlatin',"isDefault":true},// name
								{"type":'c_setlatin',"isDefault":false},// anime
								{"type":'c_latin',"isDefault":false},	// type
								{"type":'c_number',"isDefault":false},	// eps
								{"type":'c_date',"isDefault":false},	// year
								{"type":'c_latin',"isDefault":false},	// rating
								{"type":'c_latin',"isDefault":false},	// main
								{"type":'c_latin',"isDefault":false}	],// comment
			'stafflist': 	[	{"type":'c_none',"isDefault":false},	// image
								{"type":'c_setlatin',"isDefault":true},// name
								{"type":'c_latin',"isDefault":false},	// type
								{"type":'c_number',"isDefault":false},	// eps
								{"type":'c_date',"isDefault":false},	// year
								{"type":'c_latin',"isDefault":false},	// rating
								{"type":'c_latin',"isDefault":false},	// credit
								{"type":'c_latin',"isDefault":false},	// episode restriction
								{"type":'c_latin',"isDefault":false}]	// comment
		};
		var skipTables = null;
		handleTables(sortingCols,tableNames,skipTables,collapseThumbnails,get_info_pentity);
	}
}

addLoadEvent(prepPage);