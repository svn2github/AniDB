function main_addepm() {
	makeElement = (document.body ? (function (n) // would be just document.createElement but foxy throws
		{
			return document.createElement(n);
		}) : (function (n)
		{
			return document.createElementNS("http://www.w3.org/1999/xhtml", n);
		})
	);
	
	my_counter = -1;
	my_form = document.getElementById('addepm.form');
	if ( my_form ){
		removeWS(my_form);

		my_template = document.getElementById('e0');
		if(!my_template)
			return;
		
		//this user has add rights
		my_end = my_template.nextSibling;
		my_form.removeChild(my_template);
		
		my_eps = [];
		for (var i = 0, n = my_form.firstChild, last; n; i++)
		{
			var t = n.nextSibling;
			if ( n.id && n.className == 'ep' )
			{
				var ep = new MyEp(n);
				if(last){
					last.next = ep;
					ep.prev = last;
				}
				last = 
				my_eps[ep.id] = ep;
			}
			n = t;
		}
		my_form.onsubmit = do_submit;
	}
};

function do_submit(){
	var regex = /^[\x20-\x7E]{0,1000}$/;
	for (var k in my_eps){
		var ep = my_eps[k];
		if(!ep.name.value){
			alert("Ep "+ep.epno.value+": Main title can not be blank.");
			return false;
		}
		if(!regex.test(ep.name.value)){
			alert("Invalid: '"+ep.name.value+"'\nOnly printable 7-bit ASCII is allowed for main title. Max length is 1000.");
			return false;
		}
		if(!regex.test(ep.romaji.value)){
			alert("Invalid: '"+ep.romaji.value+"'\nOnly printable 7-bit ASCII is allowed for transcribed title. Max length is 1000.");
			return false;
		}
	}
	return true;
}

function removeWS(elem){
	for (var i = 0, n = elem.firstChild; n; i++)
	{
		var t = n.nextSibling;
		if ( n.nodeType==3 )
			elem.removeChild(n);
		else
			removeWS(n);
		n = t;
	}
}

function only7bit(n)
{
	var isascii = !/[^\x20-\x7E]/.test(n.value);
	var ismarkederror = n.className.indexOf("g_error") >= 0;
	if (isascii && ismarkederror) { n.className = n.className.replace(/\s*g_error/,""); }
	else if (!isascii && !ismarkederror) { n.className += " g_error"; }
}
function onchange_check7bit()
{
	only7bit(this);
}

function MyEp(elem, id)
{
	this.elem = elem;
	this.id = id || elem.id.substring(1);

	this.epno = elem.firstChild.firstChild;

	var regexp = /^(s|c|t|p|o)(\d+)$/i;
	if(regexp.test(this.epno.value))
	{
		this.type = RegExp.$1;
		this.numb = RegExp.$2*1;
	}
	else
	{
		this.type = '';
		this.numb = this.epno.value*1;
	}
	
	this.name = this.epno.nextSibling;
	
	this.length = elem.firstChild.nextSibling.firstChild;
	this.romaji = this.length.nextSibling;
	this.aired = elem.firstChild.nextSibling.nextSibling.firstChild;
	this.kanji = this.aired.nextSibling;
	if(elem.firstChild.nextSibling.nextSibling.nextSibling)
		this.extra = elem.firstChild.nextSibling.nextSibling.nextSibling.firstChild;
	
	if(id<0){
		this.epno.name = 'addepm.'+id+'.epno';
		this.name.name = 'addepm.'+id+'.title4';
		this.length.name = 'addepm.'+id+'.length';
		this.romaji.name = 'addepm.'+id+'.title75';
		this.aired.name = 'addepm.'+id+'.aired';
		this.kanji.name = 'addepm.'+id+'.title2';
		if ( this.extra ){
			var x = this.extra.name.indexOf('.title');
			this.extra.name = 'addepm.'+id+'.title'+this.extra.name.substring(x+6);
		}
	}
	
	if(!this.epno.disabled){
		this.elem.appendChild(mkButton(this.id, 'up', moveEpUp));
		this.elem.appendChild(mkButton(this.id, 'down', moveEpDown));
	}
	if(my_template)
		this.elem.appendChild(mkButton(this.id, 'new', newEp));
	if(id<0)
		this.elem.appendChild(mkButton(this.id, 'del', delEp));
	
	this.setEpno = ( function(type,numb){
		this.type = type;
		this.numb = numb;
		this.epno.value = type+numb;
	});
	
	only7bit(this.name);
	only7bit(this.romaji);
	this.name.onchange =
	this.romaji.onchange = onchange_check7bit;
}

