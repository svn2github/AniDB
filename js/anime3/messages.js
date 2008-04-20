/* file message page scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 * @contents Core Functions
 * version 1.2 (15.01.2008)
 * version 1.3 (17.04.2008) - Anidb 0.1.50 version update
 */

// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var msgTable;
var headingList;
var input_msgTO;
var select_msgTO;
var g_note = null;
var deltimer = null;
var msgListTable = null;
var msgNewTable = null;
var msgListAction = null;
var msgNewAction = null;
var msgListH1 = null;
var msgNewH1 = null;

function fetchData() {
  var req = xhttpRequest();
  if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/buddies.xml', parseData);
  else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=buddies', parseData);
}

function doMsgDelete(url) {
  var req = xhttpRequest();
	var data = url.substr(url.indexOf('?')+1,url.length);
  if (''+window.location.hostname == '') xhttpRequestPost(req, 'msg_del.html', showDelBox, data);
  else xhttpRequestPost(req, 'animedb.pl', showDelBox, data);
}

function CBuddy(node) {
  this.uid = node.getAttribute('uid');
  this.state = node.getAttribute('state');
  this.date = node.getAttribute('date');
  this.name = nodeData(node);
}

function showDelBox(xmldoc) {
	if (!g_note) {
		g_note = document.createElement('div');
		g_note.className = 'g_section g_notebox';
		var h3 = document.createElement('h3');
		h3.appendChild(document.createTextNode('NOTE:'));
		g_note.appendChild(h3);
		var p = document.createElement('p');
		p.appendChild(document.createTextNode('Message deleted.'));
		g_note.appendChild(p);
	}
	var msg_all = getElementsByClassName(document.getElementsByTagName('div'), 'g_content msg_all', true)[0];
	if (!msg_all) return;
	msg_all.insertBefore(g_note,msg_all.firstChild);
	self.clearTimeout(deltimer);
	//deltimer = self.setTimeout('removeDelBox()', 1000);
}

function removeDelBox() {
	g_note.parentNode.removeChild(g_note);
	g_note = null;
}

function deleteMessage() {
	var url = this.href;
	this.removeAttribute('href');
	var tr = this.parentNode;
	while(tr.nodeName.toLowerCase() != 'tr') tr = tr.parentNode;
	tr.parentNode.removeChild(tr);
	repaintStripes(msgListTable,1);
	doMsgDelete(url);
}

function parseData(xmldoc) {
  var root = xmldoc.getElementsByTagName('root').item(0);
  var t1 = new Date();
  var buddyEntries = root.getElementsByTagName('buddy');
	var select = createBasicSelect('msg.to','msg.to',null);
	createSelectOption(select,'manual', '', false);
  for (var i = 0; i < buddyEntries.length; i++) {
    var buddyEntry = new CBuddy(buddyEntries[i]);
    createSelectOption(select, buddyEntry.name, buddyEntry.uid, false);
  }
  select_msgTO = select;
  var msgTO = document.getElementById('msg.to');
  var pNode = msgTO.parentNode;
  pNode.removeChild(msgTO);
  pNode.insertBefore(select_msgTO.cloneNode(true),pNode.firstChild);
}

/* Shows buddy list */
function showBuddyList() {
  var msgTO = document.getElementById('msg.to');
  if (msgTO) {
    var pNode = msgTO.parentNode;
    if (msgTO.nodeName == 'input') {
      if (!select_msgTO) fetchData();
      else {
        pNode.removeChild(msgTO);
        pNode.insertBefore(select_msgTO.cloneNode(true),pNode.firstChild);
      }
    } else {
      pNode.removeChild(msgTO);
      pNode.insertBefore(input_msgTO.cloneNode(true),pNode.firstChild);
    }
  }
  var cln = this.className;
  if (cln.indexOf('f_selected') > 0) this.className = this.className.replace(' f_selected','');
  else this.className += ' f_selected';
}

