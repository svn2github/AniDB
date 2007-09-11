/* *
 * @file EpisodePage interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (10.09.2007)
 */
 
// GLOBALs //

var seeDebug = false;
var seeTimes = false;
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

function renderPage() {
	var anime = animes[uriObj['aid']];
	var episode = episodes[uriObj['eid']];
	var elems = document.getElementsByTagName('ANIME.TITLE.MAIN');
	while (elems.length) {
		var a = createLink(null, anime.title, 'anime.html?show=anime&aid='+anime.id, null, null, null, 'short_link');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	var elems = document.getElementsByTagName('EPISODE.TITLE.MAIN');
	while (elems.length) {
		//var a = createLink(null, episode.epno + ' ' + episode.getTitle(), 'http://anidb.net/e'+episode.id, null, null, null, 'short_link');
		elems[0].parentNode.replaceChild(document.createTextNode(episode.epno + ' ' + episode.getTitle()),elems[0]);
	}
	var elems = document.getElementsByTagName('EPISODE.NUMBER');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(episode.epno),elems[0]);
	elems = document.getElementsByTagName('EPISODE.ALINK');
	while (elems.length) { 
		var a = createLink(null, 'e'+episode.id, 'http://anidb.net/e'+episode.id, null, null, null, 'short_link');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	elems = document.getElementsByTagName('EPISODE.OTHER');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(episode.other),elems[0]);
	elems = document.getElementsByTagName('EPISODE.MISC');
	while (elems.length) {
		var text;
		if (episode.isRecap) {
			text = document.createElement('B');
			text.appendChild(document.createTextNode('RECAP'));
		} else text = document.createTextNode('');
		elems[0].parentNode.replaceChild(text,elems[0]);
	}
	elems = document.getElementsByTagName('EPISODE.LENGTH');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(episode.length),elems[0]);
	elems = document.getElementsByTagName('EPISODE.DATE');
	while (elems.length) {
		var text = '';
		if (episode.dates['rel']) text = episode.dates['rel'];
		else text = 'added on '+episode.dates['add'];
		elems[0].parentNode.replaceChild(document.createTextNode(text),elems[0]);
	}
	elems = document.getElementsByTagName('EPISODE.RATING');
	while (elems.length) {
		var span = document.createElement('SPAN');
		if (episode.rating['votes']) {
			createLink(span, episode.rating['rating'], base_url+'?show=animevotes&aid='+anime.id,null,null,null,null);
			span.appendChild(document.createTextNode(' ('));
			var span2 = document.createElement('SPAN');
			span2.className = 'count';
			span2.appendChild(document.createTextNode(episode.rating['votes']));
			span.appendChild(span2);
			span.appendChild(document.createTextNode(')'));
			if (episode.myvote['vote']) span.appendChild(document.createTextNode(', my vote: '+episode.myvote['vote']));
		}	else span.appendChild(document.createTextNode('- (0)'));
		elems[0].parentNode.replaceChild(span,elems[0]);
	}
	elems = document.getElementsByTagName('EPISODE.TITLES');
	while (elems.length) {
		var span = document.createElement('SPAN');
		if (episode.titles) {
			for (var lang in episode.titles) {
				var title = episode.titles[lang];
				var span2 = document.createElement('SPAN');
				var div = document.createElement('DIV');
				span2.className = 'title';
				div.className = 'icons';
				if (lang) createIcon(div,'['+lang+'] ', null, null, 'language: '+languageMap[lang]['name'], 'i_audio_'+lang);
				span2.appendChild(div);
				span2.appendChild(document.createTextNode(title));
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
	elems = document.getElementsByTagName('EPISODE.RESOURCES');
	while (elems.length) {
		var div = document.createElement('DIV');
		div.className = 'icons';
		createIcon(div, 'mylist', 'anime.html?show=anime&aid='+anime.id, null, 'back to anime', 'i_backtomylist');
		createIcon(div, 'anidb', 'http://anidb.net/e'+episode.id, null, 'link to episode page at anidb', 'i_anidb');
		elems[0].parentNode.replaceChild(div,elems[0]);
	}
	// now show the files for this episode
	var filelist = getElementsByClassName(document.getElementsByTagName('TABLE'),'filelist',false)[0];
	if (!filelist) alert('bork');
	fileRow = document.createElement('TBODY');
	fileRow.appendChild(filelist.tBodies[0].rows[0]);
	createFileList('eid_'+episode.id,filelist.tBodies[0]);
}

/* *
 * Prepares the page, also fetches XML data
 */
function prepPage() {
  uriObj = parseURI();
  if (uriObj['show'] && uriObj['show'] != 'ep' && !uriObj['eid'] && !uriObj['aid']) return;
	initTooltips();
  loadData('anime/a'+uriObj['aid']+'.xml',parseData);
}

window.onload = prepPage;