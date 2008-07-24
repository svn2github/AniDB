function myListXsltUpdater() {
    this.resultElement = document.getElementById("resultDiv");
    this.loadTopPanel();
    this.animeDetailsRenderer = new animeDetailsUpdater();
    this.loadDocuments();
}

myListXsltUpdater.prototype = {
    loadTopPanel: function() {
        insertDocumentIntoElement("topPanel")
    },
    loadDocuments: function() {
        this.xmlDocument = XmlDocument.getDocumentAndPrepareForLoading('');
        this.firstXslDocument = XslDocument.getDocumentAndPrepareForLoading();
        this.secondXslDocument = XslDocument.getDocumentAndPrepareForLoading();
        loadDocuments({"mylist.xml": this.xmlDocument, "firstPass.xsl": this.firstXslDocument,
            "mylistb.xsl": this.secondXslDocument}, this.buildIntermediateXml.bind(this));
    },
    buildIntermediateXml: function () {
        this.intermediateXml = this.firstXslDocument.transformedDocument(this.xmlDocument);
        this.update();
    },
    update: function (parameters) {
        this.secondXslDocument.transformIntoElement(this.intermediateXml, this.resultElement, parameters);
        this.animeDetailsRenderer.updateDisplay();
    },
    selectShow: function(id) {
        this.animeDetailsRenderer.handleUserSelection(id);
    }
}

var animeDetailsUpdater = function() {
    this.selectedId = 0;
    this.xslDocument = XslDocument.getDocumentAndPrepareForLoading();
    this.animeDocument = XmlDocument.getDocumentAndPrepareForLoading('');
    this.xslDocument.load("anime.xsl", this.fillRowElement())
}
animeDetailsUpdater.prototype = {
    handleUserSelection: function(id) {
        this.collapseShow();
        this.selectedId = (this.selectedId == id ? 0 : id);
        this.updateDisplay();
    },
    collapseShow: function() {
        clearElementWithId('r2-' + this.selectedId)
        clearElementWithId('r3-' + this.selectedId)
    },
    updateDisplay: function() {
        this.newCol = document.getElementById('r2-' + this.selectedId)
        if (this.selectedId == 0 || !this.newCol) return;
        insertDocumentIntoElement('r3-'+this.selectedId,"bottomClick.xml")
        this.animeDocument.load("anime/" + this.selectedId + ".xml", this.fillRowElement())
    },
    fillRowElement: function() {
        return function() {
            if (this.xslDocument.isLoaded() && this.animeDocument.isLoaded())
            {
                this.xslDocument.transformIntoElement(this.animeDocument, this.newCol);
            }
        }.bind(this)
    }
}