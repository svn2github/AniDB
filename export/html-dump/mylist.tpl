<html> 
<head> 
    <title>::AniDB.Net:: Export for <tmpl_var name=global_user> @ <tmpl_var name=global_date></title> 
</head> 
<body link="#0000ff" alink="#ff0000" vlink="#0000ff"> 
<font face="arial,helvetica" size=2> 
<h3>AniDB Export for <tmpl_var name=global_user></h3> 
<hr/> 
<table> 
<tr> 
    <td> User: </td> 
    <td> <tmpl_var name=global_user> </td> 
</tr> 
<tr> 
    <td> Created: </td> 
    <td> <tmpl_var name=global_date> </td> 
</tr> 
<tr> 
    <td> Animes in list: </td> 
    <td> <tmpl_var name=global_animecount> </td> 
</tr> 
<tr> 
    <td> Eps in list: </td> 
    <td> <tmpl_var name=global_epcount> </td> 
</tr> 
<tr> 
    <td> Files in list: </td> 
    <td> <tmpl_var name=global_filecount> </td> 
</tr> 
<tr> 
    <td> Size: </td> 
    <td> <tmpl_var name=global_bytecount> (<tmpl_var name=global_bytecount_h>) </td> 
</tr> 
<tr> 
    <td> AniDB URL: </td> 
    <td> <a href="<tmpl_var name=global_animedburl>" target="_blank"><tmpl_var name=global_animedburl></a> </td> 
</tr> 
</table> 
<hr/>
<b>Complete Anime Series:</b><br/> 
<tmpl_loop name=loop_anime><a href="#<tmpl_var name=data_anime_name>"><tmpl_var name=data_anime_name></a><br/></tmpl_loop> 
<br/><br/>
<tmpl_loop name=loop_anime>
  <b><a name="<tmpl_var name=data_anime_name>"><tmpl_var name=data_anime_name>:</b><br/>
  <tmpl_loop name=loop_ep><tmpl_loop name=loop_file>
    <a href="<tmpl_var name=data_file_ed2k_link>"><tmpl_var name=data_file_ed2k_name></a><br/>
  </tmpl_loop></tmpl_loop>
  <br/><br/>
</tmpl_loop> 
</font>
</body>
</html>
