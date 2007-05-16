var xmlURL = "mylist.xml";
var xslURL = "mylistb.xsl";
var resultElementId = "resultDiv";
var lastclass ="";
var sfv = 0;
var selected = 0;
var lastsel = 0;
var watched = 0;
var completed = 0;
var sortName = "Name";
var sortOrder = "ascending";
var sortType = "text";

function over(obj, name){
	obj.style.cursor='pointer';
	obj.className=name;
}
function out(obj){
	obj.style.cursor='';
	obj.className='';
}
function over2(obj, name){
	lastclass = obj.className;
	obj.className=name;
}
function out2(obj){
	obj.className=lastclass;
}
function sort(field, type){
	selected = 0;
	if(sortName==field){
		if(sortOrder=="ascending")
			sortOrder="descending";
		else	sortOrder="ascending";
	}
	else sortName = field;

	sortType = type;
	
	setVariable(xslDocument, "selected", selected);
	sortColumns(xmlDocument, xslDocument, resultElementId, sortName, sortType, sortOrder);
	update();
}
function select(nr){
	if(lastsel==nr) nr = 0;
	lastsel = nr;
	selected=nr;
	
	setVariable(xslDocument, "selected", selected);
	update();
}
function swWatched(obj, name){
	selectedN = '0';
	if(watched=='0'){
		watched = '1';
		obj.className=name;
	}else{
		watched = '0';
		obj.className='';
	}
	
	setVariable(xslDocument, "watched", watched);
	update();
}
function swComplete(obj){
	selectedN = 0;
	if(completed=='0'){
			completed = '1';
			obj.className='opt';
		}else{
			completed = '0';
			obj.className='';
		}

	setVariable(xslDocument, "completed", completed);
	update();
}
function swSFV(obj){
	if(sfv==0){
			sfv = 1;
			obj.className='opt';
	}else{
		sfv = 0;
		obj.className='';
	}

	setVariable(xslDocument, "sfv", sfv);
	update();
}

