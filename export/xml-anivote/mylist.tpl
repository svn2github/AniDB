<Mylist	date="<tmpl_var name=global_date>"><tmpl_loop name=loop_anime>
  <Anime id="<tmpl_var name=data_anime_id>" complete="<tmpl_var name=status_anime_iscomplete>" watched="<tmpl_var name=status_anime_iswatched>" isRestricted="<tmpl_var name=status_anime_isrestricted>" lastUpdated="<tmpl_var name=data_anime_update>">
    <Name><![CDATA[<tmpl_var name=data_anime_name>]]></Name><tmpl_if name=status_anime_my_isvoted>
    <UserPermVote date="<tmpl_var name=data_anime_my_vote_date>"><tmpl_var name=data_anime_my_vote></UserPermVote></tmpl_if><tmpl_if name=status_anime_my_istmpvoted>
    <UserTempVote date="<tmpl_var name=data_anime_my_tmpvote_date>"><tmpl_var name=data_anime_my_tmpvote></UserTempVote></tmpl_if>
    <PermRating date="<tmpl_var name=data_anime_votes>"><tmpl_var name=data_anime_rating></PermRating>
    <TempRating count="<tmpl_var name=data_anime_tmprating>"><tmpl_var name=data_anime_tmpvotes></TempRating>
    <Categories><tmpl_loop name=loop_anime_genren>
      <Category id="<tmpl_var name=data_anime_genren_id>" weight="<tmpl_var name=data_anime_genren_weight>" ><![CDATA[<tmpl_var name=data_anime_genren_name>]]></Category>
    </tmpl_loop></Categories>
  </Anime>
</tmpl_loop></Mylist>
