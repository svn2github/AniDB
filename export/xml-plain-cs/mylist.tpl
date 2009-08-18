<?xml version="1.0" encoding="utf-8"?>
<MyList>
<User Id="<tmpl_var name=global_user_id>"
Name="<tmpl_var expr="jsencodexmlcs(global_user)">"
Date="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(global_date,6,4),substr(global_date,3,2),substr(global_date,0,2),substr(global_date,11,5))">"
AnimeCount="<tmpl_var name=global_animecount>"
EpCount="<tmpl_var name=global_epcount>"
FileCount="<tmpl_var name=global_filecount>"
ByteCount="<tmpl_var expr="replace('\.','',global_bytecount)">"
AnimeDBUrl="<tmpl_var expr="jsencodexmlcs(global_animedburl)">"
AnimeAdded="<tmpl_var name=global_userstat_animeadded>"
EpsAdded="<tmpl_var name=global_userstat_epsadded>"
FilesAdded="<tmpl_var name=global_userstat_filesadded>"
GroupsAdded="<tmpl_var name=global_userstat_groupsadded>"
<tmpl_if expr="length(global_userstat_lamefiles)>0">LameFiles="<tmpl_var name=global_userstat_lamefiles>"</tmpl_if>
<tmpl_if expr="length(global_userstat_lamefilesp)>0">LameFilesP="<tmpl_var expr="replace('%','',global_userstat_lamefilesp)">"</tmpl_if>
IndependenceP="<tmpl_var expr="replace('%','',global_userstat_independencep)">"
LeechP="<tmpl_var expr="replace('%','',global_userstat_leechp)">"
Reviews="<tmpl_var name=global_userstat_reviews>"
Votes="<tmpl_var name=global_userstat_votes>"
TotalViewedP="<tmpl_var expr="replace('%','',global_userstat_totalviewedp)">"
TotalOwnedP="<tmpl_var expr="replace('%','',global_userstat_totalownedp)">"
OwnViewedP="<tmpl_var expr="replace('%','',global_userstat_ownviewedp)">"
ViewedEpCnt="<tmpl_var name=global_userstat_viewedepcnt>"
/>
<tmpl_loop name=global_genren_loop><GenreN Id="<tmpl_var name=global_genren_id>" Name="<tmpl_var expr="jsencodexmlcs(global_genren_name)">" ParentId="<tmpl_var name=global_genren_parentid>" IsHentai="<tmpl_var name=global_genren_ishentai>"/>
</tmpl_loop><tmpl_loop name=loop_anime><Anime>
	<Id><tmpl_var name=data_anime_id></Id>
	<Eps><tmpl_var name=data_anime_eps></Eps>
	<EpsSpecial><tmpl_var name=data_anime_eps_special></EpsSpecial>
<!--<EpsTotal><tmpl_var name=data_anime_eps_total></EpsTotal>-->
	<YearStart><tmpl_var expr="substr(data_anime_year,0,4)"></YearStart>
	<YearEnd><tmpl_var expr="substr(data_anime_year,-4,4)"></YearEnd>
<!--<Producer><![CDATA[<tmpl_var name=data_anime_producer>]]></Producer>-->
	<Url><![CDATA[<tmpl_var name=data_anime_url>]]></Url>
	<Other><![CDATA[<tmpl_var name=data_anime_other>]]></Other>
	<Date><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_date,6,4),substr(data_anime_date,3,2),substr(data_anime_date,0,2),substr(data_anime_date,11,5))"></Date>
<!--<DateShort><tmpl_var name=data_anime_date_short></DateShort>-->
	<Update><tmpl_var name=data_anime_update></Update>
	<UpdateShort><![CDATA[<tmpl_var name=data_anime_update_short>]]></UpdateShort>
	<StartDate><tmpl_var expr="sprintf('%s-%s-%sZ',substr(data_anime_startdate,6,4),substr(data_anime_startdate,3,2),substr(data_anime_startdate,0,2))"></StartDate>
<!--<StartDateShort><tmpl_var name=data_anime_startdate_short></StartDateShort>-->
	<tmpl_if expr="data_anime_enddate!='?'"><EndDate><tmpl_var expr="sprintf('%s-%s-%sZ',substr(data_anime_enddate,6,4),substr(data_anime_enddate,3,2),substr(data_anime_enddate,0,2))"></EndDate></tmpl_if>
