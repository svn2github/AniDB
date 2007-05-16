<!-- please insert the e-mail address that will be used to send the trade requests -->

var tmail = '';

<!-- end of user vars, don't mess with any of the below unless you are very sure of what you are doing. -->

<!-- start: vars -->
<!-- objective: defines vars to be used through out the script -->

var today = new Date();
var expiry = new Date(today.getTime() + 365 * 24 * 60 * 60 * 1000);
var state_udf = 1;
var state_unk = 1;
var state_hdd = 1;
var state_cd = 1;
var state_del = 1;
var state_sha = 1;
var state_rel = 1;
var state_all = 1;
var show_comp = 0;
var show_inc = 0;
var show_seen = 0;
var show_unvw = 0;
var show_wish = 0;
var sel_unvw = 0;
var misc = '';
var descr = '-';
var len = 0;
var pref_expand_info = getCookie("pref_expand_info") || 1;
var pref_expand_eps = getCookie("pref_expand_eps") || 0;
var pref_animes_per_page = getCookie("pref_animes_per_page") || 25;
var pref_trade = getCookie("pref_trade") || 0;
var pref_title = getCookie("pref_title") || 0;
var pref_title2 = getCookie("pref_title2") || 0;
var pref_title2opt = getCookie("pref_title2opt") || 0;
var pref_etitle = getCookie("pref_etitle") || 0;
var pref_hentai = getCookie("pref_hentai") || 0;
var pref_generic = getCookie("pref_generic") || 1;
var pref_dates = getCookie("pref_dates") || 1;
var template_ver = "1.0.2 final"
var check_wish = 0;
var print_sel = 0;
var test_trade = new Array();
var test_wishlist = new Array();
var test_file = new Array();
var pass = 0;

<!-- end: vars -->


<!-- start: wishlist check -->
<!-- objective: checks to see if theres one or more wishlisted anime -->

function check_wishlist(){
	for (i = 0; i < aids.length; i++){
		var anime = eval ( "a" + aids [ i ] )
		if (anime.isWishListed){
			check_wish = 1;
			break;
		}	
	}
}

<!-- end: wishlist check -->

<!-- start: reorder -->
<!-- objective: reorders ids based on argument. -->

function sort_AB(a,b){
	a = a.split("|");
	b = b.split("|");
	at = parseInt(a[0]);
	bt = parseInt(b[0]);
	return at - bt;
}

function sort_BA(a,b){
	a = a.split("|");
	b = b.split("|");
	at = parseInt(a[0]);
	bt = parseInt(b[0]);
	return bt - at;
}

function reverseSort(a, b) 
{ 
   if(a.toLowerCase() > b.toLowerCase()) 
      return -1 
   if(a.toLowerCase() < b.toLowerCase()) 
      return 1 
   return 0 
}

function reorder(arg,type,init){
	var new_aids = new Array();
	var sorts = new Array();
	sorts = ["name","eps","seen","type","votes","year","size","rating"];
	var sort_sel = "";
	var sort_type = "";
	for (i = 0; i < aids.length; i++){
		var aname = new Object();
		var anime = eval ( "a" + aids [ i ] );
		if (arg == 0){
			if (pref_title == 0){
				aname.a = anime.name; 
			}
			if (pref_title == 2){
				if ( anime.eng != '-'){ 
					aname.a = anime.eng; 
				} else { 
					aname.a = anime.name; 
				}
			}	
			sort_sel = "name_sort";
		}
		if (arg == 1){
			aname.a = anime.rating;
			aname.a = aname.a.replace('.', '');
			aname.a = aname.a.replace('-', '0');
			sort_sel = "rating_sort";
		}
		if (arg == 2){
			aname.a = anime.myEps;
			sort_sel = "eps_sort";
		}
		if (arg == 3){
			aname.a = anime.myWEps;
			sort_sel = "seen_sort";
		}
		if (arg == 4){
			aname.a = anime.type;
			sort_sel = "type_sort";
		}
		if (arg == 5){
			aname.a = anime.rrating;
			aname.a = aname.a.replace('.', '');
			aname.a = aname.a.replace('-', '0');
			sort_sel = "voted_sort";
		}
		if (arg == 6){
			aname.a = parseInt(anime.size);
			sort_sel = "size_sort";
		}
		if (arg == 7){
			aname.a = parseInt(anime.year);
			sort_sel = "year_sort";
		}
		aname.id = anime.id;
		aname = aname.a + '|' + aname.id
		new_aids.push (aname);
	}
	
	if (type == 0){
		if(arg == 0){
			new_aids.sort();
		} else {
			new_aids.sort(sort_AB);
		}
		sort_type = "_up";
	} else {
		if(arg == 0){
			new_aids.sort(reverseSort);
		} else {
			new_aids.sort(sort_BA);
		}
		sort_type = "_down";
	}
	
	aids = new Array();
		
	for (i = 0;i < new_aids.length; i++){
		var anime = new Array();
		anime = new_aids[i].split("|");
		aids.push (anime[1]);
	}
	
	
	if ( init != 1){
		vis = aids;
		writeAnime(vis,svids,snids);
		writeSpans();
		spanStats();
		if ( aToExpand != "" && expand == 1 ){ scrollTo ( "a" + aToExpand );};
		hideT("loading");
		for (i=0;i < sorts.length; i++){
			var aux = "lay_mylist_" + sorts[i];
			aux = eval(aux);
			if (aux == "1"){
				document.getElementById(sorts[i]+"_sort_up").src = "images/sort_up.gif";
				document.getElementById(sorts[i]+"_sort_down").src = "images/sort_down.gif";
			}
		}
		document.getElementById(sort_sel+sort_type).src = "images/sort"+sort_type+"_sel.gif";
	}
}

