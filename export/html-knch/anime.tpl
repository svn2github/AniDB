<tmpl_loop name=loop_anime >var xa = new Object();
xa.ID = <tmpl_var name=data_anime_id >;
xa.name = "<tmpl_var expr="jsencode ( data_anime_name )" >";
xa.year = <tmpl_if name=data_anime_year ><tmpl_var name=data_anime_year ><tmpl_else>"-"</tmpl_if>;
xa.prod = "<tmpl_var name=data_anime_producer >";
xa.url = "<tmpl_var name=data_anime_url >";
xa.description = "<tmpl_var expr="jsencode ( data_anime_other )" >";
xa.longDate = "<tmpl_var name=data_anime_date >";
xa.shortDate = "<tmpl_var name=data_anime_date_short >";
xa.longUpdate = "<tmpl_var name=data_anime_update >";
xa.shortUpdate = "<tmpl_var name=data_anime_update_short >";
xa.rating = <tmpl_var name=data_anime_rating >;
xa.votes = <tmpl_var name=data_anime_votes >;
xa.animeNfoID = <tmpl_var name=data_anime_animenfoid >;
xa.animeNfoID2 = "<tmpl_var name=data_anime_animenfoid2 >";
xa.animeNfoUrl = "<tmpl_var name=data_anime_animenfourl >";
xa.aliases = "";
xa.shortNames = "";
xa.genres = "";
<tmpl_loop name=loop_anime_title_alias >xa.aliases += "<tmpl_var name=data_anime_title_alias_name > , "; </tmpl_loop>
<tmpl_loop name=loop_anime_title_short >xa.shortNames += "<tmpl_var name=data_anime_title_short_name > , "; </tmpl_loop>
<tmpl_loop name=loop_anime_genre >xa.genres += "<tmpl_var name=data_anime_genre_name > , "; </tmpl_loop>
xa.e = new Array();
<tmpl_loop name=loop_ep >var e = new Object();
e.ID = <tmpl_var name=data_ep_id >;
e.num = <tmpl_if name=status_ep_state_special >"<tmpl_var name=data_ep_epno >"<tmpl_else><tmpl_var name=data_ep_epno ></tmpl_if>;
e.name = "<tmpl_var expr="jsencode ( data_ep_name )" >";
e.length = <tmpl_var name=data_ep_length >;
e.other = "<tmpl_var expr="jsencode ( data_ep_other )" >";
e.longAired = "<tmpl_var name=data_ep_aired >";
e.shortAired = "<tmpl_var name=data_ep_aired_short >";
e.longDate = "<tmpl_var name=data_ep_date >";
e.shortDate = "<tmpl_var name=data_ep_date_short >";
e.longUpdate = "<tmpl_var name=data_ep_update >";
e.shortUpdate = "<tmpl_var name=data_ep_update_short >";
e.isSpecial = <tmpl_if name=status_ep_state_special >true;<tmpl_else>false;</tmpl_if>
e.isRecap = <tmpl_if name=status_ep_state_recap >true;<tmpl_else>false;</tmpl_if>
e.isOp = <tmpl_if name=status_ep_state_op >true;<tmpl_else>false;</tmpl_if>
e.isEnd = <tmpl_if name=status_ep_state_end >true;<tmpl_else>false;</tmpl_if>
e.isHanger = <tmpl_if name=status_ep_state_hanger >true;<tmpl_else>false;</tmpl_if>
e.isWatched = <tmpl_if name=status_ep_iswatched >true;<tmpl_else>false;</tmpl_if>
e.hasFile = <tmpl_if name=status_ep_hasfile >true;<tmpl_else>false;</tmpl_if>
e.f = new Array();
<tmpl_loop name=loop_file >var f = new Object();
f.ID = <tmpl_var name=data_file_id >;
f.sizeBytes = "<tmpl_var name=data_file_size >";
f.sizeMB = "<tmpl_var name=data_file_size_h >";
f.filetype = "<tmpl_var name=data_file_filetype >";
f.crc = "<tmpl_var name=data_file_crc >";
f.md5 = "<tmpl_var name=data_file_md5 >";
f.ed2kLink = "<tmpl_var expr="jsencode ( data_file_ed2k_link )" >";
f.ed2kHash = "<tmpl_var name=data_file_ed2k_hash >";
f.ed2kName = "<tmpl_var expr="jsencode ( data_file_ed2k_name )" >";
f.groupID = <tmpl_var name=data_file_group_id >;
f.groupName = "<tmpl_var name=data_file_group_name >";
f.longReleased = "<tmpl_var name=data_file_released >";
f.shortReleased = "<tmpl_var name=data_file_released_short >";
f.qualID = <tmpl_var name=data_file_qual_id >;
f.qualName = "<tmpl_var name=data_file_qual_name >";
f.resID = <tmpl_var name=data_file_res_id >;
f.resName = "<tmpl_var name=data_file_res_name >";
f.audioBr = <tmpl_var name=data_file_abitrate >;
f.audioCodecID = <tmpl_var name=data_file_acodec_id >;
f.audioCodecName = "<tmpl_var name=data_file_acodec_name >";
f.videoBr = <tmpl_var name=data_file_vbitrate >;
f.videoCodecID = <tmpl_var name=data_file_vcodec_id >;
f.videoCodecName = "<tmpl_var name=data_file_vcodec_name >";
f.langID = <tmpl_var name=data_file_lang_id >;
f.langName = "<tmpl_var name=data_file_lang_name >";
f.subID = <tmpl_var name=data_file_sub_id >;
f.subName = "<tmpl_var name=data_file_sub_name >";
f.typeID = <tmpl_var name=data_file_type_id >;
f.typeName = "<tmpl_var name=data_file_type_name >";
f.other = "<tmpl_var expr="jsencode ( data_file_other )" >";
f.longDate = "<tmpl_var name=data_file_date >";
f.shortDate = "<tmpl_var name=data_file_date_short >";
f.longUpdate = "<tmpl_var name=data_file_update >";
f.shortUpdate = "<tmpl_var name=data_file_update_short >";
f.storage = "<tmpl_var name=data_file_storage >";
f.source = "<tmpl_var name=data_file_source >";
f.longViewdate = "<tmpl_var name=data_file_viewdate >";
f.shortViewdate = "<tmpl_var name=data_file_viewdate_short >";
f.stateIcon = "<tmpl_var expr="jsencode ( data_file_mystate_icon )" >";
f.isCrcOk = <tmpl_if name=status_file_state_crcok >true;<tmpl_else>false;</tmpl_if>
f.isCrcFailed = <tmpl_if name=status_file_state_crcfailed >true;<tmpl_else>false;</tmpl_if>
f.isV2 = <tmpl_if name=status_file_state_isv2 >true;<tmpl_else>false;</tmpl_if>
f.isV3 = <tmpl_if name=status_file_state_isv3 >true;<tmpl_else>false;</tmpl_if>
f.isV4 = <tmpl_if name=status_file_state_isv4 >true;<tmpl_else>false;</tmpl_if>
f.isV5 = <tmpl_if name=status_file_state_isv5 >true;<tmpl_else>false;</tmpl_if>
f.isWatched = <tmpl_if name=status_file_iswatched >true;<tmpl_else>false;</tmpl_if>
f.isUnknown = <tmpl_if name=status_file_mystate_unknown >true;<tmpl_else>false;</tmpl_if>
f.isOnHdd = <tmpl_if name=status_file_mystate_onhdd >true;<tmpl_else>false;</tmpl_if>
f.isOnCd = <tmpl_if name=status_file_mystate_oncd >true;<tmpl_else>false;</tmpl_if>
f.isDeleted = <tmpl_if name=status_file_mystate_deleted >true;<tmpl_else>false;</tmpl_if>
f.isShared = <tmpl_if name=status_file_mystate_shared >true;<tmpl_else>false;</tmpl_if>
f.isRelease = <tmpl_if name=status_file_mystate_release >true;<tmpl_else>false;</tmpl_if>
e.f.push ( f );
</tmpl_loop>xa.e.push ( e );
</tmpl_loop></tmpl_loop>