<!--<EndDateShort><tmpl_var name=data_anime_enddate_short></EndDateShort>-->
	<Rating><tmpl_if expr="data_anime_rating!='-'"><tmpl_var name=data_anime_rating><tmpl_else>NaN</tmpl_if></Rating>
	<Votes><tmpl_var name=data_anime_votes></Votes>
	<TmpRating><tmpl_if expr="data_anime_tmprating!='-'"><tmpl_var name=data_anime_tmprating><tmpl_else>NaN</tmpl_if></TmpRating>
	<TmpVotes><tmpl_var name=data_anime_tmpvotes></TmpVotes>
	<ReviewRating><tmpl_if expr="data_anime_reviewrating!='-'"><tmpl_var name=data_anime_reviewrating><tmpl_else>NaN</tmpl_if></ReviewRating>
	<Reviews><tmpl_var name=data_anime_reviews></Reviews>
	<TypeId><tmpl_var name=data_anime_type_id></TypeId>
	<TypeName><![CDATA[<tmpl_var name=data_anime_type_name>]]></TypeName>

	<MyEps><tmpl_var name=data_anime_my_eps></MyEps>
	<MyEpsSpecial><tmpl_var name=data_anime_my_eps_special></MyEpsSpecial>
<!--<MyEpsTotal><tmpl_var name=data_anime_my_eps_total></MyEpsTotal>-->
	<MyWatchedEps><tmpl_var name=data_anime_my_watchedeps></MyWatchedEps>
	<MyWatchedEpsSpecial><tmpl_var name=data_anime_my_watchedeps_special></MyWatchedEpsSpecial>
<!--<MyWatchedEpsTotal><tmpl_var name=data_anime_my_watchedeps_total></MyWatchedEpsTotal>-->
	<MyUnwatchedEps><tmpl_var name=data_anime_my_unwatchedeps></MyUnwatchedEps>
	<MyUnwatchedEpsSpecial><tmpl_var name=data_anime_my_unwatchedeps_special></MyUnwatchedEpsSpecial>
<!--<MyUnwatchedEpsTotal><tmpl_var name=data_anime_my_unwatchedeps_total></MyUnwatchedEpsTotal>-->
	<MySize><tmpl_var expr="replace('\.','',data_anime_my_size)"></MySize>
<!--<MySizeH><tmpl_var name=data_anime_my_size_h></MySizeH>-->
    <IsComplete><tmpl_var name=status_anime_iscomplete></IsComplete>
    <IsWatched><tmpl_var name=status_anime_iswatched></IsWatched>

	<Name><![CDATA[<tmpl_var name=data_anime_name>]]></Name>
	<NameLangName><![CDATA[<tmpl_var name=data_anime_name_langname>]]></NameLangName>
	<NameLangSName><![CDATA[<tmpl_var name=data_anime_name_langsname>]]></NameLangSName>
	<tmpl_if name=status_anime_title_has_jap_kanji><TitleJapKanji><![CDATA[<tmpl_var name=data_anime_title_jap_kanji>]]></TitleJapKanji></tmpl_if>
<!--<TitleHasJapKanji><tmpl_var name=status_anime_title_has_jap_kanji></TitleHasJapKanji>-->
	<tmpl_if name=status_anime_title_has_eng><TitleEng><![CDATA[<tmpl_var name=data_anime_title_eng>]]></TitleEng></tmpl_if>
<!--<TitleHasEng><tmpl_var name=status_anime_title_has_eng></TitleHasEng>-->
	<tmpl_if name=status_anime_title_has_other><TitleOther><![CDATA[<tmpl_var name=data_anime_title_other>]]></TitleOther></tmpl_if>
