var sfv = 0;
var selected = 0;
var lastsel = 0;
var watched = 0;
var complete = 0;
var selectedId = 0;
var lastClass


function over(obj, name) {
    obj.style.cursor = 'pointer';
    overChangeClass(obj, name)
}
function out(obj) {
    obj.style.cursor = '';
    outChangeClass(obj)
}

function overChangeClass(obj, name) {
    lastClass = obj.className;
    obj.className = name;
}

function outChangeClass(obj) {
    obj.className = lastClass;
}

//function radioButton() {
//    var checkedRadioButton = function(radioButton){return radioButton.checked}
//    alert(CollectionNameSpace.find(document.getElementsByName("sortOrder"),checkedRadioButton).value)
//}


////function select(nr){
////	if(lastsel==nr) nr = 0;
////	lastsel = nr;
////	selected=nr;
////
////	setVariable(xslDocument, "selected", selected);
////	update();
////}
//function swWatched(obj, name){
//	selectedN = '0';
//	if(watched=='0'){
//		watched = '1';
//		obj.className=name;
//	}else{
//		watched = '0';
//		obj.className='';
//	}
//
//	setVariable(xslDocument, "watched", watched);
//	update();
//}
//function swComplete(obj){
//	selectedN = '0';
//	if(complete=='0'){
//		complete = '1';
//		obj.className='opt';
//	}else{
//		complete = '0';
//		obj.className='';
//	}
//
//	setVariable(xslDocument, "complete", complete);
//	update();
//}
//function swSFV(obj){
//	if(sfv==0){
//			sfv = 1;
//			obj.className='opt';
//	}else{
//		sfv = 0;
//		obj.className='';
//	}
//
//	setVariable(xslDocument, "sfv", sfv);
//	update();
//}

function setTitle(title) {
    document.title = title;
}

function collapseShow() {
    var element = document.getElementById(animeInfoId);
    if (element)
        element.parentNode.removeChild(element)
}

function expandShow(id) {
    var element = document.getElementById(id);
    if (element) {
        var newRow = element.ownerDocument.createElement("tr");
        newRow.setAttribute("id", animeInfoId);
        var newCol = element.ownerDocument.createElement("td");
        newRow.appendChild(newCol)
        newCol.appendChild(element.ownerDocument.createTextNode("blah"))
        prevElement = element.nextSibling;
        insertAfter(newRow, element)
    }
}

function selectShow(id) {
    collapseShow();
    if (selectedId != id) {
        selectedId = id;
        expandShow(selectedId);
    }
}

window.onload = function (evt) {
    myListRenderer = new myListXsltUpdater();

}