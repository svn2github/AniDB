/* *
 * @file fileadd page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.0 (17.05.2007)
 */

// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var eid;                       // Episode Id
var searchString = "";
var replaceCell = null;
var typeSelect = null;
var table = null;
var targetRow;
var vidstrm, audstrm, substrm;
var qualSelect, sourceSelect;
var input_ed2k, input_crc32, input_fileVersion, input_size, searchbox;
var vidTracks = 1;
var maxVidTracks = 1;
var audTracks = 1;
var maxAudTracks = 3;
var subTracks = 1;
var maxSubTracks = 3;
var currentView;
var simpleView = true;
var isEditPage = false;
var seeDebug = false;
var LAY_FORMATFILESIZE = false;
var templateType = 'manual';

// FUNCTIONS

function fetchData() {
  var req = xhttpRequest();
  if (''+window.location.hostname == '') xhttpRequestFetch(req, 'xml/groupsearch.xml', parseData);
  else xhttpRequestFetch(req, 'animedb.pl?show=xml&t=groupsearch&search='+escape(searchString), parseData);
}

function CGroup(node) {
  this.id = Number(node.getAttribute('gid'));
  this.name = node.getAttribute('name');
  this.sname = node.getAttribute('sname');
}

function parseData(xmldoc) {
  var root = xmldoc.getElementsByTagName('root').item(0);
  var t1 = new Date();
  var groupEntries = root.getElementsByTagName('group');
  var select = document.createElement('SELECT');
  select.size = 1;
  select.name = 'addf.group';
  var option = document.createElement("option");
  option.value = '-1';
  option.appendChild(document.createTextNode('ignore'));
  select.appendChild(option);
  option = document.createElement("option");
  option.value = '0';
  option.appendChild(document.createTextNode('no or unknown group'));
  select.appendChild(option);
  for (var i = 0; i < groupEntries.length; i++) {
    var groupEntry = new CGroup(groupEntries[i]);
    option = document.createElement("option");
    option.value = groupEntry.id;
    option.appendChild(document.createTextNode('['+groupEntry.sname+'] '+groupEntry.name));
    select.appendChild(option);
  }
  while (replaceCell.childNodes.length)
    replaceCell.removeChild(replaceCell.childNodes[0]);
  replaceCell.appendChild(select);
}

function createFileAddSelect(source,name,id) {
	var select = source.cloneNode(true);
	if (name) select.name = name;
	if (id) select.id = id;
	return select;
}


function vidStreams() {
  this.numStreams = getElementsByName(document.getElementsByTagName('INPUT'),'addf.vid.streams',true)[0];
  this.streams = new Array();
  this.streams[0] = new Object();
  this.streams[0].toggle = getElementsByName(document.getElementsByTagName('INPUT'),'addf.vid.none',true)[0];
  this.streams[0].width = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.vid.reswidth',true)[0];
  this.streams[0].height = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.vid.resheight',true)[0];
  this.streams[0].ar = getElementsByName(document.getElementsByTagName('SELECT'),'addstrm.vid.ar',true)[0];
  this.streams[0].codec = getElementsByName(document.getElementsByTagName('SELECT'),'addstrm.vid.vcodecid',true)[0];
  this.streams[0].bitrate = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.vid.bitrate',true)[0];
  this.streams[0].fps = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.vid.fps',true)[0];
  this.streams[0].anamorphic = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.vid.flag.FLAG_ANAMORPHIC',true)[0];
  this.streams[0].clean = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.vid.flag.FLAG_CLEAN',true)[0];
  this.streams[0].vfr = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.vid.flag.FLAG_VFR',true)[0];
  this.streams[0].wrongar = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.vid.flag.FLAG_WRONGAR',true)[0];
}
vidStreams.prototype.disable = function(val, id) {
  if (id == null) id = 0;
  this.streams[id].toggle.checked = val;
  this.streams[id].width.disabled = val;
  this.streams[id].height.disabled = val;
  this.streams[id].ar.disabled = val;
  this.streams[id].codec.disabled = val;
  this.streams[id].bitrate.disabled = val;
  this.streams[id].fps.disabled = val;
  this.streams[id].anamorphic.disabled = val;
  this.streams[id].clean.disabled = val;
  this.streams[id].vfr.disabled = val;
  this.streams[id].wrongar.disabled = val;
}

