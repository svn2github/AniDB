<html>
<head>
    <style type="text/css">
    body {background-color: #c0c0c0;}
	a {color: #000000;}
	* {font-family: verdana, arial, sans-serif; font-size: 13px;}
	h1 {font-family: serif; font-weight: bold; font-size: 30px;}
	h1 a {font-family: serif; font-weight: bold; font-size: 30px;}
	b {font-family: "times new roman", serif; font-size: 17px;}
	th {font-weight: normal}
	td {vertical-align: top;}
	.visible td {background-color: #d3d3d3;}
	.visible th {background-color: #a9a9a9;}
	.onleft {text-align: right;}
	.gray {color: #606060;}
    </style>
    <title>::AniDB.Net:: Export for <tmpl_var name=global_user></title>
</head>


<body>
<h1> <a href="<tmpl_var name=global_animedburl>">AniDB</a> Export for <tmpl_var name=global_user></h1>
<hr>


<table><tr><td>
<!-- Left part -->
	<table class="visible">
	<tr>
		<td class="onleft"> User: </td>
		<td> <tmpl_var name=global_user> </td>
	</tr>
	<tr>
		<td class="onleft"> Created: </td>
		<td> <tmpl_var name=global_date> </td>
	</tr>
	<tr>
		<td class="onleft"> Animes in list: </td>
		<td> <tmpl_var name=global_animecount> </td>
	</tr>
	<tr>
		<td class="onleft"> Eps in list: </td>
		<td> <tmpl_var name=global_epcount> </td>
	</tr>
	<tr>
		<td class="onleft"> Files in list: </td>
		<td> <tmpl_var name=global_filecount> </td>
	</tr>
	<tr>
		<td class="onleft"> Size: </td>
		<td> <tmpl_var name=global_bytecount_h> </td>
	</tr>
	</table>

</td><td width="30"></td><td>
<!-- Right part -->
	<b>Complete Anime Series:</b><br>
	<table class="visible">
	<tr>
		<th class="onleft"> Name </th>
		<th> Year </th>
		<th> Eps </th>
		<th> Seen </th>
		<th> Size </th>
	<tr>
	<tmpl_loop name=loop_anime>
	<tmpl_if name=status_anime_iscomplete>
	<tr>
		<td class="onleft"> <a href="anime/a<tmpl_var name=data_anime_id>.htm"><tmpl_var name=data_anime_name></a> </td>
		<td> <tmpl_var name=data_anime_year> </td>
		<td> <tmpl_var name=data_anime_my_eps>/<tmpl_var name=data_anime_eps> </td>
		<td> <tmpl_unless name=status_anime_iswatched><span class="gray"></tmpl_unless><tmpl_var name=data_anime_my_watchedeps>/<tmpl_var name=data_anime_my_eps><tmpl_unless name=status_anime_iswatched></span></tmpl_unless> </td>
		<td> <tmpl_var name=data_anime_my_size_h> </td>   
	</tr>
	</tmpl_if>
	</tmpl_loop>
	</table>

	<hr>

	<b>Incomplete Anime Series:</b><br>
	<table class="visible">
	<tr>
		<th class="onleft"> Name </th>
		<th> Year </th>
		<th> Eps </th>
		<th> Seen </th>
		<th> Size </th>
	<tr>
	<tmpl_loop name=loop_anime>
	<tmpl_unless name=status_anime_iscomplete>
	<tr>
		<td class="onleft"> <a href="anime/a<tmpl_var name=data_anime_id>.htm"><tmpl_var name=data_anime_name></a> </td>
		<td> <tmpl_var name=data_anime_year> </td>
		<td> <tmpl_var name=data_anime_my_eps>/<tmpl_var name=data_anime_eps> </td>
		<td> <tmpl_unless name=status_anime_iswatched><span class="gray"></tmpl_unless><tmpl_var name=data_anime_my_watchedeps>/<tmpl_var name=data_anime_my_eps><tmpl_unless name=status_anime_iswatched></span></tmpl_unless> </td>
		<td> <tmpl_var name=data_anime_my_size_h> </td>   
	</tr>
	</tmpl_unless>
	</tmpl_loop>
	</table>

</td></tr></table>
</body>
</html>