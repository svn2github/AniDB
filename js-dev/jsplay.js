!Array.prototype.indexOf && (Array.prototype.indexOf = function (it)
{
	for (var i = 0, l = this.length; i < l; i++)
	{
		if (this[i] === it) { return i; }
	}
	return -1;
});

!Array.prototype.push && (Array.prototype.push = function ()
{
	for (var i = 0, l = arguments.length; i < l; i++)
	{
		this[this.length] = arguments[i];
	}
	return this.length;
});

Debug = true;

Logger = function(targ)
{
	if (targ && targ.nodeName)
	{
		this.element = targ;
		this.push = this.element_text_push;
	}
	else
	{
		this.push = this.alert_push;
	}
}

Logger.prototype.simple_string = function(a)
{
	var tmptxt = [];
	for (var i = 0, l = a.length; i < l; i++)
	{
		tmptxt.push(a[i] ? a[i].toString() : "[unknown]");
	}
	return tmptxt.join(" ");
}

Logger.prototype.element_text_push = function()
{
	this.element.appendChild(makeElement('br'));
	this.element.appendChild(document.createTextNode(this.simple_string(arguments)));
}

Logger.prototype.alert_push = function()
{
	alert(this.simple_string.apply(this, arguments));
}

Logger.prototype.multiple_push = function()
{
	for (var i = 0, l = this.logtarget.length; i < l; i++)
	{
		this.logtarget[i].apply(this, arguments);
	}
}

Logger.prototype.time = function(message, logit)
{
	if (message)
	{
		if (!this.starttime)
		{
			this.timerecord = [];
			this.lasttime = this.starttime = new Date().getTime();
			this.timerecord.push("Timing started at: "+message);
		}
		else
		{
			var timenow = new Date().getTime();
			this.timerecord.push(message+": "+((timenow-this.lasttime) / 1000)+"s");
			this.lasttime = timenow;
		}
	}
	if (logit)
	{
		this.push(this.timerecord.join("; ")+" - Total: "+((this.lasttime-this.starttime) / 1000)+"s");
		this.starttime = null;
	}
}

XmlDocument = function (url, callback)
{
	var me = this;
	var loaded = (function ()
		{
			Debug && Log.push("readyState marker:",me.doc.readyState,"parsed:",me.doc.parsed);
			if (typeof(me.doc.readyState) === 'undefined' || me.doc.readyState === 4 || me.doc.readyState === 'complete')
			{
				try{me.doc.onreadystatechange = null;
				}catch(e){Debug && Log.push("Why does this throw, explorer you bitch",e.name,e.message);}

				me.xpath = Implementations.meta.xpath(me.doc);

				callback && callback();
			}
		});
	if (document.implementation && document.implementation.createDocument)
	{
		this.doc = document.implementation.createDocument("", "", null);
		this.doc.addEventListener("load", loaded, false);
		this.doc.load(url);
	}
	else if (Retard) // want a proper can-do test
	{
		try{
		var tempelement = document.createElement("xml");
		//tempelement.src = url;
		document.body.previousSibling.appendChild(tempelement); // stick it in the head
		this.doc = tempelement.XMLDocument;
		this.doc.onreadystatechange = loaded;
		this.doc.load(url);
		document.body.previousSibling.removeChild(tempelement); // this might notwork/leak
		}catch(e){Debug && Log.push("Meh",e.name,e.message);}
	}
	else
	{
		Debug && Log.push("Xml document loading not supported in this browser");
		//return; // put object in zombie state?
	}	
}

Implementations = {meta:{}, xpath:{}};
Implementations.xpath.trident = function (xp, listwanted, contextnode)
{
	contextnode || (contextnode = this.doc);
	if (listwanted)
	{
		return contextnode.selectNodes(xp);
	}
	else
	{
		var result = contextnode.selectSingleNode(xp);
		if (typeof(listwanted) == 'string')
		{
			return (result && result.nodeValue || "");
		}
		return result;
	}
}
Implementations.xpath.mozilla = function (xp, listwanted, contextnode)
{
	contextnode || (contextnode = this.doc);
	if (listwanted)
	{
		var nodelist = [];
		var result = this.doc.evaluate(xp, contextnode, null,
			XPathResult.ORDERED_NODE_ITERATOR_TYPE, this.result);
		for (var node = result.iterateNext(); node; node = result.iterateNext())
		{
			nodelist.push(node)
		}
		return nodelist;
	}
	else
	{
		var result = this.doc.evaluate(xp, contextnode, null,
			XPathResult.FIRST_ORDERED_NODE_TYPE, this.result).singleNodeValue;
		if (typeof(listwanted) == 'string')
		{
			return (result && result.nodeValue || "");
		}
		return result;
	}
}
Implementations.xpath.not_implemented = function (xp, listwanted)
{
	Debug && Log.push("XPath not implemented in this browser:",xp);
	return (listwanted ? [] : null);
}

