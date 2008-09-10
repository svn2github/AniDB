/* file Mylist interface 
 * author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (07.08.2008)
 */
 
// GLOBALs //
var mylistTable = null;
var shownAnimes = new Array();
var filteredAnimes = new Array();
// global copy objects
var epTableHead = null;
var fileTableHead = null;
// general column definitions
var fileCols = cloneArray(genFileCols);
removeColAttribute("check-anime",fileCols);
removeColAttribute("state-anime",fileCols);
removeColAttribute("actions-anime",fileCols);
removeColAttribute("anime-source",fileCols);
removeColAttribute("resolution",fileCols);
removeColAttribute("users",fileCols);
removeColAttribute("actions-mylist",fileCols);
var fileSkips = null;
var epCols = cloneArray(genEpCols);
removeColAttribute("airdate",epCols);
removeColAttribute("users",epCols);
removeColAttribute("actions",epCols);
var epSkips = null;
var animeCols = cloneArray(genAnimeCols);
var animeSkips = null;
// filter settings
var filter = new Object();
filter.page = 0;
filter.pages = Math.ceil(animeOrder.length / entriesPerPage);
filter.letter = 'all';
filter.restricted = false;
filter.incomplete = false;
filter.complete = false;
filter.awarded = false;
filter.watched = false;
filter.unwatched = false;
filter.ongoing = false;
filter.wishlist = false;
filter.state = 'all';
filter.animecat = 'all';
filter.wishlisttype = 'all';
filter.wishlistpri = 'all';
// mylist settings
var hiddenGroups = 0;

/* Prepares the page, also fetches XML data */
function prepPage() {
	uriObj = parseURI();
	globalStatus.container = document.getElementById('statusBox');
	updateStatus('Fetching XML data...');
	fetchData('mylist.xml',parseData);
	mylistTable = document.getElementById('animelist');
	if (!LAY_SHOWCRC) removeColAttribute("crc",fileCols);
	var menu = getElementsByClassName(document.getElementsByTagName('div'), 'g_menu mylist_menu', true)[0];
	if (menu) dragDrop.initElement(menu,'Mylist filters');
}

/* Calls data handlers for xml data
 * @param xmldoc the xml document retreived by xmlhttprequest
 */
function parseData(xmldoc) {
	globalStatus.updateBarWithText('Parsing Custom node',15,'Total progress: ');
	var root = xmldoc.getElementsByTagName('root').item(0);
	base_url = root.getAttribute('anidb');
	var t1 = new Date();
	parseCustom(root.getElementsByTagName('custom').item(0));
	var parseCustomNode = (new Date()) - t1;
	globalStatus.updateBarWithText('Processing animes...',45,'Total progress: ');
	t1 = new Date();
	parseAnimes(root.getElementsByTagName('animes'));
	var parseAnimeNode = (new Date()) - t1;
	if (seeTimes) { alert('Processing...'+
		'\n\tanime: '+parseAnimeNode+'ms'+
		'\n\tcustom: '+parseCustomNode+' ms'+
		'\nTotal: '+(parseAnimeNode+parseCustomNode)+' ms'); }
	globalStatus.updateBarWithText('Rendering...',65,'Total progress: ');
	showAnimes = cloneArray(animeOrder);
	renderMylist();
	globalStatus.updateBarWithText('Done.',100,'Total progress: ');
	globalStatus.clearAfterTimeout('globalStatus',1000);
}

/* Calls data handlers for anime xml data */
function parseAnimeData(xmldoc) {
	var root = xmldoc.getElementsByTagName('root').item(0);
	var t1 = new Date();
	parseAnimes(root.getElementsByTagName('animes'));
	var parseAnimeNode = (new Date()) - t1;
	if (seeTimes) { alert('Processing...'+
		'\n\tanime: '+parseAnimeNode+'ms'+
		'\nTotal: '+(parseAnimeNode)+' ms'); }
	var reqaid = root.getElementsByTagName('anime')[0];
	if (reqaid) reqaid = Number(reqaid.getAttribute('id'));
	globalStatus.updateBarWithText('Rendering episodes table for '+animes[reqaid].getOfficialTitle(),50,'Rendering complete: ');
	createEpisodeTable(reqaid);
	var tr = document.getElementById('a'+reqaid);
	if (tr) {
		var icon = getElementsByClassName(tr.getElementsByTagName('span'), 'i_icon i_collapse', false)[0];
		if (!icon) return;
		icon.onclick = animeEntriesWork;
	}
	globalStatus.updateBarWithText('Done.',100,'Rendering complete: ');
	globalStatus.clearAfterTimeout('globalStatus',1000);
}

