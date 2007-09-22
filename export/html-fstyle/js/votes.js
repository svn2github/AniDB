/* *
 * @file Votes interfaces
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (14.09.2007)
 */

// GLOBALs //

var seeDebug = false;
var seeTimes = false;
var ginfo;

function parseData(xmldoc) {
	if (!animeOrder.length) { // to prevent double data
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
	}
  renderPage();
	showPage(curpage);
}

function changeVotesType() {
	var type = this.firstChild.nodeValue;
	clearFontWeight(this.parentNode.parentNode,this.nodeName);
	this.style.fontWeight = 'bold';
	animeFilter['myvote'] = ((type.search(/tmp/i) < 0)) ? 'normal' : 'temp';
	//alert('type: '+type+'\nanimeFilter[\'myvote\']: '+animeFilter['myvote']);
	writeData();
}

function changeRestrictedFilter() {
	var restricted = this.firstChild.nodeValue;
	clearFontWeight(this.parentNode,this.nodeName);
	this.style.fontWeight = 'bold';
	animeFilter['restricted'] = ((restricted.search(/hide/i) < 0)) ? 'hentai' : 'nohentai';
	if (restricted.search(/all/i) > -1) animeFilter['restricted'] = false;
	//alert('restricted: '+restricted+'\nanimeFilter[\'restricted\']: '+animeFilter['restricted']);
	writeData();
}

function updateAnimeRow(anime) {
  row = animeListRow.cloneNode(true);
  row.id = row.id.replace('%ANIMEID%',anime.id);
  //row.style.display = '';
  if (anime.complete) row.className = "complete ";
  if (anime.watched) row.className = "all_watched ";
	// work
	var elems = row.getElementsByTagName('ANIME.TITLE.ALINK');
	while (elems.length) {
    var title = anime.getTitle('main');
		var tooltip;
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

function writeData() {
	updateStatus('Rendering vote data...');
	var tbody = animeListTable.tBodies[0];
	clearTableRows(tbody);
	elems = document.getElementsByTagName('VOTES.TYPE');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(((animeFilter['myvote'] == 'normal') ? 'Permanent' : 'Temporary') + ' anime votes'),elems[0]);
  // first see if we have animes
  if (!animes) { alert('Do you have animes?'); return; }
	var cnt = 0;
	var avg = 0;
  for (var i = 0; i < animeOrder.length; i++) {
    var anime = animes[animeOrder[i]];
		if (!anime.myvote || (anime.myvote && anime.myvote['type'] != animeFilter['myvote'])) { anime.filtered = true; continue; }
		if (animeFilter['restricted'] == 'hentai' && !anime.restricted) { anime.filtered = true; continue; }
		if (animeFilter['restricted'] == 'nohentai' && anime.restricted) { anime.filtered = true; continue; }
		anime.filtered = false;
		//alert('found anime:\n'+anime.title+'\nvote: '+anime.myvote['vote']+' ('+anime.myvote['type']+')');
		cnt++;
		avg += Number(anime.myvote['vote']);
    var row = updateAnimeRow(anime);
    row.className += (cnt % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
		row.style.display = '';
    tbody.appendChild(row);
  }
	elems = document.getElementsByTagName('VOTES.TOTAL');
	if (elems.length) {
		while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(cnt),elems[0]);
		elems = document.getElementsByTagName('VOTES.AVERAGE');
		while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(Number(avg/cnt).toFixed(2)),elems[0]);
	} else {
		if (!ginfo) ginfo = getElementsByClassName(document.getElementsByTagName('P'),'g_info',false)[0];
		if (ginfo) {
			while (ginfo.childNodes.length) ginfo.removeChild(ginfo.childNodes[0]);
			ginfo.appendChild(document.createTextNode(cnt+' votes, average vote '+Number(avg/cnt).toFixed(2)));
		}
	}
	updateStatus('');
	curpage = 1;
	showPage(curpage);
}

function renderPage() {
	// set the filter to only normal voted animes
	animeFilter['myvote'] = 'normal';
	updateStatus('Rendering page...');
	elems = document.getElementsByTagName('USERVLINK');
	while (elems.length) elems[0].parentNode.replaceChild(createLink(null, 'v'+userInfo.id, 'http://anidb.net/v'+userInfo.id, null, null, null, 'short_link'),elems[0]);
	elems = document.getElementsByTagName('VOTES.RESOURCES');
	while (elems.length) {
		var div = document.createElement('DIV');
		div.className = 'icons';
		createIcon(div, 'mylist', 'mylist.html?show=mylist', null, 'back to mylist', 'i_backtomylist');
		elems[0].parentNode.replaceChild(div,elems[0]);
	}
  if (!animes) { alert('Do you have animes?'); return; }
  animeListTable = getElementsByClassName(document.getElementsByTagName('TABLE'),'animelist',false)[0];
  if (!animeListTable) { alert('Your html structure isn\'t valid'); return; }
  var tbody = animeListTable.tBodies[0];
  animeListRow = tbody.rows[0].cloneNode(true);
  tbody.removeChild(tbody.rows[0]);
  init_sorting(animeListTable.tHead.rows[0],'title','up');
  var ths = animeListTable.tHead.getElementsByTagName('TH');
  for (var i = 0; i < ths.length; i++) {
    ths[i].onclick = function onclick(event) {
      showPage(1);
      sortcol(this);
    }
  }
	writeData();
}

/* *
 * Prepares the page, also fetches XML data
 */
function prepPage() {
  uriObj = parseURI();
	initTooltips();
	loadData('mylist.xml',parseData);
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