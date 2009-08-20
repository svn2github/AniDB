<?xml version="1.0" encoding="utf-8"?>
<MyList>
<User Id="<tmpl_var name=global_user_id>"
Name="<tmpl_var expr="jsencodexmlcs(global_user)">"
Date="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(global_date,6,4),substr(global_date,3,2),substr(global_date,0,2),substr(global_date,11,5))">"
AnimeCount="<tmpl_var name=global_animecount>"
EpCount="<tmpl_var name=global_epcount>"
FileCount="<tmpl_var name=global_filecount>"
ByteCount="<tmpl_var expr="replace('\.','',global_bytecount)">"
AnimeDBUrl="<tmpl_var expr="jsencodexmlcs(global_animedburl)">"
AnimeAdded="<tmpl_var name=global_userstat_animeadded>"
EpsAdded="<tmpl_var name=global_userstat_epsadded>"
FilesAdded="<tmpl_var name=global_userstat_filesadded>"
GroupsAdded="<tmpl_var name=global_userstat_groupsadded>"
<tmpl_if expr="length(global_userstat_lamefiles)>0">LameFiles="<tmpl_var name=global_userstat_lamefiles>"</tmpl_if>
<tmpl_if expr="length(global_userstat_lamefilesp)>0">LameFilesP="<tmpl_var expr="replace('%','',global_userstat_lamefilesp)">"</tmpl_if>
IndependenceP="<tmpl_var expr="replace('%','',global_userstat_independencep)">"
LeechP="<tmpl_var expr="replace('%','',global_userstat_leechp)">"
Reviews="<tmpl_var name=global_userstat_reviews>"
Votes="<tmpl_var name=global_userstat_votes>"
TotalViewedP="<tmpl_var expr="replace('%','',global_userstat_totalviewedp)">"
TotalOwnedP="<tmpl_var expr="replace('%','',global_userstat_totalownedp)">"
OwnViewedP="<tmpl_var expr="replace('%','',global_userstat_ownviewedp)">"
ViewedEpCnt="<tmpl_var name=global_userstat_viewedepcnt>"
/>
<tmpl_loop name=global_genren_loop><GenreN Id="<tmpl_var name=global_genren_id>" Name="<tmpl_var expr="jsencodexmlcs(global_genren_name)">" ParentId="<tmpl_var name=global_genren_parentid>" IsHentai="<tmpl_var name=global_genren_ishentai>"/>
</tmpl_loop><tmpl_loop name=loop_anime><Anime Id="<tmpl_var name=data_anime_id>"
Eps="<tmpl_var name=data_anime_eps>"
EpsSpecial="<tmpl_var name=data_anime_eps_special>"
EpsTotal="<tmpl_var name=data_anime_eps_total>"
YearStart="<tmpl_var expr="substr(data_anime_year,0,4)">"
YearEnd="<tmpl_var expr="substr(data_anime_year,-4,4)">"
Url="<tmpl_var expr="jsencodexmlcs(data_anime_url)">"
Other="<tmpl_var expr="jsencodexmlcs(data_anime_other)">"
Date="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_date,6,4),substr(data_anime_date,3,2),substr(data_anime_date,0,2),substr(data_anime_date,11,5))">"
Update="<tmpl_var name=data_anime_update>"
StartDate="<tmpl_var expr="sprintf('%s-%s-%sZ',substr(data_anime_startdate,6,4),substr(data_anime_startdate,3,2),substr(data_anime_startdate,0,2))">"
<tmpl_if expr="data_anime_enddate!='?'">EndDate="<tmpl_var expr="sprintf('%s-%s-%sZ',substr(data_anime_enddate,6,4),substr(data_anime_enddate,3,2),substr(data_anime_enddate,0,2))">"</tmpl_if>
Rating="<tmpl_if expr="data_anime_rating!='-'"><tmpl_var name=data_anime_rating><tmpl_else>NaN</tmpl_if>"
Votes="<tmpl_var name=data_anime_votes>"
TmpRating="<tmpl_if expr="data_anime_tmprating!='-'"><tmpl_var name=data_anime_tmprating><tmpl_else>NaN</tmpl_if>"
TmpVotes="<tmpl_var name=data_anime_tmpvotes>"
ReviewRating="<tmpl_if expr="data_anime_reviewrating!='-'"><tmpl_var name=data_anime_reviewrating><tmpl_else>NaN</tmpl_if>"
Reviews="<tmpl_var name=data_anime_reviews>"
TypeId="<tmpl_var name=data_anime_type_id>"
TypeName="<tmpl_var expr="jsencodexmlcs(data_anime_type_name)">"
AnimeNfoId="<tmpl_var name=data_anime_animenfoid>"
AnimeNfoId2="<tmpl_var expr="jsencodexmlcs(data_anime_animenfoid2)">"
AnimeNfoUrl="<tmpl_var expr="jsencodexmlcs(data_anime_animenfourl)">"
AnimeNfoUrlName="<tmpl_var expr="jsencodexmlcs(data_anime_animenfourl_name)">"
AnnId="<tmpl_var name=data_anime_annid>"
<tmpl_if name=status_anime_ann_listed>AnnListed="1"</tmpl_if>
AnnUrl="<tmpl_var expr="jsencodexmlcs(data_anime_annurl)">"
AllCinemaId="<tmpl_var name=data_anime_allcinemaid>"
<tmpl_if name=status_anime_allcinema_listed>AllCinemaListed="1"</tmpl_if>
AllCinemaUrl="<tmpl_var expr="jsencodexmlcs(data_anime_allcinemaurl)">"
AnimePlanetId="<tmpl_var name=data_anime_animeplanetid>"
<tmpl_if name=status_anime_animeplanet_listed>AnimePlanetListed="1"</tmpl_if>
AnimePlanetUrl="<tmpl_var expr="jsencodexmlcs(data_anime_animeplaneturl)">"
<tmpl_if name=status_anime_isrestricted>IsRestricted="1"</tmpl_if>
MyEps="<tmpl_var name=data_anime_my_eps>"
MyEpsSpecial="<tmpl_var name=data_anime_my_eps_special>"
MyEpsTotal="<tmpl_var name=data_anime_my_eps_total>"
MyWatchedEps="<tmpl_var name=data_anime_my_watchedeps>"
MyWatchedEpsSpecial="<tmpl_var name=data_anime_my_watchedeps_special>"
MyWatchedEpsTotal="<tmpl_var name=data_anime_my_watchedeps_total>"
MyUnwatchedEps="<tmpl_var name=data_anime_my_unwatchedeps>"
MyUnwatchedEpsSpecial="<tmpl_var name=data_anime_my_unwatchedeps_special>"
MyUnwatchedEpsTotal="<tmpl_var name=data_anime_my_unwatchedeps_total>"
MySize="<tmpl_var expr="replace('\.','',data_anime_my_size)">"
<tmpl_if name=status_anime_iscomplete>IsComplete="1"</tmpl_if>
<tmpl_if name=status_anime_iswatched>IsWatched="1"</tmpl_if>
Name="<tmpl_var expr="jsencodexmlcs(data_anime_name)">"
<tmpl_if expr="data_anime_name_langid!=''">NameLangId="<tmpl_var name=data_anime_name_langid>"</tmpl_if>
NameLangName="<tmpl_var expr="jsencodexmlcs(data_anime_name_langname)">"
NameLangSName="<tmpl_var expr="jsencodexmlcs(data_anime_name_langsname)">"
TitleJapKanji="<tmpl_var expr="jsencodexmlcs(data_anime_title_jap_kanji)">"
<tmpl_if name=status_anime_title_has_jap_kanji>TitleHasJapKanji="1"</tmpl_if>
TitleEng="<tmpl_var expr="jsencodexmlcs(data_anime_title_eng)">"
<tmpl_if name=status_anime_title_has_eng>TitleHasEng="1"</tmpl_if>
TitleOther="<tmpl_var expr="jsencodexmlcs(data_anime_title_other)">"
<tmpl_if name=status_anime_title_has_other>TitleHasOther="1"</tmpl_if>
<tmpl_if name=status_anime_hasawards>HasAwards="1"</tmpl_if>
AwardIcons="<tmpl_var expr="jsencodexmlcs(data_anime_awardicons)">"
MyState="<tmpl_var name=data_anime_mystate>"
MyStateString="<tmpl_var expr="jsencodexmlcs(data_anime_mystate_string)">"
MyStateIcon="<tmpl_var expr="jsencodexmlcs(data_anime_mystate_icon)">"
<tmpl_if name=status_anime_mystate_undef>MyStateUndef="1"</tmpl_if>
<tmpl_if name=status_anime_mystate_unknown>MyStateUnknown="1"</tmpl_if>
<tmpl_if name=status_anime_mystate_onhdd>MyStateOnHdd="1"</tmpl_if>
<tmpl_if name=status_anime_mystate_oncd>MyStateOnCD="1"</tmpl_if>
<tmpl_if name=status_anime_mystate_deleted>MyStateDeleted="1"</tmpl_if>
<tmpl_if name=status_anime_mystate_release>MyStateRelease="1"</tmpl_if>
<tmpl_if name=status_anime_mystate_shared>MyStateShared="1"</tmpl_if>
<tmpl_if name=status_anime_isinwishlist>IsInWishList="1"
WishListType="<tmpl_var name=data_anime_wishlist_type>"
WishListTypeNames="<tmpl_var expr="jsencodexmlcs(data_anime_wishlist_type_name)">"
WishListPriority="<tmpl_var name=data_anime_wishlist_priority>"
WishListPriorityNames="<tmpl_var expr="jsencodexmlcs(data_anime_wishlist_priority_name)">"
WishListComment="<tmpl_var expr="jsencodexmlcs(data_anime_wishlist_comment)">"</tmpl_if>
<tmpl_if name=status_anime_my_isvoted>MyIsVoted="1"</tmpl_if>
MyVote="<tmpl_if expr="length(data_anime_my_vote)>0"><tmpl_var name=data_anime_my_vote><tmpl_else>NaN</tmpl_if>"
<tmpl_if expr="length(data_anime_my_vote_date)>0">MyVoteDate="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_my_vote_date,6,4),substr(data_anime_my_vote_date,3,2),substr(data_anime_my_vote_date,0,2),substr(data_anime_my_vote_date,11,5))">"</tmpl_if>
<tmpl_if name=status_anime_my_istmpvoted>MyIsTmpVoted="1"</tmpl_if>
MyTmpVote="<tmpl_if expr="length(data_anime_my_tmpvote)>0"><tmpl_var name=data_anime_my_tmpvote><tmpl_else>NaN</tmpl_if>"
<tmpl_if expr="length(data_anime_my_tmpvote_date)>0">MyTmpVoteDate="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_my_tmpvote_date,6,4),substr(data_anime_my_tmpvote_date,3,2),substr(data_anime_my_tmpvote_date,0,2),substr(data_anime_my_tmpvote_date,11,5))">"</tmpl_if>
>
<tmpl_loop name=loop_anime_genren><GenreN Id="<tmpl_var name=data_anime_genren_id>"
<tmpl_if name=data_anime_genren_ishentai>IsHentai="1"</tmpl_if>
Name="<tmpl_var expr="jsencodexmlcs(data_anime_genren_name)">"
<tmpl_if expr="data_anime_genren_parentid!=0">ParentId="<tmpl_var name=data_anime_genren_parentid>"</tmpl_if>
Weight="<tmpl_var name=data_anime_genren_weight>"
WeightName="<tmpl_var name=data_anime_genren_weight_name>"
/>
</tmpl_loop><tmpl_loop name=loop_anime_tag><Tag Id="<tmpl_var name=data_anime_tag_id>" Name="<tmpl_var expr="jsencodexmlcs(data_anime_tag_name)">" Date="<tmpl_var name=data_anime_tag_date>"/>
</tmpl_loop><tmpl_loop name=loop_anime_company><Company Id="<tmpl_var name=data_anime_company_id>"
Name="<tmpl_var expr="jsencodexmlcs(data_anime_company_name)">"
ShortName="<tmpl_var expr="jsencodexmlcs(data_anime_company_shortname)">"
OtherName="<tmpl_var expr="jsencodexmlcs(data_anime_company_othername)">"
Type="<tmpl_var name=data_anime_company_type>"
TypeName="<tmpl_var expr="jsencodexmlcs(data_anime_company_type_name)">"
Url="<tmpl_var expr="jsencodexmlcs(data_anime_company_url)">"
PicUrl="<tmpl_var expr="jsencodexmlcs(data_anime_company_picurl)">"
APType="<tmpl_var name=data_anime_company_aptype>"
APTypeName="<tmpl_var expr="jsencodexmlcs(data_anime_company_aptype_name)">"
/>
</tmpl_loop><tmpl_loop name=loop_anime_groups><Group Id="<tmpl_var name=data_anime_group_id>"
AGId="<tmpl_var name=data_anime_group_agid>"
Name="<tmpl_var expr="jsencodexmlcs(data_anime_group_name)">"
SName="<tmpl_var expr="jsencodexmlcs(data_anime_group_sname)">"
State="<tmpl_var expr="jsencodexmlcs(data_anime_group_state)">"
EpRange="<tmpl_var expr="jsencodexmlcs(data_anime_group_eprange)">"
LastEp="<tmpl_var name=data_anime_group_lastep>"
LastUp="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_group_lastup,6,4),substr(data_anime_group_lastup,3,2),substr(data_anime_group_lastup,0,2),substr(data_anime_group_lastup,11,5))">"
Votes="<tmpl_var name=data_anime_group_votes>"
Rating="<tmpl_if expr="data_anime_group_rating!='-'"><tmpl_var name=data_anime_group_rating><tmpl_else>NaN</tmpl_if>"
Cmts="<tmpl_var name=data_anime_group_cmts>"
EpCnt="<tmpl_var name=data_anime_group_epcnt>"
SEpCnt="<tmpl_var name=data_anime_group_sepcnt>"
MyVote="<tmpl_if expr="data_anime_group_myvote!='-'"><tmpl_var name=data_anime_group_myvote><tmpl_else>NaN</tmpl_if>"
<tmpl_if expr="data_anime_group_myvote_date!='-'">MyVoteDate="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_anime_group_myvote_date,6,4),substr(data_anime_group_myvote_date,3,2),substr(data_anime_group_myvote_date,0,2),substr(data_anime_group_myvote_date,11,5))">"</tmpl_if>
>
<tmpl_loop name=data_anime_group_alangs_loop><ALang Id="<tmpl_var name=data_anime_group_alang_id>" Name="<tmpl_var expr="jsencodexmlcs(data_anime_group_alang_name)">" SName="<tmpl_var expr="jsencodexmlcs(data_anime_group_alang_sname)">"/>
</tmpl_loop><tmpl_loop name=data_anime_group_slangs_loop><SLang Id="<tmpl_var name=data_anime_group_slang_id>" Name="<tmpl_var expr="jsencodexmlcs(data_anime_group_slang_name)">" SName="<tmpl_var expr="jsencodexmlcs(data_anime_group_slang_sname)">"/>
</tmpl_loop></Group>
</tmpl_loop><tmpl_loop name=loop_anime_titles><Title Name="<tmpl_var expr="jsencodexmlcs(data_anime_title_name)">" LangId="<tmpl_var name=data_anime_title_langid>" TypeId="<tmpl_var name=data_anime_title_type_id>" TypeString="<tmpl_var expr="jsencodexmlcs(data_anime_title_type_string)">" LangName="<tmpl_var expr="jsencodexmlcs(data_anime_title_langname)">" LangSName="<tmpl_var expr="jsencodexmlcs(data_anime_title_langsname)">"/>
</tmpl_loop><tmpl_loop name=loop_anime_award_types><AwardType Id="<tmpl_var name=data_anime_award_type_id>" Name="<tmpl_var expr="jsencodexmlcs(data_anime_award_type_name)">" Img="<tmpl_var expr="jsencodexmlcs(data_anime_award_type_img)">"/>
</tmpl_loop><tmpl_loop name=loop_anime_awards><tmpl_if expr="data_anime_award_id!=''"><Award Id="<tmpl_var name=data_anime_award_id>" Name="<tmpl_var expr="jsencodexmlcs(data_anime_award_name)">" Type="<tmpl_var expr="jsencodexmlcs(data_anime_award_type)">" Url="<tmpl_var expr="jsencodexmlcs(data_anime_award_url)">" PicUrl="<tmpl_var expr="jsencodexmlcs(data_anime_award_picurl)">"/></tmpl_if>
</tmpl_loop><tmpl_loop name=loop_ep><Ep Id="<tmpl_var name=data_ep_id>"
EpNo="<tmpl_var expr="jsencodexmlcs(data_ep_epno)">"
<tmpl_if name=status_ep_hasname_english>Name="<tmpl_var expr="jsencodexmlcs(data_ep_name)">"</tmpl_if>
<tmpl_if name=status_ep_hasname_romaji>NameRomaji="<tmpl_var expr="jsencodexmlcs(data_ep_name_romaji)">"</tmpl_if>
<tmpl_if name=status_ep_hasname_kanji>NameKanji="<tmpl_var expr="jsencodexmlcs(data_ep_name_kanji)">"</tmpl_if>
Length="<tmpl_var name=data_ep_length>"
<tmpl_if expr="data_ep_aired!='-'">Aired="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_aired,6,4),substr(data_ep_aired,3,2),substr(data_ep_aired,0,2),substr(data_ep_aired,11,5))">"</tmpl_if>
Other="<tmpl_var expr="jsencodexmlcs(data_ep_other)">"
Rating="<tmpl_if expr="data_ep_rating!='-'"><tmpl_var name=data_ep_rating><tmpl_else>NaN</tmpl_if>"
Votes="<tmpl_var name=data_ep_votes>"

