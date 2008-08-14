<?xml version="1.0" encoding="UTF-8" ?>
<!-- xml-singlefile-dataonly anidb.net export template
     Version: 3.0 (12.08.2008) by bsdude
     Based on Version 1.0 by chriv
     The following tables are in the XML document created by this template:
       user_info
       category (use "CategoryID" as primary key)
       anime (use "AnimeID" as the primary key)
       anime_group (user "AnimeID" and "GID" as primary key; use "AnimeID" for joining tables in queries)
       title (use "AnimeID" for joining tables in queries)
       award_type (use "AnimeID" and "AwardTypeID" as primary key)
       award (use "AnimeID" and "AwardID" as primary key)
       anime_category (use "AnimeID" and "CategoryID" as primary key and for joining tables in queries)
       tag (use "TagID" as primary key; use "AnimeID" for joining tables in queries)
       company (use "AnimeID" and "CompanyID" as primary key; use "AnimeID" for joining in queries)
       episode (use "EpID" as the primary key; use "AnimeID" for joining tables in queries)
       episode_title (use "AnimeID" and "EpID" for joining tables in queries)
       file (use "FID" as the primary key; use "AnimeID" and "EpID" for joining tables in queries)
       file_file
       file_episode
-->
<!-- Database : `my_anime_list` -->
<my_anime_list>
  <!-- Table: `user_info` -->
  <user_info>
    <UserName><![CDATA[<tmpl_var name=global_user>]]></UserName>
    <UserID><![CDATA[<tmpl_var name=global_user_id>]]></UserID>
    <ExportDate><![CDATA[<tmpl_var name=global_date>]]></ExportDate>
    <MyAnimeCount><![CDATA[<tmpl_var name=global_animecount>]]></MyAnimeCount>
    <MyEpisodeCount><![CDATA[<tmpl_var name=global_epcount>]]></MyEpisodeCount>
    <MyFileCount><![CDATA[<tmpl_var name=global_filecount>]]></MyFileCount>
    <MyByteCount><![CDATA[<tmpl_var name=global_bytecount>]]></MyByteCount>
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

  <!-- Table: `category` -->
  <tmpl_loop name=global_genren_loop>
  <category>
    <CategoryID><![CDATA[<tmpl_var name=global_genren_id>]]></CategoryID>
    <CategoryName><![CDATA[<tmpl_var name=global_genren_name>]]></CategoryName>
    <CategoryParentID><![CDATA[<tmpl_var name=global_genren_parentid>]]></CategoryParentID>
    <CategoryHentai><![CDATA[<tmpl_var name=global_genren_ishentai>]]></CategoryHentai>
  </category>
  </tmpl_loop>

  <!-- Table: `anime` -->
  <tmpl_loop name=loop_anime>
  <anime>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <Name><![CDATA[<tmpl_var name=data_anime_name>]]></Name>
    <NameKanji><![CDATA[<tmpl_var name=data_anime_title_jap_kanji>]]></NameKanji>
    <NameEnglish><![CDATA[<tmpl_var name=data_anime_title_eng>]]></NameEnglish>
    <Year><![CDATA[<tmpl_var name=data_anime_year>]]></Year>
    <URL><![CDATA[<tmpl_var name=data_anime_url>]]></URL>
    <AnimeDescription><![CDATA[<tmpl_var name=data_anime_other>]]></AnimeDescription>
    <Date><![CDATA[<tmpl_var name=data_anime_date>]]></Date>
    <Update><![CDATA[<tmpl_var name=data_anime_update>]]></Update>
    <StartDate><![CDATA[<tmpl_var name=data_anime_startdate>]]></StartDate>
    <EndDate><![CDATA[<tmpl_var name=data_anime_enddate>]]></EndDate>
    <Rating><![CDATA[<tmpl_var name=data_anime_rating>]]></Rating>
    <Votes><![CDATA[<tmpl_var name=data_anime_votes>]]></Votes>
    <TempRating><![CDATA[<tmpl_var name=data_anime_tmprating>]]></TempRating>
    <TempVotes><![CDATA[<tmpl_var name=data_anime_tmpvotes>]]></TempVotes> 
    <ReviewRating><![CDATA[<tmpl_var name=data_anime_reviewrating>]]></ReviewRating> 
    <Reviews><![CDATA[<tmpl_var name=data_anime_reviews>]]></Reviews>
    <MyVote><![CDATA[<tmpl_var name=data_anime_my_vote>]]></MyVote>
    <MyVoteDate><![CDATA[<tmpl_var name=data_anime_my_vote_date>]]></MyVoteDate>
    <MyTempVote><![CDATA[<tmpl_var name=data_anime_my_tmpvote>]]></MyTempVote>
    <MyTempVoteDate><![CDATA[<tmpl_var name=data_anime_my_tmpvote_date>]]></MyTempVoteDate>
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
    <Eps><![CDATA[<tmpl_var name=data_anime_eps>]]></Eps>
    <EpsSpecial><![CDATA[<tmpl_var name=data_anime_eps_special>]]></EpsSpecial>
    <EpsTotal><![CDATA[<tmpl_var name=data_anime_eps_total>]]></EpsTotal>
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
    <MyWishlistType><![CDATA[<tmpl_var name=data_anime_wishlist_type>]]></MyWishlistType>
    <MyWishlistTypeName><![CDATA[<tmpl_var name=data_anime_wishlist_type_name>]]></MyWishlistTypeName>
    <MyWishlistPriority><![CDATA[<tmpl_var name=data_anime_wishlist_priority>]]></MyWishlistPriority>
    <MyWishlistPriorityName><![CDATA[<tmpl_var name=data_anime_wishlist_priority_name>]]></MyWishlistPriorityName>
    <MyWishlistComment><![CDATA[<tmpl_var name=data_anime_wishlist_comment>]]></MyWishlistComment>
    <Hentai><![CDATA[<tmpl_var name=status_anime_isrestricted>]]></Hentai>
  </anime>

  <!-- Table: `title` -->
  <tmpl_loop name=loop_anime_titles>
  <title>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <TitleTypeID><![CDATA[<tmpl_var name=data_anime_title_type_id>]]></TitleTypeID>
    <TitleTypeName><![CDATA[<tmpl_var name=data_anime_title_type_string>]]></TitleTypeName>
    <Name><![CDATA[<tmpl_var name=data_anime_title_name>]]></Name>
    <LanguageID><![CDATA[<tmpl_var name=data_anime_title_langid>]]></LanguageID>
    <LanguageName><![CDATA[<tmpl_var name=data_anime_title_langname>]]></LanguageName>
  </title>
  </tmpl_loop>

  <!-- Table: `award_type` -->
  <tmpl_loop name=loop_anime_award_types>
  <award_type>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <AwardTypeID><![CDATA[<tmpl_var name=data_anime_award_type_id>]]></AwardTypeID>
    <AwardTypeName><![CDATA[<tmpl_var name=data_anime_award_type_name>]]></AwardTypeName>
    <AwardTypeImage><![CDATA[<tmpl_var name=data_anime_award_type_img>]]></AwardTypeImage>
  </award_type>
  </tmpl_loop>

  <!-- Table: `award` -->
  <tmpl_loop name=loop_anime_awards>
  <award>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <AwardID><![CDATA[<tmpl_var name=data_anime_award_id>]]></AwardID>
    <AwardName><![CDATA[<tmpl_var name=data_anime_award_name>]]></AwardName>
    <AwardType><![CDATA[<tmpl_var name=data_anime_award_type>]]></AwardType>
    <AwardURL><![CDATA[<tmpl_var name=data_anime_award_url>]]></AwardURL>
  </award>
  </tmpl_loop>

  <!-- Table: `anime_category` -->
  <tmpl_loop name=loop_anime_genren>
  <anime_category>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <CategoryID><![CDATA[<tmpl_var name=data_anime_genren_id>]]></CategoryID>
    <CategoryName><![CDATA[<tmpl_var name=data_anime_genren_name>]]></CategoryName>
    <CategoryParentID><![CDATA[<tmpl_var name=data_anime_genren_parentid>]]></CategoryParentID>
    <CategoryHentai><![CDATA[<tmpl_var name=data_anime_genren_ishentai>]]></CategoryHentai>
    <CategoryWeight><![CDATA[<tmpl_var name=data_anime_genren_weight>]]></CategoryWeight>
  </anime_category>
  </tmpl_loop>

  <!-- Table: `tag` -->
  <tmpl_loop name=loop_anime_tag>
  <tag>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <TagID><![CDATA[<tmpl_var name=data_anime_tag_id>]]></TagID>
    <TagName><![CDATA[<tmpl_var name=data_anime_tag_name>]]></TagName>
    <TagDate><![CDATA[<tmpl_var name=data_anime_tag_date>]]></TagDate>
  </tag>
  </tmpl_loop>

  <!-- Table: `company` -->
  <tmpl_loop name=loop_anime_company>
  <company>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <CompanyID><![CDATA[<tmpl_var name=data_anime_company_id>]]></CompanyID>
    <CompanyName><![CDATA[<tmpl_var name=data_anime_company_name>]]></CompanyName>
    <CompanyShortName><![CDATA[<tmpl_var name=data_anime_company_shortname>]]></CompanyShortName>
    <CompanyOtherName><![CDATA[<tmpl_var name=data_anime_company_othername>]]></CompanyOtherName>
    <CompanyTypeID><![CDATA[<tmpl_var name=data_anime_company_type>]]></CompanyTypeID>
    <CompanyTypeName><![CDATA[<tmpl_var name=data_anime_company_type_name>]]></CompanyTypeName>
    <CompanyURL><![CDATA[<tmpl_var name=data_anime_company_url>]]></CompanyURL>
    <CompanyPicURL><![CDATA[<tmpl_var name=data_anime_company_picurl>]]></CompanyPicURL>
    <CompanyApTypeID><![CDATA[<tmpl_var name=data_anime_company_aptype>]]></CompanyApTypeID>
    <CompanyApTypeName><![CDATA[<tmpl_var name=data_anime_company_aptype_name>]]></CompanyApTypeName>
  </company>
  </tmpl_loop>

  <!-- Table: `anime_group` -->
  <tmpl_loop name=loop_anime_groups>
  <anime_group>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <GID><![CDATA[<tmpl_var name=data_anime_group_id>]]></GID>
    <AGID><![CDATA[<tmpl_var name=data_anime_group_agid>]]></AGID>
    <GName><![CDATA[<tmpl_var name=data_anime_group_name>]]></GName>
    <GShortName><![CDATA[<tmpl_var name=data_anime_group_sname>]]></GShortName>
    <State><![CDATA[<tmpl_var name=data_anime_group_state>]]></State>
    <EpRange><![CDATA[<tmpl_var name=data_anime_group_eprange>]]></EpRange>
    <LastEp><![CDATA[<tmpl_var name=data_anime_group_lastep>]]></LastEp>
    <EpCount><![CDATA[<tmpl_var name=data_anime_group_epcnt>]]></EpCount>
    <SpecialCount><![CDATA[<tmpl_var name=data_anime_group_sepcnt>]]></SpecialCount>
    <Update><![CDATA[<tmpl_var name=data_anime_group_lastup>]]></Update>
    <Rating><![CDATA[<tmpl_var name=data_anime_group_rating>]]></Rating>
    <Votes><![CDATA[<tmpl_var name=data_anime_group_votes>]]></Votes>
    <MyVote><![CDATA[<tmpl_var name=data_anime_group_myvote>]]></MyVote>
    <MyVoteDate><![CDATA[<tmpl_var name=data_anime_group_myvote_date>]]></MyVoteDate>
    <Comments><![CDATA[<tmpl_var name=data_anime_group_cmts>]]></Comments>
  </anime_group>
  </tmpl_loop>

  <!-- Table: `episode` -->
  <tmpl_loop name=loop_ep>
  <episode>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <EpID><![CDATA[<tmpl_var name=data_ep_id>]]></EpID>
    <EpNo><![CDATA[<tmpl_var name=data_ep_epno>]]></EpNo>
    <EpName><![CDATA[<tmpl_var name=data_ep_name>]]></EpName>
    <EpNameRomaji><![CDATA[<tmpl_var name=data_ep_name_romaji>]]></EpNameRomaji>
    <EpNameKanji><![CDATA[<tmpl_var name=data_ep_name_Kanji>]]></EpNameKanji>
    <EpLength><![CDATA[<tmpl_var name=data_ep_length>]]></EpLength>
    <EpAired><![CDATA[<tmpl_var name=data_ep_aired>]]></EpAired>
    <EpDate><![CDATA[<tmpl_var name=data_ep_date>]]></EpDate>
    <EpUpdate><![CDATA[<tmpl_var name=data_ep_update>]]></EpUpdate>
    <EpOther><![CDATA[<tmpl_var name=data_ep_other>]]></EpOther>
    <EpState><![CDATA[<tmpl_var name=data_ep_state>]]></EpState>
    <EpStateSpecial><![CDATA[<tmpl_var name=status_ep_state_special>]]></EpStateSpecial>
    <EpStateRecap><![CDATA[<tmpl_var name=status_ep_state_recap>]]></EpStateRecap>
    <EpStateOp><![CDATA[<tmpl_var name=status_ep_state_op>]]></EpStateOp>
    <EpStateEnd><![CDATA[<tmpl_var name=status_ep_state_end>]]></EpStateEnd>
    <EpRating><![CDATA[<tmpl_var name=data_ep_rating>]]></EpRating>
    <EpVotes><![CDATA[<tmpl_var name=data_ep_votes>]]></EpVotes>
    <EpMyVote><![CDATA[<tmpl_var name=data_ep_myvote>]]></EpMyVote>
    <EpMyVoteDate><![CDATA[<tmpl_var name=data_ep_myvote_date>]]></EpMyVoteDate>
    <MyEpWatched><![CDATA[<tmpl_var name=data_ep_state_iswatched>]]></MyEpWatched>
    <MyEpState><![CDATA[<tmpl_var name=data_ep_mystate>]]></MyEpState>
    <MyEpStateString><![CDATA[<tmpl_var name=data_ep_mystate_string>]]></MyEpStateString>
    <MyEpStateIcon><![CDATA[<tmpl_var name=data_ep_mystate_icon>]]></MyEpStateIcon>
  </episode>

  <!-- Table: `episode_title` -->
  <tmpl_loop name=data_ep_titles>
  <episode_title>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <EpID><![CDATA[<tmpl_var name=data_ep_id>]]></EpID>
    <Name><![CDATA[<tmpl_var name=data_ep_title_name>]]></Name>
    <LanguageID><![CDATA[<tmpl_var name=data_ep_title_langid>]]></LanguageID>
    <LanguageName><![CDATA[<tmpl_var name=data_ep_title_langname>]]></LanguageName>
  </episode_title>
  </tmpl_loop>

  <!-- Table: `file` -->
  <tmpl_loop name=loop_file>
  <file>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <EpID><![CDATA[<tmpl_var name=data_ep_id>]]></EpID>
    <FID><![CDATA[<tmpl_var name=data_file_id>]]></FID>
    <Size><![CDATA[<tmpl_var name=data_file_size_plain>]]></Size>
    <Length><![CDATA[<tmpl_var name=data_file_length>]]></Length>
    <Generic><![CDATA[<tmpl_var name=status_file_isgeneric>]]></Generic>
    <FileType><![CDATA[<tmpl_var name=data_file_filetype>]]></FileType>
    <CRC><![CDATA[<tmpl_var name=data_file_crc>]]></CRC>
    <MD5><![CDATA[<tmpl_var name=data_file_md5>]]></MD5>
    <SHA1><![CDATA[<tmpl_var name=data_file_sha1>]]></SHA1>
    <TTH><![CDATA[<tmpl_var name=data_file_sha1>]]></TTH>
    <ed2kName><![CDATA[<tmpl_var name=data_file_ed2k_name>]]></ed2kName>
    <ed2kHash><![CDATA[<tmpl_var name=data_file_ed2k_hash>]]></ed2kHash>
    <ed2kLink><![CDATA[<tmpl_var name=data_file_ed2k_link>]]></ed2kLink>
    <GID><![CDATA[<tmpl_var name=data_file_group_id>]]></GID>
    <GName><![CDATA[<tmpl_var name=data_file_group_name>]]></GName>
    <GShortName><![CDATA[<tmpl_var name=data_file_group_shortname>]]></GShortName>
    <ReleaseDate><![CDATA[<tmpl_var name=data_file_released>]]></ReleaseDate>
    <QualityID><![CDATA[<tmpl_var name=data_file_qual_id>]]></QualityID>
    <QualityName><![CDATA[<tmpl_var name=data_file_qual_name>]]></QualityName>
    <ResName><![CDATA[<tmpl_var name=data_file_res_name>]]></ResName>
    <FileType><![CDATA[<tmpl_var name=data_file_type>]]></FileType>
    <VCount><![CDATA[<tmpl_var name=data_file_vidcnt>]]></VCount>
    <VAspectRatio><![CDATA[<tmpl_var name=data_file_ar>]]></VAspectRatio>
    <VAspectRatioName><![CDATA[<tmpl_var name=data_file_ar_name>]]></VAspectRatioName>
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
    <AChanTypeName><![CDATA[<tmpl_var name=data_file_chantype_name>]]></AChanTypeName>
    <ALangID><![CDATA[<tmpl_var name=data_file_lang_id>]]></ALangID>
    <ALangName><![CDATA[<tmpl_var name=data_file_lang_name>]]></ALangName>
    <AType><![CDATA[<tmpl_var name=data_file_atype>]]></AType>
    <ATypeName><![CDATA[<tmpl_var name=data_file_atype_name>]]></ATypeName>
    <ABitRate2><![CDATA[<tmpl_var name=data_file_abitrate2>]]></ABitRate2>
    <ACodecID2><![CDATA[<tmpl_var name=data_file_acodec_id2>]]></ACodecID2>
    <ACodecName2><![CDATA[<tmpl_var name=data_file_acodec_name2>]]></ACodecName2>
    <AChanType2><![CDATA[<tmpl_var name=data_file_chantype2>]]></AChanType2>
    <AChanTypeName2><![CDATA[<tmpl_var name=data_file_chantype_name2>]]></AChanTypeName2>
    <ALangID2><![CDATA[<tmpl_var name=data_file_lang_id2>]]></ALangID2>
    <ALangName2><![CDATA[<tmpl_var name=data_file_lang_name2>]]></ALangName2>
    <AType2><![CDATA[<tmpl_var name=data_file_atype2>]]></AType2>
    <ATypeName2><![CDATA[<tmpl_var name=data_file_atype_name2>]]></ATypeName2>
    <SubCount><![CDATA[<tmpl_var name=data_file_subcnt>]]></SubCount>
    <SubID><![CDATA[<tmpl_var name=data_file_sub_id>]]></SubID>
    <SubName><![CDATA[<tmpl_var name=data_file_sub_name>]]></SubName>
    <SubFlags><![CDATA[<tmpl_var name=data_file_sflags>]]></SubFlags>
    <SubType><![CDATA[<tmpl_var name=data_file_stype>]]></SubType>
    <SubTypeName><![CDATA[<tmpl_var name=data_file_stypename>]]></SubTypeName>
    <SubID2><![CDATA[<tmpl_var name=data_file_sub_id2>]]></SubID2>
    <SubName2><![CDATA[<tmpl_var name=data_file_sub_name2>]]></SubName2>
    <SubFlags2><![CDATA[<tmpl_var name=data_file_sflags2>]]></SubFlags2>
    <SubType2><![CDATA[<tmpl_var name=data_file_stype2>]]></SubType2>
    <SubTypeName2><![CDATA[<tmpl_var name=data_file_stypename2>]]></SubTypeName2>
    <TypeID><![CDATA[<tmpl_var name=data_file_type_id>]]></TypeID>
    <TypeName><![CDATA[<tmpl_var name=data_file_type_name>]]></TypeName>
    <Other><![CDATA[<tmpl_var name=data_file_other>]]></Other>
    <HasComment><![CDATA[<tmpl_var name=status_file_hascomment>]]></HasComment>
    <Date><![CDATA[<tmpl_var name=data_file_date>]]></Date>
    <Update><![CDATA[<tmpl_var name=data_file_update>]]></Update>
    <Storage><![CDATA[<tmpl_var name=data_file_storage>]]></Storage>
    <Source><![CDATA[<tmpl_var name=data_file_source>]]></Source>
    <ListDate><![CDATA[<tmpl_var name=data_file_ldate>]]></ListDate>
    <ViewDate><![CDATA[<tmpl_var name=data_file_viewdate>]]></ViewDate>
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
  </file>

  <!-- Table: `file_file` -->
  <tmpl_loop name=loop_data_file_filerel>
  <filefile>
    <FID1><![CDATA[<tmpl_var name=data_file_filerel_fid>]]></FID1>
    <FID2><![CDATA[<tmpl_var name=data_file_filerel_otherfid>]]></FID2>
    <RelationType><![CDATA[<tmpl_var name=data_file_filerel_type>]]></RelationType>
  </filefile>
  </tmpl_loop>

  <!-- Table: `file_episode` -->
  <tmpl_loop name=loop_data_file_fileeprel>
  <file_episode>
    <FID><![CDATA[<tmpl_var name=data_file_fileeprel_fid>]]></FID>
    <EpID><![CDATA[<tmpl_var name=data_file_fileeprel_eid>]]></EpID>
    <StartPercent><![CDATA[<tmpl_var name=data_file_fileeprel_startp>]]></StartPercent>
    <EndPercent><![CDATA[<tmpl_var name=data_file_fileeprel_endp>]]></EndPercent>
  </file_episode>
  </tmpl_loop>

  </tmpl_loop>
  </tmpl_loop>
  </tmpl_loop>
</my_anime_list>
