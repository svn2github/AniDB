AnimeDB Template SDK

Version: 0.11 - 26.06.2007

(C) 2003-2004 by EXP





Get new Versions here:

  http://www.anidb.net/forum/viewtopic.php?t=429





Changelog:

  0.11 - 26.06.2007
	- data files updated
	- maybe some other changes?

  0.10 - 30.04.2006

    - fixed file data (only 1. vid and 1&2. vid&aud stream data available)

    - new fields FILE: data_file_type, data_file_vidcnt, data_file_ar, data_file_fps, data_file_vflags,

    					data_file_audcnt, data_file_chantype, data_file_abitrate2, data_file_acodec_id2, data_file_acodec_name2, data_file_chantype2, data_file_lang_id2, data_file_lang_name2, data_file_subcnt, data_file_sflags, data_file_stype, data_file_sub_id2, data_file_sub_name2, data_file_sflags2, data_file_stype2, data_file_length

    			ANIME: data_anime_annid, status_anime_ann_listed, data_anime_annurl, data_anime_allcinemaid, status_anime_allcinema_listed, data_anime_allcinemaurl, loop_anime_genren, data_anime_genren_id, data_anime_genren_name, data_anime_genren_parentid, data_anime_genren_ishentai, loop_anime_tag, data_anime_tag_id, data_anime_tag_name, data_anime_tag_date, loop_anime_company, data_anime_company_id, data_anime_company_name, data_anime_company_shortname, data_anime_company_othername, data_anime_company_type, data_anime_company_url, data_anime_company_picurl, data_anime_company_aptype

    			GLOBAL: global_userstat_animeadded, global_userstat_epsadded, global_userstat_filesadded, global_userstat_groupsadded, global_userstat_lamefiles, global_userstat_lamefilesp, global_userstat_independencep, global_userstat_leechp, global_userstat_reviews, global_userstat_votes, global_userstat_totalviewedp, global_userstat_totalownedp, global_userstat_ownviewedp, global_userstat_viewedepcnt



  0.09b - 13.10.2005

    - fixed episode number issue

    - added more info on ep type ids to docs

    - removed file shared/released info from docs

  0.09 - 25.09.2004

    - added new, redundant file crc info flag

    	status_file_state_crcunverified

    - fixed animeplanet links

    	data_anime_animeplaneturl

    - added flag to show if a file comment is present

    	status_file_hascomment

    	(remember, the filecomment can be found in: data_file_other)

    - added flag to show if a file is a generic file (btw. that means size==0)

    	status_file_isgeneric

    - added new mylist filestate info

    	data_file_myfilestate (0=normal, 1=invalid crc, 2=self edited,

           10=self ripped, 11=on dvd, 12=on vhs, 13=on tv, 14=theater,

           100=other)

        data_file_myfilestate_string, data_file_myfilestate_icon

        status_file_myfilestate_normal, status_file_myfilestate_invalidcrc,

        status_file_myfilestate_selfedited, status_file_myfilestate_selfripped,

        status_file_myfilestate_ondvd, status_file_myfilestate_onvhs,

        status_file_myfilestate_ontv, status_file_myfilestate_theater,

        status_file_myfilestate_other

    - added air and end date for animes

    	data_anime_startdate, data_anime_startdate_short

    	data_anime_enddate, data_anime_enddate_short

    - added romaji and kanji episode titles

    	data_ep_name_romaji, status_ep_hasname_romaji

    	data_ep_name_kanji, status_ep_hasname_kanji

    - added wishlist info to animes

    	status_anime_isinwishlist

    	data_anime_wishlist_type (0="unknown / unfiled", 1="to watch", 2="to get", 3="blacklist"),

    	data_anime_wishlist_type_name,

    	data_anime_wishlist_priority (0="low", 1="medium", 3="high"),

    	data_anime_wishlist_priority_name,

    	data_anime_wishlist_comment

    - added vote/tmpvote info for animes

    	status_anime_my_isvoted, data_anime_my_vote

    	status_anime_my_istmpvoted, data_anime_my_tmpvote

    - added id.txt with a list of anidb ids

    - new versions of some themes included

    - updated data dump

    - minor changes



  0.08 - 18.04.2004

    - added new tempvote info to anime data

        data_anime_tmprating, data_anime_tmpvotes

    - added ep vote data

        data_ep_rating, data_ep_votes

    - added file version string (v1, v2,...)

    	data_file_state_versionname

    - added additional redundant anime award info which can be accessed without using the loops

    	data_anime_awardicons, status_anime_hasawards

    - data files updated

    - IMPORTANT: new info.dat rules see templatehowto.txt



  0.07 - 11.04.2004

    - added info on new anime titles

    	data_anime_title_jap_kanji, status_anime_title_has_jap_kanji

    	data_anime_title_eng, status_anime_title_has_eng

    	data_anime_title_other, status_anime_title_has_other

    - status_ep_state_hanger is now always 0

    - added data_file_sha1

    - added award info

    	loop_anime_award_types

    		data_anime_award_type_id

    		data_anime_award_type_name

    		data_anime_award_type_img

    	loop_anime_awards

    		data_anime_award_id

    		data_anime_award_name

    		data_anime_award_url

    		data_anime_award_picurl

    		data_anime_award_type



  0.06 - 06.09.2003

    - added animeplanet info:

        data_anime_animeplanetid (not really usefull yet, 1 for all animes, this will change someday though)

        data_anime_animeplaneturl

        status_anime_animeplanet_listed

    - fixed bug in data_anime_my_unwatchedeps_special

    - fixed bug in data_ep_mystate_*



  0.05 - 26.08.2003

    - export.pl now copies any additional files found in a themes directory into the out directory

      you can use this to automatically include stylesheet or javascript files with your templates.

      however subdirectories are not copied and you should also not use this to include images.

      simply create a zip file contraining all images in the theme directory and i will upload it

      to the anidb http server and link to it. (faq updated)

    - jsencode now also strips newlines

    - new parsing function jsencodehtml which replaces:

        ' => &rsquo;    " => &quot;     \n => <br>

    - data_anime_my_eps now only lists normal eps, not specials. same goes for data_anime_watchedeps and data_anime_unwatchedeps

      additional data fields:

        data_anime_my_*_special   - number of special eps

        data_anime_my_*_total	  - total number, same as old data_anime_my_*

    - data_anime_my_unwatchedeps, data_anime_my_unwatchedeps_special, data_anime_my_unwatchedeps_total where undocumented so far

    - status_anime_iscomplete and status_anime_iswatched haven been updated to match the current mylist style of only

      taking normal eps into account.

    - status_ep_mystate_* and status_anime_mystate_* added to reflect the latest mylist changes on anidb

      this allows you now to include state icons on episode and anime level as anidb does it.

    - plaintext dump of the small data file is now included, take a look at datadump.txt if you need info on

      undocumented vars or just want to know what data format to expect. if you encounter a problem or unexpected data in

      some vars it might also be a good idea to take a look at this file.



  0.04b - 13.08.2003

    - status_file_state_* was not working correctly, fixed now.



  0.04 - 12.08.2003

    - switch to HTML::Template::Expr to support more interaction between templates and perl code

      you have much more options in the templates.

      !!! YOU WILL NEED TO INSTALL ADDITIONAL MODULES TO GET THE NEW SDK TO WORK !!! (see below)

      there are now additional commands available, i.e.:

      	<TMPL_VAR EXPR="data_file_size_plain/8"> would give you the filesize in kB (note the 'EXPR')

      	there are also a lot a predefined functions which can be used as

      	<TMPL_VAR EXPR="function('string param')"> or <TMPL_VAR EXPR="function(variable)">

      	multiple parameters are ',' seperated. i.e. <TMPL_VAR EXPR="function(var1,var2,'string')">

      	check the url below for predefined functions

      	additional functions are:

      	jsencode() - one parameter, replaced ' with \', " with \" and ´ with \´

      	replace() - 3 parameters (find, replacewith, string), i.e. ('a','b','abc') -> bbc

      	ireplace() - same as replace but case insensitive

      	additional functions can be added on request. the code is included with the SDK so you could also play

      	around with some of your own functions.

      for more details check: http://html-template.sourceforge.net/html_template_expr.html

    - added data_anime_animenfoid, data_anime_animenfoid2, data_anime_animenfourl, data_anime_animenfourl_name

      data_anime_animenfoid is the old animenfo id, data_anime_animenfoid2 is the additional new one,

      data_anime_animenfourl is a complete url (without <a href="">, just a plain url) to the animenfo page of this anime

      data_anime_animenfourl_name is the link name as displayed on anidb (i.e. "ID: 64" or "ID: 1235,rvvvjg")

    - data_file_size_plain contains plain size without dots, this is needed if you want to construct your own ed2k links

    - data_file_acodec_id, data_file_acodec_name, data_file_vcodec_id and data_file_vcodec_name were undocumented



  0.03 - 10.08.2003

    - fixed typo, the module needed is called Storable

    - added additional big data file which can be used to check templates on big mylists

      to use it rename tpl-alist.dat to i.e. tpl-alist.dat.small and rename tpl-alist.dat.big

      to tpl-alist.dat.



  0.02 - 02.08.2003

    - additional data values

	All:

	 * all data values now also have a _short version which ommits the time.

	   i.e. data_ep_update_short = 02.08.03

        Anime:

 	 * status_anime_genre_<genre> is set to 0 or 1 according to added genres

	   i.e. status_anime_genre_action = 1, if the anime has genre action added

	   note: genres are all lowercase and spaces are replaced with _

	 * status_anime_iscomplete and status_anime_iswatched slightly changed to

	   match the new way of doing things @ mylist @ anidb. (animes with epcount = 0

	   are no longer considered complete or completly watched)

	Ep:

	 * epno does now correctly handle specials (epno 1001 => S1)

	 * data_ep_state carries anidb internal state number

	   + EP_SPECIAL=1, EP_RECAP=2, EP_OP=4, EP_END=8, EP_HANGER=16

	     (hanger = cliffhanger)

	 * status_ep_state_special, status_ep_state_recap,

	   status_ep_state_op, status_ep_state_end, status_ep_state_hanger

	     are set according to data_ep_state so you might want to use these

	     instead of data_ep_state

	File:

	 * data_file_released is now "-" if no release date was added

	 * data_file_state carries anidb internal state number

	   + FILE_CRCOK=1, FILE_CRCERR=2, FILE_ISV2=4, FILE_ISV3=8,

	     FILE_ISV4=16, FILE_ISV5=32

	     values are added to generate the data_file_state field

	 * status_file_state_crcok, status_file_state_crcfailed,

	   status_file_state_isv2, status_file_state_isv3,

	   status_file_state_isv4, status_file_state_isv5

	     are set according to data_file_state so you might want to use these

	     instead of data_file_state

	 * data_file_mystate carries anidb internal mylist file state number

	   + unknown=0, on hdd=1, on cd=2, deleted=3, shared=4, release=5

	 * data_file_mystate_string gives the string representation

	 * data_file_mystate_icon gives the icon known from anidb (direct inet link to image,

	   img tag already included)

	 * status_file_mystate_unknown, status_file_mystate_onhdd, status_file_mystate_oncd,

	   status_file_mystate_deleted, status_file_mystate_shared, status_file_mystate_release

	     are set according to data_file_mystate



  0.01 - 08.07.2003

    - first version





