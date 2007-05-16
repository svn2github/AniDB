var userName = "<tmpl_var name=global_user >";
var longDate = "<tmpl_var name=global_date >";
var shortDate = "<tmpl_var name=global_date_short >";
var animeCount = "<tmpl_var name=global_animecount >";
var episodeCount = "<tmpl_var name=global_epcount >";
var fileCount = "<tmpl_var name=global_filecount >";
var byteCount = "<tmpl_var name=global_bytecount >";
var GByteCount = "<tmpl_var name=global_bytecount_h >";
var aniDBUrl = "<tmpl_var name=global_animedburl >";
var animeArray = new Array();
<tmpl_loop name=loop_anime >
animeArray.push (<tmpl_var name=data_anime_id > );
var a<tmpl_var name=data_anime_id > = new Object();
a<tmpl_var name=data_anime_id >.ID = <tmpl_var name=data_anime_id >;
a<tmpl_var name=data_anime_id >.name = "<tmpl_var name=data_anime_name >";
a<tmpl_var name=data_anime_id >.eps = <tmpl_var name=data_anime_eps >;
a<tmpl_var name=data_anime_id >.myEps = <tmpl_var name=data_anime_my_eps >;
a<tmpl_var name=data_anime_id >.mySpecials = <tmpl_var name=data_anime_my_eps_special >;
a<tmpl_var name=data_anime_id >.myWeps = <tmpl_var name=data_anime_my_watchedeps >;
a<tmpl_var name=data_anime_id >.myWSpecials = <tmpl_var name=data_anime_my_watchedeps_special >;
a<tmpl_var name=data_anime_id >.sizeBytes = "<tmpl_var name=data_anime_my_size >";
a<tmpl_var name=data_anime_id >.sizeGB = "<tmpl_var name=data_anime_my_size_h >";
a<tmpl_var name=data_anime_id >.type = "<tmpl_var name=data_anime_type_name >";
a<tmpl_var name=data_anime_id >.rating = "<tmpl_var name=data_anime_rating >";
a<tmpl_var name=data_anime_id >.shortDate = "<tmpl_var name=data_anime_date_short >";
a<tmpl_var name=data_anime_id >.isComplete = <tmpl_if name=status_anime_iscomplete >true;<tmpl_else>false;</tmpl_if>
a<tmpl_var name=data_anime_id >.isWatched = <tmpl_if name=status_anime_iswatched >true;<tmpl_else>false;</tmpl_if>
</tmpl_loop>
