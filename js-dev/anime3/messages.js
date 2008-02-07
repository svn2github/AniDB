/**
 * @file message page scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 * @contents Core Functions
 * @version 1.1 (18.06.2007)
 */

// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var msgTable;
var headingList;
var input_msgTO;
var select_msgTO;

function fetchData() {
  var req = xhttpRequest();
  if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/buddies.xml', parseData);
  else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=buddies', parseData);
}

function CBuddy(node) {
  this.uid = node.getAttribute('uid');
  this.state = node.getAttribute('state');
  this.date = node.getAttribute('date');
  this.name = nodeData(node);
}

function parseData(xmldoc) {
  var root = xmldoc.getElementsByTagName('root').item(0);
  var t1 = new Date();
  var buddyEntries = root.getElementsByTagName('buddy');
  var select = document.createElement('SELECT');
  select.size = 1;
  select.name = 'msg.to';
  select.id = 'msg.to';
  var option = document.createElement("option");
  option.value = '';
  option.appendChild(document.createTextNode('manual'));
  select.appendChild(option);
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

function showBuddyList() {
  var msgTO = document.getElementById('msg.to');
  if (msgTO) {
    var pNode = msgTO.parentNode;
    if (msgTO.nodeName == 'INPUT') {
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

function filterMessages() {
  var filter = this.value;
  var type = this.parentNode.className.substring(this.parentNode.className.indexOf("filter_")+7,this.parentNode.className.length);
  var div = document.getElementById('layout-content');
  var rowList = msgTable.tBodies[0].getElementsByTagName('TR');
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
    if (cell.nodeName != 'TD') continue;
    if (filter == 'all') { row.style.display = ''; continue; }
    if (cell.firstChild.nodeValue != filter) row.style.display = 'none';
    else row.style.display = '';
  }
}

function createReply() {
  var quoted = false;
  if (this.name == 'msg.replyq') quoted = true;
  // fetched required data
  var divA = getElementsByClassName(document.getElementsByTagName('DIV'),'g_section message entity',true)[0];
  divA.className = divA.className.replace('entity','form');
  var inputs =  divA.getElementsByTagName('INPUT');
  var msgDO = getElementsByName(inputs,'do',true)[0];
  msgDO.value = 'send'; // value needs to change
  var msgMID = getElementsByName(inputs,'mid',true)[0];
  msgMID.parentNode.removeChild(msgMID); // got to remove this
  var msgTO = getElementsByName(inputs,'msg.to',true)[0];
  msgTO.type = "text";
  msgTO.id = msgTO.name;
  msgTO.size = "20";
  msgTO.maxlength = "16";
  var msgTITLE = getElementsByName(inputs,'msg.title',true)[0];
  msgTITLE.id = msgTITLE.name;
  msgTITLE.type = "text";
  msgTITLE.size = "80";
  msgTITLE.maxlength = "50";
  var msgBODYval = getElementsByName(inputs,'msg.body',true)[0];
  var msgBODY = document.createElement('TEXTAREA');
  msgBODY.id = msgBODYval.name;
  msgBODY.name = msgBODY.id;
  if (quoted) {
    var quote = '[quote="'+msgTO.value+'"]';
    quote += convert_input(msgBODYval.value);
    quote += '[/quote]';
    msgBODY.appendChild(document.createTextNode(quote));
  }
  msgBODYval.parentNode.removeChild(msgBODYval);
  var div = getElementsByClassName(document.getElementsByTagName('DIV'),'g_definitionlist message',true)[0];
  var table = div.getElementsByTagName('TABLE')[0];
  while (table.caption.childNodes.length) table.caption.removeChild(table.caption.childNodes[0]);
  table.caption.appendChild(document.createTextNode("New Message"));
  while (table.rows.length) table.tBodies[0].removeChild(table.rows[0]); // remove old rows
  var row = document.createElement('TR');
  var th = document.createElement('TH');
  th.className = "field";
  row.appendChild(th);
  var td = document.createElement('TD');
  td.className = "value";
  row.appendChild(td);
  var TO = row.cloneNode(true);
  TO.className = "g_odd to";
  TO.cells[0].appendChild(document.createTextNode('To'));
  TO.cells[1].appendChild(msgTO);
  input_msgTO = msgTO.cloneNode(true);
  var curItem = {'id':"Buddy",'desc':"Buddy list",'text':"Buddy",'onclick':showBuddyList,'active':false};
  var buddyButton = document.createElement('INPUT');
  buddyButton = createButton(null,'buddy',curItem);
  TO.cells[1].appendChild(buddyButton);
  var i = document.createElement('I');
  i.appendChild(document.createTextNode('(username or uid)'));
  TO.cells[1].appendChild(document.createTextNode(' '));
  TO.cells[1].appendChild(i);
  table.tBodies[0].appendChild(TO);
  var TITLE = row.cloneNode(true);
  TITLE.className = "title";
  TITLE.cells[0].appendChild(document.createTextNode('Title'));
  TITLE.cells[1].appendChild(msgTITLE);
  table.tBodies[0].appendChild(TITLE);
  var BODY = row.cloneNode(true);
  BODY.className = "g_odd body";
  BODY.cells[0].appendChild(document.createTextNode('Body'));
  BODY.cells[1].appendChild(msgBODY);
  table.tBodies[0].appendChild(BODY);
  var row = document.createElement('TR');
  row.className = 'action';
  td = document.createElement('TD');
  td.className = "value";
  td.colSpan = "2";
  var send = document.createElement('INPUT');
  send.type = "submit";
  send.id = "msg.send";
  send.name = send.id;
  send.value = "Send Message";
  td.appendChild(send);
  row.appendChild(td);
  table.tBodies[0].appendChild(row);
  if (init_formating) init_formating(); // should be loaded but you never know
}

function updateMsgList() {
  var div = getElementsByClassName(document.getElementsByTagName('DIV'), 'message list', true)[0];
  if (!div) return;
  msgTable = div.getElementsByTagName('TABLE')[0];
  if (!msgTable) return;
  headingList = msgTable.getElementsByTagName('TH');
  var actionList = getElementsByClassName(document.getElementsByTagName('UL'), 'g_actionlist', true)[0];
  // I know the headings i need so..
  headingList[0].className += ' c_latin'; // State
  headingList[1].className += ' c_datelong';  // Date
  headingList[2].className += ' c_latin'; // From
  headingList[3].className += ' c_latin'; // type
  headingList[4].className += ' c_latin'; // Title
  headingList[5].className += ' c_none';  // Action
  
  // append some extra filtering options
  // FILTER BY TYPE
  var select = document.createElement('SELECT');
  select.onchange = filterMessages;
  createSelectOption(select,'all','all',true);
  createSelectOption(select,'normal','normal',false);
  createSelectOption(select,'system','system',false);
  createSelectOption(select,'bulk','bulk',false);
  createSelectOption(select,'mod','mod',false);
  var li = document.createElement('LI');
  li.appendChild(document.createTextNode('type: '));
  li.appendChild(select);
  li.appendChild(document.createTextNode(' - '));
  li.className = 'filter_type';
  actionList.insertBefore(li,actionList.firstChild);
  // FILTER BY STATE
  select = document.createElement('SELECT');
  select.onchange = filterMessages;
  createSelectOption(select,'all','all',true);
  createSelectOption(select,'new','new',false);
  createSelectOption(select,'old','old',false);
  li = document.createElement('LI');
  li.appendChild(document.createTextNode('state: '));
  li.appendChild(select);
  li.appendChild(document.createTextNode(' - '));
  li.className = 'filter_state';
  actionList.insertBefore(li,actionList.firstChild);
  
  // apply the sorting function
  init_sorting(msgTable,'date'); 
}

function prepPage() {
  uriObj = parseURI(); // update the uriObj
  if (!uriObj['show'] || uriObj['show'] != 'msg') return; // not interested in this page
  if (!uriObj['do']) updateMsgList(); // message list
  if (uriObj['do']) {
    if (uriObj['do'] == 'new') {
      // if we are on the message send page (which is where we should be)
      var msgTO = document.getElementById('msg.to');
      if (msgTO) {
        input_msgTO = msgTO.cloneNode(true);
        var curItem = {'id':"Buddy",'desc':"Buddy list",'text':"Buddy",'onclick':showBuddyList,'active':false};
        var buddyButton = document.createElement('INPUT');
        buddyButton = createButton(null,'buddy',curItem);
        msgTO.parentNode.insertBefore(buddyButton,msgTO.nextSibling);
      }
    } else if (uriObj['do'] == 'show') {
      // search for some stuff
      var inputs = document.getElementsByTagName('INPUT');
      var reply = getElementsByName(inputs,'msg.reply',true)[0];
      var replyq = getElementsByName(inputs,'msg.replyq',true)[0];
      if (!reply || !replyq) return;
      reply.type = 'button';
      replyq.type = 'button';
      reply.onclick = createReply;
      replyq.onclick = createReply;
    }
  }
}

//window.onload = prepPage;
addLoadEvent(prepPage);
