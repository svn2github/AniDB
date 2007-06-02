<?xml version="1.0" encoding="ISO-8859-1"?><?xml-stylesheet type="text/xsl" href="mylista.xsl"?>
<AC u="<tmpl_var name=global_user>" t="<tmpl_var name=global_date>" s="<tmpl_var name=global_bytecount_h>" n="<tmpl_var name=global_animecount>" m="<tmpl_var name=global_filecount>">
<tmpl_loop name=loop_anime>
<A i="<tmpl_var name=data_anime_id>" w="<tmpl_var name=status_anime_iswatched>" c="<tmpl_var name=status_anime_iscomplete>">
	<N><![CDATA[<tmpl_var name=data_anime_name>]]></N>
	<Y><tmpl_var name=data_anime_year></Y>
	<R><tmpl_var name=data_anime_rating></R>
	<S><tmpl_var name=data_anime_my_size></S>
	<T><tmpl_var name=data_anime_type_name></T>
	<D l="<tmpl_var name=data_anime_my_eps_total>"><tmpl_loop name=loop_ep>
		<E n="<tmpl_var name=data_ep_epno>"><tmpl_loop name=loop_file>
		<F i="<tmpl_var name=data_file_id>" l="<tmpl_var name=data_file_size_plain>">
			<N><![CDATA[<tmpl_var name=data_file_ed2k_name>]]></N>
			<D><tmpl_var name=data_file_ed2k_hash></D>
			<C><tmpl_var name=data_file_crc></C>
			<T><tmpl_var name=data_file_filetype></T>
			<S><tmpl_var name=data_file_mystate_string></S>
			<G><![CDATA[<tmpl_var name=data_file_group_shortname>]]></G>
		</F></tmpl_loop>
		</E></tmpl_loop>
	</D>
</A></tmpl_loop>
</AC>
