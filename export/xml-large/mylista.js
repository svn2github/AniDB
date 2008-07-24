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

function DivDisplayer() {
    this.reset = function () {
        this.visibleDivId = 'dummy'
    }
    this.toggleDiv = function(divId) {
        this.hideDiv(this.visibleDivId);
        (divId == this.visibleDivId) ? this.reset() : this.showDiv(divId);
    }
    this.hideDiv = function(divId) {
        var div = document.getElementById(divId);
        if (div) div.className = 'invisible';
    }
    this.showDiv = function(divId) {
        var div = document.getElementById(divId);
        if (div) div.className = '';
        this.visibleDivId = divId;
    }
    this.reset();
}


function setTitle(title) {
    document.title = title;
}

window.onload = function (evt) {
    myListRenderer = new myListXsltUpdater();
    divDisplayer = new DivDisplayer();
}