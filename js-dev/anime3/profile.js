/* file profile page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *
 * version 1.0 (19.05.2007) - Initial release
 * version 1.1 (29.07.2008) - Added Javascript Tab
 * version 1.2 (03.02.2009)	- Move some stuff out of this script to customize
 */
jsVersionArray.push({
	"file":"anime3/profile.js",
	"version":"1.2",
	"revision":"$Revision: 2923 $",
	"date":"$Date:: 2009-08-04 00:34:35 +0100#$",
	"author":"$Author: fahrenheit $",
	"changelog":"Move some stuff out of this script to customize"
});

// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var cssurl;
var current_css = "";
var style_index = 0;
var base_url = "http://static.anidb.net/";
var styleList = new Array();   // Stylelist array
var style_thumbnail, style_title, style_creator, style_status, style_updated, style_description;
var style_apply, style_next, style_prev, style_chooser;
var audlangs = new Object();
var sublangs = new Object();

/* CSS STYLE STUFF */
function fetchData() {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') { base_url = ""; xhttpRequestFetch(req, "xml/stylelist.xml", parseData); }
	else { base_url = "http://static.anidb.net/"; xhttpRequestFetch(req, "../css/stylelist.xml", parseData); }
}

function CStyle(node) {
	this.name = node.getAttribute('name');
	this.title = this.name;
	this.screenshot = "";
	this.thumbnail = "";
	this.path = "";
	this.creator = "";
	this.update = "";
	this.status = "";
	this.description = "";
	for (var i = 0; i < node.childNodes.length; i++) {
		var sNode = node.childNodes.item(i);
		if (sNode.nodeType == 3) continue; // Text node, not interested
		switch (sNode.nodeName) {
			case 'title': this.title = nodeData(sNode); break;
			case 'screenshot': this.screenshot = nodeData(sNode); break;
			case 'thumbnail': this.thumbnail = nodeData(sNode); break;
			case 'path': this.path = nodeData(sNode); break;
			case 'creator': this.creator = nodeData(sNode); break;
			case 'update': this.update = nodeData(sNode); break;
			case 'status': this.status = nodeData(sNode); break;
			case 'description': this.description = nodeData(sNode); break;
			default: showAlert('Style entry for: '+this.name, node.nodeName, node.nodeName,sNode.nodeName);
		}
	}
}

function clearImg() {
	this.onerror = null;
	this.src = base_url+"css/nopic.png";
	return;
}

function updateCurrentStyle() {
	var curStyle = styleList[style_index];
	style_chooser.value = style_index;
	// Thumbnail
	if (!style_thumbnail) style_thumbnail = document.getElementById('style_thumbnail');
	style_thumbnail.title = "Style: "+curStyle.name;
	style_thumbnail.alt = curStyle.name;
	//style_thumbnail.onerror = clearImg;
	if (curStyle.thumbnail != "none") {
		if (curStyle.thumbnail == "") style_thumbnail.src = base_url+"css/"+curStyle.name+"/images/thumbnail.png";
		else style_thumbnail.src = base_url+"css/"+curStyle.thumbnail;
	} else style_thumbnail.src = base_url+"css/nopic.png";
	var i_enlarge = document.getElementById('style_enlarge');
	if (curStyle.screenshot != "none") {
		if (!i_enlarge) {
			i_enlarge = document.createElement('A');
			i_enlarge.className = "i_icon i_enlarge";
			i_enlarge.title = "Show screenshot";
			i_enlarge.alt = "show screenshot";
			i_enlarge.target = "blank";
			i_enlarge.id = 'style_enlarge';
			style_thumbnail.parentNode.appendChild(i_enlarge);
		}
		if (curStyle.screenshot == "") i_enlarge.href = base_url+"css/"+curStyle.name+"/images/screenshot.png";
		else i_enlarge.href = base_url+"css/"+curStyle.screenshot;
	}
	// Title
	if (!style_title) style_title = document.getElementById('style_title');
	while (style_title.childNodes.length) { style_title.removeChild(style_title.childNodes[0]); }
	if (curStyle.title != "") style_title.appendChild(document.createTextNode(curStyle.title));
	else style_title.appendChild(document.createTextNode(curStyle.name));
	// Creator
	if (!style_creator) style_creator = document.getElementById('style_creator');
	while (style_creator.childNodes.length) { style_creator.removeChild(style_creator.childNodes[0]); }
	if (curStyle.creator != "") style_creator.appendChild(document.createTextNode(curStyle.creator));
	else style_creator.appendChild(document.createTextNode('N/A'));
	// Status
	if (!style_status) style_status = document.getElementById('style_status');
	while (style_status.childNodes.length) style_status.removeChild(style_status.childNodes[0]);
	if (curStyle.status != "") style_status.appendChild(document.createTextNode(curStyle.status));
	else style_status.appendChild(document.createTextNode('N/A'));
	// Update
	if (!style_updated) style_updated = document.getElementById('style_updated');
	while (style_updated.childNodes.length) style_updated.removeChild(style_updated.childNodes[0]);
	if (curStyle.update != "") style_updated.appendChild(document.createTextNode(curStyle.update));
	else style_updated.appendChild(document.createTextNode('N/A'));
	// Description
	if (!style_description) style_description = document.getElementById('style_description');
	while (style_description.childNodes.length) style_description.removeChild(style_description.childNodes[0]);
	if (curStyle.description != "") style_description.appendChild(document.createTextNode(curStyle.description));
	else style_description.appendChild(document.createTextNode('N/A'));
}

