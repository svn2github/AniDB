function writeSpans(){
	numpages = Math.ceil(vis.length / pref_animes_per_page);
	if (curpage < 1){
		document.getElementById("prev").innerHTML = 'prev';
		document.getElementById("prev1").innerHTML = 'prev';
	} else {
		document.getElementById("prev").innerHTML = spanPrev;
		document.getElementById("prev1").innerHTML = spanPrev;	
	}
	if (numpages > 1){
		document.getElementById("next").innerHTML = spanNext;
		document.getElementById("next1").innerHTML = spanNext;
	} else {
		document.getElementById("next").innerHTML = 'next';
		document.getElementById("next1").innerHTML = 'next';	
	}
}

function spanStats(){
	var cur_page = curpage + 1;
	//document.getElementById("stats_vis.innerHTML = '<font size="1">&nbsp;&nbsp;&nbsp;animes filtered: ' + vis.length + ' of '+aids.length+'</font><br>'
	document.getElementById("stats_pages").innerHTML = '<font size="1">&nbsp;&nbsp;&nbsp;page: ' + cur_page +' of '+numpages+'</font>'
}

function nextPage(){
	if (curpage < numpages){
		last_vids = cur_vids;
		last_nids = cur_nids;
		cur_vids = cur_nids;
		if ( ( cur_nids + pref_animes_per_page ) > vis.length ){ cur_nids = vis.length } else { cur_nids = cur_nids + pref_animes_per_page }
		curpage = curpage + 1;
		writeAnime(vis,cur_vids,cur_nids);
		document.getElementById("next").innerHTML = spanNext;
		document.getElementById("next1").innerHTML = spanNext;
		document.getElementById("prev").innerHTML = spanPrev;
		document.getElementById("prev1").innerHTML = spanPrev;
		if ((curpage + 1) == numpages){
			document.getElementById("next").innerHTML = 'next';
			document.getElementById("next1").innerHTML = 'next';
		} else {
			document.getElementById("next").innerHTML = spanNext;
			document.getElementById("next1").innerHTML = spanNext;
		}
	} else {
		document.getElementById("next").innerHTML = 'next';
		document.getElementById("next1").innerHTML = 'next';
		document.getElementById("prev").innerHTML = spanPrev;
		document.getElementById("prev1").innerHTML = spanPrev;
	}
	spanStats();
}

function prevPage(){
	if (curpage > 0){
		var dif = 0;
		cur_vids = last_vids;
		cur_nids = last_nids;
		curpage = curpage - 1;
		writeAnime(vis,cur_vids,cur_nids);
		if (cur_vids - pref_animes_per_page < 0) { 
			dif = ( pref_animes_per_page - ( cur_vids - pref_animes_per_page )) 
		} else {
			dif = pref_animes_per_page
		}
		last_vids = cur_vids - dif;
		last_nids = cur_nids - dif;
		document.getElementById("prev").innerHTML = spanPrev;
		document.getElementById("prev1").innerHTML = spanPrev;
		document.getElementById("next").innerHTML = spanNext;
		document.getElementById("next1").innerHTML = spanNext;
		if ((curpage - 1) < 0){
			document.getElementById("prev").innerHTML = 'prev';
			document.getElementById("prev1").innerHTML = 'prev';
		} else {
			document.getElementById("prev").innerHTML = spanPrev;
			document.getElementById("prev1").innerHTML = spanPrev;
		}
	} else {
		document.getElementById("prev").innerHTML = 'prev';
		document.getElementById("prev1").innerHTML = 'prev';
		document.getElementById("next").innerHTML = spanNext;
		document.getElementById("next1").innerHTML = spanNext;
	}
	spanStats();
}

function showMyList(){
	if (print_sel == 0){
		showT('ttop');
		showT('dttop');
		showT('tbottom');
		hideT('wtbottom');
	} else {
		showT('ttop');
		hideT('dttop');
		hideT('tbottom');
		hideT('wtbottom');
	}
	document.getElementById("mlst").style.backgroundColor = "#F0F0EB"
	if (check_wish == 1){
		document.getElementById("wlst").style.backgroundColor = "#FFFFFF"
	}
	if(wishlist_sel == 1){ // restore current mylist view settings
		vis 		= w_vis;
		cur_vids 	= w_cur_vids;
		cur_nids 	= w_cur_nids;
		curpage		= w_curpage;
		numpages	= w_numpages;
		wishlist_sel = 0;
		//alert('curpage = ['+curpage+']\nnumpages = ['+numpages+']');
	}
	writeAnime(vis,cur_vids,cur_nids);
	writeSpans();
}

