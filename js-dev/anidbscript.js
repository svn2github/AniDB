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

/* generic */

/**
 * Adds onload events to window.onload
 * usage: addLoadEvent(nameOfSomeFunctionToRunOnPageLoad);
 *    or: addLoadEvent(function() {
 *           more code to run on page load 
 *         });
 */
function addLoadEvent(func) {
	var oldonload = window.onload;
	if (typeof window.onload != 'function') {
		window.onload = func;
	} else {
		window.onload = function() {
			if (oldonload) {
				oldonload();
			}
			func();
		}
	}
}

function BasicPopup(a)
{
	p = a.className.substring(a.className.lastIndexOf(' ')).split('.');
	var url = (a.href.indexOf("animedb.pl?") != -1 && a.href.indexOf("&nonav") == -1) ? a.href+"&nonav=1" : a.href;
	if (window.open(url, p[4] || "_blank", 'toolbar=0,location=0,directories=0,status=0,menubar=0'.concat(
		',width=', p[0]*1 || "400",
		',height=', p[1]*1 || "300",
		',scrollbars=', p[2] || "1",
		',resizable=', p[3] || "1")))
		{ return false; }
}

function BasicPopupSelf()
{
	return BasicPopup(this);
}

function OpenNewWindow()
{
	window.open(this.href);
	return false;
}

