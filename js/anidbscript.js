/* compat */
!Array.prototype.indexOf && (Array.prototype.indexOf = function (it){
		for (var i = 0, l = this.length; i < l; i++){
			if (this[i] === it) { return i; }
		}
		return -1;
	});

if (typeof Array.prototype.indexOf == "undefined") {
	Array.prototype.indexOf = function(what) {
		for (var i = 0; i < this.length; i++) {
			if (this[i] == what) return i;
		}
		return -1;
	}
}

function popup(url,w,h,scr,re,title)
{
	var urlp = (url.indexOf("animedb.pl?") != -1 && url.indexOf("&nonav") == -1) ? url+"&nonav=1" : url;
    window.open(urlp,title,'toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars='+scr+',resizable='+re+',width='+w+',height='+h);
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

var Magic = {
	'OpenNewWindow':(function ()
		{
			window.open(this.href);
			return false;
		}),
	'addvalidatorinterface':(function ()
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
			f.appendChild(document.createTextNode("Validate: "));

			var params = [['parsemodel',"xml"], ['doctype',doctypecheck], ['ss',1], ['fragment',""]];
			for (i = 0; k = params[i]; i++)
			{
				n = document.createElement("input");
				n.type = 'hidden'; n.name = k[0]; n.value = k[1]; f.appendChild(n);
			}
			Magic.checkfield = n;

			params = [
				["current",Magic.currentcheck,"Serialise current doc state and validate"],
				["re-fetch",Magic.basecheck,"Re-fetch current URI from server and validate"],
				["as guest", undefined, "Validate current URI as it is accessed by a guest"]];
			for (i = 0; k = params[i]; i++)
			{
				n = document.createElement("a");
				n.title = k[2]; 
				n.appendChild(document.createTextNode(k[0]));
				if( k[1] ) n.onclick = k[1];
				else n.href = validatorloc+"?uri=referer&doctype="+doctypecheck.replace(" ","+");
				if( i>0 ) f.appendChild(document.createTextNode(", "));
				f.appendChild(n);
			}
		}),
	'asxmlfixup':(function (s)
		{
			return s.replace(/<[^! >]+/g, function(m){return m.toLowerCase();})
				.replace('<?xml version="1.0"?>','')
				.replace(/selected="true"/g,'selected="selected"')
				.replace('<html','<html xmlns="http://www.w3.org/1999/xhtml"')
				.replace(/(<[^>]+\s+)lang(=[\'\"])/g,"$1xml:lang$2");
		}),
	'currentcheck':(function ()
		{
			var serialator = new XMLSerializer();
			Magic.checkfield.value = Magic.asxmlfixup(serialator.serializeToString(document));
			Magic.valiform.submit();
		}),
	'basecheck':(function ()
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
		}),
	'enable_hide':(function ()
		{
			var elems = document.getElementsByTagName('H4');
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
		})
	};

function ClassToggle(elem, classN, mode)
{
	var classes = elem.className.split(" ");
	var index = classes.indexOf(classN); // IE doesn't natively support this
	if(index<0){
		if(mode==2) return;
		classes.push(classN);
		elem.className = classes.join(" ");
	}else{
		if(mode==1) return;
		classes.splice(index,1);
		elem.className = classes.join(" ");
	}
	
}

function InitDefault()
{
	if (document.getElementsByTagName)
	{
		var linklist = document.getElementsByTagName('a');
		var rowlist = document.getElementsByTagName('tr');
		var spanlist = document.getElementsByTagName('span');
	}
	else return;
	Magic.addvalidatorinterface();
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
	
	for (var i = 0; i < linklist.length; i++)
	{
		var relstring = linklist[i].getAttribute("rel") || "";
		if (relstring.indexOf("anidb::popup") >= 0)
		{
			linklist[i].target = "_blank";
			linklist[i].onclick = BasicPopupSelf;
		}
		else if (relstring.indexOf("anidb::") >= 0) // ::extern ::wiki ::forum ::tracker
		{
			linklist[i].onclick = Magic.OpenNewWindow;
		}
	}

	Magic.enable_hide();
	Magic.enable_tabs();
	
	mbSet('layout-menu', 'menu-over');
}


/**/



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

window.onload = InitDefault;


function CookieSet( name, value, expires, path, domain, secure ) 
{
	// set time, it's in milliseconds
	var today = new Date();
	today.setTime( today.getTime() );

	/*
	if the expires variable is set, make the correct 
	expires time, the current script below will set 
	it for x number of days, to make it for hours, 
	delete * 24, for minutes, delete * 60 * 24
	*/
	if ( expires )
	{
		expires = expires * 1000 * 60 * 60 * 24;
	}
	var expires_date = new Date( today.getTime() + (expires) );

	document.cookie = name + "=" +escape( value ) +
	( ( expires ) ? ";expires=" + expires_date.toGMTString() : "" ) + 
	( ( path ) ? ";path=" + path : "" ) + 
	( ( domain ) ? ";domain=" + domain : "" ) +
	( ( secure ) ? ";secure" : "" );
}

// this fixes an issue with the old method, ambiguous values 
// with this test document.cookie.indexOf( name + "=" );
function CookieGet( check_name ) {
	// first we'll split this cookie up into name/value pairs
	// note: document.cookie only returns name=value, not the other components
	var a_all_cookies = document.cookie.split( ';' );
	var a_temp_cookie = '';
	var cookie_name = '';
	var cookie_value = '';
	var b_cookie_found = false; // set boolean t/f default f
	
	for ( i = 0; i < a_all_cookies.length; i++ )
	{
		// now we'll split apart each name=value pair
		a_temp_cookie = a_all_cookies[i].split( '=' );
		
		
		// and trim left/right whitespace while we're at it
		cookie_name = a_temp_cookie[0].replace(/^\s+|\s+$/g, '');
	
		// if the extracted name matches passed check_name
		if ( cookie_name == check_name )
		{
			b_cookie_found = true;
			// we need to handle case where cookie has no value but exists (no = sign, that is):
			if ( a_temp_cookie.length > 1 )
			{
				cookie_value = unescape( a_temp_cookie[1].replace(/^\s+|\s+$/g, '') );
			}
			// note that in cases where cookie is initialized but no value, null is returned
			return cookie_value;
			break;
		}
		a_temp_cookie = null;
		cookie_name = '';
	}
	if ( !b_cookie_found )
	{
		return null;
	}
}

function mbSet(m,c) {
var m = document.getElementById(m);
if (m) {
	m.className=c;
	
	var i, ul, s;
	l = m.getElementsByTagName('ul');
	for (i=0;i<l.length;i++) {
		ul = l[i];
		
		ul.onmouseout = (function(){
			this.className=null;
		});
		ul.onmouseover = (function(){
			this.className='popup';
		});
		
		s = document.createElement('span');
		s.appendChild(document.createTextNode(ul.title || ul.id));
		s.className = 'tab';
		ul.parentNode.insertBefore(s, ul);

		s.onmouseout = (function(){
			this.nextSibling.className=null;
		});
		s.onmouseover = (function(){
			this.nextSibling.className='popup';
		});
	}
}}
