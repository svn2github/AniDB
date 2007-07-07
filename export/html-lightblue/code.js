var today = new Date();
var expiry = new Date(today.getTime() + 365 * 24 * 60 * 60 * 1000);
var ie4=document.all;
var ns6=document.getElementById&&!document.all;
var ckstyle = unescape(getCookie('style') || 'Light Blue');
var ckanimepaging = getCookie('animepaging') || 25;
var cktitlestyle = getCookie('titlestyle') || 0;
var ckepstyle = getCookie('episodestyle') || 0;
var cktitle = getCookie('title') || 1;
var cksep1 = getCookie('sep1') || 0;
var ckepisodestr = getCookie('episodestr') || 0;
var ckepno = getCookie('epno') || 1;
var cksep2 = getCookie('sep2') || 1;
var ckepname = getCookie('epname') || 1;
var cksep3 = getCookie('sep3') || 0;
var ckcodec = getCookie('codec') || 0;
var ckcrc = getCookie('crc') || 1;
var ckspacesstyle = getCookie('spacesstyle') || 0;
var ckgroupstyle = getCookie('groupstyle') || 0;
var ckepnostyle = getCookie('epnostyle') || 1;
var ckclipboard = getCookie('clipboard') || 1;
var ckcollapse = getCookie('collapse') || 1;
var sorts = new Array();
sorts = ['name','eps','seen','size','rating','trating','review'];

function showHide(id)
{
	var itm = null;

	if (document.getElementById)
		itm = document.getElementById(id);
	else if (document.all)
		itm = document.all[id];
	else if (document.layers)
		itm = document.layers[id];

	if (!itm)
	{
		// do nothing
	}
	else if (itm.style)
	{
		if (itm.style.display == 'none')
			itm.style.display = 'block';
		else
			itm.style.display = 'none';
	}
	else
	{
		itm.visibility = 'show';
	}
}

function setPreferences()
{
	if (cktitle == '1')
		document.getElementById('title').checked = true;
	if (cksep1 == '1')
		document.getElementById('sep1').checked = true;
	if (ckepisodestr == '1')
		document.getElementById('episodestr').checked = true;
	if (ckepno == '1')
		document.getElementById('epno').checked = true;
	if (cksep2 == '1')
		document.getElementById('sep2').checked = true;
	if (ckepname == '1')
		document.getElementById('epname').checked = true;
	if (cksep3 == '1')
		document.getElementById('sep3').checked = true;
	if (ckcodec == '1')
		document.getElementById('codec').checked = true;
	if (ckcrc == '1')
		document.getElementById('crc').checked = true;
	if (ckgroupstyle == '1')
		document.getElementById('groupstyle').selectedIndex = 1;
	else
		document.getElementById('groupstyle').selectedIndex = 0;
	if (ckepnostyle == '1')
		document.getElementById('epnostyle').selectedIndex = 1;
	else
		document.getElementById('epnostyle').selectedIndex = 0;
	if (ckclipboard == '1')
		document.getElementById('clipboard').checked = true;
	if (ckcollapse == '1')
		document.getElementById('collapse').checked = true;

	if (ckspacesstyle == 0)
		document.getElementById('spacesstyle').selectedIndex = 0;
	else if (ckspacesstyle == 1)
		document.getElementById('spacesstyle').selectedIndex = 1;
	else if (ckspacesstyle == 2)
		document.getElementById('spacesstyle').selectedIndex = 2;

	if (ckstyle == 'Light Blue')
		document.getElementById('style').selectedIndex = 0;
	else if (ckstyle == 'White')
		document.getElementById('style').selectedIndex = 1;
	else if (ckstyle == 'Green')
		document.getElementById('style').selectedIndex = 2;

	if (cktitlestyle == 0)
		document.getElementById('titlestyle').selectedIndex = 0;
	else if (cktitlestyle == 1)
		document.getElementById('titlestyle').selectedIndex = 1;
	else if (cktitlestyle == 2)
		document.getElementById('titlestyle').selectedIndex = 2;

	if (ckepstyle == 0)
		document.getElementById('episodestyle').selectedIndex = 0;
	else if (ckepstyle == 1)
		document.getElementById('episodestyle').selectedIndex = 1;
	else if (ckepstyle == 2)
		document.getElementById('episodestyle').selectedIndex = 2;

	if (ckanimepaging == 20)
		document.getElementById('animepaging').selectedIndex = 0;
	else if (ckanimepaging == 25)
		document.getElementById('animepaging').selectedIndex = 1;
	else if (ckanimepaging == 30)
		document.getElementById('animepaging').selectedIndex = 2;
	else if (ckanimepaging == 50)
		document.getElementById('animepaging').selectedIndex = 3;

	setActiveStyleSheet(ckstyle);
}

