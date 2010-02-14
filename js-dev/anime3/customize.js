/* file customization support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *
 * version 1.0 (03.02.2009) - Initial release
 * version 1.5 (18.02.2009) - Merged information tooltips
 */
jsVersionArray.push({
	"file":"anime3/customize.js",
	"version":"1.5",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Merged information tooltips"
});

/* Function that sets options values */
function changeOptionValue(node) {
	if (!node) node = this;
	var name = "";
	if (node.name && node.id && node.name == node.id) name = node.name;
	else if (node.name && !node.id) name = node.name;
	else if (!node.name && node.id) name = node.id;
	else return;
	var value = (node.type == 'checkbox' ? Number(node.checked) : node.value);
	CookieSet(name,value,3650);
}

// Quick reference Table

//get_info_sz = null;
//get_info_mw = null;

/* Function that sets options values */
function changeOptionValue(node) {
	if (!node) node = this;
	var name = "";
	if (node.name && node.id && node.name == node.id) name = node.name;
	else if (node.name && !node.id) name = node.name;
	else if (!node.name && node.id) name = node.id;
	else return;
	var value = (node.type == 'checkbox' ? Number(node.checked) : node.value);
	CookieSet(name,value,3650);
}

/* Function that adds file columns */
function addColToBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var defsAnimeLayoutSelect = document.getElementById('defsAnimeLayoutSelect');
	var option;
	if (this.nodeName.toLowerCase() == 'input') // we are dealing with the button
		option = defsAnimeLayoutSelect.options[defsAnimeLayoutSelect.selectedIndex];
	else // we are dealing with an option
		option = this;
	option.ondblclick = remColFromBox;
	var selIndex = userAnimeLayoutSelect.selectedIndex;
	if (selIndex < 0)
		userAnimeLayoutSelect.appendChild(option);
	else
		userAnimeLayoutSelect.insertBefore(option,userAnimeLayoutSelect.options[selIndex]);
}

/* Function that removes file columns */
function remColFromBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var defsAnimeLayoutSelect = document.getElementById('defsAnimeLayoutSelect');
	var option;
	if (this.nodeName.toLowerCase() == 'input') { // we are dealing with the button
		var selIndex = userAnimeLayoutSelect.selectedIndex;
		option = userAnimeLayoutSelect.options[selIndex];
	} else // we are dealing with an option
		option = this;
	option.ondblclick = addColToBox;
	defsAnimeLayoutSelect.appendChild(option);
}

/* Moves a Column in the user selected cols up */
function moveColUpBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var selIndex = userAnimeLayoutSelect.selectedIndex;
	if (selIndex < 1) return; // either no selection or selection is the top option
	var curOption = userAnimeLayoutSelect.options[selIndex];
	var prevOption = userAnimeLayoutSelect.options[selIndex-1];
	userAnimeLayoutSelect.insertBefore(curOption,prevOption);
}

/* Moves a Column in the user selected cols down */
function moveColDownBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var selIndex = userAnimeLayoutSelect.selectedIndex;
	if (selIndex < 0 || selIndex > userAnimeLayoutSelect.options.length-1) return; // either no selection or selection is the bottom option
	var curOption = userAnimeLayoutSelect.options[selIndex];
	var nextOption = userAnimeLayoutSelect.options[selIndex+2];
	userAnimeLayoutSelect.insertBefore(curOption,nextOption);

}

/* Resets both cols */
function resetColsBox() {
	var userAnimeLayoutSelect = document.getElementById('userAnimeLayoutSelect');
	var defsAnimeLayoutSelect = document.getElementById('defsAnimeLayoutSelect');
	// first clear them
	while (userAnimeLayoutSelect.childNodes.length) userAnimeLayoutSelect.removeChild(userAnimeLayoutSelect.firstChild);
	while (defsAnimeLayoutSelect.childNodes.length) defsAnimeLayoutSelect.removeChild(defsAnimeLayoutSelect.firstChild);
	var tempArray = new cloneArray(animePage_defLayout);
	// Current User Layout
	//for (var lc = 0; lc < animePage_curLayout.length; lc++) {
	//	var option = animePage_curLayout[lc];
	for (var lc = 0; lc < tempArray.length; lc++) {
		var option = tempArray[lc];
		var newOption = createSelectOption(null, animeFileColsList[option].text, option, false, null, false);
		newOption.ondblclick = remColFromBox;
		userAnimeLayoutSelect.appendChild(newOption);
		//tempArray.splice(tempArray.indexOf(option),1);
	}
/*
	// Layout Columns
	for (var lc = 0; lc < tempArray.length; lc++) {
		var option = tempArray[lc];
		var newOption = createSelectOption(null, animeFileColsList[option].text, option, false, null, false);
		newOption.ondblclick = addColToBox;
		defsAnimeLayoutSelect.appendChild(newOption);
	}
*/
}

