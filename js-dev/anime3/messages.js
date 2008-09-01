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
var select_msgTO = null;
var g_note = null;
var deltimer = null;
var msgListTable = null;
var msgNewTable = null;
var msgListAction = null;
var msgNewAction = null;
var msgListH1 = null;
var msgNewH1 = null;
var oldMsgTable = null;

/* Function that fetches buddylist data */
function fetchData() {
	var req = xhttpRequest();
	if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/buddies.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=buddies', parseData);
}

/* Function that deletes a message (server)
 * @url url URL of the message to delete
 */
function doMsgDelete(url) {
	var req = xhttpRequest();
	var data = url.substr(url.indexOf('?')+1,url.length);
	if (''+window.location.hostname == '') xhttpRequestPost(req, 'msg_del.html', showDelBox, data);
	else xhttpRequestPost(req, 'animedb.pl', showDelBox, data);
}

/* Buddy Class
 * @param node Buddy data node
 */
function CBuddy(node) {
	this.uid = node.getAttribute('uid');
	this.state = node.getAttribute('state');
	this.date = node.getAttribute('date');
	this.name = nodeData(node);
}

/* Function that shows a delete box
 * @param xmldoc Response XML (can't be used)
 */
function showDelBox(xmldoc) {
	if (!g_note) {
		g_note = document.createElement('div');
		g_note.className = 'g_msg note';
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

/* Function that removes the delete box */
function removeDelBox() {
	g_note.parentNode.removeChild(g_note);
	g_note = null;
}

/* Function that deletes a message (client) */
function deleteMessage() {
	var url = this.href;
	this.removeAttribute('href');
	var tr = this.parentNode;
	while(tr.nodeName.toLowerCase() != 'tr') tr = tr.parentNode;
	tr.parentNode.removeChild(tr);
	repaintStripes(msgListTable,1);
	doMsgDelete(url);
}

/* Function that parses buddy list data
 * @param xmldoc Response XML
 */
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

/* Function that shows buddy list */
function showBuddyList() {
	var msgTO = document.getElementById('msg.to');
	if (msgTO) {
		var pNode = msgTO.parentNode;
		if (msgTO.nodeName.toLowerCase() == 'input') {
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

/* Function that selects messages on the message list based on the filter selections */
function selectMessages() {
	var div = document.getElementById('layout-content');
	var rowList = msgTable.tBodies[0].getElementsByTagName('tr');
	var ckId = -1;
	for (var i = 0; i < rowList.length; i++) {
		var row = rowList[i];
		var ck = row.getElementsByTagName('input');
		if (ckId >= 0) ck = ck[ckId];
		else {
			for (var j = 0; j < ck.length; j++) {
				if (ck[j].type != 'checkbox') continue;
				ckId = j;
				ck = ck[j];
				break;
			}
		}
		if (row.style.display == 'none') ck.checked = false;
		else ck.checked = this.checked;
	}
}

/* Creates the reply */
function createReply() {
	var quoted = false;
	if (this.name == 'msg.replyq') quoted = true;
	// alter the header
	var h1 = document.getElementById('layout-content').getElementsByTagName('h1')[0];
	if (!h1) return;
	msgListH1 = h1.cloneNode(true);
	if (!msgNewH1) {
		msgNewH1 = document.createElement('h1');
		msgNewH1.appendChild(document.createTextNode('New Message'));
	}
	h1.parentNode.replaceChild(msgNewH1,h1);
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
		var quote = '[quote]Quoted '+msgTO.value+':';
		quote += convert_input(msgBODYval.value);
		quote += '[/quote]';
		msgBodyValue = quote;
	}
	msgBODYval.parentNode.removeChild(msgBODYval);
	var div = getElementsByClassName(document.getElementsByTagName('div'),'g_definitionlist message',true)[0];
	if (!div) return;
	div.className = 'g_section message form forum';
	var table = div.getElementsByTagName('table')[0];
	div.replaceChild(createMessageInput(msgToValue,msgTitleValue,msgBodyValue),table);
	div.appendChild(createMessageActions());
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
	div.className = 'g_section message form forum';
	var form = document.createElement('form');
	form.method = 'post';
	form.action = 'animedb.pl';
	var fieldSet = document.createElement('fieldset');
	fieldSet.appendChild(createTextInput('show',null,false,true,null,'msg'));
	fieldSet.appendChild(createTextInput('do',null,false,true,null,'send'));
	form.appendChild(fieldSet);
	form.appendChild(createMessageInput());
	form.appendChild(createMessageActions());
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

/* Function that creates a new Message link list */
function createNewMessageAction() {
	var ul = document.createElement('ul');
	ul.className = 'g_list links';
	var li = document.createElement('li');
	li.className = 'g_odd';
	var a = document.createElement('a');
	a.onclick = showMsgList;
	a.appendChild(document.createTextNode('Back'));
	a.style.cursor = 'pointer';
	li.appendChild(a);
	ul.appendChild(li);
	msgNewAction = ul;
	return msgNewAction;
}

/* Function that creates corners
 * @param isTop true if we are doing the top corner */
function createCorners(isTop) {
	var corners = document.createElement('b');
	corners.className = 'r'+(isTop ? 'top' : 'bottom');
	for (var i = 1; i < 5; i++) {
		var corner = document.createElement('b');
		corner.className = 'r'+i;
		corners.appendChild(corner);
	}
	return corners;
}

/* Function that creates the new message table
 * @param msgToValue Message To value
 * @param msgTitleValue Message Title value
 * @param msgBodyValue Message contents
 */
function createMessageInput(msgToValue,msgTitleValue,msgBodyValue) {
	var div = document.createElement('div');
	div.className = 'posting-box'
	div.appendChild(createCorners(true));
	var innerDiv = document.createElement('div');
	innerDiv.className = 'inner';
	var dl = document.createElement('dl');
	dl.className = 'to';
	createDT(dl,null,'To:');
	input_msgTO = createTextInput('msg.to','20',false,false,'16',(msgToValue) ? msgToValue : '');
	input_msgTO.tabIndex = 1;
	var dd = createDD(null, null, input_msgTO);
	var curItem = {'id':"Buddy",'desc':"Buddy list",'text':"Buddy",'onclick':showBuddyList,'active':false};
	createLocalButton(dd,'buddy',curItem);
	var i = document.createElement('i');
	i.appendChild(document.createTextNode(' (username or uid)'));
	dd.appendChild(i);
	dl.appendChild(dd);
	innerDiv.appendChild(dl);
	dl = document.createElement('dl');
	dl.className = 'subject';
	createDT(dl,null,'Subject:');
	var msgTitle = createTextInput('msg.title','80',false,false,'50',(msgTitleValue) ? msgTitleValue : '');
	msgTitle.tabIndex = 2;
	createDD(dl, null, msgTitle);
	innerDiv.appendChild(dl);
	// Create the stupid smiley-box
	var smileyBox = document.createElement('div');
	smileyBox.className = 'smiley-box icons';
	smileyBox.id = 'smiley-box';
	var h3 = document.createElement('h3');
	h3.appendChild(document.createTextNode('Smilies'));
	smileyBox.appendChild(h3);
	var smileys = [
		{'id':"01",'name':"very_happy",'title':"very happy",'text':":grin:"},
		{'id':"02",'name':"happy",'title':"happy",'text':':smile:'},
		{'id':"03",'name':"sad",'title':"sad",'text':':sad:'},
		{'id':"04",'name':"shock",'title':"shocked",'text':':shock:'},
		{'id':"05",'name':"confused",'title':"confused",'text':':S'},
		{'id':"06",'name':"cool",'title':"cool",'text':':cool:'},
		{'id':"07",'name':"laughing",'title':"laughing",'text':':lol:'},
		{'id':"08",'name':"razz",'title':"razz",'text':':razz:'},
		{'id':"09",'name':"embarassed",'title':"embarassed",'text':':oops:'},
		{'id':"10",'name':"crying",'title':"crying or very sad",'text':':cry:'},
		{'id':"11",'name':"mad",'title':"evil or very mad",'text':':evil:'},
		{'id':"12",'name':"twisted",'title':"twisted evil",'text':':twisted:'},
		{'id':"13",'name':"rolleyes",'title':"rolling eyes",'text':':roll:'},
		{'id':"14",'name':"wink",'title':"wink",'text':':wink:'},
		{'id':"15",'name':"neutral",'title':"neutral",'text':':neutral:'},
		{'id':"16",'name':"sweating",'title':"upset, sighing",'text':'-_-'},
		{'id':"17",'name':"undecided",'title':"undecided",'text':':/'},
		{'id':"18",'name':"thinking",'title':"thinking",'text':'O_o'},
		{'id':"19",'name':"wtf",'title':"huh?/wtf?",'text':'o_O'},
		{'id':"20",'name':"tehehe",'title':"tehehe",'text':'-.-'},
		{'id':"21",'name':"sweatdrop",'title':"sweat drop",'text':'^_^'},
		{'id':"22",'name':"disgusted",'title':"in pain/frustration",'text':'>_<'},
		{'id':"23",'name':"surprised",'title':"surprised",'text':':O'},
		{'id':"24",'name':"dead",'title':"dead",'text':'x_X'},
		{'id':"25",'name':"nosebleed",'title':"nosebleed",'text':':nosebleed:'},
		{'id':"26",'name':"brickwall",'title':"bullheaded",'text':':brickwall:'},
		{'id':"27",'name':"zzz",'title':"sleepy",'text':':zZz:'},
		{'id':"28",'name':"mymaster",'title':"my master",'text':':mymaster:'},
		{'id':"29",'name':"thumbup",'title':"thumb up",'text':':thumbup:'},
		{'id':"30",'name':"angel",'title':"innocent",'text':':angel:'},
		{'id':"31",'name':"baka",'title':"idiot",'text':':baka:'}
	];
	for (var i = 0; i < smileys.length; i++)
		createIcon(smileyBox, smileys[i]['text'], 'removeme', null, smileys[i]['title'], 'i_smiley_'+smileys[i]['name']);
	innerDiv.appendChild(smileyBox);
	// textarea
	var messageBox = document.createElement('div');
	messageBox.className = 'message-box';
	var msgBody = document.createElement('textarea');
	msgBody.tabIndex = 3;
	msgBody.name = msgBody.id = "msg.body";
	if (msgBodyValue) msgBody.appendChild(document.createTextNode(msgBodyValue));
	messageBox.appendChild(msgBody);
	innerDiv.appendChild(messageBox);
	div.appendChild(innerDiv);
	div.appendChild(createCorners(false));
	return div;
}

/* function that creates message actions */
function createMessageActions() {
	var div = document.createElement('div');
	div.className = 'submit-box'
	div.appendChild(createCorners(true));
	var innerDiv = document.createElement('div');
	innerDiv.className = 'inner';
	var p = document.createElement('p');
	p.appendChild(createButton('do','msg.send',false,'Send','submit'));
	innerDiv.appendChild(p);
	div.appendChild(innerDiv);
	div.appendChild(createCorners(false));
	return div;
}

/* Function that updates the message list */
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
	headingList[2].className += ' c_set';		// date, anidb.sort is given so i'll just use it while i don't fix my date
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
	// SELECT MESSAGES
	li = document.createElement('li');
	li.className = 'filter_messages';
	createCheckBox(li,null,null,selectMessages,false);
	li.appendChild(document.createTextNode(' select by '));
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

/* Function that prepares the page */
function prepPage() {
	uriObj = parseURI(); // update the uriObj
	//if (!uriObj['show'] || uriObj['show'] != 'msg') return; // not interested in this page
	var areWeMessageList = getElementsByClassName(document.getElementsByTagName('div'), 'g_section message list', false)[0];
	if (!uriObj['do'] && areWeMessageList) updateMsgList(); // message list
	else if (!uriObj['do'] && !areWeMessageList) return; // i don't know this page, go away
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
