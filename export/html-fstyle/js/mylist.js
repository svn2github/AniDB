/* *
 * @file Mylist interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (22.03.2007)
 */
 
// GLOBALs //

var animeListTable; // the animelist table
var animeListRow; // the sample animeList row
var animeFilter = {'complete':false,'incomplete':false,'restricted':false,'watched':false,'ongoing':false,
                   'notwatched':false,'stalled':false,'fileinfo':true,'wishlisted':false,'awarded':false};
var curpage = 1;
var filteredAnimes = 0;

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
  var cells = row.getElementsByTagName('TD');
  if (!cells) return;
  var cell = getElementsByClassName(cells,'title',true)[0];
  if (cell) {
    var icons = getElementsByClassName(cell.getElementsByTagName('DIV'),'icons',false)[0];
    if (icons) {
      while (icons.childNodes.length) icons.removeChild(icons.firstChild);
      mylistIcons(icons,anime);
    }
    var ahref = cell.getElementsByTagName('A')[0];
    if (ahref) {
      ahref.href = ahref.href.replace('%ANIMEID%',anime.id);
      var title = anime.getTitle('main');
      if (animeTitleDisplay == 1) title += ' ('+anime.getAltTitle('official')+')';
      ahref.firstChild.nodeValue = ahref.firstChild.nodeValue.replace('%ANIMETITLE%',title);
      if (animeTitleDisplay == 2 && anime.getAltTitle('official').length) ahref.title = 'Alternative title: '+ anime.getAltTitle('official');
      cell.setAttribute('anidb:sort',title);
    }
  }
  cell = getElementsByClassName(cells,'eps',true)[0];
  if (cell) {
    var eps = anime.neps['user'] + '/' + anime.neps['cnt']+(anime.seps['user'] ? '+' + anime.seps['user'] : '');
    cell.firstChild.nodeValue = cell.firstChild.nodeValue.replace('%ANIMEEPS%',eps);
    cell.setAttribute('anidb:sort',anime.neps['user']);
  }
  cell = getElementsByClassName(cells,'seen',true)[0];
  if (cell) {
    var seen = anime.neps['seen'] + '/' + anime.neps['cnt']+(anime.seps['seen'] ? '+' + anime.seps['seen'] : '');
    cell.firstChild.nodeValue = cell.firstChild.nodeValue.replace('%ANIMESEEN%',seen);
    cell.setAttribute('anidb:sort',anime.neps['seen']);
  }
  cell = getElementsByClassName(cells,'anime',true)[0];
  if (cell) {
    cell.firstChild.nodeValue = cell.firstChild.nodeValue.replace('%ANIMERATING%',(anime.rating['votes'] ? anime.rating['rating'] : '-'));
    var span = cell.getElementsByTagName('SPAN')[0];
    span.firstChild.nodeValue = span.firstChild.nodeValue.replace('%ANIMEVOTES%',(anime.rating['votes'] ? anime.rating['votes'] : '0'));
    cell.setAttribute('anidb:sort',(anime.rating['votes'] ? anime.rating['rating'] : '0'));
  }
  cell = getElementsByClassName(cells,'vote',true)[0];
  if (cell) {
    var vote = (anime.myvote ? anime.myvote['vote'] : '');
    if (!anime.watched) cell.className += ' none';
    else {
      if (!anime.myvote) { 
        cell.className += ' pending';
        createIcon(cell,'pending',null,null,'You have not voted for this anime yet!','i_no_vote');
      } else if (anime.myvote['type'] == 'temp') cell.className += ' temp mid';
      else if (anime.myvote['type'] == 'normal') cell.className += ' anime mid';
    }
    cell.firstChild.nodeValue = cell.firstChild.nodeValue.replace('%ANIMEMYVOTE%',vote);
    cell.setAttribute('anidb:sort',(anime.myvote ? anime.myvote['vote'] : '0'));
  }
  cell = getElementsByClassName(cells,'attavg',true)[0];
  if (cell) {
    cell.firstChild.nodeValue = cell.firstChild.nodeValue.replace('%ANIMEREVIEWRATING%',(anime.reviews ? anime.rrating : '-'));
    var span = cell.getElementsByTagName('SPAN')[0];
    span.firstChild.nodeValue = span.firstChild.nodeValue.replace('%ANIMEREVIEWCNT%',(anime.reviews ? anime.reviews : '0'));
    cell.setAttribute('anidb:sort',(anime.reviews ? anime.reviews : '0'));
  }
  cell = getElementsByClassName(cells,'action',true)[0];
  if (cell) cell.firstChild.name = cell.firstChild.name.replace('%ANIMEID%',anime.id);
  cell = getElementsByClassName(cells,'year',true)[0];
  if (cell) cell.firstChild.nodeValue = cell.firstChild.nodeValue.replace('%ANIMEYEAR%',anime.year);
  cell = getElementsByClassName(cells,'type',true)[0];
  if (cell) cell.firstChild.nodeValue = cell.firstChild.nodeValue.replace('%ANIMETYPE%',anime.type);
  cell = getElementsByClassName(cells,'size',true)[0];
  if (cell) cell.firstChild.nodeValue = cell.firstChild.nodeValue.replace('%ANIMESIZE%',anime.size);
  return row;
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
  for (var i = 0; i < animeOrder.length; i++) {
    var anime = animes[animeOrder[i]];
    var row = updateAnimeRow(anime);
    row.className += (i % 2) ? '' : ((row.className.length) ? ' ' : '') + 'g_odd';
    tbody.appendChild(row);
  }
  init_sorting(animeListTable.tHead.rows[0],'title','up');
  var ths = animeListTable.tHead.getElementsByTagName('TH');
  for (var i = 0; i < ths.length; i++) {
    ths[i].onclick = function onclick(event) {
      showPage(1);
      sortcol(this);
    }
  }
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
  var dt = javascriptDate(anime.dates['end']);
  if ((anime.dates['end'] == '?' || ( anime.dates['end'] != '?' && (new Date() < dt))) && animeFilter['ongoing']) vis = true;
  if (anime.hasawards && animeFilter['awarded']) vis = true;
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
 * Prepares the page, also fetches XML data
 */
function prepPage() {
  uriObj = parseURI();
  if (uriObj['show'] && uriObj['show'] != 'mylist') return;
  loadData('mylist.xml',parseData);
  var uls = document.getElementsByTagName('UL');
  var stateUL = getElementsByClassName(uls,'state',false)[0];
  if (stateUL) {
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) { as[i].onclick = filterByState; as[i].style.cursor = 'pointer'; }
  }
  stateUL = getElementsByClassName(uls,'filters',false)[0];
  if (stateUL) {
    var as = stateUL.getElementsByTagName('A');
    for (var i = 0; i < as.length; i++) { as[i].onclick = filterByStatus; as[i].style.cursor = 'pointer'; }
  }
  stateUL = getElementsByClassName(uls,'animecat',false)[0];
  if (stateUL) {
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
}

window.onload = prepPage;