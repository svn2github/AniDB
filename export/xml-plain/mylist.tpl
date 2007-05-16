<?xml version="1.0" encoding="Shift-JIS" ?>
<!-- xml-singlefile-dataonly anidb.net export template
     Version: 1.0 (24.11.2003)
     Shift-JIS encoding was used instead of UTF-8 for compatibility with Japanese data in anidb.net
     There is NO anime.tpl template because ALL export data is sent to this ONE XML file
     The following tables are in the XML document created by this template:
       user_info (I recomend you use "UserName" as the primary key)
       animes (I recommend you use "AnimeID" as the primary key)
       aliases (I recommend you use "Alias" as the primary key; you can use "AnimeID" for joining tables in queries)
       short_titles (I recommend you use "ShortName" as the primary key; you can use "AnimeID" for joining tables in queries)
       episodes (I recommend you use "EpID" as the primary key; you can use "AnimeID" for joining tables in queries)
       files (I recommend you use "FID" as the primary key; you can use "AnimeID" and "EpID" for joining tables in queries)
     I tried to include every available field in the anidb, but if I missed something, msg me (chriv) on anidb.net -->
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
    <MyByteCountRaw><![CDATA[<tmpl_var name=global_byte_count>]]></MyByteCountRaw>
    <MyByteCountHuman><![CDATA[<tmpl_var name=global_bytecount_h>]]></MyByteCountHuman>
    <AniDBURL><![CDATA[<tmpl_var name=animedburl>]]></AniDBURL>
  </user_info>
  <!-- Table: `animes` -->
  <tmpl_loop name=loop_anime>
  <animes>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <Name><![CDATA[<tmpl_var name=data_anime_name>]]></Name>
    <Eps><![CDATA[<tmpl_var name=data_anime_eps>]]></Eps>
    <Year><![CDATA[<tmpl_var name=data_anime_year>]]></Year>
    <Producer><![CDATA[<tmpl_var name=data_anime_producer>]]></Producer>
    <URL><![CDATA[<tmpl_var name=data_anime_url>]]></URL>
    <AnimeDescription><![CDATA[<tmpl_var name=data_anime_other>]]></AnimeDescription>
    <DateTime><![CDATA[<tmpl_var name=data_anime_date>]]></DateTime>
    <Date><![CDATA[<tmpl_var name=data_anime_date_short>]]></Date>
    <UpdateTime><![CDATA[<tmpl_var name=data_anime_update>]]></UpdateTime>
    <Update><![CDATA[<tmpl_var name=data_anime_update_short>]]></Update>
    <Rating><![CDATA[<tmpl_var name=data_anime_rating>]]></Rating>
    <Votes><![CDATA[<tmpl_var name=data_anime_votes>]]></Votes>
    <TypeID><![CDATA[<tmpl_var name=data_anime_type_id>]]></TypeID>
    <TypeName><![CDATA[<tmpl_var name=data_anime_type_name>]]></TypeName>
    <NFOID><![CDATA[<tmpl_var name=data_anime_animenfoid>]]></NFOID>
    <NFOID2><![CDATA[<tmpl_var name=data_anime_animenfoid2>]]></NFOID2>
    <NFOURL><![CDATA[<tmpl_var name=data_anime_animenfourl>]]></NFOURL>
    <NFOURLName><![CDATA[<tmpl_var name=data_anime_animenfourl_name>]]></NFOURLName>
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
    <GenreAction><![CDATA[<tmpl_var name=status_anime_genre_action>]]></GenreAction>
    <GenreAdventure><![CDATA[<tmpl_var name=status_anime_genre_adventure>]]></GenreAdventure>
    <GenreComedy><![CDATA[<tmpl_var name=status_anime_genre_comedy>]]></GenreComedy>
    <GenreDrama><![CDATA[<tmpl_var name=status_anime_genre_drama>]]></GenreDrama>
    <GenreEcchi><![CDATA[<tmpl_var name=status_anime_genre_ecchi>]]></GenreEcchi>
    <GenreHentai><![CDATA[<tmpl_var name=status_anime_genre_hentai>]]></GenreHentai>
    <GenreHorror><![CDATA[<tmpl_var name=status_anime_genre_horror>]]></GenreHorror>
    <GenreRomance><![CDATA[<tmpl_var name=status_anime_genre_romance>]]></GenreRomance>
    <GenreSciFi><![CDATA[<tmpl_var name=status_anime_genre_scifi>]]></GenreSciFi>
    <GenreDemons><![CDATA[<tmpl_var name=status_anime_genre_demons>]]></GenreDemons>
    <GenreFantasy><![CDATA[<tmpl_var name=status_anime_genre_fantasy>]]></GenreFantasy>
    <GenreHistorical><![CDATA[<tmpl_var name=status_anime_genre_historical>]]></GenreHistorical>
    <GenreLoveStory><![CDATA[<tmpl_var name=status_anime_genre_love_story>]]></GenreLoveStory>
    <GenreMagic><![CDATA[<tmpl_var name=status_anime_genre_magic>]]></GenreMagic>
    <GenreMartialArts><![CDATA[<tmpl_var name=status_anime_genre_martial_arts>]]></GenreMartialArts>
    <GenreMecha><![CDATA[<tmpl_var name=status_anime_genre_mecha>]]></GenreMecha>
    <GenreMystery><![CDATA[<tmpl_var name=status_anime_genre_mystery>]]></GenreMystery>
    <GenreParody><![CDATA[<tmpl_var name=status_anime_genre_parody>]]></GenreParody>
    <GenreSamurai><![CDATA[<tmpl_var name=status_anime_genre_samurai>]]></GenreSamurai>
    <GenreSchool><![CDATA[<tmpl_var name=status_anime_genre_school>]]></GenreSchool>
    <GenreSpace><![CDATA[<tmpl_var name=status_anime_genre_space>]]></GenreSpace>
    <GenreSports><![CDATA[<tmpl_var name=status_anime_genre_sports>]]></GenreSports>
    <GenreSuperPower><![CDATA[<tmpl_var name=status_anime_genre_super_power>]]></GenreSuperPower>
    <GenreVampires><![CDATA[<tmpl_var name=status_anime_genre_vampires>]]></GenreVampires>
    <GenreDementia><![CDATA[<tmpl_var name=status_anime_genre_dementia>]]></GenreDementia>
    <GenreCars><![CDATA[<tmpl_var name=status_anime_genre_cars>]]></GenreCars>
    <GenreKids><![CDATA[<tmpl_var name=status_anime_genre_kids>]]></GenreKids>
    <GenreMusic><![CDATA[<tmpl_var name=status_anime_genre_music>]]></GenreMusic>
    <GenreYaoi><![CDATA[<tmpl_var name=status_anime_genre_yaoi>]]></GenreYaoi>
    <GenreShounen><![CDATA[<tmpl_var name=status_anime_genre_shounen>]]></GenreShounen>
    <GenreShoujo><![CDATA[<tmpl_var name=status_anime_genre_shoujo>]]></GenreShoujo>
    <GenreGame><![CDATA[<tmpl_var name=status_anime_genre_game>]]></GenreGame>
    <GenreGhibli><![CDATA[<tmpl_var name=status_anime_genre_ghibli>]]></GenreGhibli>
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
  <!-- Table: `episodes` -->
  <tmpl_loop name=loop_ep>
  <episodes>
    <AnimeID><![CDATA[<tmpl_var name=data_anime_id>]]></AnimeID>
    <EpID><![CDATA[<tmpl_var name=data_ep_id>]]></EpID>
    <EpNo><![CDATA[<tmpl_var name=data_ep_epno>]]></EpNo>
    <EpName><![CDATA[<tmpl_var name=data_ep_name>]]></EpName>
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
    <MyEpWatched><![CDATA[<tmpl_var name=data_ep_state_iswatched>]]></MyEpWatched>
    <MyEpHaveAFile><![CDATA[<tmpl_var name=data_ep_state_hasfile>]]></MyEpHaveAFile>
    <MyEpState><![CDATA[<tmpl_var name=data_ep_mystate>]]></MyEpState>
    <MyEpStateString><![CDATA[<tmpl_var name=data_ep_mystate_string>]]></MyEpStateString>
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
    <Type><![CDATA[<tmpl_var name=data_filetype>]]></Type>
    <CRC><![CDATA[<tmpl_var name=data_file_crc>]]></CRC>
    <MD5><![CDATA[<tmpl_var name=data_file_md5>]]></MD5>
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
    <ResID><![CDATA[<tmpl_var name=data_file_res_id>]]></ResID>
    <ResName><![CDATA[<tmpl_var name=data_file_res_name>]]></ResName>
    <ABitRate><![CDATA[<tmpl_var name=data_file_abitrate>]]></ABitRate>
    <ACodecID><![CDATA[<tmpl_var name=data_file_acodec_id>]]></ACodecID>
    <ACodecName><![CDATA[<tmpl_var name=data_file_acodec_name>]]></ACodecName>
    <VBitRate><![CDATA[<tmpl_var name=data_file_vbitrate>]]></VBitRate>
    <VCodecID><![CDATA[<tmpl_var name=data_file_vcodec_id>]]></VCodecID>
    <VCodecName><![CDATA[<tmpl_var name=data_file_vcodec_name>]]></VCodecName>
    <LangID><![CDATA[<tmpl_var name=data_file_lang_id>]]></LangID>
    <LangName><![CDATA[<tmpl_var name=data_file_lang_name>]]></LangName>
    <SubID><![CDATA[<tmpl_var name=data_file_sub_id>]]></SubID>
    <SubName><![CDATA[<tmpl_var name=data_file_sub_name>]]></SubName>
    <TypeID><![CDATA[<tmpl_var name=data_file_type_id>]]></TypeID>
    <TypeName><![CDATA[<tmpl_var name=data_file_type_name>]]></TypeName>
    <Other><![CDATA[<tmpl_var name=data_file_other>]]></Other>
    <DateTime><![CDATA[<tmpl_var name=data_file_date>]]></DateTime>
    <Date><![CDATA[<tmpl_var name=data_file_date_short>]]></Date>
    <UpDateTime><![CDATA[<tmpl_var name=data_file_update>]]></UpDateTime>
    <UpDate><![CDATA[<tmpl_var name=data_file_update_short>]]></UpDate>
    <Storage><![CDATA[<tmpl_var name=data_file_storage>]]></Storage>
    <Source><![CDATA[<tmpl_var name=data_file_source>]]></Source>
    <ViewDateTime><![CDATA[<tmpl_var name=data_viewdate>]]></ViewDateTime>
    <ViewDate><![CDATA[<tmpl_var name=data_file_viewdate_short>]]></ViewDate>
    <State><![CDATA[<tmpl_var name=data_file_state>]]></State>
    <StateCRCOK><![CDATA[<tmpl_var name=data_file_state_crcok>]]></StateCRCOK>
    <StateCRCFailed><![CDATA[<tmpl_var name=data_file_state_crcfailed>]]></StateCRCFailed>
    <StateISV2><![CDATA[<tmpl_var name=data_file_state_isv2>]]></StateISV2>
    <StateISV3><![CDATA[<tmpl_var name=data_file_state_isv3>]]></StateISV3>
    <StateISV4><![CDATA[<tmpl_var name=data_file_state_isv4>]]></StateISV4>
    <StateISV5><![CDATA[<tmpl_var name=data_file_state_isv5>]]></StateISV5>
    <MyWatched><![CDATA[<tmpl_var name=data_file_iswatched>]]></MyWatched>
    <MyState><![CDATA[<tmpl_var name=data_file_mystate>]]></MyState>
    <MyStateString><![CDATA[<tmpl_var name=data_file_mystate_string>]]></MyStateString>
    <MyStateIcon><![CDATA[<tmpl_var name=data_file_mystate_icon>]]></MyStateIcon>
    <MyStateUnknown><![CDATA[<tmpl_var name=data_file_mystate_unknown>]]></MyStateUnknown>
    <MyStateOnHDD><![CDATA[<tmpl_var name=data_file_mystate_onhdd>]]></MyStateOnHDD>
    <MyStateOnCD><![CDATA[<tmpl_var name=data_file_mystate_oncd>]]></MyStateOnCD>
    <MyStateDeleted><![CDATA[<tmpl_var name=data_file_mystate_deleted>]]></MyStateDeleted>
    <MyStateShared><![CDATA[<tmpl_var name=data_file_mystate_shared>]]></MyStateShared>
    <MyStateRelease><![CDATA[<tmpl_var name=data_file_mystate_release>]]></MyStateRelease>
  </files>
  </tmpl_loop>
  </tmpl_loop>
  </tmpl_loop>
</myanimelist>
