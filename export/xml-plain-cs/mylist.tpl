<?xml version="1.0" encoding="utf-8"?>
<MyList>
<User>
	<Name><![CDATA[<tmpl_var name=global_user>]]></Name>
	<Id><tmpl_var name=global_user_id></Id>
	<Date><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(global_date,6,4),substr(global_date,3,2),substr(global_date,0,2),substr(global_date,11,5))"></Date>
<!--<DateShort><tmpl_var name=global_date_short></DateShort>-->
	<AnimeCount><tmpl_var name=global_animecount></AnimeCount>
	<EpCount><tmpl_var name=global_epcount></EpCount>
	<FileCount><tmpl_var name=global_filecount></FileCount>
	<ByteCount><tmpl_var expr="replace('\.','',global_bytecount)"></ByteCount>
<!--<ByteCountH><tmpl_var name=global_bytecount_h></ByteCountH>-->
	<AnimeDBUrl><![CDATA[<tmpl_var name=global_animedburl>]]></AnimeDBUrl>
	<AnimeAdded><tmpl_var name=global_userstat_animeadded></AnimeAdded>
	<EpsAdded><tmpl_var name=global_userstat_epsadded></EpsAdded>
	<FilesAdded><tmpl_var name=global_userstat_filesadded></FilesAdded>
	<GroupsAdded><tmpl_var name=global_userstat_groupsadded></GroupsAdded>
	<LameFiles><tmpl_var name=global_userstat_lamefiles></LameFiles>
	<LameFilesP><tmpl_var expr="replace('%','',global_userstat_lamefilesp)"></LameFilesP>
	<IndependenceP><tmpl_var expr="replace('%','',global_userstat_independencep)"></IndependenceP>
	<LeechP><tmpl_var expr="replace('%','',global_userstat_leechp)"></LeechP>
	<Reviews><tmpl_var name=global_userstat_reviews></Reviews>
	<Votes><tmpl_var name=global_userstat_votes></Votes>
	<TotalViewedP><tmpl_var expr="replace('%','',global_userstat_totalviewedp)"></TotalViewedP>
	<TotalOwnedP><tmpl_var expr="replace('%','',global_userstat_totalownedp)"></TotalOwnedP>
	<OwnViewedP><tmpl_var expr="replace('%','',global_userstat_ownviewedp)"></OwnViewedP>
	<ViewedEpCnt><tmpl_var expr="replace('%','',global_userstat_viewedepcnt)"></ViewedEpCnt>
</User>
<tmpl_loop name=global_genren_loop><GenreN>
	<Id><tmpl_var name=global_genren_id></Id>
	<Name><![CDATA[<tmpl_var name=global_genren_name>]]></Name>
	<ParentId><tmpl_var name=global_genren_parentid></ParentId>
	<IsHentai><tmpl_var name=global_genren_ishentai></IsHentai>
</GenreN>
</tmpl_loop>
<tmpl_loop name=loop_anime><Anime>
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
	<Rating><tmpl_if expr="data_anime_rating!='-'"><tmpl_var name=data_anime_rating><tmpl_else>NaN</tmpl_if></Rating>-->
	<Votes><tmpl_var name=data_anime_votes></Votes>
	<TmpRating><tmpl_if expr="data_anime_tmprating!='-'"><tmpl_var name=data_anime_tmprating><tmpl_else>NaN</tmpl_if></TmpRating>-->
	<TmpVotes><tmpl_var name=data_anime_tmpvotes></TmpVotes>
	<ReviewRating><tmpl_if expr="data_anime_reviewrating!='-'"><tmpl_var name=data_anime_reviewrating><tmpl_else>NaN</tmpl_if></ReviewRating>-->
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

	<HasAwards><tmpl_if expr="status_anime_hasawards==1">1<tmpl_else>0</tmpl_if></HasAwards>
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

<tmpl_loop name=loop_anime_tag><Tag>
	<Id><tmpl_var name=data_anime_tag_id></Id>
	<Name><![CDATA[<tmpl_var name=data_anime_tag_name>]]></Name>
	<Date><tmpl_var name=data_anime_tag_date></Date>
</Tag>
</tmpl_loop>
<tmpl_loop name=loop_anime_company><Company>
	<Id><tmpl_var name=data_anime_company_id></Id>
	<Name><![CDATA[<tmpl_var name=data_anime_company_name>]]></Name>
	<ShortName><![CDATA[<tmpl_var name=data_anime_company_shortname>]]></ShortName>
	<OtherName><![CDATA[<tmpl_var name=data_anime_company_othername>]]></OtherName>
	<Type><tmpl_var name=data_anime_company_type></Type>
	<Url><![CDATA[<tmpl_var namedata_anime_company_url>]]></Url>
	<PicUrl><![CDATA[<tmpl_var data_anime_company_picurldata_anime_company_othername>]]></PicUrl>
	<APType><tmpl_var name=data_anime_company_aptype></APType>
