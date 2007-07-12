var xmlURL = "mylist.xml";
var firstPassXslURL = "firstPass.xsl";
var htmlGenerationXslURL = "mylistb.xsl";
var resultElementId = "resultDiv";
var lastclass = "";
var sfv = 0;
var selected = 0;
var lastsel = 0;
var watched = 0;
var complete = 0;
var sortName = "N";
var sortOrder = "ascending";
var sortType = "text";

function over(obj, name) {
    obj.style.cursor = 'pointer';
    obj.className = name;
}
function out(obj) {
    obj.style.cursor = '';
    obj.className = '';
}

function over2(obj, name){
	lastclass = obj.className;
	obj.className=name;
}

function out2(obj){
	obj.className=lastclass;
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

function setTitle(title)
{
    document.title = title;
}

function setXslParamShow(name, checkbox) {
    if (checkbox.checked)
     xdoc.setXslParameter(name,'false');
     else xdoc.setXslParameter(name,'true');
    xdoc.update();
}

function sortTable(field, type) {
     if(sortName==field){
		if(sortOrder=="ascending")
			sortOrder="descending";
		else	sortOrder="ascending";
	}
	else sortName = field;
    xdoc.setXslParameter("sortNodeName", field);
    xdoc.setXslParameter("sortDataType", type);
    xdoc.setXslParameter("sortDirection", sortOrder);
    xdoc.update();
}

window.onload = function (evt) {
      xdoc = new xsltUpdater(xmlURL,firstPassXslURL,htmlGenerationXslURL,resultElementId)
}