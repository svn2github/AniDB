/* *
 * @file Mylist interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (22.03.2007)
 */
 
// GLOBALs //

var animeListTable; // the animelist table
var animeListRow; // the sample animeList row
var animeFilter = {'complete':false,'incomplete':false,'restricted':false,'watched':false,'ongoing':false,
                   'notwatched':false,'stalled':false,'fileinfo':true,'wishlisted':false,'awarded':false,
									 'myvote':false,'wishlisttype':false,'wishlistpri':false};
var curpage = 1;
var filteredAnimes = 0;
var eplistTableRow;
var filelistTableRow;
var fileRow;
var layout = 'mylist'; // holds current layout

/* *
 * Calls data handlers for xml data
 * @param xmldoc the xml document retreived by xmlhttprequest
 */
function parseData(xmldoc) {
  updateStatus('parsing Custom node');
  var root = xmldoc.getElementsByTagName('root').item(0);
  base_url = root.getAttribute('anidb');
  var t1 = new Date();
  parseCustom(root.getElementsByTagName('custom').item(0));
  var parseCustomNode = (new Date()) - t1;
  replaceGlobals(document.body);
  updateStatus('Processing animes...');
  t1 = new Date();
  parseAnimes(root.getElementsByTagName('animes'));
  var parseAnimeNode = (new Date()) - t1;
  updateStatus('Processing user information...');
  if (seeTimes) { alert('Processing...'+
        '\n\tanime: '+parseAnimeNode+'ms'+
        '\n\tcustom: '+parseCustomNode+' ms'+
        '\nTotal: '+(parseAnimeNode+parseCustomNode)+' ms'); }
  updateStatus('');
  renderPage();
  showPage(curpage);
}

/* *
 * Calculates the number of non filtered animes and returns it
 */
function calculateNonFilteredAnimes() { 
  //return animeOrder.length - filteredAnimes;
  var vis = 0;
  for (var a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    if (!anime.filtered) vis++;
  }
  return vis;
}

/* *
 * Paging support
 * @param page Page to show
 */
