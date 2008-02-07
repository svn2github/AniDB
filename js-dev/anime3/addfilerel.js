/**
 * @file add file relation page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)
 *         
 * @version 1.0 (03.06.2007)
 */

// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var seeDebug = false;
var relType, relFile;
var fileType;
var fileList;
var relList;

function filterFileList(type) {
  while (relFile.childNodes.length) relFile.removeChild(relFile.childNodes[0]);
  for (var i = 0; i < fileList.options.length; i++) {
    var option = fileList.options[i].cloneNode(true);
    // Let me just state this once more, i hate IE...
    if (!option.text.length && option.innerText) option.text = option.innerText; // IE hack
    if (type == 'all' || option.value == -1) {
      relFile.appendChild(option);
    } else {
      var ftype = option.text.substr(option.text.indexOf('TYPE')+5,option.text.length);
      if (ftype.indexOf(type) >= 0) relFile.appendChild(option); 
    }
  }
}

function filterFilesByRelationType() {
  var type = 'all';
  switch (this.value) {
    case  '10': 
    case  '40':
    case  '50': type = 'video'; break;
    case  '20': type = fileType; break;
    default   : type = 'all';
  }
  filterFileList(type);
}

function cleanFilterRelations() {
  while (relType.childNodes.length) relType.removeChild(relType.childNodes[0]);
  for (var i = 0; i < relList.options.length; i++) {
    var option = relList.options[i].cloneNode(true);
    if (option.value == -1) {
      relType.appendChild(option);
    } else {
      switch (option.value) {
        case '10': if (fileType.indexOf('subtitle') >= 0) relType.appendChild(option); break;
        case '40': if (fileType.indexOf('video') >= 0) relType.appendChild(option); break;
        case '50': if (fileType.indexOf('audio') >= 0) relType.appendChild(option); break;
        default: relType.appendChild(option);
      }
    }
  }
}

function prepPage() {
  uriObj = parseURI(); // update the uriObj
  if (uriObj['show'] != 'file' || uriObj['show.filereladd'] != 1) return; // badPage
  relType = getElementsByName(document.getElementsByTagName('SELECT'),'frel.reltype',true)[0];
  relFile = getElementsByName(document.getElementsByTagName('SELECT'),'frel.otherfid',true)[0];
  fileType = getElementsByName(document.getElementsByTagName('INPUT'),'ftype',true)[0];
  fileList = relFile.cloneNode(true); // Backup :)
  relList = relType.cloneNode(true); // Backup :)
  if (!relType || !relFile || !fileType) return; // Some error
  relType.onchange = filterFilesByRelationType;
  fileType = fileType.value;
  cleanFilterRelations();
}

//window.onload = prepPage;
addLoadEvent(prepPage);