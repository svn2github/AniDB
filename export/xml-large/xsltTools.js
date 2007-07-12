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
    load: function(url) {
        this.setUnloaded();
        this._xmlDocument.load(url);
    },
    setLoaded: function(url) {
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

XmlDocument.getDocumentAndPrepareForLoading = function(xmlNameSpace, callback) {
    var xDoc = browserDetect(function IEXdoc() {
        return new IEDocument(xmlNameSpace)
    }, function MozillaXdoc() {
        return new MozillaDocument(xmlNameSpace)
    });
    if (typeof xDoc != undefined) xDoc.setForLoading(callback);
    return xDoc;
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
    this._xmlDocument.transformNodeToObject(xslDocument._xmlDocument,outputDocument._xmlDocument);
    return outputDocument;
}

var MozillaDocument = function(xmlNameSpace, callback) {
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
    return this._xmlDocument.createElementNS(this.xmlNameSpace, 'xsl:sort')
}

var XslDocument = function() {
}

XslDocument.prototype = {
    init: function(XmldocType, callback) {
        this._xslDocument = new XmldocType('http://www.w3.org/1999/XSL/Transform');
        this._xslDocument.setForLoading(callback)
    },
    load: function(url) {
        this._xslDocument.load(url);
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
    addDateSortNode: function(node, orderBy, dataType, sortOrder) {
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

XslDocument.getDocumentAndPrepareForLoading = function(callback) {
    return browserDetect(function IEXsldoc() {
        return new IEXslDocument(callback)
    }, function MozillaXsldoc() {
        return new MozillaXslDocument(callback)
    });
}

var IEXslDocument = function(callback) {
    this.init(IEDocument, callback);
}

IEXslDocument.prototype = new XslDocument();

IEXslDocument.prototype.transformedDocument = function(xmlDocument) {
    return xmlDocument.transformNodeToObject(this._xslDocument);
}

IEXslDocument.prototype.setParameter = function(name, value) {
    var node = this._xslDocument.selectSingleNode('//xsl:param[@name="' + name + '"]')
    node.removeAttribute('select');
    node.setAttribute('select',"'"+ value + "'");
}

IEXslDocument.prototype.transformIntoElement = function(xmlDocument, resultElement) {
    resultElement.innerHTML = xmlDocument.transformNode(this._xslDocument);
}

var MozillaXslDocument = function(callback) {
    var interceptedCallback = function() {
        this.xsltProcessor = new XSLTProcessor();
        this.xsltProcessor.importStylesheet(this._xslDocument._xmlDocument);
        callback()
    }.bind(this)
    this.init(MozillaDocument, interceptedCallback);
}

MozillaXslDocument.prototype = new XslDocument();

MozillaXslDocument.prototype.transformedDocument = function(xmlDocument) {
    var outputDocument = XmlDocument.getDocumentAndPrepareForLoading('');
    outputDocument.storeDomObject(this.xsltProcessor.transformToDocument(xmlDocument._xmlDocument));
    return outputDocument;
}

MozillaXslDocument.prototype.setParameter = function(name, value) {
    this.xsltProcessor.setParameter('',name,value)
}

MozillaXslDocument.prototype.transformIntoElement = function(xmlDocument, resultElement) {
    var documentFragment = this.xsltProcessor.transformToFragment(xmlDocument._xmlDocument, resultElement.ownerDocument);
    while (resultElement.hasChildNodes()) {
        resultElement.removeChild(resultElement.lastChild);
    }
    resultElement.appendChild(documentFragment);
}

function xsltUpdater(xmlUrl, firstXslUrl, secondXslUrl, resultElementId)
{
    this.resultElement = document.getElementById(resultElementId);
    this.xmlDocument = XmlDocument.getDocumentAndPrepareForLoading('', this.transformIfLoaded.bind(this))
    this.firstXslDocument = XslDocument.getDocumentAndPrepareForLoading(this.transformIfLoaded.bind(this))
    this.secondXslDocument = XslDocument.getDocumentAndPrepareForLoading(this.transformIfLoaded.bind(this))
    this.loaded = typeof this.xmlDocument != 'undefined';
    this.xmlDocument.load(xmlUrl)
    this.firstXslDocument.load(firstXslUrl)
    this.secondXslDocument.load(secondXslUrl)
}

xsltUpdater.prototype = {
    transformIfLoaded:  function() {
        if (this.xmlDocument.isLoaded() && this.firstXslDocument.isLoaded() && this.secondXslDocument.isLoaded()) {
            this.buildIntermediateXml();
        }
    },
    buildIntermediateXml: function () {
        this.intermediateXml = this.firstXslDocument.transformedDocument(this.xmlDocument)
        this.update()
    },
    update: function () {
        this.secondXslDocument.transformIntoElement(this.intermediateXml, this.resultElement);
    },
    setXslParameter: function(name, value) {
        this.secondXslDocument.setParameter(name, value);
    }
}