Content:

1. About this SDK

2. Installing Perl



--------------------------------



1. About this SDK:

  This SDK is meant to allow you to test your templates for the anidb mylist export

  function on your own pc. It should work under Windows and Linux. You might need to install

  some additional software, see (2. Installing Perl).



--------------------------------



2. Installing Perl:

  This programm needs Perl to create example html files from the templates. If you don't have

  Perl installed (preferably 5.8.0) you'll need to do so now.



  Under Windows you'll need to download ActivePerl 5.8.0 from activeperl.com:

  	http://downloads.activestate.com/ActivePerl/Windows/5.8/ActivePerl-5.8.0.806-MSWin32-x86.msi

  Install it to c:\perl (or some other directory, but remember where you installed it!).

  After that you'll need to delete the file Storable.pm in the perl/lib folder and the entire

  Storable folder in perl/lib/auto. After that replace those with the files from the perl directory

  provided with this sdk.

  You should now be able to execute export.pl, take a look at the provided bat file to see how it is used.

  (export.pl takes one parameter which is the name of the template to use)



  Under Linux it should already be installed.

  You might have to update Sharable though, try the following:

  	perl -MCPAN -eshell

  	install Storable

  	bye



3. Additional Module HTML::Template::Expr

  Under Windows with ActivePerl go to it's startmenu entry (might be called "ActiveState ActivePerl 5.8")

  and start "Perl Package Manager". Then enter

  	install HTML::Template::Expr

  	exit



  Under Linux you have to do a:

  	perl -MCPAN -eshell

  	install HTML::Template::Expr

  	bye



--------------------------------



