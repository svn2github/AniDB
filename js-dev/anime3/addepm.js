/* Mass Episode scripts
 * @author fahrenheit (alka.setzer@gmail.com)
           some code derived from epoximator work at anidb
 * version 2.1 (09.05.2008) - Initial Release
 */

// GLOBALS //
var uid;						// userID
var mod;						// isMod
var aid;						// AnimeID
var gid;						// GroupID
var anime;						// anime Object (used in animePage)
var animes = new Array();		// stored by aid
var animeOrder = new Array();	// animes ordered in db way (link to aid)
//var groupOrder = new Array();	// ordered group list (filtering porpuses)
var groups = new Array();		// stored by gid
var aGroups = new Array();		// stored by agid (gid to link groups)
var mylist = new Array();		// stored by fid
var episodes = new Array();		// stored in eid
var epOrder = new Array();		// episodes ordered in db way (link to eid)
var files = new Array();		// stored by fid
var hiddenGroups = 0;
// settings
var animeTitleLang = '';
var animeAltTitleLang = 'en';
var episodeTitleLang = '';
var episodeAltTitleLang = 'en';
var episodeTitleDisplay = 2;
var entriesPerPage = 30;
var uriObj = new Array();		// Object that holds the URI
var LAY_FORMATFILESIZE = false;
var LAY_SHOWCRC = false;
var useLangFilters = true;
var filterAudLang = new Array();
var filterSubLang = new Array();
var seeDebug = false;
var seeTimes = false;
var base_url = 'http://static.anidb.net/';
var form = null;
var showTitles = false;
var addRights = false;
var newEpsCnt = 0;
var normalEpsCnt = 0;

/* Function that fetches anime data
 * @param aid Anime ID
 */
function fetchData(aid) {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/aid'+Number(aid)+'.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=anime&aid='+Number(aid), parseData);
}

/* Function that parses xml response
 * @param xmldoc The xml response
 */
function parseData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	if (!root) { if (seeDebug) alert('Error: Could not get root node'); return; }
	updateStatus('Processing anime data...');
	t1 = new Date();
	parseAnimes(root.getElementsByTagName('animes'));
	var parseAnimeNode = (new Date()) - t1;
	var filedataNodes = root.getElementsByTagName('filedata');
	for (var k = 0; k < filedataNodes.length; k++) parseFiledata(filedataNodes[k], aid);
	updateStatus('Processing user data...');
	var t1 = new Date();
	parseCustom(root.getElementsByTagName('custom').item(0));
	var parseCustomNode = (new Date()) - t1;
	if (seeTimes) alert('Processing...\n'+
						'\n\tanimes: '+parseAnimeNode+'ms'+
						'\n\tcustom: '+parseCustomNode+' ms'+
						'\nTotal: '+(parseAnimeNode+parseCustomNode)+' ms');
	for (var i = 0; i < animes[aid].eps; i++) {
		var eid = epOrder[i];
		var episode = episodes[eid];
		if (episode.typeChar == '') normalEpsCnt++;
		else break;
	}
	updateStatus('');
	createEpisodeTable();
}

