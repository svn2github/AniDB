<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="mylist.xsl"?>
<!--
        For the XML lovers out there :)
        Attribute/element choice might be a bit arbitrary but I'm no XML expert anyways
        This index XML output is almost the same as for the different anime titles except for the episodes.
        Template created by Rafael Vuijk (aka Dark Fader) - http://darkfader.net/
        Template files modified by Chuck Renner (aka chriv)
-->
<mylist
        user="<tmpl_var name=global_user>"
        animedburl="<tmpl_var name=global_animedburl>"
        date="<tmpl_var name=global_date>"
        animecount="<tmpl_var name=global_animecount>"
        epcount="<tmpl_var name=global_epcount>"
        filecount="<tmpl_var name=global_filecount>"
        bytecount_h="<tmpl_var name=global_bytecount_h>"
>
<tmpl_loop name=loop_anime>
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

</anime>
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

</mylist>
