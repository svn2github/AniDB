<?xml version="1.0" encoding="UTF-8"?>
<list>
	<created><tmpl_var name=global_date></created><tmpl_loop name=loop_anime>
	<anime>
		<title><![CDATA[<tmpl_var name=data_anime_name>]]></title><tmpl_if name=status_anime_my_isvoted>
		<vote><tmpl_var name=data_anime_my_vote></vote></tmpl_if><tmpl_if name=status_anime_my_istmpvoted>
		<vote><tmpl_var name=data_anime_my_tmpvote></vote></tmpl_if><tmpl_if name=data_anime_eps>
		<total><tmpl_var name=data_anime_eps></total></tmpl_if>
		<watched><tmpl_if expr="data_anime_eps && status_anime_iswatched"><tmpl_var name=data_anime_eps><tmpl_else><tmpl_var name=data_anime_my_watchedeps></tmpl_if></watched><tmpl_if name=data_anime_wishlist_type>
		<status><tmpl_var name=data_anime_wishlist_type_name></status></tmpl_if>
		<anidb><tmpl_var name=data_anime_id></anidb><tmpl_if name=status_anime_animeplanet_listed>
		<animeplanet><tmpl_var name=data_anime_animeplanetid></animeplanet></tmpl_if><tmpl_if name=status_anime_ann_listed>
		<ann><tmpl_var name=data_anime_annid></ann></tmpl_if>
	</anime></tmpl_loop>
</list>
