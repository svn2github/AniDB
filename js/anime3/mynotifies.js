jsVersionArray.push({
	"file":"anime3/mynotifies.js",
	"version":"0.1",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$"
});

var sortingCols = {
	'anime_related': {	"name anime":{"type":'c_latin',"isDefault":true},
                        "name group":{"type":'c_latin',"isDefault":true},
                        "type":{"type":'c_latin'},
                        "priority sortable":{"type":'c_set'}, // anidbscript.js adds class 'sortable'
                        "mylist sortable":{"type":'c_set'},   // before handleTables() is called
                        "seen sortable":{"type":'c_set'},
                        "state":{"type":'c_latin'},
                        "date":{"type":'c_date'}},
	'forum_related': {	"name":{"type":'c_latin',"isDefault":true},
                        "topics":{"type":'c_number'},
                        "replies":{"type":'c_number'},
                        "sendreplies":{"type":'c_latin'},
                        "since":{"type":'c_date'},
                        "lastpost":{"type":'c_date'}}
};
var tableNames = ["anime_related", "forum_related"];
var skipTables = ["forum_related"];

function prepPage() {
    var tables = getElementsByClassName(document.getElementsByTagName('table'),'mynotifies_list',true);
	if (!tables) return;
    
    for (var i = 0; i < tables.length; i++) {
        var tableName = tables[i].className.split(' ')[1];
        switch (tableName) {
            case 'group':
            case 'anime':
                var tfoot = document.createElement('tfoot');
                tfoot.appendChild(tables[i].tBodies[0].rows[tables[i].tBodies[0].rows.length-1]);
                tables[i].appendChild(tfoot);
                tables[i].id = 'anime_related'; break;
            case 'forumblog':
            case 'forumforum':
            case 'forumclub':
            case 'threads':
                tables[i].id = 'forum_related'; break;
        }
    }
	
	initTooltips();
	createPreferencesTable('global');
	handleTables(sortingCols, tableNames, skipTables, collapseThumbnails, (get_info & 2));
}

// hook up the window onload event
addLoadEvent(prepPage);