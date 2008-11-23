Debug = true;

var my_fieldset;
var my_form;

function main_addfilem(){
	/*global*/makeElement = (document.body ? (function (n) // would be just document.createElement but foxy throws
		{
			return document.createElement(n);
		}) : (function (n)
		{
			return document.createElementNS("http://www.w3.org/1999/xhtml", n);
		})
	);
	!document.body && (document.body = document.lastChild.lastChild);
	/*global*/Retard = !!document.body.swapNode; // IEsucks
	/*global*/EmptyText = Retard ? "\t" : "";
	
	if(document.getElementById('addfilem.template') == undefined)
	{
		if (document.getElementById('addfilem.search'))
		{
			document.getElementById('addfilem.search').onclick = do_group_search;
		}
		
		return;
	}
	if( my_fieldset!=null && my_form )
	{
		my_form.removeChild(my_fieldset);
		my_fieldset = null;
	}
	if( document.getElementById('addfilem.done') == undefined ){
		do_template_change();
		
		my_aid = document.getElementById('addfilem.aid').value;

		my_eplist = document.getElementById('addfilem.episodes');
		if ( my_eplist )
		{			
			my_eplist.parentNode.removeChild(my_eplist);

			my_ephash = new Array();
			for (var opt = my_eplist.firstChild; opt; opt = opt.nextSibling){
				my_ephash[opt.innerHTML] = opt.value;
			}

			my_form = document.getElementById('addfilem.form').onsubmit = do_submit;
			document.getElementById('addfilem.next').onclick = do_process_dump;
			document.getElementById('addfilem.search').onclick = do_group_search;
			document.getElementById('addfilem.template').onchange = do_template_change;
			document.getElementById('addfilem.subs.hard').onchange =
			document.getElementById('addfilem.subs.soft').onchange =
			document.getElementById('addfilem.subs.none').onchange = do_no_sub;
		}
		else
		{
			document.getElementById('addfilem.form').innerHTML = '<h4>This anime has no episodes!</h4>'
		}
	}
};

function do_process_dump(){
	parse_dump(document.getElementById('addfilem.ed2k').value);
}

function do_abort(){
	my_data = new Array()
	my_result.innerHTML = "";
	my_form.appendChild(my_dump);
	document.getElementById('addfilem.next').disabled = false;
	if( my_fieldset )
	{
		my_form.removeChild(my_fieldset);
		my_fieldset = null;
	}
}

function do_submit(){
	if( my_data.length<1 ){
		alert('Nothing to submit!');
		return false;
	}
	my_result.removeChild(my_submit);
	my_result.removeChild(my_back);
	my_submit = null;
	my_back = null;

	my_fieldset = makeElement('fieldset');
	for (var i = 0, file = null; (file = my_data[i]); i++){
		var input = makeElement('input');
		input.type = 'hidden';
		input.name = 'addfilem.data.'+i;
		input.value = file.size+"."+file.ed2k+"."+file.filetype+"."+(file.crc||'');
		my_fieldset.appendChild(input);
	}
	var input = makeElement('input');
	input.type = 'hidden';
	input.name = 'addfilem.action';
	input.value = '1';
	my_fieldset.appendChild(input);		
	my_form.appendChild(my_fieldset);
	
	return true;
}

function do_no_sub(){
	if( document.getElementById('addfilem.template').value == "dvd" )
		document.getElementById('addfilem.sub.lang').disabled = document.getElementById('addfilem.subs.none').checked;
}
function do_template_change(){
	var value = document.getElementById('addfilem.template').value; 
	
	document.getElementById('addfilem.aud.lang').value = 2;
	document.getElementById('addfilem.sub.lang').value = 4;
	
	document.getElementById('addfilem.aud.lang').disabled = (value=="raw" || value=="fan");
	document.getElementById('addfilem.sub.lang').disabled = (value=="raw" || value=="dub");
	
	var bool = (value == "raw" || value == "dub");
	document.getElementById('addfilem.subs.hard').disabled = bool;
	document.getElementById('addfilem.subs.soft').disabled = bool;
	document.getElementById('addfilem.subs.none').disabled = bool;
	document.getElementById('addfilem.subs.none').checked = bool;
	
	switch(value){
		case "dvd":
			document.getElementById('addfilem.subs.soft').checked = 1;
			document.getElementById('addfilem.aud.lang').value = 4;
		case "dub":
			document.getElementById('addfilem.quality').value = 2;
			document.getElementById('addfilem.type').value = 7;
			break;			
		case "fan":
			document.getElementById('addfilem.subs.hard').checked = 1;
			document.getElementById('addfilem.subs.none').disabled = 1;
		case "raw":
			document.getElementById('addfilem.quality').value = 3;
			document.getElementById('addfilem.type').value = 5;
			break;
	}
}

function do_group_search(){
	document.getElementById('addfilem.search').disabled = true;
	var name = document.getElementById('addfilem.group').value;
	if(name.length<2){
		alert("Give a name! (At least 2 characters.)");
		document.getElementById('addfilem.search').disabled = false;
	}
	else{
		var req = xhttpRequest();
		xhttpRequestFetch(req, 'animedb.pl?show=xml&t=groupsearch&search='+escape(name), parse_groups);
	}
}

function add_option(select, value, text){
	var opt = (select.ownerDocument||select.document).createElement('OPTION');
	opt.value = value;
	opt.text = text;
	select.options.add(opt);
	//select.appendChild(new Option(text, value));
}
function parse_groups(xmldoc){
	var root = xmldoc.getElementsByTagName('root').item(0);
	var select = document.getElementById("addfilem.group.list");
	select.innerHTML = "";
	
	add_option(select, "0", "no group");
	
	var groups = root.getElementsByTagName('group');
	for (i = 0; i < groups.length; i++){
	 	add_option(select, groups[i].getAttribute('gid'),
	 		groups[i].getAttribute('name')+" ["+groups[i].getAttribute('sname')+"]");
	}
	document.getElementById('addfilem.search').disabled = false;
}

