<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-type" content="text/html;charset=UTF-8">
<title><tmpl_var name=global_user>'s Anime-Cloud</title>
</head>
<font face="arial,helvetica" size="1">
<tmpl_loop name=loop_anime><tmpl_if name=status_anime_iswatched><font size="<tmpl_if expr="data_anime_my_vote <= 4">1<tmpl_else><tmpl_if expr="data_anime_my_vote <= 5">2<tmpl_else><tmpl_if expr="data_anime_my_vote <= 6">3<tmpl_else><tmpl_if expr="data_anime_my_vote <= 7">4<tmpl_else><tmpl_if expr="data_anime_my_vote <= 8">5<tmpl_else><tmpl_if expr="data_anime_my_vote <= 9">6<tmpl_else><tmpl_if expr="data_anime_my_vote <= 10">7</tmpl_if></tmpl_if></tmpl_if></tmpl_if></tmpl_if></tmpl_if></tmpl_if>"><tmpl_var name=data_anime_name>, </font><tmpl_else></tmpl_if></tmpl_loop>
</font>
</html>
