<tmpl_loop name=loop_anime>anidb = "<tmpl_var name=global_animedburl>";

var xa = new Object();
xa.id = <tmpl_var name=data_anime_id>;
xa.eps = <tmpl_var name=data_anime_eps>;
xa.name = "<tmpl_var expr="jsencode ( data_anime_name )">";
xa.year = "<tmpl_var name=data_anime_year>";
xa.date_start = "<tmpl_var name=data_anime_startdate>";
xa.date_start_short = "<tmpl_var name=data_anime_startdate_short>";
xa.date_end = "<tmpl_var name=data_anime_enddate>";
xa.date_end_short = "<tmpl_var name=data_anime_enddate_short>";
xa.prod = "<tmpl_var name=data_anime_producer>";
xa.url = "<tmpl_var name=data_anime_url>";
xa.type = "<tmpl_var name=data_anime_type_id>";
xa.typename = "<tmpl_var name=data_anime_type_name>";
xa.state = "<tmpl_var name=data_anime_mystate>";
xa.statename = "<tmpl_var name=data_anime_mystate_string>";
xa.isComplete = <tmpl_if name=status_anime_iscomplete>true;<tmpl_else>false;</tmpl_if>
xa.isWatched = <tmpl_if name=status_anime_iswatched>true;<tmpl_else>false;</tmpl_if>
xa.hasAwards = <tmpl_if name=status_anime_hasawards>true;<tmpl_else>false;</tmpl_if>
xa.HasMyVote = <tmpl_if name=status_anime_my_isvoted>true;<tmpl_else>false;</tmpl_if>
xa.HasMyTmpVote = <tmpl_if name=status_anime_my_istmpvoted>true;<tmpl_else>false;</tmpl_if>
xa.myVote = "<tmpl_var name=data_anime_my_vote>";
xa.myTmpVote = "<tmpl_var name=data_anime_my_tmpvote>";
xa.eps = <tmpl_var name=data_anime_eps>;
xa.myEps = <tmpl_var name=data_anime_my_eps>;
xa.myWeps = <tmpl_var name=data_anime_my_watchedeps>;
xa.mySEps = <tmpl_var name=data_anime_my_eps_special>;
xa.myWSEps = <tmpl_var name=data_anime_my_watchedeps_special>;
xa.size = "<tmpl_var name=data_anime_my_size>";
xa.size_h = "<tmpl_var name=data_anime_my_size_h>";
xa.description = "<tmpl_var expr="jsencode ( data_anime_other )">";
xa.longDate = "<tmpl_var name=data_anime_date>";
xa.shortDate = "<tmpl_var name=data_anime_date_short>";
xa.longUpdate = "<tmpl_var name=data_anime_update>";
xa.shortUpdate = "<tmpl_var name=data_anime_update_short>";
xa.rating = "<tmpl_var name=data_anime_rating>";
xa.votes = <tmpl_var name=data_anime_votes>;
xa.rrating = "<tmpl_var name=data_anime_reviewrating>";
xa.reviews = <tmpl_var name=data_anime_reviews>;
xa.tmpRating = "<tmpl_var name=data_anime_tmprating>";
xa.tmpVotes = <tmpl_var name=data_anime_tmpvotes>;
xa.animeNfoID = <tmpl_var name=data_anime_animenfoid>;
xa.animeNfoID2 = "<tmpl_var name=data_anime_animenfoid2>";
xa.animeNfoUrl = "<tmpl_var name=data_anime_animenfourl>";
xa.animeNfoName = "<tmpl_var name=data_anime_animenfourl_name>";
xa.aniplaneturl = "<tmpl_var name=data_anime_animeplaneturl>";
xa.japkanji = "<tmpl_if name=status_anime_title_has_jap_kanji><tmpl_var expr="jsencode ( data_anime_title_jap_kanji)"><tmpl_else>-</tmpl_if>";
xa.eng = "<tmpl_if name=status_anime_title_has_eng><tmpl_var expr="jsencode ( data_anime_title_eng)"><tmpl_else>-</tmpl_if>";
xa.isWishListed = <tmpl_if name=status_anime_isinwishlist>true;<tmpl_else>false;</tmpl_if>
xa.wishlist_type = "<tmpl_var name=data_anime_wishlist_type>";
xa.wishlist_type_name = "<tmpl_var name=data_anime_wishlist_type_name>";
xa.wishlist_priority = "<tmpl_var name=data_anime_wishlist_priority>";
xa.wishlist_priority_name = "<tmpl_var name=data_anime_wishlist_priority_name>";
xa.wishlist_comment = "<tmpl_var name=data_anime_wishlist_comment>";
xa.aliases = "";
xa.shortNames = "";
xa.genres = "";
xa.awards = new Array();
<tmpl_loop name=loop_anime_title_alias>xa.aliases += "<tmpl_var name=data_anime_title_alias_name><tmpl_if name="__LAST__"><tmpl_else>, </tmpl_if>";</tmpl_loop>
<tmpl_loop name=loop_anime_title_short>xa.shortNames += "<tmpl_var name=data_anime_title_short_name><tmpl_if name="__LAST__"><tmpl_else>, </tmpl_if>";</tmpl_loop>
<tmpl_loop name=loop_anime_genre>xa.genres += "<tmpl_var name=data_anime_genre_name><tmpl_if name="__LAST__"><tmpl_else>, </tmpl_if>";</tmpl_loop>
<tmpl_loop name=loop_anime_awards>var aurl = "<tmpl_var name=data_anime_award_url>";
var aid = "<tmpl_var name=data_anime_award_id>";
var aname = "<tmpl_var name=data_anime_award_name>";
xa.awards.push (aurl,aid,aname );
</tmpl_loop>
xa.enums = new Array();
<tmpl_loop name=loop_ep>xa.enums.push ("<tmpl_var name=data_ep_epno>" );
</tmpl_loop>