/* Filters messages */
function filterMessages() {
  var filter = this.value;
  var type = this.parentNode.className.substring(this.parentNode.className.indexOf("filter_")+7,this.parentNode.className.length);
  var div = document.getElementById('layout-content');
  var rowList = msgTable.tBodies[0].getElementsByTagName('tr');
  // find the "type" col
  var typeCol = -1;
  for (var i = 0; headingList.length; i++){
    var cell = headingList[i];
    if (cell.className.indexOf(type) < 0) continue;
    typeCol = cell.cellIndex;
    break;
  }
  if (typeCol == -1) return; // didn't find a type col
  for (var i = 0; i < rowList.length; i++) {
    var row = rowList[i];
    var cell = row.cells[typeCol];
    if (cell.nodeName.toLowerCase() != 'td') continue;
    if (filter == 'all') { row.style.display = ''; continue; }
    if (cell.firstChild.nodeValue != filter) row.style.display = 'none';
    else row.style.display = '';
  }
}

/* Creates the reply */
function createReply() {
	var quoted = false;
	if (this.name == 'msg.replyq') quoted = true;
	// fetched required data
	var divA = getElementsByClassName(document.getElementsByTagName('div'),'g_section message entity',true)[0];
	divA.className = divA.className.replace('entity','form');
	var inputs =  divA.getElementsByTagName('input');
	var msgDO = getElementsByName(inputs,'do',true)[0];
	msgDO.value = 'send'; // value needs to change
	var msgMID = getElementsByName(inputs,'mid',true)[0];
	msgMID.parentNode.removeChild(msgMID); // got to remove this
	var msgTO = getElementsByName(inputs,'msg.to',true)[0];
	var msgToValue = msgTO.value;
	msgTO.parentNode.removeChild(msgTO);
	var msgTITLE = getElementsByName(inputs,'msg.title',true)[0];
	var msgTitleValue = msgTITLE.value;
	msgTITLE.parentNode.removeChild(msgTITLE);
	var msgBODYval = getElementsByName(inputs,'msg.body',true)[0];
	var msgBodyValue = null;
	if (quoted) {
		var quote = '[quote="'+msgTO.value+'"]';
		quote += convert_input(msgBODYval.value);
		quote += '[/quote]';
		msgBodyValue = quote;
	}
	msgBODYval.parentNode.removeChild(msgBODYval);
	var div = getElementsByClassName(document.getElementsByTagName('div'),'g_definitionlist message',true)[0];
	var table = div.getElementsByTagName('table')[0];
	createMessageInput(msgToValue,msgTitleValue,msgBodyValue);
	table.parentNode.replaceChild(msgNewTable,table);
	if (init_formating) init_formating(); // should be loaded but you never know
}

/* creates new message */
function createNewMessage() {
	// pre: we are on the message list and want to send a new msg
	var h1 = document.getElementById('layout-content').getElementsByTagName('h1')[0];
	if (!h1) return;
	msgListH1 = h1.cloneNode(true);
	if (!msgNewH1) {
		msgNewH1 = document.createElement('h1');
		msgNewH1.appendChild(document.createTextNode('New Message'));
	}
	h1.parentNode.replaceChild(msgNewH1,h1);
	var div = getElementsByClassName(document.getElementsByTagName('div'),'g_section message',true)[0];
	if (!div) return;
	var msg_all = div.parentNode;
	div.className = 'g_section message form';
	var form = document.createElement('form');
	form.method = 'post';
	form.action = 'animedb.pl';
	var fieldSet = document.createElement('fieldset');
	var ishow = createTextInput('show',null,false,true,null);
	ishow.value = "msg";
	fieldSet.appendChild(ishow);
	var ido = createTextInput('do',null,false,true,null);
	ido.value = "send";
	fieldSet.appendChild(ido);
	form.appendChild(fieldSet);
	var deflist = document.createElement('div');
	deflist.className = 'g_definitionlist message';
	createMessageInput();
	deflist.appendChild(msgNewTable);
	form.appendChild(deflist);
	while(div.childNodes.length) div.removeChild(div.firstChild);
	div.appendChild(form);
	// reset action list
	createNewMessageAction();
	var newmsgListAction = msgListAction;
	msg_all.replaceChild(msgNewAction,msgListAction);
	msgListAction = newmsgListAction;
	if (init_formating) init_formating(); // should be loaded but you never know
}

