/**
 * @file XMLHttpRequest (AJAX) implementation 
 * @author fahrenheit (alka.setzer@gmail.com)
 *         Some code derived from PetriW work at anidb
 * @version 1.0 (20.03.2007)
 */

// CORE Functions //

var isIE = false; // Are we dealing with an IE based browser?
var disableCaching = false; // Should we disable caching?

/**
 * Function that returns a new XMLHttpRequest object
 * @return an XMLHttpRequest object
 */
function xhttpRequest() {
  var httpRequest;
  if (window.XMLHttpRequest) { // Mozilla, Safari, ...
    httpRequest = new XMLHttpRequest();
  } else if (window.ActiveXObject) { // IE
    isIE = true;
    try {
      httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
    } catch (e) {
      try {
        httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
      } catch (e) {}
    }
  }
  if (!httpRequest) {
    if (seeDebug) alert('Error\nCould not create an XMLHTTP instance.');
    return false;
  }
  return httpRequest;
}

/**
 * Fetches a given url and then passes the result to the given handler
 * @param obj The XMLHttpRequest object
 * @param url HTTP protcol URL
 * @param handler Function that will handle the results
 * @param override Overrides the document mime-type with given override
 * @return void
 */
function xhttpRequestFetch(obj, url, handler, override) {
  if (!obj) return;
  obj.onreadystatechange = function() { xhttpRequestReadData(obj, handler); };
  obj.open('GET', url, true);
  if (obj.overrideMimeType && override) obj.overrideMimeType(override); // Setting the override type
  if (isIE) obj.setRequestHeader('Content-Type','application/xml'); // IE compatibility
  if (disableCaching) obj.setRequestHeader('Cache-Control','no-cache'); // disables caching
  obj.send(null);
}

/**
 * Posts given data to a given url and then passes the result to the given handler
 * @param obj The XMLHttpRequest object
 * @param url HTTP protcol URL
 * @param handler Function that will handle the results
 * @param data The Data to send to the URL
 * @return void
 */
function xhttpRequestPost(obj, url, handler, data) {
  if (!obj) return;
  obj.onreadystatechange = function() { xhttpRequestReadData(obj,handler); };
  obj.setRequestHeader('Content-Type','application/x-www-form-urlencoded'); // Setting the correct header
  obj.open('POST', url, true);
  obj.send(data);
}

/**
 * This Function handles the status changes of the reply
 * @param obj The XMLHttpRequest object
 * @param handler The function that will handle the given obj
 * @return void
 */
function xhttpRequestReadData(obj, handler) {
  if (!obj) { xhttpRequestUpdateStatus('object is null...'); return; }
  var rootDoc = 'root';
  //try {
    switch (obj.readyState) {
      case 2: xhttpRequestUpdateStatus('requesting data...'); break;
      case 3: xhttpRequestUpdateStatus('receiving data...'); break;
      case 4:
        xhttpRequestUpdateStatus('data transfer completed...');
        switch (obj.status) {
          case 0:
          case 200:
          case 304:
            var xmlData;
            if (window.XMLHttpRequest) xmlData = obj.responseXML; // Mozilla and likes
            else { // The original IE implementation
              xmlData = new ActiveXObject("MSXML2.DOMDocument");
              xmlData.loadXML(obj.responseText);
            }
            handler(xmlData);
            break;
          default:
            alert('There was a problem with the request.\nServer returned: '+obj.status+': '+obj.statusText);
            break;
        }
        break;
    }
  /*} catch ( e ) {
    alert('Caught Exception: ' + e.description)
  }*/
}

/**
 * This function is used to update the status of the Request
 * @param text Text to show
 * @return void
 */
function xhttpRequestUpdateStatus(text) {
  try {
    if (document.getElementById('statusBox')) {
      var statusBox = document.getElementById('statusBox');
      if (statusBox.firstChild) statusBox.removeChild(statusBox.firstChild);
      statusBox.appendChild(document.createTextNode(text));
    }
    else window.status = text;
  } catch (e) { }
}