/* Function that checks data before submit */
function checkData() {
	var errorCnt = 0;
	for (var i = 0; i < epOrder.length; i++) {
		var eid = epOrder[i];
		var episode = episodes[eid];
		// check episode numbers
		var epno = document.getElementById('addepm.'+eid+'.epno');
		var epTypes = [ 's','t','o','p','c' ];
		if (isNaN(epno.value[0]) && epTypes.indexOf(epno.value[0].toLowerCase()) < 0) {
			epno.style.backgroundColor = 'red';
			epno.title = "Wrong episode type.";
			errorCnt++;
		}
		// check lengths
		var eplen = document.getElementById('addepm.'+eid+'.length');
		if (isNaN(eplen.value)) {
			eplen.style.backgroundColor = 'red';
			eplen.title = "Episode length needs to be an integer.";
			errorCnt++;
		}
		// check titles
		for (var t = 0; t < episode.titleLangs; t++) {
			var lang = episode.titleLangs[t];
			var etitle = document.getElementById('addepm.'+eid+'.title'+languageMap[lang]['id']);
			if (!etitle.value.length || etitle.value.length > 1000) {
				etitle.style.backgroundColor = 'red';
				etitle++;
				document.getElementById('e'+eid+'langsTable').style.display = '';
				document.getElementById('e'+eid+'langsSpan').style.display = 'none';
				etitle.title = 'Episode title can not be null and can not be more than 1000 characters long.';
			}
			if ((lang == 'en' || lang == 'x-jat') && (/[^\x20-\x7E]/.test(etitle.value))) {
				etitle.style.backgroundColor = 'red';
				errorCnt++;
				document.getElementById('e'+eid+'langsTable').style.display = '';
				document.getElementById('e'+eid+'langsSpan').style.display = 'none';
				var text = "Only printable 7-bit ASCII is allowed for this title.";
				if (!etitle.title) etitle.title = text;
				else etitle.title += '\n'+text;				
			}
		}
	}
	if (errorCnt) 
		alert('Found '+errorCnt+' error'+(errorCnt > 1 ? 's' : '')+'.\nPlease check your submited data, errors are marked in red.');
}

/* Show/hide the title table where one can edit all the titles */
function toggleEpisodeTitles() {
	for (var eid in episodes) {
		if (!episodes[eid]) continue;
		var table = document.getElementById('e'+eid+'langsTable');
		var span = document.getElementById('e'+eid+'langsSpan');
		showTitles = this.checked;
		if (table) table.style.display = (showTitles ? '' : 'none');
		if (span) span.style.display = (showTitles ? 'none' : '');
	}
}

/* Toogles one episode title display */
function toggleEpTitles() {
	var pnode = this.parentNode;
	var eidnode = pnode;
	var eid = -1;
	while (eidnode.id.indexOf('e') < 0) eidnode = eidnode.parentNode;
	if (eidnode.id.indexOf('langs') >= 0) eid = Number(eidnode.id.substr(1,eidnode.id.indexOf('langs')-1));
	else eid = Number(eidnode.id.substr(1,eidnode.id.indexOf('.')-1));
	if (eid < 0) return;
	var table = document.getElementById('e'+eid+'langsTable');
	var span = document.getElementById('e'+eid+'langsSpan');
	if (pnode.nodeName.toLowerCase() == 'td') {
		table.style.display = 'none';
		span.style.display = '';
	} else {
		table.style.display = '';
		span.style.display = 'none';	
	}
}

/* Sets all episode lengths/dates to match the given ones */
function setAllDefaults() {
	var defLen = document.getElementById('default.len').value;
	var defDate = document.getElementById('default.date').value;
	for (var eid in episodes) {
		if (!episodes[eid]) continue;
		var lenBox = document.getElementById('addepm.'+eid+'.length');
		if (lenBox) lenBox.value = defLen;
		var dateBox = document.getElementById('addepm.'+eid+'.aired');
		if (dateBox) dateBox.value = defDate;
	}
}

/* Create a title row */
function createEpisodeTitleLine(eid,lang,title,update,verify,isUserAdd) {
	var container = document.createElement('tr');
	container.id = 'e'+eid+'.title.'+lang;
	var cell = createCell(null,null,createTextInput('addepm.'+eid+'.title'+languageMap[lang]['id'],50,false,false,255,title));
	cell.appendChild(createTextInput('addepm.'+eid+'.update'+languageMap[lang]['id'],50,false,true,null,update));
	container.appendChild(cell);
	createCell(container, 'icons', createIcon(null, lang, null, toggleEpTitles, 'language: '+mapLanguage(lang), 'i_audio_'+lang));
	var ck = createCheckbox('addepm.'+eid+'.verify'+languageMap[lang]['id'],(Number(verify) ? true : false));
	if (!mod) ck.disabled = true;
	createCell(container, 'verify', ck);
	createCell(container, 'action', (isUserAdd && lang != 'en') ? createIcon(null, 'rem', null, remTitle, 'Remove this title', 'i_minus') : document.createTextNode(' '));
	return(container);
}

