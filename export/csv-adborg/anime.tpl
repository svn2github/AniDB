AID|EID|EPNo|Name|Romaji|Kanji|Length|Aired|Rating|Votes|State|myState
<tmpl_loop name=loop_anime><tmpl_loop name=loop_ep><tmpl_var name=data_anime_id>|<tmpl_var name=data_ep_id>|<tmpl_var name=data_ep_epno>|<tmpl_var name=data_ep_name>|<tmpl_var name=data_ep_name_romanji>|<tmpl_var name=data_ep_name_kanji>|<tmpl_var name=data_ep_length>|<tmpl_var name=data_ep_aired>|<tmpl_var name=data_ep_rating>|<tmpl_var name=data_ep_votes>|<tmpl_var name=data_ep_state>|<tmpl_var name=data_ep_mystate>
</tmpl_loop></tmpl_loop>FILES
AID|EID|GID|FID|size|ed2k|md5|sha1|crc|Length|Type|Quality|Resoltuion|vCodec|aCodec|sub|dub|isWatched|State|myState|ext
<tmpl_loop name=loop_anime><tmpl_loop name=loop_ep><tmpl_loop name=loop_file><tmpl_var name=data_anime_id>|<tmpl_var name=data_ep_id>|<tmpl_var name=data_file_group_id>|<tmpl_var name=data_file_id>|<tmpl_var name=data_file_size_plain>|<tmpl_var name=data_file_ed2k_hash>|<tmpl_var name=data_file_md5>|<tmpl_var name=data_file_sha1>|<tmpl_var name=data_file_crc>|<tmpl_var name=data_file_length>|<tmpl_var name=data_file_type>|<tmpl_var name="data_file_qual_name">|<tmpl_var name="data_file_res_name">|<tmpl_var name="data_file_vcodec_name">|<tmpl_var name="data_file_acodec_name">|<tmpl_var name="data_file_sub_name">|<tmpl_var name="data_file_lang_name">|<tmpl_var name=status_file_iswatched>|<tmpl_var name=data_file_state>|<tmpl_var name=data_file_mystate>|<tmpl_var name=data_file_filetype>
</tmpl_loop></tmpl_loop></tmpl_loop>GROUPS
GID|AGID|Name|ShortName|State
<tmpl_loop name=loop_anime><tmpl_loop name=loop_anime_groups><tmpl_var name=data_anime_group_id>|<tmpl_var name=data_anime_group_agid>|<tmpl_var name=data_anime_group_name>|<tmpl_var name=data_anime_group_sname>|<tmpl_var name=data_anime_group_state>
</tmpl_loop></tmpl_loop>GENREN
GenID|GenName|isHentai|Weight
<tmpl_loop name=loop_anime><tmpl_loop name=loop_anime_genren><tmpl_var name=data_anime_genren_id>|<tmpl_var name=data_anime_genren_name>|<tmpl_var name=data_anime_genren_ishentai>|<tmpl_var name=data_anime_genren_weight>
</tmpl_loop></tmpl_loop>