/* Function that compares mylist against filters and finds out which animes to show */
function filterMylist(array) {
	if (!array) array = animeOrder;
	shownAnimes = cloneArray(array);
	filteredAnimes = new Array();
	var curItems = 0;
	for (var i = 0; i < shownAnimes.length; i++) {
		var anime = animes[shownAnimes[i]];
		var show = true;
		// letter filtering comes last
		if (filter.letter != 'all') {
			var code = anime.getOfficialTitle().toLowerCase().charCodeAt(0);
			if (filter.letter == 'other') {
				if (code < 48 || (code > 57 && code < 65) || (code > 90 && code < 97) || code > 122) show = true;
				else continue;
			}
			else if (filter.letter == '0-9' && (code > 47 && code < 58)) show = true;
			else if (filter.letter.charCodeAt(0) == code) show = true;
			else continue;
		}
		// mylist filters
		if (filter.incomplete && !anime.complete) continue;
		if (filter.complete && anime.complete) continue;
		if (filter.restricted && anime.restricted) continue;
		if (filter.awarded && anime.hasawards) continue;
		if (filter.watched && anime.watched) continue;
		if (filter.unwatched && !anime.watched) continue;
		if (filter.ongoing && (anime.dates['end'] == '?' || ( anime.dates['end'] != '?' && (new Date() < javascriptDate(anime.dates['end']))))) continue;
		if (filter.wishlist && anime.wishlist) continue;
		if (filter.animecat != 'all') {
			var atype = anime.type.toLowerCase();
			if (filter.animecat == 'movies' && atype != 'movie') continue;
			if (filter.animecat == 'ovas' && atype != 'ova') continue;
			if (filter.animecat == 'series' && atype.indexOf('tv') < 0) continue;
			if (filter.animecat == 'other' && (atype == 'movie' || atype == 'ova' || atype.indexOf('tv') >= 0)) continue;
		}
		if (filter.state != 'all') {
			var astate = anime.state.toLowerCase();
			if (astate.indexOf(filter.state) < 0) continue;
		}
		filteredAnimes.push(anime.id);
	}
	filter.pages = Math.ceil(filteredAnimes.length / entriesPerPage);
	var startIndex = filter.page * entriesPerPage;
	var howMany = (startIndex + entriesPerPage) > filteredAnimes.length ? entriesPerPage + (filteredAnimes.length - (startIndex + entriesPerPage)) : entriesPerPage;
	shownAnimes = filteredAnimes.splice(startIndex,howMany);
	return shownAnimes;
}

/* function that fills out the jumplist */
function updateJumplists() {
	var jumps = getElementsByClassName(document.getElementsByTagName('ul'), 'g_list jump', false);
	for (var i = 0; i < jumps.length; i++) {
		var jump = jumps[i];
		var lis = jump.getElementsByTagName('li');
		for (var l = 0; l < lis.length; l++) {
			var li = lis[l];
			if (li.className.indexOf('selected') >= 0) li.className = li.className.replace(' selected','');
			if (li.className.indexOf('page') < 0 && li.className == filter.letter) li.className += ' selected';
			li.onclick = jumpFilter;
		}
		var prev = getElementsByClassName(lis, 'prev', true)[0];
		var next = getElementsByClassName(lis, 'next', true)[0];
		var pages = getElementsByClassName(lis, 'page', true);
		for (var p = 0; p < pages.length; p++) jump.removeChild(pages[p]);
		for (var p = 0; p < filter.pages; p++) {
			if ((p == 0) || (p == filter.pages-1) || (p == filter.page - 1 && p > 0) ||
				(p == filter.page) || (p == filter.page + 1 && filter.page + 1 < filter.pages)) {
				var li = createLi(null,(p+1),'page'+(filter.page == p ? ' selected' : ''),jumpFilter);
				jump.insertBefore(li,next);
			}
		}
	}
}

