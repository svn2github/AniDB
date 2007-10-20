/* *
 * @file Search interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (15.10.2007)
 */
 
// GLOBALs //

var seeDebug = 0;
var seeTimes = 0;

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
  //renderPage();
  //showPage(curpage);
}

function prepPage() {
  uriObj = parseURI();
  //if (uriObj['show'] && uriObj['show'] != 'search') return;
  loadData('mylist.xml',parseData);
}

//window.onload = prepPage;
addLoadEvent(prepPage);