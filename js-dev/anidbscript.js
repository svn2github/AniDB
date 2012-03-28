/* This var will hold information about the currently loaded javascript files
 * minimal format of entries is: {"file":"path to file","version":"file version"}
 * other attributes that can go on the entry can be any of the following
 * "revision":"svn revision number", Number of the svn revision that change this text
 * "author":"author name", author of the last change
 * "date":"dd.mm.yyyy", date of last release, please use only the following formats: dd.mm.yyyy hh:mm or dd.mm.yyyy hh:mm:ss
 * "changelog":"last change"
 * Add your information to jsVersionArray like so:
 * jsVersionArray.push({"file":"anidbscript.js","version":"1.0"});
 */
 
var jsVersionArray = new Array();
jsVersionArray.push({
	"file":"anidbscript.js",
	"version":"1.04",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"added checked classes and made the last selected tab a per page preference"
});

/*	Loads settings cookie and unserializes it */
function loadJSONCookie(cookie) {
	var r;
	try {
		r = eval('(' + CookieGet(cookie)  + ')');
	} catch (e) {
		return new Object();
	}
	return r;
}

var config = new Object();
var settings = config;
// This is the Preference set holders, I will also define defaults now
config['settings'] = new Object();

// Title
settings['title']         = new Object();
settings['title']['aATL'] = 'x-jat'; // animeAltTitleLang
settings['title']['eATL'] = 'x-jat'; // episodeAltTitleLang
settings['title']['eTD']  = 2; // episodeTitleDisplay

// eD2k
settings['ed2k']               = new Object();
settings['ed2k']['pattern']    = "%ant - %enr%ver - %ept - <[%grp]><(%crc)><(%cen)><(%lang)><(%raw)>"; // ed2k_pattern
settings['ed2k']['space']      = "_"; // space_pattern
settings['ed2k']['pad']        = true;
settings['ed2k']['padonormal'] = true;

// Mylist
settings['mylist']           = new Object();
settings['mylist']['use']    = 0; // use_mylist_add
settings['mylist']['state']  = 0; // mylist_add_state
settings['mylist']['viewed'] = 0; // mylist_add_viewed_state
settings['mylist']['fstate'] = 0; // mylist_add_fstate
settings['mylist']['gstate'] = 0; // mylist_add_state (for generics, defaults to tv)

// Group
settings['group']           = new Object();
settings['group']['type']   = 0; // group_check_type
settings['group']['filter'] = 0; // group_langfilter

// Layout
settings['aLayout']          = new Object();
settings['aLayout']['aCS']   = 'default'; // animePage_curSort
settings['aLayout']['aCSO']  = 'up'; // animePage_curSortOrder
settings['aLayout']['aPL']   = '1,2,3,4,6,7,8,9,10,11,12,13,14'; // animePageLayout
settings['aLayout']['fsize'] = 0; // format_size

// Global
settings['global']             = new Object();
settings['global']['useajax']  = 0;
settings['global']['collapse'] = 0; //collapseThumbnails
settings['global']['info']     = 0; // get_info
settings['global']['infosz']   = 150; // get_info_sz
settings['global']['infomw']   = 450; // get_info_mw

// Other
settings['other']                 = new Object();
settings['other']['emode']        = 1; // currentFMode
settings['other']['jsp']          = 1; // usejspopups
settings['other']['dsearch']      = 'none'; // def_search
settings['other']['menuCollapse'] = 0; // what to do with menus that can be collapsed (0 default, 1 allways hide, 2 try to guess based on page width)
settings['other']['seeDebug']     = 0; // see debug information
settings['other']['seeTimes']     = 0; // see timing information
settings['other']['ignoreLocal']  = 0; // ignore local check information

/* Load all settings */
var cookie = loadJSONCookie('anidbsettings');
for (k in cookie) {
	switch (k) {
		// Title
		case 'ALTANIMETITLE':
			settings['title']['aATL'] = cookie[k];
			break;
		case 'ALTEPTITLE':
			settings['title']['eATL'] = cookie[k];
			break;
		case 'ALTEPTITLEDISPLAY':
			settings['title']['eTD'] = cookie[k];
			break;
		// eD2k
		case 'ED2KPATTERN':
			settings['ed2k']['pattern'] = cookie[k];
			break;
		case 'ED2KPATTERNSPACE':
			settings['ed2k']['space'] = cookie[k];
			break;
		case 'NOEPNOPADDING':
			settings['ed2k']['padonormal'] = false;
			break;
		case 'PADALLEPNOS':
			settings['ed2k']['pad'] = false;
			break;
		// My List
		case 'USEQUICKADD':
			settings['mylist']['use'] = cookie[k];
			break;
		case 'MYLISTADDSTATE':
			settings['mylist']['state'] = cookie[k];
			break;
		case 'MYLISTADDWATCHEDSTATE':
			settings['mylist']['viewed'] = cookie[k];
			break;
		case 'MYLISTADDFILESTATE':
			settings['mylist']['fstate'] = cookie[k];
			break;
		case 'MYLISTADDGENFILESTATE':
			settings['mylist']['gstate'] = cookie[k];
			break;
		// Group
		case 'FILESELECTION': // Fix // group_check_type
			settings['group']['type'] = cookie[k];
			break;
		case 'FILTERRELEASESBYLANG':
			// Might be unneeded later on, for now leave it be
			settings['group']['filter'] = cookie[k];
			break;
		// Layout
		case 'LAYOUTANIMEFILESORT':
			settings['aLayout']['aCS'] = {
				'2'  : 'fid',
				'7'  : 'codec',
				'8'  : 'resolution',
				'4'  : 'size',
				'9'  : 'source',
				'12' : 'users'
			}[cookie[k]];
			break;
		case 'LAYOUTANIMEFILESORTORDER':
			settings['aLayout']['aCSO'] = ['up', 'down'][cookie[k]];
			break;
		case 'LAYOUTANIMEFILE':
			settings['aLayout']['aPL'] = cookie[k].join(',');
			break;
		case 'FORMATFILESIZE':
			settings['aLayout']['fsize'] = cookie[k];
			break;
		// Global
		case 'USEAJAX':
			settings['global']['useajax'] = Number(cookie[k]);
			break;
		case 'USETHUMBNAILHOVER':
			settings['global']['collapse'] = cookie[k];
			break;
		case 'DEFAULTINFOTOOLTIPSIZE':
			settings['global']['infosz'] = cookie[k];
			break;
		case 'DEFAULTINFOTOOLTIPWIDTH':
			settings['global']['infomw'] = [600, 450, 300][cookie[k]];
			break;
		// Global > Tooltips
		case 'SHOWINFOTOOLTIPONANIME':
			settings['global']['info'] |= 1;
			break;
		case 'SHOWINFOTOOLTIPONMYLIST':
			settings['global']['info'] |= 2;
			break;
		case 'SHOWINFOTOOLTIPONEP':
			// Use information tooltips on Episode pages
			settings['global']['info'] |= 4;
			break;
		case 'SHOWINFOTOOLTIPONGROUP':
			settings['global']['info'] |= 8;
			break;
		case 'SHOWINFOTOOLTIPONCHAR':
			settings['global']['info'] |= 16;
			break;
		case 'SHOWINFOTOOLTIPONCREATOR':
			// Use information tooltips on Creator pages
			settings['global']['info'] |= 32;
			break;
		case 'SHOWINFOTOOLTIPONWISHLIST':
			settings['global']['info'] |= 64;
			break;
		// Other
		case 'DEFAULTEDITORMODE':
			settings['other']['emode'] = [1, 0, 2][cookie[k]];
			break;
		case 'DISABLEJSPOPUPS':
			settings['other']['jsp'] = 0;
			break;
		case 'DEFAULTSEARCHTYPE':
			settings['other']['dsearch'] = cookie[k];
			break;
		case 'DEFAULTSIDEBARBEHAVIOUR':
			settings['other']['menuCollapse'] = cookie[k];
			break;
		case 'SHOWDEBUGINFO':
			settings['other']['seeDebug'] = cookie[k];
			break;
		case 'SHOWTIMINGINFO':
			settings['other']['seeTimes'] = cookie[k];
			break;
		case 'DISABLELOCALHOSTCHECK':
			settings['other']['ignoreLocal'] = cookie[k];
			break;
	}
}
// TITLE
animeAltTitleLang   = settings['title']['aATL'];
episodeAltTitleLang = settings['title']['eATL'];
episodeTitleDisplay = Number(settings['title']['eTD']);

