{
    "user": "<tmpl_var name=global_user>",
    "date": "<tmpl_var name=global_date_short>",
    "list": {
        "count": "<tmpl_var name=global_animecount>",
        "episodes": "<tmpl_var name=global_epcount>",
        "files": "<tmpl_var name=global_filecount>",
        "size": "<tmpl_var name=global_bytecount_h>"
    },
    "genres": [ <tmpl_loop name=global_genren_loop > {
        "id": <tmpl_var name=global_genren_id > ,
        "name": "<tmpl_var name=global_genren_name>",
        "parentId": <tmpl_var name=global_genren_parentid >
    }
     <tmpl_unless __last__>,</tmpl_unless> </tmpl_loop>],
    "anime": [<tmpl_loop name= loop_anime> {
        "id": <tmpl_var name=data_anime_id>,
        "broadcastDate":"<tmpl_var name= data_anime_startdate>",
        "type":"<tmpl_var name= data_anime_type_name>",
        "rating":"<tmpl_var name= data_anime_rating>",
        "status":"<tmpl_if name=status_anime_iscomplete>complete<tmpl_else>incomplete</tmpl_if >",
        "totalEpisodes": "<tmpl_var name=data_anime_eps >", 
        "ownEpisodes": "<tmpl_var name=data_anime_my_eps >",
        "romanjiName": "<tmpl_var expr="jsencode(data_anime_name)" >",
        "englishName": "<tmpl_var expr="jsencode(data_anime_title_eng)" >",
        "specialEpisodes": {
            "totalEpisodes": "<tmpl_var name=data_anime_eps_special >",
            "ownEpisodes":"<tmpl_var name=data_anime_my_eps_special >"
        },
        "groups": [<tmpl_loop name= loop_anime_groups> {
            "id": "<tmpl_var name=data_anime_group_id >",
            "state": "<tmpl_var name=data_anime_group_state >"
        }
         <tmpl_unless __last__>,</tmpl_unless> </tmpl_loop>],
        "episodes": [<tmpl_loop name= loop_ep> {
            "id": "<tmpl_var name=data_ep_id >",
            "number": "<tmpl_var name=data_ep_epno >",
            "files": [<tmpl_loop name= loop_file> {
                "id":"<tmpl_var name=data_file_id >",
                "additionDate":"<tmpl_var name=data_file_ldate >",
                "releasedBy":"<tmpl_var name=data_file_group_id >"   
            }
             <tmpl_unless __last__>,</tmpl_unless> </tmpl_loop>]
        }
         <tmpl_unless __last__>,</tmpl_unless> </tmpl_loop>]
    }
     <tmpl_unless __last__>,</tmpl_unless> </tmpl_loop>],
    "globalDate":"<tmpl_var name=global_date >"
}