function setVariable(xslDocument, varname, value) {
  var xslNamespace = 'http://www.w3.org/1999/XSL/Transform';
  if (typeof xslDocument.setProperty != 'undefined') {
    xslDocument.setProperty('SelectionLanguage', 'XPath');
    xslDocument.setProperty('SelectionNamespaces', 'xmlns:xsl="' + xslNamespace + '"');
    var xslvar = xslDocument.selectSingleNode("//xsl:variable[@name='"+varname+"']/@select");
    xslvar.value = value;
  }
  else if (typeof xslDocument.getElementsByTagNameNS != 'undefined'){
    var xslApplyTemplates = xslDocument.getElementsByTagNameNS(xslNamespace, 'variable');
    for(var i = 0; i < xslApplyTemplates.length; i++) {
      var xslApplyTemplate = xslApplyTemplates[i];
    if (xslApplyTemplate.getAttribute('name') == varname) {
        xslApplyTemplate.setAttribute('select', value);
        break;
      }
    }
  }
}
function sortColumns(xmlDocument, xslDocument, resultElementId, orderBy, dataType, sortOrder) {
  removeXslSort(xslDocument, 'Anime');
  if (typeof orderBy != 'undefined') {
    if (typeof dataType == 'undefined') {
      dataType = 'text';
    }
    if (typeof sortOrder == 'undefined') {
      sortOrder = 'ascending';
    }
    addXslSort(xslDocument, 'Anime', orderBy, dataType, sortOrder);
  }
}
function removeXslSort (xslDocument, tagName) {
  var xslNamespace = 'http://www.w3.org/1999/XSL/Transform';
  if (typeof xslDocument.setProperty != 'undefined') {
    xslDocument.setProperty('SelectionLanguage', 'XPath');
    xslDocument.setProperty('SelectionNamespaces', 'xmlns:xsl="' + xslNamespace + '"');
    var xslSort = xslDocument.selectSingleNode('//xsl:sort');
    if (xslSort) {
      xslSort.parentNode.removeChild(xslSort);
    }
  }
  else if (typeof xslDocument.getElementsByTagNameNS != 'undefined') {
    var xslSorts = xslDocument.getElementsByTagNameNS(xslNamespace, 'sort');
    if (xslSorts.length > 0) {
      xslSorts[0].parentNode.removeChild(xslSorts[0]);
    }
  }
}
function addXslSort (xslDocument, tagName, orderBy, dataType, sortOrder) {
  var xslNamespace = 'http://www.w3.org/1999/XSL/Transform';
  if (typeof xslDocument.setProperty != 'undefined') {
    xslDocument.setProperty('SelectionLanguage', 'XPath');
    xslDocument.setProperty('SelectionNamespaces', 'xmlns:xsl="' + xslNamespace + '"');
    var xslApplyTemplates = xslDocument.selectSingleNode('//xsl:apply-templates[@select = "' + tagName + '"]');
    if (xslApplyTemplates) {
      var xslSort = xslDocument.createNode(1, 'xsl:sort', xslNamespace);
      xslSort.setAttribute('select', orderBy);
      xslSort.setAttribute('data-type', dataType);
      xslSort.setAttribute('order', sortOrder);
      xslApplyTemplates.appendChild(xslSort);
    }
  }
  else if (typeof xslDocument.getElementsByTagNameNS != 'undefined'){
    var xslApplyTemplates = xslDocument.getElementsByTagNameNS(xslNamespace, 'apply-templates');
    for (var i = 0; i < xslApplyTemplates.length; i++) {
      var xslApplyTemplate = xslApplyTemplates[i];
      if (xslApplyTemplate.getAttribute('select') == tagName) {
        var xslSort = xslDocument.createElementNS(xslNamespace, 'xsl:sort');
        xslSort.setAttribute('select', orderBy);
        xslSort.setAttribute('data-type', dataType);
        xslSort.setAttribute('order', sortOrder);
        xslApplyTemplate.appendChild(xslSort);
        break;
      }
    }
  }
}
function update(){
  var resultElement;
  if (document.getElementById){
    resultElement = document.getElementById(resultElementId);
    if (resultElement) {
      if (typeof xmlDocument.transformNode != 'undefined') {
        resultElement.innerHTML = xmlDocument.transformNode(xslDocument);
      }
      else if (typeof XSLTProcessor != 'undefined') {
        var xsltProcessor = new XSLTProcessor()
        if (typeof xsltProcessor.importStylesheet != 'undefined') {
          xsltProcessor.importStylesheet(xslDocument);
          var documentFragment = xsltProcessor.transformToFragment(xmlDocument, resultElement.ownerDocument);
          while (resultElement.hasChildNodes()) {
            resultElement.removeChild(resultElement.lastChild);
          }
          resultElement.appendChild(documentFragment);
        }
        else if (typeof xsltProcessor.transformDocument != 'undefined') {
          var resultDocument = document.implementation.createDocument('', 'dummy', null);
          xsltProcessor.transformDocument(xmlDocument, xslDocument, resultDocument, null);
          while (resultElement.hasChildNodes()) {
            resultElement.removeChild(resultElement.lastChild);
          }
          resultElement.appendChild(resultElement.ownerDocument.importNode(resultDocument.documentElement, true));
        }
      }
    }
  }
}
////INIT////
function transformIfLoaded () {
	if(xmlDocument.readyState == 4 && xslDocument.readyState == 4) {
		sortColumns(xmlDocument, xslDocument, resultElementId);
		update();
	}
}

function initDocuments (xmlURL, xslURL) {
  if (typeof ActiveXObject != 'undefined') {
    xmlDocument = new ActiveXObject('Msxml2.DOMDocument');
    xmlDocument.async = false;///!
    xmlDocument.onreadystatechange = transformIfLoaded;
    xslDocument = new ActiveXObject('Msxml2.DOMDocument');
    xslDocument.async = false;///!
    xslDocument.onreadystatechange = transformIfLoaded;
  }
  else if (typeof document.implementation != 'undefined' &&
           typeof document.implementation.createDocument != 'undefined') {
    xmlDocument = document.implementation.createDocument('', 'dummy', null);
    xmlDocument.async = true;
    xslDocument = document.implementation.createDocument('', 'dummy', null);
    xslDocument.async = true;
    xmlDocument.onload = function (evt) {
      xmlDocument.readyState = 4;
      transformIfLoaded();
    };
    xslDocument.onload = function (evt) {
      xslDocument.readyState = 4;
      transformIfLoaded();
    };
  }
  if (typeof xmlDocument != 'undefined' &&
      typeof xmlDocument.load != 'undefined') {
    xmlDocument.load(xmlURL);
    xslDocument.load(xslURL);
  }
  else {
    var message = 'This browser doesn\'t seem to support scripting of XML documents.';
    var p;
    if (document.createElement && (p = document.createElement('p'))) {
      p.appendChild(document.createTextNode(message));
      document.getElementById(resultElementId).appendChild(p);
    }
    else {
      alert(message);
    }
  }
}
window.onload = function (evt) {
  initDocuments(xmlURL, xslURL);
}