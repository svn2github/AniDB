/* *
 * @file User statistics interfaces
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (14.09.2007)
 */

// GLOBALs //

var seeDebug = false;
var seeTimes = false;

function parseData(xmldoc) {
  updateStatus('parsing Custom node');
  var root = xmldoc.getElementsByTagName('root').item(0);
  base_url = root.getAttribute('anidb');
  var t1 = new Date();
  parseCustom(root.getElementsByTagName('custom').item(0));
  var parseCustomNode = (new Date()) - t1;
  replaceGlobals(document.body);
  if (seeTimes) { alert('Processing...'+
        '\n\tanime: '+parseAnimeNode+'ms'+
        '\n\tcustom: '+parseCustomNode+' ms'+
        '\nTotal: '+(parseAnimeNode+parseCustomNode)+' ms'); }
	updateStatus('');
  renderPage();
}

function renderPage() {
	updateStatus('Rendering page...');
	var elems = document.getElementsByTagName('EXPORT.DATE');
	while (elems.length) {
		var a = createLink(null, cTimeDate(userInfo.date), null, null, null, 'Date: '+userInfo.date, 'i_hand');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	var elems = document.getElementsByTagName('EXPORT.ANIMES');
	while (elems.length) {
		var a = createLink(null, userInfo.animes['user'], null, null, null, 'Added: '+userInfo.animes['added'], 'i_hand');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	var elems = document.getElementsByTagName('EXPORT.EPISODES');
	while (elems.length) {
		var tooltip = 'Added: '+userInfo.animes['added']+' | Viewed: '+userInfo.eps['viewed']+' ('+userInfo.eps['viewedp']+') | db owned: '+userInfo.eps['dbownedp'];
		var a = createLink(null, userInfo.eps['user'], null, null, null, tooltip, 'i_hand');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	var elems = document.getElementsByTagName('EXPORT.FILES');
	while (elems.length) {
		var a = createLink(null, userInfo.files['user'], null, null, null, 'Added: '+userInfo.files['added'], 'i_hand');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	var elems = document.getElementsByTagName('EXPORT.SIZE');
	while (elems.length) {
		var a = createLink(null, userInfo.size['short'], null, null, null, 'Total: '+userInfo.size['bytes'], 'i_hand');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	elems = document.getElementsByTagName('STATS.RESOURCES');
	while (elems.length) {
		var div = document.createElement('DIV');
		div.className = 'icons';
		createIcon(div, 'mylist', 'mylist.html?show=mylist', null, 'back to mylist', 'i_backtomylist');
		elems[0].parentNode.replaceChild(div,elems[0]);
	}
	updateStatus('');
}

/* *
 * Prepares the page, also fetches XML data
 */
function prepPage() {
  uriObj = parseURI();
	initTooltips();
  loadData('mylist.xml',parseData);
}

window.onload = prepPage;