window.onload = (function(){
	InitDefault();
	my_mark = false;
	var form = document.getElementById('type.del')
	if( form ){
		add_check('creq.add');
		add_check('creq.add.close');
		add_check('creq.add.next');
		form.onsubmit = do_submit;
	}
});
function do_mark(){
	my_mark = true;
}
function do_submit(event){
	if( my_mark){
		my_mark = false;
		return event.ctrlKey || confirm("This will delete the db entry. Continue?\n(Use Ctrl to override)");
	}
	return true;
}
function add_check(name){
	var elements = document.getElementsByName(name);
	for (var i = 0, elem = null; (elem = elements[i]); i++)
		elem.onclick = do_mark;	
}