<html>
<head>
    <title>::AniDB.Net:: Export for <tmpl_var name=global_user></title>
</head>
<body link="#0000ff" alink="#ff0000" vlink="#0000ff">
<font face="arial,helvetica" size=2>
<h3>AniDB Export for <tmpl_var name=global_user></h3>
<hr>
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
</table>
<hr>
<b>Complete Anime Series:</b><br>
<table>
<tr>
    <td> Name </td>
    <td> Year </td>
    <td> Eps </td>
    <td> Seen </td>
    <td> Size </td>
<tr>
<tmpl_loop name=loop_anime>
<tmpl_if name=status_anime_iscomplete>
<tr>
    <td> <a href="anime/a<tmpl_var name=data_anime_id>.htm"><tmpl_var name=data_anime_name></a> </td>
    <td> <tmpl_var name=data_anime_year> </td>
    <td> <tmpl_var name=data_anime_my_eps>/<tmpl_var name=data_anime_eps> </td>
    <td> <tmpl_if name=status_anime_iswatched><b><tmpl_else><i></tmpl_if><tmpl_var name=data_anime_my_watchedeps>/<tmpl_var name=data_anime_my_eps><tmpl_if name=status_anime_iswatched></b><tmpl_else></i></tmpl_if> </td>
    <td> <tmpl_var name=data_anime_my_size_h> </td>    
</tr>
</tmpl_if>
</tmpl_loop>
</table>
<hr>
<b>Incomplete Anime Series:</b><br>
<table>
<tr>
    <td> Name </td>
    <td> Year </td>
    <td> Eps </td>
    <td> Seen </td>
    <td> Size </td>
<tr>
<tmpl_loop name=loop_anime>
<tmpl_unless name=status_anime_iscomplete>
<tr>
    <td> <a href="anime/a<tmpl_var name=data_anime_id>.htm"><tmpl_var name=data_anime_name></a> </td>
    <td> <tmpl_var name=data_anime_year> </td>
    <td> <tmpl_var name=data_anime_my_eps>/<tmpl_var name=data_anime_eps> </td>
    <td> <tmpl_if name=status_anime_iswatched><b><tmpl_else><i></tmpl_if><tmpl_var name=data_anime_my_watchedeps>/<tmpl_var name=data_anime_my_eps><tmpl_if name=status_anime_iswatched></b><tmpl_else></i></tmpl_if> </td>
    <td> <tmpl_var name=data_anime_my_size_h> </td>    
</tr>
</tmpl_unless>
</tmpl_loop>
</table>
