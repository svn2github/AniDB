/* *
 * @file profile page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.0 (19.05.2007)
 */
 
// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var cssurl;
var current_css = "";
var style_index = 0;
var base_url = "http://static.anidb.net/";
var styleList = new Array();   // Stylelist array
var style_thumbnail, style_title, style_creator, style_status, style_updated, style_description;
var style_apply, style_next, style_prev;
 
function fetchData() {
  var req = xhttpRequest();
  if (''+window.location.hostname == '') { base_url = ""; xhttpRequestFetch(req, "css/stylelist.xml", parseData); }
  else { base_url = "http://static.anidb.net/"; xhttpRequestFetch(req, "../css/stylelist.xml", parseData); }
}

function CStyle(node) {
  this.name = node.getAttribute('name');
  this.title = this.name; 
  this.screenshot = "";
  this.thumbnail = "";
  this.path = "";
  this.creator = "";
  this.update = "";
  this.status = "";
  this.description = "";
  for (var i = 0; i < node.childNodes.length; i++) {
    var sNode = node.childNodes.item(i);
    if (sNode.nodeType == 3) continue; // Text node, not interested
    switch (sNode.nodeName) {
      case 'title': this.title = nodeData(sNode); break;
      case 'screenshot': this.screenshot = nodeData(sNode); break;
      case 'thumbnail': this.thumbnail = nodeData(sNode); break;
      case 'path': this.path = nodeData(sNode); break;
      case 'creator': this.creator = nodeData(sNode); break;
      case 'update': this.update = nodeData(sNode); break;
      case 'status': this.status = nodeData(sNode); break;
      case 'description': this.description = nodeData(sNode); break;
      default: showAlert('Style entry for: '+this.name, node.nodeName, node.nodeName,sNode.nodeName);
    }
  }
}

function clearImg() {
  this.onerror = null;
  this.src = base_url+"css/nopic.png";
  return;
}

function updateCurrentStyle() {
  var curStyle = styleList[style_index];
  // Thumbnail
  if (!style_thumbnail) style_thumbnail = document.getElementById('style_thumbnail');
  style_thumbnail.title = "Style: "+curStyle.name;
  style_thumbnail.alt = curStyle.name;
  //style_thumbnail.onerror = clearImg;
  if (curStyle.thumbnail != "none") {
    if (curStyle.thumbnail == "") style_thumbnail.src = base_url+"css/"+curStyle.name+"/images/thumbnail.png";
    else style_thumbnail.src = base_url+"css/"+curStyle.thumbnail;
  } else style_thumbnail.src = base_url+"css/nopic.png";
  var i_enlarge = document.getElementById('style_enlarge');
  if (curStyle.screenshot != "none") {
    if (!i_enlarge) {
      i_enlarge = document.createElement('A');
      i_enlarge.className = "i_icon i_enlarge";
      i_enlarge.title = "Show screenshot";
      i_enlarge.alt = "show screenshot";
      i_enlarge.target = "blank";
      i_enlarge.id = 'style_enlarge';
      style_thumbnail.parentNode.appendChild(i_enlarge);
    }
    if (curStyle.screenshot == "") i_enlarge.href = base_url+"css/"+curStyle.name+"/images/screenshot.png";
    else i_enlarge.href = base_url+"css/"+curStyle.screenshot;    
  }
  // Title
  if (!style_title) style_title = document.getElementById('style_title');
  while (style_title.childNodes.length) { style_title.removeChild(style_title.childNodes[0]); }
  if (curStyle.title != "") style_title.appendChild(document.createTextNode(curStyle.title));
  else style_title.appendChild(document.createTextNode(curStyle.name));
  // Creator
  if (!style_creator) style_creator = document.getElementById('style_creator');
  while (style_creator.childNodes.length) { style_creator.removeChild(style_creator.childNodes[0]); }
  if (curStyle.creator != "") style_creator.appendChild(document.createTextNode(curStyle.creator));
  else style_creator.appendChild(document.createTextNode('N/A'));
  // Status
  if (!style_status) style_status = document.getElementById('style_status');
  while (style_status.childNodes.length) style_status.removeChild(style_status.childNodes[0]);
  if (curStyle.status != "") style_status.appendChild(document.createTextNode(curStyle.status));
  else style_status.appendChild(document.createTextNode('N/A'));
  // Update
  if (!style_updated) style_updated = document.getElementById('style_updated');
  while (style_updated.childNodes.length) style_updated.removeChild(style_updated.childNodes[0]);
  if (curStyle.update != "") style_updated.appendChild(document.createTextNode(curStyle.update));
  else style_updated.appendChild(document.createTextNode('N/A'));
  // Description
  if (!style_description) style_description = document.getElementById('style_description');
  while (style_description.childNodes.length) style_description.removeChild(style_description.childNodes[0]);
  if (curStyle.description != "") style_description.appendChild(document.createTextNode(curStyle.description));
  else style_description.appendChild(document.createTextNode('N/A'));
}