/* Function that does the actul title work
 * @param elem Element that triggered the action
 * @param action Action to execute
 */
function titlesActions(elem, action) {
	if (action == 'add') {
		var eid = Number(elem.id.substr(1,elem.id.indexOf('title')-1));
		var lang = document.getElementById('e'+eid+'title.sel').value;
		var tbody = document.getElementById('e'+eid+'langsTable').tBodies[0];
		tbody.appendChild(createEpisodeTitleLine(eid,lang,'',0,0,true));
		episodes[eid].titleLangs.push(lang);
		var sel = document.getElementById('e'+eid+'title.sel');
		sel.removeChild(sel.options[sel.selectedIndex]);
	}
	if (action == 'rem') {
		var pnode = elem.parentNode;
		while (pnode.nodeName.toLowerCase() != 'tr') pnode = pnode.parentNode;
		var eid = pnode.id.substr(1,pnode.id.indexOf('.')-1);
		var lang = pnode.id.substr(pnode.id.lastIndexOf('.')+1,pnode.id.length);
		episodes[eid].titleLangs.splice(episodes[eid].titleLangs.indexOf(lang),1);		
		pnode.parentNode.removeChild(pnode);
		var sel = document.getElementById('e'+eid+'title.sel');
		var option = document.createElement('option');
		var op = languageMap[lang];
		option.text = op['name'];
		option.value = lang;
		sel.appendChild(option);
	}
}

/* Function that removes a title row */
function remTitle() { titlesActions(this,'rem'); }
/* Function that adds a title row */
function addTitle() { titlesActions(this,'add'); }

/* Function that actualy does something with eps
 * @param elem The calling element
 * @param action What action to do
 */
function episodeWork(elem,action) {
	var row = elem.parentNode;
	while (row.nodeName.toLowerCase() != 'tr') row = row.parentNode;
	var rowIndex = row.rowIndex-1;
	var tbody = row.parentNode;
	var eid = Number(row.id.substr(1,row.id.length));
	var curIndex = epOrder.indexOf(eid);
	var maxOrder = epOrder.length;
	var eNoInput = row.cells[0].getElementsByTagName('input')[0];
	var epType = (!isNaN(eNoInput.value[0]) ? '' : eNoInput.value[0]);
	var epNo = Number(epType == '' ? eNoInput.value : eNoInput.value.substr(1,eNoInput.value.length));
	if (action == 'up' && curIndex > 0) {
		var prevRow = tbody.rows[rowIndex-1];
		var neNoInput = prevRow.cells[0].getElementsByTagName('input')[0];
		var nepType = (!isNaN(neNoInput.value[0]) ? '' : neNoInput.value[0]);
		if (nepType != epType) return;
		epOrder.splice(curIndex,1);
		epOrder.splice(curIndex-1,0,eid);
		updateEpisodeRow(row,action);
		updateEpisodeRow(prevRow,'down');
		tbody.insertBefore(row,prevRow);
	}
	if (action == 'down' && curIndex < maxOrder-1) {
		var nextRow = tbody.rows[rowIndex+1];
		var neNoInput = nextRow.cells[0].getElementsByTagName('input')[0];
		var nepType = (!isNaN(neNoInput.value[0]) ? '' : neNoInput.value[0]);
		if (nepType != epType) return;
		epOrder.splice(curIndex,1);
		epOrder.splice(curIndex+1,0,eid);
		updateEpisodeRow(row,action);
		updateEpisodeRow(nextRow,'up');
		rebuildTable(tbody.parentNode);
	}
	if (action == 'add') {
		if (epType == '' && animes[aid].eps && normalEpsCnt >= animes[aid].eps) return;
		var newRow = addEpisodeWork(epType,epNo,curIndex);
		tbody.insertBefore(newRow,tbody.rows[rowIndex+1]);
		updateEpisodeNumbers(curIndex+2,epType,'down');
	}
	if (action == 'rem') {
		newEpsCnt--;
		epOrder.splice(curIndex,1);
		episodes[eid] = null;
		tbody.removeChild(row);
		updateEpisodeNumbers(curIndex,epType,'up')
	}
}