<!-- end: wishlist check -->


<!-- start: print -->
<!-- objective: layouts page to allow printing -->

function print(arg){
	if (arg == 1){
		showT("print_head");
		hideT("print_off_0");
		hideT("print_off_1");
		hideT("print_off_2");
		hideT("print_off_3");
		hideT("print_off_4");
		hideT("dttop");
		hideT("tbottom");
		hideT("wtbottom");
		pref_animes_per_page = 5000;
		print_sel = 1;
	} else {
		self.location = "mylist.htm";
	}
	page = 0;
	curpage = 0;
	svids = 0;
	snids = 0;
	cur_vids = 0;
	cur_nids = 0;
	numpages = Math.ceil(aids.length / pref_animes_per_page);
	if ( pref_animes_per_page > vis.length ) { snids = vis.length; cur_nids = vis.length } else { snids = pref_animes_per_page; cur_nids = pref_animes_per_page };
	last_vids = cur_vids;
	last_nids = cur_nids;
	vis = aids;
	writeAnime(vis,0,aids.length);
}

<!-- end: print -->

<!-- start: resets -->
<!-- objective: resets all divs and vars thus showing all animes/info -->

function showAll(){
	hideState('all');		
}

<!-- end: resets -->


<!-- start: toggles -->
<!-- objective: toggles element visibility based on previous state/ shows/hides divs -->

function toggle( targetId ){
  		target = document.getElementById( targetId );
  			if (target.style.display == "none"){
  				target.style.display = "";
  			} else {
  				target.style.display = "none";
  			}
}

function showT( targetId ){
		target = document.getElementById( targetId );
  		target.style.display = "";
}

function hideT( targetId ){
		target = document.getElementById( targetId );
  		target.style.display = "none";
}

<!-- end: toggles -->


<!-- hide by status, checks to se if target exists, if exists then calls function toggle. -->
<!-- used for watched, complete, incomplete and unviewed status. -->

function dsStatus(arg){
	var tmp_vis = new Array();
	if ( show_comp == 1 && arg == 1 ){
		vis = aids;
		if (show_seen == 1){
			arg = 3;
			pass = 1;
			dsStatus(arg);
			pass = 0;
		}
		if (show_unvw == 1){
			arg = 4;
			pass = 1;
			dsStatus(arg);
			pass = 0;
		}
		if (pass == 0){
			show_comp = 0;
		}
	}
	if ( show_inc == 1 && arg == 2 ){
		vis = aids;
		if (show_seen == 1){
			arg = 3;
			pass = 1;
			dsStatus(arg);
			pass = 0;
		}
		if (show_unvw == 1){
			arg = 4;
			pass = 1;
			dsStatus(arg);
			pass = 0;
		}
		if (pass == 0){
			show_inc = 0;
		}
	}
	
	if (arg == 0){ 
		tmp_vis = aids; 
		show_comp = 0;
		show_inc = 0;
		show_seen = 0;
		show_unvw = 0;
		show_wish = 0;
	}
	if (arg == 1){
		for (i = 0; i < vis.length; i++){
			var anime = eval ( "a" + vis [ i ] )
			if (anime.isComplete){
				tmp_vis.push (vis[i]);
				show_comp = 1;
			}
		}
	}
	if (arg == 2){
		for (i = 0; i < vis.length; i++){
			var anime = eval ( "a" + vis [ i ] )
			if (anime.isComplete){
			} else {
				tmp_vis.push (vis[i]);
				show_inc = 1;
			}
		}
	}
	if (arg == 3){
		for (i = 0; i < vis.length; i++){
			var anime = eval ( "a" + vis [ i ] )
			if (anime.isWatched){
				tmp_vis.push (vis[i]);
				show_seen = 1;
			}
		}
	}
	if (arg == 4){
		for (i = 0; i < vis.length; i++){
			var anime = eval ( "a" + vis [ i ] )
			if (anime.isWatched){
			} else {
				tmp_vis.push (vis[i]);
				show_unvw = 1;
			}
		}
	}
	if (arg == 5){
		for (i = 0; i < vis.length; i++){
			var anime = eval ( "a" + vis [ i ] )
			if (anime.isWishListed){
				tmp_vis.push (vis[i]);
				show_wish = 1;
			}
		}
	}
	vis = tmp_vis;
	curpage = 0;
	
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}