function changeStyle() {
  // if (style_index == 0) cssurl.style.display = "";
  // else cssurl.style.display = "none";
  var curStyle = styleList[style_index];
  if (curStyle.path != "none") {
    if (curStyle.path == "") cssurl.value = base_url+"css/"+curStyle.name+"/"+curStyle.name+".css";
    else cssurl.value = curStyle.path;
  } else cssurl.value = "";
  updateCurrentStyle();
}

function stylePrev() {
  if (style_index > 0) style_index--;
  if (style_index == 0) style_prev.disabled = true;
  if (style_index < styleList.length-1) style_next.disabled = false;
  changeStyle();
}

function styleNext() {
  if (style_index < styleList.length-1) style_index++;
  if (style_index == styleList.length-1) style_next.disabled = true;
  if (style_index > 0) style_prev.disabled = false;
  changeStyle();
}

function parseData(xmldoc) {
  var root = xmldoc.getElementsByTagName('css_styles').item(0);
  var t1 = new Date();
  var styleEntries = root.getElementsByTagName('style');
  // the first entry is the custom node
  var scustom = new Object();
  scustom.name = "custom";
  scustom.title = "Custom/Default";
  scustom.path = "none";
  scustom.thumbnail = "none";
  scustom.screenshot = "none";
  scustom.creator = "N/A"
  scustom.update = "N/A";
  scustom.status = "N/A";
  scustom.description = "Provide a url to a custom style sheet or leave it blank if you wish to use the default style.";
  styleList.push(scustom);
  for (var i = 0; i < styleEntries.length; i++) {
    var styleEntry = new CStyle(styleEntries[i]);
    if (styleEntry.path != "none") {
      if (styleEntry.path == "") {
        if (styleEntry.name.toLowerCase() == current_css.toLowerCase()) style_index = i+1;
      } else {
        if (styleEntry.name.toLowerCase().indexOf(current_css.toLowerCase()) >= 0) style_index = i+1;
      }
    } // we just got the style the user is using (at least i hope we got it)
    styleList.push(styleEntry);
  }
  // FUCK IE //
  if (!style_prev) style_prev = document.getElementById('style_prev');
  if (!style_next) style_next = document.getElementById('style_next');
  if (style_index > 0) style_prev.disabled = false;
  if (style_index < styleList.length-1) style_next.disabled = false;
  style_next.onclick = styleNext;
  style_prev.onclick = stylePrev;
  updateCurrentStyle();
}

function prepPage() {
  uriObj = parseURI(); // update the uriObj
  fetchData(); // Fetch Data, we update the page in the meanwhile
  cssurl = document.getElementById('style_url');
  if (!cssurl) return;
  current_css = cssurl.value.substring(cssurl.value.lastIndexOf('/')+1,cssurl.value.indexOf('.css'));
  // cssurl.style.display = "none";
  // more stuff
  style_thumbnail = document.getElementById('style_thumbnail');
  style_title = document.getElementById('style_title');
  style_creator = document.getElementById('style_creator');
  style_status = document.getElementById('style_status');
  style_updated = document.getElementById('style_updated');
  style_description = document.getElementById('style_description');
  style_apply = document.getElementById('style_apply');
  style_next = document.getElementById('style_next');
  style_prev = document.getElementById('style_prev');
}
 
//window.onload = prePage;
addLoadEvent(prepPage);
