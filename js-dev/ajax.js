/* file XMLHttpRequest (AJAX) implementation 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @author paziel ()
 *         Some code derived from PetriW work at anidb
 * version 1.0 (20.03.2007) - Initial Version
 * version 1.1 (30.04.2008) - Some minor cleanup
 * version 1.2 (24.04.2009) - Addition of paziek's XHR element, plus update of IE's MSXML methods.
 * version 1.3 (28.03.2012) - Addition of support for JSON
 */
jsVersionArray.push({
	"file":"ajax.js",
	"version":"1.3",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Addition of support for JSON"
});

// CORE Functions //

var isIE = false; // Are we dealing with an IE based browser?
var isHeaderNeeded = true;
var disableCaching = false; // Should we disable caching?
var disableJSONrevivers = false; // Should we disable JSON revivers?

/* Function that returns a new XMLHttpRequest object
 * @return an XMLHttpRequest object
 */
function xhttpRequest() {
	var httpRequest;
	// test for JSON support and add it if it doesn't exist
	if (!window.JSON) { // this is the basic version
		window.JSON = {  
			parse: function (sJSON) { return eval("(" + sJSON + ")"); },
			stringify: function (vContent) {
				if (vContent instanceof Object) {  
					var sOutput = "";  
					if (vContent.constructor === Array) {  
						for (var nId = 0; nId < vContent.length; sOutput += this.stringify(vContent[nId]) + ",", nId++);  
						return "[" + sOutput.substr(0, sOutput.length - 1) + "]";  
					}  
					if (vContent.toString !== Object.prototype.toString) { return "\"" + vContent.toString().replace(/"/g, "\\$&") + "\""; }  
					for (var sProp in vContent) { sOutput += "\"" + sProp.replace(/"/g, "\\$&") + "\":" + this.stringify(vContent[sProp]) + ","; }  
					return "{" + sOutput.substr(0, sOutput.length - 1) + "}";  
				}  
				return typeof vContent === "string" ? "\"" + vContent.replace(/"/g, "\\$&") + "\"" : String(vContent);  
			}
		}; 
		disableJSONrevivers = true; // basic version
	}
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
 * @param method method (supported: xml, text, json)
 * @param data The Data to send to the URL
 * @param reviver An optional reviver function for parsing JSON data before sending it to the handler
 * @return void
 */
function doRequest(obj, url, handler, override, method, data, reviver) {
	if (!obj) return;
	try {
		if(typeof obj.responseText === 'undefined') { // Checkes if we got XHR object, or its wrapper. Does this check works cross-browser? It should.
			var xhr = obj.xhr;
		} else var xhr = obj;
		xhr.onreadystatechange = function() { xhttpRequestReadData(obj,handler,method, reviver); };
		if (data) xhr.open('POST', url, true); // POST
		else xhr.open('GET', url, true); // GET
		if (xhr.overrideMimeType && override) xhr.overrideMimeType(override); // Setting the override type
		if (isHeaderNeeded) xhr.setRequestHeader('X-LControl','x-no-cache'); // forces disabling of local caching
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

/* Fetches a given url and then passes the result to the given handler
 * @param obj The XMLHttpRequest object, or its wrapper (in that case, XHR should be in obj.xhr)
 * @param url HTTP protcol URL
 * @param handler Function that will handle the results
 * @param override Overrides the document mime-type with given override
 * @param method method (supported: xml, text, json)
 * @param reviver An optional reviver function for parsing JSON data before sending it to the handler
 * @return void
 */
function xhttpRequestFetch(obj, url, handler, override, method, reviver) {
	doRequest(obj, url, handler, override, method, null, reviver);
}

/* Posts given data to a given url and then passes the result to the given handler
 * @param obj The XMLHttpRequest object, or its wrapper (in that case, XHR should be in obj.xhr)
 * @param url HTTP protcol URL
 * @param handler Function that will handle the results
 * @param override Overrides the document mime-type with given override
 * @param method method (supported: xml, text, json)
 * @param data The Data to send to the URL
 * @param reviver An optional reviver function for parsing JSON data before sending it to the handler
 * @return void
 */
function xhttpRequestPost(obj, url, handler, data, override, method, reviver) {
	doRequest(obj, url, handler, override, method, data, reviver)
}

/* This Function handles the status changes of the reply
 * @param obj The XMLHttpRequest object
 * @param handler The function that will handle the given obj
 * @param method Data parse method, can be one of xml (for handling xml data), text or json (For handling json data)
 * @param reviver An optional reviver function for parsing JSON data before sending it to the handler
 * @return void
 */
function xhttpRequestReadData(obj, handler, method, reviver) {
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
						} else if (method == 'json') {
							if (reviver && !disableJSONrevivers)
								data = JSON.parse(xhr.responseText, reviver);
							else
								data = JSON.parse(xhr.responseText);
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
