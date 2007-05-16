<tmpl_loop name=loop_anime>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
	<title>::AniDB.Net:: Export for <tmpl_var name=global_user> - <tmpl_var name=data_anime_name></title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link href="../style.css" rel="stylesheet" type="text/css">
</head>

<body>
<table  border="0" cellspacing="0" cellpadding="5">
  <tr valign="top"> 
    <td width="250">
			<table width="250" border="0" cellspacing="0" cellpadding="2">
        <tr valign="top"> 
          <td colspan="2" class="border_header">Overview</td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Title</td>
          <td class="border_right"><a href="<tmpl_var name=global_animedburl>?show=anime&aid=<tmpl_var name=data_anime_id>" target="_blank"><tmpl_var name=data_anime_name></a></td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Synonyms</td>
          <td class="border_right"><tmpl_if name=loop_anime_title_alias><tmpl_loop name=loop_anime_title_alias><tmpl_var name=data_anime_title_alias_name><br></tmpl_loop><tmpl_else>-</tmpl_if></td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Short</td>
          <td class="border_right"><tmpl_if name=loop_anime_title_short><tmpl_loop name=loop_anime_title_short><tmpl_var name=data_anime_title_short_name><br></tmpl_loop><tmpl_else>-</tmpl_if></td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Type</td>
          <td class="border_right"><tmpl_if name=data_anime_type_name><tmpl_var name=data_anime_type_name><tmpl_else>-</tmpl_if></td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Genres</td>
          <td class="border_right"><tmpl_if name=loop_anime_genre><tmpl_loop name=loop_anime_genre><tmpl_var name=data_anime_genre_name><br></tmpl_loop><tmpl_else>-</tmpl_if></td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Episodes</td>
          <td class="border_right"><tmpl_if name=data_anime_eps><tmpl_var name=data_anime_eps><tmpl_else>-</tmpl_if></td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Year</td>
          <td class="border_right"><tmpl_if name=data_anime_year><tmpl_var name=data_anime_year><tmpl_else>-</tmpl_if></td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Producer</td>
          <td class="border_right"><tmpl_if name=data_anime_producer><tmpl_var name=data_anime_producer><tmpl_else>-</tmpl_if></td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Site</td>
          <td class="border_right"><tmpl_if name=data_anime_url><a href="<tmpl_var name=data_anime_url>" target="_blank"><img src="../images/link.gif" alt="link" width="9" height="10" border="0" align="absmiddle"></a><tmpl_else>-</tmpl_if></td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Rating</td>
          <td class="border_right"><tmpl_var name=data_anime_rating> (<tmpl_var name=data_anime_votes> votes)</td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Reviews</td>
          <td class="border_right"><tmpl_var name=data_anime_reviewrating> (<tmpl_var name=data_anime_reviews> reviews)</td>
        </tr>
        <tr valign="top"> 
          <td class="border_left">Animenfo</td>
          <td class="border_right"><tmpl_if name=data_anime_animenfourl><a href="<tmpl_var name=data_anime_animenfourl>" target="_blank"><tmpl_var name=data_anime_animenfourl_name></a><tmpl_else>-</tmpl_if></td>
        </tr>
      </table>
		</td>
	  <td width="350">
			<table width="350" border="0" cellspacing="0" cellpadding="2">
      	<tr> 
        	<td class="border_header">Synopsis</td>
      	</tr>
      	<tr> 
        	<td class="border"><tmpl_if name=data_anime_other><tmpl_var name=data_anime_other><tmpl_else>-</tmpl_if></td>
	      </tr>
			</table>
		</td>
	</tr>
  <tr> 
    <td colspan="2">
			<table width="100%" border="0" cellspacing="0" cellpadding="2">
				<tr> 
          <td class="border_header_left">No</td>
          <td class="border_header_middle">Name</td>
          <td class="border_header_middle">Length</td>
          <td class="border_header_right">Status</td>
        </tr>
				<tmpl_loop name=loop_ep>
        <tr> 
          <td class="border_left_no_bottom_border"><a href="<tmpl_var name=global_animedburl>?show=ep&eid=<tmpl_var name=data_ep_id>" target="_blank"><tmpl_var name=data_ep_epno></a></td>
          <td class="border_middle_no_bottom_border"><tmpl_var name=data_ep_name></td>
          <td class="border_middle_no_bottom_border"><tmpl_var name=data_ep_length> min</td>
          <td class="border_right_no_bottom_border"><tmpl_if name=status_ep_iswatched>watched<tmpl_else>unwatched</tmpl_if></td>
        </tr>
				<tr>
					<td class="border_left_no_bottom_border">&nbsp;</td>
					<td class="border_right_no_bottom_border" colspan="4">
						<table width="100%" border="0" cellspacing="0" cellpadding="2">
        	    <tr>
                <td class="border_header_left_small" height="18">ID</td>
                <td class="border_header_middle_small" height="18">Size</td>
                <td class="border_header_middle_small" height="18">Language</td>
                <td class="border_header_middle_small" height="18">Subtitle</td>																
                <td class="border_header_middle_small" height="18">Group</td>
                <td class="border_header_right_small" height="18">ED2K</td>
              </tr>
							<tmpl_loop name=loop_file>
              <tr>
                <td class="border_left"><a href="<tmpl_var name=global_animedburl>?show=file&fid=<tmpl_var name=data_file_id>" target="_blank"><tmpl_var name=data_file_id></a></td>
                <td class="border_middle"><tmpl_var name=data_file_size_h></td>
								<td class="border_middle"><tmpl_var name=data_file_lang_name></td>
								<td class="border_middle"><tmpl_var name=data_file_sub_name></td>
                <td class="border_middle"><tmpl_if name=data_file_group_id><a href="<tmpl_var name=global_animedburl>?show=group&gid=<tmpl_var name=data_file_group_id>" target="_blank"><tmpl_var name=data_file_group_shortname></a><tmpl_else>-</tmpl_if></td>
                <td class="border_right"><tmpl_if name=data_file_ed2k_hash><a href="<tmpl_var name=data_file_ed2k_link>"><img src="../images/link.gif" alt="link" width="9" height="10" border="0" align="absmiddle"></a><tmpl_else>-</tmpl_if></td>
              </tr>
							</tmpl_loop>
            </table>
					</td>
				</tr>
				<tr>
					<td class="border" colspan="5" height="5"><img src="../images/nothing.gif" width="1" height="1" alt="---"></td>
				</tr>
				</tmpl_loop>
      </table>
		</td>
  </tr>
</table>
</body>
</html>
</tmpl_loop>
