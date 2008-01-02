var browserDetect = function(IEObjectCreator, MozillaObjectCreator) {
    if (typeof ActiveXObject != 'undefined') return IEObjectCreator();
    else if (typeof document.implementation != 'undefined' && typeof document.implementation.createDocument != 'undefined')
        return MozillaObjectCreator();
}

var XmlDocument = function() {
}

XmlDocument.prototype = {
    storeDomObject: function(xmlDoc) {
        this.setLoaded();
        this._xmlDocument = xmlDoc;
    },
    setLoadedAndCallBack: function(callback) {
        this.setLoaded();
        callback();
    },
    load: function(url, callback) {
        this.setUnloaded();
        this.setForLoading(callback);
        this._xmlDocument.load(url);
    },
    setLoaded: function() {
        this.loaded = true;
    },
    setUnloaded: function() {
        this.loaded = false;
    },
    init: function(xmlNameSpace) {
        this.setUnloaded();
        this.xmlNameSpace = xmlNameSpace;
    },
    isLoaded: function() {
        return this.loaded;
    }
}

XmlDocument.getDocumentAndPrepareForLoading = function(xmlNameSpace) {
    return browserDetect(function IEXdoc() {
        return new IEDocument(xmlNameSpace)
    }, function MozillaXdoc() {
        return new MozillaDocument(xmlNameSpace)
    });
}


var IEDocument = function(xmlNameSpace) {
    this._xmlDocument = new ActiveXObject('Msxml2.DOMDocument');
    this.init(xmlNameSpace);
}

IEDocument.prototype = new XmlDocument();

IEDocument.prototype.setForLoading = function(callback) {
    this._xmlDocument.async = false;
    this._xmlDocument.onreadystatechange = function() {
        if (this._xmlDocument.readyState == 4) this.setLoadedAndCallBack(callback);
    }.bind(this)
}

IEDocument.prototype.selectSingleNode = function(xpath) {
    this._xmlDocument.setProperty('SelectionLanguage', 'XPath');
    this._xmlDocument.setProperty('SelectionNamespaces', 'xmlns:xsl="' + this.xmlNameSpace + '"');
    return this._xmlDocument.selectSingleNode(xpath);
}

IEDocument.prototype.createElement = function(name) {
    return this._xmlDocument.createNode('element', name, this.xmlNameSpace)
}

IEDocument.prototype.transformNode = function(xslDocument) {
    return this._xmlDocument.transformNode(xslDocument._xmlDocument);
}

IEDocument.prototype.transformNodeToObject = function(xslDocument) {
    var outputDocument = new IEDocument('')
    this._xmlDocument.transformNodeToObject(xslDocument._xmlDocument, outputDocument._xmlDocument);
    return outputDocument;
}

IEDocument.prototype.insertIntoElement = function(element) {
    element.innerHTML = this._xmlDocument.lastChild.xml;
}

var MozillaDocument = function(xmlNameSpace) {
    this._xmlDocument = document.implementation.createDocument('', 'dummy', null);
    this.init(xmlNameSpace);
}

MozillaDocument.prototype = new XmlDocument();

MozillaDocument.prototype.setForLoading = function(callback) {
    this._xmlDocument.async = true;
    this._xmlDocument.onload = function() {
        this.setLoadedAndCallBack(callback)
    }.bind(this)
}

MozillaDocument.prototype.nameSpaceResolver = function() {
    var xmlNameSpace = this.xmlNameSpace;
    return function(prefix) {
        return xmlNameSpace;
    }
}

MozillaDocument.prototype.selectSingleNode = function(xpath) {
    var result = this._xmlDocument.evaluate(xpath, this._xmlDocument, this.nameSpaceResolver(), XPathResult.ANY_UNORDERED_NODE_TYPE, null);
    return result.singleNodeValue;
}

MozillaDocument.prototype.createElement = function(name) {
    return this._xmlDocument.createElementNS(this.xmlNameSpace, name)
}

MozillaDocument.prototype.insertIntoElement = function(element) {
    var r = element.ownerDocument.createRange();
    r.selectNodeContents(element);
    r.deleteContents();
    var df = r.createContextualFragment(new XMLSerializer().serializeToString(this._xmlDocument.lastChild));
    element.appendChild(df);
}

var XslDocument = function() {
}

