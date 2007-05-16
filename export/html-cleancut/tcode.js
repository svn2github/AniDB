function trade_add(aid){
	
	var target = document.getElementById(aid + '_sel');
	var checkbox = eval(target);
	var in_list = 0;
	var in_list_id = 0;
	for (i = 0; i< test_trade.length; i++){
		if (test_trade[i] == aid){
			in_list = 1;
			in_list_id = i;
		}
	}
	if (checkbox){
		if (checkbox.checked){
			var anime = eval ( "a" + aid);
			if (in_list == 0){
				test_trade.push(aid);
			}
		} else {
			if (in_list == 1){
				test_trade.splice(in_list_id,1);
			}
		}
	}
}

function wishlist_add(aid){
	
	var target = document.getElementById(aid + '_sel');
	var checkbox = eval(target);
	var in_list = 0;
	var in_list_id = 0;
	for (i = 0; i< test_wishlist.length; i++){
		if (test_wishlist[i] == aid){
			in_list = 1;
			in_list_id = i;
		}
	}
	if (checkbox){
		if (checkbox.checked){
			var anime = eval ( "a" + aid);
			if (in_list == 0){
				test_wishlist.push(aid);
			}
		} else {
			if (in_list == 1){
				test_wishlist.splice(in_list_id,1);
			}
		}
	}
}

function sendMail(){
	var output = "";
	var woutput = "";
	var foutput = "";
	var trade_val = 0;
	for (i = 0; i < test_trade.length; i++){
		var anime = eval ( "a" + test_trade[i] );
		trade_val += parseInt(anime.size);
		output += anime.name+' ('+anime.size+')\n'
	}
	var wishlist_val = 0;
	for (i = 0; i < test_wishlist.length; i++){
		var anime = eval ( "a" + test_wishlist[i] );
		wishlist_val += parseInt(anime.size);
		woutput += anime.name+' ('+anime.size+')\n'
	}
	var test_file = getCookie('f_trade');
	test_file = test_file.split(";");
	for (i = 0; i < test_file.length; i++){
		foutput += test_file[i] + '\n'
	}
	var to = tmail;
	var trader_name = document.getElementById("form1").trader_name.value;
	var trader_mail = document.getElementById("form1").trader_mail.value;
	var trade_medium = document.getElementById("form1").sel_medium.value;
	var trader_items = document.getElementById("form1").trader_items.value;
	var subject = 'Trade request from ' + trader_name ;
	var body    = '';
	
	body += trader_name + ' <'+trader_mail+'> has requested a trade for these animes:\n\n' + output + '\n\nthe animes are arround '+trade_val+'MBs which amounts to: '+ Math.ceil(trade_val / 703) +' CD(s) or '+Math.ceil(trade_val / 4483)+' DVD(s).\n'
	if ( foutput.length > 0 ){
		body += trader_name + ' also has requested a trade for these files:\n\n' + foutput + '\n'
	}
	body += trader_name + ' has requested that the trade be made using ' + trade_medium + '.\n\nThe trade is to be made in exchange of the folowing items:\n' + trader_items + '\n\n'
	if ( woutput.lenght > 0 ){
		body += trader_name + ' also has the following animes of the wishlist\n\n'+ woutput
	}
	body += '\n\nmade using cleancut template, trade function.'
	
	if (confirm('to: ' + to +'\nsubject: '+ subject +'\nbody:\n'+body)){
		var mail = 'mailto:'+escape(to)+'?&subject='+escape(subject)+'&Body='+escape(body)
		document.location.href = mail;
	}
}

