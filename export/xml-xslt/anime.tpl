<?xml version="1.0" encoding="UTF-8"?>
<tmpl_loop name=loop_anime>
<!--
An anime info page
© 2005 by Siddhi Pittayachawan
Version 0.1
-->
<?xml-stylesheet type="text/xsl" href="../anime.xsl"?>
	<anime>
		<id><tmpl_var name=data_anime_id></id>
		<name>
			<romanji><tmpl_var name=data_anime_name></romanji>
			<kanji><tmpl_var name=data_anime_title_jap_kanji></kanji>
			<english><tmpl_var name=data_anime_title_eng></english>
			<other><tmpl_var name=data_anime_title_other></other>
			<synonym>
				<tmpl_loop name=loop_anime_title_alias>
					<alias><tmpl_var name=data_anime_title_alias_name></alias>
				</tmpl_loop>
			</synonym>
			<shorts>
				<tmpl_loop name=loop_anime_title_short>
					<short><tmpl_var name=data_anime_title_short_name></short>
				</tmpl_loop>
			</shorts>
		</name>
		<genres>
			<tmpl_loop name=loop_anime_genre>
				<genre><tmpl_var name=data_anime_genre_name></genre>
			</tmpl_loop>
		</genres>
		<type><tmpl_var name=data_anime_type_name></type>
		<epnumber>
			<total><tmpl_var name=data_anime_eps></total>
			<own><tmpl_var name=data_anime_my_eps></own>
			<watch><tmpl_var name=data_anime_my_watchedeps></watch>
			<ownsp><tmpl_var name=data_anime_my_eps_special></ownsp>
			<watchsp><tmpl_var name=data_anime_my_watchedeps_special></watchsp>
		</epnumber>
		<broadcast>
			<year><tmpl_var name=data_anime_year></year>
			<start><tmpl_var name=data_anime_startdate></start>
			<end><tmpl_var name=data_anime_enddate></end>
		</broadcast>
		<producer><tmpl_var name=data_anime_producer></producer>
		<url>
			<official><tmpl_var name=data_anime_url></official>
			<anidb><tmpl_var name=data_anime_animenfourl></anidb>
			<animeplanet><tmpl_var name=data_anime_animeplaneturl></animeplanet>
		</url>
		<feedback>
			<rating><tmpl_var name=data_anime_rating></rating>
			<vote><tmpl_var name=data_anime_votes></vote>
			<myvote><tmpl_var name=data_anime_my_vote></myvote>
			<tmprating><tmpl_var name=data_anime_tmprating></tmprating>
			<tmpvote><tmpl_var name=data_anime_tmpvotes></tmpvote>
			<mytmpvote><tmpl_var name=data_anime_my_tmpvote></mytmpvote>
		</feedback>
		<review>
			<reviewer><tmpl_var name=data_anime_reviews></reviewer>
			<rating><tmpl_var name=data_anime_reviewrating></rating>
		</review>
		<size><tmpl_var name=data_anime_my_size_h></size>
		<synopsis><![CDATA[<tmpl_var name=data_anime_other>]]></synopsis>
		<award>
			<tmpl_loop name=loop_anime_awards>
				<type>
					<name><tmpl_var name=data_anime_award_name></name>
					<pic><tmpl_var name=data_anime_award_picurl></pic>
				</type>
			</tmpl_loop>
		</award>
		<episode>
			<tmpl_loop name=loop_ep>
				<epdetail>
					<id><tmpl_var name=data_ep_id></id>
					<number><tmpl_var name=data_ep_epno></number>
					<name><tmpl_var name=data_ep_name></name>
					<length><tmpl_var name=data_ep_length></length>
					<date><tmpl_var name=data_ep_date_short></date>
					<files>
						<tmpl_loop name=loop_file>
							<file>
								<id><tmpl_var name=data_file_id></id>
								<size><tmpl_var name=data_file_size></size>
								<lang><tmpl_var name=data_file_lang_name></lang>
								<sub><tmpl_var name=data_file_sub_name></sub>
								<groupid><tmpl_var name=data_file_group_id></groupid>
								<groupname><![CDATA[<tmpl_var name=data_file_group_shortname>]]></groupname>
								<quality><tmpl_var name=data_file_qual_name></quality>
								<resolution><tmpl_var name=data_file_res_name></resolution>
								<source><tmpl_var name=data_file_source></source>
								<ed2klink><![CDATA[<tmpl_var name=data_file_ed2k_link>]]></ed2klink>
								<ed2kname><![CDATA[<tmpl_var name=data_file_ed2k_name>]]></ed2kname>
							</file>
						</tmpl_loop>
					</files>
				</epdetail>
			</tmpl_loop>
		</episode>
	</anime>
</tmpl_loop>