/* Function that updates the mylist filters */
function updateMylistFilters() {
	var menu = getElementsByClassName(document.getElementsByTagName('div'), 'g_menu mylist_menu', true)[0];
	if (!menu) return;
	var uls = menu.getElementsByTagName('ul');
	
	for (var u = 0; u < uls.length; u++) {
		var ul = uls[u];
		var lis = ul.getElementsByTagName('li');
		for (var l = 0; l < lis.length; l++) {
			var li = lis[l];
			switch(ul.className) {
				case 'filters': li.onclick = mlfilterFilter; break;
				case 'state': li.onclick = mlfilterState; break;
				case 'animecat': li.onclick = mlfilterAnimecat; break;
				case 'wishlistpri': li.onclick = mlfilterWishlistpri; break;
				case 'wishlisttype': li.onclick = mlfilterWishlisttype; break;
				default: break;
			}
		}
	}
	
	menu = getElementsByClassName(document.getElementsByTagName('div'), 'g_menu preferences', true)[0];
	if (!menu) return;
	uls = menu.getElementsByTagName('ul');
	
	for (var u = 0; u < uls.length; u++) {
		var ul = uls[u];
		var lis = ul.getElementsByTagName('li');
		for (var l = 0; l < lis.length; l++) {
			var li = lis[l];
			switch(li.className) {
				case 'mylistfilters': li.onclick = function() { 
						var div = getElementsByClassName(document.getElementsByTagName('div'), 'g_menu mylist_menu', true)[0];
						if (!div) return;
						//div.style.display == 'none' ? div.style.display = '' : div.style.display = 'none';
						//effectsTable['fx'] = new animate();
						//effectsTable['fx'].execFunc(div,'fx','fadeIn',10);
						animate.initElement(div);
						//div.fadeIn();
						div.slideDown();
					}; 
					break;
				default: break;
			}
		}
	}
}

/* function that filters out mylist according to filter */
function mlfilterFilter() {
	var myfilter = this.className.substring(0,this.className.indexOf(' '));
	var state = this.className.substring(this.className.indexOf(' ')+1);
	filter[myfilter] = (state == 'active') ? true : false;
	if (state == 'active') this.className = this.className.replace('active','inactive');
	else this.className = this.className.replace('inactive','active');
	applyFiltering();
}
/* function that filters out mylist according to type filter */
function mlfilterState() {
	var myfilter = this.className.substring(0,(this.className.indexOf(' ') > 0) ? this.className.indexOf(' ') : this.className.length);
	if (filter.state == myfilter) return;
	filter.state = myfilter;
	var ul = this.parentNode;
	var lis = ul.getElementsByTagName('li');
	for (var l = 0; l < lis.length; l++) {
		var li = lis[l];
		var cat = li.className.substring(0,(li.className.indexOf(' ') > 0) ? li.className.indexOf(' ') : li.className.length);
		li.className = li.className.replace(' active','');
		if (cat == myfilter) li.className += ' active';
	}
	applyFiltering();

}
/* function that filters out mylist according to animecat filter */
function mlfilterAnimecat() {
	var myfilter = this.className.substring(0,(this.className.indexOf(' ') > 0) ? this.className.indexOf(' ') : this.className.length);
	if (filter.animecat == myfilter) return;
	filter.animecat = myfilter;
	var ul = this.parentNode;
	var lis = ul.getElementsByTagName('li');
	for (var l = 0; l < lis.length; l++) {
		var li = lis[l];
		var cat = li.className.substring(0,(li.className.indexOf(' ') > 0) ? li.className.indexOf(' ') : li.className.length);
		li.className = li.className.replace(' active','');
		if (cat == myfilter) li.className += ' active';
	}
	applyFiltering();
}

