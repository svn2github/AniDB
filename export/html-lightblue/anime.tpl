<tmpl_loop name=loop_anime>anidb = "<tmpl_var name=global_animedburl>";

var xa = new Object();
xa.id = <tmpl_var name=data_anime_id>;
xa.eps = <tmpl_var name=data_anime_eps>;
xa.name = "<tmpl_var expr="jsencode(data_anime_name)">";
xa.year = "<tmpl_var name=data_anime_year>";
xa.startdate = "<tmpl_var name=data_anime_startdate_short>";
xa.enddate = "<tmpl_var name=data_anime_enddate_short>";
xa.url = "<tmpl_var name=data_anime_url>";
xa.typename = "<tmpl_var name=data_anime_type_name>";
xa.eps = <tmpl_var name=data_anime_eps>;
xa.myEps = <tmpl_var name=data_anime_my_eps>;
xa.myWeps = <tmpl_var name=data_anime_my_watchedeps>;
xa.mySEps = <tmpl_var name=data_anime_my_eps_special>;
xa.myWSEps = <tmpl_var name=data_anime_my_watchedeps_special>;
xa.description = "<tmpl_var expr="jsencode(data_anime_other)">";
xa.shortDate = "<tmpl_var name=data_anime_date_short>";
xa.shortUpdate = "<tmpl_var name=data_anime_update_short>";
xa.myvote = "<tmpl_var name=data_anime_my_vote>";
xa.mytmpvote = "<tmpl_var name=data_anime_my_tmpvote>";
xa.rating = "<tmpl_var name=data_anime_rating>";
xa.votes = <tmpl_var name=data_anime_votes>;
xa.temprating = "<tmpl_var name=data_anime_tmprating>";
xa.tempvotes = "<tmpl_var name=data_anime_tmpvotes>";
xa.reviews = <tmpl_var name=data_anime_reviews>;
xa.rrating = "<tmpl_var name=data_anime_reviewrating>";
xa.annUrl = "<tmpl_var name=data_anime_annurl>";
xa.animeNfoUrl = "<tmpl_var name=data_anime_animenfourl>";
xa.aniplaneturl = "<tmpl_var name=data_anime_animeplaneturl>";
xa.japkanji = "<tmpl_if name=status_anime_title_has_jap_kanji><tmpl_var expr="jsencode(data_anime_title_jap_kanji)"><tmpl_else>-</tmpl_if>";
xa.eng = "<tmpl_if name=status_anime_title_has_eng><tmpl_var expr="jsencode(data_anime_title_eng)"><tmpl_else>-</tmpl_if>";
xa.company = "";
xa.aliases = "";
xa.shortNames = "";
xa.genres = "";
xa.awards = new Array();
<tmpl_loop name=loop_anime_company>xa.company += "<tmpl_var name=data_anime_company_name><tmpl_if name="__LAST__"><tmpl_else>, </tmpl_if>";</tmpl_loop>
<tmpl_loop name=loop_anime_title_alias>xa.aliases += "<tmpl_var name=data_anime_title_alias_name><tmpl_if name="__LAST__"><tmpl_else>, </tmpl_if>";</tmpl_loop>
<tmpl_loop name=loop_anime_title_short>xa.shortNames += "<tmpl_var name=data_anime_title_short_name><tmpl_if name="__LAST__"><tmpl_else>, </tmpl_if>";</tmpl_loop>
<tmpl_loop name=loop_anime_genren>xa.genres += "<tmpl_var name=data_anime_genren_name><tmpl_if name="__LAST__"><tmpl_else>, </tmpl_if>";</tmpl_loop>
<tmpl_loop name=loop_anime_awards>var aurl = "<tmpl_var name=data_anime_award_url>";
var aid = "<tmpl_var name=data_anime_award_id>";
var aname = "<tmpl_var name=data_anime_award_name>";
xa.awards.push(aurl, aid, aname);
</tmpl_loop>
xa.enums = new Array();
var xanext = 0;
<tmpl_loop name=loop_ep>xa.enums.push(xanext);xanext++;
</tmpl_loop>

