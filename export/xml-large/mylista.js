function over(obj, name) {
    className = obj.className
    if (obj.onmouseout == null || typeof obj.onmouseout == 'undefined')
        obj.onmouseout = function(){ obj.className = className;}
    obj.className = name;
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