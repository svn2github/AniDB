<?xml version="1.0" encoding="utf-8"?>

<!--

A my list page

by path[w]

Version 0.2

-->
<tmpl_loop name= loop_anime>

    <anime id="<tmpl_var name=data_anime_id>">

        <seriesInfo>

            <name>

                <romanji><![CDATA[<tmpl_var name=data_anime_name>]]></romanji>

                <kanji><![CDATA[<tmpl_var name=data_anime_title_jap_kanji>]]></kanji>

                <english><![CDATA[<tmpl_var name=data_anime_title_eng>]]></english>

                <other><![CDATA[<tmpl_var name=data_anime_title_other>]]></other>

                <synonym>

                    <tmpl_loop name= loop_anime_title_alias>

                        <alias><![CDATA[<tmpl_var name=data_anime_title_alias_name>]]></alias>

                    </tmpl_loop>

                </synonym>

                <shorts>

                    <tmpl_loop name= loop_anime_title_short>

                        <short><![CDATA[<tmpl_var name=data_anime_title_short_name>]]></short>

                    </tmpl_loop>

                </shorts>

            </name>

            <genres>

                <tmpl_loop name= loop_anime_genren>

                    <genre name="<tmpl_var name= data_anime_genren_name>" id="<tmpl_var name= data_anime_genren_id>"
                    weight="<tmpl_var name= data_anime_genren_weight>" weightName="<tmpl_var name= data_anime_genren_weight_name>"/>

                </tmpl_loop>

            </genres>

            <airingDate year="<tmpl_var name= data_anime_year>" start="<tmpl_var name= data_anime_startdate>"
                        end="<tmpl_var name= data_anime_enddate>"/>

            <type>

                <tmpl_var name= data_anime_type_name>

            </type>

            <rating>

                <tmpl_var name= data_anime_rating>

            </rating>


            <tmpl_if name= status_anime_hasawards>
                <awards>
                    <awardTypes>
                        <tmpl_loop name= loop_anime_award_types>
                            <awardType id="<tmpl_var name= data_anime_award_type_id >">
                                <![CDATA[<tmpl_var name= data_anime_award_type_name>]]>
                            </awardType>
                        </tmpl_loop>
                    </awardTypes>
                    <tmpl_loop name= loop_anime_awards>
                        <award awardTypeId="<tmpl_var name= data_anime_award_type>">
                            <awardName>
                                <![CDATA[<tmpl_var name= data_anime_award_name>]]>
                            </awardName>
                            <awardURL>
                                <tmpl_var name= data_anime_award_url>
                            </awardURL>
                        </award>
                    </tmpl_loop>
                </awards>
            </tmpl_if>

            <companies>

                <tmpl_loop name= loop_anime_company>
                    <company typeId="<tmpl_var name= data_anime_company_aptype>">
                        <type>
                            <![CDATA[<tmpl_var name= data_anime_company_aptype_name>]]>
                        </type>
                        <name>
                            <![CDATA[<tmpl_var name= data_anime_company_name>]]>
                        </name>
                    </company>
                </tmpl_loop>
            </companies>

            <synopsis><tmpl_var expr="jsencodexml(data_anime_other)"></synopsis>
        </seriesInfo>

        <episodes status="<tmpl_if name=status_anime_iscomplete>complete<tmpl_else>incomplete</tmpl_if>">
            <size>
                <tmpl_var name= data_anime_my_size_h>
            </size>
            <episodeCount>
                <total totalEpisodes="<tmpl_var name= data_anime_eps_total>"
                       watchedEpisodes="<tmpl_var name=data_anime_my_watchedeps_total>"
                       ownEpisodes="<tmpl_var name=data_anime_my_eps_total>"/>
                <normal totalEpisodes="<tmpl_var name= data_anime_eps>"
                        watchedEpisodes="<tmpl_var name= data_anime_my_watchedeps>"
                        ownEpisodes="<tmpl_var name= data_anime_my_eps>"/>
                <special totalEpisodes="<tmpl_var name= data_anime_eps_special>"
                         watchedEpisodes="<tmpl_var name= data_anime_my_watchedeps_special>"
                         ownEpisodes="<tmpl_var name= data_anime_my_eps_special>"/>
            </episodeCount>

            <groups>
                <tmpl_loop name= loop_anime_groups>
                    <group id="<tmpl_var name= data_anime_group_id>" state="<tmpl_var name= data_anime_group_state>"
                           rating="<tmpl_var name= data_anime_group_rating>"
                           voteCount="<tmpl_var name= data_anime_group_votes>">
                        <releasedEpisodes normal="<tmpl_var name= data_anime_group_epcnt>"
                                          special="<tmpl_var name= data_anime_group_sepcnt>" range="<tmpl_var name= data_anime_group_eprange>"/>
                        <name><tmpl_var expr="jsencodexml(data_anime_group_name)"></name>
                        <shortName><tmpl_var expr="jsencodexml(data_anime_group_sname)"></shortName>
                    </group>
                </tmpl_loop>
            </groups>
            
            <file_ep_relations>
            <tmpl_loop name= loop_data_file_fileeprel>
              <file id="<tmpl_var name=data_file_fileeprel_fid>" episode="<tmpl_var name=data_file_fileeprel_eid>" 
              start="<tmpl_var name=data_file_fileeprel_startp>" end="<tmpl_var name=data_file_fileeprel_endp>"/>
            </tmpl_loop>
			</file_ep_relations>
            <tmpl_loop name= loop_ep>

                <episode id="<tmpl_var name=data_ep_id>" number="<tmpl_var name=data_ep_epno>"

                         length="<tmpl_var name=data_ep_length>">

                    <name>

                        <english><![CDATA[<tmpl_var name=data_ep_name>]]></english>

                        <romanji><![CDATA[<tmpl_var name=data_ep_name_romaji>]]></romanji>

                        <kanji><![CDATA[<tmpl_var name=data_ep_name_kanji>]]></kanji>

                        <titles>

                            <tmpl_loop name= loop_data_ep_titles>

                                <name><![CDATA[<tmpl_var name=data_ep_title_name>]]></name>

                            </tmpl_loop>

                        </titles>

                    </name>

                    <airingDate>

                        <tmpl_var name= data_ep_aired_short>

                    </airingDate>

                    <otherInfo><![CDATA[<tmpl_var name=data_ep_other>]]></otherInfo>

                    <status recap="<tmpl_if name= status_ep_state_recap>true<tmpl_else>false</tmpl_if>"

                            special="<tmpl_if name= status_ep_state_special>true<tmpl_else>false</tmpl_if>"

                            opening="<tmpl_if name= status_ep_state_op>true<tmpl_else>false</tmpl_if>"

                            ending="<tmpl_if name= status_ep_state_end>true<tmpl_else>false</tmpl_if>"

                            noFiles="<tmpl_unless name= status_ep_hasfile>true<tmpl_else>false</tmpl_unless>"

                            type="<tmpl_var name= data_ep_state>"/>

                    <files>

                        <tmpl_loop name= loop_file>

                            <file id="<tmpl_var name=data_file_id>" size="<tmpl_var name=data_file_size_h>"

                                  type="<tmpl_var name=data_file_filetype>"
                                  state="<tmpl_var name=data_file_state>"

                                  version="<tmpl_var name=data_file_state_versionname>"

                                  generic="<tmpl_if name=status_file_isgeneric>true<tmpl_else>false</tmpl_if>">

                                <hashInformation>

                                    <crc>

                                        <tmpl_var name= data_file_crc>

                                    </crc>

                                    <md5>

                                        <tmpl_var name= data_file_md5>

                                    </md5>

                                    <sha1>

                                        <tmpl_var name= data_file_sha1>

                                    </sha1>

                                    <ed2kLink><![CDATA[<tmpl_var name= data_file_ed2k_link>]]></ed2kLink>

                                </hashInformation>

                                <releasedBy id="<tmpl_var name=data_file_group_id>">

                                    <name><![CDATA[<tmpl_var name=data_file_group_name>]]></name>

                                    <shortName>
                                        <![CDATA[<tmpl_var name= data_file_group_shortname>]]></shortName>

                                </releasedBy>

                                <dates releaseDate="<tmpl_var name=data_file_date_short>"

                                       additionDate="<tmpl_var name=data_file_ldate>"/>

                                <quality name="<tmpl_var name=data_file_qual_name>"

                                         source="<tmpl_var name=data_file_type_name>"

                                         userDefinedSource="<tmpl_var name=data_file_source>">

                                    <video resolution="<tmpl_var name=data_file_res_name>"

                                           aspectRatio="<tmpl_var name=data_file_ar>"

                                           framesPerSecond="<tmpl_var name=data_file_fps>"

                                           bitRate="<tmpl_var name=data_file_vbitrate>"

                                           codec="<tmpl_var name=data_file_vcodec_name>"/>

                                    <audioStreams>

                                        <audio language="<tmpl_var name=data_file_lang_name>"

                                               channels="<tmpl_var name=data_file_chantype>"

                                               bitrate="<tmpl_var name=data_file_abitrate>"

                                               codec="<tmpl_var name=data_file_acodec_name>"/>


                                        <tmpl_if expr="data_file_audcnt > 1">

                                            <audio language="<tmpl_var name=data_file_lang_name2>"

                                                   channels="<tmpl_var name=data_file_chantype2>"

                                                   bitrate="<tmpl_var name=data_file_abitrate2>"

                                                   codec="<tmpl_var name=data_file_acodec_name2>"/>

                                        </tmpl_if>

                                    </audioStreams>

                                    <subtitleStreams>

                                        <subtitles language="<tmpl_var name=data_file_sub_name>"

                                                   flag="<tmpl_var name=data_file_sflags>"

                                                   type="<tmpl_var name=data_file_stype>"/>

                                        <tmpl_if expr="data_file_subcnt > 1">

                                            <subtitles language="<tmpl_var name=data_file_sub_name2>"

                                                       flag="<tmpl_var name=data_file_sflags2>"

                                                       type="<tmpl_var name=data_file_stype2>"/>

                                        </tmpl_if>

                                    </subtitleStreams>

                                </quality>

                                <comments><![CDATA[<tmpl_var name=data_file_other>]]></comments>

                            </file>

                        </tmpl_loop>

                    </files>

                </episode>

            </tmpl_loop>

        </episodes>

    </anime>

</tmpl_loop>