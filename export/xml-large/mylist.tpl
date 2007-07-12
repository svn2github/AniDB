<?xml version="1.0" encoding="ISO-8859-1"?><?xml-stylesheet type="text/xsl" href="firstpass.xsl"?>

<!--

A my list page

by path[w]

Version 0.2

-->

<myList user="<tmpl_var name=global_user>" creationDate="<tmpl_var name=global_date_short>">

    <animeList count="<tmpl_var name=global_animecount>" episodes="<tmpl_var name=global_epcount>"

               files="<tmpl_var name=global_filecount>"

               size="<tmpl_var name=global_bytecount_h>">

        <tmpl_loop name= loop_anime>

            <anime id="<tmpl_var name=data_anime_id>" broadcastDate="<tmpl_var name= data_anime_startdate>"
                   type="<tmpl_var name= data_anime_type_name>" rating="<tmpl_var name= data_anime_rating>"
                     status="<tmpl_if name=status_anime_iscomplete>complete<tmpl_else>incomplete</tmpl_if>">
                <romanjiName><![CDATA[<tmpl_var name=data_anime_name>]]></romanjiName>
                <englishName><![CDATA[<tmpl_var name=data_anime_title_eng>]]></englishName>


                <episodeCount>
                    <total totalEpisodes="<tmpl_var name= data_anime_eps_total>"
                                ownEpisodes="<tmpl_var name=data_anime_my_eps_total>"/>
                    <normal totalEpisodes="<tmpl_var name= data_anime_eps>"
                                 ownEpisodes="<tmpl_var name= data_anime_my_eps>"/>
                    <special totalEpisodes="<tmpl_var name= data_anime_eps_special>"
                                  ownEpisodes="<tmpl_var name= data_anime_my_eps_special>"/>
                </episodeCount>
                <episodes>
                    <tmpl_loop name= loop_ep>
                        <episode id="<tmpl_var name=data_ep_id>">
                            <files>
                                <tmpl_loop name= loop_file>
                                    <file id="<tmpl_var name=data_file_id>"
                                          additionDate="<tmpl_var name=data_file_ldate>"/>
                                </tmpl_loop>
                            </files>
                        </episode>
                    </tmpl_loop>
                </episodes>
            </anime>
        </tmpl_loop>
        <databaseColumnTableSchema date="<tmpl_var name=global_date>"/>
    </animeList>
</myList>