function writeAnime(visible,vids,nids){
	var output = "";
	var tmp = nids
	if (nids > vis.length){ nids = vis.length };
	var k = 0;
	if (vis.length>0){
		output = '<div id=page'+k+' style="display:">'
		for (i = vids; i < nids; i++){
			anime = eval ( "a" + visible [ i ] );
			if (pref_title == 0){ var aname = anime.name };
			if (pref_title == 1){ 
				if ( anime.japkanji != '-'){ var aname = anime.japkanji } else { var aname = anime.name }
			};
			if (pref_title == 2){ 
				if ( anime.eng != '-'){ var aname = anime.eng } else { var aname = anime.name }
			};
			if (pref_title2opt == 1){
				if (pref_title2 == 0){ 
					if ( anime.eng != '-'){ var bname = anime.eng } else { var bname = anime.name }
				};
				if (pref_title2 == 1){ 
					if ( anime.japkanji != '-'){ var bname = anime.japkanji } else { var bname = anime.name }
				};
				if (pref_title2 == 2){ var bname = anime.name };
				aname = aname + ' ('+bname+')';
			}
			if (anime.isHentai){
				if (pref_hentai == 1){
					output += '<div id=a' + visible[i] +' style="display:">'
				} else {
					output += '<div id=a' + visible[i] +' style="display:none">'
				}
			} else {
				output += '<div id=a' + visible[i] +' style="display:">'
			}
			output += '<table width="100%" cellpadding="0" cellspacing="0" class="lines">'
			output += '<tr height="20px">'
			output += '<td class="anime_table">&nbsp;<a href="javascript:showAnimePage(\'anime.htm?=a'+anime.id+'\');"'
			if (pref_title2opt == 2){
				if (pref_title2 == 0){ 
					if ( anime.eng != '-'){ var bname = anime.eng } else { var bname = anime.name }
				};
				if (pref_title2 == 1){ 
					if ( anime.japkanji != '-'){ var bname = anime.japkanji } else { var bname = anime.name }
				};
				if (pref_title2 == 2){ var bname = anime.name };
				output += ' title="Optional Title: '+bname+'"'
			}
			output += '>' + aname + '</a> '
			output += '</td>'
			output += '<td align="center" class="anime_table" width="25">'
			if (anime.isWishListed){
				output += '<img src="images/wish_pri_'+anime.wishlist_priority+'.gif" width="15" height="13" title="on Wishlist: '+anime.wishlist_type_name+' | priority: '+anime.wishlist_priority_name+''
				if (anime.wishlist_comment != ""){
					output += ' | comment: '+anime.wishlist_comment+''
				}
				output += '" border="0">'
			}
			output += '</td>'
			output += '<td align="center" class="anime_table" width="25">'
			if (anime.hasAwards){
				output += '<img src="images/award_1.gif" width="15" height="13" title="This anime has awards" border="0">'
			}
			output += '</td>'
			output += '<td align="center" class="anime_table" width="25"><img src="images/state_'+ anime.state +'.gif" width="15" height="13" Title="State: '+ anime.statename +'"></td>'
			output += '<td align="center" class="anime_table" width="70">'
			if (anime.isComplete){
				 output += '<b> '+ anime.myEps +' / '+ anime.eps +'</b>'
			} else { 
				output += ' ' + anime.myEps +' / '+ anime.eps
			}
			if (anime.mySEps > 0 && anime.mySEps == anime.myWSEps){
				output += ' <b>+' + anime.mySEps +'</b>'
			} else {
				if (anime.mySEps > 0){
					output += ' +' + anime.mySEps
				}
			}
			output += '</td>'
			output += '<td align="center" class="anime_table" width="70">'
			if (anime.isWatched){
				output += '<font color="#006600"><b> '+ anime.myWeps +' / '+ anime.eps +'</b></font>'
			} else { 
				output += ' ' + anime.myWeps +' / '+ anime.eps;
			}
			if (anime.myWSEps > 0 && anime.mySEps == anime.myWSEps){
				output += ' <font color="#006600"><b>+' + anime.myWSEps +'</b></font>'
			} else {
				if (anime.myWSEps > 0){
					output += ' +' + anime.myWSEps
				}
			}
			output += '</td>'
			output += '<td align="center" class="anime_table" width="100"> ' + anime.typename + ' </td>'
			output += '<td align="center" class="anime_table" width="70"> ' + anime.rating + ' ('+ anime.votes +') </td>'
			if (pref_trade == 1){
				var in_list = 0;
				if (anime.isWishListed){
					for (wk = 0; wk< test_wishlist.length; wk++){
						if (test_wishlist[wk] == anime.id){
							in_list = 1;
						}
					}
					output += '<td align="center" class="anime_table" width="25"><input type="checkbox" id="'+anime.id+'_sel" value="'+anime.size+'" onClick="javascript:wishlist_add('+anime.id+')"'				
				} else {
					for (tk = 0; tk< test_trade.length; tk++){
						if (test_trade[tk] == anime.id){
							in_list = 1;
						}
					}
					output += '<td align="center" class="anime_table" width="25"><input type="checkbox" id="'+anime.id+'_sel" value="'+anime.size+'" onClick="javascript:trade_add('+anime.id+')"'
				}
				if (in_list == 1){
					output += ' checked'
				}
				output += '></td>'
			}
			output += '</tr>'
			output += '</table>'
			output += '</div>'
		}
		vids = vids + pref_animes_per_page;
		tmp = nids + pref_animes_per_page;
		if ( tmp > visible.length ) { 
			nids = visible.length;
			tmp = nids;
		} 
		if ( tmp <= visible.length ) { 
			nids = tmp;
		}
		output += '</div>'
	} else {
		var output = '<div id=page1 style="display:">'
		output += '<table width="100%" cellpadding="0" cellspacing="0">'
		output += '<tr height="20px"><td><hr noshade color="#F0F0EB"></td><tr>'
		output += '</table>'
		output += '</div>'
	}
	document.getElementById("animeItable").innerHTML = output;
	spanStats();
}

