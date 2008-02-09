function InitGenren()
{
	if(document.getElementById && document.getElementsByTagName) // && CSS works, really
	{
		var root = document.getElementById("cattree")
		var controls = document.createElement('div');
		controls.className = "controls";
		var tag = document.createElement('span');
		tag.className = 'tab';
		tag.onclick = UnCollapseAll;
		tag.appendChild(document.createTextNode("show all"));
		controls.appendChild(tag);
		controls.appendChild(document.createTextNode(" - "));
		tag = document.createElement('span');
		tag.className = 'tab';
		tag.onclick = CollapseAll;
		tag.appendChild(document.createTextNode("hide all"));
		controls.appendChild(tag);
		root.parentNode.insertBefore(controls, root);
		
		var lis = root.getElementsByTagName('li');
		for (var i = 0; i < lis.length; i++)
		{
			if (lis[i].className == "branch")
						{
							var toggle = document.createElement('a');
							if (lis[i].parentNode.parentNode.nodeName == "FORM")
							{
								toggle.className = "i_icon i_minus";
								toggle.onclick = CollapseBranch;
								toggle.title = "Collapse node branch";
								toggle.appendChild(document.createTextNode(""));
							}
							else
							{
								lis[i].className += " collapsed";
								toggle.className = "i_icon i_plus";
								toggle.onclick = UnCollapseBranch;
								toggle.title = "Expand node branch";
								toggle.appendChild(document.createTextNode(""));
							}
							lis[i].insertBefore(toggle, lis[i].firstChild);
			}
		}
		RepaintStripes(root, true);
	}
	InitDefault();
}

function CollapseAll()
{
	var lis = document.getElementById("cattree").getElementsByTagName('li');
	for (var i = 0; i < lis.length; i++)
	{
		if (lis[i].className.indexOf("branch") >= 0 && lis[i].className.indexOf("collapsed") == -1)
		{
			CollapseBranchDo(lis[i].firstChild);
		}
	}
	RepaintStripes(document.getElementById("cattree"), true);
}
function UnCollapseAll()
{
	var lis = document.getElementById("cattree").getElementsByTagName('li');
	for (var i = 0; i < lis.length; i++)
	{
		if (lis[i].className.indexOf("branch") >= 0 && lis[i].className.indexOf("collapsed") >= 0)
		{
			UnCollapseBranchDo(lis[i].firstChild);
		}
	}
	RepaintStripes(document.getElementById("cattree"), true);
}

function CollapseBranch()
{
	CollapseBranchDo(this);
	RepaintStripes(document.getElementById("cattree"), true);
}
function UnCollapseBranch()
{
	UnCollapseBranchDo(this);
	RepaintStripes(document.getElementById("cattree"), true);
}

function CollapseBranchDo(node)
{
	node.parentNode.className += " collapsed";
	node.className = node.className.replace(/i_minus/i, "i_plus");
	node.firstChild.nodeValue = "";
	node.title = "Expand node branch";
	node.onclick = UnCollapseBranch;
}
function UnCollapseBranchDo(node)
{
	node.parentNode.className = node.parentNode.className.replace(/ collapsed|collapsed/i, "");
	node.className = node.className.replace(/i_plus/i, "i_minus");
	node.firstChild.nodeValue = "";
	node.title = "Collapse node branch";
	node.onclick = CollapseBranch;
}

function RepaintStripes(olnode, odd)
{
	for (var i = 0; i < olnode.childNodes.length; i++)
	{
		if (olnode.childNodes[i].nodeName == "LI")
		{
			if (olnode.childNodes[i].className.indexOf("branch") >= 0)
			{
				odd = PaintStripe(olnode.childNodes[i].getElementsByTagName('span')[0], odd);
				if (olnode.childNodes[i].className.indexOf("collapsed") == -1)
				{
					odd = RepaintStripes(olnode.childNodes[i].getElementsByTagName('ul')[0], odd);
				}
			}
			else
			{
				odd = PaintStripe(olnode.childNodes[i], odd);
			}
		}
	}
	return odd;
}

function PaintStripe(node, odd)
{
	if (odd)
	{
		if (node.className.indexOf("g_odd") == -1) node.className += " g_odd";
		return false;
	}
	else
	{
		if (node.className.indexOf("g_odd") >= 0) node.className = node.className.replace(/ g_odd|g_odd/i, "");
		return true;
	}
}

// window.onload = InitGenren;
addLoadEvent(InitGenren);