function wsStatus(arg){
	var tmp_vis = new Array();
	if (arg == 0){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.isWishListed && anime.wishlist_type == 0){
				tmp_vis.push (aids[i]);
			}
		}
	}
	if (arg == 1){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.isWishListed && anime.wishlist_type == 1){
				tmp_vis.push (aids[i]);
			}
		}
	}
	if (arg == 2){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.isWishListed && anime.wishlist_type == 2){
				tmp_vis.push (aids[i]);
			}
		}
	}
	if (arg == 3){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.isWishListed && anime.wishlist_type == 3){
				tmp_vis.push (aids[i]);
			}
		}
	}
	if (arg == 4){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.isWishListed){
				tmp_vis.push (aids[i]);
			}
		}
	}
	vis = tmp_vis;
	curpage = 0;
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}

<!-- end: filter by status -->


<!-- start: toggle status -->
<!-- objective: just a little code to help showAll() work -->

function toggleCOMP(){
	if (show_comp == 0){
		show_comp = 1;
	} else {
		show_comp = 0;
	}
}

function toggleINC(){
	if (show_inc == 0){
		show_inc = 1;
	} else {
		show_inc = 0;
	}
}

function toggleSEEN(){
	if (show_seen == 0){
		show_seen = 1;
	} else {
		show_seen = 0;
	}
}

function toggleUNVW(){
	if (show_unvw == 0){
		show_unvw = 1;
	} else {
		show_unvw = 0;
	}
}

<!-- end: toggle status -->


<!-- start: show anime by state -->
<!-- objective: filter animes by state on my list -->

function hideState(arg){
	if (arg == 'udf'){ showState(-1);};
	if (arg == 'unk'){ showState(0);};
	if (arg == 'hdd'){ showState(1);};
	if (arg == 'cd'){ showState(2);};
	if (arg == 'del'){ showState(3);};
	if (arg == 'sha'){ showState(4);};
	if (arg == 'rel'){ showState(5);};
	if (arg == 'awd'){ showState(-2);};
	if (arg == 'ong'){ showState(-4);};
	if (arg == 'all'){ showState(-3);};
	document.getElementById("type_sel2").value = "all";
	document.getElementById("status_sel").value = "0";
	var aux = eval('f_st_' + arg);
	document.getElementById("active_filter").innerHTML = aux;
}

function showState(arg){
	var tmp_vis = new Array();
	if (arg > -2){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.state == arg ){
				tmp_vis.push (aids[i]);
			}
		}
	}
	if ( arg == -3){
			for (i = 0; i < aids.length; i++){
				tmp_vis.push (aids[i]);
			}
	}
	if ( arg == -2){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.hasAwards){
				tmp_vis.push (aids[i]);
			}
		}
	}
	if ( arg == -4){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.eps == 0){
				tmp_vis.push (aids[i]);
			}
		}
	}
	vis = tmp_vis;
	curpage = 0;
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}

function WShideState(arg){
	if (arg == 'low'){ WSshowState(0);};
	if (arg == 'med'){ WSshowState(1);};
	if (arg == 'high'){ WSshowState(2);};
	if (arg == 'all'){ WSshowState(-1);};
}

function WSshowState(arg){
	var tmp_vis = new Array();
	if (arg > -1){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.isWishListed && anime.wishlist_priority == arg ){
				tmp_vis.push (aids[i]);
			}
		}
	}
	if ( arg == -1){
			for (i = 0; i < aids.length; i++){
				var anime = eval ( "a" + aids [ i ] )
				if (anime.isWishListed){
					tmp_vis.push (aids[i]);
				}
			}
	}
	vis = tmp_vis;
	curpage = 0;
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}

