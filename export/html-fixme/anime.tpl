<tmpl_loop name=loop_anime>
<html>
<head>
    <title>::AniDB.Net:: Export for <tmpl_var name=global_user> - <tmpl_var name=data_anime_name></title>
</head>
<body link="#0000ff" alink="#ff0000" vlink="#0000ff">
<font face="arial,helvetica" size=2>
<h3>Anime: <tmpl_var name=data_anime_name></h3>
<hr>
<table>
<tr>
    <td> Title: </td>
    <td> <tmpl_var name=data_anime_name> </td>
</tr>
<tr>
    <td> Synonyms </td>
    <td> 
	<tmpl_loop name=loop_anime_title_alias>
	<tmpl_var name=data_anime_title_alias_name>, 
	</tmpl_loop>
    </td>
</tr>
<tr>
    <td> Short Titles </td>
    <td> 
	<tmpl_loop name=loop_anime_title_short>
	<tmpl_var name=data_anime_title_short_name>, 
	</tmpl_loop>
    </td>
</tr>
<tr>
    <td> Type: </td>
    <td> <tmpl_var name=data_anime_type_name> </td>
</tr>
<tr>
    <td> Genres: </td>
    <td> 
	<tmpl_loop name=loop_anime_genre>
	<tmpl_var name=data_anime_genre_name>, 
	</tmpl_loop>
    </td>
</tr>
<tr>
    <td> Eps: </td>
    <td> <tmpl_var name=data_anime_eps> </td>
</tr>
<tr>
    <td> Year: </td>
    <td> <tmpl_var name=data_anime_year> </td>
</tr>
<tr>
    <td> Producer: </td>
    <td> <tmpl_var name=data_anime_producer> </td>
</tr>
<tr>
    <td> url: </td>
    <td> <a href="<tmpl_var name=data_anime_url>" target="_blank"><tmpl_var name=data_anime_url></a> </td>
</tr>
<tr>
    <td> Rating: </td>
    <td> <tmpl_var name=data_anime_rating> (<tmpl_var name=data_anime_votes> votes) </td>
</tr>
<tr>
    <td> Reviews: </td>
    <td> <tmpl_var name=data_anime_reviewrating> (<tmpl_var name=data_anime_reviews> reviews) </td>
</tr>
<tr>
    <td> AniDB: </td>
    <td> <a href="<tmpl_var name=global_animedburl>?show=anime&aid=<tmpl_var name=data_anime_id>" target="_blank">AniDB AID: <tmpl_var name=data_anime_id></a> </td>
</tr>
<tr>
    <td> AnimeNfo: </td>
    <td> <a href="<tmpl_var name=data_anime_animenfourl>" target="_blank"><tmpl_var name=data_anime_animenfourl_name></a> <tmpl_var name=data_anime_animenfoid> <tmpl_var name=data_anime_animenfoid2></td>
</tr>
</table>
<hr>
<tmpl_var name=data_anime_other>
<hr>
<b>Episodes:</b><br>
<table>
<tr>
    <td> EpNo </td>
    <td> Name </td>
    <td> Length </td>
    <td> Aired </td>
    <td> Status </td>
<tr>
<tmpl_loop name=loop_ep>
<tr>
    <td> <a href="<tmpl_var name=global_animedburl>?show=ep&eid=<tmpl_var name=data_ep_id>" target="_blank"><tmpl_var name=data_ep_epno></a> </td>
    <td> <tmpl_var name=data_ep_name> </td>
    <td> <tmpl_var name=data_ep_length> </td>
    <td> <tmpl_var name=data_ep_aired> </td>
    <td> <tmpl_if name=status_ep_iswatched>watched<tmpl_else>unwatched</tmpl_if> </td>
</tr>
<tmpl_if name=status_ep_hasfile>
<tr>
    <td colspan=5>
    <table border=0>
    <tr>
        <td> ID </td>
        <td> Size </td>
        <td> Group </td>
	<td> Ed2k </td>
	<td> Storage </td>
	<td> Source </td>
	<td> Status </td>
    </tr>
    <tmpl_loop name=loop_file>
    <tr>
        <td> <a href="<tmpl_var name=global_animedburl>?show=file&fid=<tmpl_var name=data_file_id>" target="_blank"><tmpl_var name=data_file_id></a> </td>
	<td> <tmpl_var name=data_file_size_plain></a> (<tmpl_var name=data_file_size_h>) </td>
        <td> <tmpl_if name=data_file_group_id><a href="<tmpl_var name=global_animedburl>?show=group&gid=<tmpl_var name=data_file_group_id>" target="_blank"><tmpl_var name=data_file_group_shortname></a><tmpl_else>-</tmpl_if> </td>
	<td> <tmpl_if name=data_file_ed2k_hash><a href="<tmpl_var name=data_file_ed2k_link>"><tmpl_var name=data_file_ed2k_name></a><tmpl_else>-</tmpl_if> </td>
        <td> <tmpl_var name=data_file_storage> </td>
        <td> <tmpl_var name=data_file_type_name> <tmpl_var name=data_file_type_id> <tmpl_var name=data_file_acodec_name> <tmpl_var name=data_file_vcodec_name></td>
	<td> <tmpl_if name=status_file_iswatched>watched on <tmpl_var name=data_file_viewdate><tmpl_else>unwatched</tmpl_if> </td>
    </tr>
    </tmpl_loop>
    </table>
    </td>
</tr>
</tmpl_if>
</tmpl_loop>
</table>
<br>
<a href="../mylist.htm">Back</a>
</font>
</body>
</html>
</tmpl_loop>
