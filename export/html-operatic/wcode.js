function writeSpans(){
	numpages = Math.ceil(vis.length / pref_animes_per_page);
	if (curpage < 1){
		document.getElementById("wprev").innerHTML = NspanPrev;
		document.getElementById("wprev1").innerHTML = NspanPrev;
	} else {
		document.getElementById("wprev").innerHTML = spanPrev;
		document.getElementById("wprev1").innerHTML = spanPrev;	
	}
	if (numpages > 1){
		document.getElementById("wnext").innerHTML = spanNext;
		document.getElementById("wnext1").innerHTML = spanNext;
	} else {
		document.getElementById("wnext").innerHTML = NspanNext;
		document.getElementById("wnext1").innerHTML = NspanNext;	
	}
}

function spanStats(){
	var cur_page = curpage + 1;
	//document.getElementById("stats_vis.innerHTML = '<font size="1">&nbsp;&nbsp;&nbsp;animes filtered: ' + vis.length + ' of '+aids.length+'</font><br>'
	document.getElementById("wstats_pages").innerHTML = '<font size="1">&nbsp;&nbsp;&nbsp;page: ' + cur_page +' of '+numpages+'</font>'
}

function nextPage(){
	if (curpage < numpages){
		last_vids = cur_vids;
		last_nids = cur_nids;
		cur_vids = cur_nids;
		if ( ( cur_nids + pref_animes_per_page ) > vis.length ){ cur_nids = vis.length } else { cur_nids = cur_nids + pref_animes_per_page }
		curpage = curpage + 1;
		writeAnime(vis,cur_vids,cur_nids);
		document.getElementById("wnext").innerHTML = spanNext;
		document.getElementById("wnext1").innerHTML = spanNext;
		document.getElementById("wprev").innerHTML = spanPrev;
		document.getElementById("wprev1").innerHTML = spanPrev;
		if ((curpage + 1) == numpages){
			document.getElementById("wnext").innerHTML = NspanNext;
			document.getElementById("wnext1").innerHTML = NspanNext;
		} else {
			document.getElementById("wnext").innerHTML = spanNext;
			document.getElementById("wnext1").innerHTML = spanNext;
		}
	} else {
		document.getElementById("wnext").innerHTML = NspanNext;
		document.getElementById("wnext1").innerHTML = NspanNext;
		document.getElementById("wprev").innerHTML = spanPrev;
		document.getElementById("wprev1").innerHTML = spanPrev;
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
		document.getElementById("wprev").innerHTML = spanPrev;
		document.getElementById("wprev1").innerHTML = spanPrev;
		document.getElementById("wnext").innerHTML = spanNext;
		document.getElementById("wnext1").innerHTML = spanNext;
		if ((curpage - 1) < 0){
			document.getElementById("wprev").innerHTML = NspanPrev;
			document.getElementById("wprev1").innerHTML = NspanPrev;
		} else {
			document.getElementById("wprev").innerHTML = spanPrev;
			document.getElementById("wprev1").innerHTML = spanPrev;
		}
	} else {
		document.getElementById("wprev").innerHTML = NspanPrev;
		document.getElementById("wprev1").innerHTML = NspanPrev;
		document.getElementById("wnext").innerHTML = spanNext;
		document.getElementById("wnext1").innerHTML = spanNext;
	}
	spanStats();
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
			if (anime.eps == 0){var ongoing = 1} else {var ongoing = 0}
			output += '<table width="100%" cellpadding="0" cellspacing="0" class="lines">'
			output += '<tr height="20px">'
			output += '<td class="anime_table" width="25px" align="center">'
			if ( aToExpand == anime.id ){
				output += '<a href="wishlist.htm"><img src="images/colapse.gif" title="colapse" alt="colapse" border="0"></a>'
			} else {
				output += '<a href="wishlist.htm?=a'+anime.id+'&amp;expand=1"><img src="images/expand.gif" title="expand" alt="expand" border="0"></a>' 
			}
			if (lay_mylist_name == 1){
				output += '</td>'
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
			}
			if (lay_mylist_stat == 1){
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
				if (ongoing){
					output += '<img src="images/ongoing_tv.gif" width="15" height="13" title="Ongoing" border="0">'
				}
				output += '</td>'
				output += '<td align="center" class="anime_table" width="25">'
				if (anime.hasAwards){
					output += '<img src="images/award_1.gif" width="15" height="13" title="This anime has awards" border="0">'
				}
				output += '</td>'
				output += '<td align="center" class="anime_table" width="25"><img src="images/state_'+ anime.state +'.gif" width="15" height="13" Title="State: '+ anime.statename +'"></td>'
			}
			if (lay_mylist_eps == 1){
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
			}
			if (lay_mylist_seen == 1){
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
			}
			if (lay_mylist_type == 1){
				output += '<td align="center" class="anime_table" width="100"> ' + anime.typename + ' </td>'
			}
			if (lay_mylist_year == 1){
				output += '<td align="center" class="anime_table" width="100"> ' + anime.year + ' </td>'
			}
			if (lay_mylist_size == 1){
				output += '<td align="center" class="anime_table" width="100"> ' + anime.size + ' </td>'
			}
			if (lay_mylist_rating == 1){
				output += '<td align="center" class="anime_table" width="100"> ' + anime.rating + ' ('+ anime.votes +') '
				if ( anime.HasMyVote ){
					output += '['+anime.myVote+']'
				} 
				if ( anime.HasMyTmpVote && !anime.HasMyVote ){
					output += '['+anime.myTmpVote+']'
				}
				output += '</td>'
			}
			if (lay_mylist_votes == 1){
				output += '<td align="center" class="anime_table" width="70"> ' + anime.rrating + ' ('+ anime.reviews +') </td>'
			}
			if (pref_trade == 1){
				var in_list = 0;
				for (tk = 0; tk< test_trade.length; tk++){
					if (test_trade[tk] == "a" + anime.id){
						in_list = 1;
					}
				}
				output += '<td align="center" class="anime_table" width="25"><input type="checkbox" id="a'+anime.id+'_sel" value="'+anime.size+'" onClick="javascript:trade_add(\'a\',\''+anime.id+'\',\'\',\'\');"'
				if (in_list == 1){
					output += ' checked'
				}
				output += '></td>'
			}
			output += '</tr>'
			output += '</table>'
			if ( aToExpand == anime.id ){ //New expand(!) layout...
				output += '<table id=ephead width="100%" border="0" cellspacing="0" cellpadding="0">'
				output += '<tr  bgcolor="#F0F0EB">'
				output += '<td width="10" height="15">&nbsp;</td>'
				output += '<td width="30" height="15">'
				output += '<a href="javascript:toggleAll();toggle(\'img_episodes_plus\');toggle(\'img_episodes_minus\');toggleAlleps();">'
				output += '<div id=img_episodes_plus style="display:"><img src="images/expand.gif" width="15" height="13" Title="Show all File Information on '+xa.name+'" border="0"></div>'
				output += '<div id=img_episodes_minus style="display:none"><img src="images/colapse.gif" width="15" height="13" Title="Hide all File Information on '+xa.name+'" border="0"></div></a>'
				output += '</td>'
				output += '<td width="25"><font size="1" >EP</font></td>'
				output += '<td bgcolor="#F0F0EB"><font size="1" >Title</font></td>'
				output += '<td width="25">&nbsp;</td>'
				output += '<td width="25">&nbsp;</td>'
				output += '<td width="60"><font size="1" >Length</font></td>'
				output += '<td width="70"><font size="1" >Date</font></td>'
				output += '</tr>'
				output += '</table>'
				for (si = 0; si < xa.enums.length; si++){
					var ename = "";
					var ep = eval("e" + xa.enums[si]);
					output += '<table id=eploop width="100%" border="0" cellspacing="0" cellpadding="0" class="lines">'
					output += '<tr height="20px">'
					output += '<td width="10" align="center" height="15"></td>'
					output += '<td width="30" align="center" height="15"><font size="1" >'
					output += '<a href="javascript:toggle(\'eid'+ep.num+'\');toggle(\'img_ep'+ep.num+'_plus\');toggle(\'img_ep'+ep.num+'_minus\');">'
					output += '<div id=img_ep'+ep.num+'_plus style="display:"><img src="images/expand.gif" width="15" height="13" Title="Show File Information" border="0"></div>'
					output += '<div id=img_ep'+ep.num+'_minus style="display:none"><img src="images/colapse.gif" width="15" height="13" Title="Hide all File Information" border="0"></div>'
					output += '</a></font></td>'
					output += '<td width="25" align="center"><font size="1"><a href="javascript:popup(\'epinfo.htm?=a'+xa.id+'&amp;e'+ep.num+'\',\'EpInfo\',\'370\',\'230\');" Title="Show/Hide information of files for this episode">'+ep.num+'</a></font></td>'
					if (pref_etitle == 0){ ename = ep.name };
					if (pref_etitle == 1){ 
						if ( ep.name_romanji != '-'){ ename = ep.name_romanji } else { ename = ep.name }
					};
					if (pref_etitle == 2){ 
						if ( ep.name_kanji != '-'){ ename = ep.name_kanji } else { ename = ep.name }
					};
					output += '<td><font size="1">'+ename+'&nbsp;</font></td>'
					output += '<td width="25" align="center"><img src="images/ep_state_'+ep.state+'.gif" width="15" height="13"'
					if (ep.state == "2"){output += 'title="Episode is a recap (summary)">'}
					output += '</td>'
					output += '<td width="25" align="center">'
					if (ep.isWatched){ output += '<a title="Seen"><img src="images/seen.gif" width="15" height="13" Title="Episode is Seen" border="0"></a>'}
					output += '</td>'
					output += '<td width="60" align="center"><font size="1">'+ep.elength+' min</font></td>'
					if (pref_dates == 0){
						output += '<td width="70" align="center"><font size="1">'+ep.shortDate+'</font></td>'
					} else {
						if (ep.shortAired != ""){
							output += '<td width="70" align="center"><font size="1">'+ep.shortAired+'</font></td>'
						} else {
							output += '<td width="70" align="center"><font size="1">'+ep.shortDate+'</font></td>'
						}
					}
					output += '</tr>'
					output += '</table>'
					output += '<div id=eid'+ep.num+' style="display:none">'
					output += '<table id=fileinfo border="0" cellspacing="1" cellpadding="0" width="100%" bgcolor="#F0F0EB">'
					output += '<tr height="20px">'
					output += '<td width="30" height="15">&nbsp;</td>'
					output += '<td width="60"><font size="1" >fID</font></td>'
					output += '<td width="110"><font size="1" >group</font></td>'
					output += '<td width="150"><font size="1" >size</font></td>'
					output += '<td width="60"><font size="1" >qual</font></td>'
					output += '<td width="130"><font size="1" >stat</font></td>'
					output += '<td width="50"><font size="1" >state</font></td>'
					output += '<td width="70"><font size="1" >storage</font></td>'
					output += '<td width="70"><font size="1" >source</font></td>'
					output += '<td width="70"><font size="1" >comment</font></td>'
					output += '<td width="45"><font size="1" >ed2k</font></td>'
					if (pref_trade == 1){
						output += '<td width="45"><font size="1" >trade</font></td>'
					} else { 
						output += '<td width="45">&nbsp;</td>'
					}
					output += '<td>&nbsp;</td>'
					output += '</tr>'
					for (sk = 0; sk < ep.fnums.length; sk++){		
						var f = eval("f" + ep.fnums[sk]);
						if (f.isGeneric && pref_generic == 0){
							//nothing happens, hopefully...
						} else {
							output += '<tr>'
							output += '<td width="30" bgcolor="#F0F0EB" height="15" align="center">&nbsp;</td>'
							if (f.isGeneric){
								output += '<td colspan="5" bgcolor="#E6E6E6"><font size="1">Generic File (<a href="http://www.anidb.net/forum/viewtopic.php?t=2493&amp;sid=7c808e884c34dc2624846cf9d7b001c3" title="What are Generic Files?" target="_blank" alt="Generic Files">what\'s this?</a>)</font></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><img src="images/filestate_'+f.myfilestate+'.gif" width="15" height="13" Title="My File State: '+f.myfilestatename+'" border="0"></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><font size="1">'
								if (f.hasStorage){ output += '<img src="images/storage.gif" width="15" height="13" title="Storage: '+f.storage+'" border="0">'}
								output += '</font></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><font size="1">'
								if (f.hasSource){ output += '<img src="images/source.gif" width="15" height="13" title="Source: '+f.source+'" border="0">'}
								output += '</font></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><font size="1">'
								if (f.hasSource){ output += '<img src="images/comment.gif" width="15" height="13" title="Source: '+f.other+'" border="0">'}
								output += '</font></td>'
								output += '<td colspan="2" align="center" bgcolor="#E6E6E6">&nbsp;</td>'
								output += '<td align="center" bgcolor="#E6E6E6">&nbsp;</td>'
							} else {
								output += '<td align="center" bgcolor="#E6E6E6"><font size="1"><a href="javascript:popup(\'fileinfo.htm?=a'+xa.id+'&amp;e'+ep.num+'&amp;f'+f.id+'\',\'FileInfo\',\'500\',\'500\');" title="Click for file information">'+f.id+'</a></font></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><font size="1"><a href="'+anidb+'?show=group&gid='+f.groupID+'" target="_blank" Title="Group: '+f.groupName+'">'+f.groupShort+'</a></font></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><font size="1"><a title="Size: '+f.sizeMB+'">'+f.sizeBytes+' bytes</a></font></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><img src="images/rate_'+f.qualID+'.gif" height="13" Title="Rate: '+f.qualName+'" border="0"></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><img src="images/audio_'+f.langID+'.gif" height="13" Title="Audio Language(s): '+f.langName+'" border="0">'
								output += '<img src="images/sub_'+f.subID+'.gif" width="15" height="13" Title="Subtitle Language: '+f.subName+'" border="0">'
								if (f.isCRCOK){ output += '<img src="images/crc_yes.gif" title="file matches official crc ('+f.crc+')" width="15" height="13" border="0">'}
								if (f.isCRCNO){ output += '<img src="images/crc_no.gif" title="File has invalid crc" width="15" height="13" border="0">' } 
								if (f.isCRCUN){ output += '<img src="images/crc_un.gif" title="File has not been CRC verified" width="15" height="13" border="0">' } 
								if (f.isWatched){ output += '<img src="images/seen.gif" width="15" height="13" Title="File is seen" border="0">'}
								if (f.isV2){ output += '<img src="images/vrs_2.gif" width="15" height="13" Title="File is version 2" border="0">'}
								if (f.isV3){ output += '<img src="images/vrs_3.gif" width="15" height="13" Title="File is version 3" border="0">'}
								if (f.isV4){ output += '<img src="images/vrs_4.gif" width="15" height="13" Title="File is version 4" border="0">'}
								if (f.isV5){ output += '<img src="images/vrs_5.gif" width="15" height="13" Title="File is version 5" border="0"></td>'}
								output += '<td align="center" bgcolor="#E6E6E6"><img src="images/state_'+f.mystate+'.gif" width="15" height="13" Title="State: '+f.mystatename+'" border="0">'
								output += '<img src="images/filestate_'+f.myfilestate+'.gif" width="15" height="13" Title="My File State: '+f.myfilestatename+'" border="0"></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><font size="1">'
								if (f.hasStorage){ output += '<img src="images/storage.gif" width="15" height="13" title="Storage: '+f.storage+'" border="0">'}
								output += '</font></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><font size="1">' + f.typeName
								if (f.hasSource){ output += '<img src="images/source.gif" width="15" height="13" title="Source: '+f.source+'" border="0">'}
								output += '</font></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><font size="1">'
								if (f.hasCommnent){ output += '<img src="images/comment.gif" width="15" height="13" title="Comment: '+f.other+'" border="0">'}
								output += '</font></td>'
								output += '<td align="center" bgcolor="#E6E6E6"><a href="'+f.ed2kLink+'"><img src="images/ed2k.gif" width="15" height="13" Title="ed2k link" border="0"></a></td>'
								if (pref_trade == 1){
									var in_list = 0;
									for (tk = 0; tk< test_trade.length; tk++){
										if (test_trade[tk] == 'f' + f.id + ':' + anime.id + ':' + ep.num){
											in_list = 1;
										}
									}
									output += '<td align="center" bgcolor="#E6E6E6"><input type="checkbox" id="f'+f.id+'_sel" value="" onClick="javascript:trade_add(\'f\','+f.id+','+xa.id+','+ep.num+')"'
									if (in_list == 1){
										output += ' checked'
									}
									output += '></td>'
									//output += '<td align="center" bgcolor="#E6E6E6">&nbsp;</td>'
								} else {
									output += '<td align="center" bgcolor="#E6E6E6">&nbsp;</td>'
								}
								output += '<td bgcolor="#E6E6E6">&nbsp;</td>'
							}
						}
					}
					output += '</tr>'
					output += '</table>'
					output += '</div>'
				}
				output += '<table id=ephead width="100%" border="0" cellspacing="0" cellpadding="0">'
				output += '<tr  bgcolor="#F0F0EB">'
				output += '<td colspan="8" height="20"></td>'
				output += '</tr>'
				output += '</table>'
				hideT('loading');
			}
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
	document.getElementById("wishItable").innerHTML = output;
	spanStats();
}