function trade(){
	var trade = "";
	for (i = 0; i < vis.length; i++){
		var target = document.getElementById(vis[i] + '_sel')
		var checkbox = eval(target);
		if (checkbox){
			if (checkbox.checked){
				var anime = eval ( "a" + vis[i]);
				trade += anime.id + ",";
			}
		}
	}
	trade += "0";
	setCookie('trade',trade);
	//alert('[trade box]\n\n\tsize = MBs\n\tanimes:\n\t'+trade);
	//popup('trade.htm','Search','500','370');
}

function showStatsPage(){
	hideT('ttop');
	hideT('dttop');
	hideT('tbottom');
	hideT('wtbottom');
	var cur_page = curpage + 1;
	if (numpages == 0){ var pages = 1 } else { var pages = numpages }
	var output = '<table border="0" cellspacing="0" cellpadding="0">'
	output += '<tr>'
	output += '<td width="98" height="10"></td>'
	output += '<td width="100" height="10"></td>'
	if (check_wish == 1){
		output += '<td width="100" height="10"></td>'
	}
	if (pref_trade == 1){
		output += '<td width="100" height="10"></td>'
	}
	output += '<td bgcolor="#F0F0EB" width="100" height="10"></td>'
	output += '</tr>'
	output += '</table>'
	output += '<table class="anime_table" width="100%" cellspacing="0" cellpadding="0">'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right">&nbsp;</td>'
	output += '<td>&nbsp;</td>'
	output += '<td>&nbsp;</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">User:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += '<font size=1"><b>' + user + '</b><br>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Created:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += '<a title="Date: ' + cdate + '">' + cdate_short + '</a><br>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Template ver:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += template_ver
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Animes in list:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += animecount+'<br>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Episodes in list:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += epcount+'<br>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Files in list:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += filecount+'<br>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Size of list:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += '<a title="Size: ' + bytecount + 'bytes">' + bytecount_h +'</a><br>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">&nbsp;</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Print:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += '<a title="Format the mylist page for printing" alt="print" href="javascript:print(1);">format myList for printing</a>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">&nbsp;</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Animes filtered:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += vis.length + ' of '+aids.length+'<br>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Pages:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += cur_page +' of '+pages+'<br>'
	output += '</td>'
	output += '</tr>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">&nbsp;</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Help:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td>'
	output += '<a href="javascript:popup(\'help.htm?=mylist\',\'Help\',\'700\',\'350\');" title="Help">show help page</a><br>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Preferences:</td>'
	output += '<td width="5">&nbsp;</td>'
	output += '<td height="100%">'
	output += '<iframe name=mainframe width="100%" height="100%" src="preferences.htm" scrolling=auto frameborder=0 style="height:360px; z-index:0; overflow: auto"><div align="right"></div></iframe>'
	output += '</td>'
	output += '</tr>'
	output += '</table>'
	document.getElementById("animeItable").innerHTML = output;
	spanStats();
}

function showAnimePage(target){
	hideT('ttop');
	hideT('dttop');
	hideT('tbottom');
	hideT('wtbottom');
	if (target == 'blank.htm' && cur_anime != '\'blank.htm\''){} else { cur_anime = target; }
	var output = '<table border="0" cellspacing="0" cellpadding="0">'
	output += '<tr>'
	output += '<td width="98" height="10"></td>'
	output += '<td width="100" height="10"></td>'
	if (check_wish == 1){
		output += '<td width="100" height="10"></td>'
	}
	if (pref_trade == 1){
		output += '<td width="100" height="10"></td>'
	}
	output += '<td width="100" height="10"></td>'
	output += '<td bgcolor="#F0F0EB" width="100" height="10"></td>'
	output += '</tr>'
	output += '</table>'
	output += '<table class="anime_table" width="100%" cellspacing="0" cellpadding="0">'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right">&nbsp;</td>'
	output += '</tr>'
	output += '<tr>'
	output += '<td>'
	output += '<iframe name=mainframe width="100%" height="100%" src="'+cur_anime+'" scrolling=auto frameborder=0 style="height:550px; z-index:0; overflow: auto"><div align="right"></div></iframe>'
	output += '</td>'
	output += '</tr>'
	output += '<tr heigth="20">'
	output += '<td width="100" valign="top" align="right">&nbsp;</td>'
	output += '</tr>'
	output += '</table>'
	sel_anime = 1;
	document.getElementById("animeItable").innerHTML = output;
	spanStats();
}