function changeStyle() {
	// if (style_index == 0) cssurl.style.display = "";
	// else cssurl.style.display = "none";
	var curStyle = styleList[style_index];
	if (curStyle.path != "none") {
		if (curStyle.path == "") cssurl.value = base_url+"css"+((""+window.location.hostname.indexOf('dev.anidb') >= 0) ? "-dev" : "")+"/"+curStyle.name+"/"+curStyle.name+".css";
		else cssurl.value = curStyle.path;
	} else cssurl.value = "";
	updateCurrentStyle();
}

function stylePrev() {
	if (style_index > 0) style_index--;
	if (style_index == 0) style_prev.disabled = true;
	if (style_index < styleList.length-1) style_next.disabled = false;
	changeStyle();
}

function styleNext() {
	if (style_index < styleList.length-1) style_index++;
	if (style_index == styleList.length-1) style_next.disabled = true;
	if (style_index > 0) style_prev.disabled = false;
	changeStyle();
}

function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('css_styles').item(0);
	var t1 = new Date();
	var styleEntries = (root) ? root.getElementsByTagName('style') : new Array();;
	// the first entry is the custom node
	var optionArray = new Object();
	var scustom = new Object();
	scustom.name = "custom";
	scustom.title = "Custom/Default";
	scustom.path = "none";
	scustom.thumbnail = "none";
	scustom.screenshot = "none";
	scustom.creator = "N/A"
	scustom.update = "N/A";
	scustom.status = "N/A";
	scustom.description = "Provide a url to a custom style sheet or leave it blank if you wish to use the default style.";
	optionArray[styleList.length] = {'text':"custom" };
	styleList.push(scustom);
	for (var i = 0; i < styleEntries.length; i++) {
		var styleEntry = new CStyle(styleEntries[i]);
		if (styleEntry.path != "none") {
			if (styleEntry.path == "") {
				if (styleEntry.name.toLowerCase() == current_css.toLowerCase()) style_index = i+1;
			} else {
				if (styleEntry.name.toLowerCase().indexOf(current_css.toLowerCase()) >= 0) style_index = i+1;
			}
		} // we just got the style the user is using (at least i hope we got it)
		optionArray[styleList.length] = {'text':styleEntry.name.toLowerCase()};
		styleList.push(styleEntry);
	}
	// FUCK IE //
	if (!style_prev) style_prev = document.getElementById('style_prev');
	if (!style_next) style_next = document.getElementById('style_next');
	if (style_index > 0) style_prev.disabled = false;
	if (style_index < styleList.length-1) style_next.disabled = false;
	style_next.onclick = styleNext;
	style_prev.onclick = stylePrev;
	style_chooser = createSelectArray(null,null,"profile.style_chooser",selectStyle,0,optionArray);
	style_next.parentNode.insertBefore(style_chooser,style_next);
	style_next.parentNode.insertBefore(document.createTextNode(' '),style_next);
	updateCurrentStyle();
}

function selectStyle() {
	style_index = this.value;
	changeStyle();
}

/* LANGUAGE STUFF */

/* Adds a language
 * @param value Language to add
 */
function addLanguageToArray(value) {
	if (this.array.indexOf(value) < 0) this.array.push(value);
}
/* Removes a language
 * @param value Language to remove
 */
function remLanguageFromArray(value) {
	if (this.array.indexOf(value) >= 0)
		this.array.splice(this.array.indexOf(value),1);
}

