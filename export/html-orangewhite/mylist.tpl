<html>
<head>
<title>::AniDB.Net:: Export for <tmpl_var name=global_user></title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="style.css" type="text/css">
<script language="javascript">
function toggle( targetId ){
  		target = document.all( targetId );
  			if (target.style.display == "none"){
  				target.style.display = "";
  			} else {
  				target.style.display = "none";
  			}
}
</script>
</head>

<body bgcolor="#FFFFFF" text="#666666" link="#333333" vlink="#333333" alink="#FF0000">
<table width="100%" border="0" cellspacing="2" cellpadding="2" class="table_mylist">
  <tr> 
    <td align="left" valign="top"> 
      <table border="0" cellspacing="2" cellpadding="0" class="table_mylist_info">
        <tr bgcolor="#FF9900"> 
          <td colspan="2"><a href="javascript:toggle('mylist');" title="Click to show//hide info"><font color="#FFFFFF">MyList</font></a></td>
        </tr>
        <tr> 
          <td> 
            <div id=mylist style="display:"> 
              <table border="0" cellspacing="2" cellpadding="0" class="table_mylist_infoText">
                <tr> 
                  <td> User: </td>
                  <td> <b><tmpl_var name=global_user></b></td>
                </tr>
                <tr> 
                  <td> Created: </td>
                  <td> <a title="Date: <tmpl_var name=global_date>"><tmpl_var name=global_date_short></a></td>
                </tr>
                <tr> 
                  <td> Animes in list: </td>
                  <td> <tmpl_var name=global_animecount></td>
                </tr>
                <tr> 
                  <td> Episodes in list: </td>
                  <td> <tmpl_var name=global_epcount></td>
                </tr>
                <tr> 
                  <td> Files in list: </td>
                  <td> <tmpl_var name=global_filecount></td>
                </tr>
                <tr> 
                  <td> Size: </td>
                  <td> <a title="Size: <tmpl_var name=global_bytecount> bytes"><tmpl_var name=global_bytecount_h></a></td>
                </tr>
              </table>
            </div>
          </td>
        </tr>
      </table>
    </td>
    <td align="left" valign="top"> 
      <table cellpadding="0" cellspacing="2" class="table_mylist_info">
        <tr class="table_mylist_infoTextWhite"> 
          <td bgcolor="#FF9900">Name</td>
          <td bgcolor="#FF9900" width="25">&nbsp;</td>
          <td bgcolor="#FF9900" width="70">Type</td>
          <td bgcolor="#FF9900" width="70">Eps</td>
          <td bgcolor="#FF9900" width="70">Seen</td>
          <td bgcolor="#FF9900" width="100">Rating</td>
          <td bgcolor="#FF9900" width="70">Reviews</td>
        </tr>
        <tr> <tmpl_loop name=loop_anime><tr class="episode" onClick="javascript:toggle('ep<tmpl_var name=data_ep_id>');" onMouseOver="this.className='episode_sel';" onMouseOut="this.className='episode';" 
          <tmpl_if name=status_anime_iscomplete>bgcolor="#CCFF99"</tmpl_if>> 
          <td> <a href="anime/a<tmpl_var name=data_anime_id>.htm"><tmpl_var name=data_anime_name></a></td>
          <td> 
            <div align="center"><tmpl_var name=data_anime_mystate_icon></div>
          </td>
          <td> 
            <div align="center"><tmpl_var name=data_anime_type_name></div>
          </td>
          <td> 
            <div align="center"><tmpl_var name=data_anime_my_eps> / <tmpl_var name=data_anime_eps></div>
          </td>
          <td> 
            <div align="center"><tmpl_var name=data_anime_my_watchedeps> / <tmpl_var name=data_anime_my_eps></div>
          </td>
          <td> 
            <div align="center"><tmpl_var name=data_anime_rating> (<tmpl_var name=data_anime_votes>)</div>
          </td>
          <td> 
            <div align="center"><tmpl_var name=data_anime_reviewrating> (<tmpl_var name=data_anime_reviews>)</div>
          </td>
        </tr></tmpl_loop>
      </table>
    </td>
</table>
<p><font size="1">List created with <a href="<tmpl_var name=global_animedburl>"><b>aniDB</b></a> 
  export feature. </font></p>
</body>
</html>