function audStreams() {
  this.numStreams = getElementsByName(document.getElementsByTagName('INPUT'),'addf.aud.streams',true)[0];
  this.streams = new Array();
  this.streams[0] = new Object();
  this.streams[0].toggle = getElementsByName(document.getElementsByTagName('INPUT'),'addf.aud.none',true)[0];
  this.streams[0].type = getElementsByName(document.getElementsByTagName('SELECT'),'addstrm.aud.type',true)[0];
  this.streams[0].lang = getElementsByName(document.getElementsByTagName('SELECT'),'addstrm.aud.lang',true)[0];
  this.streams[0].codec = getElementsByName(document.getElementsByTagName('SELECT'),'addstrm.aud.acodecid',true)[0];
  this.streams[0].bitrate = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.aud.bitrate',true)[0];
  this.streams[0].channels = getElementsByName(document.getElementsByTagName('SELECT'),'addstrm.aud.chantype',true)[0];
}
audStreams.prototype.disable = function(val, id) {
  if (id == null) id = 0;
  this.streams[id].toggle.checked = val;
  this.streams[id].type.disabled = val;
  this.streams[id].lang.disabled = val;
  this.streams[id].codec.disabled = val;
  this.streams[id].bitrate.disabled = val;
  this.streams[id].channels.disabled = val;
}
audStreams.prototype.add = function() {
  audTracks++;
  if (audTracks > maxAudTracks) return;
  this.numStreams.value = audTracks;
  var str = audTracks - 1;
  this.streams[str] = new Object();
  var lastRow = this.streams[0].channels.parentNode.parentNode;
  var index = lastRow.rowIndex + 1;
  var tbody = table.tBodies[0];
  var row = document.createElement('TR');
  row.className = "subheader";
  var cell = document.createElement('TD');
  cell.colSpan = 2;
  cell.appendChild(document.createTextNode('Audio Track Info [stream: '+audTracks+']'));
  row.appendChild(cell);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
  // track toogle
  row = document.createElement('TR'); 
  c0 = document.createElement('TD'); c0.appendChild(document.createTextNode('Track Toggle:')); row.appendChild(c0);
  c1 = document.createElement('TD');
  var checkbox = createCheckbox('addf.aud_'+audTracks+'.none');
  this.streams[str].toggle = checkbox;
  c1.appendChild(checkbox);
  c1.appendChild(document.createTextNode(' no audio track'));
  // some extra stuff only needed with more than 1 stream
  var streamNo = createTextInput('addstrm.aud_'+audTracks+'.streamno',6,false,true);
  streamNo.value = audTracks;
  this.streams[str].streamNo = streamNo;
  row.appendChild(streamNo);
  var streamId = createTextInput('addstrm.aud_'+audTracks+'.streamid',6,false,true);
  streamId.value = -1;
  this.streams[str].streamId = streamId;
  row.appendChild(streamId);
  var streamOther = createTextInput('addstrm.aud_'+audTracks+'.other',6,false,true);
  streamOther.value = "";
  this.streams[str].streamOther = streamOther;
  row.appendChild(streamOther);
  row.appendChild(c1);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
  // type
  row = document.createElement('TR'); 
  c0 = document.createElement('TD'); c0.appendChild(document.createTextNode('Type:')); row.appendChild(c0);
  c1 = document.createElement('TD');
  var select = createFileAddSelect(this.streams[0].type,'addstrm.aud_'+audTracks+'.type');
  this.streams[str].type = select;
  c1.appendChild(select);
  row.appendChild(c1);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
  // language
  row = document.createElement('TR'); 
  c0 = document.createElement('TD'); c0.appendChild(document.createTextNode('Language:')); row.appendChild(c0);
  c1 = document.createElement('TD');
  select = createFileAddSelect(this.streams[0].lang,'addstrm.aud_'+audTracks+'.lang','addfilem.aud_'+audTracks+'.lang');
  this.streams[str].lang = select;
  c1.appendChild(select);
  row.appendChild(c1);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
  // Codec/Bitrate:
  row = document.createElement('TR');
  if (simpleView) row.style.display = 'none';
  c0 = document.createElement('TD'); c0.appendChild(document.createTextNode('Codec/Bitrate:')); row.appendChild(c0);
  c1 = document.createElement('TD');
  select = createFileAddSelect(this.streams[0].codec,'addstrm.aud_'+audTracks+'.acodecid');
  this.streams[str].codec = select;
  c1.appendChild(select);
  c1.appendChild(document.createTextNode(' '));
  var input = createTextInput('addstrm.aud_'+audTracks+'.bitrate',6);
  this.streams[str].bitrate = input;
  c1.appendChild(input);
  c1.appendChild(document.createTextNode(' kBit/s'));
  row.appendChild(c1);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
  // Channels:
  row = document.createElement('TR');
  if (simpleView) row.style.display = 'none';
  c0 = document.createElement('TD'); c0.appendChild(document.createTextNode('Channels:')); row.appendChild(c0);
  c1 = document.createElement('TD');
  select = createFileAddSelect(this.streams[0].channels,'addstrm.aud_'+audTracks+'.chantype');
  this.streams[str].channels = select;
  c1.appendChild(select);
  row.appendChild(c1);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
}