/* Converts a language array to text */
function convLanguagesToText() {
	this.text = '{'+this.array.join(',')+'}';
	this.input.value = this.text;
}

/* Function that does some changes to the way anidb handles languages */
function prepLanguages() {
	var form = audlangs.input.form;
	// remove the boxes things
	var audioDiv = getElementsByClassName(form.getElementsByTagName('div'), 'audio', false)[0];
	var subtitlesDiv = getElementsByClassName(form.getElementsByTagName('div'), 'subtitles', false)[0];
	if (audioDiv) {
		var baseSelect = audioDiv.getElementsByTagName('select')[0];
		audlangs.select = new Array();
		audlangs.select[0] = createBasicSelect('audioLangSelect0','audioLangSelect0',null);
		audlangs.select[1] = createBasicSelect('audioLangSelect1','audioLangSelect1',null);
		if (baseSelect) {
			for (var i = 0; i < baseSelect.options.length; i++) {
				var option = baseSelect.options[i];
				if (audlangs.array.indexOf(option.value) >= 0) {
					var newOption = createSelectOption(null, option.text, option.value, false, null, false);
					newOption.ondblclick = remLangFromBox;
					audlangs.select[1].appendChild(newOption);
				} else {
					var newOption = createSelectOption(null, option.text, option.value, false, null, false);
					newOption.ondblclick = addLangToBox;
					audlangs.select[0].appendChild(newOption);
				}
			}
		}
		audlangs.select[0].size = audlangs.select[1].size = 8;
		audlangs.select[0].className = audlangs.select[1].className = 'shuttle';
		audioDiv.parentNode.removeChild(audioDiv);
	}
	if (subtitlesDiv) {
		var baseSelect = subtitlesDiv.getElementsByTagName('select')[0];
		sublangs.select = new Array();
		sublangs.select[0] = createBasicSelect('subtitleLangSelect0','subtitleLangSelect0',null);
		sublangs.select[1] = createBasicSelect('subtitleLangSelect1','subtitleLangSelect1',null);
		if (baseSelect) {
			for (var i = 0; i < baseSelect.options.length; i++) {
				var option = baseSelect.options[i];
				if (sublangs.array.indexOf(option.value) >= 0) {
					var newOption = createSelectOption(null, option.text, option.value, false, null, false);
					newOption.ondblclick = remLangFromBox;
					sublangs.select[1].appendChild(newOption);
				} else {
					var newOption = createSelectOption(null, option.text, option.value, false, null, false);
					newOption.ondblclick = addLangToBox;
					sublangs.select[0].appendChild(newOption);
				}
			}
		}
		sublangs.select[0].size = sublangs.select[1].size = 8;
		sublangs.select[0].className = sublangs.select[1].className = 'shuttle';
		subtitlesDiv.parentNode.removeChild(subtitlesDiv);
	}
	var langsDiv = document.createElement('div');
		langsDiv.className = "block settings languages";
	var h4 = document.createElement('h4')
	h4.appendChild(document.createTextNode('Languages'));
	langsDiv.appendChild(h4);
	var table = document.createElement('table');
	var tbody = document.createElement('tbody');
	var row = document.createElement('tr');
	var optionArray = { 0 : {"text":'Audio Languages'} , 1 : {"text":'Subtitle Languages'}};
	createCell(row, null, createSelectArray(null,'languagesTypeSelect','languagesTypeSelect',changeLangType,0,optionArray), null, 3);
	tbody.appendChild(row);
	row = document.createElement('tr');
	createHeader(row, null, 'Available');
	createHeader(row, null, null);
	createHeader(row, null, 'Selected');
	tbody.appendChild(row);
	row = document.createElement('tr');
	audlangs.row = row;
	createCell(row, null, audlangs.select[0]);
	var cell = document.createElement('td');
	var addLang = createButton('addAudLang','addAudLang',false,'>>','button');
	addLang.onclick = addLangToBox;
	cell.appendChild(addLang);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	var remLang = createButton('remAudLang','remAudLang',false,'<<','button');
	remLang.onclick = remLangFromBox;
	cell.appendChild(remLang);
	row.appendChild(cell);
	createCell(row, null, audlangs.select[1]);
	tbody.appendChild(row);
	row = document.createElement('tr');
	row.style.display = 'none';
	sublangs.row = row;
	createCell(row, null, sublangs.select[0], null, 1);
	cell = document.createElement('td');
	addLang = createButton('addSubLang','addSubLang',false,'>>','button');
	addLang.onclick = addLangToBox;
	cell.appendChild(addLang);
	cell.appendChild(document.createElement('br'));
	cell.appendChild(document.createElement('br'));
	remLang = createButton('remSubLang','remSubLang',false,'<<','button');
	remLang.onclick = remLangFromBox;
	cell.appendChild(remLang);
	row.appendChild(cell);
	createCell(row, null, sublangs.select[1]);
	tbody.appendChild(row);
	table.appendChild(tbody);
	langsDiv.appendChild(table);

	var langContainer = getElementsByClassNameDeep(form, 'languages')[0];
	langContainer.parentNode.insertBefore(langsDiv, langContainer.nextSibling);
}

