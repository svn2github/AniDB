function myListXsltUpdater() {
    this.resultElement = document.getElementById("resultDiv");
    this.loadTopPanel();
    this.animeDetailsRenderer = new animeDetailsUpdater();
    this.loadDocuments();
}

myListXsltUpdater.prototype = {
    loadTopPanel: function() {
        var topPanelDoc = XmlDocument.getDocumentAndPrepareForLoading('');
        topPanelDoc.load("topPanel.xml", function displayTopPamel() {
            topPanelDoc.insertIntoElement(document.getElementById("topPanel"));
        })
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
    this.animeInfoId = "animeInfo";
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
        document.removeElementById(this.animeInfoId);
    },
    updateDisplay: function() {
        var element = document.getElementById(this.selectedId);
        if (this.selectedId == 0 || !element) return;
        var newRow = this.createRowElement();
        this.newCol = document.createElement("td");
        this.newCol.setAttribute("colSpan", "7")
        newRow.appendChild(this.newCol)
        document.insertAfter(newRow, element);
        this.animeDocument.load("anime/" + this.selectedId + ".xml", this.fillRowElement())
    },
    createRowElement: function() {
        var newRow = document.createElement("tr");
        newRow.setAttribute("id", this.animeInfoId);
        return newRow;
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