<?xml version="1.0" encoding="UTF-8"?>
<export name="xml-mystatus" v="0.1" user="<tmpl_var name=global_user>" time="<tmpl_var name=global_date>" />
<anime>
<tmpl_loop name=loop_anime>
	<entry id="<tmpl_var name=data_anime_id>" is_watched="<tmpl_var name=status_anime_iswatched>" is_completed="<tmpl_var name=status_anime_iscomplete>" my_rating="<tmpl_var name=data_anime_my_vote>" my_rating_temp="<tmpl_var name=data_anime_my_tmpvote>" eps="<tmpl_var name=data_anime_my_watchedeps_total>">
		<name><![CDATA[<tmpl_var name=data_anime_name>]]></name>
	</entry>
</tmpl_loop>
</anime>
