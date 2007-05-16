<?xml version="1.0" encoding="UTF-8"?>
<!--
A my list page
© 2005 by Siddhi Pittayachawan
Version 0.1
-->
<?xml-stylesheet type="text/xsl" href="mylist.xsl"?>
<list>
	<userinfo>
		<name><tmpl_var name=global_user></name>
		<export><tmpl_var name=global_date_short></export>
		<anime><tmpl_var name=global_animecount></anime>
		<ep><tmpl_var name=global_epcount></ep>
		<file><tmpl_var name=global_filecount></file>
		<size><tmpl_var name=global_bytecount_h></size>
	</userinfo>
	<tmpl_loop name=loop_anime>
		<anime>
			<id><tmpl_var name=data_anime_id></id>
			<name><tmpl_var name=data_anime_name></name>
			<year><tmpl_var name=data_anime_year></year>
			<type><tmpl_var name=data_anime_type_name></type>
			<epnumber>
				<total><tmpl_var name=data_anime_eps></total>
				<own><tmpl_var name=data_anime_my_eps></own>
				<watch><tmpl_var name=data_anime_my_watchedeps></watch>
				<ownsp><tmpl_var name=data_anime_my_eps_special></ownsp>
				<watchsp><tmpl_var name=data_anime_my_watchedeps_special></watchsp>
			</epnumber>
			<vote>
				<voter><tmpl_var name=data_anime_votes></voter>
				<rating><tmpl_var name=data_anime_rating></rating>
			</vote>
			<review>
				<reviewer><tmpl_var name=data_anime_reviews></reviewer>
				<rating><tmpl_var name=data_anime_reviewrating></rating>
			</review>
			<award>
				<tmpl_loop name=loop_anime_award_types>
						<name><tmpl_var name=data_anime_award_type_name></name>
				</tmpl_loop>
			</award>
			<size><tmpl_var name=data_anime_my_size_h></size>
			<state><tmpl_var name=data_anime_mystate></state>
		</anime>
	</tmpl_loop>
</list>
