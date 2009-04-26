/* file for testing browser capabilities */
var jsVersionArray = new Array();

function doReport(text, result) {
	var ico = (typeof(result) == 'boolean' ? (result ? 'icon_message' : 'icon_error') : 'icon_warning');
	var spanTest = document.createElement('span');
	spanTest.className = 'test';
	spanTest.appendChild(document.createTextNode(text+': '));
	var spanResult = document.createElement('span');
	spanResult.className = 'result';
	var img = document.createElement('img');
	img.src = ico+'.gif';
	img.title = 'result: '+result;
	spanResult.appendChild(img);
	spanTest.appendChild(spanResult);
	document.body.appendChild(spanTest);
	document.body.appendChild(document.createElement('br'));
}

function goDeepDOMtree(node,level) {
  if (!node.childNodes.length) return (node.nodeValue);
  var spacing = '';
  for (var k = 0; k < level; k++) spacing += '..';
  var out = spacing +'<'+node.nodeName+'>';
  for (var i = 0; i < node.childNodes.length; i++){
    var sNode = node.childNodes[i];
    if (sNode.childNodes && sNode.childNodes.length) out += '\n';
    out += goDeepDOMtree(sNode,level+1);
    if (sNode.childNodes && sNode.childNodes.length) out += spacing;
  }
  out += '</'+node.nodeName+'>\n';
  return out;
} 

function showDOMtree(node) {
  var out = '<'+node.nodeName+'>\n';
  for (var i = 0; i < node.childNodes.length; i++){
    var sNode = node.childNodes[i];
    out += goDeepDOMtree(sNode,1);
  }
  out += '</'+node.nodeName+'>';
  alert(out);
}



/*

*/