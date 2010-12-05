String.exists = function(str, item) {
    return (str.indexOf(item) != -1)
}

var equality = function(item) {
    return function(other) {
        return item == other
    }
}

var CollectionNameSpace = {
    forEach: function(ary, operation) {
        for (var i = 0; i < ary.length; i++)
        {
            operation(ary[i])
        }
    },

    exists: function(ary, condition) {
        return (CollectionNameSpace.find(ary, condition) != null)
    },

    find: function(ary, condition) {
        for (var i = 0; i < ary.length; i++)
            if (condition(ary[i])) return ary[i]
        return null
    },

    findall: function(ary, condition) {
        var foundItems = new Array();
        CollectionNameSpace.forEach(ary, function collect(item) {
            if (condition(item)) foundItems.push(item);
        })
        return foundItems;
    },
    ConvertAll: function(ary, converter) {
        var result = new Array();
        CollectionNameSpace.forEach(ary, function store(item) {
            result.push(converter(item));
        })
        return result;
    }
}

Function.prototype.bind = function(object) {
    var method = this;
    return function() {
        method.apply(object, arguments);
    }
}

function unlinkParentClickEvent(element) {
    element.parentNode.onclick = null;
  }

function insertDocumentIntoElement(elementId, documentName) {
    var topPanelDoc = XmlDocument.getDocumentAndPrepareForLoading('');
    topPanelDoc.load(documentName, function displayTopPamel() {
        topPanelDoc.insertIntoElement(document.getElementById(elementId));
    })
}

function clearElementWithId(elementId) {
    var element = document.getElementById(elementId);
    if (element) element.innerHTML = ''
}