<!--<TitleHasOther><tmpl_var name=status_anime_title_has_other></TitleHasOther>-->

	<tmpl_if name=status_anime_hasawards><HasAwards>1</HasAwards></tmpl_if>
	<tmpl_if name=status_anime_hasawards><AwardIcons><![CDATA[<tmpl_var name=data_anime_awardicons>]]></AwardIcons></tmpl_if>

	<MyState><tmpl_var name=data_anime_mystate></MyState>
	<MyStateString><![CDATA[<tmpl_var name=data_anime_mystate_string>]]></MyStateString>
	<MyStateIcon><![CDATA[<tmpl_var name=data_anime_mystate_icon>]]></MyStateIcon>
	<MyStateUndef><tmpl_var name=status_anime_mystate_undef></MyStateUndef>
	<MyStateUnknown><tmpl_var name=status_anime_mystate_unknown></MyStateUnknown>
	<MyStateOnHdd><tmpl_var name=status_anime_mystate_onhdd></MyStateOnHdd>
	<MyStateOnCD><tmpl_var name=status_anime_mystate_oncd></MyStateOnCD>
	<MyStateDeleted><tmpl_var name=status_anime_mystate_deleted></MyStateDeleted>

	<IsInWishList><tmpl_var name=status_anime_isinwishlist></IsInWishList>
	<tmpl_if name=status_anime_isinwishlist>
		<WishListType><tmpl_var name=data_anime_wishlist_type></WishListType>
		<WishListTypeName><![CDATA[<tmpl_var name=data_anime_wishlist_type_name>]]></WishListTypeName>
		<WishListPriority><tmpl_if name=data_anime_wishlist_priority><tmpl_var name=data_anime_wishlist_priority><tmpl_else>0</tmpl_if></WishListPriority>
		<WishListPriorityName><![CDATA[<tmpl_var name=data_anime_wishlist_priority_name>]]></WishListPriorityName>
		<WishListComment><![CDATA[<tmpl_var name=data_anime_wishlist_comment>]]></WishListComment>
	</tmpl_if>
	<MyIsVoted><tmpl_var name=status_anime_my_isvoted></MyIsVoted>
	<tmpl_if name=status_anime_my_isvoted>
		<MyVote><tmpl_if name=status_anime_my_isvoted><tmpl_var name=data_anime_my_vote><tmpl_else>NaN</tmpl_if></MyVote>
		<MyVoteDate><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_my_vote_date,6,4),substr(data_anime_my_vote_date,3,2),substr(data_anime_my_vote_date,0,2),substr(data_anime_my_vote_date,11,5))"></MyVoteDate>
	<!--<MyVoteDateShort><tmpl_var name=data_anime_my_vote_date_short></MyVoteDateShort>-->
	</tmpl_if>
	<MyIsTmpVoted><tmpl_var name=status_anime_my_istmpvoted></MyIsTmpVoted>
	<tmpl_if name=status_anime_my_istmpvoted>
		<MyTmpVote><tmpl_var name=data_anime_my_tmpvote></MyTmpVote>
		<MyTmpVoteDate><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_my_tmpvote_date,6,4),substr(data_anime_my_tmpvote_date,3,2),substr(data_anime_my_tmpvote_date,0,2),substr(data_anime_my_tmpvote_date,11,5))"></MyTmpVoteDate>
	<!--<MyTmpVoteDateShort><tmpl_var name=data_anime_my_tmpvote_date_short></MyTmpVoteDateShort>-->
	</tmpl_if>
