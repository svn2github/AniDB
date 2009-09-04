jsVersionArray.push({
	"file":"anime3/notifies.js",
	"version":"0.1",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$"
});

var sortingCols = {
	'mynotifies_list': {	"name anime":{"type":'c_setlatin',"isDefault":true},
                            "type":{"type":'c_latin'},
                            "priority sortable":{"type":'c_set'},
                            "mylist sortable":{"type":'c_set'}, // anidbscript.js adds class 'sortable'
                            "seen sortable":{"type":'c_set'},   // before handleTables() is called
                            "state":{"type":'c_latin'},
                            "date":{"type":'c_date'}}
};
var tableNames = ["mynotifies_list"];
var skipTables = null;

function prepPage() {
    var table = getElementsByClassName(document.getElementsByTagName('table'),'mynotifies_list',false)[0];
    if (!table) return;
    table.id = 'mynotifies_list';
	var tfoot = document.createElement('tfoot');
	tfoot.appendChild(table.tBodies[0].rows[table.tBodies[0].rows.length-1]);
	table.appendChild(tfoot);
	
	initTooltips();
	createPreferencesTable('global');
	handleTables(sortingCols, tableNames, skipTables, collapseThumbnails, (get_info & 2));
}

// hook up the window onload event
addLoadEvent(prepPage);