function setFormValue(id)
{
	if (eval('ck' + id + ' == "1"'))
		setCookie(id, '0');
	else
		setCookie(id, '1');
}

function setActiveStyleSheet(title)
{
	var i, a, main;
	for (i = 0; (a = document.getElementsByTagName('link')[i]); i++)
	{
		if (a.getAttribute('rel').indexOf('style') != -1 && a.getAttribute('title'))
		{
			a.disabled = true;
			if (a.getAttribute('title') == title)
				a.disabled = false;
		}
	}
}

function setStyle()
{
	var style;
	var styleidx = document.getElementById('style').selectedIndex;

	if (styleidx == 0)
		style = 'Light Blue';
	if (styleidx == 1)
		style = 'White';
	if (styleidx == 2)
		style = 'Green';

	setActiveStyleSheet(style);
	setCookie('style', style);
}

function setTitleStyle()
{
	var titlestyle;
	var titlestyleidx = document.getElementById('titlestyle').selectedIndex;

	if (titlestyleidx == 0)
		titlestyle = '0';
	if (titlestyleidx == 1)
		titlestyle = '1';
	if (titlestyleidx == 2)
		titlestyle = '2';

	setCookie('titlestyle', titlestyle);
	window.location.reload();
}

function setEpisodeTitleStyle()
{
	var episodestyle;
	var episodestyleidx = document.getElementById('episodestyle').selectedIndex;

	if (episodestyleidx == 0)
		episodestyle = '0';
	if (episodestyleidx == 1)
		episodestyle = '1';
	if (episodestyleidx == 2)
		episodestyle = '2';

	setCookie('episodestyle', episodestyle);
	window.location.reload();
}

function setAminePaging()
{
	var animepaging;
	var animepagingidx = document.getElementById('animepaging').selectedIndex;

	if (animepagingidx == 0)
		animepaging = '20';
	if (animepagingidx == 1)
		animepaging = '25';
	if (animepagingidx == 2)
		animepaging = '30';
	if (animepagingidx == 3)
		animepaging = '50';

	setCookie('animepaging', animepaging);
	window.location.reload();
}

function setSpacesStyle()
{
	var spacesstyle;
	var spacesstyleidx = document.getElementById('spacesstyle').selectedIndex;

	if (spacesstyleidx == 0)
		spacesstyle = '0';
	if (spacesstyleidx == 1)
		spacesstyle = '1';
	if (spacesstyleidx == 2)
		spacesstyle = '2';

	setCookie('spacesstyle', spacesstyle);
}

