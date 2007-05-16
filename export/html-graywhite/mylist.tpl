<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>::AniDB.Net:: Export for <tmpl_var name=global_user></title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link href="style.css" rel="stylesheet" type="text/css">
</head>

<body>
<table  border="0" cellspacing="0" cellpadding="5">
  <tr> 
    <td width="234" valign="top"><table width="250" border="0" cellspacing="0" cellpadding="2">
        <tr> 
          <td colspan="2" class="border_header">General Infos</td>
        </tr>
        <tr> 
          <td class="border_left">User</td>
          <td class="border_right"><tmpl_var name=global_user></td>
        </tr>
        <tr > 
          <td class="border_left">Created</td>
          <td class="border_right"><tmpl_var name=global_date></td>
        </tr>
        <tr> 
          <td class="border_left">Animes in list</td>
          <td class="border_right"><tmpl_var name=global_animecount></td>
        </tr>
        <tr> 
          <td class="border_left">Eps in list</td>
          <td class="border_right"><tmpl_var name=global_epcount></td>
        </tr>
        <tr> 
          <td class="border_left">Files in list:</td>
          <td class="border_right"><tmpl_var name=global_filecount></td>
        </tr>
        <tr> 
          <td class="border_left">Size:</td>
          <td class="border_right"><tmpl_var name=global_bytecount_h></td>
        </tr>
      </table></td>
    <td width="350"><img src="images/nothing.gif" width="1" height="1" alt="nothing"></td>
  </tr>
  <tr> 
    <td colspan="2">
			<table width="100%" border="0" cellspacing="0" cellpadding="2">
        <tr> 
          <td class="border_header_left">Name</td>
          <td class="border_header_middle">Year</td>
          <td class="border_header_middle">Eps</td>
          <td class="border_header_middle">Seen</td>
          <td class="border_header_middle">Rating</td>					
          <td class="border_header_right">Size</td>
        </tr>
				<tmpl_loop name=loop_anime>
        <tr> 
          <td class="border_left"><a href="anime/a<tmpl_var name=data_anime_id>.htm"><tmpl_var name=data_anime_name></a></td>
          <td class="border_middle"><tmpl_if name=data_anime_year><tmpl_var name=data_anime_year><tmpl_else>-</tmpl_if></td>
          <td class="border_middle"><tmpl_var name=data_anime_my_eps>/<tmpl_var name=data_anime_eps></td>
          <td class="border_middle"><tmpl_unless name=status_anime_iswatched><font color="gray"></tmpl_unless><tmpl_var name=data_anime_my_watchedeps>/<tmpl_var name=data_anime_my_eps><tmpl_unless name=status_anime_iswatched></font></tmpl_unless></td>
          <td class="border_middle"><tmpl_var name=data_anime_rating></td>
          <td class="border_right"><tmpl_if name=data_anime_my_size_h><tmpl_var name=data_anime_my_size_h><tmpl_else>-</tmpl_if></td>
        </tr>
				</tmpl_loop>
      </table>
		</td>
  </tr>
</table>
</body>
</html>