var enext = 0;
var elist = new Array();
var flist = new Array();

<tmpl_loop name=loop_ep>
elist[enext] = new Object();
elist[enext].id = <tmpl_var name=data_ep_id>;
elist[enext].num = "<tmpl_var name=data_ep_epno>";
elist[enext].name = "<tmpl_var expr="jsencode(data_ep_name)">";
elist[enext].nameromaji = "<tmpl_if name=status_ep_hasname_romaji><tmpl_var expr="jsencode(data_ep_name_romaji)"><tmpl_else>-</tmpl_if>";
elist[enext].namekanji = "<tmpl_if name=status_ep_hasname_kanji><tmpl_var expr="jsencode(data_ep_name_kanji)"><tmpl_else>-</tmpl_if>";
elist[enext].elength = <tmpl_var name=data_ep_length>;
elist[enext].shortDate = "<tmpl_var name=data_ep_date_short>";
elist[enext].mystate = <tmpl_var name=data_ep_mystate>;
elist[enext].mystatename = "<tmpl_var name=data_ep_mystate_string>";
elist[enext].state = <tmpl_var name=data_ep_state>;
elist[enext].isWatched = <tmpl_if name=status_ep_iswatched>true;<tmpl_else>false;</tmpl_if>
elist[enext].fnums = new Array();

