/* file Signup auxiliar functions
 * @author AYAK ()
 * version 1.0 (24.04.2009) - Initial Version
 */
jsVersionArray.push({
	"file":"signup.js",
	"version":"1.0",
	"revision":"$Revision$",
	"date":"$Date::                           $",
	"author":"$Author$",
	"changelog":"Initial Version"
});

var seeDebug = false;
var confirmed = false;
var _username = "";

/* Function that fetches data */
function fetchData() {
	_username = document.getElementsByName('signup.user')[0].value.toLowerCase();
	var req = xhttpRequest();
	if (isLocalHost()) xhttpRequestFetch(req, 'xml/user_'+encodeURI(_username.toLowerCase())+'.xml', parseData);
	else xhttpRequestFetch(req, 'animedb.pl?show=xmln&t=search&type=user&search='+encodeURI(_username.toLowerCase()), parseData);
}

/* Function that parses data */
function parseData(xmldoc) {
	if (!xmldoc.getElementsByTagName('user')) return;
	var element = xmldoc.getElementsByTagName('user');
	var existsUser = false;
	for (var i = 0; i < element.length; i++) {
		if (element[i].getAttribute('name') != _username) continue;
		existsUser = true; 
		break; // end loop
	}
	var node = document.getElementsByName('signup.user')[0];
	var parentNode = node.parentNode;
	var testNode = document.getElementById('signup.user.result');
	if (testNode) parentNode.removeChild(testNode);
	testNode = document.createElement('img');
	testNode.id = 'signup.user.result';
	if (existsUser) {
		//document.getElementById(name).innerHTML = "<h1>User Exists</h1>";
		testNode.src = 'http://static.anidb.net/css/icons/formating/icon_error.gif';
		testNode.title = 'This username already exists, please chose another';
	}else{
		//document.getElementById(name).innerHTML = "<h1>User Does Not Exists</h1>";
		testNode.src = 'http://static.anidb.net/css/icons/formating/icon_message.gif';
		testNode.title = 'This username is free!';
	}
	parentNode.insertBefore(testNode,node.nextSibling);
}

function CreateElement(num,name){
	var el = document.createElement("span");
	el.setAttribute('id',name);
	var parent = document.getElementsByTagName("td")
	parent.item(num).appendChild(el);
}

function ConfirmPassword(num,name){
	var password = document.getElementsByName('signup.passwd').item(0).value;
	var confirmpassword = document.getElementsByName('signup.passwd2').item(0).value;
	CreateElement(num,name);
	if(password == confirmpassword ){
		//document.getElementById(name).innerHTML = "correct"
		//document.getElementById(name).setAttribute "images/correct.png";
	} else if (((password==null)||(password=="")) ||((confirmpassword == null)) || (confirmpassword == "")) {
		document.getElementById(name).style.backgroundImage = "url(images/wrong.png)";
	}else if (password != confirmpassword){
		document.getElementById(name).style.backgroundImage = "url(images/wrong.png)";
	}else{
		document.getElementById(name).style.backgroundImage = "url(images/wrong.png)";
	}
}

/*function RemoveElement(name){
	var el = document.getElementById(name);
	el.parentNode.removeChild(el);

}*/

function ConfirmMail(num,name){
	var email = document.getElementsByName("signup.email").item(0).value;
	var confirmemail = document.getElementsByName("signup.email2").item(0).value;
	regx = /^([\w]+)(.[\w]+)*@([\w]+)(.[\w]{2,3}){1,2}$/;
	if (((email!="") || (email!=null)) && ((confirmemail!="")||(confirmemail!=null))){
		CreateElement(num,name)
		if (!regx.test(confirmemail)){
			document.getElementById(name).innerHTML = "correct";
		}else if (email !=confirmemail){
			document.getElementById(name).innerHTML = "wrong";
		}else{
			document.getElementById(name).innerHTML = "";
		}
	}
}

function prepPage() {
	
}

// hook up the window onload event
addLoadEvent(prepPage);