<tmpl_loop name=loop_anime><?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="../anime.xsl"?>
<!--
        TPL to XML wrapper
        Status are attributes; not defined or value "1".
        Others are CDATA elements to allow special characters and future changes.
-->
<anime
        <tmpl_if name=status_anime_iscomplete>complete="1"</tmpl_if>
        <tmpl_if name=status_anime_iswatched>watched="1"</tmpl_if>
>
        <id><![CDATA[<tmpl_var name=data_anime_id>]]></id>
        <name><![CDATA[<tmpl_var name=data_anime_name>]]></name>
        <eps><![CDATA[<tmpl_var name=data_anime_eps>]]></eps>
        <year><![CDATA[<tmpl_var name=data_anime_year>]]></year>
        <producer><![CDATA[<tmpl_var name=data_anime_producer>]]></producer>
        <url><![CDATA[<tmpl_var name=data_anime_url>]]></url>
        <date><![CDATA[<tmpl_var name=data_anime_date>]]></date>
        <update><![CDATA[<tmpl_var name=data_anime_update>]]></update>
        <rating><![CDATA[<tmpl_var name=data_anime_rating>]]></rating>
        <votes><![CDATA[<tmpl_var name=data_anime_votes>]]></votes>
        <type_id><![CDATA[<tmpl_var name=data_anime_type_id>]]></type_id>
        <type_name><![CDATA[<tmpl_var name=data_anime_type_name>]]></type_name>
        <animenfoid><![CDATA[<tmpl_var name=data_anime_animenfoid>]]></animenfoid>
        <my_eps><![CDATA[<tmpl_var name=data_anime_my_eps>]]></my_eps>
        <my_watchedeps><![CDATA[<tmpl_var name=data_anime_my_watchedeps>]]></my_watchedeps>
        <my_size><![CDATA[<tmpl_var name=data_anime_my_size>]]></my_size>
        <my_size_h><![CDATA[<tmpl_var name=data_anime_my_size_h>]]></my_size_h>

        <reviewrating><![CDATA[<tmpl_var name=data_anime_reviewrating>]]></reviewrating>
        <reviews><![CDATA[<tmpl_var name=data_anime_reviews>]]></reviews>

        <other><![CDATA[<tmpl_var name=data_anime_other>]]></other>
        <tmpl_loop name=loop_anime_title_alias>
                <title_alias><![CDATA[<tmpl_var name=data_anime_title_alias_name>]]></title_alias>
        </tmpl_loop>
        <tmpl_loop name=loop_anime_title_short>
                <title_short><![CDATA[<tmpl_var name=data_anime_title_short_name>]]></title_short>
        </tmpl_loop>
        <tmpl_loop name=loop_anime_genre>
                <genre>
                        <id><![CDATA[<tmpl_var name=data_anime_genre_id>]]></id>
                        <![CDATA[<tmpl_var name=data_anime_genre_name>]]>
                </genre>
        </tmpl_loop>
        <tmpl_loop name=loop_ep>
        <episode
                <tmpl_if name=status_ep_iswatched>iswatched="1"</tmpl_if>
                <tmpl_if name=status_ep_hasfile>hasfile="1"</tmpl_if>

                <tmpl_if name=status_ep_state_special>state_special="1"</tmpl_if>
                <tmpl_if name=status_ep_state_recap>state_recap="1"</tmpl_if>
                <tmpl_if name=status_ep_state_op>state_op="1"</tmpl_if>
                <tmpl_if name=status_ep_state_end>state_end="1"</tmpl_if>
                <tmpl_if name=status_ep_state_hanger>state_hanger="1"</tmpl_if>
        >
                <id><![CDATA[<tmpl_var name=data_ep_id>]]></id>
                <epno><![CDATA[<tmpl_var name=data_ep_epno>]]></epno>
                <name><![CDATA[<tmpl_var name=data_ep_name>]]></name>
                <length><![CDATA[<tmpl_var name=data_ep_length>]]></length>
                <aired><![CDATA[<tmpl_var name=data_ep_aired>]]></aired>
                <date><![CDATA[<tmpl_var name=data_ep_date>]]></date>
                <update><![CDATA[<tmpl_var name=data_ep_update>]]></update>
                <update_short><![CDATA[<tmpl_var name=data_ep_update_short>]]></update_short>
                <state><![CDATA[<tmpl_var name=data_ep_state>]]></state>

                <tmpl_loop name=loop_file>
                <file
                        <tmpl_if name=status_file_iswatched>iswatched="1"</tmpl_if>

                        <tmpl_if name=status_file_state_crcok>state_crcok="1"</tmpl_if>
                        <tmpl_if name=status_file_state_crcfailed>state_crcfailed="1"</tmpl_if>
                        <tmpl_if name=status_file_state_isv2>state_isv2="1"</tmpl_if>
                        <tmpl_if name=status_file_state_isv3>state_isv3="1"</tmpl_if>
                        <tmpl_if name=status_file_state_isv4>state_isv4="1"</tmpl_if>
                        <tmpl_if name=status_file_state_isv5>state_isv5="1"</tmpl_if>

                        <tmpl_if name=status_file_mystate_unknown>mystate_unknown="1"</tmpl_if>
                        <tmpl_if name=status_file_mystate_onhdd>mystate_onhdd="1"</tmpl_if>
                        <tmpl_if name=status_file_mystate_oncd>mystate_oncd="1"</tmpl_if>
                        <tmpl_if name=status_file_mystate_deleted>mystate_deleted="1"</tmpl_if>
                        <tmpl_if name=status_file_mystate_shared>mystate_shared="1"</tmpl_if>
                        <tmpl_if name=status_file_mystate_release>mystate_release="1"</tmpl_if>
                >
                        <fid><![CDATA[<tmpl_var name=data_file_fid>]]></fid>
                        <size><![CDATA[<tmpl_var name=data_file_size>]]></size>
                        <size_h><![CDATA[<tmpl_var name=data_file_size_h>]]></size_h>
                        <filetype><![CDATA[<tmpl_var name=data_file_filetype>]]></filetype>
                        <crc><![CDATA[<tmpl_var name=data_file_crc>]]></crc>
                        <md5><![CDATA[<tmpl_var name=data_file_md5>]]></md5>
                        <ed2k_link><![CDATA[<tmpl_var name=data_file_ed2k_link>]]></ed2k_link>
                        <ed2k_hash><![CDATA[<tmpl_var name=data_file_ed2k_hash>]]></ed2k_hash>
                        <ed2k_name><![CDATA[<tmpl_var name=data_file_ed2k_name>]]></ed2k_name>
                        <group_id><![CDATA[<tmpl_var name=data_file_group_id>]]></group_id>
                        <group_name><![CDATA[<tmpl_var name=data_file_group_name>]]></group_name>
                        <group_shortname><![CDATA[<tmpl_var name=data_file_group_shortname>]]></group_shortname>
                        <released><![CDATA[<tmpl_var name=data_file_released>]]></released>
                        <qual_id><![CDATA[<tmpl_var name=data_file_qual_id>]]></qual_id>
                        <qual_name><![CDATA[<tmpl_var name=data_file_qual_name>]]></qual_name>
                        <red_id><![CDATA[<tmpl_var name=data_file_res_id>]]></red_id>
                        <res_name><![CDATA[<tmpl_var name=data_file_res_name>]]></res_name>
                        <abitrate><![CDATA[<tmpl_var name=data_file_abitrate>]]></abitrate>
                        <vbitrate><![CDATA[<tmpl_var name=data_file_vbitrate>]]></vbitrate>
                        <lang_id><![CDATA[<tmpl_var name=data_file_lang_id>]]></lang_id>
                        <lang_name><![CDATA[<tmpl_var name=data_file_lang_name>]]></lang_name>
                        <sub_id><![CDATA[<tmpl_var name=data_file_sub_id>]]></sub_id>
                        <sub_name><![CDATA[<tmpl_var name=data_file_sub_name>]]></sub_name>
                        <type_id><![CDATA[<tmpl_var name=data_file_type_id>]]></type_id>
                        <type_name><![CDATA[<tmpl_var name=data_file_type_name>]]></type_name>
                        <date><![CDATA[<tmpl_var name=data_file_date>]]></date>
                        <update><![CDATA[<tmpl_var name=data_file_update>]]></update>
                        <storage><![CDATA[<tmpl_var name=data_file_storage>]]></storage>
                        <source><![CDATA[<tmpl_var name=data_file_source>]]></source>
                        <viewdate><![CDATA[<tmpl_var name=data_file_viewdate>]]></viewdate>

                        <state><![CDATA[<tmpl_var name=data_file_state>]]></state>

                        <mystate><![CDATA[<tmpl_var name=data_file_mystate>]]></mystate>
                        <mystate_string><![CDATA[<tmpl_var name=data_file_mystate_string>]]></mystate_string>

                        <mystate_icon><![CDATA[<tmpl_var name=data_file_mystate_icon>]]></mystate_icon>

                        <other><![CDATA[<tmpl_var name=data_ep_other>]]></other>
                </file>
                </tmpl_loop>
        </episode>
        </tmpl_loop>


        <!-- hmm, might be wrong location for globals ;) -->
        <global>
                <user><![CDATA[<tmpl_var name=global_user>]]></user>
                <date><![CDATA[<tmpl_var name=global_date>]]></date>
                <animecount><![CDATA[<tmpl_var name=global_animecount>]]></animecount>
                <epcount><![CDATA[<tmpl_var name=global_epcount>]]></epcount>
                <filecount><![CDATA[<tmpl_var name=global_filecount>]]></filecount>
                <bytecount><![CDATA[<tmpl_var name=global_bytecount>]]></bytecount>
                <bytecount_h><![CDATA[<tmpl_var name=global_bytecount_h>]]></bytecount_h>
                <animedburl><![CDATA[<tmpl_var name=global_animedburl>]]></animedburl>
        </global>

</anime>
</tmpl_loop>
