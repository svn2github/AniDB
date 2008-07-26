<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="mylista.xsl"?>
<AC u="<tmpl_var name=global_user>" t="<tmpl_var name=global_date>" n="<tmpl_var name=global_animecount>" e="<tmpl_var name=global_epcount>">
<tmpl_loop name=loop_anime><A i="<tmpl_var name=data_anime_id>" w="<tmpl_var name=status_anime_iswatched>" c="<tmpl_var name=status_anime_iscomplete>" y="<tmpl_var name=data_anime_year>" r="<tmpl_var name=data_anime_rating>" v="<tmpl_var name=data_anime_votes>" rt="<tmpl_var name=data_anime_tmprating>" vt="<tmpl_var name=data_anime_tmpvotes>" rr="<tmpl_var name=data_anime_reviewrating>" vr="<tmpl_var name=data_anime_reviews>" e="<tmpl_var name=data_anime_my_eps>" es="<tmpl_var name=data_anime_my_eps_special>" we="<tmpl_var name=data_anime_my_watchedeps>" ws="<tmpl_var name=data_anime_my_watchedeps_special>" d="<tmpl_var name=data_ep_aired_short>">
	<N><![CDATA[<tmpl_var name=data_anime_name>]]></N>
	<tmpl_loop name=loop_anime_title_official><NO i="<tmpl_var name=data_anime_title_official_langid>" n="<tmpl_var name=data_anime_title_official_langname>">
		<N><![CDATA[<tmpl_var name=data_anime_title_official_name>]]></N>
	</NO></tmpl_loop>
	<T><tmpl_var name=data_anime_type_name></T>
	<tmpl_loop name=loop_anime_awards><W i="<tmpl_var name=data_anime_award_id>" t="<tmpl_var name=data_anime_award_type>" n="<tmpl_var name=data_anime_award_name>"/>
	</tmpl_loop><tmpl_loop name=loop_ep><E i="<tmpl_var name=data_ep_epno>" l="<tmpl_var name=data_ep_length>" s="<tmpl_var name=data_ep_state>" w="<tmpl_var name=status_ep_iswatched>">
		<N><![CDATA[<tmpl_var name=data_ep_name>]]></N>
	</E></tmpl_loop>
</A></tmpl_loop>
</AC>