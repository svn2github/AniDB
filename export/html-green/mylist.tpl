<html>
<head>
    <style type="text/css">
    * {font-family: verdana, arial, sans-serif; font-size: 13px;}
    h1 {font-family: "times new roman", serif; font-weight: bold; font-size: 30px;}
    h1 a {font-family: "times new roman", serif; font-weight: bold; font-size: 30px;}
    h4 {font-family: "times new roman", serif; font-size: 17px; margin: 0;}
    td {vertical-align: top;}
    .onleft {text-align: right;}
	.leftpart .visible td.onleft {font-weight: bold;}

    *, hr
	         {color: #000000;}            /* The main font color. */
    body
	         {background-color: #79b379;} /* The main background color. */
    .visible td 
	         {background-color: #90c48a;} /* The second background color. */
	.leftpart .visible td.onleft, .visible th 
	         {background-color: #00AE00;} /* The special background color. */
    td a:hover
	         {color: #008474;}            /* The special font color. */
    .gray    
	         {color: #807A37;}            /* The very visible font color. */ 
    </style>
    <title>::AniDB.Net:: Export for <tmpl_var name=global_user></title>
</head>


<body>
<h1> <a href="<tmpl_var name=global_animedburl>">AniDB</a> Export for <tmpl_var name=global_user></h1>
<hr>


<table><tr><td class="leftpart">

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

</td><td width="30"></td><td class="rightpart">

	<table class="visible">
	<tr>
		<th class="onleft"> Name </th>
		<th> Year </th>
		<th> Eps </th>
		<th> Seen </th>
		<th> Size </th>
	<tr>
	<tmpl_loop name=loop_anime>
	<tr>
		<td class="onleft"> <a href="anime/a<tmpl_var name=data_anime_id>.htm"><tmpl_var name=data_anime_name></a> </td>
		<td> <tmpl_var name=data_anime_year> </td>
		<td> <tmpl_unless name=status_anime_iscomplete><span class="gray"></tmpl_unless><tmpl_var name=data_anime_my_eps>/<tmpl_var name=data_anime_eps><tmpl_unless name=status_anime_iscomplete></span></tmpl_unless> </td>
		<td> <tmpl_unless name=status_anime_iswatched><span class="gray"></tmpl_unless><tmpl_var name=data_anime_my_watchedeps>/<tmpl_var name=data_anime_my_eps><tmpl_unless name=status_anime_iswatched></span></tmpl_unless> </td>
		<td> <tmpl_var name=data_anime_my_size_h> </td>   
	</tr>
	</tmpl_loop>
	</table>

	
</td></tr></table>
</body>
</html>