<tmpl_loop name=loop_ep>var e<tmpl_var name=data_ep_epno> = new Object();
e<tmpl_var name=data_ep_epno>.id = <tmpl_var name=data_ep_id>;
e<tmpl_var name=data_ep_epno>.num = "<tmpl_var name=data_ep_epno>";
e<tmpl_var name=data_ep_epno>.name = "<tmpl_var expr="jsencode ( data_ep_name )">";
e<tmpl_var name=data_ep_epno>.name_kanji = "<tmpl_if name=status_ep_hasname_kanji><tmpl_var expr="jsencode ( data_ep_name_kanji)"><tmpl_else>-</tmpl_if>";
e<tmpl_var name=data_ep_epno>.name_romanji = "<tmpl_if name=status_ep_hasname_romanji><tmpl_var expr="jsencode ( data_ep_name_romanji)"><tmpl_else>-</tmpl_if>";
e<tmpl_var name=data_ep_epno>.elength = <tmpl_var name=data_ep_length>;
e<tmpl_var name=data_ep_epno>.other = "<tmpl_var expr="jsencode ( data_ep_other )">";
e<tmpl_var name=data_ep_epno>.longAired = "<tmpl_var name=data_ep_aired>";
e<tmpl_var name=data_ep_epno>.shortAired = "<tmpl_var name=data_ep_aired_short>";
e<tmpl_var name=data_ep_epno>.longDate = "<tmpl_var name=data_ep_date>";
e<tmpl_var name=data_ep_epno>.shortDate = "<tmpl_var name=data_ep_date_short>";
e<tmpl_var name=data_ep_epno>.longUpdate = "<tmpl_var name=data_ep_update>";
e<tmpl_var name=data_ep_epno>.shortUpdate = "<tmpl_var name=data_ep_update_short>";
e<tmpl_var name=data_ep_epno>.rating = "<tmpl_var name=data_ep_rating>";
e<tmpl_var name=data_ep_epno>.votes = "<tmpl_var name=data_ep_votes>";
e<tmpl_var name=data_ep_epno>.state = "<tmpl_var name=data_ep_state>";
e<tmpl_var name=data_ep_epno>.statename = "<tmpl_var name=data_ep_mystate_string>";
e<tmpl_var name=data_ep_epno>.isWatched = <tmpl_if name=status_ep_iswatched>true;<tmpl_else>false;</tmpl_if>
e<tmpl_var name=data_ep_epno>.hasFile = <tmpl_if name=status_ep_hasfile>true;<tmpl_else>false;</tmpl_if>
e<tmpl_var name=data_ep_epno>.fnums = new Array();