// ED2K
// The hash Object holds hashing defaults
var hashObj = new Object();
hashObj.usePatterns       = settings['ed2k']['pattern'];
hashObj.convertSpaces     = true;
hashObj.defaultSpacesChar = settings['ed2k']['space'];
hashObj.spacesChar        = hashObj.defaultSpacesChar;
hashObj.defaultPattern    = settings['ed2k']['pattern'] ?
	settings['ed2k']['pattern'] :
	"%ant - %enr%ver - %ept - <[%grp]><(%crc)><(%cen)><(%lang)><(%raw)>";
hashObj.pattern           = hashObj.defaultPattern;
hashObj.ed2k              = "ed2k://|file|" + hashObj.pattern + ".%ext|%flen|%ed2k|";
hashObj.sfv               = hashObj.pattern + ".%ext %crc";
hashObj.validHashes       = [ "ed2k", "sfv" ];
ed2k_pattern              = settings['ed2k']['pattern'];
space_pattern             = settings['ed2k']['space'];
pad_epnums                = settings['ed2k']['pad'];
pad_only_normal_epnums    = settings['ed2k']['padonormal'];

// MYLIST
use_mylist_add          = Number(settings['mylist']['use']);
mylist_add_viewed_state = Number(settings['mylist']['viewed']);
mylist_add_state        = Number(settings['mylist']['state']);
mylist_add_fstate       = Number(settings['mylist']['fstate']);
mylist_add_gstate       = Number(settings['mylist']['gstate']);

// GLOBAL
collapseThumbnails = Number(settings['global']['collapse']);
get_info           = Number(settings['global']['info']); // bitwise value
get_info_sz        = Number(settings['global']['infosz']);
get_info_mw        = Number(settings['global']['infomw']);

// GROUP
group_check_type = Number(settings['group']['type']);
group_langfilter = Number(settings['group']['filter']);

// OTHER
def_search       = settings['other']['dsearch'];
currentFMode     = Number(settings['other']['emode']);
//if (def_search != 'none' && searchTypeSelect) searchTypeSelect.value = def_search;

// LAYOUT
animePage_curSort       = settings['aLayout']['aCS'];
animePage_curSortOrder  = settings['aLayout']['aCSO'];
animePageLayout         = settings['aLayout']['aPL'];
animePageLayout         = animePageLayout.split(',');
var animePage_defLayout = [
	'check-anime',
	'fid',
	'group',
	'size',
	'crc',
	'langs',
	'cf',
	'resolution',
	'anime-source',
	'quality',
	'anime-hashes',
	'users',
	'state-anime',
	'actions-anime'
];
animePage_curLayout     = new Array();
for (var ci = 0; ci < animePageLayout.length; ci++)
	animePage_curLayout.push(animePage_defLayout[animePageLayout[ci] - 1]);

// MISC
var usejspopups             = settings['other']['jsp'] || true;
var curPageID               = null;
var searchTypeSelect        = null;
var searchTypeDefaultSelect = settings['other']['dsearch'] || 'none';
var ignoreLocal             = Number(settings['other']['ignoreLocal']) || 0;
var menuCollapse            = Number(settings['other']['menuCollapse']) || 0;
var username                = null;
var defaultTabs             = tabCookieGet();
var picbase                 = 'http://img7.anidb.net/pics/anime/';
var seeDebug                = Number(settings['other']['seeDebug']) || 0;
var seeTimes                = Number(settings['other']['seeTimes']) || 0;

var charInfos = new Array(); 		// Character information	(indexed by charid)
var creatorInfos = new Array(); 	// Creator information		(indexed by creatorid)
var animeInfos = new Array();		// Anime information		(indexed by aid)

function loadSettings() { }

/* Function declarations below, end of config here */

/* returns true if the script is being executed on localhost */
function isLocalHost() {
	if (ignoreLocal) return false; // magic code for DerIdiot
	return (''+window.location.hostname == '' || ''+window.location.hostname.toLowerCase() == 'localhost');
}

/* compat */
if (typeof Array.prototype.indexOf == "undefined") {
	Array.prototype.indexOf = function(it) {
		for (var i = 0; i < this.length; i++) {
			if (this[i] == it) return i;
		}
		return -1;
	}
}
/*
Array.prototype.sum = function(){
	for(var i=0,sum=0;i<this.length;sum+=this[i++]);
	return sum;
}
*/

/*function popup(url,w,h,scr,re,title)
{
	var urlp = (url.indexOf("animedb.pl?") != -1 && url.indexOf("&nonav") == -1) ? url+"&nonav=1" : url;
    window.open(urlp,title,'toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars='+scr+',resizable='+re+',width='+w+',height='+h);
}*/

/* generic event functions */

/* returns computed style information about a specific element
 * @param element where to get attributes
 * @param attribute what to get
 */
function getStyleInformation(element,attribute,debug) {
	if (!element && attribute) return;
	var useCurrentStyle = (element.currentStyle) ? true : false;
	var useGetCumputedStyle = (window.getComputedStyle) ? true : false;

	if (debug) {
		var result = '';
		if (!attribute) {
			if (useGetCumputedStyle)
				result += '\n!attribute>useGetCumputedStyle: ' + window.getComputedStyle(element,null);
			if (useCurrentStyle)
				result += '\n!attribute>useCurrentStyle: ' + element.currentStyle;
		} else {
			if (useGetCumputedStyle)
				result += '\nattribute>useGetCumputedStyle: ' + window.getComputedStyle(element,null)[attribute];
			if (useCurrentStyle)
				result += '\nattribute>useCurrentStyle: ' + element.currentStyle[attribute];
		}
		alert('useCurrentStyle: '+useCurrentStyle+
			'\nuseGetCumputedStyle: '+useGetCumputedStyle+
			'\nelement: '+element+
			'\nattribute: '+attribute+
			'\nresult: '+result
		);
	}

	if (!attribute) {
		if (useGetCumputedStyle)
			return window.getComputedStyle(element,null);
		else if (useCurrentStyle)
			return element.currentStyle;
		else return '';
	} else {
		if (useGetCumputedStyle)
			return window.getComputedStyle(element,null)[attribute];
		else if (useCurrentStyle)
			return element.currentStyle[attribute];
		else return '';
	}
	return null;
}

/* Finds the position of a given element */
function findPos(element) {
	var curleft = curtop = 0;
	if (element.offsetParent) {
		do {
			curleft += element.offsetLeft;
			curtop += element.offsetTop;
		} while (obj = element.offsetParent);
	}
	return [curleft,curtop];
}

/* Adds an event to an element
 * @param element Element to which the event will be added
 * @param eventName Name of the event ["click", "load", "unload", "mouseover", etc]
 * @param callback Function that will be executed
 */
function addEventSimple(element, eventName, callback) {
	if(typeof(element) == "string")
		element = document.getElementById(element);
	if(element == null)
		return;
	if(element.addEventListener) {
		element.addEventListener(eventName, callback, false);
	} else if(element.attachEvent)
		element.attachEvent("on" + eventName, callback);
}

/* Removes an event from an element
 * @param element Element from which the event will be removed
 * @param eventName Name of the event ["click", "load", "unload", "mouseover", etc]
 * @param callback Function that will be executed
 */
function removeEventSimple(element, eventName, callback) {
	if(typeof(element) == "string")
		element = document.getElementById(element);
	if(element == null)
		return;
	if(element.removeEventListener)
		element.removeEventListener(eventName, callback, false);
	else if(element.detachEvent)
		element.detachEvent("on" + eventName, callback);
}

/* Gets the target of an event */
function getEventTarget(e) {
	e = e ? e : window.event;
	return e.target ? e.target : e.srcElement;
}

/* Cancels one event */
function cancelEvent(e) {
	e = e ? e : window.event;
	if(e.stopPropagation)
		e.stopPropagation();
	if(e.preventDefault)
		e.preventDefault();
	e.cancelBubble = true;
	e.cancel = true;
	e.returnValue = false;
	return false;
}

/* finds out what key was pressed */
function getPressedKey(e) {
	if (!e) e= event;
	if(window.event) // IE
		keynum = e.keyCode;
	else if(e.which) // Netscape/Firefox/Opera
		keynum = e.which;
	return keynum;
}

/* Alias */
function addLoadEvent(func) { addEventSimple(window, "load", func); }
function hookEvent(element, eventName, callback) { addEventSimple(element, eventName, callback); }
function unhookEvent(element, eventName, callback) { removeEventSimple(element, eventName, callback); }

/* Generic ANIDB functions */

/* Adds some text to the footer, useful for debug and stuff
 * @param text String with text to show
 * @param append Boolean which if set to true will append the current text to any other text existing
 */
function addInfoToFooter(text, append) {
	var footer = document.getElementById('layout-footer');
	if (!footer) return; // nonav page
	var p = document.getElementById('layout-footer-info');
	if (!p) {
		p = document.createElement('p');
		p.id = 'layout-footer-info';
		p.appendChild((typeof text == 'string' ? document.createTextNode(text) : text));
		footer.appendChild(p);
	} else {
		if (!append) while (p.childNodes.length) p.removeChild(p.firstChild);
		p.appendChild((typeof text == 'string' ? document.createTextNode(text) : text));
	}
}