function parse_dump(text){
	if( text.length<1 ){
		alert('No data in dump.');
		return;
	}
	document.getElementById('addfilem.next').disabled = true;
	
	var re_ed2k_link = /ed2k\:\/\/\|file\|(.+)\|(\d+)\|([a-f0-9]{32})\|/i;	
	var lines = text.split("\n");
	var index = 0;
	var skipped = '';
	my_data = new Array();
	for (i in lines){
		if(re_ed2k_link.test(lines[i])){
			var file = new MyFile(RegExp.$1, RegExp.$2, RegExp.$3);
			my_data[index++] = file;
		}
		else if(/ed2k\:/.test(lines[i]))
			skipped += "\n"+lines[i];
	}
	if( my_data.length>26){
		alert('Too many links. Max 26 is allowed.');
		document.getElementById('addfilem.next').disabled = false;
		return;
	}
	if( my_data.length<1 ){
		alert('No ed2k links found.');
		document.getElementById('addfilem.next').disabled = false;
		return;
	}
	if( skipped.length>0 )
		alert('Some lines were skipped (invalid): '+skipped);
	
	my_dump = document.getElementById('addfilem.dump');
	my_form = my_dump.parentNode;
	my_form.removeChild(my_dump);
  
	var t = make_table_new_files();
	run_table_new_files(t, my_data);
	
	my_result = makeElement('div');
	my_result.id = 'addfilem.result';
	my_result.className = 'result';
	
	var h4 = makeElement('h4');
	h4.appendChild(document.createTextNode('Result'));
	
	my_submit = makeElement('input');
	my_submit.type = "submit";
	my_submit.name = "addf.mass";
	my_submit.value = "Add files";

	my_back = makeElement('input');
	my_back.type = "button";
	my_back.value = "Back";
	my_back.onclick = do_abort;
	
	my_result.appendChild(h4);
	my_result.appendChild(t.root);
	my_result.appendChild(my_back);
	my_result.appendChild(document.createTextNode(' '));
	my_result.appendChild(my_submit);
	my_form.appendChild(my_result);
}

function MyEpisode(id, epno){
	this.id = id;
	this.epno = epno;
	this.toString = function(){
		return this.id+"\t"+this.epno;
	}
}
function MyFile(name, size, ed2k){	
	this.toString = function(){
		return this.name+"\n"+this.size+"\n"+this.ed2k;
	}
	this.find_filetype = function(text){
		var regx = /\.([a-z0-9]{2,4})$/i;
		if(regx.test(text))
			return RegExp.$1.toLowerCase();
	}
	this.find_crc = function(text){
		var regx = /[\[\(]([a-f0-9]{8})[\]\)]/i;
		if(regx.test(text))
			return (RegExp.$1).toLowerCase();
	}
	this.find_epno = function(text){
		var regx = /\u7B2C(\d+)(\u8A71|\u5DFB)/;
		if(regx.test(text))
			return (RegExp.$1)*1;
		else if (/\[([0-9]{0,3})\]/.test(text))
			return (RegExp.$1)*1;
		else{
			regx = /(\-[\s\_|\.]?|[\s\_|\.])\[*([SCTPO]?\d+)\]*(v\d)?[\s\_|\.\-\(\[\.]/;
			if(regx.test(text))
				return (RegExp.$2)*1 || RegExp.$2;
		}
	}
	this.find_version = function(text){
		var regx = /v(\d)[\_\.\(\[]/;
		if(regx.test(text))
			return (RegExp.$1)*1
	}
	this.name = name;
	this.size = size;
	this.ed2k = ed2k.toLowerCase();
	this.filetype = this.find_filetype(name);
	this.crc = this.find_crc(name);
	this.version = this.find_version(name);
	this.eid = my_ephash[this.find_epno(name)];
}

make_table_new_files = function ()
{
	var t = new TableTemplate({'class':"filelist",'id':"fileList"},
		['id','name','crc','ext', 'eid','version']);

	t.define_columns(
		["id","id","#",t.get('id')],
		["name","name","name",t.get('name')],
		["crc","crc","crc",t.get('crc')],
		["ext","ext","ftyp",t.get('ext')],
		["eid","eid","epno",make_select_epno(t.get('id'), t.get('eid')) ],
		["version","version","ver",make_select_version(t.get('id'), t.get('version')) ]
		);
	return t;
}

run_table_new_files = (function (table, data)
{
	for (var i = 0, file = null; (file = data[i]); i++)
	{
		table.apply_test(i,
			file.name,
			file.crc||'N/A',
			file.filetype,
			file.eid,
			file.version||'1'
			);		
	}
});

make_select_epno = (function(id, eid)
{
	var select = my_eplist.cloneNode(true);

	var func = (function(t,a){
		t.name = "addfilem.eid."+a[id.key];
		t.value = a[eid.key];
	});

	func.outline = select;
	return func;
});

make_select_version = (function(id, version)
{
	var select = makeElement('select');

	var func = (function(t,a){
		t.name = "addfilem.version."+a[id.key];
		for (var i = 1; i<6; i++){
			//t.appendChild(new Option('v'+i, i, false, i==a[version.key]));
			add_option(t, i, 'v'+i);
		}
		t.selectedIndex = (a[version.key]||1) - 1;
	});

	func.outline = select;
	return func;
});

addLoadEvent(main_addfilem);
