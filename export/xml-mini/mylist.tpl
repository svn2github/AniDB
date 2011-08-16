<?xml version="1.0" encoding="UTF-8" ?>
<mylist uid="<tmpl_var name=global_user_id>"
        username="<tmpl_var name=global_user>"
        date="<tmpl_var name=global_date>"><tmpl_loop name=loop_anime>
	<anime aid="<tmpl_var name=data_anime_id>"
		   type="<tmpl_var name=data_anime_type_name>"
		   year="<tmpl_var name=data_anime_year>">
		<titles>
			<default><![CDATA[<tmpl_var name=data_anime_name>]]></default>
			<nihongo><![CDATA[<tmpl_var name=data_anime_title_jap_kanji>]]></nihongo>
			<english><![CDATA[<tmpl_var name=data_anime_title_eng>]]></english>
		</titles><tmpl_if expr="data_anime_my_eps > 0">
		<episodes total="<tmpl_var name=data_anime_eps>"><tmpl_loop name=loop_ep>
			<episode eid="<tmpl_var name=data_ep_id>"
					 epno="<tmpl_var name=data_ep_epno>"
					 aired="<tmpl_var name=data_ep_aired>"
					 watched="<tmpl_var name=status_ep_iswatched>">
				<titles>
					<english><![CDATA[<tmpl_var name=data_ep_name>]]></english>
					<nihongo><![CDATA[<tmpl_var name=data_ep_name_kanji>]]></nihongo>
					<romaji><![CDATA[<tmpl_var name=data_ep_name_romaji>]]></romaji>
				</titles><tmpl_if name=status_ep_hasfile>
				<files><tmpl_loop name=loop_file>
					<file fid="<tmpl_var name=data_file_id>"
						  lid="<tmpl_var name=data_file_lid>"
						  watched="<tmpl_var name=data_file_viewdate>"<tmpl_if name=status_file_isgeneric>
						  generic="<tmpl_var name=status_file_isgeneric>" /><tmpl_else><tmpl_if expr="data_file_group_id > 0">
						  gid="<tmpl_var name=data_file_group_id>"</tmpl_if>
						  ed2k="<tmpl_var name=data_file_ed2k_hash>"
						  length="<tmpl_var name=data_file_length>"
						  size="<tmpl_var name=data_file_size_plain>"
						  source="<tmpl_var name=data_file_type_name>"
						  acodec="<tmpl_var name=data_file_acodec_name>"						  
						  vcodec="<tmpl_var name=data_file_vcodec_name>"
						  vres="<tmpl_var name=data_file_res_name>"<tmpl_if expr="data_file_group_id > 0">>
						  <group_name><![CDATA[<tmpl_var name=data_file_group_name>]]></group_name>
						  <group_abbr><![CDATA[<tmpl_var name=data_file_group_shortname>]]></group_abbr>
					</file><tmpl_else> /></tmpl_if></tmpl_if></tmpl_loop>
				</files></tmpl_if>
			</episode></tmpl_loop>
		</episodes></tmpl_if>
	</anime></tmpl_loop>
</mylist>