/* Function that realy does all the episode adding work
 * @param epType Episode Type
 * @param epNo Episode Number
 */
function addEpisodeWork(epType,epNo,curIndex) {
	newEpsCnt++;
	// create a new episode node
	var newEp = document.createElement('episode');
	newEp.setAttribute('id','-'+newEpsCnt);
	var epno = document.createElement('epno');
	epno.appendChild(document.createTextNode(epNo+1));
	newEp.appendChild(epno);
	if (epType != '') {
		var flags = document.createElement('flags');
		var flag = 0;
		if (epType == 'S') flag = 1;
		if (epType == 'C') flag = 4;
		if (epType == 'T') flag = 32;
		if (epType == 'P') flag = 64;
		if (epType == 'O') flag = 128;
		flags.appendChild(document.createTextNode(flag));
		newEp.appendChild(flags);
	}
	var titles = document.createElement('titles');
	var title = document.createElement('title');
	title.setAttribute('lang','en');
	title.setAttribute('update','0');
	title.setAttribute('verify','0');
	title.appendChild(document.createTextNode('Episode '+epType+(epNo+1)));
	titles.appendChild(title);
	newEp.appendChild(titles);
	var episodeEntry = new CEpisodeEntry(newEp);
	episodes[episodeEntry.id] = episodeEntry;
	epOrder.splice(curIndex+1,0,episodeEntry.id);
	return(createEpisodeRow(episodeEntry.id));
}

/* Updates episode numbers
 * @param from Index where to start
 * @param type Type of the episodes we are changing eps
 * @param action What action are we doing
 */
function updateEpisodeNumbers(from,type,action) {
	for (var i = from; i < epOrder.length; i++) {
		var eid = epOrder[i];
		var row = document.getElementById('e'+eid);
		var eNoInput = row.cells[0].getElementsByTagName('input')[0];
		var epType = (!isNaN(eNoInput.value[0]) ? '' : eNoInput.value[0]);
		if (epType != type) return;
		var epNo = Number(epType == '' ? eNoInput.value : eNoInput.value.substr(1,eNoInput.value.length));
		if (action == 'up') epNo--;
		if (action == 'down') epNo++;
		eNoInput.value = epType+epNo;
	}
}

/* Updates a row number */
function updateEpisodeRow(row,action) {
	var eid = row.id.substr(1,row.id.length);
	var eNoInput = row.cells[0].getElementsByTagName('input')[0];
	if (!eNoInput) return;
	var epType = (!isNaN(eNoInput.value[0]) ? '' : eNoInput.value[0]);
	var epNo = Number(epType == '' ? eNoInput.value : eNoInput.value.substr(1,eNoInput.value.length));
	if (action == 'up') epNo--;
	if (action == 'down') epNo++;
	eNoInput.value = epType+epNo;
}

/* rebuilds the table */
function rebuildTable(table) {
	var tbody = document.createElement('tbody');
	var oldTbody = table.tBodies[0];
	oldTbody.style.display = 'none';
	for (var i = 0; i < epOrder.length; i++) {
		var eid = epOrder[i];
		var row = document.getElementById('e'+eid);
		tbody.appendChild(row);
	}
	table.replaceChild(tbody,oldTbody);
}

/* Episode Functions */
function moveEpUp() { episodeWork(this,'up'); }
function moveEpDown() { episodeWork(this,'down'); }
function addEp() { episodeWork(this,'add'); }
function remEp() { episodeWork(this,'rem'); }