Implementations.meta.xpath = function (doc)
{
	if (doc.evaluate)
	{
		return Implementations.xpath.mozilla;
	}
	else if (typeof(doc.selectSingleNode) != 'undefined')
	{
		return Implementations.xpath.trident;	
	}
	else
	{
		return Implementations.xpath.not_implemented;
	}
}


TableTemplate = function(attribs, datalist)
{
	this.C = {AppendText:0, AppendNode:1};
	this.root = makeElement('table');
	this.row_skeleton = makeElement('tr');
	this.heading = makeElement('tr');
	if (Retard)
	{
		//this.root.appendChild(makeElement('thead'));
		//this.root.firstChild.appendChild(this.heading);
		this.body = makeElement('tbody');
		this.root.appendChild(this.body);
		this.root.firstChild.appendChild(this.heading);
	}
	else
	{
		this.body = this.root;
		this.body.appendChild(this.heading);
	}
	this.rowdata = datalist;
	this.colcount = 0;
	Debug && (this.colnames = []);
	this.actions = [[this.stripe_painter()] ];
	attribs['class'] && (this.root.className = attribs['class']);
	attribs['id'] && (this.root.id = attribs['id']);
}
TableTemplate.prototype.define_columns = function(maybeobject)
{
	var i = 0;
	if (!arguments[i].length)
	{
		// do stuff with object params
		i = 1;
	}
	for (var a = null, l = arguments.length; (i < l) && (a = arguments[i]); i++)
	{
		this.define_column(a[0],a[1],a[2],a[3],a[4],a[5]);
	}
}
TableTemplate.prototype.define_column = function(colname, classes, headdata, celldata, cellprop)
{
	var head = makeElement('th');
	var cell = makeElement('td');
	Debug && this.colnames.push(colname);
	cell.className = colname;
	head.className = [colname].concat(classes).join(" ");
	if (typeof(headdata) == 'string') head.appendChild(document.createTextNode(headdata));
	this.heading.appendChild(head);
	this.row_skeleton.appendChild(cell);
	this.add_actions(this.colcount, celldata, cellprop, cell)
	this.colcount += 1;
}
TableTemplate.prototype.add_actions = function TableTemplate_add_actions(colnumber, celldata, cellprop, cell)
{
	switch (typeof(celldata))
	{
	case 'function':
		// plan: apply the Done bits, and add callback
		var outline = celldata.outline;
		if (outline)
		{
			cell.appendChild(outline);
			this.actions[colnumber+1] = (function(t, a) { celldata(t.firstChild, a); });
		}
		else
		{
			Debug && Log.push("Function not understood in col",this.colnames[colnumber]);
		}
		break;
	case 'object':
		if (celldata.length) // non empty list
		{
			var l = celldata.length;
			var safeto = 0;
			var cheat = [];
			for (; safeto < l && typeof(celldata[safeto]) == 'string'; safeto++);
			if (safeto)
			{
				cell.appendChild(
					document.createTextNode(celldata.slice(0,safeto).join("")));
			}
			while (safeto < l && typeof(celldata[safeto]) == 'function' && celldata[safeto].outline)
			{
				cell.appendChild(celldata[safeto].outline);
				cheat.push(celldata[safeto]);
				safeto++;
			}
			if (safeto < l && typeof(celldata[safeto]) == 'object' && celldata[safeto].key)
			{
				// cheating
				cell.appendChild(document.createTextNode(EmptyText));
				var k = celldata[safeto].key;
				var aftertext = celldata.slice(safeto+1).join("");
				this.actions[colnumber+1] = (function(t, a) { a[k] && (t.firstChild.data += a[k]+aftertext); });
				safeto = l;
			}
			if (safeto < l)
			{
				Debug && Log.push(arguments.callee.name,"List contained stuff (",typeof(celldata[safeto]),") not handled in col",this.colnames[colnumber]);
			}
			if (cheat.length)
			{
				var c = cheat.length;
				this.actions[colnumber+1] = (function (t, a)
					{
						for (var i = 0, n = t.firstChild; i < c; i++)
						{
							cheat[i](n , a);
							n = n.nextSibling;
						}
					});
			}
		}
		else if (celldata.key+1)
		{
			var k = celldata.key;
			cell.appendChild(document.createTextNode(EmptyText));
			this.actions[colnumber+1] = (function(t, a) { t.firstChild.data += a[k]; });
		}
		else if (celldata.tagName) // make proper check for element nodes?
		{
			cell.appendChild(celldata);
		}
		else
		{
			Debug && Log.push("Action adder got an object it didn't know what to do with on col",this.colnames[colnumber]);
		}
		break;
	case 'number': // short-term cheat
		Debug && Log.push("Cheating param passthrough on col",this.colnames[colnumber]);
	case 'string':
		cell.appendChild(document.createTextNode(celldata));
		break;
	default:
		Debug && Log.push("TableTemplate.add_actions() hit default case in switch on col", colnumber);
	}
	
}
TableTemplate.prototype.stripe_painter = function()
{
	var even = false;
	return (function (t) { (even ^= true) && (t.className = "g_odd"); });
	// (this.even ^= true) && (row.className = "g_odd");
}
TableTemplate.prototype.apply_test = function()
{
	var row = this.row_skeleton.cloneNode(true);
	// whole row funcs
	var a = this.actions[0];
	for (var j = 0, l = a.length; j < l; j++)
	{
		a[j](row, arguments);
	}
	// cell funcs
	//try{
	for (var i = 1, n = row.firstChild; i <= this.colcount; i++)
	{
		//Debug && Log.push("apply",i);
		a = this.actions[i];		
		a && a(n , arguments);
		n = n.nextSibling;
	}
	//}catch(e){Debug && Log.push(e.name,e.message);}
	this.body.appendChild(row);
	//this.body.appendChild(this.row_skeleton.cloneNode(true));
}
TableTemplate.prototype.get = function(v)
{
	//var temp = this.rowdata.indexOf(v);
	//return (function() { return temp; });
	return {'key':this.rowdata.indexOf(v)};
}