<tmpl_loop name=loop_file>var f<tmpl_var name=data_file_id> = new Object();
e<tmpl_var name=data_ep_epno>.fnums.push (<tmpl_var name=data_file_id> );
f<tmpl_var name=data_file_id>.id = <tmpl_var name=data_file_id>;
f<tmpl_var name=data_file_id>.sizeBytes = "<tmpl_var name=data_file_size>";
f<tmpl_var name=data_file_id>.sizeMB = "<tmpl_var name=data_file_size_h>";
f<tmpl_var name=data_file_id>.filetype = "<tmpl_var name=data_file_filetype>";
f<tmpl_var name=data_file_id>.crc = "<tmpl_var name=data_file_crc>";
f<tmpl_var name=data_file_id>.md5 = "<tmpl_var name=data_file_md5>";
f<tmpl_var name=data_file_id>.sha = "<tmpl_var name=data_file_sha1>";
f<tmpl_var name=data_file_id>.ed2kLink = "<tmpl_var expr="jsencode ( data_file_ed2k_link )">";
f<tmpl_var name=data_file_id>.ed2kHash = "<tmpl_var name=data_file_ed2k_hash>";
f<tmpl_var name=data_file_id>.ed2kName = "<tmpl_var expr="jsencode ( data_file_ed2k_name )">";
f<tmpl_var name=data_file_id>.groupID = <tmpl_var name=data_file_group_id>;
f<tmpl_var name=data_file_id>.groupName = "<tmpl_var name=data_file_group_name>";
f<tmpl_var name=data_file_id>.groupShort = "<tmpl_var name=data_file_group_shortname>";
f<tmpl_var name=data_file_id>.longReleased = "<tmpl_var name=data_file_released>";
f<tmpl_var name=data_file_id>.shortReleased = "<tmpl_var name=data_file_released_short>";
f<tmpl_var name=data_file_id>.qualID = <tmpl_var name=data_file_qual_id>;
f<tmpl_var name=data_file_id>.qualName = "<tmpl_var name=data_file_qual_name>";
f<tmpl_var name=data_file_id>.resName = "<tmpl_var name=data_file_res_name>";
f<tmpl_var name=data_file_id>.audioBr = <tmpl_var name=data_file_abitrate>;
f<tmpl_var name=data_file_id>.audioCodecID = <tmpl_var name=data_file_acodec_id>;
f<tmpl_var name=data_file_id>.audioCodecName = "<tmpl_var name=data_file_acodec_name>";
f<tmpl_var name=data_file_id>.videoBr = <tmpl_var name=data_file_vbitrate>;
f<tmpl_var name=data_file_id>.videoCodecID = <tmpl_var name=data_file_vcodec_id>;
f<tmpl_var name=data_file_id>.videoCodecName = "<tmpl_var name=data_file_vcodec_name>";
f<tmpl_var name=data_file_id>.langID = <tmpl_var name=data_file_lang_id>;
f<tmpl_var name=data_file_id>.langName = "<tmpl_var name=data_file_lang_name>";
f<tmpl_var name=data_file_id>.subID = <tmpl_var name=data_file_sub_id>;
f<tmpl_var name=data_file_id>.subName = "<tmpl_var name=data_file_sub_name>";
f<tmpl_var name=data_file_id>.typeID = <tmpl_var name=data_file_type_id>;
f<tmpl_var name=data_file_id>.typeName = "<tmpl_var name=data_file_type_name>";
f<tmpl_var name=data_file_id>.other = "<tmpl_var expr="jsencode ( data_file_other )">";
f<tmpl_var name=data_file_id>.longDate = "<tmpl_var name=data_file_date>";
f<tmpl_var name=data_file_id>.shortDate = "<tmpl_var name=data_file_date_short>";
f<tmpl_var name=data_file_id>.longUpdate = "<tmpl_var name=data_file_update>";
f<tmpl_var name=data_file_id>.shortUpdate = "<tmpl_var name=data_file_update_short>";
f<tmpl_var name=data_file_id>.storage = "<tmpl_var name=data_file_storage>";
f<tmpl_var name=data_file_id>.source = "<tmpl_var name=data_file_source>";
f<tmpl_var name=data_file_id>.longViewdate = "<tmpl_var name=data_file_viewdate>";
f<tmpl_var name=data_file_id>.shortViewdate = "<tmpl_var name=data_file_viewdate_short>";
f<tmpl_var name=data_file_id>.state = <tmpl_var name=data_file_state>;
f<tmpl_var name=data_file_id>.stateIcon = "<tmpl_var expr="jsencode ( data_file_mystate_icon )">";
f<tmpl_var name=data_file_id>.mystate = "<tmpl_var name=data_file_mystate>";
f<tmpl_var name=data_file_id>.mystatename = "<tmpl_var name=data_file_mystate_string>";
f<tmpl_var name=data_file_id>.myfilestate = "<tmpl_var name=data_file_myfilestate>";
f<tmpl_var name=data_file_id>.myfilestatename = "<tmpl_var name=data_file_myfilestate_string>";
f<tmpl_var name=data_file_id>.isCRCOK = <tmpl_if name=status_file_state_crcok>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.isCRCNO = <tmpl_if name=status_file_state_crcfailed>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.isCRCUN = <tmpl_if name=status_file_state_crcunverified>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.isWatched = <tmpl_if name=status_file_iswatched>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.isV2 = <tmpl_if name=status_file_state_isv2>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.isV3 = <tmpl_if name=status_file_state_isv3>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.isV4 = <tmpl_if name=status_file_state_isv4>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.isV5 = <tmpl_if name=status_file_state_isv5>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.hasStorage = <tmpl_if name=data_file_storage>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.hasSource = <tmpl_if name=data_file_source>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.hasComment = <tmpl_if name=status_file_hascomment>true;<tmpl_else>false;</tmpl_if>
f<tmpl_var name=data_file_id>.isGeneric = <tmpl_if name=status_file_isgeneric>true;<tmpl_else>false;</tmpl_if>
</tmpl_loop></tmpl_loop></tmpl_loop>