/* Shows the message list */
function showMsgList() {
	var h1 = document.getElementById('layout-content').getElementsByTagName('h1')[0];
	if (!h1) return;
	msgNewH1 = h1.cloneNode(true);
	h1.parentNode.replaceChild(msgListH1,h1);
	var div = getElementsByClassName(document.getElementsByTagName('div'),'g_section message',true)[0];
	if (!div) return;
	var msg_all = div.parentNode;
	div.className = 'g_section message list';
	while(div.childNodes.length) div.removeChild(div.firstChild);
	div.appendChild(msgListTable);
	// reset action list
	msg_all.replaceChild(msgListAction,msgNewAction);
	msgNewAction = null;
	createNewMessageAction();
}

function createNewMessageAction() {
	var ul = document.createElement('ul');
	ul.className = 'g_list links';
	var li = document.createElement('li');
	li.className = 'g_odd';
	var a = document.createElement('a');
	a.onclick = showMsgList;
	a.appendChild(document.createTextNode('Back'));
	li.appendChild(a);
	ul.appendChild(li);
	msgNewAction = ul;
	return msgNewAction;
}

function createMessageInput(msgToValue,msgTitleValue,msgBodyValue) {
	var table = document.createElement('table');
	var caption = document.createElement('caption');
	caption.appendChild(document.createTextNode('New Message'));
	table.appendChild(caption);
	var tbody = document.createElement('tbody');
	var row = document.createElement('tr');
	row.className = 'g_odd to';
	createHeader(row, 'field', 'To', null);
	input_msgTO = createTextInput('msg.to','20',false,false,'16');
	input_msgTO.tabIndex = 1;
	if (msgToValue) input_msgTO.value = msgToValue;
	var cell = createCell(null, 'value', input_msgTO, null);
	var curItem = {'id':"Buddy",'desc':"Buddy list",'text':"Buddy",'onclick':showBuddyList,'active':false};
	createLocalButton(cell,'buddy',curItem);
	var i = document.createElement('I');
	i.appendChild(document.createTextNode(' (username or id)'));
	cell.appendChild(i);
	row.appendChild(cell);
	tbody.appendChild(row);
	row = document.createElement('tr');
	row.className = 'title';
	createHeader(row, 'field', 'Title', null);
	var msgTitle = createTextInput('msg.title','80',false,false,'50');
	msgTitle.tabIndex = 2;
	if (msgTitleValue) msgTitle.value = msgTitleValue;
	createCell(row, 'value', msgTitle, null);
	tbody.appendChild(row);
	row = document.createElement('tr');
	row.className = 'g_odd body';
	createHeader(row, 'field', 'Body', null);
	var msgBody = document.createElement('textarea');
	msgBody.tabIndex = 3;
	msgBody.rows = "15";
	msgBody.cols = "60";
	msgBody.name = msgBody.id = "msg.body";
	if (msgBodyValue) msgBody.appendChild(document.createTextNode(msgBodyValue));
	createCell(row, 'value', msgBody, null);
	tbody.appendChild(row);
	row = document.createElement('tr');
	row.className = 'action'; 
	cell = createCell(null, 'value', createButton('do','msg.send',false,'Send Message','submit'), null);
	cell.colSpan = "2";
	row.appendChild(cell);
	tbody.appendChild(row);
	table.appendChild(tbody);
	msgNewTable = table;
	return msgNewTable;
}