function ed2k(xaobj, epnum, fnum)
{
	var eobj = elist[epnum];
	var fobj = flist[fnum];
	var xaobjname = xaobj.name;
	var eobjname = eobj.name;

	var regX = /[ ]/g;
	var ed2kstring = 'ed2k://|file|';

	var eptotLen = xa.eps.toString().length;

	if (eptotLen == 1)
		eptotLen = 2;

	var eptoFmt = '00'.concat(eobj.num).slice(-eptotLen);

	if (cktitlestyle == 1)
	{
		if (xa.japkanji != '-')
			xaobjname = xaobj.japkanji;
	}
	if (cktitlestyle == 2)
	{
		if (xa.eng != '-')
			xaobjname = xaobj.eng;
	}
	if (ckepstyle == 1)
	{
		if (eobj.namekanji != '-')
			eobjname = eobj.namekanji;
	}
	if (ckepstyle == 2)
	{
		if (eobj.nameromaji != '-')
			eobjname = eobj.nameromaji;
	}

	if (ckgroupstyle == '1')
	{
		if (fobj.groupShort == '')
			ed2kstring += '[RAW] ';
		else
			ed2kstring += '[' + fobj.groupShort + '] ';
	}
	if (cktitle == '1')
		ed2kstring += xaobjname;
	if (cksep1 == '1')
		ed2kstring += ' -';
	if (ckepisodestr == '1')
		ed2kstring += ' Episode';
	if (ckepno == '1')
	{
		if (ckepnostyle == '1')
			ed2kstring += ' ' + eptoFmt;
		else
			ed2kstring += ' ' + eobj.num;
	}
	if (cksep2 == '1')
		ed2kstring += ' -';
	if (ckepname == '1')
		ed2kstring += ' ' + eobjname;
	if (cksep3 == '1')
		if (fobj.videoCodecName != 'unknown')
			ed2kstring += ' -';
	if (ckcodec == '1')
		if (fobj.videoCodecName != 'unknown')
			ed2kstring += ' ' + fobj.videoCodecName;
	if (ckgroupstyle == '0')
	{
		if (fobj.groupShort == '')
			ed2kstring += ' [RAW]';
		else
			ed2kstring += ' [' + fobj.groupShort + ']';
	}
	if (ckcrc == '1')
	{
		if (fobj.crc != '')
		{
			if (ckgroupstyle == '0')
				ed2kstring += '(' + fobj.crc + ')';
			else
				ed2kstring += ' (' + fobj.crc + ')';
		}
	}

	ed2kstring += '.' + fobj.filetype + '|' + fobj.sizePlain + '|' + fobj.ed2kHash + '|/';

	if (ckspacesstyle == 1)
		ed2kstring = ed2kstring.replace(regX, '.');
	else if (ckspacesstyle == 2)
		ed2kstring = ed2kstring.replace(regX, '_');

	toClipboard(ed2kstring);
}

function toClipboard(txt)
{
	if (ckclipboard == '1')
	{
		if (window.clipboardData)
		{
			window.clipboardData.clearData();
			window.clipboardData.setData('Text', txt);
		}
		else if (navigator.userAgent.indexOf('Opera') != -1)
		{
			window.location = txt;
		}
		else if (window.netscape)
		{
			try
			{
				netscape.security.PrivilegeManager.enablePrivilege('UniversalXPConnect');
			}
			catch (e)
			{
				alert('Permission denied! Browse to \'about:config\' in your browser\nand set \'signed.applets.codebase_principal_support\' to true');
			}

			var clip = Components.classes['@mozilla.org/widget/clipboard;1'].createInstance(Components.interfaces.nsIClipboard);

			if (!clip)
				return;

			var trans = Components.classes['@mozilla.org/widget/transferable;1'].createInstance(Components.interfaces.nsITransferable);

			if (!trans)
				return;

			trans.addDataFlavor('text/unicode');

			var str = new Object();
			var str = Components.classes['@mozilla.org/supports-string;1'].createInstance(Components.interfaces.nsISupportsString);
			var copytext = txt;
			str.data = copytext;

			trans.setTransferData('text/unicode',str,copytext.length*2);

			var clipid = Components.interfaces.nsIClipboard;

			if (!clip)
				return false;

			clip.setData(trans,null,clipid.kGlobalClipboard);
		}
	}
	else
	{
		window.location = txt;
	}
}

function setCookie(name, value)
{
	if (value != null && value != '')
		document.cookie = name + '=' + escape(value) + '; expires=' + expiry.toGMTString();

	dc = document.cookie;

	eval( 'ck' + name + ' = "' + escape(value) + '";' );
}

function getCookie(name)
{
	var dc = document.cookie;
	var prefix = name + '=';
	var begin = dc.indexOf('; ' + prefix);

	if (begin == -1)
	{
		begin = dc.indexOf(prefix);
		if (begin != 0)
			return null;
	}
	else
	{
		begin += 2;
	}

	var end = document.cookie.indexOf(';', begin);

	if (end == -1)
		end = dc.length;

	return unescape(dc.substring(begin + prefix.length, end));
}

function toggle(targetId)
{
	target = document.getElementById(targetId);

	if (target.style.display == 'none')
	{
		target.style.display = '';
	}
	else
	{
		target.style.display = 'none';
	}
}

