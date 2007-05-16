var anidb = '<tmpl_var name=global_animedburl>';
var user = '<tmpl_var name=global_user>';
var cdate = '<tmpl_var name=global_date>';
var cdate_short = '<tmpl_var name=global_date_short>';
var animecount = '<tmpl_var name=global_animecount>';
var epcount = '<tmpl_var name=global_epcount>';
var filecount = '<tmpl_var name=global_filecount>';
var bytecount = '<tmpl_var name=global_bytecount>';
var bytecount_h = '<tmpl_var name=global_bytecount_h>';

var wishlist_ids = new Array();
var myvotes_ids = new Array();
var mytempvotes_ids = new Array();
var aids = new Array();<tmpl_loop name=loop_anime>
aids.push (<tmpl_var name=data_anime_id> );
var a<tmpl_var name=data_anime_id > = new Object();
a<tmpl_var name=data_anime_id>.id = <tmpl_var name=data_anime_id>;
a<tmpl_var name=data_anime_id>.name = "<tmpl_var name=data_anime_name>";
a<tmpl_var name=data_anime_id>.japkanji = "<tmpl_if name=status_anime_title_has_jap_kanji><tmpl_var expr="jsencode ( data_anime_title_jap_kanji)"><tmpl_else>-</tmpl_if>";
a<tmpl_var name=data_anime_id>.eng = "<tmpl_if name=status_anime_title_has_eng><tmpl_var expr="jsencode ( data_anime_title_eng)"><tmpl_else>-</tmpl_if>";
a<tmpl_var name=data_anime_id>.year = "<tmpl_var name=data_anime_year>";
a<tmpl_var name=data_anime_id>.eps = <tmpl_var name=data_anime_eps>;
a<tmpl_var name=data_anime_id>.myEps = <tmpl_var name=data_anime_my_eps>;
a<tmpl_var name=data_anime_id>.myWeps = <tmpl_var name=data_anime_my_watchedeps>;
a<tmpl_var name=data_anime_id>.mySEps = <tmpl_var name=data_anime_my_eps_special>;
a<tmpl_var name=data_anime_id>.myWSEps = <tmpl_var name=data_anime_my_watchedeps_special>;
a<tmpl_var name=data_anime_id>.type = <tmpl_var name=data_anime_type_id>;
a<tmpl_var name=data_anime_id>.nfo = <tmpl_var name=data_anime_animenfoid>;
a<tmpl_var name=data_anime_id>.size = "<tmpl_var name=data_anime_my_size_h>";
a<tmpl_var name=data_anime_id>.typename = "<tmpl_var name=data_anime_type_name>";
a<tmpl_var name=data_anime_id>.state = <tmpl_var name=data_anime_mystate>;
a<tmpl_var name=data_anime_id>.statename = "<tmpl_var name=data_anime_mystate_string>";
a<tmpl_var name=data_anime_id>.rating = "<tmpl_var name=data_anime_rating>";
a<tmpl_var name=data_anime_id>.votes = "<tmpl_var name=data_anime_votes>";
a<tmpl_var name=data_anime_id>.reviews = "<tmpl_var name=data_anime_reviews>";
a<tmpl_var name=data_anime_id>.rrating = "<tmpl_var name=data_anime_reviewrating>";
a<tmpl_var name=data_anime_id>.isComplete = <tmpl_if name=status_anime_iscomplete>true;<tmpl_else>false;</tmpl_if>
a<tmpl_var name=data_anime_id>.isWatched = <tmpl_if name=status_anime_iswatched>true;<tmpl_else>false;</tmpl_if>
a<tmpl_var name=data_anime_id>.hasAwards = <tmpl_if name=status_anime_hasawards>true;<tmpl_else>false;</tmpl_if>
a<tmpl_var name=data_anime_id>.isHentai = <tmpl_if name=status_anime_genre_hentai>true;<tmpl_else>false;</tmpl_if>
a<tmpl_var name=data_anime_id>.isWishListed = <tmpl_if name=status_anime_isinwishlist>true;<tmpl_else>false;</tmpl_if>
if (a<tmpl_var name=data_anime_id>.isWishListed){ wishlist_ids.push (a<tmpl_var name=data_anime_id>.id);}
a<tmpl_var name=data_anime_id>.wishlist_type = "<tmpl_var name=data_anime_wishlist_type>";
a<tmpl_var name=data_anime_id>.wishlist_type_name = "<tmpl_var name=data_anime_wishlist_type_name>";
a<tmpl_var name=data_anime_id>.wishlist_priority = "<tmpl_var name=data_anime_wishlist_priority>";
a<tmpl_var name=data_anime_id>.wishlist_priority_name = "<tmpl_var name=data_anime_wishlist_priority_name>";
a<tmpl_var name=data_anime_id>.wishlist_comment = "<tmpl_var name=data_anime_wishlist_comment>";
a<tmpl_var name=data_anime_id>.producer = "<tmpl_var name=data_anime_producer>";
a<tmpl_var name=data_anime_id>.HasMyVote = <tmpl_if name=status_anime_my_isvoted>true;<tmpl_else>false;</tmpl_if>
a<tmpl_var name=data_anime_id>.HasMyTmpVote = <tmpl_if name=status_anime_my_istmpvoted>true;<tmpl_else>false;</tmpl_if>
a<tmpl_var name=data_anime_id>.myVote = "<tmpl_var name=data_anime_my_vote>";
a<tmpl_var name=data_anime_id>.myTmpVote = "<tmpl_var name=data_anime_my_tmpvote>";
if (a<tmpl_var name=data_anime_id>.HasMyVote){ myvotes_ids.push (a<tmpl_var name=data_anime_id>.id);}
if (a<tmpl_var name=data_anime_id>.HasMyTmpVote){ mytempvotes_ids.push (a<tmpl_var name=data_anime_id>.id);}
a<tmpl_var name=data_anime_id>.genres = new Array();<tmpl_loop name=loop_anime_genre>
a<tmpl_var name=data_anime_id>.genres.push (<tmpl_var name=data_anime_genre_id> );</tmpl_loop>
a<tmpl_var name=data_anime_id>.aliases = "";
a<tmpl_var name=data_anime_id>.shortNames = "";
<tmpl_loop name=loop_anime_title_alias>a<tmpl_var name=data_anime_id>.aliases += "<tmpl_var expr="jsencode ( data_anime_title_alias_name)"><tmpl_if name="__LAST__"><tmpl_else> </tmpl_if>";</tmpl_loop>
<tmpl_loop name=loop_anime_title_short>a<tmpl_var name=data_anime_id>.shortNames += "<tmpl_var expr="jsencode ( data_anime_title_short_name)"><tmpl_if name="__LAST__"><tmpl_else> </tmpl_if>";</tmpl_loop>
a<tmpl_var name=data_anime_id>.ngenres = a<tmpl_var name=data_anime_id>.genres.length
</tmpl_loop>
var num_aids = aids.length;