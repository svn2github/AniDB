var anidb = "<tmpl_var name=global_animedburl>";
var user = "<tmpl_var name=global_user>";
var userid = "<tmpl_var name=global_user_id>"
var cdate_short = "<tmpl_var name=global_date_short>";
var animecount = "<tmpl_var name=global_animecount>";
var epcount = "<tmpl_var name=global_epcount>";
var filecount = "<tmpl_var name=global_filecount>";
var bytecount_h = "<tmpl_var name=global_bytecount_h>";

var aids = new Array();
var alist = new Array();

<tmpl_loop name=loop_anime>
alist[<tmpl_var name=data_anime_id>] = new Object();
aids.push(<tmpl_var name=data_anime_id>);
alist[<tmpl_var name=data_anime_id>].id = <tmpl_var name=data_anime_id>;
alist[<tmpl_var name=data_anime_id>].name = "<tmpl_var name=data_anime_name>";
alist[<tmpl_var name=data_anime_id>].japkanji = "<tmpl_if name=status_anime_title_has_jap_kanji><tmpl_var expr="jsencode(data_anime_title_jap_kanji)"><tmpl_else>-</tmpl_if>";
alist[<tmpl_var name=data_anime_id>].eng = "<tmpl_if name=status_anime_title_has_eng><tmpl_var expr="jsencode(data_anime_title_eng)"><tmpl_else>-</tmpl_if>";
alist[<tmpl_var name=data_anime_id>].year = "<tmpl_var name=data_anime_year>";
alist[<tmpl_var name=data_anime_id>].eps = <tmpl_var name=data_anime_eps>;
alist[<tmpl_var name=data_anime_id>].myEps = <tmpl_var name=data_anime_my_eps>;
alist[<tmpl_var name=data_anime_id>].myWeps = <tmpl_var name=data_anime_my_watchedeps>;
alist[<tmpl_var name=data_anime_id>].type = <tmpl_var name=data_anime_type_id>;
alist[<tmpl_var name=data_anime_id>].size = "<tmpl_var name=data_anime_my_size_h>";
alist[<tmpl_var name=data_anime_id>].sizebytes = "<tmpl_var name=data_anime_my_size>";
alist[<tmpl_var name=data_anime_id>].typename = "<tmpl_var name=data_anime_type_name>";
alist[<tmpl_var name=data_anime_id>].state = <tmpl_var name=data_anime_mystate>;
alist[<tmpl_var name=data_anime_id>].statename = "<tmpl_var name=data_anime_mystate_string>";
alist[<tmpl_var name=data_anime_id>].rating = "<tmpl_if expr="data_anime_votes > 10"><tmpl_var name=data_anime_rating><tmpl_else>-</tmpl_if>";
alist[<tmpl_var name=data_anime_id>].votes = "<tmpl_var name=data_anime_votes>";
alist[<tmpl_var name=data_anime_id>].temprating = "<tmpl_if expr="data_anime_tmpvotes > 10"><tmpl_var name=data_anime_tmprating><tmpl_else>-</tmpl_if>";
alist[<tmpl_var name=data_anime_id>].tempvotes = "<tmpl_var name=data_anime_tmpvotes>";
alist[<tmpl_var name=data_anime_id>].reviews = "<tmpl_var name=data_anime_reviews>";
alist[<tmpl_var name=data_anime_id>].rrating = "<tmpl_var name=data_anime_reviewrating>";
alist[<tmpl_var name=data_anime_id>].isComplete = <tmpl_if name=status_anime_iscomplete>true;<tmpl_else>false;</tmpl_if>
alist[<tmpl_var name=data_anime_id>].isWatched = <tmpl_if name=status_anime_iswatched>true;<tmpl_else>false;</tmpl_if>
alist[<tmpl_var name=data_anime_id>].isWishlist = <tmpl_if name=status_anime_isinwishlist>true;<tmpl_else>false;</tmpl_if>
alist[<tmpl_var name=data_anime_id>].wishlistprio = "<tmpl_var name=data_anime_wishlist_priority>";
alist[<tmpl_var name=data_anime_id>].wishlisttypename = "<tmpl_var name=data_anime_wishlist_type_name>";
alist[<tmpl_var name=data_anime_id>].wishlistcomment = "<tmpl_var expr="jsencode(data_anime_wishlist_comment)">";
alist[<tmpl_var name=data_anime_id>].hasAwards = <tmpl_if name=status_anime_hasawards>true;<tmpl_else>false;</tmpl_if>
alist[<tmpl_var name=data_anime_id>].aliases = "";
alist[<tmpl_var name=data_anime_id>].shortNames = "";
<tmpl_loop name=loop_anime_title_alias>alist[<tmpl_var name=data_anime_id>].aliases += "<tmpl_var expr="jsencode(data_anime_title_alias_name)"><tmpl_if name="__LAST__"><tmpl_else> </tmpl_if>";</tmpl_loop>
<tmpl_loop name=loop_anime_title_short>alist[<tmpl_var name=data_anime_id>].shortNames += "<tmpl_var expr="jsencode(data_anime_title_short_name)"><tmpl_if name="__LAST__"><tmpl_else> </tmpl_if>";</tmpl_loop>
</tmpl_loop>
var num_aids = aids.length;