function filterStatus()
{
	var arg = document.getElementById('status_sel').value;

	if (arg == 'complete')
	{
		dsStatus(1);
		document.getElementById('filterused').innerHTML = 'Complete';
	}
	if (arg == 'incomplete')
	{
		dsStatus(2);
		document.getElementById('filterused').innerHTML = 'Incomplete';
	}
	if (arg == 'viewed')
	{
		dsStatus(3);
		document.getElementById('filterused').innerHTML = 'Viewed';
	}
	if (arg == 'unviewed')
	{
		dsStatus(4);
		document.getElementById('filterused').innerHTML = 'Unviewed';
	}
	if (arg == 'wishlist')
	{
		dsStatus(5);
		document.getElementById('filterused').innerHTML = 'On wishlist';
	}
	if (arg == 'all')
	{
		dsStatus(0);
		document.getElementById('filterused').innerHTML = 'NONE';
	}
}

function dsStatus(arg)
{
	var tmp_vis = new Array();

	if (arg == 0)
		tmp_vis = aids;

	if (arg == 1)
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];

			if (anime.isComplete)
			{
				tmp_vis.push(aids[i]);
			}
		}
	}
	if (arg == 2)
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];

			if (!anime.isComplete)
			{
				tmp_vis.push(aids[i]);
			}
		}
	}
	if (arg == 3)
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];

			if (anime.isWatched)
			{
				tmp_vis.push(aids[i]);
			}
		}
	}
	if (arg == 4)
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];

			if (!anime.isWatched)
			{
				tmp_vis.push(aids[i]);
			}
		}
	}
	if (arg == 5)
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];

			if (anime.isWishlist)
			{
				tmp_vis.push(aids[i]);
			}
		}
	}
	vis = tmp_vis;
	cur_vids = 0;
	cur_nids = ckanimepaging;
	curpage = 0;
	clearSort();
	writeAnime(vis,svids,snids);
	writeSpans();
}

function selectState(arg)
{
	if (arg == 'udf')
	{
		showState(-1);
		document.getElementById('filterused').innerHTML = 'mixed state';
	}
	if (arg == 'unk')
	{
		showState(0);
		document.getElementById('filterused').innerHTML = 'unknown state';
	}
	if (arg == 'hdd')
	{
		showState(1);
		document.getElementById('filterused').innerHTML = 'on hdd';
	}
	if (arg == 'cd')
	{
		showState(2);
		document.getElementById('filterused').innerHTML = 'on cd';
	}
	if (arg == 'del')
	{
		showState(3);
		document.getElementById('filterused').innerHTML = 'deleted';
	}
	if (arg == 'awd')
	{
		showState(-2);
		document.getElementById('filterused').innerHTML = 'has award(s)';
	}
	if (arg == 'all')
	{
		showState(-3);
		document.getElementById('filterused').innerHTML = 'NONE';
	}
}

function showState(arg)
{
	var tmp_vis = new Array();

	if (arg > -2)
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];

			if (anime.state == arg)
				tmp_vis.push(aids[i]);
		}
	}
	if ( arg == -3)
	{
			for (i = 0; i < aids.length; i++)
				tmp_vis.push(aids[i]);
	}
	if ( arg == -2)
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];

			if (anime.hasAwards)
				tmp_vis.push(aids[i]);
		}
	}

	vis = tmp_vis;
	curpage = 0;
	cur_vids = 0;
	cur_nids = ckanimepaging;
	clearSort();
	writeAnime(vis,svids,snids);
	writeSpans();
}

function filterType()
{
	var arg = document.getElementById('type_sel').value;

	if (arg == 'all')
	{
		showType(0);
		document.getElementById('filterused').innerHTML = 'NONE';
	}
	if (arg == 'unk')
	{
		showType(1);
		document.getElementById('filterused').innerHTML = 'Unknown type';
	}
	if (arg == 'tv')
	{
		showType(2);
		document.getElementById('filterused').innerHTML = 'TV';
	}
	if (arg == 'ova')
	{
		showType(3);
		document.getElementById('filterused').innerHTML = 'OVA';
	}
	if (arg == 'mov')
	{
		showType(4);
		document.getElementById('filterused').innerHTML = 'Movie';
	}
	if (arg == 'otr')
	{
		showType(5);
		document.getElementById('filterused').innerHTML = 'Other';
	}
	if (arg == 'web')
	{
		showType(6);
		document.getElementById('filterused').innerHTML = 'Web';
	}
}

