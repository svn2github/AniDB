<?xml version="1.0" encoding="UTF-8"?>
<root anidb="<tmpl_var name=global_animedburl>">
	<custom uid="<tmpl_var name=global_user_id>">
		<userinfo>
			<name><tmpl_var name=global_user></name>
			<animes added="<tmpl_var name=global_userstat_animeadded>" user="<tmpl_var name=global_animecount>" />
			<eps added="<tmpl_var name=global_userstat_epsadded>" dbviewedp="<tmpl_var name=global_userstat_totalviewedp>" dbownedp="<tmpl_var name=global_userstat_totalownedp>" viewedp="<tmpl_var name=global_userstat_ownviewedp>" viewed="<tmpl_var name=global_userstat_viewedepcnt>" user="<tmpl_var name=global_epcount>" />
			<files added="<tmpl_var name=global_userstat_filesadded>" lamecnt="<tmpl_var name=global_userstat_lamefiles>" user="<tmpl_var name=global_filecount>" />
			<groups added="<tmpl_var name=global_userstat_groupsadded>" />
			<size longn="<tmpl_var name=global_bytecount>" shortn="<tmpl_var name=global_bytecount_h>" />
			<stats votes="<tmpl_var name=global_userstat_reviews>" reviews="<tmpl_var name=global_userstat_votes>" />
		</userinfo>
	</custom>
	<animes><tmpl_loop name=loop_anime>
		<anime id="<tmpl_var name=data_anime_id>" type="<tmpl_var name=data_anime_type_name>" year="<tmpl_var name=data_anime_year>">
			<status complete="<tmpl_var name=status_anime_iscomplete>" watched="<tmpl_var name=status_anime_iswatched>" hasawards="<tmpl_var name=status_anime_hasawards>" restricted="<tmpl_var name=status_anime_isrestricted>" />
			<neps user="<tmpl_var name=data_anime_my_eps>" seen="<tmpl_var name=data_anime_my_watchedeps>" cnt="<tmpl_var name=data_anime_eps>" />
			<seps user="<tmpl_var name=data_anime_my_eps_special>" seen="<tmpl_var name=data_anime_my_watchedeps_special>" cnt="<tmpl_var name=data_anime_eps_special>" /> 
			<titles><tmpl_loop name=loop_anime_titles>
				<title type="<tmpl_var name=data_anime_title_type_string>" lang="<tmpl_var name=data_anime_title_langsname>"><![CDATA[<tmpl_var name=data_anime_title_name>]]></title></tmpl_loop>
			</titles><tmpl_if expr="data_anime_votes">
			<tags><tmpl_loop name=loop_anime_tag>
				<tag id="<tmpl_var name=data_anime_tag_id>" date="<tmpl_var name=data_anime_tag_date>"><tmpl_var name=data_anime_tag_name></tag></tmpl_loop>
			</tags>
			<state><tmpl_var name=data_anime_mystate_string></state><tmpl_if expr="data_anime_my_vote || data_anime_my_tmpvote">
			<myvote type="<tmpl_if expr="data_anime_my_vote">normal</tmpl_if><tmpl_if expr="data_anime_my_tmpvote">temp</tmpl_if>" date="<tmpl_if expr="data_anime_my_vote"><tmpl_var name=data_anime_my_vote_date_short></tmpl_if><tmpl_if expr="data_anime_my_tmpvote"><tmpl_var name=data_anime_my_tmpvote_date_short></tmpl_if>" vote="<tmpl_if expr="data_anime_my_vote"><tmpl_var name=data_anime_my_vote></tmpl_if><tmpl_if expr="data_anime_my_tmpvote"><tmpl_var name=data_anime_my_tmpvote></tmpl_if>" /></tmpl_if><tmpl_if expr="data_anime_wishlist_priority">
			<wishlist type="<tmpl_var name=data_anime_wishlist_type_name>" pri="<tmpl_var name=data_anime_wishlist_priority_name >"><![CDATA[<tmpl_var name=data_anime_wishlist_comment>]]></wishlist></tmpl_if>
			<size longn="<tmpl_var name=data_anime_my_size>" shortn="<tmpl_var name=data_anime_my_size_h>" />
			<rating votes="<tmpl_var name=data_anime_votes>" rating="<tmpl_var name=data_anime_rating>" /><tmpl_else><trating votes="<tmpl_var name=data_anime_tmpvotes>" rating="<tmpl_var name=data_anime_tmprating>" /></tmpl_if><tmpl_if expr="data_anime_reviews">
			<reviews cnt="<tmpl_var name=data_anime_reviews>" rating="<tmpl_var name=data_anime_reviewrating>" /></tmpl_if>
			<dates added="<tmpl_var name=data_anime_date_short>" update="<tmpl_var name=data_anime_update_short>" start="<tmpl_var name=data_anime_startdate>" end="<tmpl_var name=data_anime_enddate>" />
			<info>
				<genres><tmpl_loop name=loop_anime_genre>
					<genre id="<tmpl_var name=data_anime_genre_id>"><tmpl_var name=data_anime_genre_name></genre></tmpl_loop>
				</genres>
				<cats><tmpl_loop name=loop_anime_genren>
					<cat id="<tmpl_var name=data_anime_genren_id>" pid="<tmpl_var name=data_anime_genren_parentid>" restricted="<tmpl_var name=data_anime_genren_ishentai>"><tmpl_var name=data_anime_genren_name></cat></tmpl_loop>
				</cats>
				<companys><tmpl_loop name=loop_anime_company>
					<company id="<tmpl_var name=data_anime_company_id>" type="<tmpl_var name=data_anime_company_type_name>" atype="<tmpl_var name=data_anime_company_atype_name>"><tmpl_if expr="data_anime_company_name">
						<name><![CDATA[<tmpl_var name=data_anime_company_name>]]></name></tmpl_if><tmpl_if expr="data_anime_company_shortname">
						<shortname><![CDATA[<tmpl_var name=data_anime_company_shortname>]]></shortname></tmpl_if><tmpl_if expr="data_anime_company_othername">
						<othername><![CDATA[<tmpl_var name=data_anime_company_othername>]]></othername></tmpl_if><tmpl_if expr="data_anime_company_url">
						<url><tmpl_var name=data_anime_company_url></url></tmpl_if><tmpl_if expr="data_anime_company_picurl">
						<picurl><tmpl_var name=data_anime_company_picurl></picurl></tmpl_if>
					</company></tmpl_loop>
				</companys>
				<resources><tmpl_if expr="data_anime_url ne ''">
					<url><tmpl_var name=data_anime_url></url></tmpl_if><tmpl_if expr="status_anime_ann_listed">
					<ann id="<tmpl_var name=data_anime_annid>"><tmpl_var name=data_anime_annurl></ann></tmpl_if><tmpl_if expr="data_anime_animenfourl ne ''">
					<animenfo id="<tmpl_var name=data_anime_animenfourl_name>"><tmpl_var name=data_anime_animenfourl></animenfo></tmpl_if><tmpl_if expr="status_anime_allcinema_listed">
					<allcinema id="<tmpl_var name=data_anime_allcinemaid>"><tmpl_var name=data_anime_allcinemaurl></allcinema></tmpl_if><tmpl_if expr="status_anime_animeplanet_listed">
					<animeplanet id="<tmpl_var name=data_anime_animeplanetid>"><tmpl_var name=data_anime_animeplaneturl></animeplanet></tmpl_if>
				</resources>
				<awards><tmpl_loop name=loop_anime_awards>
					<award id="<tmpl_var name=data_anime_award_id>" type="<tmpl_var name=data_anime_award_type>" url="<tmpl_var name=data_anime_award_url>"><tmpl_var name=data_anime_award_name></award></tmpl_loop>
				</awards>
				<awards_type><tmpl_loop name=loop_anime_award_types>
					<award_type id="<tmpl_var name=data_anime_award_type_id>"><tmpl_var name=data_anime_award_type_name></award_type></tmpl_loop>
				</awards_type>
				<desc><![CDATA[<tmpl_var expr="jsencode (data_anime_other)">]]></desc>
			</info>
			<groups><tmpl_loop name=loop_anime_groups>
				<group id="<tmpl_var name=data_anime_group_id>" agid="<tmpl_var name=data_anime_group_agid>">
					<name><tmpl_var name=data_anime_group_name></name>
					<sname><tmpl_var name=data_anime_group_sname></sname>
					<state><tmpl_var name=data_anime_group_state></state>
					<lastep><tmpl_var name=data_anime_group_lastep></lastep>
					<lastup><tmpl_var name=data_anime_group_lastup></lastup><tmpl_if name=data_anime_group_votes>
					<rating cnt="<tmpl_var name=data_anime_group_votes>"><tmpl_var name=data_anime_group_rating></rating></tmpl_if><tmpl_if name=data_anime_group_cmts>
					<cmts cnt="<tmpl_var name=data_anime_group_cmts>" /></tmpl_if><tmpl_if expr="data_anime_group_myvote ne '-'">
					<myvote vote="<tmpl_var name=data_anime_group_myvote>" date="<tmpl_var name=data_anime_group_myvote_date>"></myvote></tmpl_if>
					<epcnt><tmpl_var name=data_anime_group_epcnt></epcnt>
					<sepcnt><tmpl_var name=data_anime_group_sepcnt></sepcnt>
				</group></tmpl_loop>
			</groups>
			<eps><tmpl_loop name=loop_ep>
				<ep id="<tmpl_var name=data_ep_id>"><tmpl_if name=data_ep_state>
					<flags><tmpl_var name=data_ep_state></flags></tmpl_if>
					<state watched="<tmpl_var name=status_ep_iswatched>"><tmpl_var name=data_ep_mystate_string></state>
					<epno><tmpl_var name=data_ep_epno></epno>
					<len><tmpl_var name=data_ep_length></len>
					<date rel="<tmpl_var name=data_ep_aired_short>"><tmpl_var name=data_ep_date></date><tmpl_if expr="data_ep_myvote ne '-'">
					<myvote vote="<tmpl_var name=data_ep_myvote>" date="<tmpl_var name=data_ep_myvote_date>"></myvote></tmpl_if>
					<titles><tmpl_loop name=data_ep_titles>
						<title lang="<tmpl_var name=data_ep_title_langsname>"><![CDATA[<tmpl_var name=data_ep_title_name>]]></title></tmpl_loop>
					</titles>
					<rating votes="<tmpl_var name=data_ep_votes>"><tmpl_var name=data_ep_rating></rating>
				</ep></tmpl_loop>
			</eps>
		</anime></tmpl_loop>
	</animes>
</root>