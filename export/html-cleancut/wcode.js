function WSAwsStatus(){
	var arg = document.getElementById("wish_status_sel").value
	wsStatus(arg);
}

function WSThideState(){
	var arg = document.getElementById("wish_state_sel").value
	WShideState(arg);
}


function showWishlistPage(){
	showT('ttop');
	showT('dttop');
	hideT('tbottom');
	showT('wtbottom');
	document.getElementById("mlst").style.backgroundColor = "#FFFFFF"
	document.getElementById("wlst").style.backgroundColor = "#F0F0EB"
	if (wishlist_sel == 0){
		w_vis 		= vis;
		w_cur_vids 	= cur_vids;
		w_cur_nids 	= cur_nids;
		w_curpage 	= curpage;
		w_numpages	= numpages;
		curpage		= 0;
		
		wishlist_sel = 1;
		//alert('curpage = ['+w_curpage+']\nnumpages = ['+w_numpages+']');
	}
	dsStatus(5);
	
	var output = '<div id=swtbottom style="display:">';
	output += '<table width="100%" border="0" cellspacing="0" cellpadding="0">'
	output += '<tr height="20px">' 
	output += '<td align="left">'
	output += '<table border="0" cellspacing="0" cellpadding="0" class="anime_table">'
	output += '<tr>'
	output += '<td width="30" height="20" align="center" valign="top"><a href="javascript:toggle(\'wish_filters_min\');toggle(\'wish_filters_max\');toggle(\'wish_fil_mais\');toggle(\'wish_fil_menos\');"><span id=wish_fil_mais style="display:"><img src="images/plus.gif" width="15" height="13" border="0"></span><span id=wish_fil_menos style="display:none"><img src="images/minus.gif" width="15" height="13" border="0"></span></a></td>'
	output += '<td>'
	output += '<div id=wish_filters_min style="display:">'
	output += '<table border="0" cellspacing="0" cellpadding="0" class="anime_table">'
	output += '<tr valign="top">'
	output += '<td width="175" valign="middle">Show: '
	output += '<select id="wish_status_sel" size="1" class="forms" onChange="javascript:WSAwsStatus();">'
	output += '<option value="4" selected>All</option>'
	output += '<option value="0">Unknown / Unfiled</option>'
	output += '<option value="1">To watch</option>'
	output += '<option value="2">To get</option>'
	output += '<option value="3">Blacklist</option>'
	output += '</select>'
	output += '</td>'
	output += '<td width="175" valign="middle">Priority: '
	output += '<select id="wish_state_sel" size="1" class="forms" onChange="javascript:WSThideState();">'
	output += '<option value="all" selected>All</option>'
	output += '<option value="low">Low</option>'
	output += '<option value="med">Medium</option>'
	output += '<option value="high">High</option>'
	output += '</select>'
	output += '</td>'
	output += '</tr>'
	output += '</table>'
	output += '</div>'
	output += '<div id=wish_filters_max style="display:none">'
	output += '<table border="0" cellspacing="0" cellpadding="0" class="anime_table">'
	output += '<tr>'
	output += '<td valign="top" width="175">Show:<font size="1"><br>'
	output += '&nbsp;&nbsp;&nbsp;<a href="javascript:wsStatus(0);" title="Click to Show/Hide animes by unknown/unfiled status">unknown/unfliled<br></a>'
	output += '&nbsp;&nbsp;&nbsp;<a href="javascript:wsStatus(1);" title="Click to Show/Hide animes by to watch status">to watch</a><br>'
	output += '&nbsp;&nbsp;&nbsp;<a href="javascript:wsStatus(2);" title="Click to Show/Hide animes by to get status">to get<br></a>'
	output += '&nbsp;&nbsp;&nbsp;<a href="javascript:wsStatus(3);" title="Click to Show/Hide animes by blacklist status">blacklist</a><br>'
	output += '&nbsp;&nbsp;&nbsp;<a href="javascript:wsStatus(4);" title="Click to Show all animes/reset view">show all</a></font></td>'
	output += '<td valign="top" width="175">Priority:<b></b><font size="1"><b><br></b>'
	output += '&nbsp;&nbsp;&nbsp;<a href="javascript:WShideState(\'low\');"><img src="images/wish_pri_0.gif" width="15" height="13" title="Priority: Low" border="0"> - low</a><br>'
	output += '&nbsp;&nbsp;&nbsp;<a href="javascript:WShideState(\'med\');"><img src="images/wish_pri_1.gif" width="15" height="13" title="Priority: Medium" border="0"> - medium</a><br>'
	output += '&nbsp;&nbsp;&nbsp;<a href="javascript:WShideState(\'high\');"><img src="images/wish_pri_3.gif" width="15" height="13" title="Priority: High" border="0"> - high</a><br>'
	output += '&nbsp;&nbsp;&nbsp;<a href="javascript:WShideState(\'all\');"><img src="images/state_all.gif" width="15" height="13" title="Priority: All" border="0"> - all</a></font></td>'
	output += '</tr>'
	output += '</table>'
	output += '</div>'
	output += '</td>'
	output += '</tr>'
	output += '</table>'
	output += '</td>'
	output += '<td align="right" valign="top"><span id=stats_pages></span></td>'
	output += '</tr>'
	output += '</table>'
	output += '</div>';
	
	document.getElementById("wtbottom").innerHTML = output;
	spanStats();
}