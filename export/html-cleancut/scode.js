function resetST(){
	document.getElementById("ep_sel").value = "0";
	document.getElementById("year_sel").value = "0";
	ep_selects();
	year_selects();
}

function ep_selects(){
	if (document.getElementById("ep_sel").value == "3"){ showT("ep_sel_from"); } else { hideT("ep_sel_from")}
}

function year_selects(){
	if (document.getElementById("year_sel").value == "3"){ showT("year_sel_from"); } else { hideT("year_sel_from")}
}

function searchAnimes(){
	var title = document.getElementById("anime_name").value;
	title = title.toLowerCase();
	var splited = title.split(/\s+/g);
	var tslength = splited.length;
	var episodes = document.getElementById("anime_eps").value;
	var ep_sType = document.getElementById("ep_sel").value;
	if (ep_sType == "3"){
		var eps_max = document.getElementById("anime_eps_to").value;
	}
	var atype = document.getElementById("atype_sel").value;
	var year = document.getElementById("anime_year").value;
	var year_sType = document.getElementById("year_sel").value;
	if (year_sType == "3"){
		var year_max = document.getElementById("anime_year_to").value;
	}
	var rating = document.getElementById("anime_rating").value;
	var votes = document.getElementById("anime_votes").value;
	var atts = document.getElementById("anime_atts").value;
	var attavg = document.getElementById("anime_attavg").value;
	var genres = new Array();
	for (i = 1; i < 39; i++){
		if (i != 13 && i != 32){
			var tgenr = 'search_genre_'+ i;
			if (document.getElementById(tgenr).checked){ genres.push (i ); };
		}
	}
	var sh_ids = new Array();
	for (i = 0; i < aids.length; i++){
		var anime = eval ( "a" + aids [ i ] );
		var t_n = 0;
		var t_e = 0;
		var t_t = 0;
		var t_y = 0;
		var t_r = 0;
		var t_v = 0;
		var t_rv = 0;
		var t_ra = 0;
		var t_gn = 0;
		if (title != ''){
			anime.name = anime.name.toLowerCase();
			anime.eng = anime.eng.toLowerCase();
			anime.shortNames = anime.shortNames.toLowerCase();
			anime.aliases = anime.aliases.toLowerCase();
			var title_split = anime.name.split(/\s+/g);
			if ( anime.eng != '-'){ var title_split_eng = anime.eng.split(/\s+/g) } else { var title_split_eng = anime.name.split(/\s+/g) };
			var short_split = anime.shortNames.split(/\s+/g);
			var alias_split = anime.aliases.split(/\s+/g);
			for (k = 0; k < tslength; k++){
				for (x0 = 0; x0 < title_split.length; x0++){
					if (title_split[x0] == splited[k]){ t_n = 1;}
				}
				for (x1 = 0; x1 < title_split_eng.length; x1++){
					if (title_split_eng[x1] == splited[k]){ t_n = 1;}
				}
				for (x2 = 0; x2 < short_split.length; x2++){
					if (short_split[x2] == splited[k]){ t_n = 1;}
				}
				for (x3 = 0; x3 < alias_split.length; x3++){
					if (alias_split[x3] == splited[k]){ t_n = 1;}
				}
			}
		} else { t_n = 1;}
		if (episodes != ''){
			if (ep_sType == "0"){
				if (episodes == anime.eps){
					t_e = 1;
				}
			}
			if (ep_sType == "1"){
				if (anime.eps <= episodes){
					t_e = 1;
				}
			}
			if (ep_sType == "2"){
				if (anime.eps >= episodes){
					t_e = 1;
				}
			}
			if (ep_sType == "3"){
				if (anime.eps >= episodes && anime.eps <= eps_max){
					t_e = 1;
				}
			}
		} else { t_e = 1;}
		if (atype != 'all'){
			if (atype == anime.type){
				t_t = 1;
			}
		} else { t_t = 1; }
		if (year != ''){
			if (year_sType == "0"){
				if (year == anime.year){
					t_y = 1;
				}
			}
			if (year_sType == "1"){
				if (anime.year <= year){
					t_y = 1;
				}
			}
			if (year_sType == "2"){
				if (anime.year >= year){
					t_y = 1;
				}
			}
			if (year_sType == "3"){
				if (anime.year >= year && anime.year <= year_max){
					t_y = 1;
				}
			}
		} else { t_y = 1;}
		if (votes != ''){
			if (anime.votes >= votes){
				t_v = 1;
			}
		} else { v_ids_ok = aids; t_v = 1;}
		if (rating != ''){
			if (anime.rating >= rating){
				t_r = 1;
			}
		} else { t_r = 1;}
		if (atts != ''){
			if (anime.reviews >= atts){
				t_rv = 1;
			}
		} else { t_rv = 1;}
		if (attavg != ''){
			if (anime.rrating >= attavg){
				t_ra = 1;
			}
		} else { t_ra = 1;}
		if (genres.length > 0 ){
			var t_gn_s = 0
			for (k = 0; k < anime.ngenres; k++){
				for (x = 0; x < genres.length; x++){
					if ( anime.genres[k] == genres[x] ) {
					 	t_gn_s += 1;
					}
				}
			}
			if (t_gn_s == genres.length){t_gn = 1}
		} else { t_gn = 1;}
		if ( t_n == 1 && t_e == 1 && t_t == 1 && t_y == 1 && t_r == 1 && t_v == 1 && t_rv == 1 && t_ra == 1 && t_gn == 1 ){ sh_ids.push (aids[i]);}
	}	
	if (sh_ids.length > 0){
		var string = '';
		for (k = 0; k < sh_ids.length; k++){
			var anime = eval ( "a" + sh_ids[k] );
			if (pref_title == 0){ var aname = anime.name };
			if (pref_title == 1){ 
				if ( anime.japkanji != '-'){ var aname = anime.japkanji } else { var aname = anime.name }
			};
			if (pref_title == 2){ 
				if ( anime.eng != '-'){ var aname = anime.eng } else { var aname = anime.name }
			};
					string += '<div id=a' + sh_ids[k] +' style="display:">'
					string += '<table width="100%" cellpadding="0" cellspacing="0" class="lines">'
					string += '<tr height="20px">'
					string += '<td class="anime_table">&nbsp;<a href="javascript:showAnimePage(\'anime.htm?=a'+anime.id+'\');">' + aname + '</a> '
					string += '</td>'
					string += '<td align="center" class="anime_table" width="25">'
					if (anime.hasAwards){
						string += '<img src="images/award_1.gif" width="15" height="13" title="This anime has awards" border="0">'
					}
					string += '</td>'
					string += '<td align="center" class="anime_table" width="25"><img src="images/state_'+ anime.state +'.gif" width="15" height="13" Title="State: '+ anime.statename +'"></td>'
					string += '<td align="center" class="anime_table" width="70">'
					if (anime.isComplete){
						 string += '<b> '+ anime.myEps +' / '+ anime.eps +'</b>'
					} else { 
						string += ' ' + anime.myEps +' / '+ anime.eps
					}
					string += '</td>'
					string += '<td align="center" class="anime_table" width="70">'
					if (anime.isWatched){
						string += '<font color="#006600"><b> '+ anime.myWeps +' / '+ anime.eps +'</b></font>'
					} else { 
						string += ' ' + anime.myWeps +' / '+ anime.eps;
					}
					string += '</td>'
					string += '<td align="center" class="anime_table" width="100"> ' + anime.typename + ' </td>'
					string += '<td align="center" class="anime_table" width="100"> ' + anime.rating + ' ('+ anime.votes +') </td>'
					string += '<td align="center" class="anime_table" width="70"> <a href="'+ anidb +'?show=animeatt&aid='+ anime.id +'" title="View reviews for '+ anime.name +'" target="_blank">'+ anime.rrating +'</a> ('+ anime.reviews +') </td>'
					string += '</tr>'
					string += '</table>'
					string += '</div>'
		}
	} else { string = 'no animes found using search definitions.';}
	document.getElementById("results").innerHTML = string;
	showT('prefs');
	document.location.href = '#preferences'
}