function showTradePage(){
var test_file = getCookie('f_trade');
test_file = test_file.split(";");
var trade_val = 0;
for (i = 0; i < test_trade.length; i++){
	var anime = eval ( "a" + test_trade[i] );
	trade_val += parseInt(anime.size);
}
var wishlist_val = 0;
for (i = 0; i < test_wishlist.length; i++){
	var anime = eval ( "a" + test_trade[i] );
	wishlist_val += parseInt(anime.size);
}
hideT('ttop');
hideT('dttop');
hideT('tbottom');
hideT('wtbottom');
var output = '<form id="form1" method="post" action="javascript:sendMail();">'
output += '<table border="0" cellspacing="0" cellpadding="0">'
output += '<tr>'
output += '<td width="98" height="10"></td>'
output += '<td width="100" height="10"></td>'
if (check_wish == 1){
	output += '<td width="100" height="10"></td>'
}
output += '<td bgcolor="#F0F0EB" width="100" height="10"></td>'
output += '<td width="100" height="10"></td>'
output += '</tr>'
output += '</table>'
output += '<table class="anime_table" width="100%" cellspacing="0" cellpadding="0">'
output += '<tr>'
output += '<td width="175" valign="top" align="right">&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
if (tmail != ''){
output += '<tr>'
output += '<td width="175" valign="top" align="right" bgcolor="#F0F0EB" height="20">Total Size of Trade:</td>'
output += '<td width="5">&nbsp;</td>'
output += '<td valign="top"><font size="1">'+trade_val+' MBs<br>(arround '+ Math.ceil(trade_val / 703) +' CD(s) or '+Math.ceil(trade_val / 4483)+' DVD(s))</font><br></td>'
output += '</tr>'
output += '<tr>'
output += '<td width="175" valign="top" align="right" bgcolor="#F0F0EB" height="20">Animes Traded:</td>'
output += '<td>&nbsp;</td>'
output += '<td valign="top">'
for (i = 0; i < test_trade.length; i++){
	var anime = eval ( "a" + test_trade[i] );
	output += anime.name+' ('+anime.size+')<br>'
}
output += '</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="175" valign="top" align="right" bgcolor="#F0F0EB" height="20">Files Traded:</td>'
output += '<td>&nbsp;</td>'
output += '<td valign="top">'
for (i = 0; i < test_file.length; i++){
	output += test_file[i] + '<br>'
}
output += '</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="175" align="right" bgcolor="#F0F0EB" height="20">Recording Medium for Trade:</td>'
output += '<td>&nbsp;</td>'
output += '<td><select id="sel_medium" size="1">'
output += '<option selected value="CDs">----</option>'
output += '<option value="CDs">CD</option>'
output += '<option value="DVDs">DVD</option>'
output += '</select>'
output += '</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="175" align="right" bgcolor="#F0F0EB" height="20">&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="175" valign="top" align="right" bgcolor="#F0F0EB" height="20">Your name:</td>'
output += '<td>&nbsp;</td>'
output += '<td><input type="text" id="trader_name" size="50"></td>'
output += '</tr>'
output += '<tr>'
output += '<td width="175" valign="top" align="right" bgcolor="#F0F0EB" height="20">Your e-mail:</td>'
output += '<td>&nbsp;</td>'
output += '<td><input type="text" id="trader_mail" size="50"></td>'
output += '</tr>'
output += '<tr>'
output += '<td width="175" valign="top" align="right" bgcolor="#F0F0EB" height="20">Your animes for trade:</td>'
output += '<td>&nbsp;</td>'
output += '<td><textarea id="trader_items" cols="40" rows="5"></textarea></td>'
output += '</tr>'
output += '<tr>'
output += '<td width="175" valign="top" align="right" bgcolor="#F0F0EB" height="20">Your animes from my wishlist:</td>'
output += '<td>&nbsp;</td>'
output += '<td>'
for (i = 0; i < test_wishlist.length; i++){
	var anime = eval ( "a" + test_wishlist[i] );
	output += anime.name+' ('+anime.size+')<br>'
}
output += '</td>'
output += '</tr>'
output += '<tr>'
output += '<td align="right" valign="top" bgcolor="#F0F0EB" width="175" height="20">Action:</td>'
output += '<td>&nbsp;</td>'
output += '<td align="left"><input type="submit" id="Submit" value="Send Mail"></td>'
output += '</tr>'
} else {
output += '<tr>'
output += '<td width="50" align="right" valign="top" bgcolor="#F0F0EB" height="20">Alert:</td>'
output += '<td>&nbsp;</td>'
output += '<td>Sorry but the owner of this list has not configured the Trade function.<br>If you are the owner of this list, read the readme file to help you configure this function.</td>'
output += '</tr>'
}
output += '</table>'
output += '</form>'
document.getElementById("animeItable").innerHTML = output;
}