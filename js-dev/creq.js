jsVersionArray.push({
	"file":"creq.js",
	"version":"1.0",
	"revision":"$Revision: 2128 $",
	"date":"$Date:: 2009-01-31 15:09:06 +0000#$",
	"author":"$Author: fahrenheit $",
	"changelog":"Initial version"
});

window.onload = (function()
{
	InitDefault();
	my_mark = false;
	var form = document.getElementById('type.del')
	if( form )
	{
		add_check('do.del.notify.all');
		form.onsubmit = do_submit;
	}
});
function do_mark()
{
	my_mark = true;
}
function do_submit(event)
{
	if( my_mark)
	{
		my_mark = false;
		return confirm("This will delete ALL notifies. Continue?";
	}
	return true;
}
function add_check(name)
{
	var elements = document.getElementsByName(name);
	for (var i = 0, elem = null; (elem = elements[i]); i++)
		elem.onclick = do_mark;	
}