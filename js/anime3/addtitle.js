jsVersionArray.push({
	"file":"anime3/addtitle.js",
	"version":"0.1"
});

var baseCols = {	"date":{"type":'c_date'},
					"name":{"type":'c_setlatin'},
					"language":{"type":'c_latin'},
					"verified?":{"type":'c_latin'}
};

var sortingCols = {};
var tableNames = ["current", "deleted"];
var skipTables = tableNames;


function prepPage() {
	switch(curPageID) {
		case 'addeptitle': break;
		case 'addanimetitle': baseCols["type"] = {"type":'c_latin'}; break;
		default:
			errorAlert('prepPage',"unknown page identifier ("+curPageID+"), will not continue.");
			return;
	}
	
	for (var i = 0; i < tableNames.length; i++) {
		sortingCols[tableNames[i]] = baseCols;
	}
	
	handleTables(sortingCols, tableNames, skipTables, false, false);
	for (var i = 0; i < tableNames.length; i++) {
		init_sorting(document.getElementById(tableNames[i]), "date", "up");
	}
}

// hook up the window onload event
addLoadEvent(prepPage);