function mkButton(id, value, func)
{
	var butt = makeElement('input');
	butt.id = id;
	butt.type = 'button';
	butt.value = value;
	butt.onclick = func;
	return butt;
}

function newEp(event){
	var eid = event.target.id;
	var cur = my_eps[eid];
	var nex = cur.next;
	if(nex && cur.type==nex.type && nex.epno.disabled)
		return;
	var ins = my_form.insertBefore(my_template.cloneNode(true), nex ? nex.elem : my_end );

	var ep = my_eps[my_counter] = new MyEp(ins, my_counter--);
	ep.setEpno(cur.type, cur.numb+1);
	ep.name.value = 'Episode '+ep.epno.value;

	ep.next = nex;
	ep.prev = cur;
	cur.next = ep;
	if(nex)	nex.prev = ep;
	
  updateEpno(ep.next, ep.type, 1);
}

function delEp(event){
	var eid = event.target.id;
	var cur = my_eps[eid];
	var pre = cur.prev;
	var nex = cur.next;
	if(pre) pre.next = cur.next;
	if(nex)	nex.prev = pre;
	
	my_form.removeChild(cur.elem);
	my_eps[eid] = null;

	if(nex) updateEpno(nex, cur.type, -1);
}

function moveEpDown(event){
	var eid = event.target.id;
	var cur = my_eps[eid];
	var pre = cur.prev;
	var nex = cur.next;
	if(!nex || cur.type!=nex.type || nex.epno.disabled) return;

	if(pre) pre.next = nex;
	var end = nex.next;
	if(end) end.prev = cur;
	nex.prev = pre;
	nex.next = cur;
	cur.prev = nex;
	cur.next = end;

	my_form.removeChild(cur.elem);
	my_form.insertBefore(cur.elem, end ? end.elem : my_end);
	
	var tmp = cur.numb;
	cur.setEpno(nex.type, nex.numb);
	nex.setEpno(nex.type, tmp);
}

function moveEpUp(event){
	var eid = event.target.id;
	var cur = my_eps[eid];
	var pre = cur.prev;
	var nex = cur.next;
	if(!pre || cur.type!=pre.type || pre.epno.disabled) return;

	if(nex) nex.prev = pre;
	var beg = pre.prev;
	if(beg) beg.next = cur;
	pre.next = nex;
	pre.prev = cur;
	cur.prev = beg;
	cur.next = pre;

	my_form.removeChild(cur.elem);
	my_form.insertBefore(cur.elem, pre.elem);
	
	var tmp = cur.numb;
	cur.setEpno(pre.type, pre.numb);
	pre.setEpno(pre.type, tmp);
}

function parseEpNum(str,type,val) {
  var num = (!type) ? Number(str) : Number(str.substr(1));
  if (val) num += val;
  return num;
}

function updateEpno(elem, type, val){
	if(elem && elem.type==type){
    var cur = parseEpNum(elem.epno.value,type);
    var after = parseEpNum(elem.epno.value,type,val);
    var prev = parseEpNum(elem.prev.epno.value,type);
    var next = parseEpNum(elem.next.epno.value,type);
    var tgt = (val > 0) ? next : prev;
		if (!isNaN(tgt)) elem.setEpno(type, elem.numb+val);
    if (isNaN(tgt) && ((after == next) || (cur == prev))) elem.setEpno(type, elem.numb+val);
    updateEpno(elem.next, type, val);
	}
}

addLoadEvent(main_addepm);