/* Get's a JS file version from the Array */
function getJsFileVersionArrayInfo(id) {
	var array = new Array();
	if (!jsVersionArray[id]) return null;
	for (var elem in jsVersionArray[id])
		array.push(elem+"="+jsVersionArray[id][elem]);
	return array;
}

/* Shows an alert with the loaded scripts */
function alertJsFileVersionArray() {
	var text = "Loaded scripts:";
	for (var id = 0; id < jsVersionArray.length; id++) {
		for (var elem in jsVersionArray[id]) {
			if (elem == 'file') text += '\n'+jsVersionArray[id][elem];
			else text += '\n\t'+elem+"="+jsVersionArray[id][elem];
		}
	}
	alert(text);
}

/* Writes the Javascript Footer */
function writeJsFooter() {
	var last = jsVersionArray[jsVersionArray.length - 1];
	if (!last) return;
	addInfoToFooter('Javascript: ');
	var link = document.createElement('a');
	link.onclick = alertJsFileVersionArray;
	link.title = 'click to see a list of loaded scripts';
	link.appendChild(document.createTextNode(last['file']));
	addInfoToFooter(link,true);
	addInfoToFooter(' '+last['revision']+', '+last['date'],true);
}

addLoadEvent(writeJsFooter);

/* Creates a basic popup */
function BasicPopup(a) {
	p = a.className.substring(a.className.lastIndexOf(' ')).split('.');
	var url = (a.href.indexOf("animedb.pl?") != -1 && a.href.indexOf("&nonav") == -1) ? a.href+"&nonav=1" : a.href;
	if (window.open(url, p[4] || "_blank", 'toolbar=0,location=0,directories=0,status=0,menubar=0'.concat(
		',width=', p[0]*1 || "400",
		',height=', p[1]*1 || "300",
		',scrollbars=', p[2] || "1",
		',resizable=', p[3] || "1")))
		{ return false; }
}

function BasicPopupSelf() { return BasicPopup(this); }

function OpenNewWindow() { window.open(this.href); return false; }

function ClassToggle(elem, name, mode) {
	var classes = elem.className.split(" ");
	var index = classes.indexOf(name);
	if(index<0){
		if(mode==2) return;
		classes.push(name);
		elem.className = classes.join(" ");
		return 1;
	}else{
		if(mode==1) return;
		classes.splice(index,1);
		elem.className = classes.join(" ");
		return 1;
	}
}

function ckChangeEvnt(event,object) {
	var ck = object || this;
	if (ck._parentTR == null) {
		var MAX_PARENT_DEPTH = 3;
		var node = ck.parentNode;
		var depth = 0;
		while (node && node.nodeName && node.nodeName.toLowerCase() != 'tr' && depth < MAX_PARENT_DEPTH) {
			node = node.parentNode;
			depth++;
		}
		// last check, if this fails ignore this checkbox
		if (node && node.nodeName && node.nodeName.toLowerCase() == 'tr') {
			ck._parentTR = node;
			if (node.className.indexOf('checked') >= 0)
				node.className =  node.className.replace(/ checked|checked|checked /ig,'');
			if (ck.checked) node.className = 'checked '+node.className;
		} else
			this._parentTR = 'undefined'; // stop further searches
	} else if (ck._parentTR == 'undefined') {
		//alert('undefined, boring i know..');
		return;
	} else {
		var node = ck._parentTR;
		if (node && node.className && node.className.indexOf('checked') >= 0)
			node.className = node.className.replace(/ checked|checked|checked /ig,'');
		if (ck.checked) node.className = 'checked '+node.className;
	}
}

/* This function makes a checkbox change the parent row classname based on it's checked state
 * @param parent The node where checkboxes will be given this event */
function addCheckboxesEvent(parent) {
	var checkboxes = parent.getElementsByTagName('input');
	for (var i = 0; i < checkboxes.length; i++) {
		if (checkboxes[i].type != 'checkbox') continue;
		// apply to the onclick event of the checkboxes a new function
		addEventSimple(checkboxes[i],"click",ckChangeEvnt);
		//addEventSimple(checkboxes[i],"change",ckChangeEvnt);
	}
}

/* This function gives SHIFT-SELECT abilities to checkboxes
 * @param parent The node where checkboxes will be given this event */
function enhanceCheckboxes(parent) {
	parent._shiftKey = false;
	parent._lastCheck = null;

	addEventSimple(parent,"click",function(event) { parent._shiftKey = event.shiftKey; });
	addEventSimple(parent,"change",function(event) {
			var currentIndex, checkbox, start, end;

			if (event.target.nodeName.toLowerCase() == "label") {
				if (event.target.hasAttribute("for"))
					checkbox = document.getElementById(event.target.getAttribute("for")) || document.getElementsByName(event.target.getAttribute("for")).item(0);
				else
					checkbox = event.target.getElementsByTagName("input").item(0);
			} else
				checkbox = event.target;

			if (parent._shiftKey && parent._lastCheck != null && parent._lastCheck != checkbox) {
				for (var i = 0; i < checkbox.form.elements.length; i++) {
					if (checkbox.form.elements[i] == checkbox) {
						start = checkbox;
						end = parent._lastCheck;
						currentIndex = i;
						break;
					} else if (checkbox.form.elements[i] == parent._lastCheck) {
						start = parent._lastCheck;
						end = checkbox;
						currentIndex = i;
						break;
					}
				}

				// Check for multicolumn environment
				var check_column = function(node) {
					var classes = node.className.split(" ");
					for (i in classes) {
						var t = /^column(\d+)$/.exec(classes[i]);
						if (t != null) {
							return t[1];
						}
					}
					return null
				}
				if (!checkbox._inColumn)
					checkbox._inColumn = check_column(checkbox);

				for (currentIndex += 1; currentIndex < checkbox.form.elements.length && checkbox.form.elements[currentIndex] != end; currentIndex++) {
					if (checkbox.form.elements[currentIndex].type == "checkbox" && !checkbox.form.elements[currentIndex].disabled) {
						if (!checkbox._inColumn || checkbox._inColumn == check_column(checkbox.form.elements[currentIndex])) {
							checkbox.form.elements[currentIndex].checked = checkbox.checked;
							ckChangeEvnt(null,checkbox.form.elements[currentIndex]);
						}
					}
				}

				parent._lastCheck = end;
			} else
				parent._lastCheck = checkbox;

			parent._shiftKey = false;
		}
	);
}

function compressMenus() {
	if (!document.getElementsByTagName) return;
	var divs = document.getElementsByTagName('div');
	var i = 0;
	while (i < divs.length) {
		var div = divs[i];
		if (!div.className || div.className.indexOf('g_menu filter_menu') < 0) { i++; continue; }
		var header = document.createElement('h3');
		header.width = 'auto';
		var toggleIcon = document.createElement('a');
		toggleIcon._pwidth = getStyleInformation(div,'width');
		toggleIcon.className = 'i_icon ' + (menuCollapse == 0 || (menuCollapse == 2 && window.innerWidth > 1152) ? 'i_expanded' : 'i_collapsed');
		toggleIcon.title = 'Toggle display of menu';
		toggleIcon.alt = 'toggle display';
		toggleIcon.onclick = function toogle() {
			var expand = (this.className.indexOf('i_expanded') >= 0);
			this.className = this.className.replace(/i_expanded|i_collapsed/mgi,'');
			this.className += (expand ? ' i_collapsed' : ' i_expanded');
			var rootNode = this.parentNode.parentNode;
			var cdiv = rootNode.getElementsByTagName('div')[0];
			if (cdiv.className == 'content')
				cdiv.style.display = (expand ? 'none' : '');
			// (re)set root div
			rootNode.style.width = (expand ? 'auto' : this._pwidth);
			rootNode.className = rootNode.className.replace(/expanded|collapsed/mgi,'');
			rootNode.className += (expand ? ' collapsed' : ' expanded');
		}
		header.appendChild(toggleIcon);
		var content = document.createElement('div');
		content.className = 'content';
		if (menuCollapse > 0) { // let's do some work
			var doCollapse = false;
			if (menuCollapse == 1) doCollapse = true;
			else { // check if collapse is needed
				if (window.innerWidth < 1152) {
					//alert("window width: "+window.innerWidth+"\ncollapse? "+(window.innerWidth < 1152));
					doCollapse = true;
				}
			}
			if (doCollapse) {
				content.style.display = 'none';
				div.style.width = 'auto'
				div.className = div.className.replace(/expanded|collapsed/mgi,'');
				div.className += ' collapsed';
			}
		}
		while(div.childNodes.length) content.appendChild(div.childNodes[0]);
		div.appendChild(header);
		div.appendChild(content);
		i++;
	}
}