function showType(arg)
{
	var tmp_vis = new Array();

	if (arg > 0)
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];

			if (anime.type == arg)
				tmp_vis.push(aids[i]);
		}
	}
	else
	{
		for (i = 0; i < aids.length; i++)
			tmp_vis.push(aids[i]);
	}

	vis = tmp_vis;
	curpage = 0;
	cur_vids = 0;
	cur_nids = ckanimepaging;
	clearSort();
	writeAnime(vis,svids,snids);
	writeSpans();
}

function filterLetter(arg)
{
	var tmp_vis = new Array();
	if (arg != 'all' && arg != 'number')
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];
			var fc = anime.name.substring(0,1);

			if (cktitlestyle == 2)
			{
				if (anime.eng != '-')
					fc = anime.eng.substring(0,1);
			}

			fc = fc.toLowerCase();

			if (fc == arg)
				tmp_vis.push(aids[i]);
		}
	}
	if (arg == 'all')
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];
			tmp_vis.push(aids[i]);
		}
	}
	if (arg == 'number')
	{
		for (i = 0; i < aids.length; i++)
		{
			var anime = alist[aids[i]];
			var fc = anime.name.substring(0,1).toLowerCase();

			if (cktitlestyle == 2)
			{
				if (anime.eng != '-')
					fc = anime.eng.substring(0,1);
			}

			if (fc != 'a' && fc != 'b' && fc != 'c' && fc != 'd' && fc != 'e' && fc != 'f' && fc != 'g' && fc != 'h' && fc != 'i' && fc != 'j' && fc != 'k' && fc != 'l' && fc != 'm' && fc != 'n' && fc != 'o' && fc != 'p' && fc != 'q' && fc != 'r' && fc != 's' && fc != 't' && fc != 'u' && fc != 'v' && fc != 'w' && fc != 'x' && fc != 'y' && fc != 'z')
			{
				tmp_vis.push(aids[i]);
			}
		}
	}

	vis = tmp_vis;
	curpage = 0;
	cur_vids = 0;
	cur_nids = ckanimepaging;
	clearSort();
	writeAnime(vis,svids,snids);
	writeSpans();
}

function toggleAll()
{
	for (i = 0; i < xa.enums.length; i++)
	{
		var ep = elist[xa.enums[i]];

		if (document.getElementById(['eid' + ep.id]))
			toggle('eid' + ep.id);
	}
}

function toggleAlleps()
{
	for (i = 0; i < xa.enums.length; i++)
	{
		var ep = elist[xa.enums[i]];

		if (document.getElementById(['img_ep' + ep.id + '_plus']))
			toggle('img_ep' + ep.id + '_plus');
		if (document.getElementById(['img_ep' + ep.id + '_minus']))
			toggle('img_ep' + ep.id + '_minus');
	}
}

function openAnimePage(animeid)
{
	if (navigator.userAgent.indexOf('Opera') != -1)
	{
		window.location = 'anime_opera.htm?=a' + animeid;
	}
	else
	{
		var itm = null;
		var itmapsrc = null;
		var anpgid = 'apage';
		var anpgsrcid = 'apagesrc';

		if (document.getElementById)
		{
			itm = document.getElementById(anpgid);
			itmapsrc = document.getElementById(anpgsrcid);
		}
		else if (document.all)
		{
			itm = document.all[pgid];
			itmapsrc = document.all[anpgsrcid];
		}
		else if (document.layers)
		{
			itm = document.layers[pgid];
			itmapsrc = document.layers[anpgsrcid];
		}

		if (itm.style)
			itmapsrc.src = 'anime.htm?=a' + animeid;
	}
}

function closeAnimePage()
{
	var itm = null;
	var anpgid = 'apage';
	var mlpgid = 'mylist';

	if (document.getElementById)
	{
		itm = document.getElementById(anpgid);
		itmml = document.getElementById(mlpgid);
	}
	else if (document.all)
	{
		itm = document.all[pgid];
		itmml = document.all[mlpgid];
	}
	else if (document.layers)
	{
		itm = document.layers[pgid];
		itmml = document.layers[mlpgid];
	}

	if (!itm)
	{
		// do nothing
	}
	else if (itm.style)
	{
		itm.style.display = 'none';
		itmml.style.display = 'block';
	}
	else
	{
		itm.visibility = 'hidden';
		itmml.visibility = 'show';
	}
}

function initCollapsePref()
{
	if (ckcollapse == '1')
	{
		toggleAll();
		toggleAlleps();
	}
}