<tmpl_loop name=loop_file>
flist[<tmpl_var name=data_file_id>] = new Object();
elist[enext].fnums.push (<tmpl_var name=data_file_id>);
flist[<tmpl_var name=data_file_id>].id = <tmpl_var name=data_file_id>;
flist[<tmpl_var name=data_file_id>].sizeBytes = "<tmpl_var name=data_file_size>";
flist[<tmpl_var name=data_file_id>].sizePlain = "<tmpl_var name=data_file_size_plain>";
flist[<tmpl_var name=data_file_id>].sizeMB = "<tmpl_var name=data_file_size_h>";
flist[<tmpl_var name=data_file_id>].filetype = "<tmpl_var name=data_file_filetype>";
flist[<tmpl_var name=data_file_id>].crc = "<tmpl_var name=data_file_crc>";
flist[<tmpl_var name=data_file_id>].ed2kHash = "<tmpl_var name=data_file_ed2k_hash>";
flist[<tmpl_var name=data_file_id>].groupID = <tmpl_var name=data_file_group_id>;
flist[<tmpl_var name=data_file_id>].groupName = "<tmpl_var name=data_file_group_name>";
flist[<tmpl_var name=data_file_id>].groupShort = "<tmpl_var name=data_file_group_shortname>";
flist[<tmpl_var name=data_file_id>].qualID = <tmpl_var name=data_file_qual_id>;
flist[<tmpl_var name=data_file_id>].qualName = "<tmpl_var name=data_file_qual_name>";
flist[<tmpl_var name=data_file_id>].resName = "<tmpl_var name=data_file_res_name>";
flist[<tmpl_var name=data_file_id>].videoBr = <tmpl_var name=data_file_vbitrate>;
flist[<tmpl_var name=data_file_id>].videoCodecName = "<tmpl_var name=data_file_vcodec_name>";
flist[<tmpl_var name=data_file_id>].videoAR = <tmpl_var name=data_file_ar>;
flist[<tmpl_var name=data_file_id>].videoFlags = <tmpl_if name=data_file_vflags><tmpl_var name=data_file_vflags>;<tmpl_else>0</tmpl_if>
flist[<tmpl_var name=data_file_id>].audioCount = <tmpl_var name=data_file_audcnt>;
flist[<tmpl_var name=data_file_id>].audioChannels = <tmpl_var name=data_file_chantype>;
flist[<tmpl_var name=data_file_id>].audioBr = <tmpl_var name=data_file_abitrate>;
flist[<tmpl_var name=data_file_id>].audioCodecName = "<tmpl_var name=data_file_acodec_name>";
flist[<tmpl_var name=data_file_id>].langID = <tmpl_var name=data_file_lang_id>;
flist[<tmpl_var name=data_file_id>].langName = "<tmpl_var name=data_file_lang_name>";
flist[<tmpl_var name=data_file_id>].audio2Channels = <tmpl_var name=data_file_chantype2>;
flist[<tmpl_var name=data_file_id>].audio2Br = <tmpl_var name=data_file_abitrate2>;
flist[<tmpl_var name=data_file_id>].audio2CodecName = "<tmpl_var name=data_file_acodec_name2>";
flist[<tmpl_var name=data_file_id>].lang2ID = <tmpl_var name=data_file_lang_id2>;
flist[<tmpl_var name=data_file_id>].lang2Name = "<tmpl_var name=data_file_lang_name2>";
flist[<tmpl_var name=data_file_id>].subCount = <tmpl_var name=data_file_subcnt>;
flist[<tmpl_var name=data_file_id>].subID = <tmpl_var name=data_file_sub_id>;
flist[<tmpl_var name=data_file_id>].subName = "<tmpl_var name=data_file_sub_name>";
flist[<tmpl_var name=data_file_id>].subType = <tmpl_var name=data_file_stype>;
flist[<tmpl_var name=data_file_id>].subFlags = <tmpl_var name=data_file_sflags>;
flist[<tmpl_var name=data_file_id>].sub2ID = <tmpl_var name=data_file_sub_id2>;
flist[<tmpl_var name=data_file_id>].sub2Name = "<tmpl_var name=data_file_sub_name2>";
flist[<tmpl_var name=data_file_id>].sub2Type = <tmpl_var name=data_file_stype2>;
flist[<tmpl_var name=data_file_id>].sub2Flags = <tmpl_var name=data_file_sflags2>;
flist[<tmpl_var name=data_file_id>].typeName = "<tmpl_var name=data_file_type_name>";
flist[<tmpl_var name=data_file_id>].hasComment = <tmpl_if name=status_file_hascomment>true;<tmpl_else>false;</tmpl_if>
flist[<tmpl_var name=data_file_id>].other = "<tmpl_var expr="jsencode(data_file_other)">";
flist[<tmpl_var name=data_file_id>].hasStorage = <tmpl_if name=data_file_storage>true;<tmpl_else>false;</tmpl_if>
flist[<tmpl_var name=data_file_id>].storage = "<tmpl_var name=data_file_storage>";
flist[<tmpl_var name=data_file_id>].hasSource = <tmpl_if name=data_file_source>true;<tmpl_else>false;</tmpl_if>
flist[<tmpl_var name=data_file_id>].source = "<tmpl_var name=data_file_source>";
flist[<tmpl_var name=data_file_id>].mystate = <tmpl_var name=data_file_mystate>;
flist[<tmpl_var name=data_file_id>].mystatename = "<tmpl_var name=data_file_mystate_string>";
flist[<tmpl_var name=data_file_id>].myfilestate = <tmpl_var name=data_file_myfilestate>;
flist[<tmpl_var name=data_file_id>].myfilestatename = "<tmpl_var name=data_file_myfilestate_string>";
flist[<tmpl_var name=data_file_id>].isGeneric = <tmpl_if name=status_file_isgeneric>true;<tmpl_else>false;</tmpl_if>
flist[<tmpl_var name=data_file_id>].isWatched = <tmpl_if name=status_file_iswatched>true;<tmpl_else>false;</tmpl_if>
flist[<tmpl_var name=data_file_id>].Version = "<tmpl_var name=data_file_state_versionname>";
flist[<tmpl_var name=data_file_id>].crcStatus = "<tmpl_if name=status_file_state_crcok>1</tmpl_if><tmpl_if name=status_file_state_crcfailed>2</tmpl_if>";
</tmpl_loop>
enext++;
</tmpl_loop></tmpl_loop>