MyVote="<tmpl_if expr="data_ep_myvote!='-'"><tmpl_var name=data_ep_myvote><tmpl_else>NaN</tmpl_if>"
<tmpl_if expr="data_ep_myvote_date!='-'">MyVoteDate="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_myvote_date,6,4),substr(data_ep_myvote_date,3,2),substr(data_ep_myvote_date,0,2),substr(data_ep_myvote_date,11,5))">"</tmpl_if>
Date="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_date,6,4),substr(data_ep_date,3,2),substr(data_ep_date,0,2),substr(data_ep_date,11,5))">"
<tmpl_if expr="data_ep_update!='-'">Update="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_ep_update,6,4),substr(data_ep_update,3,2),substr(data_ep_update,0,2),substr(data_ep_update,11,5))">"</tmpl_if>
State="<tmpl_var name=data_ep_state>"
<tmpl_if name=status_ep_state_special>StateSpecial="1"</tmpl_if>
<tmpl_if name=status_ep_state_recap>StateRecap="1"</tmpl_if>
<tmpl_if name=status_ep_state_op>StateOp="1"</tmpl_if>
<tmpl_if name=status_ep_state_end>StateEnd="1"</tmpl_if>
<tmpl_if name=status_ep_iswatched>StateIsWatched="1"</tmpl_if>
<tmpl_if name=status_ep_hasfile>StateHasFile="1"</tmpl_if>
MyState="<tmpl_var name=data_ep_mystate>"
MyStateString="<tmpl_var expr="jsencodexmlcs(data_ep_mystate_string)">"
MyStateIcon="<tmpl_var expr="jsencodexmlcs(data_ep_mystate_icon)">"