<!-- end: show anime by state -->


<!-- start: filter by type -->
<!-- objective: filter animes by type, this works the same away as filter by state. -->

function filter_type(){
	var arg = document.getElementById("type_sel").value;
	if (arg == 'all'){ showType(0);};
	if (arg == 'unk'){ showType(1);};
	if (arg == 'tv'){ showType(2);};
	if (arg == 'ova'){ showType(3);};
	if (arg == 'mov'){ showType(4);};
	if (arg == 'otr'){ showType(5);};
	if (arg == 'web'){ showType(6);};
}

function filter_type2(){
	var arg = document.getElementById("type_sel2").value;
	if (arg == 'all'){ showType(0);};
	if (arg == 'unk'){ showType(1);};
	if (arg == 'tv'){ showType(2);};
	if (arg == 'ova'){ showType(3);};
	if (arg == 'mov'){ showType(4);};
	if (arg == 'otr'){ showType(5);};
	if (arg == 'web'){ showType(6);};
}

function showType(arg){
	var tmp_vis = new Array();
	if (arg > 0){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] )
			if (anime.type == arg ){
				tmp_vis.push (aids[i]);
			}
		}
	} else {
		for (i = 0; i < aids.length; i++){
			tmp_vis.push (aids[i]);
		}
	}
	vis = tmp_vis;
	curpage = 0;
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}


<!-- end: filter by type -->


<!-- start: filter by genre -->
<!-- objective: filter by genre, simple filter, 1 genre at a time -->

function filter_genre(){
	var arg = document.getElementById("genre_sel").value;
	var tmp_vis = new Array();
	if (arg == '0'){ 
		for (i = 0; i < aids.length; i++){
			tmp_vis.push (aids[i]);
		}
	} else {
		for (i = 0; i < aids.length; i++){			
			var genre_ok = 0;
			var anime = eval ( "a" + aids [ i ] );
			for (k = 0; k < anime.ngenres ; k++){
				if (anime.genres[k] == arg){
					genre_ok = genre_ok + 1;
				} else {
					genre_ok = genre_ok + 0;
				}
			}
			if (genre_ok > 0){
				tmp_vis.push (aids[i]);
			}
		}		
	}
	vis = tmp_vis;
	curpage = 0;
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}

function filter_genre2(){
	var arg = document.getElementById("genre_sel2").value;
	var tmp_vis = new Array();
	if (arg == '0'){ 
		for (i = 0; i < aids.length; i++){
			tmp_vis.push (aids[i]);
		}
	} else {
		for (i = 0; i < aids.length; i++){			
			var genre_ok = 0;
			var anime = eval ( "a" + aids [ i ] );
			for (k = 0; k < anime.ngenres ; k++){
				if (anime.genres[k] == arg){
					genre_ok = genre_ok + 1;
				} else {
					genre_ok = genre_ok + 0;
				}
			}
			if (genre_ok > 0){
				tmp_vis.push (aids[i]);
			}
		}		
	}
	vis = tmp_vis;
	curpage = 0;
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}

<!-- end: filter by genre -->


<!-- start: filter letter -->
<!-- objective: filter animes by letter -->

function filterletter(arg){
	var tmp_vis = new Array();
	if (arg != 'all' && arg != 'number' ){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] );
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
					tmp_vis.push (aids[i]);
				}
			} else {
				if ( fc == arg ){
					tmp_vis.push (aids[i]);
				}
			}
		}
		var aux = 'letter ' + arg;
	}
	if (arg == 'all'){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] );
			if ( wishlist_sel == 1){
				if ( anime.isWishListed ){
					tmp_vis.push (aids[i]);
				}
			} else {
				tmp_vis.push (aids[i]);
			}
		}
		var aux = 'none';
	}
	if (arg == 'number'){
		for (i = 0; i < aids.length; i++){
			var anime = eval ( "a" + aids [ i ] );
			var fc = anime.name.substring(0,1);
			fc = fc.toLowerCase();
			if ( wishlist_sel == 1){
				if ( anime.isWishListed && fc != 'a' && fc != 'b' && fc != 'c' && fc != 'd' && fc != 'e' && fc != 'f' && fc != 'g' && fc != 'h' && fc != 'i' && fc != 'j' && fc != 'k' && fc != 'l' && fc != 'm' && fc != 'n' && fc != 'o' && fc != 'p' && fc != 'q' && fc != 'r' && fc != 's' && fc != 't' && fc != 'u' && fc != 'v' && fc != 'w' && fc != 'x' && fc != 'y' && fc != 'z' ){
					tmp_vis.push (aids[i]);
				}
			} else {
				if ( fc != 'a' && fc != 'b' && fc != 'c' && fc != 'd' && fc != 'e' && fc != 'f' && fc != 'g' && fc != 'h' && fc != 'i' && fc != 'j' && fc != 'k' && fc != 'l' && fc != 'm' && fc != 'n' && fc != 'o' && fc != 'p' && fc != 'q' && fc != 'r' && fc != 's' && fc != 't' && fc != 'u' && fc != 'v' && fc != 'w' && fc != 'x' && fc != 'y' && fc != 'z' ){
					tmp_vis.push (aids[i]);
				}
			}
		}
		var aux = 'letter 0-9';
	}
	document.getElementById("type_sel2").value = "all";
	document.getElementById("status_sel").value = "0";
	document.getElementById("active_filter").innerHTML = aux;
	vis = tmp_vis;
	curpage = 0;
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}