/* function that filters out mylist according to wishlist filter */
function mlfilterWishlistpri() {
	var myfilter = this.className.substring(0,(this.className.indexOf(' ') > 0) ? this.className.indexOf(' ') : this.className.length);
	if (filter.animecat == myfilter) return;
	filter.wishlistpri = myfilter;
	var ul = this.parentNode;
	var lis = ul.getElementsByTagName('li');
	for (var l = 0; l < lis.length; l++) {
		var li = lis[l];
		var cat = li.className.substring(0,(li.className.indexOf(' ') > 0) ? li.className.indexOf(' ') : li.className.length);
		li.className = li.className.replace(' active','');
		if (cat == myfilter) li.className += ' active';
	}
	applyFiltering();
}

/* function that filters out mylist according to wishlist filter */
function mlfilterWishlisttype() {
	var myfilter = this.className.substring(0,(this.className.indexOf(' ') > 0) ? this.className.indexOf(' ') : this.className.length);
	if (filter.animecat == myfilter) return;
	filter.wishlisttype = myfilter;
	var ul = this.parentNode;
	var lis = ul.getElementsByTagName('li');
	for (var l = 0; l < lis.length; l++) {
		var li = lis[l];
		var cat = li.className.substring(0,(li.className.indexOf(' ') > 0) ? li.className.indexOf(' ') : li.className.length);
		li.className = li.className.replace(' active','');
		if (cat == myfilter) li.className += ' active';
	}
	applyFiltering();
}

/* Function that works out the jump filters */
function jumpFilter() {
	if (this.className.indexOf('selected') >= 0) return; // already selected
	var func = this.className;
	switch(func) {
		case 'prev': if (filter.page > 0) filter.page--; break;
		case 'next': if (filter.page < filter.pages-1) filter.page++; break;
		case 'page': filter.page = Number(this.firstChild.nodeValue)-1; break;
		case 'all':
		case 'other':
		case '0-9': 
		default: filter.letter = func; filter.page = 0; break;
	}
	applyFiltering();
}

/* function that applies filtering */
function applyFiltering() {
	filterMylist();
	updateJumplists();
	refreshMylist();
}

/* Function that refreshs mylist after filtering */
function refreshMylist(array) {
	if (!array) array = animeOrder;
	for (var i = 0; i < array.length; i++) {		
		var anime = animes[array[i]];
		if (!anime) continue;
		var row = document.getElementById('a'+anime.id);
		if (shownAnimes.indexOf(anime.id) < 0) row.style.display = 'none';
		else row.style.display = '';
	}
}

/* Function that toggles mylist settings visibility */
function toggleMylistSettings() {
	var div = getElementsByClassName(document.getElementsByTagName('div'), 'g_menu preferences', false)[0];
	if (!div) return;
	var isShown = div.style.display == 'none' ? false : true;
	if (!isShown) {
		div.style.display = '';
		this.className = this.className.replace('i_settings','i_settings_active');
	} else {
		div.style.display = 'none';
		this.className = this.className.replace('i_settings_active','i_settings');
	}
}

/* Function that prepares the file table for sorting */
function prepareForSort() {
	var node = this;
	while (node.nodeName.toLowerCase() != 'table') node = node.parentNode;
	var tbody = node.tBodies[0];
	var newAnimeOrder = new Array();
	var i = 0;
	/* I would actualy prefer to not do two passes.. */
	while (i < tbody.rows.length) {
		var row = tbody.rows[i];
		var aid = Number(row.id.substring(1));
		if (isNaN(aid)) { tbody.removeChild(row); continue; }
		i++;
	}
	sortcol(this);
	for (var i = 0;i < tbody.rows.length; i++) {
		var row = tbody.rows[i];
		var aid = Number(row.id.substring(1));
		newAnimeOrder.push(aid);
	}
	if (newAnimeOrder.length != animeOrder.length) { alert('crap'); errorAlert('prepareForSort','different size orders'); return; }
	animeOrder = newAnimeOrder;
	filterMylist();
	refreshMylist();
}

