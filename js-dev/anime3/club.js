jsVersionArray.push({
	"file":"anime3/club.js",
	"version":"0.1",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$"
});

var userlistCols = {	"name user":{"type":'c_latin'},
						//"level":{"type":'c_latin'}, //sorting as c_latin is not quite right
						"name user added":{"type":'c_latin'},
						"date":{"type":'c_date'}};

var sortingCols = {};
var tableNames = ["invited", "reject", "request", "denied", "member", "exmember", "revoked", "kicked", "banned"];
var skipTables = tableNames;

function prepPage() {
	for (var i = 0; i < tableNames.length; i++) {
		sortingCols[tableNames[i]] = userlistCols;
	}
	handleTables(sortingCols, tableNames, skipTables, false, false);
}

// hook up the window onload event
addLoadEvent(prepPage);