<!-- end: filter letter -->


<!-- start: anime.tpl functions -->
<!-- objective: functions used by anime.tpl -->

function toggleAll(){
	for (i = 0; i < xa.enums.length; i++){
		var target = document.getElementById(['eid' + xa.enums[i]]);
		var etarget = eval(target);
		if (etarget){
			toggle('eid' + xa.enums[i] );
		}
	}
}

function toggleAlleps(){
	for (i = 0; i < xa.enums.length; i++){
		var targeta = document.getElementById(['img_ep' + xa.enums[i] + '_plus']);
		var targete = document.getElementById(['img_ep' + xa.enums[i] + '_minus']);
		var etarget = eval(targeta);
		var atarget = eval(targete);
		if (etarget){
			toggle('img_ep' + xa.enums[i] + '_plus');
		}
		if (atarget){
			toggle('img_ep' + xa.enums[i] + '_minus');
		}
	}
}

<!-- end: anime.tpl functions -->


<!-- start: preferences -->
<!-- objective: apply preferences to page -->

function set_prefs(){
	if (pref_expand_info == 0){
		hideT('info');
		showT('noinfo');
		showT('img_info_plus');
		hideT('img_info_minus');
	} else {
		showT('info');
		hideT('noinfo');
		hideT('img_info_plus');
		showT('img_info_minus');
	}
	if (pref_expand_eps == 1){
		toggleAll();
		hideT('img_episodes_plus');
		showT('img_episodes_minus');
		toggleAlleps();	
	}
}

<!-- end: preferences -->


<!-- start: popups -->
<!-- objective: writes and executes popups for file/episode info -->

function popup(url,title,w,h){
newWindow = window.open(url,title,'toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=1,resizable=1,width='+w+',height='+h);
newWindow.document.close();
}

<!-- end: popups -->


<!-- start: cookies -->
<!-- objective: control cookie functions -->

function setCookie(name,value)
{
    if (value != null && value != "")
		document.cookie = name + "=" + escape(value) + "; expires=" + expiry.toGMTString();
	dc = document.cookie;
	eval( "ck" + name + " = \"" + escape(value) + "\";" ); 
}

function getCookie(name)
{
	var dc = document.cookie;
	var prefix = name + "=";
	var begin = dc.indexOf("; " + prefix);
	if (begin == -1) {
		begin = dc.indexOf(prefix);
		if (begin != 0)
			return null;
	} else
		begin += 2;
	var end = document.cookie.indexOf(";", begin);
	if (end == -1)
		end = dc.length;
	return unescape(dc.substring(begin + prefix.length, end));
}

<!-- end: cookies -->

<!-- start: simple search -->
<!-- objective: simple search on mylist page -->

function simple_searchAnimes(){
	var title = document.getElementById('simple_search').value;
	title = title.toLowerCase();
	var splited = title.split(/\s+/g);
	var tslength = splited.length;
	var sh_ids = new Array();

	for (i = 0; i < aids.length; i++){
		var anime = eval ( "a" + aids [ i ] );

		var t_n = 0;

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
		
		if (t_n == 1){
			sh_ids.push(aids[i]);
		}
	}

	
	var aux = '';
	if (sh_ids.length > 0){ aux = 'search' } else { aux = 'not found'; };	
	document.getElementById("type_sel2").value = "all";
	document.getElementById("status_sel").value = "0";
	document.getElementById("active_filter").innerHTML = aux;
	//alert('searched by: ' + title +'\nResults: ['+sh_ids.length+']');
	vis = sh_ids;
	curpage = 0;
	writeAnime(vis,svids,snids);
	writeSpans();
	spanStats();
}