/* Function that expands/collapses anime entries */
function animeEntriesWork() {
	var doExpand = this.className.indexOf('expand') >= 0 ? true : false;
	var node = this.parentNode;
	while (node.nodeName.toLowerCase() != 'tr') node = node.parentNode;
	var reqaid = Number(node.id.substring(1));
	if (!doExpand) {
		this.className = this.className.replace('collapse','expand');
		this.title = 'Expand this entry';
		this.firstChild.firstChild.nodeValue = '[+]';
		this.onclick = animeEntriesWork;
	} else { 
		this.className = this.className.replace('expand','collapse');
		this.title = 'Collapse this entry';
		this.firstChild.firstChild.nodeValue = '[-]';
		this.onclick = null;
	}
	var tr = document.getElementById('a'+reqaid+'_epsRow');
	if (doExpand) {
		if (!tr) {
			tr = document.createElement('tr');
			tr.id = 'a'+reqaid+'_epsRow';
			var cell = document.createElement('td');
			cell.colSpan = node.cells.length;
			cell.appendChild(document.createTextNode('please wait while loading data...'));
			tr.appendChild(cell);
			node.parentNode.insertBefore(tr,node.nextSibling);
			globalStatus.updateBarWithText('Fetching '+animes[reqaid].getOfficialTitle()+' data...',0,'Loading anime data: ');
			fetchData('anime/a'+reqaid+'.xml',parseAnimeData);
		} else {
			tr.style.display = '';
			this.onclick = animeEntriesWork;
		}
	} else {
		if (tr) tr.style.display = 'none';
		else { errorAlert('animeEntriesWork','no cell'); return;}
		this.onclick = animeEntriesWork;
	}
}

/* Creates an episode files table thead */
function createFilesTableHead() {
	var thead = createTableHead(fileCols);
	fileTableHead = thead;
	return(fileTableHead.cloneNode(true));
}

/* Function that creates an episode files table
 * @param eid Episode ID
 * @return Returns an episode file table inside the row
 */
function createFilesTable(eid) {
	var insertRow = document.createElement('tr');
	insertRow.id = 'e'+eid+'_filesRow';
	insertRow.style.display = (LAY_FILEMODE == '1' ? '' : 'none');
	insertRow.className = 'filelist_container';
	var insertCell = document.createElement('td');
	//insertCell.colSpan = (LAY_NOEPTB) ? '7' : '6';
	insertCell.colSpan = '7';
	// Create the files table
	var table = document.createElement('table');
	table.className = 'filelist';
	table.id = 'e'+eid+'_filesTable';
	var thead = (fileTableHead ? fileTableHead.cloneNode(true) : createFilesTableHead());
	table.appendChild(thead);
	var tbody = document.createElement('tbody');
	// Add files
	var episode = episodes[eid];
	for (var i = 0; i < episode.files.length; i++) {
		var fid = episode.files[i];
		if (!mylist[fid]) continue;
		var row = createFileRow(eid,fid,fileCols,fileSkips);
		var classNames = row.className.split(' ');
		classNames.push((i % 2) ? '' : 'g_odd');
		row.className = classNames.join(' ');
		tbody.appendChild(row);
	}
	// Piece it all together
	table.appendChild(tbody);
	init_sorting(table);
	insertCell.appendChild(table);
	insertRow.appendChild(insertCell);
	return insertRow;
}

/* Creates an episode table thead */
function createEpisodeTableHead() {
	var thead = createTableHead(epCols);
	epTableHead = thead;
	return(epTableHead.cloneNode(true));
}

/* Function that crates an episode table
 * @param aid Anime id to create the episode table
 */