/* function that adds new episodes */
function addEpisode() {
	var type = document.getElementById('episode.add.sel').value;
	if (type == 'N' && anime.eps != 0 && anime.eps == normalEpsCnt) return;
	if (type == 'N') type = '';
	// start realy work
	var curIndex = -1;
	var foundType = false;
	var epNo = 0;
	for (var i = 0; i < epOrder.length; i++) {
		var eid = epOrder[i];
		var episode = episodes[eid];
		if (episode.typeChar != type) {
			if (!foundType) continue;
			else {
				curIndex = i;
				break;
			}
		} else {
			foundType = true;
			var eNoInput = document.getElementById('addepm.'+eid+'.epno').value;
			epNo = Number(!isNaN(eNoInput[0]) ? eNoInput : eNoInput.substr(1,eNoInput.length));
		}
	}
	var episodeNumber = type+(epNo+1);
	var tbody = document.getElementById('eplist').tBodies[0];
	var nextRow = document.getElementById('e'+epOrder[curIndex]);
	var newRow = addEpisodeWork(type,epNo,curIndex);
	tbody.insertBefore(newRow,nextRow);
	updateEpisodeNumbers(curIndex+2,type,'down');
}

/* Function that creates Episode action icons for a given eid */
function createEpisodeActions(eid) {
	var cell = document.createElement('td');
	cell.className = 'action icons';
	cell.appendChild(createIcon(null, 'up', null, moveEpUp, 'Move Episode up', 'i_up1'));
	cell.appendChild(createIcon(null, 'down', null, moveEpDown, 'Move Episode down', 'i_down1'));
	if (addRights) cell.appendChild(createIcon(null, 'add', null, addEp, 'Add a new Episode', 'i_plus'));
	if (eid < 0) cell.appendChild(createIcon(null, 'rem', null, remEp, 'Remove this new Episode', 'i_minus'));
	return cell;
}

/* function that creates special episode rows */
function createEpisodeRow(eid) {
	var episode = episodes[eid];
	if (!episode) return;
	var row = document.createElement('tr');
	row.id = 'e'+eid;
	var cell = createCell(null, 'epno', createTextInput('addepm.'+eid+'.epno',5,false,false,5,episode.typeChar+episode.epno));
	cell.appendChild(createTextInput('addepm.'+eid+'.update',20,false,true,20,episode.update));
	row.appendChild(cell);
	cell = createCell(null, 'title', null);
	var table = document.createElement('table');
	table.id = 'e'+eid+'langsTable';
	table.style.display = (showTitles ? '' : 'none');
	var thead = document.createElement('thead');
	var theadrow = document.createElement('tr');
	var theadcell = createHeader(null, null, 'Title', null);
	theadcell.colSpan = 2;
	theadrow.appendChild(theadcell);
	createHeader(theadrow, null, 'V', 'Is title verified?');
	createHeader(theadrow, null, 'A', 'Action');
	thead.appendChild(theadrow);
	table.appendChild(thead);
	var tbody = document.createElement('tbody');
	episode.titleLangs = new Array();
	for (var lang in episode.titles) {
		episode.titleLangs.push(lang);
		tbody.appendChild(createEpisodeTitleLine(eid,lang,episode.titles[lang]['title'],episode.titles[lang]['update'],episode.titles[lang]['verify']));
	}
	table.appendChild(tbody);
	var tfoot = document.createElement('tfoot');
	var tfootrow = document.createElement('tr');
	var tfootcell = createCell(null, null, document.createTextNode('Add title: '), null, 4);
	var langsel = createBasicSelect('e'+eid+'title.sel','e'+eid+'title.sel',null);
	for (var lang in languageMap) {
		if (episode.titleLangs.indexOf(lang) >= 0) continue;
		var option = document.createElement('option');
		var op = languageMap[lang];
		option.text = op['name'];
		option.value = lang;
		langsel.appendChild(option);
	}
	tfootcell.appendChild(langsel);
	tfootcell.appendChild(document.createTextNode(' '));
	var addButton = createBasicButton('e'+eid+'title.add','Add');
	addButton.onclick = addTitle;
	tfootcell.appendChild(addButton);
	tfootrow.appendChild(tfootcell);
	tfoot.appendChild(tfootrow);
	table.appendChild(tfoot);
	cell.appendChild(table);
	var container = document.createElement('span');
	container.id = 'e'+eid+'langsSpan';
	container.style.display = (showTitles ? 'none' : '');
	var spanLang = document.createElement('span');
	spanLang.className = 'icons';
	createIcon(spanLang, episodeTitleLang, null, toggleEpTitles, 'language: '+mapLanguage(episodeTitleLang), 'i_audio_'+episodeTitleLang);
	container.appendChild(spanLang);
	var label = document.createElement('label');
	label.appendChild(document.createTextNode(episode.getTitle()));
	container.appendChild(label);
	cell.appendChild(container);
	row.appendChild(cell);
	createCell(row, 'duration', createTextInput('addepm.'+eid+'.length',5,false,false,5,episode.playLength));
	cell = createCell(null, 'date', createTextInput('addepm.'+eid+'.aired',10,false,false,10,cTimeDate(episode.date)));
	if (episode.addDate) cell.title = 'DB add date: '+ episode.addDate;
	row.appendChild(cell);
	createCell(row, 'count files', document.createTextNode(episode.fileCount));
	row.appendChild(createEpisodeActions(eid));
	return row;
}

