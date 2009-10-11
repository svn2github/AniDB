<?xml version="1.0" encoding="UTF-8"?>
<mylist
	date="<tmpl_var name=global_date>"
>
<tmpl_loop name=loop_anime>
	<anime
		<tmpl_if name=status_anime_iscomplete>complete="1"</tmpl_if>
		<tmpl_if name=status_anime_iswatched>watched="1"</tmpl_if>
	>
		<id><![CDATA[<tmpl_var name=data_anime_id>]]></id>
		<name><![CDATA[<tmpl_var name=data_anime_name>]]></name>
		<my_vote><![CDATA[<tmpl_var name=data_anime_my_vote>]]></my_vote>
	</anime>
</tmpl_loop>
</mylist>