TableTemplate.prototype.write = function()
{
	document.body.appendChild(this.root);
}

null_func = meta_iso_date = (function() { return []; });
toggle_episodes = toggle_episode = (function() { Debug && Log.push("You're 'avin' a laugh guvvna"); });
meta_href_2  = function(args)
{
	var bits = [[] ];
	var whats = [];
	for (var i = 0; i < args.length; i++)
	{
		(args[i].indexOf("=") + 1) ?
			bits[bits.length-1].push(args[i]) :
			bits.push([args[i]+"="]) && bits.push([]) && ++i && whats.push(args[i].key);
	}
	switch (bits.length)
	{
	case 3:
		return (function (t) { t.href = "http://anidb.info/perl-bin/" + "anidb.pl?" + bits[0].concat(bits[1]+arguments[whats[0]],bits[2]).join("&"); });
	case 5:
		return (function (t) { t.href = "http://anidb.info/perl-bin/" + "anidb.pl?" + bits[0].concat(bits[1]+arguments[whats[0]],bits[2],bits[3]+arguments[whats[1]],bits[4]).join("&"); });
	default:
		return (function (t)
			{
				var temp = bits[0];
				for (var i = 0, l = (bits.length-1) / 2; i < l; i++)
				{
					temp = temp.concat(bits[(i*2)+1]+arguments[whats[i]],bits[(i*2)+2]);
				}
				//Debug && Log.push("should be setting link", temp.join("&"));
				t.href = "http://anidb.info/perl-bin/" + "anidb.pl?" + temp.join("&");
			});
	}
}
make_action_icon = (function(action, iconclass, content, attribs)
{
	var a = makeElement('a');
	a.className = "i_icon i_"+iconclass;
	a.title = content;
	//var s = makeElement('span');
	//s.appendChild(document.createTextNode(content));
	a.appendChild(document.createTextNode(content));
	switch (typeof(action))
	{
	case 'string':
		a.href = action;
		return a;
	case 'function':
		var func = (function(t) { t.onclick = action; });
		func.outline = a;
		return func;
	case 'object':
		a.href = "";
		var h = meta_href_2(action);
		var func = (function(t, a) { h(t, a); });
		func.outline = a;
		return func;
	default:
		Debug && Log.push("make_action_icon was given an action it wigged out on");
	}
});
make_link = (function(href, content, attribs)
{
	if (content.key)
	{
		var k = content.key;
		var h = meta_href_2(href);
		var func = (function(t, a)
			{
				t.firstChild.data += a[k];
				h(t, a);
			});
		var link_skeleton = makeElement('a');
		func.outline = link_skeleton;
		link_skeleton.href = "";
		link_skeleton.appendChild(document.createTextNode(EmptyText));
		return func;
	}
	else
	{
		Debug && Log.push("Cheating link ignore for ",href);
		return content;
	}
});