<tmpl_loop name=loop_anime_genren><GenreN
Id="<tmpl_var name=data_anime_genren_id>"
IsHentai="<tmpl_var name=data_anime_genren_ishentai>"
Name="<tmpl_var expr="jsencodexmlcs(data_anime_genren_name)">"
ParentId="<tmpl_var name=data_anime_genren_parentid>"
Weight="<tmpl_var name=data_anime_genren_weight>"
WeightName="<tmpl_var name=data_anime_genren_weight_name>"
/>
</tmpl_loop><tmpl_loop name=loop_anime_tag><Tag Id="<tmpl_var name=data_anime_tag_id>" Name="<tmpl_var expr="jsencodexmlcs(data_anime_tag_name)">" Date="<tmpl_var name=data_anime_tag_date>"/>
</tmpl_loop><tmpl_loop name=loop_anime_company><Company
Id="<tmpl_var name=data_anime_company_id>"
Name="<tmpl_var expr="jsencodexmlcs(data_anime_company_name)">"
ShortName="<tmpl_var expr="jsencodexmlcs(data_anime_company_shortname)">"
OtherName="<tmpl_var expr="jsencodexmlcs(data_anime_company_othername)">"
Type="<tmpl_var name=data_anime_company_type>"
TypeName="<tmpl_var expr="jsencodexmlcs(data_anime_company_type_name)">"
Url="<tmpl_var expr="jsencodexmlcs(data_anime_company_url)">"
PicUrl="<tmpl_var expr="jsencodexmlcs(data_anime_company_picurl)">"
APType="<tmpl_var name=data_anime_company_aptype>"
APTypeName="<tmpl_var expr="jsencodexmlcs(data_anime_company_aptype_name)">"
/>
</tmpl_loop><tmpl_loop name=loop_anime_groups><Group
Id="<tmpl_var name=data_anime_group_id>"
AGId="<tmpl_var name=data_anime_group_agid>"
Name="<tmpl_var expr="jsencodexmlcs(data_anime_group_name)">"
SName="<tmpl_var expr="jsencodexmlcs(data_anime_group_sname)">"
State="<tmpl_var expr="jsencodexmlcs(data_anime_group_state)">"
EpRange="<tmpl_var expr="jsencodexmlcs(data_anime_group_eprange)">"
LastEp="<tmpl_var name=data_anime_group_lastep>"
LastUp="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_group_lastup,6,4),substr(data_anime_group_lastup,3,2),substr(data_anime_group_lastup,0,2),substr(data_anime_group_lastup,11,5))">"
Votes="<tmpl_var name=data_anime_group_votes>"
Rating="<tmpl_if expr="data_anime_group_rating!='-'"><tmpl_var name=data_anime_group_rating><tmpl_else>NaN</tmpl_if>"
Cmts="<tmpl_var name=data_anime_group_cmts>"
EpCnt="<tmpl_var name=data_anime_group_epcnt>"
SEpCnt="<tmpl_var name=data_anime_group_sepcnt>"
MyVote="<tmpl_if expr="data_anime_group_myvote!='-'"><tmpl_var name=data_anime_group_myvote><tmpl_else>NaN</tmpl_if>"
<tmpl_if expr="data_anime_group_myvote_date!='-'">MyVoteDate="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_group_myvote_date,6,4),substr(data_anime_group_myvote_date,3,2),substr(data_anime_group_myvote_date,0,2),substr(data_anime_group_myvote_date,11,5))">"</tmpl_if>
>
<tmpl_loop name=data_anime_group_alangs_loop><ALang Id="<tmpl_var name=data_anime_group_alang_id>" Name="<tmpl_var expr="jsencodexmlcs(data_anime_group_alang_name)">" SName="<tmpl_var expr="jsencodexmlcs(data_anime_group_alang_sname)">"/>
</tmpl_loop><tmpl_loop name=data_anime_group_slangs_loop><SLang Id="<tmpl_var name=data_anime_group_slang_id>" Name="<tmpl_var expr="jsencodexmlcs(data_anime_group_slang_name)">" SName="<tmpl_var expr="jsencodexmlcs(data_anime_group_slang_sname)">"/>
</tmpl_loop></Group>
</tmpl_loop><tmpl_loop name=loop_ep><Ep Id="<tmpl_var name=data_ep_id>"
EpNo="<tmpl_var expr="jsencodexmlcs(data_ep_epno)">"
<tmpl_if name=status_ep_hasname_english>Name="<tmpl_var expr="jsencodexmlcs(data_ep_name)">"</tmpl_if>
<tmpl_if name=status_ep_hasname_romaji>NameRomaji="<tmpl_var expr="jsencodexmlcs(data_ep_name_romaji)">"</tmpl_if>
<tmpl_if name=status_ep_hasname_kanji>NameKanji="<tmpl_var expr="jsencodexmlcs(data_ep_name_kanji)">"</tmpl_if>
Length="<tmpl_var name=data_ep_length>"
<tmpl_if expr="data_ep_aired!='-'">Aired="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_aired,6,4),substr(data_ep_aired,3,2),substr(data_ep_aired,0,2),substr(data_ep_aired,11,5))">"</tmpl_if>
Other="<tmpl_var expr="jsencodexmlcs(data_ep_other)">"
Rating="<tmpl_if expr="data_ep_rating!='-'"><tmpl_var name=data_ep_rating><tmpl_else>NaN</tmpl_if>"
Votes="<tmpl_var name=data_ep_votes>"