</Company>
</tmpl_loop>
<tmpl_loop name=loop_anime_groups><Group>
	<Id><tmpl_var name=data_anime_group_id></Id>
	<AGId><tmpl_var name=data_anime_group_agid></AGId>
	<Name><![CDATA[<tmpl_var name=data_anime_group_name>]]></Name>
	<SName><![CDATA[<tmpl_var name=data_anime_group_sname>]]></SName>
	<State><![CDATA[<tmpl_var name=data_anime_group_state>]]></State>
	<LastEp><tmpl_var name=data_anime_group_lastep></LastEp>
	<LastUp><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_group_lastup,6,4),substr(data_anime_group_lastup,3,2),substr(data_anime_group_lastup,0,2),substr(data_anime_group_lastup,11,5))"></LastUp>
	<Votes><tmpl_var name=data_anime_group_votes></Votes>
	<Rating><tmpl_if expr="data_anime_group_rating!='-'"><tmpl_var name=data_anime_group_rating><tmpl_else>NaN</tmpl_if></Rating>
	<Cmts><tmpl_var name=data_anime_group_cmts></Cmts>
	<EpCnt><tmpl_var name=data_anime_group_epcnt></EpCnt>
	<SEpCnt><tmpl_var name=data_anime_group_sepcnt></SEpCnt>
	<MyVote><tmpl_if expr="data_anime_group_myvote!='-'"><tmpl_var name=data_anime_group_myvote><tmpl_else>NaN</tmpl_if></MyVote>
	<tmpl_if expr="data_anime_group_myvote_date!='-'"><MyVoteDate><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_group_myvote_date,6,4),substr(data_anime_group_myvote_date,3,2),substr(data_anime_group_myvote_date,0,2),substr(data_anime_group_myvote_date,11,5))"></MyVoteDate></tmpl_if>
</Group>
</tmpl_loop>
<tmpl_loop name=loop_ep><Ep>
	<Id><tmpl_var name=data_ep_id></Id>
	<EpNo><tmpl_var name=data_ep_epno></EpNo>
	<Name><![CDATA[<tmpl_var name=data_ep_name>]]></Name>
	<tmpl_if name=status_ep_hasname_romaji><NameRomaji><![CDATA[<tmpl_var name=data_ep_name_romaji>]]></NameRomaji></tmpl_if>
	<tmpl_if name=status_ep_hasname_kanji><NameKanji><![CDATA[<tmpl_var name=data_ep_name_kanji>]]></NameKanji></tmpl_if>
	<Length><tmpl_var name=data_ep_length></Length>
	<tmpl_if expr="data_ep_aired!='-'"><Aired><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_aired,6,4),substr(data_ep_aired,3,2),substr(data_ep_aired,0,2),substr(data_ep_aired,11,5))"></Aired></tmpl_if>
	<Other><![CDATA[<tmpl_var name=data_ep_other>]]></Other>
	<Rating><tmpl_if expr="data_ep_rating!='-'"><tmpl_var name=data_ep_rating><tmpl_else>NaN</tmpl_if></Rating>-->
	<Votes><tmpl_var name=data_ep_votes></Votes>
	<MyVote><tmpl_var name=data_ep_votes></MyVote>
	<tmpl_if expr="data_ep_myvote_date!='-'"><MyVoteDate><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_myvote_date,6,4),substr(data_ep_myvote_date,3,2),substr(data_ep_myvote_date,0,2),substr(data_ep_myvote_date,11,5))"></MyVoteDate></tmpl_if>
	<Date><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_date,6,4),substr(data_ep_date,3,2),substr(data_ep_date,0,2),substr(data_ep_date,11,5))"></Date>
	<tmpl_if expr="data_ep_update!='-'"><Update><tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_update,6,4),substr(data_ep_update,3,2),substr(data_ep_update,0,2),substr(data_ep_update,11,5))"></Update></tmpl_if>
	<State><tmpl_var name=data_ep_state></State>-->
	<StateSpecial><tmpl_var name=status_ep_state_special></StateSpecial>
	<StateRecap><tmpl_var name=status_ep_state_recap></StateRecap>
	<StateOp><tmpl_var name=status_ep_state_op></StateOp>
	<StateEnd><tmpl_var name=status_ep_state_end></StateEnd>
	<StateIsWatched><tmpl_var name=status_ep_iswatched></StateIsWatched>
	<StateHasFile><tmpl_var name=status_ep_hasfile></StateHasFile>

	<MyState><tmpl_var name=data_ep_mystate></MyState>
	<tmpl_if expr="data_ep_mystate!=-1">
	<MyStateString><tmpl_var name=data_ep_mystate_string></MyStateString>
	<MyStateIcon><![CDATA[<tmpl_var name=data_ep_mystate_icon>]]></MyStateIcon>
	<MyStateUndef><tmpl_var name=status_ep_mystate_undef></MyStateUndef>
	<MyStateUnknown><tmpl_var name=status_ep_mystate_unknown></MyStateUnknown>
	<MyStateOnHdd><tmpl_var name=status_ep_mystate_onhdd></MyStateOnHdd>
	<MyStateOnCD><tmpl_var name=status_ep_mystate_oncd></MyStateOnCD>
	<MyStateDeleted><tmpl_var name=status_ep_mystate_deleted></MyStateDeleted>
	</tmpl_if>
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
	</File>
	</tmpl_loop>
</Ep>
</tmpl_loop>
</Anime>
</tmpl_loop>

</MyList>
