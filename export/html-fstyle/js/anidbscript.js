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

function InitDefault()
{
	if (document.getElementsByTagName)
	{
		var linklist = document.getElementsByTagName('a');
		var rowlist = document.getElementsByTagName('tr');
		var spanlist = document.getElementsByTagName('span');
	}
	else return;
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