MyVote="<tmpl_if expr="data_ep_myvote!='-'"><tmpl_var name=data_ep_myvote><tmpl_else>NaN</tmpl_if>"
<tmpl_if expr="data_ep_myvote_date!='-'">MyVoteDate="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_myvote_date,6,4),substr(data_ep_myvote_date,3,2),substr(data_ep_myvote_date,0,2),substr(data_ep_myvote_date,11,5))">"</tmpl_if>
Date="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_date,6,4),substr(data_ep_date,3,2),substr(data_ep_date,0,2),substr(data_ep_date,11,5))">"
<tmpl_if expr="data_ep_update!='-'">Update="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_update,6,4),substr(data_ep_update,3,2),substr(data_ep_update,0,2),substr(data_ep_update,11,5))">"</tmpl_if>
State="<tmpl_var name=data_ep_state>"
<tmpl_if name=status_ep_state_special>StateSpecial="1"</tmpl_if>
<tmpl_if name=status_ep_state_recap>StateRecap="1"</tmpl_if>
<tmpl_if name=status_ep_state_op>StateOp="1"</tmpl_if>
<tmpl_if name=status_ep_state_end>StateEnd="1"</tmpl_if>
<tmpl_if name=status_ep_iswatched>StateIsWatched="1"</tmpl_if>
<tmpl_if name=status_ep_hasfile>StateHasFile="1"</tmpl_if>
MyState="<tmpl_var name=data_ep_mystate>"
MyStateString="<tmpl_var expr="jsencodexmlcs(data_ep_mystate_string)">"

<tmpl_if name=status_ep_mystate_undef>MyStateUndef="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_unknown>MyStateUnknown="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_onhdd>MyStateOnHdd="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_oncd>MyStateOnCD="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_deleted>MyStateDeleted="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_release>MyStateRelease="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_shared>MyStateShared="1"</tmpl_if>
<tmpl_if name=status_ep_state_hanger>MyStateHanger="1"</tmpl_if>
>
<tmpl_loop name=loop_file><File>
	<Id><tmpl_var name=data_file_id></Id>
	<Size><tmpl_var name=data_file_size_plain></Size>
	<IsGeneric><tmpl_var name=status_file_isgeneric></IsGeneric>
	<FileType><tmpl_var name=data_file_filetype></FileType>
	<Crc><tmpl_var name=data_file_crc></Crc>
	<Md5><tmpl_var name=data_file_md5></Md5>
	<Sha1><tmpl_var name=data_file_sha1></Sha1>
	<Tth><tmpl_var name=data_file_tth></Tth>
	<Ed2kLink><![CDATA[<tmpl_var name=data_file_ed2k_link>]]></Ed2kLink>
	<Ed2kHash><tmpl_var name=data_file_ed2k_hash></Ed2kHash>
	<Ed2kName><![CDATA[<tmpl_var name=data_file_ed2k_name>]]></Ed2kName>
	<GroupId><tmpl_var name=data_file_group_id></GroupId>
	<GroupName><![CDATA[<tmpl_var name=data_file_group_name>]]></GroupName>
	<GroupShortName><![CDATA[<tmpl_var name=data_file_group_shortname>]]></GroupShortName>
	<tmpl_if expr="data_file_released!='-'"><ReleaseD><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_file_released,6,4),substr(data_file_released,3,2),substr(data_file_released,0,2),substr(data_file_released,11,5))"></ReleaseD></tmpl_if>
	<QualId><tmpl_var name=data_file_qual_id></QualId>
	<QualName><tmpl_var name=data_file_qual_name></QualName>
	<ResName><tmpl_var name=data_file_res_name></ResName>
	<VidCnt><tmpl_var name=data_file_vidcnt></VidCnt>
	<AR><tmpl_var name=data_file_ar></AR>
	<ARName><tmpl_var name=data_file_ar_name></ARName>
	<Fps><tmpl_var name=data_file_fps></Fps>
	<VBitRate><tmpl_var name=data_file_vbitrate></VBitRate>
	<VCodecId><tmpl_var name=data_file_vcodec_id></VCodecId>
	<VCodecName><tmpl_var name=data_file_vcodec_name></VCodecName>
	<VFlags><tmpl_var name=data_file_vflags></VFlags>