function showPage(page) {
  if (isNaN(page)) page = this.firstChild.nodeValue;
  if (!page) return;
  var vis = 0;
  var i = 0;
  var start = ((page - 1) * entriesPerPage);
  for (var a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    var row = document.getElementById('a'+anime.id);
    if (!row) continue;
    if (!anime.filtered) { // anime is not filtered
      vis++;
      if (vis <= start || vis > start + entriesPerPage) row.style.display = 'none';
      else {
        row.style.display = '';
        row.className = row.className.replace('g_odd','');
        row.className += (i % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
        i++;
      }
    }
  }
  createPageJump(page);
}

/* *
 * Shows next page
 */
function nextPage() {
  var maxPage = Math.ceil(calculateNonFilteredAnimes()/entriesPerPage);
  if (curpage + 1 <= maxPage) showPage(++curpage);
}

/* *
 * Shows previous page
 */
function prevPage() {
  if (curpage - 1 >= 1) showPage(--curpage);
}

/* *
 * Creates the Page Jump
 */
function createPageJump(page) {
  if (!isNaN(page)) curpage = Number(page);
  var jumppages = document.getElementsByTagName('jumppage');
  if (!jumppages.length) jumppages = getElementsByClassName(document.getElementsByTagName('UL'),'pageJump',true);
  for (var j = 0; j < jumppages.length; j++) {
    var ul = document.createElement('UL');
    ul.className = 'pageJump';
    var pages = Math.ceil(calculateNonFilteredAnimes()/entriesPerPage);
		//alert('calculateNonFilteredAnimes(): '+calculateNonFilteredAnimes()+'\nePP: '+entriesPerPage+'\npages: '+pages);
    for (var i = 1; i <= pages; i++) {
      if ((i == 1) ||
          (i == pages) || 
          (i == curpage - 2 && i > 2) ||
          (i == curpage - 1 && i > 1) || 
          (i == curpage) ||
          (i == curpage + 1 && curpage + 1 < pages) ||
          (i == curpage + 2 && curpage + 2 < pages) 
         )
      {
        var li = document.createElement('LI');
        li.className = 'jitem';
        if (i == page) li.className += '_current';
        li.onclick = showPage;
        li.appendChild(document.createTextNode(i));
        li.style.cursor = "pointer";
        ul.appendChild(li);
      }
    }
    jumppages[j].parentNode.replaceChild(ul,jumppages[j]);
  }
}

/* *
 * Creates the mylist status Icons
 * @param parentNode where the icons go
 * @anime object that holds anime data
 */
function mylistIcons(parentNode,anime) {
  var dt = javascriptDate(anime.dates['end']);
  if (anime.dates['end'] == '?' || ( anime.dates['end'] != '?' && (new Date() < dt))) createIcon(parentNode, '[>]', null, null, 'ongoing anime','i_gstate_ongoing');
  if (anime.wishlist) {
    var pri = anime.wishlist['pri'];
    if (pri == 'medium') pri = 'med';
    var tooltip = anime.wishlist['type']+' type with '+anime.wishlist['pri']+' priority';
    if (anime.wishlist['comment'].length) tooltip += ', comment: '+anime.wishlist['comment'];
    createIcon(parentNode, '[w]', null, null, tooltip,'i_pri_'+pri);
  }
  if (anime.hasawards) createIcon(parentNode, '[*]', null, null, 'this anime has awards','i_araward');
  var state = anime.state;
  if (state) { // Wishlisted anime with no files don't have state
    if (state.indexOf('hdd') >= 0) state = 'onhdd';
    if (state.indexOf('cd') >= 0) state = 'oncd';
    createIcon(parentNode, '['+anime.state+']', null, null, 'state: '+anime.state,'i_state_'+state);
  }
}

/* *
 * Creates an Anime mylist Entry
 * @param anime The object that holds the anime data
 */ 
function updateAnimeRow(anime) {
  row = animeListRow.cloneNode(true);
  row.id = row.id.replace('%ANIMEID%',anime.id);
  //row.style.display = '';
  if (anime.complete) row.className = "complete ";
  if (anime.watched) row.className = "all_watched ";
	// work
	var elems = row.getElementsByTagName('ANIME.EXPAND');
	while (elems.length) {
		var a = document.createElement('A');
		var img = document.createElement('IMG');
		img.src = 'css/icons/plus.gif';
		img.title = 'expand all entries';
		img.alt = '(+)';
		img.width = 15;
		img.height = 13;
		a.appendChild(img);
		a.onclick = expandEpsByAnime;
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	var elems = row.getElementsByTagName('ANIME.TITLE.ALINK');
	while (elems.length) {
    var title = anime.getTitle('main');
		var tooltip = "";
    if (animeTitleDisplay == 1) title += ' ('+anime.getAltTitle('official')+')';
    if (animeTitleDisplay == 2 && anime.getAltTitle('official').length) tooltip = 'Alternative title: '+ anime.getAltTitle('official');
    elems[0].parentNode.setAttribute('anidb:sort',title);
		var a = createLink(null, title, 'anime.html?show=anime&aid='+anime.id, null, null, tooltip, null);
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.ICONS');
	while (elems.length) {
    var div = document.createElement('DIV');
		div.className = 'icons';
		mylistIcons(div,anime);
		elems[0].parentNode.replaceChild(div,elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.EPS.OWNED');
	while (elems.length) {
		var eps = anime.neps['user'] + '/' + anime.neps['cnt']+(anime.seps['user'] ? '+' + anime.seps['user'] : '');
		elems[0].parentNode.setAttribute('anidb:sort',anime.neps['user']);
		elems[0].parentNode.replaceChild(document.createTextNode(eps),elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.EPS.SEEN');
	while (elems.length) {
		var seen = anime.neps['seen'] + '/' + anime.neps['cnt']+(anime.seps['seen'] ? '+' + anime.seps['seen'] : '');
		elems[0].parentNode.setAttribute('anidb:sort',anime.neps['seen']);
		elems[0].parentNode.replaceChild(document.createTextNode(seen),elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.RATING');
	while (elems.length) {
		elems[0].parentNode.setAttribute('anidb:sort',(anime.rating['votes'] ? anime.rating['rating'] : '0'));
		elems[0].parentNode.replaceChild(document.createTextNode((anime.rating['votes'] ? anime.rating['rating'] : '-')),elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.VOTES');
	while (elems.length) {
		var span = document.createElement('SPAN');
		span.className = 'count';
		span.appendChild(document.createTextNode((anime.rating['votes'] ? anime.rating['votes'] : '0')));
		elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.MYVOTE');
	while (elems.length) {
		var vote = (anime.myvote ? anime.myvote['vote'] : '');
		var rep = document.createTextNode(vote);
		if (!anime.watched) elems[0].parentNode.className += ' none';
		else {
			if (!anime.myvote) { 
				elems[0].parentNode.className += ' pending';
				rep = createIcon(null,'[!]',null,null,'You have not voted for this anime yet!','i_no_vote');
			} else {
				if (anime.myvote['type'] == 'temp') elems[0].parentNode.className += ' temp mid';
				if (anime.myvote['type'] == 'normal') elems[0].parentNode.className += ' anime mid';
				if (Number(vote) >= 8) { rep = document.createElement('B'); rep.appendChild(document.createTextNode(vote)); }
			}
		}
		elems[0].parentNode.setAttribute('anidb:sort',(anime.myvote ? anime.myvote['vote'] : '0'));
		elems[0].parentNode.replaceChild(rep,elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.REVIEW.RATING');
	while (elems.length) {
		elems[0].parentNode.setAttribute('anidb:sort',(anime.reviews ? anime.reviews : '0'));
		elems[0].parentNode.replaceChild(document.createTextNode((anime.reviews ? anime.rrating : '-')),elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.REVIEW.CNT');
	while (elems.length) {
		var span = document.createElement('SPAN');
		span.className = 'count';
		span.appendChild(document.createTextNode((anime.reviews ? anime.reviews : '0')));
		elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.ID');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(anime.id),elems[0]);
	elems = row.getElementsByTagName('ANIME.YEAR');
	while (elems.length) { 
		elems[0].parentNode.setAttribute('anidb:sort',anime.year);
		elems[0].parentNode.replaceChild(document.createTextNode(anime.year),elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.TYPE');
	while (elems.length) { 
		elems[0].parentNode.setAttribute('anidb:sort',anime.type);
		elems[0].parentNode.replaceChild(document.createTextNode(anime.type),elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.SIZE');
	while (elems.length) { 
		elems[0].parentNode.setAttribute('anidb:sort',anime.size);
		elems[0].parentNode.replaceChild(document.createTextNode(anime.size),elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.ACTION');
	while (elems.length) { 
		var input = document.createElement('INPUT');
		input.type = 'checkbox';
		input.name = 'trade.a'+anime.id;
		elems[0].parentNode.replaceChild(input,elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.MYVOTE');
	while (elems.length) {
		var vote = (anime.myvote ? anime.myvote['vote'] : '');
		var rep = document.createTextNode(vote);
		if (!anime.watched) elems[0].parentNode.className += ' none';
		else {
			if (!anime.myvote) { 
				elems[0].parentNode.className += ' pending';
				rep = createIcon(null,'[!]',null,null,'You have not voted for this anime yet!','i_no_vote');
			} else {
				if (anime.myvote['type'] == 'temp') elems[0].parentNode.className += ' temp mid';
				if (anime.myvote['type'] == 'normal') elems[0].parentNode.className += ' anime mid';
				if (Number(vote) >= 8) { rep = document.createElement('B'); rep.appendChild(document.createTextNode(vote)); }
			}
		}
		elems[0].parentNode.setAttribute('anidb:sort',(anime.myvote ? anime.myvote['vote'] : '0'));
		elems[0].parentNode.replaceChild(rep,elems[0]);
	}
	elems = row.getElementsByTagName('ANIME.MYVOTE.DATE');
	while (elems.length) {
		elems[0].parentNode.setAttribute('anidb:sort',anime.myvote['date']);
		elems[0].parentNode.replaceChild(document.createTextNode(anime.myvote['date']),elems[0]);
	}
	return row;
}

function expandEpsByAnime() {
	var aid=this.parentNode.parentNode.id;
	if (!document.getElementById(aid+'_eplist')) fetchAnime(aid);
	else document.getElementById(aid+'_eplist').style.display = '';
	this.onclick = foldEpsByAnime;
	var img = this.getElementsByTagName('IMG')[0];
	img.src = 'css/icons/minus.gif';
	img.title = 'collapse all entries';
	img.alt = '[-]';
}
function foldEpsByAnime() { 
	var aid=this.parentNode.parentNode.id;
	if (document.getElementById(aid+'_eplist')) document.getElementById(aid+'_eplist').style.display = 'none';
	this.onclick = expandEpsByAnime;
	var img = this.getElementsByTagName('IMG')[0];
	img.src = 'css/icons/plus.gif';
	img.title = 'expand all entries';
	img.alt = '[+]';
}

function fetchAnime(aid) {
	updateStatus('Fetching anime data...');
	loadData('anime/'+aid+'.xml',parseAnimeData);
	uriObj['show'] = 'anime';
}

function parseAnimeData(xmldoc) {
  updateStatus('parsing Custom node');
  var root = xmldoc.getElementsByTagName('root').item(0);
  var t1 = new Date();
  parseCustom(root.getElementsByTagName('custom').item(0));
  var parseCustomNode = (new Date()) - t1;
  updateStatus('Processing anime...');
  t1 = new Date();
  parseAnimes(root.getElementsByTagName('animes'));
  var parseAnimeNode = (new Date()) - t1;
  updateStatus('Processing user information...');
  if (seeTimes) { alert('Processing...'+
        '\n\tanime: '+parseAnimeNode+'ms'+
        '\n\tcustom: '+parseCustomNode+' ms'+
        '\nTotal: '+(parseAnimeNode+parseCustomNode)+' ms'); }
  updateStatus('');
  createEpList('a'+anime.id,null);
}

function createEpList(aid,nbody) {
	var epTable, body;
	if (!nbody) {
		epTable = eplistTableRow.cloneNode(true);
		epTable.id = aid+'_eplist';
	  body = epTable.getElementsByTagName('TBODY')[0];
	} else body = nbody;
	var anime = animes[aid.substr(1,aid.length)];
	if (!anime) return;
	var cloneTBody = document.createElement('TBODY');
	cloneTBody.appendChild(body.rows[0]);
	filelistTableRow = document.createElement('TBODY');
	filelistTableRow.appendChild(body.rows[0]);
	fileRow = document.createElement('TBODY');
	fileRow.appendChild(filelistTableRow.getElementsByTagName('TBODY')[0].rows[0]);
	var cnt = -1;
	for (var e = 0; e < anime.eps.length; e++) {
		var episode = episodes[anime.eps[e]];
		if (!episode) continue;
		var row = cloneTBody.rows[0].cloneNode(true);
		row.id = 'eid_'+episode.id;
		cnt++;
    row.className += (cnt % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
		// Work
		var elems = row.getElementsByTagName('ANIME.EP.EXPAND');
		while (elems.length) {
			var a = document.createElement('A');
			var img = document.createElement('IMG');
			img.src = 'css/icons/plus.gif';
			img.title = 'expand all entries';
			img.alt = '(+)';
			img.width = 15;
			img.height = 13;
			a.appendChild(img);
			a.onclick = expandFilesByEp;
			elems[0].parentNode.replaceChild(a,elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.EP.EPNO');
		while (elems.length) {
			var a = createLink(null,episode.epno,'episode.html?show=ep&eid='+episode.id+'&aid='+anime.id,null,null,null,null);
			elems[0].parentNode.replaceChild(a,elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.EP.TITLE');
		while (elems.length) {
			var span = document.createElement('SPAN');
			span.appendChild(document.createTextNode(episode.getTitle()));
			var altTitle = curTitle = '';
			if (episodeTitleLang != episodeAltTitleLang && 
					episode.titles[episodeAltTitleLang] && 
					episode.titles[episodeAltTitleLang] != '' &&
					episode.titles[episodeAltTitleLang] != curTitle) altTitle = episode.titles[episodeAltTitleLang];
			if (altTitle != '') {
				if (episodeTitleDisplay == 1) span.appendChild(document.createTextNode(' ('+altTitle+')'));
				if (episodeTitleDisplay == 2) span.title = mapLanguage(episodeAltTitleLang) + ' title: '+ altTitle;
			}
			elems[0].parentNode.replaceChild(span,elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.EP.ICONS');
		while (elems.length) {
			var div = document.createElement('DIV');
			div.className = 'icons';
			createEpIcons(div,episode);
			elems[0].parentNode.replaceChild(div,elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.EP.DURATION');
		while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(episode.length),elems[0]);
		elems = row.getElementsByTagName('ANIME.EP.FILES');
		while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(episode.files.length),elems[0]);
		elems = row.getElementsByTagName('ANIME.EP.AIRDATE');
		while (elems.length) {
			var text,tooltip;
			if (episode.dates['rel']) { text = cTimeDate(episode.dates['rel']); tooltip = 'added on '+episode.dates['add']; }
			else text = cTimeDate(episode.dates['add']);
			var a = createLink(null,text,null,null,null,(tooltip ? tooltip : null),null);
			elems[0].parentNode.replaceChild(a,elems[0]);
		}
		body.appendChild(row);
	}
	var animeRow = document.getElementById(aid);
	if (animeRow) { 
		animeRow.parentNode.insertBefore(epTable,animeRow.nextSibling);
		epTable.style.display = '';
	}
}

/* *
 * Writes the Data to the animetable
 */
function writeData() {
	updateStatus('Rendering page...');
	var tbody = animeListTable.tBodies[0];
	clearTableRows(tbody);
  for (var i = 0; i < animeOrder.length; i++) {
    var anime = animes[animeOrder[i]];
		anime.filtered = false;
    var row = updateAnimeRow(anime);
    row.className += (i % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
    tbody.appendChild(row);
  }
}

/* *
 * Writes the Wishlist data to the animetable
 */
function writeWishlistData() {
	updateStatus('Rendering page...');
	var tbody = animeListTable.tBodies[0];
	clearTableRows(tbody);
	var cnt = 0;
  for (var i = 0; i < animeOrder.length; i++) {
    var anime = animes[animeOrder[i]];
		if (!anime.wishlist) { anime.filtered = true; continue; }
		if (animeFilter['wishlisttype'] && anime.wishlist['type'] != animeFilter['wishlisttype']) { anime.filtered = true; continue; }
		if (animeFilter['wishlistpri'] && anime.wishlist['pri'] != animeFilter['wishlistpri']) { anime.filtered = true; continue; }
		anime.filtered = false;
		cnt++;
    var row = updateAnimeRow(anime);
    row.className += (cnt % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
    tbody.appendChild(row);
  }
}

/* *
 * Writes the Wishlist data to the animetable
 */
function writeMyvotesData() {
	updateStatus('Rendering page...');
	var tbody = animeListTable.tBodies[0];
	clearTableRows(tbody);
	var cnt = 0;
  for (var i = 0; i < animeOrder.length; i++) {
    var anime = animes[animeOrder[i]];
		if (!anime.myvote) { anime.filtered = true; continue; }
		if (animeFilter['restricted'] == 'hentai' && !anime.restricted) { anime.filtered = true; continue; }
		if (animeFilter['restricted'] == 'nohentai' && anime.restricted) { anime.filtered = true; continue; }
		anime.filtered = false;
		cnt++;
    var row = updateAnimeRow(anime);
		if (anime.myvote && anime.myvote['type'] != animeFilter['myvote']) anime.filtered = true;
    row.className += (cnt % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
    tbody.appendChild(row);
  }
}

/* *
 * Renders page, fills in anime rows etc
 */
function renderPage() {
  // first see if we have animes
  if (!animes) { alert('Do you have animes?'); return; }
  animeListTable = getElementsByClassName(document.getElementsByTagName('TABLE'),'animelist',false)[0];
  if (!animeListTable) { alert('Your html structure isn\'t valid'); return; }
	
  var tbody = animeListTable.tBodies[0];
  animeListRow = tbody.rows[0].cloneNode(true);
  tbody.removeChild(tbody.rows[0]);
	if (tbody.rows[0]) {
		eplistTableRow = tbody.rows[0].cloneNode(true);
		tbody.removeChild(tbody.rows[0]);
	}
  init_sorting(animeListTable.tHead.rows[0],'title','up');
  var ths = animeListTable.tHead.getElementsByTagName('TH');
  for (var i = 0; i < ths.length; i++) {
			ths[i].onclick = function onclick(event) {
      showPage(1);
      sortcol(this);
    }
  }
	if (uriObj['show'] && uriObj['show'] == 'wishlist') layout = 'wishlist';
	if (uriObj['show'] && uriObj['show'] == 'myvotes') layout = 'myvotes';
	switchLayout(null);
	updateStatus('');
}

/* *
 * Function that switches between layouts
 */
function switchLayout(node) {
	//alert('current layout: '+layout);
	var uls = document.getElementsByTagName('UL');
	if (!uls.length) return;
	var wishlisttype = getElementsByClassName(uls,'wishlisttype',false)[0];
	var wishlistpri = getElementsByClassName(uls,'wishlistpri',false)[0];
	if (node) {
		var text = this.firstChild.nodeValue;
		var state = text.substring(0,text.indexOf(' '));
		if (state == 'show' && layout == 'mylist') state = 'hide';
		var new_state = (state == 'show') ? 'hide' : 'show';
		this.firstChild.nodeValue = this.firstChild.nodeValue.replace(state,new_state);
	}
	switch (layout) {
		case 'wishlist':
			layout = 'mylist';
			if (wishlisttype) wishlisttype.style.display = '';
			if (wishlistpri) wishlistpri.style.display = '';
			curDataWrite = function() { writeWishlistData(); }
			break;
		case 'mylist':
			layout = 'wishlist';
			if (wishlisttype) wishlisttype.style.display = 'none';
			if (wishlistpri) wishlistpri.style.display = 'none';
			curDataWrite = function() { writeData(); }
			break;
		case 'myvotes':
			layout = 'mylist';
			animeFilter['myvote'] = 'normal';
			curDataWrite = function() { writeMyvotesData(); }
			break;
		default:
			alert('uknown layout type');
	}
	curDataWrite();
	curpage = 1;
	showPage(curpage);
	updateStatus('');
}

/* *
 * Filters Animes by state
 */ 
function filterByState() {
  var state = this.firstChild.className.substr(this.firstChild.className.indexOf('i_state_')+8,this.firstChild.className.length);
  if (!state) return;
  
  if (state.indexOf('onhdd') >= 0) state = 'hdd';
  if (state.indexOf('oncd') >= 0) state = 'cd';
  
  for (var a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    var row = document.getElementById('a'+anime.id);
    if (!row) continue;
    if (state == 'all') { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
    else {
      if (anime.state.indexOf(state) >= 0) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else { row.style.display = 'none'; filteredAnimes++; anime.filtered = true; }
    }
  }
  curpage = 1; showPage(curpage);
}

/* *
 * Function that checks if an anime should filtered or not
 * @param anime Anime to filter
 * @return true if anime should be filtered, false otherwise
 */
function filterAnime(anime) {
  var vis = false;
  if (anime.complete && animeFilter['complete']) vis = true;
  if (!anime.complete && animeFilter['incomplete']) vis = true;
  if (anime.watched && animeFilter['watched']) vis = true;
  if (!anime.watched && animeFilter['notwatched']) vis = true;
  if (anime.wishlist && animeFilter['wishlisted']) vis = true;
  if (anime.restricted && animeFilter['restricted']) vis = true;
	if (animeFilter['wishlisttype'] && anime.wishlist && anime.wishlist['type'] == animeFilter['wishlisttype']) vis = true;
	if (animeFilter['wishlistpri'] && anime.wishlist && anime.wishlist['pri'].indexOf(animeFilter['wishlistpri']) > -1) vis = true;
	if (anime.restricted && animeFilter['restricted']) vis = true;
  var dt = javascriptDate(anime.dates['end']);
  if ((anime.dates['end'] == '?' || ( anime.dates['end'] != '?' && (new Date() < dt))) && animeFilter['ongoing']) vis = true;
  if (anime.hasawards && animeFilter['awarded']) vis = true;
	if (anime.myvote && anime.myvote['type'] == animeFilter['myvote']) vis = true;
  return vis;
}

/* *
 * Filters Animes By Status
 */
function filterByStatus() {
  var text = this.firstChild.nodeValue;
  var state = text.substring(0,text.indexOf(' '));
  var filter = text.substring(text.indexOf(' ')+1,text.length);
  filter = filter.replace(' ','');
  var new_state = (state == 'show') ? 'hide' : 'show';
  this.firstChild.nodeValue = this.firstChild.nodeValue.replace(state,new_state);
  state = !(state == 'show');
  for (a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    var row = document.getElementById('a'+anime.id);
    if (!row) continue;
    animeFilter[filter] = state;
    var filter = filterAnime(anime);
    row.style.display = (filter ? 'none' : '');
    if (filter) { filteredAnimes++; anime.filtered = true; }
    else { filteredAnimes--; anime.filtered = false; }
  }
  curpage = 1; showPage(curpage);
}

/* *
 * Filter Animes by Type
 */
function filterByType() {
  var type = this.firstChild.nodeValue.substring(0,this.firstChild.nodeValue.indexOf(' '));
  if (!type) return;
  var localA;
  var stateUL = this.parentNode.parentNode;
  if (stateUL) {
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) {
      var a = as[i];
      var text = a.firstChild.nodeValue.substring(0,a.firstChild.nodeValue.indexOf(' '));
      a.firstChild.nodeValue = a.firstChild.nodeValue.replace(' *','');
      if (text == type) { localA = a; a.firstChild.nodeValue += ' *' }
    }
  }
  type = type.substring(0,type.length-1).toLowerCase();
  for (var a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    var row = document.getElementById('a'+anime.id);
    if (!row) continue;
    if (type == 'al') { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
    else {
      if (anime.type.toLowerCase().indexOf(type) >= 0) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else { row.style.display = 'none'; filteredAnimes++; anime.filtered = true; }
    }
  }
  curpage = 1; showPage(curpage);
}

/* *
 * Filter animes by Letter
 */
function filterByLetter() {
  var letter = this.className;
  if (!letter) return;
  for (var a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    var row = document.getElementById('a'+anime.id);
    if (!row) continue;
    if (letter == 'all') { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
    else {
      var code = anime.getTitle('main').charAt(0).toLowerCase();
      code = code.charCodeAt(0);
      if (letter == '0-9' && (code > 47 && code < 58)) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else if (letter == 'other' && (code < 48 || (code > 57 && code < 65) || (code > 90 && code < 97) || code > 122)) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else if (code == letter.charCodeAt(0)) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else { row.style.display = 'none'; filteredAnimes++; anime.filtered = true; }
    }
  }
  curpage = 1; showPage(curpage);
}

/* *
 * Filters the list by text
 */
function filterByText() {
  var inputs = this.parentNode.getElementsByTagName('INPUT');
  var text = '';
  for (var i = 0; i < inputs.length; i++) {
    if (inputs[i].type == 'text' && inputs[i].name == 'searchbox')  { text = inputs[i].value; break; }
  }
  if (text == '') return;
  var textArray = text.split(' '); // split on spaces
  for (var a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    var row = document.getElementById('a'+anime.id);
    if (!row) continue;
    var found = 0;
    for (var t = 0; t < textArray.length; t++) {
      var word = new RegExp(textArray[t],'mgi');
      for (var types in anime.titles) {
        for (var lang in anime.titles[types]) {
          if (anime.titles[types][lang].match(word)) found++;
        }
      }
    }
    if (search_logic == 'OR') { 
      if (found) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else { row.style.display = 'none'; filteredAnimes++; anime.filtered = true; }
    } else { 
      if (found == textArray.length) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else { row.style.display = 'none'; filteredAnimes++; anime.filtered = true; }
    }
  }
  curpage = 1; showPage(curpage);
}

/* *
 * Filter Animes by Wishlist Type
 */
function filterByWishlistType() {
  var type = this.firstChild.nodeValue;
  if (!type) return;
  var localA;
  var stateUL = this.parentNode.parentNode;
  if (stateUL) {
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) {
      var a = as[i];
      var text = a.firstChild.nodeValue;
      a.firstChild.nodeValue = a.firstChild.nodeValue.replace(' *','');
      if (text == type) { localA = a; a.firstChild.nodeValue += ' *' }
    }
  }
  if (type.search(/all/i) > -1) type = false;
	animeFilter['wishlisttype'] = type;
  for (var a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    var row = document.getElementById('a'+anime.id);
    if (!row) continue;
    if (!type) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
    else {
      if (anime.wishlist && anime.wishlist['type'] == animeFilter['wishlisttype']) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else { row.style.display = 'none'; filteredAnimes++; anime.filtered = true; }
    }
  }
  curpage = 1; showPage(curpage);
}

/* *
 * Filter Animes by Wishlist Priority
 */
function filterByWishlistPriority() {
  var pri = this.firstChild.className.substr(this.firstChild.className.indexOf('i_pri_')+6,this.firstChild.className.length);
  if (!pri) return;
	animeFilter['wishlistpri'] = pri;
  for (var a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    var row = document.getElementById('a'+anime.id);
    if (!row) continue;
    if (!pri) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
    else {
      if (anime.wishlist && anime.wishlist['pri'].indexOf(animeFilter['wishlistpri']) > -1) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else { row.style.display = 'none'; filteredAnimes++; anime.filtered = true; }
    }
  }
  curpage = 1; showPage(curpage);
}


/* *
 * Filter animes by vote type
 */
function changeVotesType() {
	var type = this.firstChild.nodeValue;
	if (!type) return;
	clearFontWeight(this.parentNode.parentNode,this.nodeName);
	this.style.fontWeight = 'bold';
	animeFilter['myvote'] = ((type.search(/tmp/i) < 0)) ? 'normal' : 'temp';
  for (var a in animes) {
    var anime = animes[a];
    if (!anime) continue;
    var row = document.getElementById('a'+anime.id);
    if (!row) continue;
    if (!type) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
    else {
      if (anime.myvote && anime.myvote['type'] == animeFilter['myvote']) { row.style.display = ''; filteredAnimes--; anime.filtered = false; }
      else { row.style.display = 'none'; filteredAnimes++; anime.filtered = true; }
    }
  }
  curpage = 1; showPage(curpage);
}

/* *
 * Filter Animes by restricted value
 */
function changeRestrictedFilter() {
	var restricted = this.firstChild.nodeValue;
	clearFontWeight(this.parentNode,this.nodeName);
	this.style.fontWeight = 'bold';
	animeFilter['restricted'] = ((restricted.search(/hide/i) < 0)) ? 'hentai' : 'nohentai';
	if (restricted.search(/all/i) > -1) animeFilter['restricted'] = false;
	//alert('restricted: '+restricted+'\nanimeFilter[\'restricted\']: '+animeFilter['restricted']);
	writeData();
}
/* *
 * Prepares the page, also fetches XML data
 */
function prepPage() {
  uriObj = parseURI();
  //if (uriObj['show'] && uriObj['show'] != 'mylist') return;
  loadData('mylist.xml',parseData);
  var uls = document.getElementsByTagName('UL');
  var stateUL = getElementsByClassName(uls,'state',false)[0];
  if (stateUL) {
		//initToggle(stateUL,'expand');
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) { as[i].onclick = filterByState; as[i].style.cursor = 'pointer'; }
  }
  stateUL = getElementsByClassName(uls,'filters',false)[0];
  if (stateUL) {
		//initToggle(stateUL,'expand');
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) { as[i].onclick = filterByStatus; as[i].style.cursor = 'pointer'; }
  }
  stateUL = getElementsByClassName(uls,'animecat',false)[0];
  if (stateUL) {
		//initToggle(stateUL,'expand');
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) { as[i].onclick = filterByType; as[i].style.cursor = 'pointer'; }
  }
  stateUL = getElementsByClassName(uls,'g_jumplist',false);
  if (stateUL) {
    for (var j in stateUL) {
      var jumpList = stateUL[j];
      var lis = jumpList.getElementsByTagName('LI');
      for (var i = 0; i < lis.length; i++) {
        var li = lis[i];
        if (li.className == 'prev') li.onclick = prevPage;
        else if (li.className == 'next') li.onclick = nextPage;
        else if (li.className == 'jump') continue;
        else li.onclick = filterByLetter;
        li.getElementsByTagName('A')[0].style.cursor = 'pointer';
      }
    }
  }
  stateUL = getElementsByClassName(uls,'wishlisttype',false)[0];
  if (stateUL) {
		//initToggle(stateUL,'fold');
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) { as[i].onclick = filterByWishlistType; as[i].style.cursor = 'pointer'; }
  }
  stateUL = getElementsByClassName(uls,'wishlistpri',false)[0];
  if (stateUL) {
		//initToggle(stateUL,'fold');
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) { as[i].onclick = filterByWishlistPriority; as[i].style.cursor = 'pointer'; }
  }
  stateUL = getElementsByClassName(uls,'user',false)[0];
  if (stateUL) {
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) { 
			var a = as[i];
			if (a.firstChild.nodeValue.search(/wishlist/i) < 0) continue;
			a.onclick = switchLayout;
			a.style.cursor = 'pointer'; 
		}
  }
	var eppUL = getElementsByClassName(uls,'g_numonpage',true);
	if (eppUL) {
    for (var j in eppUL) {
      var epps = eppUL[j];
      var lis = epps.getElementsByTagName('LI');
      for (var i = 0; i < lis.length; i++) {
        var li = lis[i];
				var a = li.getElementsByTagName('A')[0];
				if (a.firstChild.nodeValue == entriesPerPage) a.style.fontWeight = 'bold';
        a.onclick = setEpp;
        li.style.cursor = 'pointer';
      }
    }
  }
	var typeUL = getElementsByClassName(uls,'g_navlist type',true);
	if (typeUL) {
    for (var j in typeUL) {
      var type = typeUL[j];
      var lis = type.getElementsByTagName('LI');
      for (var i = 0; i < lis.length; i++) {
        var li = lis[i];
				var a = li.getElementsByTagName('A')[0]
        a.onclick = changeVotesType;
        li.style.cursor = 'pointer';
      }
    }
  }
	var restrcitedDIV = getElementsByClassName(document.getElementsByTagName('DIV'),'g_filterlist',true);
	if (restrcitedDIV) {
    for (var j in restrcitedDIV) {
      var restricted = restrcitedDIV[j];
      var as = restricted.getElementsByTagName('A');
      for (var i = 0; i < as.length; i++) {
				as[i].onclick = changeRestrictedFilter;
				as[i].style.cursor = 'pointer';
			}
    }
  }
}

//window.onload = prepPage;
addLoadEvent(prepPage);