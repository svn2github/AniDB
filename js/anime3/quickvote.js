/* file Quickvote interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (19.11.2008) - Initial version
 */
jsVersionArray.push({
	"file":"anime3/quickvote.js",
	"version":"1.0",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Initial version"
});

var sortingCols = {
	'animelist': 	{	"name":{"type":'c_setlatin',"isDefault":true},
						"old":{"type":'c_number'}}
};
var tableNames = ['animelist'];
var skipTables = null;
 
function prepPage() {
	var table = document.getElementById('animelist');
	if (!table) return;
	var tfoot = document.createElement('tfoot');
	tfoot.appendChild(table.tBodies[0].rows[table.tBodies[0].rows.length-1]);
	table.appendChild(tfoot);
	
	initTooltips();
	createPreferencesTable('mylist');
	handleTables(sortingCols, tableNames, skipTables, collapseThumbnails, (get_info & 2));
	
	return; //noec: Previously this function was not added to the onLoad event.
	
	var table = getElementsByClassName(document.getElementsByTagName('table'), 'animelist', false)[0];
	if (!table) return;
	var tbody = table.tBodies[0];
	var thead = document.createElement('thead');
	var row = tbody.rows[0];
	row.cells[2].colSpan = 1;
	thead.appendChild(row);
	table.insertBefore(thead,tbody);
	var tfoot = document.createElement('tfoot');
	row = tbody.rows[tbody.rows.length-1];
	row.cells[0].colSpan = 3;
	tfoot.appendChild(row);
	table.appendChild(tfoot);
	// okay, fun begins now!
	for (var i = 0; i < tbody.rows.length; i++) {
		var row = tbody.rows[i];
		while (row.cells.length > 3) row.removeChild(row.cells[3]);
		var cell = row.cells[2];
		var input = cell.getElementsByTagName('input')[0];
		var name = input.name;
		var value = (row.cells[1].firstChild ? Number(row.cells[1].firstChild.nodeValue) : 0);
		while (cell.childNodes.length) cell.removeChild(cell.firstChild);
		spinCtrl = new SpinControl();
		spinCtrl.SetMaxValue(10);
		spinCtrl.SetMinValue(1);
		spinCtrl.SetCurrentValue(value);
		spinCtrl.GetAccelerationCollection().Add(new SpinControlAcceleration(0.10,  500));
		spinCtrl.GetAccelerationCollection().Add(new SpinControlAcceleration(0.50, 1000));
		cell.appendChild(spinCtrl.GetContainer());
		var hiddenInput = createTextInput(name,20,false,true,null,0);
		spinCtrl.SetHiddenInput(hiddenInput);
		spinCtrl.GetInput().value = value;
		spinCtrl.AttachValueChangedListener(function updateHiddenInputValue(sender,newval) { sender.GetHiddenInput().value = Number(newval)*100; });
		cell.appendChild(hiddenInput);
		spinCtrl.StartListening();
	}
}

addLoadEvent(prepPage);