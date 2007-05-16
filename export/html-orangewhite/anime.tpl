<tmpl_loop name=loop_anime>
<html>
<head>
<title>::aniDB.Net::</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="../style.css" type="text/css">
<script language="javascript">
function toggle( targetId ){
  		target = document.all( targetId );
		nest = document.all( targetId + "info" );
  			if (target.style.display == "none"){
  				target.style.display = "";
				nest.style.display = "none";
  			} else {
  				target.style.display = "none";
				nest.style.display = "none";
  			}
}

function toggleB( targetId ){
  		target = document.all( targetId + "info" );
		nest = document.all( targetId );
  			if (target.style.display == ""){
  				target.style.display = "none";				
  			} else {
  				if (nest.style.display == "none"){
					target.style.display = "none";
				} else {
				target.style.display = "";
				}					
  			}
}



function hideAll()
{
	<tmpl_loop name=loop_ep>toggle('ep<tmpl_var name=data_ep_id>');
	</tmpl_loop>
}

function hideAllInfo()
{
	<tmpl_loop name=loop_ep>toggleB('ep<tmpl_var name=data_ep_id>');
	</tmpl_loop>
}
</script>
</head>

<body bgcolor="#FFFFFF" text="#666666" link="#333333" vlink="#333333" alink="#FF0000">
<table width="100%" border="0" cellspacing="2" cellpadding="2" class="table_mylist">
  <tr> 
    <td align="left" valign="top">
      <table border="0" cellspacing="2" cellpadding="0">
        <tr> 
          <td valign="top">
            <table border="0" cellspacing="0" cellpadding="0" width="100%">
              <tr class="table_mylist_info"> 
                <td width="100">Title: </td>
                <td><b><tmpl_var name=data_anime_name></b></td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Jap. Kanji:</td>
                <td><tmpl_var name=data_anime_title_jap_kanji></td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Synonyms:</td>
                <td> <tmpl_loop name=loop_anime_title_alias><tmpl_var name=data_anime_title_alias_name><tmpl_if name="__LAST__"><tmpl_else>, 
                  </tmpl_if></tmpl_loop> </td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Short:</td>
                <td> <tmpl_loop name=loop_anime_title_short><tmpl_var name=data_anime_title_short_name><tmpl_if name="__LAST__"><tmpl_else>, 
                  </tmpl_if></tmpl_loop> </td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Type: </td>
                <td><tmpl_var name=data_anime_type_name></td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Genres: </td>
                <td><tmpl_loop name=loop_anime_genre><tmpl_var name=data_anime_genre_name><tmpl_if name="__LAST__"><tmpl_else>, 
                  </tmpl_if></tmpl_loop></td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Year: </td>
                <td> <tmpl_var name=data_anime_year> </td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Producer: </td>
                <td> <tmpl_var name=data_anime_producer> </td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">URL: </td>
                <td> <a href="<tmpl_var name=data_anime_url>" target="_blank"><tmpl_var name=data_anime_url></a> 
                </td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Eps: </td>
                <td> <tmpl_var name=data_anime_eps> ( <tmpl_var name=data_anime_my_eps> 
                  in mylist / <tmpl_var name=data_anime_my_watchedeps> seen ) 
                </td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Specials:</td>
                <td> <tmpl_var name=data_anime_my_eps_special> in mylist / <tmpl_var name=data_anime_my_watchedeps_special> 
                  seen</td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Size: </td>
                <td> <a title="<tmpl_var name=data_anime_my_size> bytes"><tmpl_var name=data_anime_my_size_h></a></td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Rating: </td>
                <td> <tmpl_var name=data_anime_rating> (<tmpl_var name=data_anime_votes> 
                  votes)</td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Reviews: </td>
                <td> <tmpl_var name=data_anime_reviewrating> (<tmpl_var name=data_anime_reviews> 
                  reviews)</td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">AniDB AID: </td>
                <td> <a href="<tmpl_var name=global_animedburl>?show=anime&aid=<tmpl_var name=data_anime_id>" target="_blank"> 
                  <tmpl_var name=data_anime_id></a></td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">AnimeNfo ID:</td>
                <td> <a href="<tmpl_var name=data_anime_animenfourl>"> <tmpl_var name=data_anime_animenfoid></a> 
                </td>
              </tr>
              <tr class="table_mylist_info"> 
                <td width="100">Date:</td>
                <td> <tmpl_var name=data_anime_date></td>
              </tr>
            </table>
          </td>
        </tr>
        <tr> 
          <td class="table_mylist_infoText">&nbsp;</td>
        </tr>
        <tr> 
          <td class="table_mylist_infoText"><tmpl_var name=data_anime_other>&nbsp;</td>
        </tr>
        <tr> 
          <td class="table_mylist_infoText">&nbsp;</td>
        </tr>
        <tmpl_loop name=loop_anime_awards> 
        <tr> 
          <td class="table_mylist_infoText">award: <a href="<tmpl_var name=data_anime_award_url>"><tmpl_var name=data_anime_award_name></a></td>
        </tr>
        </tmpl_loop> 
      </table>
    </td>
  </tr>
  <tr> 
    <td align="left" valign="top"><font size="1"><a href="javascript:hideAll();">[Show/Hide 
      File Information]</a> | <a href="javascript:hideAllInfo();">[Show/Hide Full 
      File Information]</a> | <a href="../mylist.htm">[ Back to list ]</a> | <a href="javascript:alert('A green background in a episode field means a watched episode.\nA green background in a file field means a watched file.\nA green background in a extra info field means a CRC OK file.');">[Help]</a></font></td>
  </tr>
  <tr> 
    <td align="left" valign="top"> 
      <table cellpadding="0" class="table_mylist_info">
        <tr class="table_mylist_infoTextWhite"> 
          <td width="20" bgcolor="#FF9900">Ep</td>
          <td bgcolor="#FF9900">Name</td>
          <td width="60" bgcolor="#FF9900">Length</td>
          <td width="70" bgcolor="#FF9900">Aired</td>
          <td width="40" bgcolor="#FF9900">State</td>
        <tr> 
          <td><tmpl_loop name=loop_ep> 
		  <tr class="episode" onClick="javascript:toggle('ep<tmpl_var name=data_ep_id>');" onMouseOver="this.className='episode_sel';" onMouseOut="this.className='episode';" <tmpl_if name=status_ep_iswatched>bgcolor="#CCFF99"</tmpl_if>> 
          <td> <a href="<tmpl_var name=global_animedburl>?show=ep&eid=<tmpl_var name=data_ep_id>" target="_blank"><tmpl_var name=data_ep_epno></a> 
          </td>
          <td> <tmpl_var name=data_ep_name>&nbsp;</td>
          <td> 
            <div align="center"><tmpl_var name=data_ep_length> min</div>
          </td>
          <td> 
            <div align="center"><tmpl_var name=data_ep_aired_short></div>
          </td>
          <td> 
            <div align="center"><tmpl_var name=data_ep_mystate_icon></div>
          </td>
        </tr>
        <tr> 
          <td colspan="5"> 
            <blockquote> 
			<div id=ep<tmpl_var name=data_ep_id> style="display:none;"> 
              <table width="870" border="0" cellspacing="1" cellpadding="0" class="table_mylist_info" align="center">
                <tr bgcolor="#FF9900" class="table_mylist_infoTextWhite"> 
                  <td width="40">fID</td>
                  <td width="50">Size</td>
                  <td width="150">Lang. (Aud./Sub.)</td>
                  <td width="90">Group</td>
                  <td width="70">FileType</td>
                  <td width="70">Quality</td>
                  <td width="80">Resolution</td>
                  <td width="90">Video Codec</td>
                  <td width="90">Audio Codec</td>
                  <td width="80">Source</td>
                  <td width="50">Ed2k</td>
				  <td width="20">...</td>
                </tr>
                <tmpl_loop name=loop_file> <tr class="episode" onMouseOver="this.className='episode_sel';" onMouseOut="this.className='episode';" 
                <tmpl_if name=status_file_iswatched>bgcolor="#CCFF99"</tmpl_if>> 
                <td><font size="1"><a target="_blank" href="<tmpl_var name=global_animedburl>?show=file&aid=<tmpl_var name=data_anime_id>&eid=<tmpl_var name=data_ep_id>&fid=<tmpl_var name="data_file_id">&nonav=1"><tmpl_var name="data_file_id"></a></font></td>
                <td> <font size="1"><a title="<tmpl_var name=data_file_size> bytes"><tmpl_var name=data_file_size_h></a> 
                  </font></td>
                <td> 
                  <div align="center"><font size="1"><tmpl_var name=data_file_lang_name> 
                    / <tmpl_var name=data_file_sub_name> </font></div>
                </td>
                <td> 
                  <div align="center"><font size="1"><a href="<tmpl_var name=global_animedburl>?show=group&gid=<tmpl_var name=data_file_group_id>" target="_blank" Title="Group: <tmpl_var name=data_file_group_name>"><tmpl_var name=data_file_group_shortname></a> 
                    </font></div>
                </td>
                <td> 
                  <div align="center"><font size="1"><tmpl_var name=data_file_filetype> 
                    </font></div>
                </td>
                <td> 
                  <div align="center"><font size="1"><tmpl_var name=data_file_qual_name> 
                    </font></div>
                </td>
                <td> 
                  <div align="center"><font size="1"><tmpl_var name=data_file_res_name> 
                    </font></div>
                </td>
                <td> 
                  <div align="center"><font size="1"><a title="bitrate: <tmpl_var name=data_file_vbitrate>"><tmpl_var name=data_file_vcodec_name></a> 
                    </font></div>
                </td>
                <td> 
                  <div align="center"><font size="1"><a title="bitrate: <tmpl_var name=data_file_abitrate>"><tmpl_var name=data_file_acodec_name></a> 
                    </font></div>
                </td>
                <td> 
                  <div align="center"><font size="1"><tmpl_var name=data_file_source> 
                    </font></div>
                </td>
                <td> 
                  <div align="center"><a href="<tmpl_var name=data_file_ed2k_link>" title="<tmpl_var name=data_file_ed2k_name>"><font size="1">link</font></a></div>
                </td>
                <td> 
                  <div align="center"><a href="javascript:toggle('ep<tmpl_var name=data_ep_id>info');" Title="Click for more file information"><font size="1">[...]</font></a></div>
                </td>
                </tr>
              </table>
              </div>
              <div id=ep<tmpl_var name=data_ep_id>info style="display:none;">
              <table width="870" border="0" cellspacing="2" cellpadding="0" class="table_mylist_info">
                <tr bgcolor="#DDDDDD" class="table_mylist_infoText"> 
                  <td width="70">CRC sum</td>
                  <td width="200">MD5 sum</td>
                  <td width="200">SHA1 sum</td>
                  <td width="70">Release</td>
                  <td width="280">Other</td>
                  <td width="50">State</td>
                </tr>
                <tr class="episode" onMouseOver="this.className='episode_sel';" onMouseOut="this.className='episode';" 
                <tmpl_if name=status_file_state_crcok>bgcolor="#CCFF99"</tmpl_if>> 
                  <td><div align="center"><font size="1"><tmpl_var name=data_file_crc></font></div></td>
                  <td><div align="center"><font size="1"><tmpl_var name=data_file_md5></font></div></td>
                  <td><div align="center"><font size="1"><tmpl_var name=data_file_sha1></font></div></td>
                  <td><div align="center"><font size="1"><tmpl_var name=data_file_released_short></font></div></td>
                  <td><div align="center"><font size="1"><tmpl_var name=data_file_other></font></div></td>
                  <td><div align="center"><font size="1"><tmpl_var name=data_file_mystate_icon></font></div></td>
                </tr>
				</tmpl_loop> 
              </table>
              </div>
              </blockquote>
          </td>
          <tmpl_if name=status_ep_hasfile> </tmpl_if> </tr></tmpl_loop>
      </table>
    </td>
  </tr>
</table>
<p><font size="1">List created with <a href="<tmpl_var name=global_animedburl>"><b>aniDB</b></a> 
  export feature.</font></p>
<p>&nbsp;</p>
</body>
</html>
</tmpl_loop>