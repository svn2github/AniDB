/* *
 * @file AnimePage interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.1 (05.09.2007)
 */
 
// GLOBALs //

var seeDebug = false;
var seeTimes = false;
var filelistTableRow = null;
var fileRow = null;

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
  renderPage();
}

function showAllHiddenSpans() {
	var spans = this.parentNode.parentNode.getElementsByTagName('SPAN');
	for (var c = 0; c < spans.length; c++) spans[c].style.display = '';
	this.parentNode.parentNode.removeChild(this.parentNode);
}

function renderPage() {
	updateStatus('Rendering page...');
	var elems = document.getElementsByTagName('ANIME.TITLE.MAIN');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(anime.title),elems[0]);
	elems = document.getElementsByTagName('ANIME.ALINK');
	while (elems.length) {
		var a = createLink(null, 'a'+anime.id, 'http://anidb.net/a'+anime.id, null, null, null, 'short_link');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.TYPE');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(anime.type),elems[0]);
	elems = document.getElementsByTagName('ANIME.RELEASE');
	while (elems.length) {
		var text = anime.type + ((anime.type != 'Movie') ? ', '+anime.neps['cnt'] + ' episodes': '');
		var tooltip;
		if (anime.dates['start'] != anime.dates['end'] && anime.dates['start'].length)
			tooltip = 'aired from: ' + anime.dates['start'] + ' until ' + anime.dates['end'];
		else if (anime.dates['start'] == anime.dates['end'] && anime.dates['start'].length)
			tooltip += 'aired in: '+anime.dates['start'];
		var a = createLink(null, anime.year, null,null,null, (tooltip ? tooltip : null), null);
		var span = document.createElement('SPAN');
		span.appendChild(document.createTextNode(text+', '));
		span.appendChild(a);
		elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.TYPEINFO');
	while (elems.length) {
		var text = anime.type + ((anime.type != 'Movie') ? ', '+anime.neps['cnt'] + ' episodes': '');
		elems[0].parentNode.replaceChild(document.createTextNode(text),elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.YEARINFO');
	while (elems.length) {
		var text;
		if (anime.dates['start'] != anime.dates['end'] && anime.dates['start'].length)
			text = 'aired: from ' + anime.dates['start'] + ' until ' + anime.dates['end'];
		else if (anime.dates['start'] == anime.dates['end'] && anime.dates['start'].length)
			text = 'aired: in '+anime.dates['start'];
		var a = createLink(null, anime.year, null,null,null, (text ? text : null), null);
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.YEAR');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(anime.year),elems[0]);
	elems = document.getElementsByTagName('ANIME.EPS');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(anime.neps['cnt']),elems[0]);
	elems = document.getElementsByTagName('ANIME.RATING');
	while (elems.length) {
		var span = document.createElement('SPAN');
		span.className = "rating "+((anime.rating['type'] != 'normal') ? 'tmp' : '')+"anime mid";
		if (anime.rating['votes']) {
			createLink(span, anime.rating['rating'], base_url+'?show=animevotes&aid='+anime.id,null,null,null,null);
			span.appendChild(document.createTextNode(' ('));
			var span2 = document.createElement('SPAN');
			span2.className = 'count';
			span2.appendChild(document.createTextNode(anime.rating['votes']));
			span.appendChild(span2);
			span.appendChild(document.createTextNode(')'));
		}	else span.appendChild(document.createTextNode('- (0)'));
		elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.REVIEWS');
	while (elems.length) {
		var span = document.createElement('SPAN');
		span.className = "rating attavg mid";
		if (anime.reviews) { 
			createLink(span, anime.rrating, base_url+'?show=animeatt&aid='+anime.id,null,null,null,null);
			span.appendChild(document.createTextNode(' ('));
			var span2 = document.createElement('SPAN');
			span2.className = 'count';
			span2.appendChild(document.createTextNode(anime.reviews));
			span.appendChild(span2);
			span.appendChild(document.createTextNode(')'));
		}	else span.appendChild(document.createTextNode('- (0)'));
		elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.TITLE.OFFICIAL');
	while (elems.length) {
		var span = document.createElement('SPAN');
		var title = anime.getTitle('official',animeOfficialTitleLang);
		var lang = anime.temp;
		var div = document.createElement('DIV');
		div.className = 'icons';
		if (lang) createIcon(div,'['+lang+'] ', null, null, 'language: '+languageMap[lang]['name'], 'i_audio_'+lang);
		span.appendChild(div);
		span.appendChild(document.createTextNode(title+' '));
		createLink(span, '[more]', 'titles.html?show=titles&aid='+anime.id, null, null, 'show other languages and/or alias', null);
		elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.TITLES.OFFICIAL');
	while (elems.length) {
		var span = document.createElement('SPAN');
		if (anime.titles['official']) {
			var cnt = 0;
			for (var lang in anime.titles['official']) {
				var title = anime.titles['official'][lang];
				cnt++;
				var span2 = document.createElement('SPAN');
				var div = document.createElement('DIV');
				span2.className = 'title';
				if (cnt > 3) span2.style.display = 'none';
				div.className = 'icons';
				if (lang) createIcon(div,'['+lang+'] ', null, null, 'language: '+languageMap[lang]['name'], 'i_audio_'+lang);
				span2.appendChild(div);
				span2.appendChild(document.createTextNode(title));
				span.appendChild(span2);
			}
			if (cnt > 3) {
				var span2 = document.createElement('SPAN');
				var a = createLink(null, '[more]', 'removeme', null, showAllHiddenSpans, 'show all official titles for this anime', null);
				a.removeAttribute('href');
				var span2 = document.createElement('SPAN');
				span2.appendChild(document.createTextNode('... '));
				span2.appendChild(a);
				span.appendChild(span2);
			}
		} else {
			if (elems[0].parentNode.nodeName == 'TD') {
				var tr = elems[0].parentNode.parentNode;
				tr.parentNode.removeChild(tr);
			}
		}
		if (elems[0]) elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.TITLES.SHORT');
	while (elems.length) {
		var span = document.createElement('SPAN');
		if (anime.titles['short']) {
			var cnt = 0;
			for (var c = 0; c < anime.titles['short'].length; c++) {
				var title = anime.titles['short'][c];
				cnt++;
				var span2 = document.createElement('SPAN');
				if (cnt > 5) span2.style.display = 'none';
				span2.appendChild(document.createTextNode(title));
				if (c < anime.titles['short'].length - 1) span2.appendChild(document.createTextNode(', '));
				span.appendChild(span2);
			}
			if (cnt > 5) {
				var span2 = document.createElement('SPAN');
				var a = createLink(null, '[more]', 'removeme', null, showAllHiddenSpans, 'show all short titles for this anime', null);
				a.removeAttribute('href');
				var span2 = document.createElement('SPAN');
				span2.appendChild(document.createTextNode('... '));
				span2.appendChild(a);
				span.appendChild(span2);
			}
		} else {
			if (elems[0].parentNode.nodeName == 'TD') {
				var tr = elems[0].parentNode.parentNode;
				tr.parentNode.removeChild(tr);
			}
		}
		if (elems[0]) elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.TITLES.ALIAS');
	while (elems.length) {
		var span = document.createElement('SPAN');
		if (anime.titles['syn']) {
			var cnt = 0;
			for (var c = 0; c < anime.titles['syn'].length; c++) {
				var title = anime.titles['syn'][c];
				cnt++;
				var span2 = document.createElement('SPAN');
				if (cnt > 3) span2.style.display = 'none';
				span2.appendChild(document.createTextNode(title));
				if (c < anime.titles['syn'].length - 1) span2.appendChild(document.createTextNode(', '));
				span.appendChild(span2);
			}
			if (cnt > 3) {
				var span2 = document.createElement('SPAN');
				var a = createLink(null, '[more]', 'removeme', null, showAllHiddenSpans, 'show all synomnys for this anime', null);
				a.removeAttribute('href');
				var span2 = document.createElement('SPAN');
				span2.appendChild(document.createTextNode('... '));
				span2.appendChild(a);
				span.appendChild(span2);
			}
		} else {
			if (elems[0].parentNode.nodeName == 'TD') {
				var tr = elems[0].parentNode.parentNode;
				tr.parentNode.removeChild(tr);
			}
		}
		if (elems[0]) elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.DESCRIPTION');
	while (elems.length) {
		//elems[0].parentNode.replaceChild(document.createTextNode(anime.other),elems[0]);
		elems[0].parentNode.replaceChild(convertStringToDom(anime.other),elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.RESOURCES');
	while (elems.length) {
		var div = document.createElement('DIV');
		div.className = 'icons';
		createIcon(div, 'mylist', 'mylist.html?show=mylist', null, 'back to mylist', 'i_backtomylist');
		createIcon(div, 'anidb', 'http://anidb.net/a'+anime.id, null, 'link to anime page at anidb', 'i_anidb');
		if (anime.resources['url']) createIcon(div, 'url', anime.resources['url']['link'], null, 'link to website', 'i_url');
		if (anime.resources['ann']) createIcon(div, 'ann', anime.resources['ann']['link'], null, 'link to anime page at news network site', 'i_ann');
		if (anime.resources['animenfo']) createIcon(div, 'anfo', anime.resources['animenfo']['link'], null, 'link to anime page at animenfo', 'i_animenfo');
		if (anime.resources['animeplanet']) createIcon(div, 'aplanet', anime.resources['animeplanet']['link'], null, 'link to anime page at animeplanet', 'i_animeplanet');
		if (anime.resources['allcinema']) createIcon(div, 'allcinema', anime.resources['allcinema']['link'], null, 'link to anime page at allcinema', 'i_allcinema');
		elems[0].parentNode.replaceChild(div,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.GENRES');
	while (elems.length) {
		if (anime.genres && anime.genres.length) {
			var span = document.createElement('SPAN');
			for (var g = 0; g < anime.genres.length; g++) {
				var genre = anime.genres[g];
				createLink(span, genre['name'], 'adv_search.html?show=results&searchby=genre&id='+genre['id'], null, null, null, null);
				if (g < anime.genres.length-1) span.appendChild(document.createTextNode(', '));
			}
			elems[0].parentNode.replaceChild(span,elems[0]);
		} else {
			if (elems[0].parentNode.nodeName == 'TD') {
				var tr = elems[0].parentNode.parentNode;
				tr.parentNode.removeChild(tr);
			}
		}
	}
	elems = document.getElementsByTagName('ANIME.CATS');
	while (elems.length) {
		var span = document.createElement('SPAN');
		if (anime.cats && anime.cats.length) {
			var cnt = 0;
			for (var g = 0; g < anime.cats.length; g++) {
				cnt++;
				var span2 = document.createElement('SPAN');
				span2.className = 'cat';
				if (cnt > 5) span2.style.display = 'none';
				var cat = anime.cats[g];
				createLink(span2, cat['name'], 'adv_search.html?show=results&searchby=cat&id='+cat['id'], null, null, null, null);
				if (g < anime.cats.length-1) span2.appendChild(document.createTextNode(', '));
				span.appendChild(span2);
			}
			if (cnt > 5) {
				var a = createLink(null, '[more]', 'removeme', null, showAllHiddenSpans, 'show all categories for this anime', null);
				a.removeAttribute('href');
				var span2 = document.createElement('SPAN');
				span2.appendChild(document.createTextNode('... '));
				span2.appendChild(a);
				span.appendChild(span2);
			}
			elems[0].parentNode.replaceChild(span,elems[0]);
		} else {
			if (elems[0].parentNode.nodeName == 'TD') {
				var tr = elems[0].parentNode.parentNode;
				tr.parentNode.removeChild(tr);
			}
		}
	}
	elems = document.getElementsByTagName('ANIME.PRODUCERS');
	while (elems.length) {
		if (anime.companys && anime.companys.length) {
			var span = document.createElement('SPAN');
			for (var c = 0; c < anime.companys.length; c++) {
				var acompany = anime.companys[c];
				var company = companys[acompany['id']];
				createLink(span, company['name'], 'adv_search.html?show=results&searchby=company&id='+acompany['id'], null, null, acompany['type'], null);
				if (c < anime.companys.length-1) span.appendChild(document.createTextNode(', '));
			}
			elems[0].parentNode.replaceChild(span,elems[0]);
		} else {
			if (elems[0].parentNode.nodeName == 'TD') {
				var tr = elems[0].parentNode.parentNode;
				tr.parentNode.removeChild(tr);
			}
		}
	}
	elems = document.getElementsByTagName('ANIME.AWARDS');
	while (elems.length) {
		var span = document.createElement('SPAN');
		for (var a = 0; a < anime.awards.length; a++) {
			var award = anime.awards[a];
			if (!award['award'].length) continue;
			var ahref = createLink(null, null, award['url'], null, null, null, null);
			var img = document.createElement('IMG');
			img.src = 'pics/a'+award['id']+'.jpg';
			img.title = award['award'];
			img.alt = award['award'];
			img.setAttribute('anidb:url',award['url']);
			img.onerror = function onerror(event) { 
				this.parentNode.replaceChild(document.createTextNode(this.title),this); 
			}
			ahref.appendChild(img);
			span.appendChild(ahref);
			if (a < anime.awards.length-1) span.appendChild(document.createTextNode(' '));
		}
		elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = document.getElementsByTagName('ANIME.WISHLIST.STATUS');
	while (elems.length) {
		var span = document.createElement('SPAN');
		if (anime.wishlist) {
			var pri = anime.wishlist['pri'];
			if (pri == 'medium') pri = 'med';
			var div = document.createElement('DIV');
			div.className = 'icons';
			span.appendChild(div);
			span.appendChild(document.createTextNode('type: '+anime.wishlist['type']));
			createIcon(div, '[w]', null, null, 'priority: '+anime.wishlist['pri'],'i_pri_'+pri);
		} else {
			if (elems[0].parentNode.nodeName == 'TD') {
				var tr = elems[0].parentNode.parentNode;
				tr.parentNode.removeChild(tr);
			}
		}
		if (elems[0]) elems[0].parentNode.replaceChild(span,elems[0]);
	}
	if (!anime.neps['user'] && !anime.seps['user']) {
		var mylistTable = getElementsByClassName(document.getElementsByTagName('TABLE'),'llist',false)[0];
		if (mylistTable && !anime.neps['user'] && !anime.seps['user']) mylistTable.parentNode.removeChild(mylistTable);
	} else {
		elems = document.getElementsByTagName('ANIME.MYLIST.STATUS');
		while (elems.length) {
			var div = document.createElement('DIV');
			div.className = 'icons';
			var state = anime.state;
			if (state) { // Wishlisted anime with no files don't have state
				if (state.indexOf('hdd') >= 0) state = 'onhdd';
				if (state.indexOf('cd') >= 0) state = 'oncd';
				createIcon(div, '['+anime.state+']', null, null, 'state: '+anime.state,'i_state_'+state);
			}
			elems[0].parentNode.replaceChild(div,elems[0]);
		}
		elems = document.getElementsByTagName('ANIME.MYLIST.EPS');
		while (elems.length) {
			var eps = anime.neps['user'] + '/' + anime.neps['cnt']+(anime.seps['user'] ? '+' + anime.seps['user'] : '');
			if (anime.complete) elems[0].parentNode.className += ' complete';
			elems[0].parentNode.replaceChild(document.createTextNode(eps),elems[0]);
		}
		elems = document.getElementsByTagName('ANIME.MYLIST.WATCHED');
		while (elems.length) {
			var seen = anime.neps['seen'] + '/' + anime.neps['cnt']+(anime.seps['seen'] ? '+' + anime.seps['seen'] : '');
			if (anime.watched) elems[0].parentNode.className += ' watched';
			elems[0].parentNode.replaceChild(document.createTextNode(seen),elems[0]);
		}
	}
	elems = document.getElementsByTagName('ANIME.IMAGE');
	while (elems.length) {
		var img = document.createElement('IMG');
		img.src = 'pics/i'+anime.id+'.jpg';
		img.alt = '';
		img.onerror = function onerror(event) { this.parentNode.removeChild(this); }
		elems[0].parentNode.replaceChild(img,elems[0]);
	}
	updateStatus('Rendering group list...');
	renderGroupList();
	updateStatus('Rendering episode list...');
	renderEpisodeList();
	updateStatus('');
}

function renderGroupList() {
	var groupTable = getElementsByClassName(document.getElementsByTagName('TABLE'),'grouplist',false)[0];
	if (!groupTable) return;
	if (anime.groups.length <= 1) { groupTable.parentNode.removeChild(groupTable); return; }
	var cloneTBody = document.createElement('TBODY');
	cloneTBody.appendChild(groupTable.tBodies[0].rows[0]);
	var cnt = -1;
	for (var g = 0; g < anime.groups.length; g++) {
		var group = groups[anime.groups[g]];
		if (!group || !group.id) continue;
		var row = cloneTBody.rows[0].cloneNode(true);
		row.id = 'gid_'+group.id;
		row.className = group.state;
		cnt++;
    row.className += (cnt % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
		// Work
		var elems = row.getElementsByTagName('ANIME.GROUP.EXPAND');
		while (elems.length) {
			elems[0].parentNode.setAttribute('anidb:sort','0');
			var a = document.createElement('A');
			var img = document.createElement('IMG');
			img.src = 'css/icons/plus.gif';
			img.title = 'expand all entries';
			img.alt = '(+)';
			img.width = 15;
			img.height = 13;
			a.appendChild(img);
			a.onclick = expandFilesByGroup;
			elems[0].parentNode.replaceChild(a,elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.GROUP.LASTUPDATE');
		while (elems.length) {
			if (Number(new Date())/1000 - Number(javascriptDate(cTimeDate(group.lastUp)))/1000 < 86400) 
				elems[0].parentNode.className += ' new';
			elems[0].parentNode.replaceChild(document.createTextNode(cTimeDate(group.lastUp)),elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.GROUP.NAME');
		while (elems.length) {
			var a = createLink(null,group.shortName,base_url+'animedb.pl?show=group&gid=' + group.id, null, null, group.name, null);
			elems[0].parentNode.replaceChild(a,elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.GROUP.STATE');
		while (elems.length) {
			elems[0].parentNode.className += ' '+group.state;
			elems[0].parentNode.replaceChild(document.createTextNode(group.state),elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.GROUP.NOTE');
		while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('-'),elems[0]);
		elems = row.getElementsByTagName('ANIME.GROUP.EPS');
		while (elems.length) {
			elems[0].parentNode.setAttribute('anidb:sort',String(group.epCnt));
			var maps = {'0' : {'use':true,'type': 0,'desc':"",'img':"blue"},
									'1' : {'use':false,'type': 1,'desc':"done: "+group.epRange,'img':"darkblue"},
									'2' : {'use':false,'type': 2,'desc':"in mylist: "+group.inMylistRange,'img':"lime"}};
			var range = expandRange(null,anime.neps['cnt'],maps[0],null);
			if (group.epRange != '' && group.epRange != '0') { maps[1]['use'] = true; range = expandRange(group.epRange,anime.neps['cnt'],maps[1],range); }
			if (group.isInMylistRange != '') { maps[2]['use'] = true; range = expandRange(group.isInMylistRange,anime.neps['cnt'],maps[2],range); }
			var span = makeCompletionBar(null,range,maps);
			elems[0].parentNode.replaceChild(span,elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.GROUP.LASTEP');
		while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(group.lastEp),elems[0]);
		elems = row.getElementsByTagName('ANIME.GROUP.SPECIALS');
		while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(group.sepCnt),elems[0]);
		elems = row.getElementsByTagName('ANIME.GROUP.LANGUAGES');
		while (elems.length) {
			var icons = document.createElement('DIV');
			icons.className = 'icons';
			if (group.audioLangs) {
				for (var i = 0; i < group.audioLangs.length; i++)
					createIcon(icons, mapLanguage(group.audioLangs[i]), '', '', 'audio lang: '+mapLanguage(group.audioLangs[i]), 'i_audio_'+group.audioLangs[i]);
			}
			if (group.subtitleLangs) {
				for (var i = 0; i < group.subtitleLangs.length; i++)
					createIcon(icons, mapLanguage(group.subtitleLangs[i]), '', '', 'sub lang: '+mapLanguage(group.subtitleLangs[i]), 'i_sub_'+group.subtitleLangs[i]);  
			}
			elems[0].parentNode.replaceChild(icons,elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.GROUP.RATING');
		while (elems.length) {
			elems[0].parentNode.setAttribute('anidb:sort',(group.rating == '-') ? '0' : group.rating);
			var span = createLink(null,((group.rating == '-') ? 'N/A' : group.rating) + ' ('+group.ratingCount+')',base_url+'animedb.pl?show=animegroupvotes&agid='+group.agid,'anidb::popup',null,null,'350.400.1.1.agvotes');
			elems[0].parentNode.replaceChild(span,elems[0]);
		}
		elems = row.getElementsByTagName('ANIME.GROUP.CMTS');
		while (elems.length) {
			elems[0].parentNode.setAttribute('anidb:sort',group.commentCount);
			var span = createLink(null,group.commentCount,base_url+'animedb.pl?show=agcmts&agid='+group.agid,'anidb::popup',null,null,'600.500.1.1.agcmts action');
			elems[0].parentNode.replaceChild(span,elems[0]);
		}
		groupTable.tBodies[0].appendChild(row);
	}
}

function expandFilesByGroup() { }
function foldFilesByGroup() { }

function createEpIcons(parentNode,episode) {
	if (episode.watched) createIcon(parentNode, 'watched', null, null, 'watched', 'i_seen');
	var state = episode.state;
	if (state)
		if (state) { // Wishlisted anime with no files don't have state
		if (state.indexOf('hdd') >= 0) state = 'onhdd';
		if (state.indexOf('cd') >= 0) state = 'oncd';
		createIcon(parentNode, '['+anime.state+']', null, null, 'state: '+anime.state,'i_state_'+state);
	}
}

function renderEpisodeList() {
	var epTable = getElementsByClassName(document.getElementsByTagName('TABLE'),'eplist',false)[0];
	if (!epTable) return;
	if (!anime.neps['user'] && !anime.seps['user']) { epTable.parentNode.removeChild(epTable); return; }
	var cloneTBody = document.createElement('TBODY');
	cloneTBody.appendChild(epTable.tBodies[0].rows[0]);
	filelistTableRow = document.createElement('TBODY');
	filelistTableRow.appendChild(epTable.tBodies[0].rows[0]);
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
		epTable.tBodies[0].appendChild(row);
	}
}

function createFileIcons(file,icons) {
  if (file.type != 'generic') {
    var tooltip = 'type: '+file.type+' | added: '+cTimeDate(file.dates['add']);
    if (file.dates['rel'] > 0) tooltip += ' | released: '+cTimeDate(file.dates['rel']);
    if (file.fileType != '') tooltip += ' | extension: '+file.fileType;
    createIcon(icons, file.type, null, null, tooltip, 'i_ftype_'+file.type);
  }
  // /TEST //
  if (file.type == 'video') { // VIDEO STREAM
    for (var st in file.videoTracks) {
      var stream = file.videoTracks[st];
      if (stream && stream.codec) {
        var res;
        var vc_RXP = /[^\w]/ig;
        var vcodec = 'unknown';
        if (stream.codec) vcodec = stream.codec.toLowerCase();
        var vstyle = vcodec.replace(vc_RXP,'_');
        var tooltip = 'video';
        if (file.resolution != 'unknown') tooltip += ' | resolution: '+file.resolution;
        if (stream.codec != 'unknown') tooltip += ' | codec: '+stream.codec;
        if (stream.ar != 'unknown')tooltip += ' | ar: '+stream.ar;
        if (file.length) tooltip += ' | duration: '+formatFileLength(file.length,'long');
        createIcon(icons, 'vid0 ', null, null, tooltip, 'i_videostream_'+vstyle);
      }
    }
  }
  if (file.type == 'video' || file.type == 'audio') { // AUDIO STREAM
    for (var st in file.audioTracks) {
      var stream = file.audioTracks[st];
      if (stream && stream.lang) {
        var tooltip = 'audio: '+mapLanguage(stream.lang);
        if (st < 2) { // EXP only exports info other than lang for the first 2 streams
          if (stream.codec != 'unknown') tooltip += ' | codec: '+stream.codec;
          if (stream.chan != 'other') tooltip += ' | channels: '+mapAudioChannels(stream.chan);
          if (stream.type) tooltip += ' | type: '+mapAudioType(stream.type);
          if (file.length) tooltip += ' | duration: '+formatFileLength(file.length,'long');
        }
        createIcon(icons, 'aud'+st+'.'+stream.lang+' ', null, null, tooltip, 'i_audio_'+stream.lang);
      }
    }
  }
  if (file.type == 'video' || file.type == 'subtitle' || file.type == 'other') { // SUBTITLE STREAM
    for (var st in file.subtitleTracks) {
      var stream = file.subtitleTracks[st];
      if (stream && stream.lang) {
        var tooltip = 'subtitle: '+mapLanguage(stream.lang);
        if (st < 2) {
         if (stream.type && stream.type != 'unknown') tooltip += ' | type: '+mapSubTypeData(stream.type);
         if (stream.flags) tooltip += ' | flags: '+mapSubFlagData(stream.flags);
        }
        createIcon(icons, 'sub'+st+'.'+stream.lang+' ', null, null, tooltip, 'i_sub_'+stream.lang);
      }
    }
  }
  // FILE CRC STATUS
  if (file.crcStatus == 'valid') createIcon(icons, 'crc.ok ', null, null, 'crc *matches* official crc ('+file.crc32.toUpperCase()+')', 'i_crc_yes');
  else if (file.crcStatus == 'invalid') createIcon(icons, 'crc.bad ', null, null, 'crc *does not match* official crc', 'i_crc_no');
  else if (file.type != 'generic') createIcon(icons, 'crc.unv ', null, null, 'crc *not* compared to official crc', 'i_crc_unv');
  // FILE VERSION
  if (file.version != 'v1') createIcon(icons, file.version+' ', null, null, 'version *'+file.version.charAt(1)+'* (error corrections)', 'i_vrs_'+file.version.charAt(1));
  // FILE CENSOR STATUS
  if (file.isCensored) createIcon(icons, 'cen ', null, null, '*censored*', 'i_censored');
  if (file.isUncensored) createIcon(icons, 'uncen ', null, null, '*uncensored*', 'i_uncensored');
  // FILE<->EP RELATIONS
  if (file.epRelations && file.epRelations[file.episodeId]) { // Do some work
    var rel = file.epRelations[file.episodeId];
    var tooltip = 'File covers: [';
    for (var bar = 0; bar < rel['startp']/10; bar++) tooltip += '-';
    for (var bar = rel['startp']/10; bar < rel['endp']/10; bar++) tooltip += '=';
    for (var bar = rel['endp']/10; bar < 10; bar++) tooltip += '-';
    createIcon(icons, '['+rel['startp']+'-'+rel['endp']+']', null, null, tooltip+']', 'i_file2ep_rel');
  }
  // FILE<->FILE RELATIONS
  if (file.fileRelations && file.fileRelations.length) { // Do some work
    for (var r in file.fileRelations) {
      var rel = file.fileRelations[r];
      if (rel['relfile'] == undefined) continue;
      var tooltip = 'File relation of type \"'+rel['type']+'\" \"'+rel['dir']+'\" with file id \"'+rel['relfile']+'\"';
      createIcon(icons,'[rel: '+ rel['relfile'] + ']', null, null, tooltip, 'i_file2file_rel');
    }
  }
  // FILE COMMENT
  if ((file.other) && (file.other != '') && (file.other != undefined)) createIcon(icons, 'comment ', null, null, 'comment: '+file.other, 'i_comment');
  // NEW FILE
  if (file.newFile) createIcon(icons, 'new.file ', null, null, '*new file* (last 24h)', 'i_new_icon');
  // MYLIST
  var temp = new Array();
	var mylistEntry = file.mylist;
  createIcon(icons, 'in.mylist ', null, null, '*in mylist*', 'i_mylist');
  // Mylist status icon
  var tooltip = 'mylist status: '+mylistEntry.status;
	if (mylistEntry.storage && mylistEntry.storage != '') tooltip += ' | storage: '+mylistEntry.storage;
	var className = mapMEStatusName(mylistEntry.status);
	createIcon(icons, mylistEntry.status+' ', null, null, tooltip, 'i_state_'+className);
	// Mylist FileState
	if (mylistEntry.fstate && ( mylistEntry.fstate != 'unknown' && mylistEntry.fstate != 'normal/original' )) {
		var className = mapFState(mylistEntry.fstate);
		var tooltip = 'mylist state: '+mylistEntry.fstate;
		createIcon(icons, mylistEntry.fstate+' ', null, null, tooltip, className);
	}
	// Seen status
	if (mylistEntry.seen) createIcon(icons, 'seen ', null, null, 'seen on: '+cTimeDate(mylistEntry.seenDate), 'i_seen');
	// mylist comment
	if ((mylistEntry.other) && (mylistEntry.other != '') && (mylistEntry.other != undefined)) createIcon(icons, 'mylist comment ', null, null, 'mylist comment: '+mylistEntry.other, 'i_comment');
}

function createFileList(eid,nbody) {
	updateStatus('Rendering file list...');
	var fileTable, body;
	if (!nbody) {
		fileTable = filelistTableRow.rows[0].cloneNode(true);
		fileTable.id = eid+'_filelist';
	  body = fileTable.getElementsByTagName('TBODY')[0];
	} else body = nbody;
	var episode = episodes[eid.substr(4,eid.length)];
	if (!episode) return;
	var cnt = -1;
	for (var i = 0; i < episode.files.length; i++) {
		var file = files[episode.files[i]];
		if (!file) continue;
		var row = fileRow.rows[0].cloneNode(true);
		row.id = 'fid_'+file.id;
		cnt++;
    row.className += (cnt % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
		// work
		var elems = row.getElementsByTagName('ANIME.FILE.ID');
		while (elems.length) {
			var a = createLink(null, file.id, 'file.html?show=file&fid='+file.id+'&aid='+file.animeId+'&eid='+file.episodeId, null, null, null, null);
			elems[0].parentNode.replaceChild(a,elems[0]);
		}
		if (file.type != 'generic') {
			elems = row.getElementsByTagName('ANIME.FILE.SIZE');
			while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(formatFileSize(file.size,false)),elems[0]);
			elems = row.getElementsByTagName('ANIME.FILE.GROUP');
			while (elems.length) {
				var group = groups[file.groupId];
				if (!group) continue;
				var a = createLink(null, group.shortName, base_url+'animedb.pl?show=group&gid=' + group.id, null, null, group.name, null);
				elems[0].parentNode.replaceChild(a,elems[0]);
			}
			elems = row.getElementsByTagName('ANIME.FILE.RESOLUTION');
			while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(file.resolution),elems[0]);
			elems = row.getElementsByTagName('ANIME.FILE.SOURCE');
			while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(file.source),elems[0]);
			elems = row.getElementsByTagName('ANIME.FILE.QUALITY');
			while (elems.length) {
				var span = buildQualityIcon(null,file.quality);
				elems[0].className += file.quality;
				elems[0].parentNode.replaceChild(span,elems[0]);
			}
			elems = row.getElementsByTagName('ANIME.FILE.ED2K');
			while (elems.length) {
				var span;
				if (file.ed2k != '') {
					elems[0].parentNode.onmouseover = createHashLink;
					if (file.crcStatus != 'invalid') span = createIcon(null, 'ed2k', "!fillme!", null, 'ed2k hash', 'i_file_ed2k');
					else createIcon(null, 'ed2k.crc.bad', "!fillme!", null, 'ed2k hash (invalid CRC file)', 'i_file_ed2k_corrupt');
				} else span = document.createTextNode('-');
				elems[0].parentNode.replaceChild(span,elems[0]);
			}
		} else {
			elems = row.getElementsByTagName('ANIME.FILE.SIZE');
			while (elems.length) {
				var a = createLink(null, 'generic file', 'http://wiki.anidb.net/w/Files:Generic_files', null, null, null, null);
				var b = document.createElement('B');
				b.appendChild(a);
				elems[0].parentNode.replaceChild(b,elems[0]);
			}
		}
		elems = row.getElementsByTagName('ANIME.FILE.ICONS');
		while (elems.length) {
			var icons = document.createElement('DIV');
			icons.className = 'icons';
			createFileIcons(file,icons);
			elems[0].parentNode.replaceChild(icons,elems[0]);
		}
		body.appendChild(row);
	}
	var epRow = document.getElementById(eid);
	if (epRow) epRow.parentNode.insertBefore(fileTable,epRow.nextSibling);
	updateStatus('');
}

function expandFilesByEp() {
	var eid=this.parentNode.parentNode.id;
	/*alert('expanding: '+eid+
				'\nfileTablePresent: '+document.getElementById(eid+'_filelist'));*/
	if (!document.getElementById(eid+'_filelist')) createFileList(eid,null);
	else document.getElementById(eid+'_filelist').style.display = '';
	this.onclick = foldFilesByEp;
	var img = this.getElementsByTagName('IMG')[0];
	img.src = 'css/icons/minus.gif';
	img.title = 'collapse all entries';
	img.alt = '[-]';
}
function foldFilesByEp() { 
	var eid=this.parentNode.parentNode.id;
	/*alert('collapsing: '+eid+
				'\nfileTablePresent: '+document.getElementById(eid+'_filelist'));*/
	if (document.getElementById(eid+'_filelist')) document.getElementById(eid+'_filelist').style.display = 'none';
	this.onclick = expandFilesByEp;
	var img = this.getElementsByTagName('IMG')[0];
	img.src = 'css/icons/plus.gif';
	img.title = 'expand all entries';
	img.alt = '[+]';
}

/* *
 * Prepares the page, also fetches XML data
 */
function prepPage() {
  uriObj = parseURI();
	if (!uriObj['show']) return;
  if (uriObj['show'] && uriObj['show'] != 'anime' && !uriObj['aid']) return;
	initTooltips();
  loadData('anime/a'+uriObj['aid']+'.xml',parseData);
}

//window.onload = prepPage;
addLoadEvent(prepPage);