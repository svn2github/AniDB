/* file XMLHttpRequest (AJAX) implementation 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @author paziel ()
 *         Some code derived from PetriW work at anidb
 * version 1.0 (20.03.2007) - Initial Version
 * version 1.1 (30.04.2008) - Some minor cleanup
 * version 1.2 (24.04.2009) - Addition of paziek's XHR element, plus update of IE's MSXML methods.
 */
jsVersionArray.push({
	"file":"ajax.js",
	"version":"1.2",
	"revision":"$Revision: 2718 $",
	"date":"$Date:: 2009-05-24 00:16:50 +0100#$",
	"author":"$Author: fahrenheit $",
	"changelog":"Addition of paziek's XHR element, plus update of IE's MSXML methods"
});

// CORE Functions //

var isIE = false; // Are we dealing with an IE based browser?
var disableCaching = false; // Should we disable caching?

/* Function that returns a new XMLHttpRequest object
 * @return an XMLHttpRequest object
 */
function xhttpRequest() {
	var httpRequest;
	if (window.XMLHttpRequest) { // Mozilla, Safari, ...
		httpRequest = new XMLHttpRequest();
	} else if (window.ActiveXObject) { // IE
		isIE = true;
		var progIDs = [ 'Msxml2.XMLHTTP.6.0', 'Msxml2.XMLHTTP.3.0', 'Msxml2.XMLHTTP', 'Microsoft.XMLHTTP' ]; // MSXML5.0, MSXML4.0 and Msxml2.DOMDocument all have issues
        for (var i = 0; i < progIDs.length; i++) {
            try {
                return new ActiveXObject(progIDs[i]);
            }
            catch (ex) {
            }
        }
	}
	if (!httpRequest) {
		if (seeDebug) alert('Error\nCould not create an XMLHTTP instance.');
		return false;
	}
	return httpRequest;
}

/* Does the request
 * @param obj The XMLHttpRequest object, or its wrapper (in that case, XHR should be in obj.xhr)
 * @param url HTTP protcol URL
 * @param handler Function that will handle the results
 * @param override Overrides the document mime-type with given override
 * @param data The Data to send to the URL
 * @param method method
 * @return void
 */
function doRequest(obj, url, handler, override, method, data) {
	if (!obj) return;
	try {
		if(typeof obj.responseText === 'undefined') { // Checkes if we got XHR object, or its wrapper. Does this check works cross-browser? It should.
			var xhr = obj.xhr;
		} else var xhr = obj;
		xhr.onreadystatechange = function() { xhttpRequestReadData(obj,handler,method); };
		if (data) xhr.open('POST', url, true); // POST
		else xhr.open('GET', url, true); // GET
		if (xhr.overrideMimeType && override) xhr.overrideMimeType(override); // Setting the override type
		if (disableCaching) xhr.setRequestHeader('Cache-Control','no-cache'); // disables caching
		if (data) {
			xhr.setRequestHeader('Content-Type','application/x-www-form-urlencoded'); // Setting the correct header
			xhr.send(data);
		} else {
			if (isIE) xhr.setRequestHeader('Content-Type','application/xml'); // IE compatibility
			xhr.send(null);
		}
	} catch(e) {
		alert('error: '+ e.description+'\nurl: '+url);
	}
}

/* Fetches a given url and then passes the result to the given handler */
function xhttpRequestFetch(obj, url, handler, override, method) {
	doRequest(obj, url, handler, override, method, null);
}

/* Posts given data to a given url and then passes the result to the given handler */
function xhttpRequestPost(obj, url, handler, data, override, method) {
	doRequest(obj, url, handler, override, method, data)
}

/* This Function handles the status changes of the reply
 * @param obj The XMLHttpRequest object
 * @param handler The function that will handle the given obj
 * @return void
 */
function xhttpRequestReadData(obj, handler, method) {
	if (!obj) { xhttpRequestUpdateStatus('object is null...'); return; }
	var rootDoc = 'root';
//	try {
		if(typeof obj.responseText === 'undefined') { // Checkes if we got XHR object, or its wrapper. Does this check works cross-browser? It should.
			var xhr = obj.xhr;
		} else var xhr = obj;
		switch (xhr.readyState) {
			case 2: xhttpRequestUpdateStatus('requesting data...',25); break;
			case 3: xhttpRequestUpdateStatus('receiving data...',50); break;
			case 4:
				xhttpRequestUpdateStatus('data transfer completed...',100);
				switch (xhr.status) {
					case 0:
					case 200:
					case 304:
						var data = null;
						if (!method || method == 'xml') {
							if (window.XMLHttpRequest) data = xhr.responseXML; // Mozilla and likes
							else { // The original IE implementation
								var progIDs = [ 'Msxml2.DOMDocument.6.0', 'Msxml2.DOMDocument.3.0']; // MSXML5.0, MSXML4.0 and Msxml2.DOMDocument all have issues
								for (var i = 0; i < progIDs.length; i++) {
									try {
										data = new ActiveXObject(progIDs[i]);
										data.loadXML(xhr.responseText);
										break;
									}
									catch (ex) {
									}
								}
							}
						} else if (method == 'text') {
							data = xhr.responseText;
						}
						if (handler) handler(data, obj);
					break;
					default:
						alert('There was a problem with the request.\nServer returned: '+xhr.status+': '+xhr.statusText);
				break;
			}
			break;
		}
/*	} catch ( e ) {
		alert('error: '+ e.description);
	}*/
}

/* This function is used to update the status of the Request
 * @param text Text to show
 * @param percentage Percentage done
 * @return void
 */
function xhttpRequestUpdateStatus(text,percentage) {
	try {
		if (document.getElementById('statusBox')) {
			if (globalStatus && globalStatus.container) {
				globalStatus.loadingbar_color = 'red';
				globalStatus.updateBarWithText(text,percentage,'Data request completion: ');
				globalStatus.loadingbar_color = 'green';
			} else {
				var statusBox = document.getElementById('statusBox');
				if (statusBox.firstChild) statusBox.removeChild(statusBox.firstChild);
				statusBox.appendChild(document.createTextNode(text));
			}
		}
		else window.status = text;
	} catch (e) { }
}
