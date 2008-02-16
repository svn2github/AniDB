/* *
 * @file wishlist page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.5 (07.07.2007)
 */
 
// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var undef, towatch, toget, buddyrecs, blacklist; // tableheads

function applySorting(node) {
  var ths = node.getElementsByTagName('TH');
  for (var i = 0; i < ths.length; i++) {
    var th = ths[i];
    switch (th.className) {
      case 'expand': th.className += ' c_none'; break;
      case 'anime': th.className += ' c_latin'; break;
      case 'pri': th.className += ' c_set'; break;
      case 'by': th.className += ' c_latin'; break;
      case 'nfy': th.className += ' c_set'; break;
      case 'releaseinfo': th.className += ' c_setlatin'; break;
      case 'mylist': th.className += ' c_set'; break;
      case 'seen': th.className += ' c_set'; break;
      case 'state': th.className += ' c_none'; break;
      case 'date added': th.className += ' c_date'; break;
      case 'rating': th.className += ' c_latin'; break;
      case 'tmprating': th.className = 'tmpanime c_latin'; break;
      case 'revrating': th.className = 'review c_latin'; break;
      case 'cmts': th.className += ' c_none'; break; // can break
      default: if (seeDebug) alert('unknown class: '+th.className); break;
    }
  }
}

function createTooltips(node) {
  if (!node) node = document;
  var infos = getElementsByClassName(node.getElementsByTagName('DIV'),'releaseinfotooltip',false);
  for (var i = 0; i < infos.length; i++) {
    var div = infos[i];
    var parentNode = div.parentNode;
    parentNode.title = null;
    if (parentNode.className.indexOf('g_state_none') >= 0) continue;
    parentNode.onmouseout = hideTooltip;
    parentNode.onmouseover = function onmouseover(event) { 
      var div = getElementsByClassName(this.getElementsByTagName('DIV'),'releaseinfotooltip',false)[0];
      if (div) { var ndiv = div.cloneNode(true); ndiv.style.display = ''; setTooltip(ndiv,true); }
    };
  }
}

function prepPage() {
  uriObj = parseURI(); // update the uriObj
  if (!uriObj['show'] || uriObj['show'] != 'mywishlist') return; // badPage
  initTooltips();
  var divs = document.getElementsByTagName('DIV');
  if (!divs) return;
  var dv = new Array();
  var u = getElementsByClassName(divs,'undefined',false)[0];
  var w = getElementsByClassName(divs,'to_watch',false)[0];
  var g = getElementsByClassName(divs,'to_get',false)[0];
  var br = getElementsByClassName(divs,'buddy_recommendations',false)[0];
  var b = getElementsByClassName(divs,'blacklist',false)[0];
  if (u) dv.push(u);
  if (w) dv.push(w);
  if (g) dv.push(g);
  if (br) dv.push(br);
  if (b) dv.push(b);
  for (var i = 0; i < dv.length; i++) {
    var div = dv[i];
    var tbody = div.getElementsByTagName('TBODY')[0];
    if (!tbody) continue;
    var head = tbody.rows[0];
    applySorting(head);
    var table = tbody.parentNode;
    var thead = document.createElement('THEAD');
    thead.appendChild(head);
    table.appendChild(thead);
    init_sorting(head);
    createTooltips(table);
  }
}
 
//window.onload = prePage;
addLoadEvent(prepPage);