function updateMsgList() {
	var div = getElementsByClassName(document.getElementsByTagName('div'), 'message list', true)[0];
	if (!div) return;
	msgTable = div.getElementsByTagName('table')[0];	
	if (!msgTable) return;
	msgListTable = msgTable;
	headingList = msgTable.getElementsByTagName('th');
	var actionList = getElementsByClassName(document.getElementsByTagName('ul'), 'g_list links', true)[0];
	// I know the headings i need so..
	headingList[0].className += ' c_none';		// checkbox
	headingList[1].className += ' c_latin';	// state
	headingList[2].className += ' c_date';		// date
	headingList[3].className += ' c_latin';	// from
	headingList[4].className += ' c_latin';	// type
	headingList[5].className += ' c_latin';	// title
	headingList[6].className += ' c_none';		// action
  
	// append some extra filtering options
	// FILTER BY TYPE
	var optionArray = {	'all':{"text":' all '},'normal':{"text":' normal '},'system':{"text":' system '},
						'bulk':{"text":' bulk '},'mod':{"text":' mod '}};
	var select = createSelectArray(null,null,null,filterMessages,'all',optionArray);
	var li = document.createElement('li');
	li.className = 'filter_type';
	li.appendChild(document.createTextNode('type: '));
	li.appendChild(select);
	li.appendChild(document.createTextNode(' '));
	actionList.insertBefore(li,actionList.firstChild);
	// FILTER BY STATE
	optionArray = {'all':{"text":' all '},'new':{"text":' new '},'old':{"text":' old '}};
	select = createSelectArray(null,null,null,filterMessages,'all',optionArray);
	li = document.createElement('li');
	li.className = 'filter_state';
	li.appendChild(document.createTextNode('state: '));
	li.appendChild(select);
	li.appendChild(document.createTextNode(' '));
	actionList.insertBefore(li,actionList.firstChild);

	// update the delete action for messages
	var rows = msgTable.tBodies[0].rows;
	for (var i = 1; i < rows.length; i++) {
		var row = rows[i];
		var cell = getElementsByClassName(row.getElementsByTagName('td'), 'action', true)[0];
		if (!cell) continue;
		var a = cell.getElementsByTagName('a')[0];
		if (!a) continue;
		a.onclick = deleteMessage;
	}
	
	//update the new message link
	var newMsg = getElementsByClassName(actionList.getElementsByTagName('li'), 'new', true)[0];
	var a = newMsg.getElementsByTagName('a')[0];
	a.removeAttribute('href');
	a.onclick = createNewMessage;
	a.style.cursor = 'pointer';
	
	// now do some clean up of the table to allow for cleaner sort
	var tbody = msgTable.tBodies[0];
	var thead = document.createElement('thead');
	var tfoot = document.createElement('tfoot');
	thead.appendChild(tbody.rows[0]);
	tfoot.appendChild(tbody.rows[tbody.rows.length-1]);
	msgTable.insertBefore(thead,tbody);
	msgTable.appendChild(tfoot);
	
	msgListAction = actionList;
  
	// apply the sorting function
	init_sorting(msgTable,'date','up');
}

function prepPage() {
	uriObj = parseURI(); // update the uriObj
	if (!uriObj['show'] || uriObj['show'] != 'msg') return; // not interested in this page
	if (!uriObj['do']) updateMsgList(); // message list
	else {
		if (uriObj['do'] == 'new') {
			// if we are on the message send page (which is where we should be)
			var msgTO = document.getElementById('msg.to');
			if (msgTO && msgTO.type != 'hidden') {
				input_msgTO = msgTO.cloneNode(true);
				var curItem = {'id':"Buddy",'desc':"Buddy list",'text':"Buddy",'onclick':showBuddyList,'active':false};
				msgTO.parentNode.insertBefore(createLocalButton(null,'buddy',curItem),msgTO.nextSibling);
			}
		} else if (uriObj['do'] == 'show') {
			// search for some stuff
			var inputs = document.getElementsByTagName('input');
			var reply, replyq;
			for (var i = 0; i < inputs.length; i++) {
				var input = inputs[i];
				if (!reply && input.value.toLowerCase() == 'reply') { reply = input; continue; }
				if (!replyq && input.value.toLowerCase() == 'quote') { replyq = input; continue; }
				if (reply && replyq) break; // no need to do more;
			}
			if (!reply || !replyq) return;
			// i'm going to regret this, but thus is the way js works cross-browser...
			var nreply = createButton('msg.reply','msg.reply',false,'Reply');			
			var nreplyq = createButton('msg.replyq','msg.replyq',false,'Quote');
			nreply.onclick = createReply;
			nreplyq.onclick = createReply;
			reply.parentNode.replaceChild(nreply,reply);
			replyq.parentNode.replaceChild(nreplyq,replyq);
		}
	}
}

//window.onload = prepPage;
addLoadEvent(prepPage);
