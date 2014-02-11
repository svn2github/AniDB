{
"Animes": [
	<tmpl_loop name=loop_anime>
	{
		"aid": <tmpl_var name=data_anime_id>,
		"romaji": "<tmpl_var name=data_anime_name>",
		"type": "<tmpl_var name=data_anime_type_name >",
		"Episodes": [<tmpl_loop name=loop_ep>
			{
				"eid": <tmpl_var name=data_ep_id>,
				"episode": "<tmpl_var name=data_ep_epno>",
				"name": "<tmpl_var name=data_ep_name>",
				"Files": [<tmpl_loop name=loop_file>
					{
						"fid": <tmpl_var name=data_file_id>,
						"crc": "<tmpl_var name=data_file_crc>",
						"md5": "<tmpl_var name=data_file_md5>",
						"size": <tmpl_var name=data_file_size_plain>,
						"ed2k": "<tmpl_var name=data_file_ed2k_hash>",
						"Group": {
							"gid": <tmpl_var name=data_file_group_id>,
							"group": "<tmpl_var name=data_file_group_name>"
						}
					}<tmpl_unless __last__>,</tmpl_unless>
</tmpl_loop>
				]
			}<tmpl_unless __last__>,</tmpl_unless>
</tmpl_loop>
		]
	}<tmpl_unless __last__>,</tmpl_unless>
</tmpl_loop>
]
}
