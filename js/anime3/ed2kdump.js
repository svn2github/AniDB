/* file common anime/episode/files interfaces and functions
 * @author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (31.01.2009) - Initial version
 */
jsVersionArray.push({
	"file":"anime3/ed2kdump.js",
	"version":"1.0",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"initial version"
});

function checkNumberOfLines() {
	var MAX_LINES = 100;
	var numOfLines = this.value.match(/(.*)\n/g);
	if (numOfLines != null && numOfLines.length > MAX_LINES) 
		this.value = this.value.substring(0,this.value.indexOf(numOfLines[MAX_LINES])-1);
}

/* This function prepares the mylist page for use with my scripts */
function prepPage() {
	var uriObj = parseURI();
	if (uriObj['ajax'] && uriObj['ajax'] == 0) return; // in case i want to quickly change ajax state
	var textArea = getElementsByName(document.getElementsByTagName('textarea'), 'ed2kdump', false)[0];
	if (!textArea) return;
	textArea.onkeyup = checkNumberOfLines;
}

// hook up the window onload event
addLoadEvent(prepPage);