function showSearchPage(){
	hideT('ttop');
	hideT('dttop');
	hideT('tbottom');
	hideT('wtbottom');
var output = '<form id="searchA" method="post" action="javascript:searchAnimes();">'
output += '<table border="0" cellspacing="0" cellpadding="0">'
output += '<tr>'
output += '<td width="98" height="10"></td>'
output += '<td bgcolor="#F0F0EB" width="100" height="10"></td>'
if (pref_trade == 1){
output += '<td width="100" height="10"></td>'
}
output += '<td width="100" height="10"></td>'
output += '</tr>'
output += '</table>'
output += '<table class="anime_table" width="100%" cellspacing="0" cellpadding="0">'
output += '<tr>'
output += '<td width="100" valign="top" align="right">&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Title:</td>'
output += '<td width="5">&nbsp;</td>'
output += '<td>'
output += '<input type="text" id="anime_name" maxlength=500 size=50 class="forms"><br>(note: currently only searchs by words, not characthers)</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Episodes:</td>'
output += '<td>&nbsp;</td>'
output += '<td>'
output += '<select id="ep_sel" onChange="javascript:ep_selects();" class="forms">'
output += '<option value="0" selected>Equals</option>'
output += '<option value="1">Less than</option>'
output += '<option value="2">More than</option>'
output += '<option value="3">From</option>'
output += '</select>'
output += '&nbsp;<input type="text" id="anime_eps" maxlength=5 size=5 class="forms">'
output += '<span id="ep_sel_from" style="display:none"> to'
output += '<input type="text" id="anime_eps_to" maxlength=5 size=5 class="forms"></span></td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">&nbsp;</td>'
output += '<td></td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Type:</td>'
output += '<td>&nbsp;</td>'
output += '<td><select id="atype_sel" size="1" width="125" height="15" class="forms">'
output += '<option value="all" selected>All</option>'
output += '<option value="1">Unknown</option>'
output += '<option value="2">TV</option>'
output += '<option value="3">OVA</option>'
output += '<option value="4">Movie</option>'
output += '<option value="6">Web</option>'
output += '<option value="5">Other</option>'
output += '</select></td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB"></td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Release Year:</td>'
output += '<td>&nbsp;</td>'
output += '<td>'
output += '<select id="year_sel" onChange="javascript:year_selects();" class="forms">'
output += '<option value="0" selected>Equals</option>'
output += '<option value="1">Less than</option>'
output += '<option value="2">More than</option>'
output += '<option value="3">From</option>'
output += '</select>'
output += '&nbsp;<input type="text" id="anime_year" maxlength=9 size=10 class="forms">'
output += '<span id="year_sel_from" style="display:none"> to'
output += '<input type="text" id="anime_year_to" maxlength=9 size=10 class="forms"></span><br>'
output += '(format: yyyy or yyyy-yyyy)</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB"></td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Rating:</td>'
output += '<td>&nbsp;</td>'
output += '<td>at least <input type="text" id="anime_votes" maxlength=3 size=5 class="forms">'
output += ' votes and a rating of at least <input type="text" id="anime_rating" maxlength=5 size=5 class="forms"> (input: x.xx, i.e. 3.15)</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB"></td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Reviews:</td>'
output += '<td>&nbsp;</td>'
output += '<td>at least <input type="text" id="anime_atts" maxlength=3 size=5 class="forms">'
output += ' reviews and a rating of at least <input type="text" id="anime_attavg" maxlength=5 size=5 class="forms">'
output += ' (input: x.xx, i.e. 3.15)</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Genres:</td>'
output += '<td>&nbsp;</td>'
output += '<td>'
output += '<table border=0 class="ani_tableHelp">'
output += '<tr>'
output += '<td><input type="checkbox" id="search_genre_1" class="forms">&nbsp;Action </td>'
output += '<td><input type="checkbox" id="search_genre_2" class="forms">&nbsp;Adventure </td>'
output += '<td><input type="checkbox" id="search_genre_26" class="forms">&nbsp;Cars </td>'
output += '<td><input type="checkbox" id="search_genre_3" class="forms">&nbsp;Comedy </td>'
output += '<td> <input type="checkbox" id="search_genre_25" class="forms">&nbsp;Dementia </td>'
output += '</tr>'
output += '<tr>'
output += '<td><input type="checkbox" id="search_genre_10" class="forms">&nbsp;Demons </td>'
output += '<td><input type="checkbox" id="search_genre_35" class="forms">&nbsp;Detective </td>'
output += '<td><input type="checkbox" id="search_genre_4" class="forms">&nbsp;Drama </td>'
output += '<td><input type="checkbox" id="search_genre_5" class="forms">&nbsp;Ecchi </td>'
output += '<td><input type="checkbox" id="search_genre_11" class="forms">&nbsp;Fantasy </td>'
output += '</tr>'
output += '<tr>'
output += '<td><input type="checkbox" id="search_genre_33" class="forms">&nbsp;Game </td>'
output += '<td><input type="checkbox" id="search_genre_34" class="forms">&nbsp;Ghibli </td>'
output += '<td><input type="checkbox" id="search_genre_6" class="forms">&nbsp;Hentai </td>'
output += '<td><input type="checkbox" id="search_genre_12" class="forms">&nbsp;Historical </td>'
output += '<td><input type="checkbox" id="search_genre_7" class="forms">&nbsp;Horror </td>'
output += '</tr>'
output += '<tr>'
output += '<td><input type="checkbox" id="search_genre_27" class="forms">&nbsp;Kids </td>'
output += '<td><input type="checkbox" id="search_genre_14" class="forms">&nbsp;Magic </td>'
output += '<td><input type="checkbox" id="search_genre_15" class="forms">&nbsp;Martial Arts </td>'
output += '<td><input type="checkbox" id="search_genre_16" class="forms">&nbsp;Mecha </td>'
output += '<td><input type="checkbox" id="search_genre_28" class="forms">&nbsp;Music </td>'
output += '</tr>'
output += '<tr>'
output += '<td><input type="checkbox" id="search_genre_17" class="forms">&nbsp;Mystery </td>'
output += '<td><input type="checkbox" id="search_genre_18" class="forms">&nbsp;Parody </td>'
output += '<td><input type="checkbox" id="search_genre_8" class="forms">&nbsp;Romance </td>'
output += '<td><input type="checkbox" id="search_genre_19" class="forms">&nbsp;Samurai </td>'
output += '<td><input type="checkbox" id="search_genre_20" class="forms">&nbsp;School </td>'
output += '</tr>'
output += '<tr>'
output += '<td><input type="checkbox" id="search_genre_9" class="forms">&nbsp;SciFi </td>'
output += '<td><input type="checkbox" id="search_genre_31" class="forms">&nbsp;Shoujo </td>'
output += '<td><input type="checkbox" id="search_genre_37" class="forms">&nbsp;Shoujo-Ai </td>'
output += '<td><input type="checkbox" id="search_genre_30" class="forms">&nbsp;Shounen </td>'
output += '<td><input type="checkbox" id="search_genre_38" class="forms">&nbsp;Shounen-Ai </td>'
output += '</tr>'
output += '<tr>'
output += '<td><input type="checkbox" id="search_genre_21" class="forms">&nbsp;Space </td>'
output += '<td><input type="checkbox" id="search_genre_22" class="forms">&nbsp;Sports </td>'
output += '<td><input type="checkbox" id="search_genre_23" class="forms">&nbsp;Super Power </td>'
output += '<td><input type="checkbox" id="search_genre_24" class="forms">&nbsp;Vampires </td>'
output += '<td><input type="checkbox" id="search_genre_29" class="forms">&nbsp;Yaoi </td>'
output += '</tr>'
output += '<tr>'
output += '<td><input type="checkbox" id="search_genre_36" class="forms">&nbsp;Yuri </td>'
output += '</tr>'
output += '</table><br><small>NOTE: none checked means all.</small></td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Action</td>'
output += '<td>&nbsp;</td>'
output += '<td><input type="submit" id="do.search" value=" Start Search " class="forms"> <input type="reset" id="Reset" value="Reset Search" class="forms"></form></td>'
output += '</tr>'
output += '<tr>'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '<td>&nbsp;</td>'
output += '</tr>'
output += '</table>'
output += '<a id="preferences"></a>'
output += '<div id=prefs style="display:none">'
output += '<table width="100%" border="0" cellspacing="0" cellpadding="0">'
output += '<tr class="anime_table">'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB">Results:</td>'
output += '<td align="center">&nbsp;</td>'
output += '</tr>'
output += '<tr class="anime_table">'
output += '<td width="100" valign="top" align="right" bgcolor="#F0F0EB"></td>'
output += '<td><span id=results></span></td>'
output += '</tr>'
output += '</table>'
output += '</div>'
document.getElementById("animeItable").innerHTML = output;
}