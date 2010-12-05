<tmpl_loop name=loop_anime>
{
   "id":"<tmpl_var name=data_anime_id>",
   "seriesInfo": {
       "name": {
           "romanji":"<tmpl_var expr="jsencode(data_anime_name)">",
           "kanji":"<tmpl_var expr="jsencode(data_anime_title_jap_kanji)">",
           "english":"<tmpl_var expr="jsencode(data_anime_title_eng)">",
           "other":"<tmpl_var expr="jsencode(data_anime_title_other)">",
           "synonym": [<tmpl_loop name=loop_anime_title_alias>
               "<tmpl_var expr="jsencode(data_anime_title_alias_name)">"
            <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>],
           "shorts":[<tmpl_loop name=loop_anime_title_short>
                       "<tmpl_var expr="jsencode(data_anime_title_short_name)">"
            <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>]
        },
       "genres": [<tmpl_loop name=loop_anime_genren> {
           "name":"<tmpl_var name=data_anime_genren_name>",
           "id":"<tmpl_var name=data_anime_genren_id>",
           "weight":"<tmpl_var name=data_anime_genren_weight>",
           "weightName":"<tmpl_var name=data_anime_genren_weight_name>"
        }
        <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>],
   "airingDate": {
       "year":"<tmpl_var name=data_anime_year>",
       "start":"<tmpl_var name=data_anime_startdate>",
       "end":"<tmpl_var name=data_anime_enddate>"  
    },
   "type":"<tmpl_var name=data_anime_type_name>",
   "rating":"<tmpl_var name=data_anime_rating>",
    <tmpl_if name=status_anime_hasawards>
   "awards": {
       "awardTypes": [<tmpl_loop name=loop_anime_award_types> {
           "id":"<tmpl_var name=data_anime_award_type_id>",
           "name":"<tmpl_var name=data_anime_award_type_name>"
        }
        <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>],
       "awardNames": [<tmpl_loop name=loop_anime_awards> {
           "awardTypeId":"<tmpl_var name=data_anime_award_type>",
           "awardName":"<tmpl_var expr="jsencode(data_anime_award_name)">",
           "awardURL":"<tmpl_var expr="jsencode(data_anime_award_url)">"
        }
        <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>]
    },
    </tmpl_if>
   "companies": [<tmpl_loop name=loop_anime_company>{
   "typeId":"<tmpl_var name=data_anime_company_aptype>",
   "type":"<tmpl_var expr="jsencode(data_anime_company_aptype_name)">",
   "name":"<tmpl_var expr="jsencode(data_anime_company_name)">"
    }
    <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>],
       "synopsis":"<tmpl_var expr="jsencode(data_anime_other)">"
    },
   "episodeInfo": {
       "status":"<tmpl_if name=status_anime_iscomplete>complete<tmpl_else>incomplete</tmpl_if>",
       "size":"<tmpl_var name=data_anime_my_size_h>",
       "episodeCount": {
           "total": {
               "totalEpisodes":"<tmpl_var name=data_anime_eps_total>",
               "watchedEpisodes":"<tmpl_var name=data_anime_my_watchedeps_total>",
               "ownEpisodes":"<tmpl_var name=data_anime_my_eps_total>"
            },
           "normal": {
               "totalEpisodes":"<tmpl_var name=data_anime_eps>",
               "watchedEpisodes":"<tmpl_var name=data_anime_my_watchedeps>",
               "ownEpisodes":"<tmpl_var name=data_anime_my_eps>"
            },
           "special": {
               "totalEpisodes":"<tmpl_var name=data_anime_eps_special>",
               "watchedEpisodes":"<tmpl_var name=data_anime_my_watchedeps_special>",
               "ownEpisodes":"<tmpl_var name=data_anime_my_eps_special>"
            }
        },
       "groups": [<tmpl_loop name=loop_anime_groups> {
           "id":"<tmpl_var name=data_anime_group_id>",
           "state":"<tmpl_var name=data_anime_group_state>",
           "rating":"<tmpl_var name=data_anime_group_rating>",
           "voteCount":"<tmpl_var name=data_anime_group_votes>",
           "releasedEpisodes": {
               "normal":"<tmpl_var name=data_anime_group_epcnt>",
               "special":"<tmpl_var name=data_anime_group_sepcnt>",
               "range":"<tmpl_var name=data_anime_group_eprange>"
            },
           "name":"<tmpl_var expr="jsencode(data_anime_group_name)">",
           "shortName":"<tmpl_var expr="jsencode(data_anime_group_sname)">"
        }
        <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>],
   "episodes": [<tmpl_loop name=loop_ep> {
   "id":"<tmpl_var name=data_ep_id>",
   "number":"<tmpl_var name=data_ep_epno>",
   "length":"<tmpl_var name=data_ep_length>",
   "name": {
       "english":"<tmpl_var expr="jsencode(data_ep_name)">",
       "romanji":"<tmpl_var expr="jsencode(data_ep_name_romaji)">",
       "kanji":"<tmpl_var expr="jsencode(data_ep_name_kanji)">",
       "titles": [<tmpl_loop name=loop_data_ep_titles>
       "<tmpl_var expr="jsencode(data_ep_title_name)">"
        <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>]
    },
   "airingDate":"<tmpl_var name=data_ep_aired_short>",
   "otherInfo":"<tmpl_var expr="jsencode(data_ep_other)">",
   "status": {
       "recap":"<tmpl_if name=status_ep_state_recap> true <tmpl_else> false </tmpl_if>",
       "special":"<tmpl_if name=status_ep_state_special>true<tmpl_else>false</tmpl_if>",
       "opening":"<tmpl_if name=status_ep_state_op> true <tmpl_else> false </tmpl_if>",
       "ending":"<tmpl_if name=status_ep_state_end>true<tmpl_else>false</tmpl_if>",
       "noFiles":"<tmpl_unless name=status_ep_hasfile> true <tmpl_else> false </tmpl_unless>",
       "type":"<tmpl_var name=data_ep_state>"
    },
   "files": [<tmpl_loop name=loop_file> {
       "fileEpRelations": [<tmpl_loop name=loop_data_file_fileeprel> {
           "id":"<tmpl_var name=data_file_fileeprel_fid>",
           "episode":"<tmpl_var name=data_file_fileeprel_eid>",
           "start":"<tmpl_var name=data_file_fileeprel_startp>",
           "end":"<tmpl_var name=data_file_fileeprel_endp>"
        }
        <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>],
   "id":"<tmpl_var name=data_file_id>",
   "size":"<tmpl_var name=data_file_size_h>",
   "type":"<tmpl_var name=data_file_filetype>",
   "state":"<tmpl_var name=data_file_state>",
   "version":"<tmpl_var name=data_file_state_versionname>",
   "generic":"<tmpl_if name=status_file_isgeneric>true<tmpl_else>false</tmpl_if>",
   "hashInformation": {
       "crc":"<tmpl_var name=data_file_crc>",
       "md5":"<tmpl_var name=data_file_md5>",
       "sha1":"<tmpl_var name=data_file_sha1>",
       "ed2kLink":"<tmpl_var expr="jsencode(data_file_ed2k_link)">"
    },
   "group": { 
       "id":"<tmpl_var name=data_file_group_id>",
       "name":"<tmpl_var expr="jsencode(data_file_group_name)">",
       "shortName":"<tmpl_var expr="jsencode(data_file_group_shortname)">"
    },
   "dates": { 
       "release":"<tmpl_var name=data_file_date_short>",
       "addition":"<tmpl_var name=data_file_ldate>" 
    },
   "quality":{ 
       "name":"<tmpl_var name=data_file_qual_name>",
       "source":"<tmpl_var name=data_file_type_name>",
       "userDefinedSource":"<tmpl_var name=data_file_source>"
    },
   "video": { 
           "resolution":"<tmpl_var name=data_file_res_name>",
           "aspectRatio":"<tmpl_var name=data_file_ar>",
           "framesPerSecond":"<tmpl_var name=data_file_fps>",
           "bitRate":"<tmpl_var name=data_file_vbitrate>",
           "codec":"<tmpl_var name=data_file_vcodec_name>"
        },
   "audioStreams": [ {
       "language":"<tmpl_var name=data_file_lang_name>",
       "channels":"<tmpl_var name=data_file_chantype>",
       "bitrate":"<tmpl_var name=data_file_abitrate>",
       "codec":"<tmpl_var name=data_file_acodec_name>"
    }
    <tmpl_if expr="data_file_audcnt> 1"> , {
       "language":"<tmpl_var name=data_file_lang_name2>",
       "channels":"<tmpl_var name=data_file_chantype2>",
       "bitrate":"<tmpl_var name=data_file_abitrate2>",
       "codec":"<tmpl_var name=data_file_acodec_name2>" 
    }
    </tmpl_if>],
   "subtitleStreams": [{
       "language":"<tmpl_var name=data_file_sub_name>",
       "flag":"<tmpl_var name=data_file_sflags>",
       "type":"<tmpl_var name=data_file_stype>"
    }
    <tmpl_if expr="data_file_subcnt> 1">, {
       "language":"<tmpl_var name=data_file_sub_name2>",
       "flag":"<tmpl_var name=data_file_sflags2>",
       "type":"<tmpl_var name=data_file_stype2>" 
    }
        </tmpl_if>],               
       "comments":"<tmpl_var expr="jsencode(data_file_other)">"
    }
        <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>]
    }
    <tmpl_unless __last__>,</tmpl_unless></tmpl_loop>]
  }
}
 </tmpl_loop>