<tmpl_if name=status_ep_mystate_undef>MyStateUndef="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_unknown>MyStateUnknown="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_onhdd>MyStateOnHdd="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_oncd>MyStateOnCD="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_deleted>MyStateDeleted="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_release>MyStateRelease="1"</tmpl_if>
<tmpl_if name=status_ep_mystate_shared>MyStateShared="1"</tmpl_if>
<tmpl_if name=status_ep_state_hanger>MyStateHanger="1"</tmpl_if>
>
<tmpl_loop name=data_ep_titles><Title Name="<tmpl_var expr="jsencodexmlcs(data_ep_title_name)">" LangId="<tmpl_var name=data_ep_title_langid>" LangName="<tmpl_var expr="jsencodexmlcs(data_ep_title_langname)">" LangSName="<tmpl_var expr="jsencodexmlcs(data_ep_title_langsname)">"/>
</tmpl_loop><tmpl_loop name=loop_file><File Id="<tmpl_var name=data_file_id>"
LId="<tmpl_var name=data_file_lid>"
SizePlain="<tmpl_var name=data_file_size_plain>"
<tmpl_if name=status_file_isgeneric>IsGeneric="1"</tmpl_if>
FileType="<tmpl_var expr="jsencodexmlcs(data_file_filetype)">"
Crc="<tmpl_var name=data_file_crc>"
Md5="<tmpl_var name=data_file_md5>"
Sha1="<tmpl_var name=data_file_sha1>"
Tth="<tmpl_var name=data_file_tth>"
Ed2kLink="<tmpl_var expr="jsencodexmlcs(data_file_ed2k_link)">"
Ed2kHash="<tmpl_var name=data_file_ed2k_hash>"
Ed2kName="<tmpl_var expr="jsencodexmlcs(data_file_ed2k_name)">"
GroupId="<tmpl_var name=data_file_group_id>"
GroupName="<tmpl_var expr="jsencodexmlcs(data_file_group_name)">"
GroupShortName="<tmpl_var expr="jsencodexmlcs(data_file_group_shortname)">"
<tmpl_if expr="data_file_released!='-'">ReleaseD="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_file_released,6,4),substr(data_file_released,3,2),substr(data_file_released,0,2),substr(data_file_released,11,5))">"</tmpl_if>
QualId="<tmpl_var name=data_file_qual_id>"
QualName="<tmpl_var expr="jsencodexmlcs(data_file_qual_name)">"
ResName="<tmpl_var expr="jsencodexmlcs(data_file_res_name)">"
VidCnt="<tmpl_var name=data_file_vidcnt>"
AudCnt="<tmpl_var name=data_file_audcnt>"
SubCnt="<tmpl_var name=data_file_subcnt>"
Length="<tmpl_var name=data_file_length>"
Type="<tmpl_var name=data_file_type>"
TypeName="<tmpl_var expr="jsencodexmlcs(data_file_typename)">"
SourceId="<tmpl_var name=data_file_type_id>"
SourceName="<tmpl_var expr="jsencodexmlcs(data_file_type_name)">"
LDate="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_file_ldate,6,4),substr(data_file_ldate,3,2),substr(data_file_ldate,0,2),substr(data_file_ldate,11,5))">"
Date="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_file_date,6,4),substr(data_file_date,3,2),substr(data_file_date,0,2),substr(data_file_date,11,5))">"
<tmpl_if expr="data_file_update!='-'">Update="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_file_update,6,4),substr(data_file_update,3,2),substr(data_file_update,0,2),substr(data_file_update,11,5))">"</tmpl_if>
Storage="<tmpl_var expr="jsencodexmlcs(data_file_storage)">"
Source="<tmpl_var expr="jsencodexmlcs(data_file_source)">"
<tmpl_if name=status_file_hascomment>HasComment="<tmpl_var name=status_file_hascomment>"</tmpl_if>
Other="<tmpl_var expr="jsencodexmlcs(data_file_other)">"
<tmpl_if expr="data_file_viewdate!='-'">ViewDate="<tmpl_var expr="sprintf('%s-%s-%sT%s:00Z',substr(data_file_viewdate,6,4),substr(data_file_viewdate,3,2),substr(data_file_viewdate,0,2),substr(data_file_viewdate,11,5))">"</tmpl_if>
State="<tmpl_var name=data_file_state>"
StateCrcOk="<tmpl_var name=status_file_state_crcok>"
StateCrcFailed="<tmpl_var name=status_file_state_crcfailed>"
StateCrcUnverified="<tmpl_var name=status_file_state_crcunverified>"
StateIsV2="<tmpl_var name=status_file_state_isv2>"
StateIsV3="<tmpl_var name=status_file_state_isv3>"
StateIsV4="<tmpl_var name=status_file_state_isv4>"
StateIsV5="<tmpl_var name=status_file_state_isv5>"
IsWathed="<tmpl_var name=status_file_iswatched>"
StateVersionName="<tmpl_var expr="jsencodexmlcs(data_file_state_versionname)">"
MyOther="<tmpl_var name=data_file_myother>"
MyState="<tmpl_var name=data_file_mystate>"
MyStateString="<tmpl_var expr="jsencodexmlcs(data_file_mystate_string)">"
MyStateIcon="<tmpl_var expr="jsencodexmlcs(data_file_mystate_icon)">"
MyStateUnknown="<tmpl_var name=status_file_mystate_unknown>"
MyStateOnHdd="<tmpl_var name=status_file_mystate_onhdd>"
MyStateOnCD="<tmpl_var name=status_file_mystate_oncd>"
MyStateDeleted="<tmpl_var name=status_file_mystate_deleted>"
<tmpl_if expr="data_file_myfilestate!=''">MyFileState="<tmpl_var name=data_file_myfilestate>"</tmpl_if>
MyFileStateString="<tmpl_var expr="jsencodexmlcs(data_file_myfilestate_string)">"
MyFileStateIcon="<tmpl_var expr="jsencodexmlcs(data_file_myfilestate_icon)">"
MyFileStateNormal="<tmpl_var name=status_file_myfilestate_normal>"
MyFileStateInvalidCrc="<tmpl_var name=status_file_myfilestate_invalidcrc>"
MyFileStateSelfEdited="<tmpl_var name=status_file_myfilestate_selfedited>"
MyFileStateSelfRipped="<tmpl_var name=status_file_myfilestate_selfripped>"
MyFileStateOnDvd="<tmpl_var name=status_file_myfilestate_ondvd>"
MyFileStateOnVhs="<tmpl_var name=status_file_myfilestate_onvhs>"
MyFileStateOnTV="<tmpl_var name=status_file_myfilestate_ontv>"
MyFileStateTheater="<tmpl_var name=status_file_myfilestate_theater>"
MyFileStateOther="<tmpl_var name=status_file_myfilestate_other>"
MyFileStateRelease="<tmpl_var name=status_file_mystate_release>"
MyFileStateShared="<tmpl_var name=status_file_mystate_shared>"
>
<tmpl_if expr="data_file_vidcnt>=1"><Video
AR="<tmpl_var name=data_file_ar>"
ARName="<tmpl_var expr="jsencodexmlcs(data_file_ar_name)">"
Fps="<tmpl_var expr="data_file_fps/1000.0">"
BitRate="<tmpl_var name=data_file_vbitrate>"
CodecId="<tmpl_var name=data_file_vcodec_id>"
CodecName="<tmpl_var expr="jsencodexmlcs(data_file_vcodec_name)">"
Flags="<tmpl_var name=data_file_vflags>"
/></tmpl_if>
<tmpl_if expr="data_file_audcnt>=1"><Audio
Type="<tmpl_var name=data_file_atype>"
TypeName="<tmpl_var expr="jsencodexmlcs(data_file_atype_name)">"
BitRate="<tmpl_var name=data_file_abitrate>"
CodecId="<tmpl_var name=data_file_acodec_id>"
CodecName="<tmpl_var expr="jsencodexmlcs(data_file_acodec_name)">"
ChanType="<tmpl_var name=data_file_chantype>"
ChanTypeName="<tmpl_var expr="jsencodexmlcs(data_file_chantype_name)">"
LangId="<tmpl_var name=data_file_lang_id>"
LangName="<tmpl_var expr="jsencodexmlcs(data_file_lang_name)">"
LangSName="<tmpl_var expr="jsencodexmlcs(data_file_lang_sname)">"
/></tmpl_if>
<tmpl_if expr="data_file_audcnt>=2"><Audio
Type="<tmpl_var name=data_file_atype2>"
TypeName="<tmpl_var expr="jsencodexmlcs(data_file_atype_name2)">"
BitRate="<tmpl_var name=data_file_abitrate2>"
CodecId="<tmpl_var name=data_file_acodec_id2>"
CodecName="<tmpl_var expr="jsencodexmlcs(data_file_acodec_name2)">"
ChanType="<tmpl_var name=data_file_chantype2>"
ChanTypeName="<tmpl_var expr="jsencodexmlcs(data_file_chantype_name2)">"
LangId="<tmpl_var name=data_file_lang_id2>"
LangName="<tmpl_var expr="jsencodexmlcs(data_file_lang_name2)">"
LangSName="<tmpl_var expr="jsencodexmlcs(data_file_lang_sname2)">"
/></tmpl_if>
<tmpl_if expr="data_file_subcnt>=1"><Sub
Flags="<tmpl_var name=data_file_sflags>"
Type="<tmpl_var name=data_file_stype>"
TypeName="<tmpl_var expr="jsencodexmlcs(data_file_stypename)">"
SubId="<tmpl_var name=data_file_sub_id>"
SubName="<tmpl_var expr="jsencodexmlcs(data_file_sub_name)">"
SubSName="<tmpl_var expr="jsencodexmlcs(data_file_sub_sname)">"
/></tmpl_if>
<tmpl_if expr="data_file_subcnt>=2"><Sub
Flags="<tmpl_var name=data_file_sflags2>"
Type="<tmpl_var name=data_file_stype2>"
TypeName="<tmpl_var expr="jsencodexmlcs(data_file_stypename2)">"
SubId="<tmpl_var name=data_file_sub_id2>"
SubName="<tmpl_var expr="jsencodexmlcs(data_file_sub_name2)">"
SubSName="<tmpl_var expr="jsencodexmlcs(data_file_sub_sname2)">"
/></tmpl_if>
<tmpl_loop name=loop_data_file_filerel><FileRel FId="<tmpl_var expr="jsencodexmlcs(data_file_filerel_fid)">" OtherFId="<tmpl_var expr="jsencodexmlcs(data_file_filerel_otherfid)">" Type="<tmpl_var expr="jsencodexmlcs(data_file_filerel_type)">"/>
</tmpl_loop></File>
</tmpl_loop><tmpl_loop name=loop_data_ep_fileeprel><FileEpRel FId="<tmpl_var expr="jsencodexmlcs(data_ep_fileeprel_fid)">" EId="<tmpl_var expr="jsencodexmlcs(data_ep_fileeprel_eid)">" StartP="<tmpl_var expr="jsencodexmlcs(data_ep_fileeprel_startp)">" EndP="<tmpl_var expr="jsencodexmlcs(data_ep_fileeprel_endp)">"/>
</tmpl_loop></Ep>
</tmpl_loop></Anime>
</tmpl_loop></MyList>