/* specific */
var Magic = {
	'add_validator_interface':(function () {
			if (document.evaluate && window.XMLSerializer) // interested people don't use IE anyway
			{
				var footernode = document.getElementById('layout-footer');
				if (footernode) { footernode.onclick = Magic.noreallydoitthistime; }
			}
		}),
	'noreallydoitthistime':(function () {
			// this is going to be horrible for a bit
			//options:
			// * tell validator to get current uri (will always be guest access, and sometimes denied)
			// * send current doc serialised (always well-formed, maybe not valid, good for dhtml checking)
			// * xmlhttpreq the current uri and send the text recieved (bad for post and borked gets, good for well-formedness check)
			// XHTML+Basic+1.0 when, well: <epoximator> 1.0 when we have a tablewriter
			var footernode = document.getElementById('layout-footer');
			footernode.onclick = null;
			var validatorloc = "http://validator.w3.org/check", doctypecheck = "XHTML 1.1", f, n, i, k;
			f = document.createElement("form");
			f.method = "post"; f.enctype = "multipart/form-data"; f.action = validatorloc;
			footernode.appendChild(f, footernode);
			Magic.valiform = f;
			f = f.appendChild(document.createElement("div"));
			f.appendChild(document.createTextNode("W3 Validator: "));

			var params = [['parsemodel',"xml"], /*['doctype',doctypecheck],*/ ['ss',1], ['fragment',""]];
			for (i = 0; k = params[i]; i++)
			{
				n = document.createElement("input");
				n.type = 'hidden'; n.name = k[0]; n.value = k[1]; f.appendChild(n);
			}
			Magic.checkfield = n;

			params = [
				["current",Magic.check_current,"Serialise current doc state and validate"],
				["re-fetch",Magic.check_base,"Re-fetch current URI from server and validate"],
				["as guest", undefined, "Validate current URI as it is accessed by a guest"]];
			for (i = 0; k = params[i]; i++)
			{
				n = document.createElement("a");
				n.title = k[2];
				n.appendChild(document.createTextNode(k[0]));
				if( k[1] ){ n.href=""; n.onclick = k[1]; }
				else n.href = validatorloc+"?uri=referer&doctype="+doctypecheck.replace(" ","+");
				if( i>0 ) f.appendChild(document.createTextNode(", "));
				f.appendChild(n);
			}
		}),
	'asxmlfixup':(function (s) {
			return "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/tr/xhtml11/DTD/xhtml11.dtd\" [ <!ATTLIST td anidb:sort CDATA #IMPLIED> <!ATTLIST select value CDATA #IMPLIED> ]>"
				+s
				.replace(/<[^! >]+/g, function(m){return m.toLowerCase();})
				.replace(/<\?xml\ version="1.0"\?>/,'')
				.replace(/<!DOCTYPE\ html>/,'')
				.replace(/<html/,'<html xmlns="http://www.w3.org/1999/xhtml"')
				.replace(/(<[^>]+\s+)lang(=[\'\"])/g,"$1xml:lang$2")
				.replace(/selected="true"/g,'selected="selected"') //for opera
				.replace(/checked="true"/g,'checked="checked"') //for opera
				.replace(/\/\*&#x5d\]>\*\//,'/*]]&gt;*/') //for opera
				//.replace(/<select\s+name="(.+)"\s+size="(\d+)"\svalue="1">/g,'<select name="$1" size="$2">') //for opera
				.replace(/\ class=""/g,'') //for js disabled classes
			;
		}),
	'check_current':(function () {
			var serialator = new XMLSerializer();
			Magic.checkfield.value = Magic.asxmlfixup(serialator.serializeToString(document));
			Magic.valiform.submit();
			return false;
		}),
	'check_base':(function () {
			if (!window.location.search) { alert("Page seems to have been POST-ed, can't re-fetch"); }
			else
			{
				var req = new XMLHttpRequest();
				req.open('GET', window.location, false);
				req.send(null);
				if(req.status != 200) { alert("Refetching the document failed with code: "+req.status); }
				else
				{
					Magic.checkfield.value = Magic.asxmlfixup(req.responseText);
					Magic.valiform.submit();
				}
			}
			return false;
		}),
	'enable_row_kid_classes':(function () {
			var rowlist = document.getElementsByTagName('tr');
			for (var i = 0; i < rowlist.length; i++)
			{
				var row = rowlist[i];
				for (var j = 0; j < row.childNodes.length; j++)
				{
					var kidclass = row.childNodes[j].className || "";
					if (kidclass.indexOf(" ") >= 0)
					{
						row.className += kidclass.substring(kidclass.indexOf(" "));
					}
				}
			}
		}),
	'enable_a_onclick_by_rel':(function () {
			var linklist = document.getElementsByTagName('a');
			for (var i = 0; i < linklist.length; i++)
			{
				var relstring = linklist[i].getAttribute("rel") || "";
				if (usejspopups && relstring.indexOf("anidb::popup") >= 0)
				{
					linklist[i].onclick = BasicPopupSelf;
				}
				else if (relstring.indexOf("anidb::") >= 0 && relstring.indexOf('extern') < 0) // ::extern ::wiki ::forum ::tracker
				{
					linklist[i].onclick = OpenNewWindow;
				}
				else if (relstring.indexOf("anidb::") >= 0 && relstring.indexOf('extern') >= 0)
				{
					linklist[i].target = '_blank';
				}
			}
		}),
	'enable_hover_menu':(function (m) {
			var m = document.getElementById('layout-menu');
			if (m) {
				var i, ul, s;
				m.normalize();
				l = m.getElementsByTagName('ul');
				for (i=0;i<l.length;i++) {
					ul = l[i];

					ul.onmouseout = (function(){
						this.className=null;
					});
					ul.onmouseover = (function(){
						this.className='popup';
					});

					s = ul.parentNode.getElementsByTagName('span')[0];
					/*s.appendChild(document.createTextNode(ul.title || ul.id));
					s.className = 'tab';
					ul.parentNode.insertBefore(s, ul);*/

					s.onmouseout = (function(){
						this.nextSibling.className=null;
					});
					s.onmouseover = (function(){
						this.nextSibling.className='popup';
					});
				}
			}
		}),
	'enable_hide':(function () {
			var elems = document.getElementsByTagName('h4');
			for (var i = 0; i < elems.length; i++){
				var spans = elems[i].getElementsByTagName('span');
				if(spans.length>0)
					spans[0].onclick = Magic.toggle_hide;
			}
		}),
	'toggle_hide':(function (e) {
			var block = this.parentNode;
			ClassToggle(block, 'collapsed');
			while ( block = block.nextSibling ){
				if( block.nodeType==1 )
					ClassToggle(block, 'hide');
			}
		}),
	'enable_tabs':(function () {
			var elems = document.getElementsByTagName('ul');
			// find out page name
			var selected = (defaultTabs[(curPageID ? curPageID : 'tab')] ? defaultTabs[(curPageID ? curPageID : 'tab')] : 'tab1');//CookieGet((curPageID ? curPageID + "_" : "")+'tab') || 'tab1';
			for (var i = 0; i < elems.length; i++){
				if(elems[i].className == "tabs"){
					var li = elems[i].getElementsByTagName('li');
					for (var j = 0; j < li.length; j++){
						var tab = li[j];
						if(tab.className.indexOf('_default') >= 0 || tab.id == selected) {
							Magic.toggle_tabs(0, tab);
							selected = 0;
						}
						tab.onclick = Magic.toggle_tabs;
					}
					if( li.length>0 && selected ) {
						Magic.toggle_tabs('tab1', li[0]);
					}
				}
			}
		}),
	'toggle_tabs':(function (e, o) {
			var tab = o || this;
			if(tab){
				var classes = tab.className.split(" ");
				if (classes.indexOf("fake")<0) {
					var pane = document.getElementById(tab.id+'_pane');
					if(pane){
						var elems = tab.parentNode.getElementsByTagName('li');
						for (var i = 0; i < elems.length; i++){
							ClassToggle(elems[i], 'selected', 2);
						}
						ClassToggle(tab, 'selected', 1);
						tabCookieSet((curPageID ? curPageID : 'tab'), tab.id);
						elems = tab.parentNode.parentNode.getElementsByTagName('div');
						var topTab = null;
						for (var i = 0; i < elems.length; i++){
							// this isn't a pane so no need to hide it
							if (elems[i].className.indexOf('tabbed_pane') >= 0) {
								// also show first item
								topTab = elems[i];
								continue;
							}
							if (topTab && elems[i].parentNode.parentNode == topTab) {
								continue;
							}
							if (elems[i].className.indexOf('pane') < 0) continue;
							ClassToggle(elems[i], 'hide', 1);
						}
						ClassToggle(pane, 'hide', 2);
					}
				}
			}
		}),
	'upgrade_search':(function() {
			var divs = document.getElementsByTagName('div');
			for (var i = 0; i < divs.length; i++) {
				var div = divs[i];
				if (div.className.indexOf('search') < 0) continue;
				searchTypeSelect = div.getElementsByTagName('select')[0];
				var inputs = div.getElementsByTagName('input');
				var input = null;
				for (var k = 0; k < inputs.length; k++) {
					if (inputs[k].className && inputs[k].className.indexOf('text') >= 0) {
						input = inputs[k];
						break;
					}
				}
				if (!searchTypeSelect || !input) break;
				if (searchTypeDefaultSelect != 'none' && searchTypeSelect) searchTypeSelect.value = searchTypeDefaultSelect;
				addEventSimple(searchTypeSelect,'change',function() { input.focus(); });
				break;
			}
		}),
	'applySpoilerInputs':(function() {
			var inputs = document.getElementsByTagName('input');
			for (var i = 0; i < inputs.length; i++) {
				var isSpoiler = inputs[i].value.toLowerCase().indexOf('spoiler') >= 0;
				if (isSpoiler) inputs[i].onclick = Magic.toggleSpoiler;
			}
		}),
	'toggleSpoiler':(function() {
			var div = this.parentNode;
			while (div.nodeName.toLowerCase() != 'div') div = div.parentNode;
			var spans = div.getElementsByTagName('span');
			for (var i = 0; i < spans.length; i++) {
				var span = spans[i];
				if (span.className.indexOf('spoiler') < 0) continue;
				var isHidden = (span.className.indexOf('hide') >= 0);
				if (isHidden) {
					this.value = 'Hide Spoiler';
					span.className = span.className.replace(' hide','');
				} else {
					this.value = 'Show Spoiler';
					span.className += ' hide';
				}
			}
		}),
	'addNotifyCloseSubmit':(function() {
			var form = document.getElementsByTagName('form')[0];
			if (!form) return;
			var outputLink = form.action+'?';
			var inputs = form.getElementsByTagName('input');
			var inputValues = new Array();
			var newSubmit = document.createElement('input');
			for (var i = 0; i < inputs.length; i++) {
				var input = inputs[i];
				if (input.type == 'submit') { // create a new "submit" button
					inputValues.push(input.name+"=1");
					newSubmit.type = 'button';
					newSubmit.value = input.value;
					newSubmit.name = input.name;
					input.parentNode.replaceChild(newSubmit,input);
				} else {
					inputValues.push(input.name+"="+input.value);
				}
			}
			newSubmit.onclick = function postData() {
				var req = xhttpRequest();
				var data = inputValues.join('&');
				if (isLocalHost()) xhttpRequestPost(req, 'msg_del.html', null, data);
				else xhttpRequestPost(req, 'animedb.pl', null, data);
				window.close();
			}
		}),
	'enhanceCheckboxes':(function() {
			var formElems = document.getElementsByTagName('form');
			for (var i = 0; i < formElems.length; i++) {
				enhanceCheckboxes(formElems[i]);
				addCheckboxesEvent(formElems[i]);
			}

		})
};

/* init */
function InitDefault() {
	if (!document.getElementsByTagName){
		return;
	}

	if (document.getElementById('layout-nav')) {
		var nav = document.getElementById('layout-nav');
		curPageID = nav.className.substring(0,nav.className.indexOf('_nav'));
	} else if (document.getElementById('layout-main')) {
		var nav = document.getElementById('layout-main');
		var firstDiv = nav.getElementsByTagName('div')[0];
		if (firstDiv) {
			curPageID = firstDiv.className.substring(firstDiv.className.indexOf('g_content')+10,firstDiv.className.indexOf('_all'));
		}
	}

	//Magic.enable_hover_menu();
	Magic.enable_a_onclick_by_rel();	//for popup and "open in new window"
	//Magic.enable_row_kid_classes();		//for styling entire rows by td classes
	Magic.enable_hide();				//for h4 collapsing
	Magic.enable_tabs();				//for global structure ul.tabs
	Magic.add_validator_interface();
	Magic.upgrade_search();				//makes the search box take focus after search type change
	Magic.applySpoilerInputs();			//apply spoiler tag js support
	Magic.enhanceCheckboxes();			//add gmail like checkbox select and the like
	compressMenus();

	enable_sort(navigator.appName=='Opera'||navigator.userAgent.indexOf('Firefox/3.0')>0
		?do_sort_opera_and_ff3:do_sort_generic);
	var un = document.getElementById('user-name');
	if (un) {
		var sp = un.getElementsByTagName('span')[0];
		if (sp)
			username = sp.firstChild.nodeValue;
	}
	//if (curPageID == 'notify') Magic.addNotifyCloseSubmit();
}

function enable_sort(func){
	var tables = document.getElementsByTagName('table');
	for (var i = 0; i < tables.length; i++){
		var rows = tables[i].getElementsByTagName('tr');
		if(rows[0] && rows[1] && rows[0].className=='header'){
			var ths = rows[0].getElementsByTagName('th');
			var tds = rows[1].getElementsByTagName('td');
			if(ths.length==tds.length)
				for(var j=0; j<ths.length; j++)
					if(tds[j].getAttribute("anidb:sort")!=undefined && ths[j].getElementsByTagName('a').length<1){
						ClassToggle(ths[j], 'sortable', 1);
						ths[j].onclick = func;
					}
		}
	}
}
function do_sort_generic(){
	var t = new Date().getTime(), s;
	var tx = new Array();

	var type = this.getAttribute("anidb:sort");
	var desc = ClassToggle(this, 'ascending', 2);

	var ths = this.parentNode.getElementsByTagName('th');
	for (var i = 0; i < ths.length; i++){
		ClassToggle(ths[i], 'ascending', 2);
		ClassToggle(ths[i], 'descending', 2);
		if(ths[i]==this) index = i;
	}
	if(index<0){
		alert("Internal error: this is not a th...");
		return;
	}

	var section = this.parentNode.parentNode;
	var table = section.parentNode;

	table.normalize();

	s = new Date().getTime(); tx.push(s-t); t=s;

	var tmp = new Array();
	var num = true; var reg = /^\d+$/;
	var rows = section.getElementsByTagName('tr');
	while(rows.length>1){
		var tds = rows[1].getElementsByTagName('td');
		if(tds.length!=ths.length) //irregular row
			break;
		var val = tds[index].getAttribute("anidb:sort");
		tmp.push([ section.removeChild(rows[1]), val, rows.length ]);
		if(num && !reg.test(val)) num = false;
	}

	s = new Date().getTime(); tx.push(s-t); t=s;

	if( desc ){
		tmp.sort(num?sort_number_r:sort_text_r);
		ClassToggle(this, 'descending', 1);
	}else{
		tmp.sort(num?sort_number:sort_text);
		ClassToggle(this, 'ascending', 1);
	}

	s = new Date().getTime(); tx.push(s-t); t=s;


	if(rows.length>1){
		var last = rows[1];
		for (var i = 0; i < tmp.length; i++){
			ClassToggle(tmp[i][0], 'g_odd', i%2==0?1:2);
			section.insertBefore(tmp[i][0], last);
		}
	}else{
		for (var i = 0; i < tmp.length; i++){
			ClassToggle(tmp[i][0], 'g_odd', i%2==0?1:2);
			section.appendChild(tmp[i][0]);
		}
	}

	s = new Date().getTime(); tx.push(s-t); t=s;

	/*var t = document.getElementById('layout-footer');
	var p = t.appendChild(document.createElement('p'));
	p.appendChild(document.createTextNode(tx.join(', ')+ " => "+tx.sum()+" / "+rows.length));*/
}

function do_sort_opera_and_ff3(){
	var t = new Date().getTime(), s;
	var tx = new Array();

	var type = this.getAttribute("anidb:sort");
	var desc = ClassToggle(this, 'ascending', 2);

	var ths = this.parentNode.getElementsByTagName('th');
	for (var i = 0; i < ths.length; i++){
		ClassToggle(ths[i], 'ascending', 2);
		ClassToggle(ths[i], 'descending', 2);
		if(ths[i]==this) index = i;
	}
	if(index<0){
		alert("Internal error: this is not a th...");
		return;
	}

	var section = this.parentNode.parentNode;
	var table = section.parentNode;

	table.removeChild(section);
	//ClassToggle(table, 'hide', 1);

	s = new Date().getTime(); tx.push(s-t); t=s;

	table.normalize();

	s = new Date().getTime(); tx.push(s-t); t=s;

	var tmp = new Array();
	var num = true; var reg = /^\d+$/;
	var rows = section.getElementsByTagName('tr');
	var last;
	for(var i=1;i<rows.length; i++){
		var tds = rows[i].getElementsByTagName('td');
		if(tds.length!=ths.length){ //irregular row
			last = i;
			break;
		}
		var val = tds[index].getAttribute("anidb:sort");
		tmp.push([ rows[i], val, rows.length-i ]);
		if(num && !reg.test(val)) num = false;
	}

	s = new Date().getTime(); tx.push(s-t); t=s;

	if( desc ){
		tmp.sort(num?sort_number_r:sort_text_r);
		ClassToggle(this, 'descending', 1);
	}else{
		tmp.sort(num?sort_number:sort_text);
		ClassToggle(this, 'ascending', 1);
	}

	s = new Date().getTime(); tx.push(s-t); t=s;


	if(last){
		var last = rows[last];
		for (var i = 0; i < tmp.length; i++){
			ClassToggle(tmp[i][0], 'g_odd', i%2==0?1:2);
			section.insertBefore(tmp[i][0], last);
		}
	}else{
		for (var i = 0; i < tmp.length; i++){
			ClassToggle(tmp[i][0], 'g_odd', i%2==0?1:2);
			section.appendChild(tmp[i][0]);
		}
	}

	s = new Date().getTime(); tx.push(s-t); t=s;
	table.appendChild(section);

	s = new Date().getTime(); tx.push(s-t); t=s;

	/*var t = document.getElementById('layout-footer');
	var p = t.appendChild(document.createElement('p'));
	p.appendChild(document.createTextNode(tx.join(', ')+ " => "+tx.sum()+" / "+rows.length));*/
}
function sort_number(a,b){
	return a[1]-b[1] || b[2]-a[2];
}
function sort_number_r(a,b){
	return b[1]-a[1] || b[2]-a[2];
}
function sort_text(a,b){
	if(a[1]<b[1]) return -1;
	if(a[1]>b[1]) return 1;
	return b[2]-a[2];
}
function sort_text_r(a,b){
	if(a[1]>b[1]) return -1;
	if(a[1]<b[1]) return 1;
	return b[2]-a[2];
}

window.onload = InitDefault;

/* Cookie Functions */

/* This is a special function to get tab cookies */
function tabCookieGet() {
	return CookieGetToArray('default_tabs')
}

/* This is a special function to store tab cookies */
function tabCookieSet(name, value, expires, path, domain, secure) {
	defaultTabs[name] = value;
	CookieSetFromArray('default_tabs', defaultTabs, expires, path, domain, secure);
}

/* Stores cookies from values in an array
 * @param name Name of the cookie
 * @param array Array which holds the key/value array which will be stored in the cookie
 * @param expires Number of days the cookie will last
 */
function CookieSetFromArray(name, array, expires, path, domain, secure) {
	var aux = new Array();
	for (var k in array) {
		var val = array[k];
		if (typeof(val) != 'string') continue;
		aux.push(k+':'+val);
	}
	CookieSet(name,aux.join('|'),expires, path, domain, secure);
}

/* Sets a cookie */
function CookieSet( name, value, expires, path, domain, secure ) {
	var today = new Date();
	today.setTime( today.getTime() );

	if ( expires ){
		expires = expires * 1000 * 60 * 60 * 24;
	}
	var expires_date = new Date( today.getTime() + (expires) );

	//alert('CookieSet: '+name+'='+value);

	var text = name + "=" +escape( value ) +
	( ( expires ) ? ";expires=" + expires_date.toGMTString() : "" ) +
	( ( path ) ? ";path=" + path : "" ) +
	( ( domain ) ? ";domain=" + domain : "" ) +
	( ( secure ) ? ";secure" : "" );

	//alert('cookie['+text.length+']:\n'+text);

	document.cookie = text;
}

/* Gets a Cookie */
function CookieGet( check_name ) {
	var a_all_cookies = document.cookie.split( ';' );
	var a_temp_cookie, cookie_name, cookie_value;

	for ( i = 0; i < a_all_cookies.length; i++ )
	{
		a_temp_cookie = a_all_cookies[i].split( '=' );
		cookie_name = a_temp_cookie[0].replace(/^\s+|\s+$/g, '');

		if ( cookie_name == check_name ){
			if ( a_temp_cookie.length > 1 ){
				cookie_value = unescape( a_temp_cookie[1].replace(/^\s+|\s+$/g, '') );
			}
			return cookie_value;
		}
	}
	return null;
}

/* Gets a cookie value stored on a serialized cookie
 * @param holder The cookie name
 * @param key The Key of the value we want to get
 * @return Value associated with given key or null if not found
 */
function CookieGetByKey(holder, key) {
	var cookie = CookieGet(holder);
	if (!cookie) return null;
	var aux = new Array();
	var elems = cookie.split('|');
	for (var i = 0; i < elems.length; i++) {
		var kv = elems[i].split(':');
		if (kv[0] == key) return kv[1];
	}
	return null;
}

/* Fetchs a serialized cookie and restores it's array
 * @param name Name of the cookie
 * @param array Array where to store the contents of the cookie
 * @return Array with de-serialized cookie
 */
function CookieGetToArray(name, array) {
	if (!name) return null;
	if (!array) array = new Array();
	var cookie = CookieGet(name);
	if (!cookie) return array;
	//alert(name+'='+cookie);
	var elems = cookie.split('|');
	for (var i = 0; i < elems.length; i++) {
		var kv = elems[i].split(':');
		array[kv[0]] = kv[1];
	}
	return array;
}

// Tag Search Auto Completion. (C) 2008 antennen
// Relies on ajax.js by fahrenheit
var lastSearch = "";
var seeDebug = false;
var searchData = [];
var lastHitCount = 0;
var searchLimit = 50;

function search() {
	var target = document.getElementById("tagsearch");
	var type = this.parentNode.getElementsByTagName("select")[0].value;

	if(this.value.length >= 3 && (type != "modtag" && type != "mylist")) {
		// Check if a new search is necessary
		var ll = lastSearch.length
		var cl = this.value.length
		var min = Math.min(ll, cl);

//		unfortunately the new ranking algoririthm invalidates caching as we used to do it
//		if(!(lastSearch.substr(0, min).toLowerCase() == this.value.substr(0, min).toLowerCase() && ll && cl)) {
		// refetch data unless we have value of search hits lower than the limit in that case we can use our cache
/*
		alert("lastSearch = "+lastSearch+" ("+ll+")\n"+
			"search ="+this.value+" ("+cl+")\n"+
			"method 1 = "+!(lastSearch.substr(0, min).toLowerCase() == this.value.substr(0, min).toLowerCase() && ll && cl)+"\n"+
			"lastHitCount = "+lastHitCount+" vs searchLimit = "+searchLimit+" = "+(lastHitCount == searchLimit));*/
		if (!(lastSearch.substr(0, min).toLowerCase() == this.value.substr(0, min).toLowerCase() && ll && cl) || lastHitCount == searchLimit) { 
			var url = "animedb.pl?show=json&action=search&query="+encodeURI(this.value)+"&offset=0&limit="+searchLimit+"&type=";
			lastSearch = this.value;
			switch(type) {
				case "animelist":
					url += "anime";
					break;
				case "animetag":
					url += "animetag";
					break;
				case "characterlist":
					url += "character";
					break;
				case "chartags":
					url += "charactertag";
					break;
				case "clublist":
					url += "club";
					break;
				case "collectionlist":
					url += "collection";
					break;
				case "creatorlist":
					url += "creator";
					break;
				case "grouplist":
					url += "group";
					break;
				case "songlist":
					url += "song";
					break;
				case "userlist":
					url += "user";
					break;
				default: // unknown or unsupported option
					target.style.display = "none";
					return;
/*
				case "chartags":
					var url = 'animedb.pl?show=xmln&t=search&type=chartags&search=';
					var element = 'chartags';
					break;
				case "animetag":
				case "modtag":
					var url = 'animedb.pl?show=xml&t=tagsearch&search=';
					var element = 'tag';
					break;
				case "grouplist":
					var url = 'animedb.pl?show=xml&t=groupsearch&search=';
					var element = 'group';
					break;
				case "producerlist":
					var url = 'animedb.pl?show=xml&t=producersearch&search=';
					var element = 'producer';
					break;
*/
			}
			// hack for local test
			//url = "search-anime.json";
			xhttpRequestFetch(xhttpRequest(), url, function(jsonData) {
				if (jsonData['error'] || jsonData['warning']) {
					// either a warning or an error are a bit fatal so we stop right here
					target.style.display = "none";
					return;
				}
				searchData = jsonData['results'];
				lastHitCount = searchData.length;
				printTags();
			}, null, 'json');
/*
			function(xml) {
				var root = xml.getElementsByTagName('root').item(0);
				if (!root) { if (seeDebug) alert('Error: Could not get root node'); return; }
				searchData = root.getElementsByTagName(element);
				printTags();
			}
*/
		} else {
//			alert("Using cached data");
			printTags(); // Print matched
		}
	} else target.style.display = "none";
}

function printTags() {
	// Clear old result
	var target = document.getElementById("tagsearch");
	var search = target.parentNode.parentNode.getElementsByTagName("input")[0]
	if(target.hasChildNodes()) {
		while(target.childNodes.length > 0) {
			target.removeChild(target.firstChild);
		}
	}
	// Loop search result and filter
	var i = 0;
	var height = 0;
	for(var n = 0; n < searchData.length; n++) {
		// var tag = searchData[n].getAttribute("name");
		var tag = searchData[n]['name'];
		if(tag.toLowerCase().search(search.value.toLowerCase()) != -1) {
			var result = document.createElement("li");
			// do a bit of highlighting //
			var b = document.createElement('b');
			var si = tag.toLowerCase().indexOf(search.value.toLowerCase());
			if (si >= 0) {
				var firstBlock = document.createTextNode(tag.substring(0,si));
				var middleBlock = document.createTextNode(tag.substr(si,search.value.length));
				var lastBlock = document.createTextNode(tag.substring(si+search.value.length,tag.length));
				result.appendChild(firstBlock);
				b.appendChild(middleBlock);
				result.appendChild(b);
				result.appendChild(lastBlock);
			} else continue;
			result.id = 'tag_'+n;
			result.onclick = function() {
				var id = Number(this.id.substr(4,this.id.length));
				//var tag = searchData[id].getAttribute("name");
				var tag = searchData[id]['name'];
				search.value = tag;
				target.style.display = "none";
			}
			result.ondblclick = function() { this.onclick(); }
			result.onmousedown = function() { this.onclick(); }
			target.appendChild(result);
			i++;
		}
	}

	target.style.display = "block";
	target.style.position = "absolute";
	target.style.left = search.offsetLeft + "px";
	target.style.top = search.offsetTop + search.offsetHeight + "px";
	target.style.width = search.offsetWidth - 2 + "px";

	if(i >= 8) {
		height = target.firstChild.offsetHeight * 8;
		if(height > 0) target.style.height = height + "px";
	} else target.style.height = "auto";

	// Don't display if tag is matched or no tags are matched
	if(i == 0 || (i == 1 && target.firstChild.firstChild.data.toLowerCase() == search.value.toLowerCase())) {
		target.style.display = "none";
	}
}

if (settings['global']['useajax']) {
	// Initialize the script
	addLoadEvent(function() {
		// Find target form
		var target = document.getElementById("layout-search");

		if(target) {
			// Find search box and bind stuff to it
			var textfield = target.getElementsByTagName("input")[0];
			textfield.id = 'adb.search';
			textfield.onkeyup = search;
			textfield.onfocus = search;
			textfield.onchange = function() {
				setTimeout('document.getElementById("tagsearch").style.display = "none"', 100);
			}

			// Find search type dropdown
			var dropdown = target.getElementsByTagName("select")[0];
			if(dropdown) {
				function getSearchTypeChange(value) {
					if(value == undefined) value = this.value
					switch(value) {
						case "animetag":
						case "chartags":
						case "modtag":
						case "grouplist":
						case "producerlist":
							textfield.setAttribute("autocomplete", "off");
							break;
						default:
							textfield.setAttribute("autocomplete", "on");
					}
				}
				addEventSimple(dropdown,'change',getSearchTypeChange)
				//dropdown.onchange = getSearchTypeChange;
				getSearchTypeChange(dropdown.value);
			}

			// Spawn result dropdown
			result = document.createElement("ul");
			result.setAttribute("id", "tagsearch");
			result.className = "acdropdown";
			result.style.display = "none";
			textfield.parentNode.appendChild(result);
		}
	});
}

/* Legacy code */

//anidb.js code (C) 2003 by PetriW
function myGetElement(name) {
	if (document.getElementById) {
		// Standards
		return document.getElementById(name);
	} else if (document.layers) {
		// NS 4
		return document.layers[name];
	} else if (document.all) {
		// IE 4
		return document.all[name];
	} else {
		return false;
	}
}

function cbSelect(files) {
	for (var i = 0; i < files.length; i++) {
		myGetElement('mylmod.f.' + files[i]).checked = true;
	}
}

function cbDeselect(files) {
	for (var i = 0; i < files.length; i++) {
		myGetElement('mylmod.f.' + files[i]).checked = false;
	}
}

function cbToggle(files) {
	for (var i = 0; i < files.length; i++) {
		obj = myGetElement('mylmod.f.' + files[i]);
		if (obj)
			obj.checked = !obj.checked;
	}
}

/* SPINNERs */

function SpinControlAcceleration(increment, milliseconds) {
	increment = parseFloat(increment);
	if(isNaN(increment) || increment < 0) increment = 0;

	milliseconds = parseInt(milliseconds);
	if(isNaN(milliseconds) || milliseconds < 0) milliseconds = 0;

	this.GetIncrement = function() { return increment; }
	this.GetMilliseconds = function() { return milliseconds; }
}

function SpinControlAccelerationCollection() {
	var _array = new Array();

	this.GetCount = function() { return _array.length; }

	this.GetIndex = function(index) {
		if(index < 0 || index >= _array.length) return null;
		return _array[index];
	}

	this.RemoveIndex = function(index) {
		if(index < 0 || index >= _array.length) return;

		newArray = new Array();
		for(var i=0; i<_array.length; i++) {
			if(i == index) continue;
			newArray.push(_array[i]);
		}
		_array = newArray;
	}

	this.Clear = function() { _array = new Array(); }

	this.Add = function(spa) {
		if(spa.constructor != SpinControlAcceleration) return;
		if(_array.length == 0) { _array.push(spa); return; }

		var newSec = spa.GetMilliseconds();
		if(newSec > _array[_array.length-1].GetMilliseconds()) { _array.push(spa); return; }

		var added = false;
		var newArray = new Array();
		var indexSec;
		for(var i=0; i<_array.length; i++) {
			if(added) newArray.push(_array[i]);
			else {
				indexSec = _array[i].GetMilliseconds();
				if(indexSec < newSec) newArray.push(_array[i]);
				else if(indexSec == newSec) { newArray.push(spa); added = true; }
				else { newArray.push(_array[i]); newArray.push(spa); added = true; }
			}
		}
		_array = newArray;
		return;
	}
}

function SpinControl() {
	var _this = this;

	var _accelerationCollection = new SpinControlAccelerationCollection();
	var _callbackArray = new Array();
	var _currentValue = 1;
	var _maximumVal = 100;
	var _minimumVal = 0;
	var _increment = 0.01;
	var _width = 50;
	var _updateInterval = 600;
	var _isDisabled = false;

	var _running = 0;
	var _interval = -1;
	var _timeStart = 0;

	var _bodyEventHooked = false;

	var _hiddenInput = null;

	var _container = document.createElement("div");
	_container.className = 'spinContainer';

	var _textBox = document.createElement("input");
	_textBox.type = 'text';
	_textBox.className = 'spinInput';
	_textBox.value = _currentValue;

	var _upButton = document.createElement("div");
	_upButton.className = 'spinUpBtn';

	var _downButton = document.createElement("div");
	_downButton.className = 'spinDownBtn';

	_container.appendChild(_textBox);
	_container.appendChild(_upButton);
	_container.appendChild(_downButton);

	function Run() {
		if(_isDisabled) return;
		if(_running == 0) return;

		var elapsed = new Date().getTime() - _timeStart;
		var inc = _increment;

		if(_accelerationCollection.GetCount() != 0) {
			inc = 0;
			for(var i = 0; i<_accelerationCollection.GetCount(); i++) {
				if(elapsed < _accelerationCollection.GetIndex(i).GetMilliseconds()) break;
				inc = _accelerationCollection.GetIndex(i).GetIncrement();
			}
		} else if(elapsed < _updateInterval) {
			return;
		}

		DoChange(inc);
	}

	function CancelRunning() {
		if(_isDisabled) return;
		_running = 0;
		if(_interval != -1) { clearInterval(_interval); _interval = -1; }
	}

	function DoChange(inc) {
		var newVal = _currentValue + inc * _running;
		UpdateCurrentValue(newVal);
	}

	function StartRunning(newState) {
		if(_isDisabled) return;
		if(_running != 0) CancelRunning();

		_running = newState;

		DoChange(_increment);

		_timeStart = new Date().getTime();
		_interval = setInterval(Run, 150);
	}

	function UpdateCurrentValue(newVal) {
		if(newVal <_minimumVal) newVal = _minimumVal;
		if(newVal > _maximumVal) newVal = _maximumVal;

		newVal = Math.round(1000*newVal)/1000;

		_textBox.value = newVal;
		if(newVal == _currentValue) return;

		_currentValue = newVal;

		for(var i=0; i<_callbackArray.length; i++)
			_callbackArray[i](_this, _currentValue);
	}

	function UpPress(e) {
		_upButton.className = 'spinUpBtnPress';
		_downButton.className = 'spinDownBtn';
		StartRunning(1);
		_textBox.focus();
		return cancelEvent(e);
	}

	function DownPress(e) {
		_upButton.className = 'spinUpBtn';
		_downButton.className = 'spinDownBtnPress';
		StartRunning(-1);
		_textBox.focus();
		return cancelEvent(e);
	}

	function UpHover(e) {
		if(!_bodyEventHooked)
			hookEvent(document.body, 'mouseover', ClearBtns);

		_upButton.className = 'spinUpBtnHover';
		_downButton.className = 'spinDownBtn';
		CancelRunning();
		return cancelEvent(e);
	}

	function DownHover(e) {
		if(!_bodyEventHooked)
			hookEvent(document.body, 'mouseover', ClearBtns);

		_upButton.className = 'spinUpBtn';
		_downButton.className = 'spinDownBtnHover';
		CancelRunning();
		return cancelEvent(e);
	}

	function ClearBtns(e) {
		var target = getEventTarget(e);
		if(target == _upButton || target == _downButton)
			return;
		_upButton.className = 'spinUpBtn';
		_downButton.className = 'spinDownBtn';
		CancelRunning();

		if(_bodyEventHooked) {
			unhookEvent(document.body, 'mouseover', ClearBtns);
			_bodyEventHooked = false;
		}
		return cancelEvent(e);
	}

	function BoxChange() {
		var val = parseFloat(_textBox.value);
		if(isNaN(val)) val = _currentValue;

		UpdateCurrentValue(val);
	}

	function MouseWheel(e) {
		e = e ? e : window.event;
		var movement = e.detail ? e.detail / -3 : e.wheelDelta/120;
		UpdateCurrentValue(_currentValue + _increment * movement);
		return cancelEvent(e);
	}

	function TextFocused(e) {
		hookEvent(window, 'DOMMouseScroll', MouseWheel);
		hookEvent(document, 'mousewheel', MouseWheel);
		return cancelEvent(e);
	}

	function TextBlur(e) {
		unhookEvent(window, 'DOMMouseScroll', MouseWheel);
		unhookEvent(document, 'mousewheel', MouseWheel);
		return cancelEvent(e);
	}

	function keysPressed(e) {
		var keynum = getPressedKey(e);
		switch (keynum) {
			case 40: DownPress(e); break;
			case 38: UpPress(e); break;
			default: break;
		}
	}

	function keysReleased(e) {
		var keynum = getPressedKey(e);
		switch (keynum) {
			case 40: DownHover(e); break;
			case 38: UpHover(e); break;
			default: break;
		}
	}

	this.StartListening = function() {
		hookEvent(_upButton, 'mousedown', UpPress);
		hookEvent(_upButton, 'mouseup', UpHover);
		hookEvent(_upButton, 'mouseover', UpHover);

		hookEvent(_downButton, 'mousedown', DownPress);
		hookEvent(_downButton, 'mouseup', DownHover);
		hookEvent(_downButton, 'mouseover', DownHover);

		hookEvent(_textBox, 'change', BoxChange);
		hookEvent(_textBox, 'focus', TextFocused);
		hookEvent(_textBox, 'blur', TextBlur);
		hookEvent(_textBox, 'keydown', keysPressed);
		hookEvent(_textBox, 'keyup', keysReleased);
	}

	this.StopListening = function() {
		unhookEvent(_upButton, 'mousedown', UpPress);
		unhookEvent(_upButton, 'mouseup', UpHover);
		unhookEvent(_upButton, 'mouseover', UpHover);

		unhookEvent(_downButton, 'mousedown', DownPress);
		unhookEvent(_downButton, 'mouseup', DownHover);
		unhookEvent(_downButton, 'mouseover', DownHover);

		unhookEvent(_textBox, 'change', BoxChange);
		unhookEvent(_textBox, 'focus', TextFocused);
		unhookEvent(_textBox, 'blur', TextBlur);
		unhookEvent(_textBox, 'keydown', keysPressed);
		unhookEvent(_textBox, 'keyup', keysReleased);

		if(_bodyEventHooked) {
			unhookEvent(document.body, 'mouseover', ClearBtns);
			_bodyEventHooked = false;
		}
	}

	this.SetMaxValue = function(value) {
		value = parseFloat(value);
		if(isNaN(value)) value = 1;
		_maximumVal = value;
		UpdateCurrentValue(_currentValue);
	}

	this.SetMinValue = function(value) {
		value = parseFloat(value);
		if(isNaN(value)) value = 0;
		_minimumVal = value;
		UpdateCurrentValue(_currentValue);
	}

	this.SetCurrentValue = function(value) {
		value = parseFloat(value);
		if(isNaN(value)) value = 0;
		UpdateCurrentValue(value);
	}

	this.SetWidth = function(value) {
		value = parseInt(value);
		if(isNaN(value) || value < 25) value = 25;
		_width = value;
    	_container.style.width = _width + 'px';
		_textBox.style.width = (_width - 20) + 'px';
	}

	this.SetUpdateInterval = function(value) {
		value = Number(value);
		if(isNaN(value)) value = 600;
		if (value < 300) value = 300; // this kind of update is ridiculous
		if (value > 10000) value = 10000; // more than 10 seconds between updates !?
		_updateInterval = value;
	}

	this.SetIncrement = function(value) {
		value = parseFloat(value);
		if(isNaN(value)) value = 0;
		if(value < 0) value = -value;

		_increment = value;
	}

	this.SetInputName = function(name) { _textBox.name = name; }
	this.SetInputId = function(id) { _textBox.id = id; }
	this.SetInputValue = function(value) { _textBox.value = value; }
	this.SetInputFields = function(name, id, value) {
		if (name != null) _textBox.name = name;
		if (id != null) _textBox.id = id;
		if (value != null) _textBox.value = value;
	}

	this.SetDisabled = function(value) {
		_isDisabled = value;
		_textBox.disabled = value;
	}

	this.SetHiddenInput = function(element) {
		_hiddenInput = element;
		_textBox._hiddenInput = _hiddenInput;
	}

	this.AttachValueChangedListener = function(listener) {
		for(var i=0; i<_callbackArray.length; i++)
			if(_callbackArray[i] == listener)
				return;

		_callbackArray.push(listener);
	}

	this.DetachValueChangedListener = function(listener) {
		newArray = new Array();
		for(var i=0; i<_callbackArray.length; i++)
			if(_callbackArray[i] != listener)
				newArray.push(_callbackArray[i]);

		_callbackArray = newArray;
	}

	this.GetContainer = function() { return _container; }
	this.GetCurrentValue = function() { return _currentValue; }
	this.GetMaxValue = function() { return _maximumVal; }
	this.GetMinValue = function() { return _minimumVal; }
	this.GetWidth = function() { return _width; }
	this.GetIncrement = function() { return _increment; }
	this.GetUpdateInterval = function() { return _updateInterval; }
	this.GetAccelerationCollection = function() { return _accelerationCollection; }
	this.GetInput = function() { return _textBox; }
	this.GetDisabled = function() { return _isDisabled; }
	this.GetHiddenInput = function() { return _hiddenInput; }

	_this.SetWidth(_width);
}

// Add a class to the banner div to change it dynamically depending on browser window size
addEventSimple(window, "load", function() {
	var banner = document.getElementById("layout-search");
	if (!document.body.offsetWidth) {
		return;
	} else if (document.body.offsetWidth > 1900) {
		banner.className += " widest";
	} else if (document.body.offsetWidth > 1550) {
		banner.className += " wider";
	} else if (document.body.offsetWidth > 1200) {
		banner.className += " wide";
	}
});
// Add expand support to resource links Q&D
addEventSimple(window, "load", function() {
	var spans = document.getElementsByTagName("span");
	if (!spans.length) {
		return;
	}
	for (var i = 0; i < spans.length; i++) {
		if (spans[i].className.match(/(?:^|\s+)morelinks(?:\s+|$)/)) {
			(function(span) {
				var links = span.getElementsByTagName("a");
				if (!links.length) {
					return;
				}
				var more = links[links.length - 1];
				addEventSimple(more, "click", function() {
					var spans = span.getElementsByTagName("span");
					if (!spans) {
						return;
					}
					spans[0].className = "";
					more.parentNode.removeChild(more);
					return false;
				});
			})(spans[i]);
		}
	}
});