function changeLangType() {
	if (this.value == 0) {
		audlangs.row.style.display = '';
		sublangs.row.style.display = 'none';
	} else {
		audlangs.row.style.display = 'none';
		sublangs.row.style.display = '';
	}
}

function addLangToBox() {
	var type = 'aud';
	if (this.nodeName.toLowerCase() == 'input') type = (this.id.indexOf('Aud') > 0) ? 'aud' : 'sub';
	else type = (this.parentNode.id.indexOf('audio') >= 0) ? 'aud' : 'sub';
	if (type == 'aud') {
		if (audlangs.select[0].value == '') return;
		var selectedId = audlangs.select[0].selectedIndex;
		if (audlangs.select[0].value != '0') {
			audlangs.add(audlangs.select[0].value);
			if (audlangs.array.indexOf('0') >= 0) {
				audlangs.rem('0');
				for (var i = 0; i < audlangs.select[1].options.length; i++) {
					if (audlangs.select[1].options[i].value != 0) continue;
					var newOption = audlangs.select[1].options[i];
					newOption.ondblclick = addLangToBox;
					audlangs.select[0].appendChild(newOption);
					break;
				}
			}
			var newOption = audlangs.select[0].options[selectedId];
			newOption.ondblclick = remLangFromBox;
			audlangs.select[1].appendChild(newOption);
		} else { // we are adding the no language lang, we need to remove all other langs
			while(audlangs.select[1].options.length) {
				var newOption = audlangs.select[1].options[0];
				newOption.ondblclick = addLangToBox;
				audlangs.select[0].appendChild(newOption);
			}
			var newOption = audlangs.select[0].options[selectedId];
			newOption.ondblclick = remLangFromBox;
			audlangs.select[1].appendChild(newOption);
			audlangs.array = [ '0' ];
		}
	} else {
		if (sublangs.select[0].value == '') return;
		var selectedId = sublangs.select[0].selectedIndex;
		if (sublangs.select[0].value != '0') {
			sublangs.add(sublangs.select[0].value);
			if (sublangs.array.indexOf('0') >= 0) {
				sublangs.rem('0');
				for (var i = 0; i < sublangs.select[1].options.length; i++) {
					if (sublangs.select[1].options[i].value != 0) continue;
					var newOption = sublangs.select[1].options[i];
					newOption.ondblclick = addLangToBox;
					sublangs.select[0].appendChild(newOption);
					break;
				}
			}
			var newOption = sublangs.select[0].options[selectedId];
			newOption.ondblclick = remLangFromBox;
			sublangs.select[1].appendChild(newOption);
		} else { // we are adding the no language lang, we need to remove all other langs
			var selectedId = sublangs.select[0].selectedIndex;
			while(sublangs.select[1].options.length) {
				var newOption = sublangs.select[1].options[0];
				newOption.ondblclick = addLangToBox;
				sublangs.select[0].appendChild(newOption);
			}
			var newOption = sublangs.select[0].options[selectedId];
			newOption.ondblclick = remLangFromBox;
			sublangs.select[1].appendChild(newOption);
			sublangs.array = [ '0' ];
		}
	}
	audlangs.toString();
	sublangs.toString();
}

