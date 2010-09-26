# MYLIST export for user: <tmpl_var global_user> on <tmpl_var global_date>.
# MyList anime count: <tmpl_var global_animecount>, episode count: <tmpl_var global_epcount>, file count: <tmpl_var global_filecount>
# Format: 2 lines per MyList entry, almost like MYLIST from the server:
#   # file size in bytes:ED2K hash:ED2K file name (this is used a double check until I get things verified)
#   221 MYLIST
#   {int4 lid}|{int4 fid}|{int4 eid}|{int4 aid}|{int4 gid}|{int4 date}|{int2 state}|{int4 viewdate}|{str storage}|{str source}|{str other}|{int2 filestate} 
<tmpl_loop name=loop_anime><tmpl_loop name=loop_ep><tmpl_loop name=loop_file><tmpl_loop name=loop_anime_groups># <tmpl_var name=data_file_size_plain>:<tmpl_var name=data_file_ed2k_hash>:<tmpl_var name=data_file_ed2k_name>
221 MYLIST
<tmpl_var name=data_file_lid>|<tmpl_var name=data_file_id>|<tmpl_var name=data_ep_id>|<tmpl_var name=data_anime_id>|<tmpl_var data_anime_group_id>|<tmpl_var data_file_ldate>|<tmpl_var data_file_mystate>|<tmpl_var data_file_viewdate>|<tmpl_var data_file_storage>|<tmpl_var data_file_source>|<tmpl_var data_file_other>|<tmpl_var data_file_filestate>
</tmpl_loop></tmpl_loop></tmpl_loop></tmpl_loop>