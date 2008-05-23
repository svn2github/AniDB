/* Mass Episode scripts
 * @author fahrenheit (alka.setzer@gmail.com)
           some code derived from epoximator work at anidb
 * version 2.0 (23.05.2008) - Reworked version
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
var errorCount = 0;

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

// EPISODE WORK FUNCTIONS //

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
	var episode = episodes[eid];
	var epNo = Number(episode.typeChar == '' ? episode.epno : episode.inputs['epno'].value.substr(1,episode.inputs['epno'].value.length));
	var curIndex = epOrder.indexOf(eid);
	if (action == 'up') {
		if (!episode.prevEp || episode.prevEp.typeChar != episode.typeChar) return;
		episode.inputs['epno'].value = episode.epno = episode.typeChar + (epNo-1);
		episode.prevEp.inputs['epno'].value = episode.prevEp.epno = episode.prevEp.typeChar + epNo;
		tbody.insertBefore(row,episode.prevEp.epRow);
		var auxNext = episode.nextEp;
		var auxPrev = episode.prevEp;
		if (auxNext) auxNext.prevEp = auxPrev;
		if (auxPrev) auxPrev.nextEp = auxNext;
		episode.nextEp = auxPrev;
		if (auxPrev.prevEp) auxPrev.prevEp.nextEp = episode;
		episode.prevEp = auxPrev.prevEp;
		if (auxPrev) auxPrev.prevEp = episode;
		epOrder.splice(curIndex,1);
		epOrder.splice(curIndex-1,0,eid);
	}
	if (action == 'down') {
		if (!episode.nextEp || episode.nextEp.typeChar != episode.typeChar) return;
		episode.inputs['epno'].value = episode.epno = episode.typeChar + (epNo+1);
		episode.nextEp.inputs['epno'].value = episode.nextEp.epno = episode.nextEp.typeChar + epNo;
		tbody.insertBefore(episode.nextEp.epRow,row);
		var auxNext = episode.nextEp;
		var auxPrev = episode.prevEp;
		if (auxPrev) auxPrev.nextEp = auxNext;
		if (auxNext) auxNext.prevEp = auxPrev;
		episode.prevEp = auxNext;
		if (auxNext.nextEp) auxNext.nextEp.prevEp = episode;
		episode.nextEp = auxNext.nextEp;
		if (auxNext) auxNext.nextEp = episode;
		epOrder.splice(curIndex,1);
		epOrder.splice(curIndex+1,0,eid);
	}
	if (action == 'add') {
		if (episode.typeChar == '' && anime.eps && normalEpsCnt >= anime.eps) return;
		var newEp = createNewEpisode(episode.typeChar,epNo);
		var newRow = createEpisodeRow('-'+newEpsCnt,newEp);
		newEp.epRow = newRow;
		if (episode.nextEp) tbody.insertBefore(newRow,episode.nextEp.epRow);
		else tbody.appendChild(newRow);
		newEp.prevEp = episode;
		newEp.nextEp = episode.nextEp;
		if (episode.nextEp) {
			episode.nextEp.prevEp = newEp;
			episode.nextEp = newEp;
		} else episode.nextEp = newEp;
		updateEpisodeNumbers(newEp.nextEp,episode.typeChar,'down');
		epOrder.splice(curIndex+1,0,newEp.id);
	}
	if (action == 'rem') {
		newEpsCnt--;
		var auxNext = episode.nextEp;
		var auxPrev = episode.prevEp;
		if (auxPrev) auxPrev.nextEp = auxNext;
		if (auxNext) auxNext.prevEp = auxPrev;
		episodes[eid] = null;
		tbody.removeChild(row);
		updateEpisodeNumbers(auxNext,episode.typeChar,'up');
		epOrder.splice(curIndex,1);
	}
}

/* Updates episode numbers
 * @param from Episode where to start
 * @param type Type of the episodes we are changing eps
 * @param action What action are we doing
 */
function updateEpisodeNumbers(episode,type,action) {
	while (episode && episode.typeChar == type) {
		var diff = (action == 'up' ? -1 : 1);
		var epNo = Number(episode.typeChar == '' ? episode.epno : episode.inputs['epno'].value.substr(1,episode.inputs['epno'].value.length));
		episode.inputs['epno'].value = episode.epno = episode.typeChar + (epNo+diff);
		episode = episode.nextEp;
	}
}

/* Function that creates a new Episode entry
 * @param epType Episode Type
 * @param epNo Episode Number
 */
