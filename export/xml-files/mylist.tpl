<?xml version="1.0" encoding="ISO-8859-1"?>
<mylist
	uid="<tmpl_var name=global_user_id>"
	username="<tmpl_var name=global_user>"
	date="<tmpl_var name=global_date>"
	size="<tmpl_var name=global_bytecount>"
><tmpl_loop name=loop_anime>
	<anime aid="<tmpl_var name=data_anime_id>">
		<title><![CDATA[<tmpl_var name=data_anime_name>]]></title>
		<date start="<tmpl_var name=data_anime_startdate>" end="<tmpl_var name=data_anime_enddate>" />
		<episodes normal="<tmpl_var name=data_anime_eps>" special="<tmpl_var name=data_anime_eps_special>" />
		<status complete="<tmpl_var name=status_anime_iscomplete>" watched="<tmpl_var name=status_anime_iswatched>" size="<tmpl_var name=data_anime_my_size>" />
		<files><tmpl_loop name=loop_ep><tmpl_loop name=loop_file>
			<file
				fid="<tmpl_var name=data_file_id>"
				eid="<tmpl_var name=data_ep_id>"
				epno="<tmpl_var name=data_ep_epno>"
				gid="<tmpl_var name=data_file_group_id>"
				added="<tmpl_var name=data_file_ldate>"
				watched="<tmpl_var name=data_file_viewdate>"
				ed2k="<tmpl_var name=data_file_ed2k_hash>"
				size="<tmpl_var name=data_file_size_plain>"
				length="<tmpl_var name=data_file_length>"
				generic="<tmpl_var name=status_file_isgeneric>"
			/></tmpl_loop></tmpl_loop>
		</files>
	</anime></tmpl_loop>
</mylist>