function remLangFromBox() {
	var type = 'aud';
	if (this.nodeName.toLowerCase() == 'input') type = (this.id.indexOf('Aud') > 0) ? 'aud' : 'sub';
	else type = (this.parentNode.id.indexOf('audio') >= 0) ? 'aud' : 'sub';
	if (type == 'aud') {
		if (audlangs.select[1].value == '') return;
		if (audlangs.array.length == 1 && audlangs.array[0] == '0') return;
		audlangs.rem(audlangs.select[1].value);
		var newOption = audlangs.select[1].options[audlangs.select[1].selectedIndex];
		newOption.ondblclick = addLangToBox;
		audlangs.select[0].appendChild(newOption);
		if (!audlangs.select[1].options.length) { // clean
			for (var i = 0; i < audlangs.select[0].options.length; i++) {
				if (audlangs.select[0].options[i].value != 0) continue;
				var newOption = audlangs.select[0].options[i];
				newOption.ondblclick = remLangFromBox;
				audlangs.select[1].appendChild(newOption);
				break;
			}
			audlangs.array = [ '0' ];
		}
	} else {
		if (sublangs.select[1].value == '') return;
		if (sublangs.array.length == 1 && sublangs.array[0] == '0') return;
		sublangs.rem(sublangs.select[1].value);
		var newOption = sublangs.select[1].options[sublangs.select[1].selectedIndex];
		newOption.ondblclick = addLangToBox;
		sublangs.select[0].appendChild(newOption);
		if (!sublangs.select[1].options.length) { // clean
			for (var i = 0; i < sublangs.select[0].options.length; i++) {
				if (sublangs.select[0].options[i].value != 0) continue;
				var newOption = sublangs.select[0].options[i];
				newOption.ondblclick = remLangFromBox;
				sublangs.select[1].appendChild(newOption);
				break;
			}
			sublangs.array = [ '0' ];
		}
	}
	audlangs.toString();
	sublangs.toString();
}

function prepPage() {
	uriObj = parseURI(); // update the uriObj
	fetchData(); // Fetch Data, we update the page in the meanwhile
	cssurl = document.getElementById('style_url');
	if (!cssurl) return;
	current_css = cssurl.value.substring(cssurl.value.lastIndexOf('/')+1,cssurl.value.indexOf('.css'));
	// cssurl.style.display = "none";
	// more stuff
	style_thumbnail = document.getElementById('style_thumbnail');
	style_title = document.getElementById('style_title');
	style_creator = document.getElementById('style_creator');
	style_status = document.getElementById('style_status');
	style_updated = document.getElementById('style_updated');
	style_description = document.getElementById('style_description');
	style_apply = document.getElementById('style_apply');
	style_next = document.getElementById('style_next');
	style_prev = document.getElementById('style_prev');
	var inputs = document.getElementsByTagName('input');
	audlangs.input = getElementsByName(inputs, 'lang.audin', false)[0];
	if (audlangs.input) {
		audlangs.text = audlangs.input.value;
		audlangs.array = audlangs.text.replace(/[{}]/mgi,'').split(',');
		if (audlangs.array.length == 1 && !audlangs.array[0].length) audlangs.array = new Array();
		if (!audlangs.array.length) audlangs.array.push('0');
		audlangs.add = addLanguageToArray;
		audlangs.rem = remLanguageFromArray;
		audlangs.toString = convLanguagesToText;
		audlangs.input.name = 'lang.filealang';
		audlangs.input = audlangs.input.form.getElementsByTagName('fieldset')[0].appendChild(
			audlangs.input.parentNode.removeChild(audlangs.input));
	}
	sublangs.input = getElementsByName(inputs, 'lang.subin', false)[0];
	if (sublangs.input) {
		sublangs.text = sublangs.input.value;
		sublangs.array = sublangs.text.replace(/[{}]/mgi,'').split(',');
		if (sublangs.array.length == 1 && !sublangs.array[0].length) sublangs.array = new Array();
		if (!sublangs.array.length) sublangs.array.push('0');
		sublangs.add = addLanguageToArray;
		sublangs.rem = remLanguageFromArray;
		sublangs.toString = convLanguagesToText;
		sublangs.input.name = 'lang.fileslang';
		sublangs.input = sublangs.input.form.getElementsByTagName('fieldset')[0].appendChild(
			sublangs.input.parentNode.removeChild(sublangs.input));
	}
	if (audlangs.input && sublangs.input) prepLanguages();

	// Show everything JS related profile option
	var js_hidden = getElementsByClassNameDeep(document.getElementById('layout-main'), "javascript");
	for (var i = 0; i < js_hidden.length; i++) {
		var classes = js_hidden[i].className.split(' ');
		for (var j = 0; j < classes.length; j++) {
			if (classes[j] == "hidden") {
				classes.splice(j, 1);
				js_hidden[i].className = classes.join(' ');
				break;
			}
		}
	}

	// Make shuttles where necessary
	var shuttles = getElementsByClassName(document.getElementsByTagName("table"), "shuttle");
	for (var i = 0; i < shuttles.length; i++) {
		createShuttle(shuttles[i]);
	}

}