function operaInit()
{
	setActiveStyleSheet(ckstyle);
	initCollapsePref();
	document.onkeydown = keyDown;
}

function sort_AB(a, b)
{
	var ar = a.split('|');
	var br = b.split('|');

	return ar[0] - br[0];
}

function sort_BA(a, b)
{
	var ar = a.split('|');
	var br = b.split('|');

	return br[0] - ar[0];
}

function reverseSort(a, b)
{
	if (a.toLowerCase() > b.toLowerCase())
		return -1;
	if (a.toLowerCase() < b.toLowerCase())
		return 1;

	return 0;
}

function reorder(arg, type, init)
{
	var new_aids = new Array();
	var sort_sel = '';
	var sort_type = '';

	for (i = 0; i < vis.length; i++)
	{
		var aname = new Object();
		var anime = alist[vis[i]];

		if (arg == 0)
		{
			if (cktitlestyle == 2)
			{
				if (anime.eng != '-')
					aname.a = anime.eng;
				else
					aname.a = anime.name;
			}
			else
			{
				aname.a = anime.name;
			}
			sort_sel = 'name_sort';
		}
		if (arg == 1)
		{
			aname.a = anime.eps;
			sort_sel = 'eps_sort';
		}
		if (arg == 2)
		{
			aname.a = anime.myWeps;
			sort_sel = 'seen_sort';
		}
		if (arg == 3)
		{
			aname.a = anime.sizebytes;
			aname.a = aname.a.replace(/[.]/g, '');
			sort_sel = 'size_sort';
		}
		if (arg == 4)
		{
			aname.a = anime.rating;
			aname.a = aname.a.replace('.', '');
			aname.a = aname.a.replace('-', '0');
			sort_sel = 'rating_sort';
		}
		if (arg == 5)
		{
			aname.a = anime.temprating;
			aname.a = aname.a.replace('.', '');
			aname.a = aname.a.replace('-', '0');
			sort_sel = 'trating_sort';
		}
		if (arg == 6)
		{
			aname.a = anime.rrating;
			aname.a = aname.a.replace('.', '');
			aname.a = aname.a.replace('-', '0');
			sort_sel = 'review_sort';
		}

		aname.id = anime.id;
		aname = aname.a + '|' + aname.id;
		new_aids.push(aname);
	}

	if (type == 0)
	{
		if(arg == 0)
			new_aids.sort();
		else
			new_aids.sort(sort_AB);

		sort_type = '_up';
	}
	else
	{
		if(arg == 0)
			new_aids.sort(reverseSort);
		else
			new_aids.sort(sort_BA);

		sort_type = '_down';
	}

	sort_aids = new Array();

	for (i = 0; i < new_aids.length; i++)
	{
		var anime = new Array();
		anime = new_aids[i].split('|');
		sort_aids.push(anime[1]);
	}

	if (init != 1)
	{
		vis = sort_aids;
		writeAnime(vis, svids, snids);
		writeSpans();

		for (i=0; i < sorts.length; i++)
		{
			document.getElementById(sorts[i]+'_sort_up').src = 'images/srtupoff.gif';
			document.getElementById(sorts[i]+'_sort_down').src = 'images/srtdnoff.gif';
		}

		if (sort_type == '_down')
			document.getElementById(sort_sel + sort_type).src = 'images/srtdnon.gif';
		else if (sort_type == '_up')
			document.getElementById(sort_sel + sort_type).src = 'images/srtupon.gif';
	}
}

function clearSort()
{
	for (i=0; i < sorts.length; i++)
	{
		document.getElementById(sorts[i]+'_sort_up').src = 'images/srtupoff.gif';
		document.getElementById(sorts[i]+'_sort_down').src = 'images/srtdnoff.gif';
	}
	document.getElementById('name_sort_up').src = 'images/srtupon.gif';
}

function keyDown(e)
{
	if (!e) e = window.event;
	var k = e.keyCode || e.which;
	// Detect ESC
	if (k == 27)
	{
		if (navigator.userAgent.indexOf('Opera') != -1)
			window.location='mylist.htm';
		else
			closeAnimePage();
		CancelKeyEvent(e);
	}
}

function CancelKeyEvent(e)
{
	if (!e) return;
	if (e.stopPropagation) e.stopPropagation();
	if (e.preventDefault) e.preventDefault();
	try { e.returnValue = false; } catch(ex){}
}
