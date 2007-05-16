<tmpl_loop name=loop_anime>
<html>
<head>
    <style type="text/css">
    * {font-family: verdana, arial, sans-serif; font-size: 13px;}
    h1 {font-family: "times new roman", serif; font-weight: bold; font-size: 30px;}
    h1 a {font-family: "times new roman", serif; font-weight: bold; font-size: 30px;}
    td {vertical-align: top;}
    th {text-align: left;}
    .onleft {text-align: right;}
    .bottompart td {font-size: 11px;}
    .leftpart .visible td.onleft {font-weight: bold;}

    *, hr 
             {color: #000000;}            /* The main font color.*/
    body, .bottompart .visible td.unvisible
	         {background-color: #c0c0c0;} /* The main background color.*/
    .visible td, .bottompart .visible .unvisible th, .bottompart .visible .unvisible td
	         {background-color: #d3d3d3;} /* The second background color. */	
    .leftpart .visible td.onleft, .visible th, .bottompart .visible td, a.back:hover
             {background-color: #f0c060;} /* The special background color. */
    .rightpart .visible td 
	         {border-top: solid #f0c060;} /* Put the same color as above. */
	td a:hover
	         {color: #d09000;}            /* The special font color. */
    </style>
    <title>::AniDB.Net:: Export for <tmpl_var name=global_user> - <tmpl_var name=data_anime_name></title>
</head>

<body>

<h1>Anime: <tmpl_var name=data_anime_name></h1>
<hr>

<table><tr><td class="leftpart">

	<table class="visible">
	<tr>
		<td class="onleft"> Title: </td>
		<td> <tmpl_var name=data_anime_name> </td>
	</tr>
	<tr>
		<td class="onleft"> Synonyms </td>
		<td>
	   <tmpl_loop name=loop_anime_title_alias>
	   <tmpl_var name=data_anime_title_alias_name>,
	   </tmpl_loop>
		</td>
	</tr>
	<tr>
		<td class="onleft"> Short&nbsp;Titles </td>
		<td>
	   <tmpl_loop name=loop_anime_title_short>
	   <tmpl_var name=data_anime_title_short_name>,
	   </tmpl_loop>
		</td>
	</tr>
	<tr>
		<td class="onleft"> Type: </td>
		<td> <tmpl_var name=data_anime_type_name> </td>
	</tr>
	<tr>
		<td class="onleft"> Genres: </td>
		<td>
	   <tmpl_loop name=loop_anime_genre>
	   <tmpl_var name=data_anime_genre_name>,
	   </tmpl_loop>
		</td>
	</tr>
	<tr>
		<td class="onleft"> Eps: </td>
		<td> <tmpl_var name=data_anime_eps> </td>
	</tr>
	<tr>
		<td class="onleft"> Year: </td>
		<td> <tmpl_var name=data_anime_year> </td>
	</tr>
	<tr>
		<td class="onleft"> Producer: </td>
		<td> <tmpl_var name=data_anime_producer> </td>
	</tr>
	<tr>
		<td class="onleft"> url: </td>
		<td> <a href="<tmpl_var name=data_anime_url>" target="_blank"><tmpl_var name=data_anime_url></a> </td>
	</tr>
	<tr>
		<td class="onleft"> Rating: </td>
		<td> <tmpl_var name=data_anime_rating> (<tmpl_var name=data_anime_votes> votes) </td>
	</tr>
	<tr>
		<td class="onleft"> Reviews: </td>
		<td> <tmpl_var name=data_anime_reviewrating> (<tmpl_var name=data_anime_reviews> reviews) </td>
	</tr>
	<tr>
		<td class="onleft"> AniDB: </td>
		<td> <a href="<tmpl_var name=global_animedburl>?show=anime&aid=<tmpl_var name=data_anime_id>" target="_blank">AniDB&nbsp;AID: <tmpl_var name=data_anime_id></a> </td>
	</tr>
	</table>

</td><td width="30"></td><td class="rightpart">

     <table class="visible"><tr><td>
	<tmpl_var name=data_anime_other>
    </td></tr></table>

</td></tr></table>

<hr>

<span class="bottompart">

<b>Episodes:</b><br>
<table class="visible">
<tr>
    <th> EpNo </th>
    <th> Name </th>
    <th> Length </th>
    <th> Aired </th>
    <th> Status </th>
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
    <td colspan=5 class="unvisible">
    <table>
    <tr>
        <th> ID </th>
        <th> Size </th>
        <th> Group </th>
        <th> Ed2k </th>
        <th> Storage </th>
        <th> Source </th>
        <th> Status </th>
    </tr>
    <tmpl_loop name=loop_file>
    <tr>
        <td> <a href="<tmpl_var name=global_animedburl>?show=file&fid=<tmpl_var name=data_file_id>" target="_blank"><tmpl_var name=data_file_id></a> </td>
   <td> <tmpl_var name=data_file_size></a> (<tmpl_var name=data_file_size_h>) </td>
        <td> <tmpl_if name=data_file_group_id><a href="<tmpl_var name=global_animedburl>?show=group&gid=<tmpl_var name=data_file_group_id>" target="_blank"><tmpl_var name=data_file_group_shortname></a><tmpl_else>-</tmpl_if> </td>
   <td> <tmpl_if name=data_file_ed2k_hash><a href="<tmpl_var name=data_file_ed2k_link>"><tmpl_var name=data_file_ed2k_name></a><tmpl_else>-</tmpl_if> </td>
        <td> <tmpl_var name=data_file_storage> </td>
        <td> <tmpl_var name=data_file_source> </td>
   <td> <tmpl_if name=status_file_iswatched>watched on <tmpl_var name=data_file_viewdate><tmpl_else>unwatched</tmpl_if> </td>
    </tr>
    </tmpl_loop>
    </table>
    </td>
</tr>
</tmpl_if>
</tmpl_loop>
</table>

</span>

<br>
<a href="../mylist.htm" class="back">Back</a>
</body>
</html>
</tmpl_loop>


</body>
</html>