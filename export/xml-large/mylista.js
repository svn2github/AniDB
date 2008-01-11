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

function setTitle(title) {
    document.title = title;
}

window.onload = function (evt) {
    myListRenderer = new myListXsltUpdater();

}