audStreams.prototype.remove = function(id) {
  var str;
  var numRows = 6;
  audTracks--;
  if (id != null) str = id;
  else {
    if (audTracks < 1) return;
    str = audTracks - 1;
  }
  this.numStreams.value = audTracks;
  var firstRow = this.streams[str].toggle.parentNode.parentNode;
  var index = firstRow.rowIndex;
  var tbody = table.tBodies[0];
  for (var i = 0; i < numRows; i++) tbody.removeChild(tbody.rows[index-1]);
  this.streams.splice(str,1);
}

function subStreams() {
  this.numStreams = getElementsByName(document.getElementsByTagName('INPUT'),'addf.sub.streams',true)[0];
  this.streams = new Array();
  this.streams[0] = new Object();
  this.streams[0].toggle = getElementsByName(document.getElementsByTagName('INPUT'),'addf.sub.none',true)[0];
  this.streams[0].type = getElementsByName(document.getElementsByTagName('SELECT'),'addstrm.sub.type',true)[0];
  this.streams[0].lang = getElementsByName(document.getElementsByTagName('SELECT'),'addstrm.sub.lang',true)[0];
  this.streams[0].dubsubbed = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.sub.flag.FLAG_DUBSUBBED',true)[0];
  this.streams[0].hearingimp = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.sub.flag.FLAG_HEARINGIMP',true)[0];
  this.streams[0].image = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.sub.flag.FLAG_IMAGE',true)[0];
  this.streams[0].styled = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.sub.flag.FLAG_STYLED',true)[0];
  this.streams[0].forcommentary = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.sub.flag.FLAG_FORCOMMENTARY',true)[0];
  this.streams[0].unstyled = getElementsByName(document.getElementsByTagName('INPUT'),'addstrm.sub.flag.FLAG_UNSTYLED',true)[0];
}
subStreams.prototype.disable = function(val, id) {
  if (id == null) id = 0;
  this.streams[id].toggle.checked = val;
  this.streams[id].type.disabled = val;
  this.streams[id].lang.disabled = val;
  this.streams[id].dubsubbed.disabled = val;
  this.streams[id].hearingimp.disabled = val;
  this.streams[id].image.disabled = val;
  this.streams[id].styled.disabled = val;
  this.streams[id].forcommentary.disabled = val;
  this.streams[id].unstyled.disabled = val;
}
subStreams.prototype.add = function() {
  subTracks++;
  if (subTracks > maxSubTracks) return;
  var str = subTracks - 1;
  this.streams[str] = new Object();
  var lastRow = this.streams[0].channels.parentNode.parentNode;
  var index = lastRow.rowIndex + 1;
  var tbody = table.tBodies[0];
  var row = document.createElement('TR');
  row.className = "subheader";
  var cell = document.createElement('TD');
  cell.colSpan = 2;
  cell.appendChild(document.createTextNode('Subtitle Track Info [stream: '+subTracks+']'));
  row.appendChild(cell);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
  // track toogle
  row = document.createElement('TR'); 
  c0 = document.createElement('TD'); c0.appendChild(document.createTextNode('Track Toggle:')); row.appendChild(c0);
  c1 = document.createElement('TD');
  var checkbox = createCheckbox('addf.sub_'+subTracks+'.none');
  this.streams[str].toggle = checkbox;
  c1.appendChild(checkbox);
  // some extra stuff only needed with more than 1 stream
  var streamNo = createTextInput('addstrm.sub_'+subTracks+'.streamno',6,false,true);
  streamNo.value = subTracks;
  this.streams[str].streamNo = streamNo;
  row.appendChild(streamNo);
  var streamId = createTextInput('addstrm.sub_'+subTracks+'.streamid',6,false,true);
  streamId.value = -1;
  this.streams[str].streamId = streamId;
  row.appendChild(streamId);
  var streamOther = createTextInput('addstrm.sub_'+subTracks+'.other',6,false,true);
  streamOther.value = "";
  this.streams[str].streamOther = streamOther;
  row.appendChild(streamOther);
  c1.appendChild(document.createTextNode(' no subtitle track'));
  row.appendChild(c1);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
  // type
  row = document.createElement('TR'); 
  c0 = document.createElement('TD'); c0.appendChild(document.createTextNode('Type:')); row.appendChild(c0);
  c1 = document.createElement('TD');
  select = createFileAddSelect(this.streams[0].type,'addstrm.sub_'+subTracks+'.type');
  this.streams[str].type = select;
  c1.appendChild(select);
  row.appendChild(c1);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
  // language
  row = document.createElement('TR'); 
  c0 = document.createElement('TD'); c0.appendChild(document.createTextNode('Language:')); row.appendChild(c0);
  c1 = document.createElement('TD');
  select = createFileAddSelect(this.streams[0].lang,'addstrm.sub_'+subTracks+'.lang','addfilem.sub_'+subTracks+'.lang');
  this.streams[str].lang = select;
  c1.appendChild(select);
  row.appendChild(c1);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
  // Flags:
  row = document.createElement('TR'); 
  c0 = document.createElement('TD'); c0.appendChild(document.createTextNode('Flags:')); row.appendChild(c0);
  c1 = document.createElement('TD');
  var span = document.createElement('SPAN');
  checkbox = createCheckbox('addstrm.sub_'+subTracks+'.flag.FLAG_DUBSUBBED');
  this.streams[str].dubsubbed = checkbox;
  span.appendChild(checkbox);
  span.appendChild(document.createTextNode(' dubsubbed'));
  span.appendChild(document.createElement('BR'));
  c1.appendChild(span);
  span = document.createElement('SPAN');
  checkbox = createCheckbox('addstrm.sub_'+subTracks+'.flag.FLAG_HEARINGIMP');
  this.streams[str].hearingimp = checkbox;
  span.appendChild(checkbox);
  span.appendChild(document.createTextNode(' hearing impaired subs'));
  span.appendChild(document.createElement('BR'));
  c1.appendChild(span);
  span = document.createElement('SPAN');
  checkbox = createCheckbox('addstrm.sub_'+subTracks+'.flag.FLAG_IMAGE');
  this.streams[str].image = checkbox;
  span.appendChild(checkbox);
  span.appendChild(document.createTextNode(' image subs (VOBSUB)'));
  span.appendChild(document.createElement('BR'));
  c1.appendChild(span);
  span = document.createElement('SPAN');
  checkbox = createCheckbox('addstrm.sub_'+subTracks+'.flag.FLAG_STYLED');
  this.streams[str].styled = checkbox;
  span.appendChild(checkbox);
  span.appendChild(document.createTextNode(' styled subs (ASS/SSA)'));
  span.appendChild(document.createElement('BR'));
  c1.appendChild(span);
  span = document.createElement('SPAN');
  checkbox = createCheckbox('addstrm.sub_'+subTracks+'.flag.FLAG_FORCOMMENTARY');
  this.streams[str].forcommentary = checkbox;
  span.appendChild(checkbox);
  span.appendChild(document.createTextNode(' subs for commentary audio stream'));
  span.appendChild(document.createElement('BR'));
  c1.appendChild(span);
  span = document.createElement('SPAN');
  checkbox = createCheckbox('addstrm.sub_'+subTracks+'.flag.FLAG_UNSTYLED');
  this.streams[str].unstyled = checkbox;
  span.appendChild(checkbox);
  span.appendChild(document.createTextNode(' unstyled subs (SRT)'));
  c1.appendChild(span);
  row.appendChild(c1);
  tbody.insertBefore(row,tbody.rows[index]);
  index++;
}
subStreams.prototype.remove = function(id) {
  var str;
  var numRows = 5;
  subTracks--;
  if (id != null) str = id;
  else {
    if (subTracks < 1) return;
    str = subTracks - 1;
  }
  var firstRow = this.streams[str].toggle.parentNode.parentNode;
  var index = firstRow.rowIndex;
  var tbody = table.tBodies[0];
  for (var i = 0; i < numRows; i++) tbody.removeChild(tbody.rows[index-1]);
  this.streams.splice(str,1);
}
function CView() {
  var row = document.createElement('TR');
  var cell = document.createElement('TD');
  cell.colSpan = 2;
  var a = document.createElement('A');
  a.onclick = changeView;
  if (simpleView) a.appendChild(document.createTextNode('[switch to full view]'));
  else a.appendChild(document.createTextNode('[switch to simple view]'));
  cell.appendChild(a);
  row.appendChild(cell);
  table.tBodies[0].insertBefore(row,table.tBodies[0].rows[0]);
  var test = getElementsByName(document.getElementsByTagName('INPUT'),'addf.md5',true)[0];
  if (test) this.md5 = test.parentNode.parentNode;
  test = getElementsByName(document.getElementsByTagName('INPUT'),'addf.sha1',true)[0];
  if (test) this.sha1 = test.parentNode.parentNode;
  test = getElementsByName(document.getElementsByTagName('INPUT'),'addf.size',true)[0];
  if (test) this.size = test.parentNode.parentNode;
  test = getElementsByName(document.getElementsByTagName('INPUT'),'addf.length',true)[0];
  if (test) this.len = test.parentNode.parentNode;
  test = getElementsByName(document.getElementsByTagName('SELECT'),'addf.filetype',true)[0];
  if (test) this.filetype = test.parentNode.parentNode;
  test = getElementsByName(document.getElementsByTagName('SELECT'),'addf.fileversion',true)[0];
  if (test) this.fileversion = test.parentNode.parentNode;
  this.type = typeSelect.parentNode.parentNode;
  if (!isEditPage) {
    this.vid_resolution = vidstrm.streams[0].height.parentNode.parentNode;
    this.vid_codec = vidstrm.streams[0].codec.parentNode.parentNode;
  }
}

