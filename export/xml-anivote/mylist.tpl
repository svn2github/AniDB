<Mylist	date="<tmpl_var name=global_date>">
<tmpl_loop name=loop_anime>
	<Anime id="<tmpl_var name=data_anime_id>" complete="<tmpl_var name=status_anime_iscomplete>" watched="<tmpl_var name=status_anime_iswatched>">
		<Name><![CDATA[<tmpl_var name=data_anime_name>]]></Name>
		<tmpl_if name=status_anime_my_isvoted>
      <MLPermVote date="<tmpl_var name=data_anime_my_vote_date>"><![CDATA[<tmpl_var name=data_anime_my_vote>]]></MLPermVote>
    </tmpl_if>
		<tmpl_if name=status_anime_my_istmpvoted>
      <MLTempVote date="<tmpl_var name=data_anime_my_tmpvote_date>"><![CDATA[<tmpl_var name=data_anime_my_tmpvote>]]></MLTempVote>
    </tmpl_if>
	</Anime>
</tmpl_loop>
</Mylist>