/* Function that makes the FileTable Column layout preferences */
function makeLayoutPreferencesTable() {
	var table = document.createElement('table');
	var tbody = document.createElement('tbody');
	var row = document.createElement('tr');
	createHeader(row, null, "Available columns");
	createHeader(row);
	createHeader(row, null, "Selected columns");
	createHeader(row);
	tbody.appendChild(row);
	row = document.createElement('tr');
	// first build users personal layout table
	// reference: animeFileColsList, animePage_defLayout, animePage_curLayout
	var userAnimeLayoutSelect = createBasicSelect('userAnimeLayoutSelect','userAnimeLayoutSelect',null);
	var defsAnimeLayoutSelect = createBasicSelect('defsAnimeLayoutSelect','defsAnimeLayoutSelect',null);
	var tempArray = new cloneArray(animePage_defLayout);
	// Current User Layout
	for (var lc = 0; lc < animePage_curLayout.length; lc++) {
		var option = animePage_curLayout[lc];
		var newOption = createSelectOption(null, animeFileColsList[option].text, option, false, null, false);
		newOption.ondblclick = remColFromBox;
		userAnimeLayoutSelect.appendChild(newOption);
		tempArray.splice(tempArray.indexOf(option),1);
	}
	// Layout Columns
	for (var lc = 0; lc < tempArray.length; lc++) {
		var option = tempArray[lc];
		var newOption = createSelectOption(null, animeFileColsList[option].text, option, false, null, false);
		newOption.ondblclick = addColToBox;
		defsAnimeLayoutSelect.appendChild(newOption);
	}
	userAnimeLayoutSelect.size = defsAnimeLayoutSelect.size = 14;
	// Defaults Cell
	createCell(row, null, defsAnimeLayoutSelect);
	// Switch Boxes Cell
	var cell = document.createElement('td');
	var addCol = createButton('addCol','addCol',false,'>>','button');
	addCol.onclick = addColToBox;
	cell.appendChild(addCol);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	var remCol = createButton('remCol','remCol',false,'<<','button');
	remCol.onclick = remColFromBox;
	cell.appendChild(remCol);
	row.appendChild(cell);
	// User Selects Cell
	createCell(row, null, userAnimeLayoutSelect);
	// Up and Down Cell
	cell = document.createElement('td');
	var upCol = createButton('upCol','upCol',false,'\u2191','button');
	upCol.onclick = moveColUpBox;
	cell.appendChild(upCol);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	var resetCol = createButton('resetCol','resetCol',false,'x','button');
	resetCol.onclick = resetColsBox;
	cell.appendChild(resetCol);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	var downCol = createButton('downCol','downCol',false,'\u2193','button');
	downCol.onclick = moveColDownBox;
	cell.appendChild(downCol);
	row.appendChild(cell);
	tbody.appendChild(row);
	table.appendChild(tbody);
	return table;
}

/* Creates Profile options groups for the customize tab
 * @param parentNode If supplied will append resulting div to given node
 * @param headText The heading text
 * @param options A list with the options
 */
function createProfileGroup(parentNode, headText, options) {
	var div = document.createElement('div');
	var h4 = document.createElement('h3');
	if (headText) {
		if (typeof(headText) == 'string') h4.appendChild(document.createTextNode(headText));
		else h4.appendChild(headText);
		div.appendChild(h4);
	}
	var ul = document.createElement('ul');
	for (var e in options) {
		var option = options[e];
		var li = document.createElement('li');
		if (option['url']) {
			createLink(li, '?', option['url'], 'anidb::wiki', null, null, 'i_inline i_help');
			li.appendChild(document.createTextNode(' '));
		}
		var ckname = option['type']+'.'+option['var'];
		var ckchecked = false;
		if (config && config[option['type']] && config[option['type']][option['var']]) ckchecked = config[option['type']][option['var']];
		createLabledCheckBox(li,ckname,null,null,ckchecked, option['text'], null);
		ul.appendChild(li);
	}
	div.appendChild(ul);
	if (parentNode) parentNode.appendChild(div);
	else return div;
}

