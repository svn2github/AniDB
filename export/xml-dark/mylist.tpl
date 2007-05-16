<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="mylista.xsl"?>

<AC
	user="<tmpl_var name=global_user>" 
	time="<tmpl_var name=global_date>" 
	size="<tmpl_var name=global_bytecount_h>" 
	nanime="<tmpl_var name=global_animecount>"
	nfiles="<tmpl_var name=global_filecount>" >

	<tmpl_loop name=loop_anime>
	<Anime watched="<tmpl_var name=status_anime_iswatched>" complete="<tmpl_var name=status_anime_iscomplete>">
		<Name><![CDATA[<tmpl_var name=data_anime_name>]]></Name>
		<Year><tmpl_var name=data_anime_year></Year>
		<Url ><tmpl_var name=data_anime_id></Url>
		<Rate><tmpl_var name=data_anime_rating></Rate>
		<Size><tmpl_var name=data_anime_my_size></Size>
		<Type><tmpl_var name=data_anime_type_name></Type>
		<Files number="<tmpl_var name=data_anime_my_eps_total>"><tmpl_loop name=loop_ep>
			<Episode nr="<tmpl_var name=data_ep_epno>"><tmpl_loop name=loop_file>
			<File fid="<tmpl_var name=data_file_id>">
				<Name><![CDATA[<tmpl_var name=data_file_ed2k_name>]]></Name>
				<ED2K><![CDATA[<tmpl_var name=data_file_ed2k_link>]]></ED2K>								<Size><tmpl_var name=data_file_size_plain></Size>
				<CRC><tmpl_var name=data_file_crc></CRC>
				<Type><tmpl_var name=data_file_filetype></Type>
				<State><tmpl_var name=data_file_mystate_string></State>
				<Group><![CDATA[<tmpl_var name=data_file_group_shortname>]]></Group>
			</File></tmpl_loop>
			</Episode></tmpl_loop>
		</Files>
	</Anime></tmpl_loop>
</AC>
