/* *
 * @file Search interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (15.10.2007)
 */
 
// GLOBALs //

var seeDebug = 0;
var seeTimes = 0;
var animeFilter = {
	'complete':false,
	'incomplete':false,
	'restricted':false,
	'watched':false,
	'ongoing':false,
	'notwatched':false,
	'stalled':false,
	'wishlisted':false,
	'awarded':false,
	'myvote':false,
	'wishlisttype':false,
	'wishlistpri':false,
	'state':"all",
	'neps':{'type':"all",'comparator':"==",'value':-1},
	'seps':{'type':"all",'comparator':"==",'value':-1},
	'type':"all",
	'text':"",
	'genres':(new Array()),
	'cats':(new Array()),
	'size':{'comparator':"==",'value':-1},
	'rating':{'type':"all",'comparator':"==",'value':-1},
	'votes':{'type':"all",'comparator':"==",'value':-1},
	'reviews':{'comparator':"==",'value':-1},
	'reviewcnt':{'comparator':"==",'value':-1},
	'year':{'comparator':"==",'value':-1},
	'age':"all"
	};

var animeListTable;
var animeListRow;
var eplistTableRow;
var curpage = 1;
var filteredAnimes = 0;
var results = new Array();

// FUNCTIONS //

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
  for (var a in results) {
    var anime = results[a];
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
  for (var a in results) {
    var anime = results[a];
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

function renderPage() {
	updateStatus('Rendering page...');
	// see if we have animes
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
	updateStatus('');
}

/* *
 * Process the search options and start a search
 */
function processSearch() {
	animeFilter['complete'] = document.getElementById('search.complete').checked;
	animeFilter['incomplete'] = document.getElementById('search.incomplete').checked;
	animeFilter['watched'] = document.getElementById('search.watched').checked;
	animeFilter['notwatched'] = document.getElementById('search.notwatched').checked;
	var wtype = document.getElementById('search.wishlist.type').value;
	animeFilter['wishlisttype'] = (wtype == 'all' ? false : wtype);
	var wpri = document.getElementById('search.wishlist.pri').value;
	animeFilter['wishlistpri'] = (wpri == 'all' ? false : wpri);
	animeFilter['text'] = document.getElementById('search.text').value;
	animeFilter['type'] = document.getElementById('search.type').value;
	animeFilter['neps']['value'] = document.getElementById('search.neps').value;
	animeFilter['neps']['comparator'] = document.getElementById('search.neps.comparator').value;
	animeFilter['seps']['value'] = document.getElementById('search.seps').value;
	animeFilter['seps']['comparator'] = document.getElementById('search.seps.comparator').value;
	animeFilter['rating']['type'] = document.getElementById('search.rating.type').value;
	animeFilter['rating']['comparator'] = ">";
	animeFilter['rating']['value'] = document.getElementById('search.rating.rating').value;
	animeFilter['votes']['type'] = document.getElementById('search.rating.type').value;
	animeFilter['votes']['comparator'] = ">";
	animeFilter['votes']['value'] = document.getElementById('search.rating.votes').value;
	animeFilter['reviews']['comparator'] = ">";
	animeFilter['reviews']['value'] = document.getElementById('search.reviews.rating').value;
	animeFilter['reviewcnt']['comparator'] = ">";
	animeFilter['reviewcnt']['value'] = document.getElementById('search.reviews.count').value;
	animeFilter['age'] = document.getElementById('search.restricted').value;
	if (animeFilter['age'] == "censor") animeFilter['restricted'] = true;
	else animeFilter['restricted'] = false;
	processAnimes();
}

/* *
 * Process the anime list against the filters
 */
function processAnimes() {
	if (seeDebug) {
		alert("Search definitions: "+
					"\ntext: "+animeFilter['text']+
					"\ntype: "+animeFilter['type']+
					"\nneps: "+animeFilter['neps']['value']+
					"\nneps.comparator: "+animeFilter['neps']['comparator']+
					"\nseps: "+animeFilter['seps']['value']+
					"\nseps.comparator: "+animeFilter['seps']['comparator']+
					"\nrating.type: "+animeFilter['rating']['type']+
					"\nrating.comparator: "+animeFilter['rating']['comparator']+
					"\nrating.value: "+animeFilter['rating']['value']+
					"\nvotes.type: "+animeFilter['votes']['type']+
					"\nvotes.comparator: "+animeFilter['votes']['comparator']+
					"\nreviews.comparator: "+animeFilter['reviews']['comparator']+
					"\nreviews.value: "+animeFilter['reviews']['value']+
					"\nreviewcnt.comparator: "+animeFilter['reviewcnt']['comparator']+
					"\nreviewcnt.value: "+animeFilter['reviewcnt']['value']+
					"\nage: "+animeFilter['age']+
					"\nrestricted: "+animeFilter['restricted']+
					"\ncomplete: "+animeFilter['complete']+
					"\nincomplete: "+animeFilter['incomplete']+
					"\nwatched: "+animeFilter['watched']+
					"\nnotwatched: "+animeFilter['notwatched']+
					"\nwishlisted: "+animeFilter['wishlisted']+
					"\nwishlist.type: "+animeFilter['wishlisttype']+
					"\nwishlist.pri: "+animeFilter['wishlistpri']+
					"\nawarded: "+animeFilter['awarded']+
					"\nongoing: "+animeFilter['ongoing']
					);
	}
	results = new Array();
	for (var a in animes) {
		var anime = animes[a];
		if (!anime) continue;
		if (filterAnime(anime)) results.push(anime);
	}
	writeData(results);
	curpage = 1;
	showPage(curpage);
}

/* *
 * Writes the Data to the animetable
 */
function writeData(results) {
	updateStatus('Rendering page...');
	var tbody = animeListTable.tBodies[0];
	clearTableRows(tbody);
	for (var i = 0; i < results.length; i++) {
		updateStatus('Rendering page...'+(i+1)+' of '+results.length+' animes done.');
		var anime = results[i];
		anime.filtered = false;
		var row = updateAnimeRow(anime);
		row.className += (i % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
		row.style.display = "";
		tbody.appendChild(row);
	}
	updateStatus('');
}

/* *
 * This function is used to compare values based on a given operator
 */
function compareSymbol(symbol, a, b) {
  switch (symbol) {
    case '>' : return (a >  b);
    case '<' : return (a <  b);
    case '<=': return (a <= b);
    case '>=': return (a >= b);
    case '==': return (a == b);
    case '!=': return (a != b);
  }
  return 0;
};

/* *
 * Function that filters animes based on rules (AND based search)
 * @param anime Anime to compare to filter
 * @return true if anime should be displayed, false otherwise
 */
function filterAnime(anime) {
	if (!anime) return true;
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
	if (anime.type == animeFilter['type']) vis = true;
	if (animeFilter['text'] != "") {
		var textArray = animeFilter['text'].split(' '); // split on spaces
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
			if (found) vis = true;
			else vis = false;
		} else { 
			if (found == textArray.length) vis = true;
			else vis = false;
		}
	}
	if (animeFilter['neps']['value'] && animeFilter['neps']['value'] != "") {
		var targetEps = Number(animeFilter['neps']['value']);
		if (compareSymbol(animeFilter['neps']['comparator'],anime.neps['cnt'],targetEps)) vis = true;
	}
	if (animeFilter['seps']['value'] && animeFilter['seps']['value'] != "") {
		var targetEps = Number(animeFilter['seps']['value']);
		if (compareSymbol(animeFilter['seps']['comparator'],anime.seps['cnt'],targetEps)) vis = true;
	}
	return vis;
}

function prepPage() {
  uriObj = parseURI();
  //if (uriObj['show'] && uriObj['show'] != 'search') return;
  loadData('mylist.xml',parseData);
	var elems = document.getElementsByTagName('SEARCH.LANGUAGE');
	while (elems.length) {
		var select2 = createLanguageSelect(null,'search.language','search.language',null,null);
		var select = document.createElement('SELECT');
		var none = document.createElement('OPTION');
		none.value = "all";
		none.text = "none selected";
		select.appendChild(none);
		while (select2.options.length) select.appendChild(select2.options[0]);
		select.options[0].selected = true;
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('SEARCH.NEPS.COMPARATOR');
	while (elems.length) {
		var objects = {
			"==":{'text':"exactly"},
			"!=":{'text':"not"},
			">":{'text':"more than"},
			"<":{'text':"less than"},
			">=":{'text':"more or equal to"},
			"<=":{'text':"less or equal to"}};
		var select = createSelectArray(null,"search.neps.comparator","search.neps.comparator",null,"==",objects);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('SEARCH.SEPS.COMPARATOR');
	while (elems.length) {
		var objects = {
			"==":{'text':"exactly"},
			"!=":{'text':"not"},
			">":{'text':"more than"},
			"<":{'text':"less than"},
			">=":{'text':"more or equal to"},
			"<=":{'text':"less or equal to"}};
		var select = createSelectArray(null,"search.seps.comparator","search.seps.comparator",null,"==",objects);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('SEARCH.WISHLIST.TYPE');
	while (elems.length) {
		var objects = {
			"all":{'text':"all"},
			"unknown":{'text':"unknown"},
			"to get":{'text':"to get"},
			"to watch":{'text':"to watch"},
			"blacklisted":{'text':"blacklisted"}};
		var select = createSelectArray(null,"search.wishlist.type","search.wishlist.type",null,"all",objects);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('SEARCH.WISHLIST.PRI');
	while (elems.length) {
		var objects = {
			"all":{'text':"all"},
			"high":{'text':"high"},
			"med":{'text':"medium"},
			"low":{'text':"low"}};
		var select = createSelectArray(null,"search.wishlist.pri","search.wishlist.pri",null,"all",objects);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	document.getElementById('search.do').onclick = processSearch;
	//document.getElementById('search').onsubmit = processSearch;
	var uls = document.getElementsByTagName('UL');
	var stateUL = getElementsByClassName(uls,'g_jumplist',false);
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
}

//window.onload = prepPage;
addLoadEvent(prepPage);