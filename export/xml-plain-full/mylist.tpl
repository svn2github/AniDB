<!--
  AniDB mylist template
  Version 1.0.0.$Rev: 10 $
  (C) 2008 Finalspace
s-->
<?xml version="1.0" encoding="UTF-8"?>
<mylist>

  <info>
    <cdate>
      <long><![CDATA[<tmpl_var name=global_date>]]></long>
      <short><![CDATA[<tmpl_var name=global_date_short>]]></short>
    </cdate>
    <sourceurl><![CDATA[<tmpl_var name=global_animedburl>]]></sourceurl>
  </info>

  <user>
    <name><![CDATA[<tmpl_var name=global_user>]]></name>
    <id><tmpl_var name=global_user_id></id>
  </user>
  
  <list>
    <animecount><tmpl_var name=global_animecount></animecount>
    <epcount><tmpl_var name=global_animecount></epcount>
    <filecount><tmpl_var name=global_filecount></filecount>
    <bytecount normal="<tmpl_var name=global_bytecount>" formated="<tmpl_var name=global_bytecount_h>" />
  </list>
  
  <userstat>
    <animeadded><tmpl_var name=global_userstat_animeadded></animeadded>
    <epsadded><tmpl_var name=global_userstat_epsadded></epsadded>
    <filesadded><tmpl_var name=global_userstat_filesadded></filesadded>
    <groupsadded><tmpl_var name=global_userstat_groupsadded></groupsadded>

    <lamefiles count="<tmpl_var name=global_userstat_lamefiles>" percent="<tmpl_var name=global_userstat_lamefilesp>" />
    <independence percent="<tmpl_var name=global_userstat_independencep>" />
    <leech percent="<tmpl_var name=global_userstat_leechp>" />

    <reviews><tmpl_var name=global_userstat_reviews></reviews>
    <votes><tmpl_var name=global_userstat_votes></votes>
    
    <totalviewed percent="<tmpl_var name=global_userstat_totalviewedp>" />
    <totalowned percent="<tmpl_var name=global_userstat_totalownedp>" />
    <viewed epcount="<tmpl_var name=global_userstat_viewedepcnt>" percent="<tmpl_var name=global_userstat_ownviewedp>" />
  </userstat>
  
  <animes><tmpl_loop name=loop_anime>
    <anime aid="<tmpl_var name=data_anime_id>" eps="<tmpl_var name=data_anime_eps_total>">
      <type id="<tmpl_var name=data_anime_type_id>"><![CDATA[<tmpl_var name=data_anime_type_name>]]></type>
      <title><![CDATA[<tmpl_var name=data_anime_name>]]></title>
    </anime>
  </tmpl_loop></animes>
  
</mylist>