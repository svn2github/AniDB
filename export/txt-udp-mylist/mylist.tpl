# MYLIST export for user: <tmpl_var global_user> started at: <tmpl_var global_date>.
# MYLIST file count: <tmpl_var global_filecount>
# Format: 4 lines per MyList entry, simulating MYLIST from the server with some extra info:
#   # File: ED2K file name
#   # UID: file size in bytes:ED2K hash
#   221 MYLIST
#   {int4 lid}|{int4 fid}|{int4 eid}|{int4 aid}|{int4 gid}|{int4 date}|{int2 state}|{int4 viewdate}|{str storage}|{str source}|{str other}|{int2 filestate} 
<tmpl_loop name=loop_anime><tmpl_loop name=loop_ep><tmpl_loop name=loop_file># File: <tmpl_var name=data_file_ed2k_name>
# UID: <tmpl_var name=data_file_size_plain>:<tmpl_var name=data_file_ed2k_hash>
221 MYLIST
<tmpl_var name=data_file_lid>|<tmpl_var name=data_file_id>|<tmpl_var name=data_ep_id>|<tmpl_var name=data_anime_id>|<tmpl_var data_file_group_id>|<tmpl_var data_file_ldate_raw>|<tmpl_var data_file_mystate>|<tmpl_var data_file_viewdate_raw>|<tmpl_var data_file_storage>|<tmpl_var data_file_source>|<tmpl_var data_file_other>|<tmpl_var data_file_myfilestate>
</tmpl_loop></tmpl_loop></tmpl_loop># Done at <tmpl_var global_date>.