CView.prototype.simpleView = function() {
  simpleView = true;
  for (var e in this) {
    if (this[e] && this[e].style) this[e].style.display = "none";
  }
  if (!isEditPage) {
    for (var i in audstrm.streams) {
      var stream = audstrm.streams[i];
      if (!stream) continue;
      if (stream.codec) stream.codec.parentNode.parentNode.style.display = "none";
      if (stream.channels) stream.channels.parentNode.parentNode.style.display = "none";
    }
    for (var i in substrm.streams) {
      var stream = substrm.streams[i];
      if (!stream) continue;
      if (stream.image) stream.image.parentNode.style.display = "none";
      if (stream.styled) stream.styled.parentNode.style.display = "none";
      if (stream.unstyled) stream.unstyled.parentNode.style.display = "none";
    }
  }
}
CView.prototype.fullView = function() {
  simpleView = false;
  for (var e in this) {
    if (this[e] && this[e].style) this[e].style.display = "";
  }
  if (!isEditPage) {
    for (var i in audstrm.streams) {
      var stream = audstrm.streams[i];
      if (!stream) continue;
      if (stream.codec) stream.codec.parentNode.parentNode.style.display = "";
      if (stream.channels) stream.channels.parentNode.parentNode.style.display = "";
    }
    for (var i in substrm.streams) {
      var stream = substrm.streams[i];
      if (!stream) continue;
      if (stream.image) stream.image.parentNode.style.display = "";
      if (stream.styled) stream.styled.parentNode.style.display = "";
      if (stream.unstyled) stream.unstyled.parentNode.style.display = "";
    }
  }
}
function changeView() {
  if (!currentView) return;
  while (this.childNodes.length) this.removeChild(this.childNodes[0]);
  if (simpleView) { this.appendChild(document.createTextNode('[switch to simple view]')); currentView.fullView(); }
  else { this.appendChild(document.createTextNode('[switch to full view]')); currentView.simpleView(); }
}