function createEpisodeTable(aid) {
	var pRow = document.getElementById('a'+aid);
	// Create the insert row for later insertion
	var insertRow = document.createElement('tr');
	insertRow.id = 'a'+aid+'_epsRow';
	insertRow.className = 'eplist_container';
	var insertCell = document.createElement('td');
	insertCell.colSpan = pRow.cells.length;
	var table = document.createElement('table');
	var colSpan;
	if (LAY_FILEMODE == 0) {
		removeColAttribute("epno",fileCols);
		table.className = 'eplist';
		table.id = 'a'+aid+'_episodesTable';
		colSpan = 7;
		var thead = (epTableHead ? epTableHead.cloneNode(true) : createEpisodeTableHead(colSpan));
		table.appendChild(thead);
		var tbody = document.createElement('tbody');
		var anime = animes[aid];
		for (var i = 0; i < anime.episodes.length; i++) {
			var eid = anime.episodes[i];
			var row = createEpisodeRow(aid,eid,epCols,epSkips);
			var classNames = row.className.split(' ');
			classNames.push((i % 2) ? '' : 'g_odd');
			classNames.push('files');
			row.className = classNames.join(' ');
			tbody.appendChild(row);
			row = createFilesTable(eid);
			tbody.appendChild(row);
		}
		table.appendChild(tbody);
	} else { // only show files
		table.className = 'filelist';
		table.id = 'a'+aid+'_filesTable';
		colSpan = 12;
		if (LAY_NOEPTB) colSpan++;
		if ((uriObj['showcrc'] && uriObj['showcrc'] == '1') || LAY_SHOWCRC) colSpan++;
		var thead = (fileTableHead ? fileTableHead.cloneNode(true) : createFilesTableHead());
		table.appendChild(thead);
		var tbody = document.createElement('tbody');
		var anime = animes[aid];
		// Add files		
		for (var e = 0; e < anime.episodes.length; e++) {
			var eid = anime.episodes[e];
			var episode = episodes[eid];
			for (var f = 0; f < episode.files.length; f++) {
				var fid = episode.files[f];
				if (!mylist[fid]) continue;
				var row = createFileRow(eid,fid,fileCols,fileSkips);
				tbody.appendChild(row);
			}
		}
		for (var i = 0 ; i < tbody.rows.length; i++) {
			var classNames = tbody.rows[i].className.split(' ');
			classNames.push((i % 2) ? '' : 'g_odd');
			tbody.rows[i].className = classNames.join(' ');
		}
		table.appendChild(tbody);
	}
	// Now piece it all together
	insertCell.appendChild(table);
	insertRow.appendChild(insertCell);
	// add the new row to the mylist table
	pRow.parentNode.replaceChild(insertRow,document.getElementById('a'+aid+'_epsRow'));
}

/* Function that expands episode entries */
function episodeEntriesWork() {
	var doExpand = this.className.indexOf('expand') >= 0 ? true : false;
	var node = this.parentNode;
	while (node.nodeName.toLowerCase() != 'tr') node = node.parentNode;
	var tr = document.getElementById(node.id+'_filesRow');
	if (!doExpand) {
		this.className = this.className.replace('collapse','expand');
		this.title = 'Expand this entry';
		this.firstChild.firstChild.nodeValue = '[+]';
		if (tr) tr.style.display = 'none';
		
	} else { 
		this.className = this.className.replace('expand','collapse');
		this.title = 'Collapse this entry';
		this.firstChild.firstChild.nodeValue = '[-]';
		if (tr) tr.style.display = '';
	}
}

/* Function that renders mylist */
function renderMylist() {
	var mylistStatus = new statusInformation();
	mylistStatus.container = document.getElementById('mylistLoadStatus');
	mylistStatus.loadingbar_color = 'blue';
	var caption = mylistTable.caption;
	var span = document.createElement('span');
	span.className = 'icons';
	createIcon(span, 'settings', null, toggleMylistSettings, 'Mylist settings', 'i_settings');
	caption.insertBefore(span,caption.firstChild);
	var tbody = mylistTable.tBodies[0];
	var thead = createTableHead(animeCols);
	mylistTable.insertBefore(thead,tbody);
	var rtbody = document.createElement('tbody');
	filterMylist();
	for (var i = 0; i < animeOrder.length; i++) {		
		var anime = animes[animeOrder[i]];
		if (!anime) continue;
		mylistStatus.updateBar(Math.round((i+1)/animeOrder.length*100),'Total progress: ');
		var row = createAnimeRow(anime.id,animeCols,animeSkips);
		if (anime.complete) row.className = "complete";
		if (anime.watched) row.className = "all_watched";
		if (shownAnimes.indexOf(anime.id) < 0) row.style.display = 'none';
		rtbody.appendChild(row);
	}
	mylistTable.replaceChild(rtbody,tbody);
	updateJumplists();
	updateMylistFilters();
	init_sorting(thead,'title','down');
	var ths = thead.getElementsByTagName('th');
	for (var i = 0; i < ths.length; i++) ths[i].onclick = prepareForSort;
}

/* Add load event */
addLoadEvent(prepPage);