<?xml version="1.0" encoding="UTF-8" ?>
<!-- xml-singlefile-dataonly anidb.net export template
     Version: 2.1 (21.09.2006)
     Based on Version 1.0 by chriv
     The following tables are in the XML document created by this template:
       user_info (I recomend you use "UserName" as the primary key)
       animes (use "AnimeID" as the primary key)
       aliases (use "Alias" as the primary key; use "AnimeID" for joining tables in queries)
       short_titles (use "ShortName" as the primary key; use "AnimeID" for joining tables in queries)
       award_types (use "AnimeID" and "AwardTypeID" as primary key)
       awards (use "AnimeID" and "AwardID" as primary key)
       genres (use "AnimeID" and "GenreID" as primary key)
       categories (use "AnimeID" and "CategoryID" as primary key)
       tags (use "TagID" as primary key; use "AnimeID" for joining in queries)
       companies (use "AnimeID" and "CompanyID" as primary key)
       episodes (use "EpID" as the primary key; use "AnimeID" for joining tables in queries)
       files (use "FID" as the primary key; use "AnimeID" and "EpID" for joining tables in queries)
     I tried to include all available information from the anidb, but msg me (bsdude) on anidb.net if I missed something -->
<!-- Database : `myanimelist` -->
<myanimelist>
  <!-- Table: `user_info` -->
  <user_info>
    <UserName><![CDATA[<tmpl_var name=global_user>]]></UserName>
    <ExportDateTime><![CDATA[<tmpl_var name=global_date>]]></ExportDateTime>
    <ExportDate><![CDATA[<tmpl_var name=global_date_short>]]></ExportDate>
    <MyAnimeCount><![CDATA[<tmpl_var name=global_animecount>]]></MyAnimeCount>
    <MyEpisodeCount><![CDATA[<tmpl_var name=global_epcount>]]></MyEpisodeCount>
    <MyFileCount><![CDATA[<tmpl_var name=global_filecount>]]></MyFileCount>
    <MyByteCountRaw><![CDATA[<tmpl_var name=global_bytecount>]]></MyByteCountRaw>
    <MyByteCountHuman><![CDATA[<tmpl_var name=global_bytecount_h>]]></MyByteCountHuman>
    <MyAnimesAdded><![CDATA[<tmpl_var name=global_userstat_animeadded>]]></MyAnimesAdded>
    <MyEpisodesAdded><![CDATA[<tmpl_var name=global_userstat_epsadded>]]></MyEpisodesAdded>
    <MyFilesAdded><![CDATA[<tmpl_var name=global_userstat_filesadded>]]></MyFilesAdded>
    <MyGroupsAdded><![CDATA[<tmpl_var name=global_userstat_groupsadded>]]></MyGroupsAdded>
    <MyLameFiles><![CDATA[<tmpl_var name=global_userstat_lamefiles>]]></MyLameFiles>
    <MyLameFilesPercent><![CDATA[<tmpl_var name=global_userstat_lamefilesp>]]></MyLameFilesPercent>
    <MyIndependencePercent><![CDATA[<tmpl_var name=global_userstat_independencep>]]></MyIndependencePercent>
    <MyLeechPercent><![CDATA[<tmpl_var name=global_userstat_leechp>]]></MyLeechPercent>
    <MyReviews><![CDATA[<tmpl_var name=global_userstat_reviews>]]></MyReviews>
    <MyVotes><![CDATA[<tmpl_var name=global_userstat_votes>]]></MyVotes>
    <MyTotalViewedPercent><![CDATA[<tmpl_var name=global_userstat_totalviewedp>]]></MyTotalViewedPercent>
    <MyTotalOwnedPercent><![CDATA[<tmpl_var name=global_userstat_totalownedp>]]></MyTotalOwnedPercent>
    <MyOwnViewedPercent><![CDATA[<tmpl_var name=global_userstat_ownviewedp>]]></MyOwnViewedPercent>
    <MyViewedEpisodeCount><![CDATA[<tmpl_var name=global_userstat_viewedepcnt>]]></MyViewedEpisodeCount>
    <AniDBURL><![CDATA[<tmpl_var name=global_animedburl>]]></AniDBURL>
  </user_info>
  <!-- Table: `animes` -->
  <tmpl_loop name=loop_anime>
  <animes>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <Name><![CDATA[<tmpl_var name=data_anime_name>]]></Name>
    <NameKanji><![CDATA[<tmpl_var name=data_anime_title_jap_kanji>]]></NameKanji>
    <NameEnglish><![CDATA[<tmpl_var name=data_anime_title_eng>]]></NameEnglish>
    <Eps><![CDATA[<tmpl_var name=data_anime_eps>]]></Eps>
    <Year><![CDATA[<tmpl_var name=data_anime_year>]]></Year>
    <URL><![CDATA[<tmpl_var name=data_anime_url>]]></URL>
    <AnimeDescription><![CDATA[<tmpl_var name=data_anime_other>]]></AnimeDescription>
    <DateTime><![CDATA[<tmpl_var name=data_anime_date>]]></DateTime>
    <Date><![CDATA[<tmpl_var name=data_anime_date_short>]]></Date>
    <UpdateTime><![CDATA[<tmpl_var name=data_anime_update>]]></UpdateTime>
    <Update><![CDATA[<tmpl_var name=data_anime_update_short>]]></Update>
    <StartDateTime><![CDATA[<tmpl_var name=data_anime_startdate>]]></StartDateTime>
    <StartDate><![CDATA[<tmpl_var name=data_anime_startdate_short>]]></StartDate>
    <EndDateTime><![CDATA[<tmpl_var name=data_anime_enddate>]]></EndDateTime>
    <EndDate><![CDATA[<tmpl_var name=data_anime_enddate_short>]]></EndDate>
    <Rating><![CDATA[<tmpl_var name=data_anime_rating>]]></Rating>
    <Votes><![CDATA[<tmpl_var name=data_anime_votes>]]></Votes>
    <TempRating><![CDATA[<tmpl_var name=data_anime_tmprating>]]></TempRating>
    <TempVotes><![CDATA[<tmpl_var name=data_anime_tmpvotes>]]></TempVotes> 
    <Reviews><![CDATA[<tmpl_var name=data_anime_reviews>]]></Reviews>
    <ReviewRating><![CDATA[<tmpl_var name=data_anime_reviewrating>]]></ReviewRating> 
    <MyVote><![CDATA[<tmpl_var name=data_anime_my_vote>]]></MyVote>
    <MyTempVote><![CDATA[<tmpl_var name=data_anime_my_tmpvote>]]></MyTempVote>
    <TypeID><![CDATA[<tmpl_var name=data_anime_type_id>]]></TypeID>
    <TypeName><![CDATA[<tmpl_var name=data_anime_type_name>]]></TypeName>
    <NFOID><![CDATA[<tmpl_var name=data_anime_animenfoid>]]></NFOID>
    <NFOID2><![CDATA[<tmpl_var name=data_anime_animenfoid2>]]></NFOID2>
    <NFOURL><![CDATA[<tmpl_var name=data_anime_animenfourl>]]></NFOURL>
    <NFOURLName><![CDATA[<tmpl_var name=data_anime_animenfourl_name>]]></NFOURLName>
    <ANNID><![CDATA[<tmpl_var name=data_anime_annid>]]></ANNID>
    <ANNURL><![CDATA[<tmpl_var name=data_anime_annurl>]]></ANNURL>
    <AllCinemaID><![CDATA[<tmpl_var name=data_anime_allcinemaid>]]></AllCinemaID>
    <AllCinemaURL><![CDATA[<tmpl_var name=data_anime_allcinemaurl>]]></AllCinemaURL>
    <AnimePlanetURL><![CDATA[<tmpl_var name=data_anime_animeplaneturl>]]></AnimePlanetURL>
    <MyEps><![CDATA[<tmpl_var name=data_anime_my_eps>]]></MyEps>
    <MyEpsSpecial><![CDATA[<tmpl_var name=data_anime_my_eps_special>]]></MyEpsSpecial>
    <MyEpsTotal><![CDATA[<tmpl_var name=data_anime_my_eps_total>]]></MyEpsTotal>
    <WatchedEps><![CDATA[<tmpl_var name=data_anime_my_watchedeps>]]></WatchedEps>
    <WatchedSpecial><![CDATA[<tmpl_var name=data_anime_my_watchedeps_special>]]></WatchedSpecial>
    <WatchedTotal><![CDATA[<tmpl_var name=data_anime_my_watchedeps_total>]]></WatchedTotal>
    <UnWatchedEps><![CDATA[<tmpl_var name=data_anime_my_unwatchedeps>]]></UnWatchedEps>
    <UnWatchedSpecial><![CDATA[<tmpl_var name=data_anime_my_unwatchedeps_special>]]></UnWatchedSpecial>
    <UnWatchedTotal><![CDATA[<tmpl_var name=data_anime_my_unwatchedeps_total>]]></UnWatchedTotal>
    <ByteCount><![CDATA[<tmpl_var name=data_anime_my_size>]]></ByteCount>
    <ByteCountHuman><![CDATA[<tmpl_var name=data_anime_my_size_h>]]></ByteCountHuman>
    <MyComplete><![CDATA[<tmpl_var name=status_anime_iscomplete>]]></MyComplete>
    <MyWatched><![CDATA[<tmpl_var name=status_anime_iswatched>]]></MyWatched>
    <MyState><![CDATA[<tmpl_var name=data_anime_mystate>]]></MyState>
    <MyStateString><![CDATA[<tmpl_var name=data_anime_mystate_string>]]></MyStateString>
    <MyStateIcon><![CDATA[<tmpl_var name=data_anime_mystate_icon>]]></MyStateIcon>
    <MyWishlistType><![CDATA[<tmpl_var name=data_anime_wishlist_type>]]></MyWishlistType>
    <MyWishlistTypeName><![CDATA[<tmpl_var name=data_anime_wishlist_type_name>]]></MyWishlistTypeName>
    <MyWishlistPriority><![CDATA[<tmpl_var name=data_anime_wishlist_priority>]]></MyWishlistPriority>
    <MyWishlistPriorityName><![CDATA[<tmpl_var name=data_anime_wishlist_priority_name>]]></MyWishlistPriorityName>
    <MyWishlistComment><![CDATA[<tmpl_var name=data_anime_wishlist_comment>]]></MyWishlistComment>
    <AwardIcons><![CDATA[<tmpl_var name=data_anime_awardicons>]]></AwardIcons>
  </animes>

  <!-- Table: `aliases` -->
  <tmpl_loop name=loop_anime_title_alias>
  <aliases>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <Alias><![CDATA[<tmpl_var name=data_anime_title_alias_name>]]></Alias>
  </aliases>
  </tmpl_loop>

  <!-- Table: `short_titles` -->
  <tmpl_loop name=loop_anime_title_short>
  <short_titles>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <ShortName><![CDATA[<tmpl_var name=data_anime_title_short_name>]]></ShortName>
  </short_titles>
  </tmpl_loop>
  <tmpl_if name=status_anime_hasawards>

  <!-- Table: `award_types` -->
  <tmpl_loop name=loop_anime_award_types>
  <award_types>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <AwardTypeID><![CDATA[<tmpl_var name=data_anime_award_type_id>]]></AwardTypeID>
    <AwardTypeName><![CDATA[<tmpl_var name=data_anime_award_type_name>]]></AwardTypeName>
    <AwardTypeImage><![CDATA[<tmpl_var name=data_anime_award_type_img>]]></AwardTypeImage>
  </award_types>
  </tmpl_loop>

  <!-- Table: `awards` -->
  <tmpl_loop name=loop_anime_awards>
  <awards>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <AwardID><![CDATA[<tmpl_var name=data_anime_award_id>]]></AwardID>
    <AwardName><![CDATA[<tmpl_var name=data_anime_award_name>]]></AwardName>
    <AwardType><![CDATA[<tmpl_var name=data_anime_award_type>]]></AwardType>
    <AwardURL><![CDATA[<tmpl_var name=data_anime_award_url>]]></AwardURL>
    <AwardPicURL><![CDATA[<tmpl_var name=data_anime_award_picurl>]]></AwardPicURL>
  </awards>
  </tmpl_loop>
  </tmpl_if>

  <!-- Table: `genres` -->
  <tmpl_loop name=loop_anime_genre>
  <genres>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <GenreID><![CDATA[<tmpl_var name=data_anime_genre_id>]]></GenreID>
    <GenreName><![CDATA[<tmpl_var name=data_anime_genre_name>]]></GenreName>
  </genres>
  </tmpl_loop>

  <!-- Table: `categories` -->
  <tmpl_loop name=loop_anime_genren>
  <categories>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <CategoryID><![CDATA[<tmpl_var name=data_anime_genren_id>]]></CategoryID>
    <CategoryName><![CDATA[<tmpl_var name=data_anime_genren_name>]]></CategoryName>
    <CategoryParentID><![CDATA[<tmpl_var name=data_anime_genren_parentid>]]></CategoryParentID>
    <CategoryHentai><![CDATA[<tmpl_var name=data_anime_genren_ishentai>]]></CategoryHentai>
  </categories>
  </tmpl_loop>

  <!-- Table: `tags` -->
  <tmpl_loop name=loop_anime_tag>
  <tags>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <TagID><![CDATA[<tmpl_var name=data_anime_tag_id>]]></TagID>
    <TagName><![CDATA[<tmpl_var name=data_anime_tag_name>]]></TagName>
    <TagDate><![CDATA[<tmpl_var name=data_anime_tag_date>]]></TagDate>
  </tags>
  </tmpl_loop>

  <!-- Table: `companies` -->
  <tmpl_loop name=loop_anime_company>
  <companies>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <CompanyID><![CDATA[<tmpl_var name=data_anime_company_id>]]></CompanyID>
    <CompanyName><![CDATA[<tmpl_var name=data_anime_company_name>]]></CompanyName>
    <CompanyShortName><![CDATA[<tmpl_var name=data_anime_company_shortname>]]></CompanyShortName>
    <CompanyOtherName><![CDATA[<tmpl_var name=data_anime_company_othername>]]></CompanyOtherName>
    <CompanyType><![CDATA[<tmpl_var name=data_anime_company_type>]]></CompanyType>
    <CompanyURL><![CDATA[<tmpl_var name=data_anime_company_url>]]></CompanyURL>
    <CompanyPicURL><![CDATA[<tmpl_var name=data_anime_company_picurl>]]></CompanyPicURL>
    <CompanyApType><![CDATA[<tmpl_var name=data_anime_company_aptype>]]></CompanyApType>
  </companies>
  </tmpl_loop>

  <!-- Table: `episodes` -->
  <tmpl_loop name=loop_ep>
  <episodes>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <EpID><![CDATA[<tmpl_var name=data_ep_id>]]></EpID>
    <EpNo><![CDATA[<tmpl_var name=data_ep_epno>]]></EpNo>
    <EpName><![CDATA[<tmpl_var name=data_ep_name>]]></EpName>
    <EpNameRomaji><![CDATA[<tmpl_var name=data_ep_name_romaji>]]></EpNameRomaji>
    <EpNameKanji><![CDATA[<tmpl_var name=data_ep_name_Kanji>]]></EpNameKanji>
    <EpLength><![CDATA[<tmpl_var name=data_ep_length>]]></EpLength>
    <EpAired><![CDATA[<tmpl_var name=data_ep_aired>]]></EpAired>
    <EpAiredShort><![CDATA[<tmpl_var name=data_ep_aired_short>]]></EpAiredShort>
    <EpDateTime><![CDATA[<tmpl_var name=data_ep_date>]]></EpDateTime>
    <EpDate><![CDATA[<tmpl_var name=data_ep_date_short>]]></EpDate>
    <EpUpdateTime><![CDATA[<tmpl_var name=data_ep_update>]]></EpUpdateTime>
    <EpUpdate><![CDATA[<tmpl_var name=data_ep_update_short>]]></EpUpdate>
    <EpOther><![CDATA[<tmpl_var name=data_ep_other>]]></EpOther>
    <EpState><![CDATA[<tmpl_var name=data_ep_state>]]></EpState>
    <EpStateSpecial><![CDATA[<tmpl_var name=data_ep_state_special>]]></EpStateSpecial>
    <EpStateRecap><![CDATA[<tmpl_var name=data_ep_state_recap>]]></EpStateRecap>
    <EpStateOp><![CDATA[<tmpl_var name=data_ep_state_op>]]></EpStateOp>
    <EpStateEnd><![CDATA[<tmpl_var name=data_ep_state_end>]]></EpStateEnd>
    <EpStateHanger><![CDATA[<tmpl_var name=data_ep_state_hanger>]]></EpStateHanger>
    <EpRating><![CDATA[<tmpl_var name=data_ep_rating>]]></EpRating>
    <EpVotes><![CDATA[<tmpl_var name=data_ep_votes>]]></EpVotes>
    <MyEpWatched><![CDATA[<tmpl_var name=data_ep_state_iswatched>]]></MyEpWatched>
    <MyEpHaveAFile><![CDATA[<tmpl_var name=data_ep_state_hasfile>]]></MyEpHaveAFile>
    <MyEpState><![CDATA[<tmpl_var name=data_ep_mystate>]]></MyEpState>
    <MyEpStateString><![CDATA[<tmpl_var name=data_ep_mystate_string>]]></MyEpStateString>
    <MyEpStateIcon><![CDATA[<tmpl_var name=data_ep_mystate_icon>]]></MyEpStateIcon>
  </episodes>
  <!-- Table: `files` -->
  <tmpl_loop name=loop_file>
  <files>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <EpID><![CDATA[<tmpl_var name=data_ep_id>]]></EpID>
    <FID><![CDATA[<tmpl_var name=data_file_id>]]></FID>
    <Size><![CDATA[<tmpl_var name=data_file_size>]]></Size>
    <SizePlain><![CDATA[<tmpl_var name=data_file_size_plain>]]></SizePlain>
    <SizeHuman><![CDATA[<tmpl_var name=data_file_size_h>]]></SizeHuman>
    <Length><![CDATA[<tmpl_var name=data_file_length>]]></Length>
    <Generic><![CDATA[<tmpl_var name=status_file_isgeneric>]]></Generic>
    <FileType><![CDATA[<tmpl_var name=data_file_filetype>]]></FileType>
    <CRC><![CDATA[<tmpl_var name=data_file_crc>]]></CRC>
    <MD5><![CDATA[<tmpl_var name=data_file_md5>]]></MD5>
    <SHA1><![CDATA[<tmpl_var name=data_file_sha1>]]></SHA1>
    <ed2kName><![CDATA[<tmpl_var name=data_file_ed2k_name>]]></ed2kName>
    <ed2kHash><![CDATA[<tmpl_var name=data_file_ed2k_hash>]]></ed2kHash>
    <ed2kLink><![CDATA[<tmpl_var name=data_file_ed2k_link>]]></ed2kLink>
    <GID><![CDATA[<tmpl_var name=data_file_group_id>]]></GID>
    <GName><![CDATA[<tmpl_var name=data_file_group_name>]]></GName>
    <GShortName><![CDATA[<tmpl_var name=data_file_group_shortname>]]></GShortName>
    <ReleaseDateTime><![CDATA[<tmpl_var name=data_file_released>]]></ReleaseDateTime>
    <ReleaseDate><![CDATA[<tmpl_var name=data_file_released_short>]]></ReleaseDate>
    <QualityID><![CDATA[<tmpl_var name=data_file_qual_id>]]></QualityID>
    <QualityName><![CDATA[<tmpl_var name=data_file_qual_name>]]></QualityName>
    <ResName><![CDATA[<tmpl_var name=data_file_res_name>]]></ResName>
    <Type><![CDATA[<tmpl_var name=data_file_type>]]></Type>
    <VCount><![CDATA[<tmpl_var name=data_file_vidcnt>]]></VCount>
    <VAspectRatio><![CDATA[<tmpl_var name=data_file_ar>]]></VAspectRatio>
    <VFPS><![CDATA[<tmpl_var name=data_file_fps>]]></VFPS>
    <VFlags><![CDATA[<tmpl_var name=data_file_vflags>]]></VFlags>
    <VBitRate><![CDATA[<tmpl_var name=data_file_vbitrate>]]></VBitRate>
    <VCodecID><![CDATA[<tmpl_var name=data_file_vcodec_id>]]></VCodecID>
    <VCodecName><![CDATA[<tmpl_var name=data_file_vcodec_name>]]></VCodecName>
    <ACount><![CDATA[<tmpl_var name=data_file_audcnt>]]></ACount>
    <ABitRate><![CDATA[<tmpl_var name=data_file_abitrate>]]></ABitRate>
    <ACodecID><![CDATA[<tmpl_var name=data_file_acodec_id>]]></ACodecID>
    <ACodecName><![CDATA[<tmpl_var name=data_file_acodec_name>]]></ACodecName>
    <AChanType><![CDATA[<tmpl_var name=data_file_chantype>]]></AChanType>
    <ALangID><![CDATA[<tmpl_var name=data_file_lang_id>]]></ALangID>
    <ALangName><![CDATA[<tmpl_var name=data_file_lang_name>]]></ALangName>
    <ABitRate2><![CDATA[<tmpl_var name=data_file_abitrate2>]]></ABitRate2>
    <ACodecID2><![CDATA[<tmpl_var name=data_file_acodec_id2>]]></ACodecID2>
    <ACodecName2><![CDATA[<tmpl_var name=data_file_acodec_name2>]]></ACodecName2>
    <AChanType2><![CDATA[<tmpl_var name=data_file_chantype2>]]></AChanType2>
    <ALangID2><![CDATA[<tmpl_var name=data_file_lang_id2>]]></ALangID2>
    <ALangName2><![CDATA[<tmpl_var name=data_file_lang_name2>]]></ALangName2>
    <SubCount><![CDATA[<tmpl_var name=data_file_subcnt>]]></SubCount>
    <SubID><![CDATA[<tmpl_var name=data_file_sub_id>]]></SubID>
    <SubName><![CDATA[<tmpl_var name=data_file_sub_name>]]></SubName>
    <SubFlags><![CDATA[<tmpl_var name=data_file_sflags>]]></SubFlags>
    <SubType><![CDATA[<tmpl_var name=data_file_stype>]]></SubType>
    <SubID2><![CDATA[<tmpl_var name=data_file_sub_id2>]]></SubID2>
    <SubName2><![CDATA[<tmpl_var name=data_file_sub_name2>]]></SubName2>
    <SubFlags2><![CDATA[<tmpl_var name=data_file_sflags2>]]></SubFlags2>
    <SubType2><![CDATA[<tmpl_var name=data_file_stype2>]]></SubType2>
    <TypeID><![CDATA[<tmpl_var name=data_file_type_id>]]></TypeID>
    <TypeName><![CDATA[<tmpl_var name=data_file_type_name>]]></TypeName>
    <Other><![CDATA[<tmpl_var name=data_file_other>]]></Other>
    <HasComment><![CDATA[<tmpl_var name=status_file_hascomment>]]></HasComment>
    <DateTime><![CDATA[<tmpl_var name=data_file_date>]]></DateTime>
    <Date><![CDATA[<tmpl_var name=data_file_date_short>]]></Date>
    <UpDateTime><![CDATA[<tmpl_var name=data_file_update>]]></UpDateTime>
    <UpDate><![CDATA[<tmpl_var name=data_file_update_short>]]></UpDate>
    <Storage><![CDATA[<tmpl_var name=data_file_storage>]]></Storage>
    <Source><![CDATA[<tmpl_var name=data_file_source>]]></Source>
    <ViewDateTime><![CDATA[<tmpl_var name=data_viewdate>]]></ViewDateTime>
    <ViewDate><![CDATA[<tmpl_var name=data_file_viewdate_short>]]></ViewDate>
    <State><![CDATA[<tmpl_var name=data_file_state>]]></State>
    <StateCRCOK><![CDATA[<tmpl_var name=status_file_state_crcok>]]></StateCRCOK>
    <StateCRCFailed><![CDATA[<tmpl_var name=status_file_state_crcfailed>]]></StateCRCFailed>
    <StateCRCUnverified><![CDATA[<tmpl_var name=status_file_state_crcunverified>]]></StateCRCUnverified>
    <VersionName><![CDATA[<tmpl_var name=data_file_state_versionname>]]></VersionName>
    <MyWatched><![CDATA[<tmpl_var name=status_file_iswatched>]]></MyWatched>
    <MyState><![CDATA[<tmpl_var name=data_file_mystate>]]></MyState>
    <MyStateString><![CDATA[<tmpl_var name=data_file_mystate_string>]]></MyStateString>
    <MyStateIcon><![CDATA[<tmpl_var name=data_file_mystate_icon>]]></MyStateIcon>
    <MyFileState><![CDATA[<tmpl_var name=data_file_myfilestate>]]></MyFileState>
    <MyFileStateString><![CDATA[<tmpl_var name=data_myfilestate_string>]]></MyFileStateString>
    <MyFileStateIcon><![CDATA[<tmpl_var name=data_myfilestate_icon>]]></MyFileStateIcon>
  </files>
  </tmpl_loop>
  </tmpl_loop>
  </tmpl_loop>
</myanimelist>