function createNewEpisode(epType,epNo) {
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
	return(episodeEntry);
}

function moveEpUp() { episodeWork(this,'up'); }
function moveEpDown() { episodeWork(this,'down'); }
function addEp() { episodeWork(this,'add'); }
function remEp() { episodeWork(this,'rem'); }

/* Function that sets defaults for episode inputs */
function setAllDefaults() {
	var defLen = document.getElementById('default.len').value;
	var defDate = document.getElementById('default.date').value;
	if (!defLen.length && !defDate.length) return;
	for (var eid in episodes) {
		if (!episodes[eid]) continue;
		if (defLen.length) {
			var lenBox = document.getElementById('addepm.'+eid+'.length');
			if (lenBox) lenBox.value = defLen;
		}
		if (defDate.length) {
			var dateBox = document.getElementById('addepm.'+eid+'.aired');
			if (dateBox) dateBox.value = defDate;
		}
	}
}

/* function that adds episodes */
function addEpisode() {
	var type = document.getElementById('episode.add.sel').value;
	if (type == 'N' && anime.eps != 0 && anime.eps == normalEpsCnt) return;
	if (type == 'N') type = '';
	// start real work
	var curIndex = -1;
	var foundType = false;
	var epNo = 0;
	var eid;
	var episode;
	for (var i = 0; i < epOrder.length; i++) {
		eid = epOrder[i];
		episode = episodes[eid];
		if (episode.typeChar != type) {
			if (!foundType) continue;
			else {
				curIndex = i;
				episode = episode.prevEp;
				break;
			}
		} else {
			foundType = true;
			var eNoInput = episode.inputs['epno'].value;
			epNo = Number(!isNaN(eNoInput[0]) ? eNoInput : eNoInput.substr(1,eNoInput.length));
		}
	}
	var newEp = createNewEpisode(type,epNo);
	var newRow = createEpisodeRow('-'+newEpsCnt,newEp);
	newEp.epRow = newRow;
	var tbody = document.getElementById('eplist').tBodies[0];
	if (episode.nextEp) tbody.insertBefore(newRow,episode.nextEp.epRow);
	else tbody.appendChild(newRow);
	newEp.prevEp = episode;
	newEp.nextEp = (episode ? episode.nextEp : null);
	if (episode && episode.nextEp) {
		episode.nextEp.prevEp = newEp;
		episode.nextEp = newEp;
	} else episode.nextEp = newEp;
	updateEpisodeNumbers(newEp.nextEp,type,'down');
	if (curIndex >= 0) epOrder.splice(curIndex+1,0,newEp.id);
	else epOrder.push(newEp.id);
}

// EPISODE TITLE FUNCTIONS //

/* Function that toggles all episode titles display */
function toggleEpisodeTitles() {
	showTitles = this.checked;
	for (var eid in episodes) {
		if (!episodes[eid]) continue;
		var table = document.getElementById('e'+eid+'langsTable');
		var span = document.getElementById('e'+eid+'langsSpan');
		if (table) table.style.display = (showTitles ? '' : 'none');
		if (span) span.style.display = (showTitles ? 'none' : '');
	}
}

