function InitHint2()
{
	if(document.getElementById && document.getElementsByTagName) // && CSS works, really
	{
		var basediv = document.getElementById("Base");
		var root = document.createElement('div');
		root.className = "hint2_jscontrols";
		basediv.insertBefore(root, basediv.firstChild);
		var tag = root.appendChild(document.createElement('h4'));
		tag.appendChild(document.createTextNode('Advanced Options'));
		var list = root.appendChild(document.createElement('ul'));
		var taglist = document.getElementsByTagName('div');
		for (var i = 0; i < taglist.length; i++)
		{
			if (taglist[i].className == "collapsible")
			{
				tag = list.appendChild(document.createElement('li'));
				tag = tag.appendChild(document.createElement('a'));
				tag.onclick = UnCollapseOption;
				tag.title = taglist[i].id;
				tag.appendChild(document.createTextNode("Show "+tag.title+" Options"));
				taglist[i].className = "collapse";
			}
		}
		tag = list.appendChild(document.createElement('li'));
		tag = tag.appendChild(document.createElement('a'));
		tag.onclick = CollapseNotes;
		tag.title = "Descriptions";
		tag.appendChild(document.createTextNode('Hide '+tag.title));
		basediv.className += " jason";
	}
	InitDefault();
}

function CollapseOption()
{
	document.getElementById(this.title).className = "collapse";
	this.firstChild.nodeValue = "Show " + this.title + " Options";
	this.onclick = UnCollapseOption;
}
function UnCollapseOption()
{
	document.getElementById(this.title).className = "collapsible";
	this.firstChild.nodeValue = "Hide " + this.title + " Options";
	this.onclick = CollapseOption;
}

function CollapseNotes()
{
	document.getElementById("Base").className += " nonote";
	this.firstChild.nodeValue = "Show " + this.title;
	this.onclick = UnCollapseNotes;
}
function UnCollapseNotes()
{
	document.getElementById("Base").className = document.getElementById("Base").className.replace(/ nonote|nonote/i, "");
	this.firstChild.nodeValue = "Hide " + this.title;
	this.onclick = CollapseNotes;
}

//window.onload = InitHint2;
addLoadEvent(InitHint2);