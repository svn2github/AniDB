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
		<date><tmpl_var name=global_date></date>
	</userinfo>
	</custom>
	<cats><tmpl_loop name=global_genren_loop>
		<cat id="<tmpl_var name=global_genren_id>" pid="<tmpl_var name=global_genren_parentid>" restricted="<tmpl_var name=global_genren_ishentai>"><tmpl_var name=global_genren_name></cat></tmpl_loop>
	</cats>
	<animes><tmpl_loop name=loop_anime>
		<anime id="<tmpl_var name=data_anime_id>" type="<tmpl_var name=data_anime_type_name>" year="<tmpl_var name=data_anime_year>">
			<status complete="<tmpl_var name=status_anime_iscomplete>" watched="<tmpl_var name=status_anime_iswatched>" hasawards="<tmpl_var name=status_anime_hasawards>" restricted="<tmpl_var name=status_anime_isrestricted>" />
			<neps user="<tmpl_var name=data_anime_my_eps>" seen="<tmpl_var name=data_anime_my_watchedeps>" cnt="<tmpl_var name=data_anime_eps>" />
			<seps user="<tmpl_var name=data_anime_my_eps_special>" seen="<tmpl_var name=data_anime_my_watchedeps_special>" cnt="<tmpl_var name=data_anime_eps_special>" /> 
			<titles><tmpl_loop name=loop_anime_titles>
				<title type="<tmpl_var name=data_anime_title_type_string>" lang="<tmpl_var name=data_anime_title_langsname>"><![CDATA[<tmpl_var name=data_anime_title_name>]]></title></tmpl_loop>
			</titles>
			<tags><tmpl_loop name=loop_anime_tag>
				<tag id="<tmpl_var name=data_anime_tag_id>" date="<tmpl_var name=data_anime_tag_date>"><tmpl_var name=data_anime_tag_name></tag></tmpl_loop>
			</tags>
			<state><tmpl_var name=data_anime_mystate_string></state><tmpl_if expr="data_anime_my_vote || data_anime_my_tmpvote">
			<myvote type="<tmpl_if expr="data_anime_my_vote">normal<tmpl_else>temp</tmpl_if>" date="<tmpl_if expr="data_anime_my_vote"><tmpl_var name=data_anime_my_vote_date_short><tmpl_else><tmpl_var name=data_anime_my_tmpvote_date_short></tmpl_if>" vote="<tmpl_if expr="data_anime_my_vote"><tmpl_var name=data_anime_my_vote><tmpl_else><tmpl_var name=data_anime_my_tmpvote></tmpl_if>" /></tmpl_if><tmpl_if expr="data_anime_wishlist_priority">
			<wishlist type="<tmpl_var name=data_anime_wishlist_type_name>" pri="<tmpl_var name=data_anime_wishlist_priority_name >"><![CDATA[<tmpl_var name=data_anime_wishlist_comment>]]></wishlist></tmpl_if>
			<size longn="<tmpl_var name=data_anime_my_size>" shortn="<tmpl_var name=data_anime_my_size_h>" /><tmpl_if expr="data_anime_votes">
			<rating type="normal" votes="<tmpl_var name=data_anime_votes>" rating="<tmpl_var name=data_anime_rating>" /><tmpl_else><rating type="temp" votes="<tmpl_var name=data_anime_tmpvotes>" rating="<tmpl_var name=data_anime_tmprating>" /></tmpl_if><tmpl_if expr="data_anime_reviews">
			<reviews cnt="<tmpl_var name=data_anime_reviews>" rating="<tmpl_var name=data_anime_reviewrating>" /></tmpl_if>
			<dates added="<tmpl_var name=data_anime_date_short>" update="<tmpl_var name=data_anime_update_short>" start="<tmpl_var name=data_anime_startdate>" end="<tmpl_var name=data_anime_enddate>" />
		</anime></tmpl_loop>
	</animes>
</root>