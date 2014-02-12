{<tmpl_loop name=loop_anime>
	"aid": <tmpl_var name=data_anime_id>,
	"romaji": "<tmpl_var name=data_anime_name>",<tmpl_if expr="status_anime_title_has_jap_kanji == 1">
	"kanji": "<tmpl_var name=data_anime_title_jap_kanji>",</tmpl_if><tmpl_if expr="status_anime_title_has_eng == 1">
	"english": "<tmpl_var name=data_anime_title_eng>",</tmpl_if>
	"type": "<tmpl_var name=data_anime_type_name>",
	"Episodes": [<tmpl_loop name=loop_ep>
		{
			"eid": <tmpl_var name=data_ep_id>,
			"number": "<tmpl_var name=data_ep_epno>",
			"english": "<tmpl_var name=data_ep_name>",<tmpl_if expr="status_ep_hasname_kanji == 1">
			"kanji": "<tmpl_var name=data_ep_name_kanji>",</tmpl_if><tmpl_if expr="status_ep_hasname_romaji == 1">
			"romaji": "<tmpl_var name=data_ep_name_romaji>",</tmpl_if>
			"Files": [<tmpl_loop name=loop_file>
				{
					"fid": <tmpl_var name=data_file_id>,
					"crc": "<tmpl_var name=data_file_crc>",
					"md5": "<tmpl_var name=data_file_md5>",
					"size": <tmpl_var name=data_file_size_plain>,
					"ed2k": "<tmpl_var name=data_file_ed2k_hash>",
					"Group": {
						"gid": <tmpl_var name=data_file_group_id>,
						"name": "<tmpl_var name=data_file_group_name>",
						"shortName": "<tmpl_var name=data_file_group_shortname>"
					}, 
					"MyList": {
							"mystate": <tmpl_var name=data_file_mystate>,
							"mystateString": "<tmpl_var name=data_file_mystate_string>",
							"state": <tmpl_var name=data_file_state>,
							"version": "<tmpl_var name=data_file_state_versionname>",
							"viewdate": "<tmpl_var name=data_file_viewdate>"
					}
				}<tmpl_unless __last__>,</tmpl_unless> </tmpl_loop>
			]
		}<tmpl_unless __last__>,</tmpl_unless> </tmpl_loop>
	],
	"Categories": [<tmpl_loop name=loop_anime_genren>
		{
			"id": "<tmpl_var name=data_anime_genren_id>",
			"name": "<tmpl_var name=data_anime_genren_name>",
			"isHentai": <tmpl_if expr="data_anime_genren_ishentai == 1">true</tmpl_if><tmpl_if expr="data_anime_genren_ishentai == 0">false</tmpl_if>,
			"weight": <tmpl_var name=data_anime_genren_weight>
		}<tmpl_unless __last__>,</tmpl_unless></tmpl_loop>
	]
}<tmpl_unless __last__>,</tmpl_unless> </tmpl_loop>