/*window.onload = (function()
{
	//if (typeof(Done) !== 'undefined') return;

	
	Debug && (Log = new Logger(document.getElementById('logger')));
	Debug && Log.time("onload");

	SourceDoc = new XmlDocument("naruto2.xml", xml_loaded);

	var con = makeElement("p");
	var arun = makeElement("a");
	con.appendChild(arun);
	document.body.insertBefore(con, document.getElementById('logger'));
	arun.appendChild(document.createTextNode("Time a bunch of tables"+(Retard ? ": makes you leak like the bitch you are, IE" : "")));
	arun.onclick = repeat_tables;
	arun.href = "";
});*/

xml_loaded = (function ()
{
	Debug && Log.time("xml loaded");
	/*global*/Aid = SourceDoc.xpath("/root/anime/@id", "");
	var first_table = make_table();
	Debug && Log.time("table prepared");
	run_table(first_table, SourceDoc, 'jp');
	Debug && Log.time("table created");
	first_table.write();
	Debug && Log.time("table written", true);
	/*global*/Done = true;
	Init();
	//window.document = XmlDocument.doc;
});

make_table = function ()
{
	var t =  new TableTemplate({'class':"eplist",'id':"episodeList"},
		['eid','epno','eptitle','eplen','epdate','epusers','epfilecount']);
		
	t.define_columns(
		{
			'id':["e",t.get('eid')]
		},
		["expand",[],
			[
				"all: ", make_action_icon(toggle_episodes,"plus","expand all entries")
			],
			make_action_icon(toggle_episode,"plus","expand this entry")
		],
		[
			"ep","c_number","EP", // sometimes titled "PA" (for movie parts)
			make_link(["show=ep","eid",t.get('eid'),"nonav=1"],t.get('epno'))
		],
		[
			"title","c_latin","Title", // should vary sort by title type
			t.get('eptitle')
		],
		[
			"len","c_number","Len",
			[t.get('eplen'),"m"],
			{'title':t.get('eplen')}
		],
		[
			"date","c_datetime","Date",
			t.get('epdate'), {'title':meta_iso_date(t.get('epdate'))}],
		[
			"users","c_number","Users",
			make_link(["show=userlist","eid",t.get("eid")], t.get('epusers'))
		],
		[
			"files","c_number","Files",
			t.get('epfilecount')
		],
		["epaction",[],"Action",
			[
				make_action_icon(["show=addfile","aid="+Aid,"edit",t.get("eid")],"file_add","add a new file"),
				make_action_icon(["show=addep","aid="+Aid,"edit",t.get("eid")],"file_edit","edit episode"),
				make_action_icon(["show=ep","eid",t.get('eid'),"nonav=1"],"ep_revote","click for re-vote, your vote: 8")
			]
		]
		);
	return t;
}

run_table = (function (table, doc, titlepref)
{
	eps = doc.xpath("/root/anime/eps/ep", []);
	for (var i = 0, ep = null; (ep = eps[i]); i++)
	{
		table.apply_test(doc.xpath("@id", "", ep),
			doc.xpath("epno/text()", "", ep),
			doc.xpath("titles/"+titlepref+"/text()", "", ep) || "-",
			doc.xpath("len/text()", "", ep),
			doc.xpath("date/@rel", "", ep) || "-",
			doc.xpath("ucnt/text()", "", ep),
			doc.xpath("fcnt/text()", "", ep));
		
	}
});


make_table_alt = function ()
{
	var t = new TableTemplate({}, ['eid','epno']);
	//t.define_columns({}, ["eid","c_number","EpID",t.get('eid')], ["ep","c_number","EP",t.get('epno')]);
	t.define_columns({}, ["eid","c_number","EpID","A LIE"]);
	return t;
}

run_table_alt = (function (table, eps, titlepref)
{
	for (var i = 0, ep = null; (i < 1) && (ep = eps.getElementsByTagName('ep')[i]); i++)
	{
		table.apply_test(ep.getAttribute('id'),
			ep.getElementsByTagName('epno')[0] && ep.getElementsByTagName('epno')[0].firstChild.data);
	}
	Retard = false;
	//Debug && Log.push("ran table of",i,"rows");
});


repeat_tables = (function ()
{
	try{
	Done && (Done = false) || whole_table(10) || (Done = true);
	}catch(e){Debug && Log.push(e.name,e.message);}
	return false;
});

whole_table = (function (times)
{
	times || (times = 1);
	for (var i = 0; i < times; i++)
	{
		document.body.removeChild(document.body.getElementsByTagName('table')[0]);
		delete TableNode;
		Debug && Log.time("t"+i);
		var temp_table = make_table()
		run_table(temp_table, SourceDoc, 'jp');
		temp_table.write()
	}
	Debug && Log.time("t"+i, true);
});