function showAnimePage(target){
	if (target == 'blank.htm' && cur_anime != '\'blank.htm\''){} else { cur_anime = target; }
	javascript:popup(target,'anime','960','500');
}


<!-- start: filter letter -->
<!-- objective: filter animes by letter -->

function wfilterletter(arg){
	var tmp_vis = new Array();
	if (arg != 'all' && arg != 'number' ){
		for (i = 0; i < wishlist_ids.length; i++){
			var anime = eval ( "a" + wishlist_ids [ i ] );
			if (pref_title == 0){
				aux = anime.name; 
			}
			if (pref_title == 2){
				if ( anime.eng != '-'){ 
					aux = anime.eng; 
				} else { 
					aux = anime.name; 
				}
			}
			var fc = aux.substring(0,1);
			fc = fc.toLowerCase();
			if ( wishlist_sel == 1){
				if ( fc == arg && anime.isWishListed){
					tmp_vis.push (wishlist_ids[i]);
				}
			} else {
				if ( fc == arg ){
					tmp_vis.push (wishlist_ids[i]);
				}
			}
		}
		var aux = 'letter ' + arg;
	}
	if (arg == 'all'){
		for (i = 0; i < wishlist_ids.length; i++){
			var anime = eval ( "a" + wishlist_ids [ i ] );
			if ( wishlist_sel == 1){
				if ( anime.isWishListed ){
					tmp_vis.push (wishlist_ids[i]);
				}
			} else {
				tmp_vis.push (wishlist_ids[i]);
			}
		}
		var aux = 'none';
	}
	if (arg == 'number'){
		for (i = 0; i < wishlist_ids.length; i++){
			var anime = eval ( "a" + wishlist_ids [ i ] );
			var fc = anime.name.substring(0,1);
			fc = fc.toLowerCase();
			if ( wishlist_sel == 1){
				if ( anime.isWishListed && fc != 'a' && fc != 'b' && fc != 'c' && fc != 'd' && fc != 'e' && fc != 'f' && fc != 'g' && fc != 'h' && fc != 'i' && fc != 'j' && fc != 'k' && fc != 'l' && fc != 'm' && fc != 'n' && fc != 'o' && fc != 'p' && fc != 'q' && fc != 'r' && fc != 's' && fc != 't' && fc != 'u' && fc != 'v' && fc != 'w' && fc != 'x' && fc != 'y' && fc != 'z' ){
					tmp_vis.push (wishlist_ids[i]);
				}
			} else {
				if ( fc != 'a' && fc != 'b' && fc != 'c' && fc != 'd' && fc != 'e' && fc != 'f' && fc != 'g' && fc != 'h' && fc != 'i' && fc != 'j' && fc != 'k' && fc != 'l' && fc != 'm' && fc != 'n' && fc != 'o' && fc != 'p' && fc != 'q' && fc != 'r' && fc != 's' && fc != 't' && fc != 'u' && fc != 'v' && fc != 'w' && fc != 'x' && fc != 'y' && fc != 'z' ){
					tmp_vis.push (wishlist_ids[i]);
				}
			}
		}
		var aux = 'letter 0-9';
	}
	document.getElementById("wish_status_sel").value = "4";
	document.getElementById("active_filter").innerHTML = aux;
	vis = tmp_vis;
	curpage = 0;
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}

<!-- end: filter letter -->