XslDocument.prototype = {
    init: function(XmldocType) {
        this._xslDocument = new XmldocType('http://www.w3.org/1999/XSL/Transform');
    },
    load: function(url, callback) {
        this._xslDocument.load(url, callback);
    },
    isLoaded: function() {
        return this._xslDocument.isLoaded();
    },
    removeSortNodes: function(node) {
        if (!node) return;
        var sortNodes = CollectionNameSpace.findall(node.childNodes, function isSortNode(childNode) {
            return childNode.nodeName == 'xsl:sort';
        })
        CollectionNameSpace.forEach(sortNodes, function remove(childNode) {
            node.removeChild(childNode);
        });
    },
    addSortNode: function(node, orderBy, dataType, sortOrder) {
        if (!node) return;
        if (dataType == 'date') this.addDateSortNode(node, orderBy, dataType, sortOrder);
        else {
            var xslSort = this._xslDocument.createElement('xsl:sort');
            xslSort.setAttribute('select', orderBy);
            xslSort.setAttribute('data-type', dataType);
            xslSort.setAttribute('order', sortOrder);
            node.appendChild(xslSort);
        }
    },
    addDateSortNode: function(node, orderBy,sortOrder) {
        this.addSortNode(node, 'substring(normalize-space(' + orderBy + '),7,4)', 'number', sortOrder);
        this.addSortNode(node, 'substring(normalize-space(' + orderBy + '),4,2)', 'number', sortOrder);
        this.addSortNode(node, 'substring(normalize-space(' + orderBy + '),1,2)', 'number', sortOrder);
    },
    selectApplyTemplateNode: function(tagName) {
        return this._xslDocument.selectSingleNode('//xsl:apply-templates[@select = "' + tagName + '"]');
    },
    removeXslSortFromApplyTemplate: function(tagName) {
        this.removeSortNodes(this.selectApplyTemplateNode(tagName));
    },
    addXslSortToApplyTemplate: function(tagName, orderBy, dataType, sortOrder) {
        this.addSortNode(this.selectApplyTemplateNode(tagName), orderBy, dataType, sortOrder);
    },
    replaceXslSortOnApplyTemplate: function(tagName, orderBy, dataType, sortOrder) {
        var node = this.selectApplyTemplateNode(tagName);
        this.removeSortNodes(node)
        this.addSortNode(node, orderBy, dataType, sortOrder)
    }
}

XslDocument.getDocumentAndPrepareForLoading = function() {
    return browserDetect(function IEXsldoc() {
        return new IEXslDocument()
    }, function MozillaXsldoc() {
        return new MozillaXslDocument()
    });
}

var IEXslDocument = function() {
    this.init(IEDocument);
}

IEXslDocument.prototype = new XslDocument();

IEXslDocument.prototype.transformedDocument = function(xmlDocument) {
    return xmlDocument.transformNodeToObject(this._xslDocument);
}

IEXslDocument.prototype.setParameter = function(name, value) {
    var node = this._xslDocument.selectSingleNode('//xsl:param[@name="' + name + '"]')
    node.removeAttribute('select');
    node.setAttribute('select', "'" + value + "'");
}

IEXslDocument.prototype.transformIntoElement = function(xmlDocument, resultElement) {
    resultElement.innerHTML = xmlDocument.transformNode(this._xslDocument);
}

var MozillaXslDocument = function() {
    this.init(MozillaDocument);
}

MozillaXslDocument.prototype = new XslDocument();

MozillaXslDocument.prototype.load = function(url, callback) {
    var interceptedCallback = function() {
        this.xsltProcessor = new XSLTProcessor();
        this.xsltProcessor.importStylesheet(this._xslDocument._xmlDocument);
        callback()
    }.bind(this)
    this._xslDocument.load(url, interceptedCallback);
}

MozillaXslDocument.prototype.transformedDocument = function(xmlDocument) {
    var outputDocument = XmlDocument.getDocumentAndPrepareForLoading('');
    outputDocument.storeDomObject(this.xsltProcessor.transformToDocument(xmlDocument._xmlDocument));
    return outputDocument;
}

MozillaXslDocument.prototype.setParameter = function(name, value) {
    this.xsltProcessor.setParameter('', name, value)
}

MozillaXslDocument.prototype.transformIntoElement = function(xmlDocument, resultElement) {
    var documentFragment = this.xsltProcessor.transformToFragment(xmlDocument._xmlDocument, resultElement.ownerDocument);
    while (resultElement.hasChildNodes()) {
        resultElement.removeChild(resultElement.lastChild);
    }
    resultElement.appendChild(documentFragment);
}

var loadDocuments = function(documentMap, callback) {
    var callbackFunction = function() {
        var status = true;
        for (var key in documentMap)
        {
            if (!documentMap[key].isLoaded()) {
                status = false;
            }
        }
        if (status) callback();
    }


    for (var key in documentMap) {
        documentMap[key].load(key, callbackFunction);
    }
}