function ClassToggle(elem, name, mode)
{
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

/* specific */
var Magic = {
	'add_validator_interface':(function ()
		{
			if (document.evaluate && window.XMLSerializer) // interested people don't use IE anyway
			{
				var footernode = document.getElementById('layout-footer');
				if (footernode) { footernode.onclick = Magic.noreallydoitthistime; }
			}
		}),
	'noreallydoitthistime':(function ()
		{
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
	'asxmlfixup':(function (s)
		{
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
	'check_current':(function ()
		{
			var serialator = new XMLSerializer();
			Magic.checkfield.value = Magic.asxmlfixup(serialator.serializeToString(document));
			Magic.valiform.submit();
			return false;
		}),
	'check_base':(function ()
		{
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
	'enable_row_kid_classes':(function ()
		{
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
	'enable_a_onclick_by_rel':(function ()
		{
			var linklist = document.getElementsByTagName('a');
			for (var i = 0; i < linklist.length; i++)
			{
				var relstring = linklist[i].getAttribute("rel") || "";
				if (relstring.indexOf("anidb::popup") >= 0)
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
	'enable_hover_menu':(function (m)
		{
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
	'enable_hide':(function ()
		{
			var elems = document.getElementsByTagName('h4');
			for (var i = 0; i < elems.length; i++){
				var spans = elems[i].getElementsByTagName('span');
				if(spans.length>0)
					spans[0].onclick = Magic.toggle_hide;
			}
		}),
	'toggle_hide':(function (e)
		{
			var block = this.parentNode;
			ClassToggle(block, 'collapsed'); 
			while ( block = block.nextSibling ){
				if( block.nodeType==1 )
					ClassToggle(block, 'hide');
			}
		}),
	'enable_tabs':(function ()
		{
			var elems = document.getElementsByTagName('ul');
			var selected = CookieGet('tab') || 'tab1';
			for (var i = 0; i < elems.length; i++){				
				if(elems[i].className == "tabs"){
					var li = elems[i].getElementsByTagName('li');
					for (var j = 0; j < li.length; j++){
						var tab = li[j];
						if(tab.id == selected)
						{
							Magic.toggle_tabs(0, tab);
							selected = 0;
						}
						tab.onclick = Magic.toggle_tabs;
					}
					if( li.length>0 && selected ){
						Magic.toggle_tabs('tab1', li[0]);
					}
				}
			}
		}),
	'toggle_tabs':(function (e, o)
		{
			var tab = o || this;
			if(tab){
				var pane = document.getElementById(tab.id+'_pane');
				if(pane){
					var elems = tab.parentNode.getElementsByTagName('li');
					for (var i = 0; i < elems.length; i++){
						ClassToggle(elems[i], 'selected', 2);						
					}
					ClassToggle(tab, 'selected', 1);
					CookieSet('tab', tab.id);
					elems = tab.parentNode.parentNode.getElementsByTagName('div');
					for (var i = 0; i < elems.length; i++){
						// this isn't a pane so no need to hide it
						if (elems[i].className.indexOf('pane') < 0) continue;
						ClassToggle(elems[i], 'hide', 1);
					}
					ClassToggle(pane, 'hide', 2);
				}
			}
		}),
	'upgrade_search':(function()
		{
			var divs = document.getElementsByTagName('div');
			for (var i = 0; i < divs.length; i++) {
				var div = divs[i];
				if (div.className.indexOf('search') < 0) continue;
				var select = div.getElementsByTagName('select')[0];
				var inputs = div.getElementsByTagName('input');
				var input = null;
				for (var k = 0; k < inputs.length; k++) {
					if (inputs[k].className && inputs[k].className.indexOf('text') >= 0) {
						input = inputs[k];
						break;
					}
				}
				if (!select || !input) break;
				select.onchange = function() { input.focus(); }
				break;
			}
		}),
	'applySpoilerInputs':(function()
		{
			var inputs = document.getElementsByTagName('input');
			for (var i = 0; i < inputs.length; i++) {
				var isSpoiler = inputs[i].value.toLowerCase().indexOf('spoiler') >= 0;
				if (isSpoiler) inputs[i].onclick = Magic.toggleSpoiler;
			}
		}),
	'toggleSpoiler':(function()
		{
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
		})
	};

/* init */

function InitDefault()
{
	if (!document.getElementsByTagName){
		return;
	}

	//Magic.enable_hover_menu();
	Magic.enable_a_onclick_by_rel();	//for popup and "open in new window"
	//Magic.enable_row_kid_classes();		//for styling entire rows by td classes
	Magic.enable_hide();				//for h4 collapsing
	Magic.enable_tabs();				//for global structure ul.tabs
	Magic.add_validator_interface();
	Magic.upgrade_search();				//makes the search box take focus after search type change
	Magic.applySpoilerInputs();			//apply spoiler tag js support
	
	enable_sort(navigator.appName=='Opera'||navigator.userAgent.indexOf('Firefox/3.0')>0
		?do_sort_opera_and_ff3:do_sort_generic);
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


function CookieSet( name, value, expires, path, domain, secure ) 
{
	var today = new Date();
	today.setTime( today.getTime() );

	if ( expires ){
		expires = expires * 1000 * 60 * 60 * 24;
	}
	var expires_date = new Date( today.getTime() + (expires) );

	document.cookie = name + "=" +escape( value ) +
	( ( expires ) ? ";expires=" + expires_date.toGMTString() : "" ) + 
	( ( path ) ? ";path=" + path : "" ) + 
	( ( domain ) ? ";domain=" + domain : "" ) +
	( ( secure ) ? ";secure" : "" );
}

function CookieGet( check_name )
{
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

// Tag Search Auto Completion. (C) 2008 antennen 
// Relies on ajax.js by fahrenheit
var lastSearch = "";
var searchData = [];

function getPosition(obj) {
	var left = 0;
	var top = 0;
	
	if(obj.offsetParent) {
		do {
			left += obj.offsetLeft;
			top += obj.offsetTop;
		} while (obj = obj.offsetParent);
		return [left, top];
	}
	
	return [0, 0];
}

function search() {
	var target = document.getElementById("tagsearch");
	
	if(this.value.length >= 3 && this.parentNode.getElementsByTagName("select")[0].value == "animetag") {
		// Check if a new search is necessary
		var ll = lastSearch.length
		var cl = this.value.length
		var min = Math.min(ll, cl);
		
		if(!(lastSearch.substr(0, min).toLowerCase() == this.value.substr(0, min).toLowerCase() && ll && cl)) {
			lastSearch = this.value;
			xhttpRequestFetch(xhttpRequest(), 'animedb.pl?show=xml&t=tagsearch&search='+encodeURI(this.value), function(xml) {
				var root = xml.getElementsByTagName('root').item(0);
				if (!root) { if (seeDebug) alert('Error: Could not get root node'); return; }
				searchData = root.getElementsByTagName('tag');
				
				printTags();
			});
		}
		
		// Print matched
		printTags();
	} else {
		
		target.style.display = "none";
	}
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
		var tag = searchData[n].getAttribute("name");
		if(tag.toLowerCase().search(search.value.toLowerCase()) != -1) {
			var result = document.createElement("li");
				result.appendChild(document.createTextNode(tag));
				result.onclick = function() {
					search.value = this.firstChild.data;
					target.style.display = "none";
				}
			
			target.appendChild(result);
			
			if(i++ < 8) {
				height += result.offsetHeight;
			}
		}
	}
	
	// Set auto complete display properties
	var pos = getPosition(search);
	
	target.style.display = "block";
	target.style.position = "absolute";
	target.style.left = search.offsetLeft + "px";
	target.style.top = search.offsetTop + search.offsetHeight + "px";
	target.style.width = search.offsetWidth - 2 + "px";
	
	if(i >= 8 && height > 0) {
		target.style.height = height + "px";
	} else {
		target.style.height = "auto";
	}
	
	// Don't display if tag is matched or no tags are matched
	if(i == 0) {
		target.style.display = "none";
	}
}

// Initialize the script
addLoadEvent(function() {
	// Find target form
	var target = document.getElementById("layout-search");

	if(target) {
		// Find search box and bind stuff to it
		var textfield = target.getElementsByTagName("input")[0];
		textfield.onkeyup = search;
		textfield.onfocus = search;
		textfield.onchange = function() {
			document.getElementById("tagsearch").style.display = "none";
		}
		
		// Find search type dropdown
		var dropdown = target.getElementsByTagName("select")[0];
		if(dropdown) {
			dropdown.onchange = function() {
				if(this.value == "animetag") {
					textfield.setAttribute("autocomplete", "off");
				} else {
					textfield.setAttribute("autocomplete", "on");
				}
			}
			
			if(dropdown.value == "animetag") {
				textfield.setAttribute("autocomplete", "off");
			} else {
				textfield.setAttribute("autocomplete", "on");
			}
		}
		
		// Spawn result dropdown
		result = document.createElement("ul");
		result.setAttribute("id", "tagsearch");
		result.className = "acdropdown";
		result.style.display = "none";
		textfield.parentNode.appendChild(result);
	}
});

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