/** ed2k link parsing functions by epoximator **/

function MyFile(name, size, ed2k){
	this.find_crc = function(text){
		var regx = /[\[\(]([a-f0-9]{8})[\]\)]/i;
		if(regx.test(text))
			return (RegExp.$1).toLowerCase();
    else return('');
	}
	this.find_version = function(text){
		var regx = /(\d)v(\d)/;
		if(regx.test(text))
			return (RegExp.$2)*1
    else return(1);
	}
  this.find_filetype = function(text){
		var regx = /\.([a-z0-9]{2,4})$/i;
		if(regx.test(text))
			return RegExp.$1.toLowerCase();
	}
  this.find_group = function(text){
    var regx = /(\[|\{)(.+?)(\]|\})(\[|\(|\.|_).+$/i;
    if(regx.test(text))
			return (RegExp.$2)
    else return('');
  }
	this.crc = this.find_crc(name);
	this.version = this.find_version(name);
  this.filetype = this.find_filetype(name);
  this.group = this.find_group(name);
  this.size = size;
}

function parseEd2k() {
  var re_ed2k_link = /ed2k\:\/\/\|file\|(.+)\|(\d+)\|([a-f0-9]{32})\|/i;
  var file = null;
  if(re_ed2k_link.test(this.value)) {
		file = new MyFile(RegExp.$1, RegExp.$2, RegExp.$3);
  } else return;
  // we have a file let's update some fields
  input_crc32.value = file.crc;
  input_fileVersion.value = file.version;
  input_fileExtension.value = file.filetype;
  input_size.value = formatFileSize(file.size);
  searchbox.value = file.group;
  workEd2k(file);
}

function workEd2k(file) {
  // Stuff that i'll possibly edit
  if (templateType == 'dub' || templateType == 'sub') vidstrm.disable(true); else vidstrm.disable(false);
  if (templateType == 'sub') audstrm.disable(true); else audstrm.disable(false);
  if (templateType == 'raw' || templateType == 'dub') substrm.disable(true); else substrm.disable(false);
  switch (file.filetype) {
    case 'srt':
      vidstrm.disable(true,0);
      audstrm.disable(true,0);
      if (audstrm.streams[1]) audstrm.remove(1);
      typeSelect.value = 20;
      substrm.streams[0].type.value = 20;
      substrm.streams[0].image.checked = false;
      substrm.streams[0].styled.checked = false;
      substrm.streams[0].unstyled.checked = true;
      break;
    case 'ssa':
    case 'ass':
      vidstrm.disable(true,0);
      audstrm.disable(true,0);
      if (audstrm.streams[1]) audstrm.remove(1);
      typeSelect.value = 20;
      substrm.streams[0].type.value = 20;
      substrm.streams[0].image.checked = false;
      substrm.streams[0].styled.checked = true;
      substrm.streams[0].unstyled.checked = false;
      break;
    default:
      break;
  }
}

function workTemplate() {
  // Stuff that i'll possibly edit
  qualSelect.value = 3;
  sourceSelect.value = 5;
  typeSelect.value = 10;
  vidstrm.disable(false);
  audstrm.disable(false);
  substrm.disable(false);
  templateType = this.value;
  //typeSelect.disabled = true;
  switch (this.value) {
    case 'manual':
      typeSelect.disabled = false;
      break;
    case 'raw':
      substrm.disable(true,0);
      audstrm.streams[0].lang.value = 2;
      if (audstrm.streams[1]) audstrm.remove(1);
      break;
    case 'fansub':
      audstrm.streams[0].lang.value = 2;
      if (audstrm.streams[1]) audstrm.remove(1);
      break;
    case 'dual':
      qualSelect.value = 2;
      sourceSelect.value = 7;
      audstrm.streams[0].lang.value = 2;
      substrm.streams[0].lang.value = 2;
      substrm.streams[0].type.value = 20;
      if (!audstrm.streams[1]) { 
        audstrm.add();
        audstrm.streams[1].toggle.onclick = function () { audstrm.remove(1); }
        audstrm.streams[1].lang.value = 4;
        audstrm.streams[1].type.value = 10;
      } else {
        audstrm.disable(false,1);
      }
      break;
    case 'dub':
      vidstrm.disable(true,0);
      substrm.disable(true,0);
      if (audstrm.streams[1]) audstrm.remove(1);
      typeSelect.value = 30;
      break;
    case 'sub':
      vidstrm.disable(true,0);
      audstrm.disable(true,0);
      if (audstrm.streams[1]) audstrm.remove(1);
      typeSelect.value = 20;
      break;
    case 'other':
      vidstrm.disable(true,0);
      typeSelect.value = 100;
      qualSelect.value = 1;
      if (audstrm.streams[1]) audstrm.remove(1);
      sourceSelect.value = 1;
      break;
    default: return;
  }
}

function addTemplate() {
  var row = document.createElement('TR');
  createCell(row, null, document.createTextNode('Template:'), null);
  var optionArray = {"manual":{"text":'manual input'},"raw":{"text":'raw (japanese audio, no subtitles)'},
					 "fansub":{"text":'fansub (japanese audio, ? subtitles)'},"dual":{"text":'dual (japanese audio, ? audio, ? subtitles)'},
					 "dub":{"text":'external dub file (? audio)'},"sub":{"text":'external sub file (? subtitles)'},"other":{"text":'other'}};
  var select = createSelectArray(null,null,null,null,null,optionArray);
  select.onchange = workTemplate;
  createCell(row, null, select, null);
  table.tBodies[0].insertBefore(row,targetRow);
  vidstrm = new vidStreams();
  audstrm = new audStreams();
  substrm = new subStreams();
  vidstrm.streams[0].toggle.onclick = function () { vidstrm.disable(vidstrm.streams[0].toggle.checked, 0); }
  audstrm.streams[0].toggle.onclick = function () { audstrm.disable(audstrm.streams[0].toggle.checked, 0); }
  substrm.streams[0].toggle.onclick = function () { substrm.disable(substrm.streams[0].toggle.checked, 0); }
  qualSelect = getElementsByName(document.getElementsByTagName('SELECT'),'addf.quality',true)[0];
  sourceSelect = getElementsByName(document.getElementsByTagName('SELECT'),'addf.type',true)[0];

}

function updateSize() {
	if (this.value.indexOf(' ') < -1) return;
	var replace = new RegExp(" +","mgi");
	this.value = this.value.replace(replace,'.');
}

function updateDates() {
	if (this.value.indexOf('/') < 0 && this.value.indexOf('-') < 0) return;
	var replace = new RegExp("\/+","mgi");
	this.value = this.value.replace(replace,'.');
	replace = new RegExp("\-+","mgi");
	this.value = this.value.replace(replace,'.');

}

function updateSearchString() { 
	while (replaceCell.childNodes.length)
    replaceCell.removeChild(replaceCell.childNodes[0]);
  replaceCell.appendChild(document.createTextNode('please wait while searching...'));
  searchString = searchbox.value;
  fetchData(); 
}

function prepPage() {
  uriObj = parseURI(); // update the uriObj
  if (uriObj['eid'] && uriObj['show'] && uriObj['show'].indexOf('addfile') >= 0) eid = uriObj['eid'];
  else return; // badPage
  if (uriObj['edit'] && uriObj['edit'] != 0) { isEditPage = true; simpleView = false; }
  // find the result cell
  var input = getElementsByName(document.getElementsByTagName('INPUT'),'addf.do.searchgroup',true)[0];
  searchbox = getElementsByName(document.getElementsByTagName('INPUT'),'addf.groupsearch',true)[0];
  if (!input || !searchbox) return;
  var cellId = input.parentNode.cellIndex;
  var prow = input.parentNode.parentNode;
  var rowId = prow.rowIndex;
  var nrow = prow.parentNode.rows[rowId+1];
  replaceCell = nrow.cells[cellId];
  if (!replaceCell) errorAlert('prepPage','noReplaceCell');
  var newinput = createBasicButton('addf.do.searchgroup',' Search ');
  newinput.onclick = updateSearchString;
  input.parentNode.replaceChild(newinput,input);
  // Stuff for the template part
  table = getElementsByClassName(document.getElementsByTagName('TABLE'),'file_add',true)[0];
  if (!table) return;
  targetRow = table.tBodies[0].rows[8];
  var cell0 = targetRow.cells[0];
  var cell1 = targetRow.cells[1];
  typeSelect = cell1.getElementsByTagName('SELECT')[0]; // The file type select
  targetRow = table.tBodies[0].rows[1];
  if (!isEditPage) {
    vidstrm = new vidStreams();
    audstrm = new audStreams();
    substrm = new subStreams();
    vidstrm.streams[0].toggle.onclick = function () { vidstrm.disable(vidstrm.streams[0].toggle.checked, 0); }
    audstrm.streams[0].toggle.onclick = function () { audstrm.disable(audstrm.streams[0].toggle.checked, 0); }
    substrm.streams[0].toggle.onclick = function () { substrm.disable(substrm.streams[0].toggle.checked, 0); }
  }
  qualSelect = getElementsByName(document.getElementsByTagName('SELECT'),'addf.quality',true)[0];
  sourceSelect = getElementsByName(document.getElementsByTagName('SELECT'),'addf.type',true)[0];
  if (!typeSelect.disabled && !isEditPage) addTemplate(); // if file is verified don't do more stuff
  currentView = new CView();
  if (!isEditPage) currentView.simpleView();
  else currentView.fullView();
  input_ed2k = getElementsByName(document.getElementsByTagName('INPUT'),'addf.ed2k',true)[0];
  input_ed2k.onchange = parseEd2k;
  input_crc32 = getElementsByName(document.getElementsByTagName('INPUT'),'addf.crc',true)[0];
  input_fileVersion = getElementsByName(document.getElementsByTagName('SELECT'),'addf.fileversion',true)[0];
  input_size = getElementsByName(document.getElementsByTagName('INPUT'),'addf.size',true)[0];
  input_size.onchange = updateSize;
  input_fileExtension = getElementsByName(document.getElementsByTagName('SELECT'),'addf.filetype',true)[0];
  getElementsByName(document.getElementsByTagName('INPUT'),'addf.released',true)[0].onchange = updateDates;
}


//window.onload = prepPage;
addLoadEvent(prepPage);
