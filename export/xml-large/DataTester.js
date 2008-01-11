function testMyList(sender) {
    xmlTest(sender, new Array('mylist.xml', 'malformed.xml', 'unknown.xml'));
}

function testanimeFiles(sender) {
    var getAnimeFile = function(node) {
        return 'anime/a' + node.getAttribute('id') + '.xml';
    }
    var listDoc = XmlDocument.getDocumentAndPrepareForLoading('');
    var callback = function() {
        var nodes = listDoc.selectNodes('//anime');
        var fileNames = CollectionNameSpace.ConvertAll(nodes, getAnimeFile);
        xmlTest(sender, fileNames);
    }
    listDoc.load("mylist.xml", callback);
}

function xmlTest(sender, fileList) {
    var fileNumber = 0;
    var failureCount = 0;
    var message = '';
    var loadFile = function(fileName) {
        var listDoc = XmlDocument.getDocumentAndPrepareForLoading('');
        listDoc.load(fileName, callBack(listDoc, fileName))
    }

    var callBack = function(listDoc, fileName) {
        return function()
        {
            var parseError = listDoc.parseError;
            if (parseError.hasError) {
                failureCount += 1;
                message += '<div>' + fileName + ' : ' + parseError.reason + '</div>';
            }
            fileNumber += 1;
            if (fileNumber == fileList.length)
                displayResult();
        }
    }

    var displayResult = function() {
        document.getElementById(sender.id + 'Result').innerHTML = (failureCount == 0) ? 'Success' : 'Failure on ' + failureCount + ' files.'
        document.getElementById(sender.id + 'ResultMessage').innerHTML = message;
    }
    CollectionNameSpace.forEach(fileList, loadFile);
}




