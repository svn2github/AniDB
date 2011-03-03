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

	if(document.getElementById('type.del'))
	{
		var btns = document.getElementsByName("do.del.notify.all");
		for (var i = 0; i < btns.length; i++) {
			btns[i].onclick = function(e) {
				return confirm("This will delete ALL notifies. Continue?");
			};
		}
	}
});