// Creates shuttle
function createShuttle(target) {
	if (!target) {
		return;
	}

	var select_boxes     = getElementsByClassNameDeep(target, "shuttle");
	var select_available = select_boxes[0];
	var select_selected  = select_boxes[1];

	select_available.getSelected = select_selected.getSelectedIndex = function() {
		var options = this.getElementsByTagName("option");
		for (var i = 0; i < options.length; i++) {
			if (options[i].selected) {
				return i;
			}
		}
	}
	select_available.getSelected = select_selected.getSelected = function() {
		var options = this.getElementsByTagName("option");
		for (var i = 0; i < options.length; i++) {
			if (options[i] && options[i].selected) {
				return options[i];
			}
		}
	}

	// Get global data
	var fieldset = target.getElementsByTagName("fieldset")[0];
	var name     = fieldset.getElementsByTagName("input")[0].name.substr(5);

	// Binding to update input fields, to submit something useful
	select_selected.updateInputs = function() {
		// Delete old inputs, but leave the config name input
		var inputs = fieldset.getElementsByTagName("input");
		for (var i = inputs.length - 1; i > 0; i--) {
			inputs[i].parentNode.removeChild(inputs[i]);
		}

		// Re-add options
		var options = select_selected.getElementsByTagName("option");
		for (var i = 0; i < options.length; i++) {
			var input = document.createElement("input");
				input.type  = "hidden";
				input.name  = name + "[]";
				input.value = options[i].value;
			fieldset.appendChild(input);
		}
	}

	// Bind doubleclick event to all options and remember order
	var options = target.getElementsByTagName("option");
	var order   = [];
	for (var i = 0; i < options.length; i++) {
		if (options[i].parentNode == select_selected) {
			order.push(options[i].value);
		}
		addEventSimple(options[i], "dblclick", function() {
			if (this.parentNode == select_selected) {
				select_available.appendChild(this.parentNode.removeChild(this));
			} else {
				select_selected.appendChild(this.parentNode.removeChild(this));
			}
			select_selected.updateInputs();
		});
	}

	// Get all buttons, to bind events to
	var inputs  = target.getElementsByTagName("input");
	var buttons = { };
	for (var i = 0; i < inputs.length; i++) {
		if (inputs[i].type == "button") {
			buttons[inputs[i].name] = inputs[i];
		}
	}

	// Attach move button functions
	addEventSimple(buttons["add"], "click", function() {
		var option = select_available.getSelected();
		if (!option) {
			return;
		}
		select_selected.appendChild(option.parentNode.removeChild(option));
		select_selected.updateInputs();
	});
	addEventSimple(buttons["remove"], "click", function() {
		var option = select_selected.getSelected();
		if (!option) {
			return;
		}
		option = option.parentNode.removeChild(option);
		option.selected = false;
		select_available.appendChild(option);
		select_selected.updateInputs();
	});
	addEventSimple(buttons["moveup"], "click", function() {
		var option = select_selected.getSelected();
		if (!option) {
			return;
		}
		var options = select_selected.getElementsByTagName("option");
		var i = select_selected.getSelectedIndex();

		if (i == null || i == 0) {
			return;
		} else {
			select_selected.insertBefore(option.parentNode.removeChild(option), options[i - 1]);
		}
		select_selected.updateInputs();
	});
	addEventSimple(buttons["movedown"], "click", function() {
		var option = select_selected.getSelected();
		if (!option) {
			return;
		}
		var options = select_selected.getElementsByTagName("option");
		var i = select_selected.getSelectedIndex();

		if (i == null) {
			return;
		} else if (i + 2 >= options.length) {
			select_selected.appendChild(option.parentNode.removeChild(option));
		} else {
			select_selected.insertBefore(option.parentNode.removeChild(option), options[i + 1]);
		}
		select_selected.updateInputs();
	});
	addEventSimple(buttons["reset"], "click", function() {
		for (var i = 0; i < options.length; i++) {
			if (options[i].parentNode == select_selected) {
				select_available.appendChild(options[i].parentNode.removeChild(options[i]));
			}
		}
		for (var i = 0; i < order.length; i++) {
			for (var j = 0; j < options.length; j++) {
				if (order[i] == options[j].value) {
					select_selected.appendChild(options[j].parentNode.removeChild(options[j]));
					break;
				}
			}
		}
		select_selected.updateInputs();
	});
}

addLoadEvent(prepPage);
