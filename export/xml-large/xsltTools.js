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
        this.parseError = this.checkParseError();
        callback();
    },
    load: function(url, callback) {
        this.setUnloaded();
        this.setForLoading(callback);
        try {
            this._xmlDocument.load(url);
        }
        catch (ex) {
            this.handleLoadError(ex, callback);
        }
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
        this.parseError = this.createParseError(false);
    },
    isLoaded: function() {
        return this.loaded;
    },
    createParseError: function(hasError, reason, errorCode) {
        if (typeof errorCode == 'undefined')
            errorCode = (hasError) ? 1 : 0;
        return {'hasError' : hasError,'errorCode' : errorCode, 'reason' : reason}
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
        if (this._xmlDocument.readyState != 4) return;
        this.parseError = this._xmlDocument.parseError;
        this.setLoadedAndCallBack(callback);
    }.bind(this)
}

IEDocument.prototype.handleLoadError = function(exception, callback) {

}

IEDocument.prototype.xml = function() {
    return this._xmlDocument.xml;
}

IEDocument.prototype.text = function() {
    return this._xmlDocument.text;
}

IEDocument.prototype.selectSingleNode = function(xpath) {
    this._xmlDocument.setProperty('SelectionLanguage', 'XPath');
    this._xmlDocument.setProperty('SelectionNamespaces', 'xmlns:xsl="' + this.xmlNameSpace + '"');
    return this._xmlDocument.selectSingleNode(xpath);
}

IEDocument.prototype.selectNodes = function(xpath) {
    this._xmlDocument.setProperty('SelectionLanguage', 'XPath');
    this._xmlDocument.setProperty('SelectionNamespaces', 'xmlns:xsl="' + this.xmlNameSpace + '"');
    return this._xmlDocument.selectNodes(xpath);
}

IEDocument.prototype.createElement = function(name) {
    return this._xmlDocument.createNode('element', name, this.xmlNameSpace)
}

IEDocument.prototype.checkParseError = function() {
    var parseError = this._xmlDocument.parseError;
    return this.createParseError((parseError.errorCode != 0), parseError.reason, parseError.errorCode)
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

MozillaDocument.prototype.checkParseError = function() {
    var parseError = this._xmlDocument.documentElement.nodeName == "parsererror";
    if (parseError) var reason = this.xml();
    return this.createParseError(parseError, reason)
}

MozillaDocument.prototype.handleLoadError = function(exception, callback) {
    this.parseError = this.createParseError(true, exception.message);
    callback();
}

MozillaDocument.prototype.xml = function () {
    return new XMLSerializer().serializeToString(this._xmlDocument);
}

MozillaDocument.prototype.text = function() {
    return this._xmlDocument.lastChild.textContent;
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

MozillaDocument.prototype.selectNodes = function(xpath) {
    var result = this._xmlDocument.evaluate(xpath, this._xmlDocument, this.nameSpaceResolver(), XPathResult.ORDERED_NODE_ITERATOR_TYPE, null);
    var returnVal = new Array();
    var node = result.iterateNext();
    while (node) {
        returnVal.push(node);
        node = result.iterateNext();
    }
    return returnVal;
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
    processParameters: function(parameters) {
        if (typeof parameters != 'undefined')
            for (var param in parameters) {
                this.setParameter(param, parameters[param])
            }
    },
    transformedDocument: function(xmlDocument, parameters) {
        this.processParameters(parameters);
        return this.getTransformedDocument(xmlDocument);
    },
    transformIntoElement: function(xmlDocument, resultElement, parameters) {
        this.processParameters(parameters);
        this.performTransformIntoElement(xmlDocument, resultElement);
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

IEXslDocument.prototype.getTransformedDocument = function(xmlDocument) {
    return xmlDocument.transformNodeToObject(this._xslDocument);
}

IEXslDocument.prototype.setParameter = function(name, value) {
    var node = this._xslDocument.selectSingleNode('/xsl:stylesheet/xsl:param[@name="' + name + '"]')
    node.removeAttribute('select');
    node.setAttribute('select', "'" + value + "'");
}

IEXslDocument.prototype.performTransformIntoElement = function(xmlDocument, resultElement) {
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

MozillaXslDocument.prototype.getTransformedDocument = function(xmlDocument) {
    var outputDocument = XmlDocument.getDocumentAndPrepareForLoading('');
    outputDocument.storeDomObject(this.xsltProcessor.transformToDocument(xmlDocument._xmlDocument));
    return outputDocument;
}

MozillaXslDocument.prototype.setParameter = function(name, value) {
    this.xsltProcessor.removeParameter(null, name);
    this.xsltProcessor.setParameter(null, name, value);
}

MozillaXslDocument.prototype.performTransformIntoElement = function(xmlDocument, resultElement) {
    var documentFragment = this.xsltProcessor.transformToFragment(xmlDocument._xmlDocument, resultElement.ownerDocument);
    while (resultElement.hasChildNodes()) {
        resultElement.removeChild(resultElement.lastChild);
    }
    resultElement.appendChild(documentFragment);
}

var getTransformationOutput = function(xmlName, xslName, passOutput) {
    var xmlDoc = XmlDocument.getDocumentAndPrepareForLoading();
    var xslDoc = XslDocument.getDocumentAndPrepareForLoading();
    callback = function() {
        var output = xslDoc.transformedDocument(xmlDoc);
        passOutput(output);
    }
    var docMap = {}
    docMap[xmlName] = xmlDoc;
    docMap[xslName] = xslDoc;
    loadDocuments(docMap, callback);
}

var transformXmlIntoElement = function(xmlName, xslName, element) {
    var xmlDoc = XmlDocument.getDocumentAndPrepareForLoading();
    var xslDoc = XslDocument.getDocumentAndPrepareForLoading();
    insertIntoElement = function() {
        xslDoc.transformIntoElement(xmlDoc, element);
    }
    var docMap = {}
    docMap[xmlName] = xmlDoc
    docMap[xslName] = xslDoc
    loadDocuments(docMap, insertIntoElement)
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


