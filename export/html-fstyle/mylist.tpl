<?xml version="1.0" encoding="UTF-8"?>
<root anidb="<tmpl_var name=global_animedburl>">
  <custom uid="<tmpl_var name=global_user_id>">
    <userinfo>
      <name><tmpl_var name=global_user></name>
      <animes added="<tmpl_var name=global_userstat_animeadded>" user="<tmpl_var name=global_animecount>" />
      <eps added="<tmpl_var name=global_userstat_epsadded>" dbviewedp="<tmpl_var name=global_userstat_totalviewedp>" dbownedp="<tmpl_var name=global_userstat_totalownedp>" viewedp="<tmpl_var name=global_userstat_ownviewedp>" viewed="<tmpl_var name=global_userstat_viewedepcnt>" user="<tmpl_var name=global_epcount>" />
      <files added="<tmpl_var name=global_userstat_filesadded>" lamecnt="<tmpl_var name=global_userstat_lamefiles>" user="<tmpl_var name=global_filecount>" />
      <groups added="<tmpl_var name=global_userstat_groupsadded>" />
      <size byte="<tmpl_var name=global_bytecount>" short="<tmpl_var name=global_bytecount_h>" />
      <stats votes="<tmpl_var name=global_userstat_reviews>" reviews="<tmpl_var name=global_userstat_votes>" />
    </userinfo>
  </custom>
	<animes><tmpl_loop name=loop_anime>
		<anime id="<tmpl_var name=data_anime_id>" type="<tmpl_var name=data_anime_type_name>" year="<tmpl_var name=data_anime_year>">
      <status complete="<tmpl_var name=status_anime_iscomplete>" watched="<tmpl_var name=status_anime_iswatched>" hasawards="<tmpl_var name=status_anime_hasawards>" />
		  <neps cnt="<tmpl_var expr="data_anime_eps_total - data_anime_eps_special">" />
		  <seps cnt="<tmpl_var name=data_anime_eps_special>" /> 
			<titles><tmpl_loop name=loop_anime_titles>
        <title type="<tmpl_var name=data_anime_title_type_string>" lang="<tmpl_var name=data_anime_title_langsname>"><![CDATA[<tmpl_var name=data_anime_title_name>]]></title></tmpl_loop>
      </titles><tmpl_if expr="data_anime_votes">
      <state><tmpl_var name=data_anime_mystate_string></state><tmpl_if expr="data_anime_my_vote || data_anime_my_tmpvote">
      <myvotes>
        <tmpl_if expr="data_anime_my_vote"><normal date="<tmpl_var name=data_anime_my_vote_date_short>" vote="<tmpl_var name=data_anime_my_vote>" /></tmpl_if>
        <tmpl_if expr="data_anime_my_tmpvote"><temp date="<tmpl_var name=data_anime_my_tmpvote_date_short>" vote="<tmpl_var name=data_anime_my_tmpvote>" /></tmpl_if>
      </myvotes></tmpl_if><tmpl_if expr="data_anime_wishlist_priority">
      <wishlist type="<tmpl_var name=data_anime_wishlist_type_name>">
        <pri id="<tmpl_var name=data_anime_wishlist_priority_name >" /> 
        <comment><![CDATA[<tmpl_var name=data_anime_wishlist_comment>]]></comment>
      </wishlist></tmpl_if>
      <size long="<tmpl_var name=data_anime_my_size>" short="<tmpl_var name=data_anime_my_size_h>" />
			<rating votes="<tmpl_var name=data_anime_votes>" rating="<tmpl_var name=data_anime_rating>" /><tmpl_else><trating votes="<tmpl_var name=data_anime_tmpvotes>" rating="<tmpl_var name=data_anime_tmprating>" /></tmpl_if><tmpl_if expr="data_anime_reviews">
      <reviews cnt="<tmpl_var name=data_anime_reviews>" rating="<tmpl_var name=data_anime_reviewrating>" /></tmpl_if>
			<dates added="<tmpl_var name=data_anime_date_short>" update="<tmpl_var name=data_anime_update_short>" start="<tmpl_var name=data_anime_startdate_short>" end="<tmpl_var name=data_anime_enddate_short>" />
		</anime></tmpl_loop>
  </animes>
</root>