/* Function that creates the episode table */
function createEpisodeTable() {
	// first of all clear all the old divs
	var epDivs = form.getElementsByTagName('div');
	while(epDivs.length) epDivs[0].parentNode.removeChild(epDivs[0]);
	var table = document.createElement('table');
	table.className = 'eplist';
	table.id = 'eplist';
	var thead = document.createElement('thead');
	var cols = [ 'Ep', 'Titles', 'Len', 'Date', 'Files', 'Action' ];
	var row = document.createElement('tr');
	for (var i = 0; i < cols.length; i++) {
		var cell = document.createElement('th');
		if (i == 2) {
			var abbr = document.createElement('abbr');
			abbr.title = 'Play length';
			abbr.appendChild(document.createTextNode(cols[i]));
			cell.appendChild(abbr);
		} else cell.appendChild(document.createTextNode(cols[i]));
		row.appendChild(cell);
	}
	thead.appendChild(row);
	table.appendChild(thead);
	var tbody = document.createElement('tbody');
	for (var i = 0; i < epOrder.length; i++)
		tbody.appendChild(createEpisodeRow(epOrder[i]));
	table.appendChild(tbody);
	var tfoot = document.createElement('tfoot');
	row = document.createElement('tr');
	createCell(row, 'action', document.createTextNode('Options'));
	var cell = createCell(null, null, createCheckBox(null,null,null,toggleEpisodeTitles,false));
	cell.appendChild(document.createTextNode(' toggle titles'));
	row.appendChild(cell);
	createCell(row, 'duration', createTextInput('default.len',5,false,false,5,null));
	createCell(row, 'date', createTextInput('default.date',10,false,false,10,null));
	var inputButton = createBasicButton(null,'Apply these values');
	inputButton.onclick = setAllDefaults;
	createCell(row, 'action', inputButton, null, 2);
	tfoot.appendChild(row);
	row = document.createElement('tr');
	var cell = createCell(null, null, document.createTextNode('Add new episode of type: '), null, 6);
	var optionArray = { 'N':{'text':"Normal"}, 'S':{'text':"Special"}, 'C':{'text':"Opening/Ending/Credits"},
						'T':{'text':"Trailer/Promo/Ads"}, 'P':{'text':"Parody/Fandub"}, 'O':{'text':"Other Episodes"}};
	createSelectArray(cell,'episode.add.sel','episode.add.sel',null,null,optionArray);
	cell.appendChild(document.createTextNode(' '));
	var addInput = createBasicButton(null,'add');
	addInput.onclick = addEpisode;
	cell.appendChild(addInput);
	row.appendChild(cell);
	tfoot.appendChild(row);
	table.appendChild(tfoot);
	form.insertBefore(table,form.firstChild);
}

function prepPage() {
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	// first of all let's put this in a decent table scheme
	aid = document.getElementById('aid') ? Number(document.getElementById('aid').value) : 0;
	form = document.getElementById('addepm.form');
	addRights = document.getElementById('e0') ? true : false;
	if (!form || !aid) return;
	fetchData(aid);
}

// hook up the window onload event
addLoadEvent(prepPage);