<tmpl_loop name=loop_data_file_filerel><FileRel FId="<tmpl_var expr="jsencodexmlcs(data_file_filerel_fid)">" OtherFId="<tmpl_var expr="jsencodexmlcs(data_file_filerel_otherfid)">" Type="<tmpl_var expr="jsencodexmlcs(data_file_filerel_type)">"/>
</tmpl_loop></File>
</tmpl_loop><tmpl_loop name=loop_data_ep_fileeprel><FileEpRel FId="<tmpl_var expr="jsencodexmlcs(data_ep_fileeprel_fid)">" EId="<tmpl_var expr="jsencodexmlcs(data_ep_fileeprel_eid)">" StartP="<tmpl_var expr="jsencodexmlcs(data_ep_fileeprel_startp)">" EndP="<tmpl_var expr="jsencodexmlcs(data_ep_fileeprel_endp)">"/>
</tmpl_loop></Ep>
</tmpl_loop></Anime>
</tmpl_loop></MyList>
<!-- TODO:
data_anime_allcinemaid
data_anime_allcinemaurl
data_anime_animenfoid
data_anime_animenfourl
data_anime_animenfourl_name
data_anime_animeplanetid
data_anime_animeplaneturl
data_anime_annid
data_anime_annurl
data_anime_award_id
data_anime_award_name
data_anime_award_type
data_anime_award_type_id
data_anime_award_type_img
data_anime_award_type_name
data_anime_award_url
data_anime_name_langid
data_anime_title_alias_langid
data_anime_title_alias_langname
data_anime_title_alias_langsname
data_anime_title_alias_name
data_anime_title_eng
data_anime_title_jap_kanji
data_anime_title_langid
data_anime_title_langname
data_anime_title_langsname
data_anime_title_name
data_anime_title_official_langid
data_anime_title_official_langname
data_anime_title_official_langsname
data_anime_title_official_name
data_anime_title_other
data_anime_title_short_langid
data_anime_title_short_langname
data_anime_title_short_langsname
data_anime_title_short_name
data_anime_title_type_id
data_anime_title_type_string
data_ep_mystate_icon
data_ep_title_langid
data_ep_title_langname
data_ep_title_langsname
data_ep_title_name
data_ep_titles
data_file_abitrate
data_file_acodec_id
data_file_acodec_name
data_file_atype
data_file_atype_name
data_file_audcnt
data_file_chantype
data_file_chantype_name
data_file_date
data_file_date_short
data_file_ed
data_file_lang_id
data_file_lang_name
data_file_lang_sname
data_file_ldate
data_file_ldate_short
data_file_length
data_file_lid
data_file_md
data_file_myfilestate
data_file_myfilestate_icon
data_file_myfilestate_string
data_file_myother
data_file_mystate
data_file_mystate_icon
data_file_mystate_string
data_file_other
data_file_sflags
data_file_sha
data_file_size
data_file_size_h
data_file_size_plain
data_file_source
data_file_state
data_file_state_versionname
data_file_storage
data_file_stype
data_file_stypename
data_file_sub_id
data_file_sub_name
data_file_sub_sname
data_file_subcnt
data_file_type
data_file_type_id
data_file_type_name
data_file_typename
data_file_update
data_file_update_short
data_file_viewdate
data_file_viewdate_short
loop_anime_award_types
loop_anime_awards
loop_anime_title_alias
loop_anime_title_official
loop_anime_title_short
loop_anime_titles
status_anime_allcinema_listed
status_anime_animeplanet_listed
status_anime_ann_listed
status_anime_isrestricted
status_anime_mystate_release
status_anime_mystate_shared
status_file_hascomment
status_file_iswatched
status_file_myfilestate_invalidcrc
status_file_myfilestate_normal
status_file_myfilestate_ondvd
status_file_myfilestate_ontv
status_file_myfilestate_onvhs
status_file_myfilestate_other
status_file_myfilestate_selfedited
status_file_myfilestate_selfripped
status_file_myfilestate_theater
status_file_mystate_deleted
status_file_mystate_oncd
status_file_mystate_onhdd
status_file_mystate_release
status_file_mystate_shared
status_file_mystate_unknown
status_file_state_crcfailed
status_file_state_crcok
status_file_state_crcunverified
status_file_state_isv
-->