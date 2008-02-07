/* compat */
!Array.prototype.indexOf && (Array.prototype.indexOf = function (it)
	{
		for (var i = 0, l = this.length; i < l; i++)
		{
			if (this[i] === it) { return i; }
		}
		return -1;
	});


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
	'interestedusers':["rar","epoximator","deridiot","exp","pelican","fahrenheit"],
	'addvalidatorinterface':(function ()
		{
			if (document.evaluate && window.XMLSerializer) // interested people don't use IE anyway
			{
				var username = document.evaluate("//div[@id='layout-menu']/div[@class='usermenu' and not(form)]/p[@class='user']",
					document.body, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;
				if (username && Magic.interestedusers.indexOf(username.firstChild.nodeValue) >= 0)
				{
					var footernode = document.getElementById('layout-footer');
					if (footernode) { footernode.onclick = Magic.noreallydoitthistime; }
				}
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
			n = document.createElement("a");
			n.href = validatorloc+"?uri=referer&doctype="+doctypecheck.replace(" ","+");
			n.title = "Validate current URI as it is accessed by a guest";
			n.appendChild(document.createTextNode("@"));
			f.appendChild(n);
			var params = [['parsemodel',"xml"], ['doctype',doctypecheck], ['ss',1], ['fragment',""]];
			for (i = 0; k = params[i]; i++)
			{
				n = document.createElement("input");
				n.type = 'hidden'; n.name = k[0]; n.value = k[1]; f.appendChild(n);
			}
			Magic.checkfield = n;
			params = [["<",Magic.currentcheck,"Serialise current doc state and validate"],
				[">",Magic.basecheck,"Re-fetch current URI from server and validate"]];
			for (i = 0; k = params[i]; i++)
			{
				n = document.createElement("a");
				n.title = k[2]; f.appendChild(document.createTextNode(" "));
				n.appendChild(document.createTextNode(k[0])); n.onclick = k[1]; f.appendChild(n);
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
		})
	};

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
			linklist[i].target = "_blank";
		}
	}
}

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