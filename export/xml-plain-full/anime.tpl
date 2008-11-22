<?xml version="1.0" encoding="UTF-8"?>
<!-- 
  AniDB mylist anime template 
  Version 1.0.0.$Rev: 12 $ 
  (C) 2008 Finalspace
-->
<tmpl_loop name=loop_anime>
<anime>
  <id><tmpl_var name=data_anime_id></id>
  <count eps="<tmpl_var name=data_anime_eps>" specials="<tmpl_var name=data_anime_eps_special>" total="<tmpl_var name=data_anime_eps_total>" />
  <type id="<tmpl_var name=data_anime_type_id>"><![CDATA[<tmpl_var name=data_anime_type_name>]]></type>

  <defaulttitle>
    <langshort><![CDATA[<tmpl_var name=data_anime_name_langsname>]]></langshort>
    <langlong><![CDATA[<tmpl_var name=data_anime_name_langname>]]></langlong>
    <name><![CDATA[<tmpl_var name=data_anime_name>]]></name>
  </defaulttitle>
  
  <tmpl_if name=status_anime_title_has_jap_kanji><japtitle><![CDATA[<tmpl_var name=data_anime_title_jap_kanji>]]></japtitle></tmpl_if>
  <tmpl_if name=status_anime_title_has_eng><engtitle><![CDATA[<tmpl_var name=data_anime_title_eng>]]></engtitle></tmpl_if>
  <tmpl_if name=status_anime_title_has_other><othertitle><![CDATA[<tmpl_var name=data_anime_title_other>]]></othertitle></tmpl_if>
    
  <year><tmpl_var name=data_anime_year></year>
  <url><![CDATA[<tmpl_var name=data_anime_url>]]></url>
  <other><![CDATA[<tmpl_var name=data_anime_other>]]></other>
  
  <date>
    <short><![CDATA[<tmpl_var name=data_anime_date_short>]]></short>
    <long><![CDATA[<tmpl_var name=data_anime_date_long>]]></long>
  </date>
  <update>
    <unix><![CDATA[<tmpl_var name=data_anime_update>]]></unix>
    <short><![CDATA[<tmpl_var name=data_anime_update_short>]]></short>
  </update>
  <startdate>
    <short><![CDATA[<tmpl_var name=data_anime_startdate_short>]]></short>
    <long><![CDATA[<tmpl_var name=data_anime_startdate>]]></long>
  </startdate>
  <enddate>
    <short><![CDATA[<tmpl_var name=data_anime_enddate_short>]]></short>
    <long><![CDATA[<tmpl_var name=data_anime_enddate>]]></long>
  </enddate>
  
  <rating><tmpl_var name=data_anime_rating></rating>
  <votes><tmpl_var name=data_anime_votes></votes>
  <tmprating><tmpl_var name=data_anime_tmprating></tmprating>
  <tmpvotes><tmpl_var name=data_anime_tmpvotes></tmpvotes>
  <reviewrating><tmpl_var name=data_anime_reviewrating></reviewrating>
  <reviews><tmpl_var name=data_anime_reviews></reviews>
  
  <web>
    <animenfo>
      <id><tmpl_var name=data_anime_animenfoid></id>
      <id2><![CDATA[<tmpl_var name=data_anime_animenfoid2>]]></id2>
      <url><![CDATA[<tmpl_var name=data_anime_animenfourl>]]></url>
      <urlname><![CDATA[<tmpl_var name=data_anime_animenfourl_name>]]></urlname>
    </animenfo>
    <ann>
      <id><tmpl_var name=data_anime_annid></id>
      <listed><tmpl_var name=status_anime_ann_listed></listed>
      <url><![CDATA[<tmpl_var name=data_anime_annurl>]]></url>
    </ann>
    <allcinema>
      <id><tmpl_var name=data_anime_allcinemaid></id>
      <listed><tmpl_var name=status_anime_allcinema_listed></listed>
      <url><![CDATA[<tmpl_var name=data_anime_allcinemaurl>]]></url>
    </allcinema>
    <animeplanet>
      <id><tmpl_var name=data_anime_animeplanetid></id>
      <listed><tmpl_var name=status_anime_animeplanet_listed></listed>
      <url><![CDATA[<tmpl_var name=data_anime_animeplaneturl>]]></url>
    </animeplanet>
  </web>

  <mylist_entry>
    <count eps="<tmpl_var name=data_anime_my_eps>" specials="<tmpl_var name=data_anime_my_eps_special>" total="<tmpl_var name=data_anime_my_eps_total>" />
    <watched eps="<tmpl_var name=data_anime_my_watchedeps>" specials="<tmpl_var name=data_anime_my_watchedeps_special>" total="<tmpl_var name=data_anime_my_watchedeps_total>" />
    <unwatched eps="<tmpl_var name=data_anime_my_unwatchedeps>" specials="<tmpl_var name=data_anime_my_unwatchedeps_special>" total="<tmpl_var name=data_anime_my_unwatchedeps_total>" />
    <size normal="<tmpl_var name=data_anime_my_size>" formated="<tmpl_var name=data_anime_my_size_h>" />
    <state id="<tmpl_var name=data_anime_mystate>">
      <str><![CDATA[<tmpl_var name=data_anime_mystate_string>]]></str>
      <icon><![CDATA[<tmpl_var name=data_anime_mystate_icon>]]></icon>
      <isundef><tmpl_var name=status_anime_mystate_undef></isundef>
      <isunknown><tmpl_var name=status_anime_mystate_unknown></isunknown>
      <isonhdd><tmpl_var name=status_anime_mystate_onhdd></isonhdd>
      <isoncd><tmpl_var name=status_anime_mystate_oncd></isoncd>
      <isdeleted><tmpl_var name=status_anime_mystate_deleted></isdeleted>
    </state>
  </mylist_entry>
  
  <status>
    <iscomplete><tmpl_var name=status_anime_iscomplete></iscomplete>
    <iswatched><tmpl_var name=status_anime_iswatched></iswatched>
    <hasawards><tmpl_var name=status_anime_hasawards></hasawards>
  </status>
  
  <tmpl_if name=status_anime_hasawards><awardicons><![CDATA[<tmpl_var name=data_anime_awardicons>]]></awardicons></tmpl_if>
  
  <tmpl_if name=status_anime_isinwishlist><onwishlist>
    <type id="<tmpl_var name=data_anime_wishlist_type>"><![CDATA[<tmpl_var name=data_anime_wishlist_type_name>]]></type>
    <priority id="<tmpl_var name=data_anime_wishlist_priority>"><![CDATA[<tmpl_var name=data_anime_wishlist_priority_name>]]></priority>
    <comment><![CDATA[<tmpl_var name=data_anime_wishlist_comment>]]></comment>
  </onwishlist></tmpl_if>
  
  <voted>
    <tmpl_if name=status_anime_my_isvoted><myvote>
      <value><tmpl_var name=data_anime_my_vote></value>
      <date short="<tmpl_var name=data_anime_my_vote_date_short>"><tmpl_var name=data_anime_my_vote_date></date>
    </myvote></tmpl_if>

    <tmpl_if name=status_anime_my_istmpvoted><mytmpvote>
      <value><tmpl_var name=data_anime_my_tmpvote></value>
      <date short="<tmpl_var name=data_anime_my_tmpvote_date_short>"><tmpl_var name=data_anime_my_tmpvote_date></date>
    </mytmpvote></tmpl_if>
  </voted>
       
  <genres>
    <tmpl_loop name=loop_anime_genren>
    <genre id="<tmpl_var name=data_anime_genren_id>" parentid="<tmpl_var name=data_anime_genren_parentid>" ishentai="<tmpl_var name=data_anime_genren_ishentai>">
      <![CDATA[<tmpl_var name=data_anime_genren_name>]]>
    </genre></tmpl_loop>
  </genres>
  
  <tags>
    <tmpl_loop name=loop_anime_tag>
    <tag id="<tmpl_var name=data_anime_tag_id>">
      <name><![CDATA[<tmpl_var name=data_anime_tag_name>]]></name>
      <date><![CDATA[<tmpl_var name=data_anime_tag_date>]]></date>
    </tag></tmpl_loop>
  </tags>

  <companies>
    <tmpl_loop name=loop_anime_company>
    <company id="<tmpl_var name=data_anime_company_id>" type="<tmpl_var name=data_anime_company_type>" role="<tmpl_var name=data_anime_company_aptype>">
      <name><![CDATA[<tmpl_var name=data_anime_company_name>]]></name>
      <shortname><![CDATA[<tmpl_var name=data_anime_company_shortname>]]></shortname>
      <othername><![CDATA[<tmpl_var name=data_anime_company_othername>]]></othername>
      <url><![CDATA[<tmpl_var name=data_anime_company_url>]]></url>
      <picurl><![CDATA[<tmpl_var name=data_anime_company_picurl>]]></picurl>
    </company></tmpl_loop>
  </companies>

  <groups>
    <tmpl_loop name=loop_anime_groups>
    <group id="<tmpl_var name=data_anime_group_id>" agid="<tmpl_var name=data_anime_group_agid>">
      <name><![CDATA[<tmpl_var name=data_anime_group_name>]]></name>
      <shortname><![CDATA[<tmpl_var name=data_anime_group_sname>]]></shortname>
      <state><![CDATA[<tmpl_var name=data_anime_group_state>]]></state>
      <lastep><tmpl_var name=data_anime_group_lastep></lastep>
      <lastup><tmpl_var name=data_anime_group_lastup></lastup>
      <votes><tmpl_var name=data_anime_group_votes></votes>
      <rating><tmpl_var name=data_anime_group_rating></rating>
      <cmts><tmpl_var name=data_anime_group_cmts></cmts>
      <epcount><tmpl_var name=data_anime_group_epcnt></epcount>
      <specialcount><tmpl_var name=data_anime_group_sepcnt></specialcount>
      <myvote date="<tmpl_var name=data_anime_group_myvote_date>"><tmpl_var name=data_anime_group_myvote></myvote>
    </group></tmpl_loop>
  </groups>

  <titles>
  
    <alias>
      <tmpl_loop name=loop_anime_title_alias>
      <title>
        <name><![CDATA[<tmpl_var name=data_anime_title_alias_name>]]></name>
        <langname><![CDATA[<tmpl_var name=data_anime_title_alias_langname>]]></langname>
        <langshort><![CDATA[<tmpl_var name=data_anime_title_alias_langsname>]]></langshort>
      </title></tmpl_loop>
    </alias>
  
    <short>
      <tmpl_loop name=loop_anime_title_short>
      <title>
        <name><![CDATA[<tmpl_var name=data_anime_title_short_name>]]></name>
        <langname><![CDATA[<tmpl_var name=data_anime_title_short_langname>]]></langname>
        <langshort><![CDATA[<tmpl_var name=data_anime_title_short_langsname>]]></langshort>
      </title></tmpl_loop>
    </short>

    <official>
      <tmpl_loop name=loop_anime_title_official>
      <title>
        <name><![CDATA[<tmpl_var name=data_anime_title_official_name>]]></name>
        <langname><![CDATA[<tmpl_var name=data_anime_title_official_langname>]]></langname>
        <langshort><![CDATA[<tmpl_var name=data_anime_title_official_langsname>]]></langshort>
      </title></tmpl_loop>
    </official>

    <all>
      <tmpl_loop name=loop_anime_titles>
      <title>
        <name><![CDATA[<tmpl_var name=data_anime_title_name>]]></name>
        <lang id="<tmpl_var name=data_anime_title_langid>">
          <type id="<tmpl_var name=data_anime_title_type_id>"><![CDATA[<tmpl_var name=data_anime_title_type_string>]]></type>
          <long><![CDATA[<tmpl_var name=data_anime_title_langname>]]></long>
          <short><![CDATA[<tmpl_var name=data_anime_title_langsname>]]></short>
        </lang>
      </title></tmpl_loop>
    </all>

  </titles>
  
  <award_types>
    <tmpl_loop name=loop_anime_award_types>
    <awardtype id="<tmpl_var name=data_anime_award_type_id>">
      <name><![CDATA[<tmpl_var name=data_anime_award_type_name>]]></name>
      <img><![CDATA[<tmpl_var name=data_anime_award_type_img>]]></img>
    </awardtype></tmpl_loop>
  </award_types>

  <awards>
    <tmpl_loop name=loop_anime_awards>
    <award id="<tmpl_var name=data_anime_award_id>">
      <name><![CDATA[<tmpl_var name=data_anime_award_name>]]></name>
      <type><tmpl_var name=data_anime_award_type></type>
      <url><![CDATA[<tmpl_var name=data_anime_award_url>]]></url>
      <picurl><![CDATA[<tmpl_var name=data_anime_award_picurl>]]></picurl>
    </award></tmpl_loop>
  </awards>

  <genres>
    <tmpl_loop name=loop_anime_genre>
    <genre id="<tmpl_var name=data_anime_genre_id>">
      <name><![CDATA[<tmpl_var name=data_anime_genre_name>]]></name>
    </genre></tmpl_loop>
  </genres>
  
  <episodes>
    <tmpl_loop name=loop_ep>
    <episode id="<tmpl_var name=data_ep_id>" number="<tmpl_var name=data_ep_epno>">
      <name><![CDATA[<tmpl_var name=data_ep_name>]]></name>
      <tmpl_if name=status_ep_hasname_romaji><name_romaji><![CDATA[<tmpl_var name=data_ep_name_romaji>]]></name_romaji></tmpl_if>
      <tmpl_if name=status_ep_hasname_kanji><name_kanji><![CDATA[<tmpl_var name=data_ep_name_kanji>]]></name_kanji></tmpl_if>
      <length><tmpl_var name=data_ep_length></length>
      <aired short="<tmpl_var name=data_ep_aired_short>"><tmpl_var name=data_ep_aired></aired>
      <other><![CDATA[<tmpl_var name=data_ep_other>]]></other>
      <rating><tmpl_var name=data_ep_rating></rating>
      <votes><tmpl_var name=data_ep_votes></votes>
      <myvote date="<tmpl_var name=data_ep_myvote_date>"><tmpl_var name=data_ep_myvote></myvote>
      <date short="<tmpl_var name=data_ep_date_short>"><tmpl_var name=data_ep_date></date>
      <update short="<tmpl_var name=data_ep_update_short>"><tmpl_var name=data_ep_update></update>
      <state><tmpl_var name=data_ep_state></state>
      <state_special><tmpl_var name=status_ep_state_special></state_special>
      <state_recap><tmpl_var name=status_ep_state_recap></state_recap>
      <state_op><tmpl_var name=status_ep_state_op></state_op>
      <state_end><tmpl_var name=status_ep_state_end></state_end>
      <iswatched><tmpl_var name=status_ep_iswatched></iswatched>
      <hasfiles><tmpl_var name=status_ep_hasfile></hasfiles>

      <mystate id="<tmpl_var name=data_ep_mystate>">
        <value><tmpl_var name=data_ep_mystate></value>
        <icon><![CDATA[<tmpl_var name=data_ep_mystate_icon>]]></icon>
        <undef><tmpl_var name=status_ep_mystate_undef></undef>
        <unknown><tmpl_var name=status_ep_mystate_undef></unknown>
        <onhdd><tmpl_var name=status_ep_mystate_undef></onhdd>
        <oncd><tmpl_var name=status_ep_mystate_undef></oncd>
        <deleted><tmpl_var name=status_ep_mystate_undef></deleted>
      </mystate>
      
      <files>
        <tmpl_loop name=loop_file>
        <file id="<tmpl_var name=data_file_id>">
          <size><tmpl_var name=data_file_size></size>
          <size_plain><tmpl_var name=data_file_size_plain></size_plain>
          <size_formated><tmpl_var name=data_file_size_h></size_formated>
          <isgeneric><tmpl_var name=status_file_isgeneric></isgeneric>
          <filetype><![CDATA[<tmpl_var name=data_file_filetype>]]></filetype>
          
          <hash_crc><tmpl_var name=data_file_crc></hash_crc>
          <hash_md5><tmpl_var name=data_file_md5></hash_md5>
          <hash_sha1><tmpl_var name=data_file_sha1></hash_sha1>
          <hash_tth><tmpl_var name=data_file_tth></hash_tth>
          <hash_ed2k>
            <key><tmpl_var name=data_file_ed2k_hash></key>
            <link><![CDATA[<tmpl_var name=data_file_ed2k_link>]]></link>
            <name><![CDATA[<tmpl_var name=data_file_ed2k_name>]]></name>
          </hash_ed2k>
                    
          <group id="<tmpl_var name=data_file_group_id>">
            <name><![CDATA[<tmpl_var name=data_file_group_name>]]></name>
            <shortname><![CDATA[<tmpl_var name=data_file_group_shortname>]]></shortname>
          </group>
                    
          <released short="<tmpl_var name=data_file_released_short>"><tmpl_var name=data_file_released></released>
          
          <video>
            <count><tmpl_var name=data_file_vidcnt></count>
            <tmpl_if expr="data_file_vidcnt > 0"><stream1>
              <res><![CDATA[<tmpl_var name=data_file_res_name>]]></res>
              <aspectratio><tmpl_var name=data_file_ar></aspectratio>
              <bitrate><tmpl_var name=data_file_vbitrate></bitrate>
              <codec id="<tmpl_var name=data_file_vcodec_id>"><![CDATA[<tmpl_var name=data_file_vcodec_name>]]></codec>
              <flags><tmpl_var name=data_file_length></flags>
            </stream1></tmpl_if>
          </video>
          
          <audio>
            <count><tmpl_var name=data_file_audcnt></count>
            <stream1>
              <bitrate><tmpl_var name=data_file_abitrate></bitrate>
              <codec id="<tmpl_var name=data_file_acodec_id>"><![CDATA[<tmpl_var name=data_file_acodec_name>]]></codec>
              <chantype><tmpl_var name=data_file_chantype></chantype>
              <language id="<tmpl_var name=data_file_lang_id>">
               <name><![CDATA[<tmpl_var name=data_file_lang_name>]]></name>
                <shortname><![CDATA[<tmpl_var name=data_file_lang_sname>]]></shortname>
              </language>
            </stream1>
            <tmpl_if expr="data_file_audcnt > 1"><stream2>
              <bitrate><tmpl_var name=data_file_abitrate2></bitrate>
              <codec id="<tmpl_var name=data_file_acodec_id2>"><![CDATA[<tmpl_var name=data_file_acodec_name2>]]></codec>
              <chantype><tmpl_var name=data_file_chantype2></chantype>
              <language id="<tmpl_var name=data_file_lang_id2>">
                <name><![CDATA[<tmpl_var name=data_file_lang_name2>]]></name>
                <shortname><![CDATA[<tmpl_var name=data_file_lang_sname2>]]></shortname>
              </language>
            </stream2></tmpl_if>
          </audio>
                                    
          <subtitle>
            <count><tmpl_var name=data_file_subcnt></count>
            <stream1>
              <flags><tmpl_var name=data_file_sflags></flags>
              <type><tmpl_var name=data_file_stype></type>
              <language id="<tmpl_var name=data_file_sub_id>">
                <name><![CDATA[<tmpl_var name=data_file_sub_name>]]></name>
                <shortname><![CDATA[<tmpl_var name=data_file_sub_sname>]]></shortname>
              </language>
            </stream1>
            <tmpl_if expr="data_file_subcnt > 1"><stream2>
              <flags><tmpl_var name=data_file_sflags2></flags>
              <type><tmpl_var name=data_file_stype2></type>
              <language id="<tmpl_var name=data_file_sub_id2>">
                <name><![CDATA[<tmpl_var name=data_file_sub_name2>]]></name>
                <shortname><![CDATA[<tmpl_var name=data_file_sub_sname2>]]></shortname>
              </language>
            </stream2></tmpl_if>
          </subtitle>

          <container>
            <qual id="<tmpl_var name=data_file_qual_id>"><![CDATA[<tmpl_var name=data_file_qual_name>]]></qual>
            <fps><tmpl_var name=data_file_fps></fps>
            <length><tmpl_var name=data_file_length></length>
          </container>
          
          <type id="<tmpl_var name=data_file_type_id>">
            <value><tmpl_var name=data_file_type></value>
            <name><![CDATA[<tmpl_var name=data_file_type_name>]]></name>
          </type>
          
          <ldate>
            <long><![CDATA[<tmpl_var name=data_file_ldate>]]></long>
            <short><![CDATA[<tmpl_var name=data_file_ldate_short>]]></short>
          </ldate>
          <date>
            <long><![CDATA[<tmpl_var name=data_file_date>]]></long>
            <short><![CDATA[<tmpl_var name=data_file_date_short>]]></short>
          </date>
          <update>
            <long><![CDATA[<tmpl_var name=data_file_update>]]></long>
            <short><![CDATA[<tmpl_var name=data_file_update_short>]]></short>
          </update>
          <viewdate>
            <long><![CDATA[<tmpl_var name=data_file_viewdate>]]></long>
            <short><![CDATA[<tmpl_var name=data_file_viewdate_short>]]></short>
          </viewdate>
          
          <storage><![CDATA[<tmpl_var name=data_file_storage>]]></storage>
          <source><![CDATA[<tmpl_var name=data_file_source>]]></source>

          <hascomment><tmpl_var name=status_file_hascomment></hascomment>
          <tmpl_if name=data_file_other><comment>
            <![CDATA[<tmpl_var name=data_file_other>]]>
          </comment></tmpl_if>
          
          <state code="<tmpl_var name=data_file_state>">
            <iscrcok><tmpl_var name=status_file_state_crcok></iscrcok>
            <iscrcfailed><tmpl_var name=status_file_state_crcfailed></iscrcfailed>
            <iscrcunverified><tmpl_var name=status_file_state_crcunverified></iscrcunverified>
            <isv2><tmpl_var name=status_file_state_isv2></isv2>
            <isv3><tmpl_var name=status_file_state_isv3></isv3>
            <isv4><tmpl_var name=status_file_state_isv4></isv4>
            <isv5><tmpl_var name=status_file_state_isv5></isv5>
            <iswatched><tmpl_var name=status_file_iswatched></iswatched>
            <versionname><![CDATA[<tmpl_var name=data_file_state_versionname>]]></versionname>
          </state>
          
          <mystate id="<tmpl_var name=data_file_mystate>">
            <string><![CDATA[<tmpl_var name=data_file_mystate_string>]]></string>
            <icon><![CDATA[<tmpl_var name=data_file_mystate_icon>]]></icon>
            <isunknown><tmpl_var name=status_file_mystate_unknown></isunknown>
            <isonhdd><tmpl_var name=status_file_mystate_onhdd></isonhdd>
            <isoncd><tmpl_var name=status_file_mystate_oncd></isoncd>
            <isdeleted><tmpl_var name=status_file_mystate_deleted></isdeleted>
          </mystate>
          
          <myfilestate id="<tmpl_var name=data_file_myfilestate>">
            <string><![CDATA[<tmpl_var name=data_file_myfilestate_string>]]></string>
            <icon><![CDATA[<tmpl_var name=data_file_myfilestate_icon>]]></icon>
            <isnormal><tmpl_var name=status_file_myfilestate_normal></isnormal>
            <isinvalidcrc><tmpl_var name=status_file_myfilestate_invalidcrc></isinvalidcrc>
            <isselfedited><tmpl_var name=status_file_myfilestate_selfedited></isselfedited>
            <isselfripped><tmpl_var name=status_file_myfilestate_selfripped></isselfripped>
            <isondvd><tmpl_var name=status_file_myfilestate_ondvd></isondvd>
            <isonvhs><tmpl_var name=status_file_myfilestate_onvhs></isonvhs>
            <isontv><tmpl_var name=status_file_myfilestate_ontv></isontv>
            <istheater><tmpl_var name=status_file_myfilestate_theater></istheater>
            <isother><tmpl_var name=status_file_myfilestate_other></isother>
          </myfilestate>
                  
          <filerelations>
            <tmpl_loop name=loop_data_file_filerel>
            <filerelation fid="<tmpl_var name=data_file_filerel_fid>" otherfid="<tmpl_var name=data_file_filerel_otherfid>" type="<tmpl_var name=data_file_filerel_type>" />
            </tmpl_loop>
          </filerelations>
        
          <eprelations>
            <tmpl_loop name=loop_data_file_fileeprel>
            <eprelation fid="<tmpl_var name=data_file_fileeprel_fid>" eid="<tmpl_var name=data_file_fileeprel_eid>">
              <startp><tmpl_var name=data_file_fileeprel_startp></startp>
              <endp><tmpl_var name=data_file_fileeprel_endp></endp>
            </eprelation></tmpl_loop>
          </eprelations>

        </file></tmpl_loop>
      </files>
        
    </episode></tmpl_loop>
  </episodes>

</anime></tmpl_loop>
