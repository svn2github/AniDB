/*
 * @file add file relation page support scripts
 * @author fahrenheit (alka.setzer@gmail.com)         
 * @version 1.0 (03.06.2007)
 */
jsVersionArray.push({
	"file":"anime3/addfilerel.js",
	"version":"1.5",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Filtered some more stuff"
});

// GLOBALS
var uriObj = new Array();      // Object that holds the URI
var relType, relFile;
var fileType;
var fileEp;
var fileGid;
var fileAid;
var fileList;
var relList;

function filterFileList(type,filterEp) {
	while (relFile.childNodes.length) relFile.removeChild(relFile.childNodes[0]);
	var blah = 0;
	for (var i = 0; i < fileList.options.length; i++) {
		var option = fileList.options[i].cloneNode(true);
		// Let me just state this once more, i hate IE...
		if (!option.text.length && option.innerText) option.text = option.innerText; // IE hack
		if (type == 'all' || option.value == -1) {
			relFile.appendChild(option);
		} else {
			var fep = option.text.substring(option.text.indexOf('EP')+3,option.text.indexOf(' ',option.text.indexOf('EP')+3));
			var ftype = option.text.substr(option.text.indexOf('TYPE')+5,option.text.length);
			var fgid = option.text.substring(option.text.indexOf('GID')+4,option.text.indexOf(' ',option.text.indexOf('GID')+4));
			if (ftype.indexOf(type) >= 0 && 
					(!fileGid || relType.value == 10 ? true : (Number(fgid) == fileGid)) && // (Number(fgid) == fileGid || Number(fgid) == 0)
					(!filterEp ? true : (fep == fileEp)))
				relFile.appendChild(option); 
		}
	}
}

function filterFilesByRelationType() {
	var type = 'all';
	var filterEp = false;
	switch (this.value) {
		case  '10': 
		case  '40': 
		case  '50': type = 'video'; break;
		case  '20': type = fileType; filterEp = true; break;
		default   : type = 'all';
	}
	filterFileList(type,filterEp);
}

function cleanFilterRelations() {
	while (relType.childNodes.length) relType.removeChild(relType.childNodes[0]);
	for (var i = 0; i < relList.options.length; i++) {
		var option = relList.options[i].cloneNode(true);
		if (option.value == -1) {
			relType.appendChild(option);
		} else {
			switch (option.value) {
				case '10': if (fileType.indexOf('subtitle') >= 0 || fileType.indexOf('other') >= 0) relType.appendChild(option); break;
				case '40': if (fileType.indexOf('video') >= 0) relType.appendChild(option); break;
				case '50': if (fileType.indexOf('audio') >= 0 || fileType.indexOf('other') >= 0) relType.appendChild(option); break;
				default: relType.appendChild(option);
			}
		}
	}
}

function prepPage() {
	uriObj = parseURI(); // update the uriObj
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return;
	var fileSection = getElementsByClassName(document.getElementsByTagName('div'),'g_section file_add_rel',false)[0];
	if (!fileSection) return;
	relType = getElementsByName(fileSection.getElementsByTagName('select'),'frel.reltype',true)[0];
	relFile = getElementsByName(fileSection.getElementsByTagName('select'),'frel.otherfid',true)[0];
	fileType = getElementsByName(fileSection.getElementsByTagName('input'),'ftype',true)[0];
	fileGid = getElementsByName(fileSection.getElementsByTagName('input'),'gid',true)[0];
	
	var a = getElementsByClassName(fileSection.getElementsByTagName('a'),'anime',false)[0];
	if (a) fileAid = Number(a.href.substring(a.href.indexOf('aid=')+4));
	a = getElementsByClassName(fileSection.getElementsByTagName('a'),'episode',false)[0];
	if (a) {
		fileEp = a.firstChild.nodeValue.substring(0,a.firstChild.nodeValue.indexOf(' '));
		while (fileEp.charAt(0) == '0')
			fileEp = fileEp.substring(1);
	}

	fileList = relFile.cloneNode(true); // Backup :)
	relList = relType.cloneNode(true); // Backup :)
	if (!relType || !relFile || !fileType || !fileGid) return; // Some error
	relType.onchange = filterFilesByRelationType;
	fileType = fileType.value;
	fileGid = fileGid.value;
	cleanFilterRelations();
}

//window.onload = prepPage;
addLoadEvent(prepPage);