/* Function that toggles titles for a specific episode */
function toggleEpTitles() {
	var pnode = this.parentNode;
	var eidnode = pnode;
	var eid = 0;
	while (eidnode.id.indexOf('e') < 0) eidnode = eidnode.parentNode;
	if (eidnode.id.indexOf('langs') >= 0) eid = Number(eidnode.id.substr(1,eidnode.id.indexOf('langs')-1));
	else eid = Number(eidnode.id.substr(1,eidnode.id.indexOf('.')-1));
	if (eid == 0) return;
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

/* Function that does the actul title work
 * @param elem Element that triggered the action
 * @param action Action to execute
 */
function titlesActions(elem, action) {
	if (action == 'add') {
		var eid = Number(elem.id.substr(1,elem.id.indexOf('title')-1));
		var lang = document.getElementById('e'+eid+'title.sel').value;
		var tbody = document.getElementById('e'+eid+'langsTable').tBodies[0];
		tbody.appendChild(createEpisodeTitleLine(eid,lang,languageMap[lang]['name'] + ' title',0,0,true));
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

// DISPLAY FUNCTIONS //

/* Create a title row
 * @param eid Episode ID
 * @param lang Title language
 * @param title Actual title
 * @param update Update date
 * @param verify Verify date
 * @param isUserAdd indicates if a user has added this title in this page
 */
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

/* Function that creates episode titles and assorted stuff
 * @param eid Episode ID
 * @param episode Episode object [optional]
 */
function createEpisodeTitles(eid,episode) {
	if (!episode) episode = episodes[eid];
	var cell = document.createElement('td');
	cell.className = 'title';
	var table = document.createElement('table');
	table.id = 'e'+eid+'langsTable';
	table.style.display = (showTitles ? '' : 'none');
	var thead = document.createElement('thead');
	var row = document.createElement('tr');
	var cols = [ {'title':"Title",'colspan':2},{'title':"V",'abbr':"Is title verified?"},{'title':"A",'abbr':"Actions"} ];
	for (var i = 0; i < cols.length; i++) 
		createHeader(row, null, cols[i]['title'], cols[i]['abbr'], null, cols[i]['colspan']);
	thead.appendChild(row);
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
	return cell;
}

/* Function that creates the normal inputs for a given episode
 * @param eid Episode ID
 * @param episode Episode object [optional]
 */
function createEpisodeInputs(eid,episode) {
	if (!episode) episode = episodes[eid];
	episode.inputs = new Object();
	// Episode Number
	var inp = createTextInput('addepm.'+eid+'.epno',5,false,false,5,episode.typeChar+episode.epno);
	inp.onchange = function testForEpisodeType() {
		var regexp = /^(s|c|t|p|o)(\d+)$/i;
		if(!regexp.test(this.value)) {
			this.style.backgroundColor = 'red';
			this.title = "Wrong Episode type.";
			errorCount++;
		} else {
			this.style.backgroundColor = null;
			this.title = null;
			var eptype = RegExp.$1;
			var epno = RegExp.$2;
			var eid = this.id.substring(this.id.indexOf('.')+1,this.id.lastIndexOf('.'));
			var episode = episodes[eid];
			if (episode.typeChar.toLowerCase() != eptype.toLowerCase()) {
				this.style.backgroundColor = 'red';
				this.title = "Wrong Episode type.";
				errorCount++;
			} else {
				this.style.backgroundColor = null;
				this.title = null;
				episode.epno = eptype+epno;
				episode.typeChar = eptype.toUpperCase();
				errorCount--;
			}
		}
	}
	episode.inputs['epno'] = inp;
	// Episode Update
	episode.inputs['update'] = createTextInput('addepm.'+eid+'.update',20,false,true,20,episode.update);
	// Episode Play Length
	inp = createTextInput('addepm.'+eid+'.length',5,false,false,5,episode.playLength);
	inp.onchange = function checkEpLength() { 
		if (isNaN(this.value) || (!isNaN(this.value) && Number(this.value) < 0)) {
			this.style.backgroundColor = 'red';
			this.title = "Episode play length must be a positive integer.";
			errorCount++;
		} else {
			this.style.backgroundColor = null;
			this.title = null;
			errorCount--;
		}
	}
	episode.inputs['duration'] = inp;
	// Episode Air Date
	episode.inputs['airdate'] = createTextInput('addepm.'+eid+'.aired',10,false,false,10,cTimeDate(episode.date));
	// Action inputs
	episode.inputs['epup'] = createIcon(null, 'up', null, moveEpUp, 'Move Episode up', 'i_up1');
	episode.inputs['epdown'] = createIcon(null, 'down', null, moveEpDown, 'Move Episode down', 'i_down1');
	inp = createIcon(null, 'add', null, addEp, 'Add a new Episode', 'i_plus');
	if (addRights && (episode.typeChar != '' || (episode.typeChar == '' && (!anime.eps || normalEpsCnt < anime.eps))))
		inp.style.display = '';
	else inp.style.display = 'none';
	episode.inputs['addep'] = inp;
	inp = createIcon(null, 'rem', null, remEp, 'Remove this new Episode', 'i_minus');
	if (eid > 0) inp.style.display = 'none';
	episode.inputs['remep'] = inp;
}

/* Function that creates an episode row for a given Episode Id
 * @param eid Episode Id
 * @param episode Episode object [optional]
 */
function createEpisodeRow(eid,episode) {
	if (!episode) episode = episodes[eid];
	createEpisodeInputs(eid,episode);
	var row = document.createElement('tr');
	row.id = 'e'+eid;
	// Episode ID
	var ico = (eid < 0) ? document.createTextNode(' ') : createIcon(null, episode.id, 'animedb.pl?show=ep&eid='+episode.id, null, 'show episode details - EID: '+episode.id, 'i_file_details');
	createCell(row,'eid icons', ico);
	// Episode Number
	var cell = createCell(null, 'epno', episode.inputs['epno']);
	cell.appendChild(episode.inputs['update']);
	row.appendChild(cell);
	// Title
	row.appendChild(createEpisodeTitles(eid,episode));
	// Play length
	createCell(row, 'duration', episode.inputs['duration']);
	// Air Date
	cell = createCell(null, 'date', episode.inputs['airdate']);
	if (episode.addDate) cell.title = 'DB add date: '+ episode.addDate;
	row.appendChild(cell);
	// File Count
	createCell(row, 'count files', document.createTextNode(episode.fileCount));
	// Actions
	cell = createCell(null, 'action icons');
	cell.appendChild(episode.inputs['epup']);
	cell.appendChild(episode.inputs['epdown']);
	cell.appendChild(episode.inputs['addep']);
	cell.appendChild(episode.inputs['remep']);
	row.appendChild(cell);
	episode.epRow = row;
	return row;
}

/* Function that creates the episode table */
function createEpisodeTable() {
	var table = document.createElement('table');
	table.className = 'eplist';
	table.id = 'eplist';
	var thead = document.createElement('thead');
	var cols = [{'title':"ID",'abbr':"Episode ID"},{'title':"Ep"},{'title':"Title"},
				{'title':"Len",'abbr':"Play length"},{'title':"Date",'abbr':"Air date"},
				{'title':"Files"},{'title':"Action"}];
	var row = document.createElement('tr');
	for (var i = 0; i < cols.length; i++) 
		createHeader(row, null, cols[i]['title'], cols[i]['abbr']);
	thead.appendChild(row);
	table.appendChild(thead);
	var tbody = document.createElement('tbody');
	for (var i = 0; i < epOrder.length; i++) {
		var eid = epOrder[i];
		var episode = episodes[eid];
		if (!episode) continue;
		episode.prevEp = (i > 0) ? episodes[epOrder[i-1]] : null;
		episode.nextEp = (i < epOrder.length - 1) ? episodes[epOrder[i+1]] : null;
		tbody.appendChild(createEpisodeRow(eid,episode));
	}
	table.appendChild(tbody);
	var tfoot = document.createElement('tfoot');
	row = document.createElement('tr');
	createCell(row, null, document.createTextNode('Options'), null, 2);
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
	var cell = createCell(null, null, document.createTextNode('Add new episode of type: '), null, 7);
	var optionArray = new Object();
	if (!anime.eps || normalEpsCnt < anime.eps) optionArray['N']={'text':"Normal"};
	optionArray['S']={'text':"Special"};
	optionArray['C']={'text':"Opening/Ending/Credits"};
	optionArray['T']={'text':"Trailer/Promo/Ads"};
	optionArray['P']={'text':"Parody/Fandub"};
	optionArray['O']={'text':"Other Episodes"};
	createSelectArray(cell,'episode.add.sel','episode.add.sel',null,null,optionArray);
	cell.appendChild(document.createTextNode(' '));
	var addInput = createBasicButton(null,'add');
	addInput.onclick = addEpisode;
	cell.appendChild(addInput);
	row.appendChild(cell);
	tfoot.appendChild(row);
	table.appendChild(tfoot);
	form.removeChild(form.firstChild);
	form.insertBefore(table,form.firstChild);
}

/* Function that prepares the page for the script */
function prepPage() {
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	// first of all let's put this in a decent table scheme
	aid = document.getElementById('aid') ? Number(document.getElementById('aid').value) : 0;
	form = document.getElementById('addepm.form');
	addRights = document.getElementById('e0') ? true : false;
	if (!form || !aid) return;
	var epDivs = form.getElementsByTagName('div');
	while(epDivs.length) epDivs[0].parentNode.removeChild(epDivs[0]);
	var span = document.createElement('span');
	span.appendChild(document.createTextNode('Please wait while loading data...'));
	form.insertBefore(span,form.firstChild);
	var p = form.getElementsByTagName('p')[0];
	var submitButton = p.getElementsByTagName('input')[0];
	submitButton.onclick = function checkSubmit() {
		if (errorCount < 1) return true;
		alert('There are at least '+errorCount+' error'+(errorCount > 1 ? 's' : '')+'.\nPlease fix the highlighted errors before submiting.');
		return false;
	}	
	fetchData(aid);
}

// hook up the window onload event
addLoadEvent(prepPage);