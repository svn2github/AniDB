/* file Preferences script
 * author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (03.07.2007)
 */

// template info
var theme = {'name':"html-xstyle",'version':"0.1.0beta",'source':"svn",'date':"08.08.2008"};
// settings
var animeTitleLang = CookieGet('animeTitleLang') || 'x-jat';
var animeAltTitleLang = CookieGet('animeAltTitleLang') || 'en';
var animeOfficialTitleLang = CookieGet('animeOfficialTitleLang') || 'en';
var animeTitleDisplay = CookieGet('animeTitleDisplay') || 2;
var episodeTitleLang = CookieGet('episodeTitleLang') || 'en';
var episodeAltTitleLang = CookieGet('episodeAltTitleLang') || 'x-jat';
var episodeTitleDisplay = CookieGet('episodeTitleDisplay') || 2;
var entriesPerPage = Number(CookieGet('entriesPerPage')) || 20;
var LAY_HEADER = (CookieGet('LAY_HEADER') == 'true');
var LAY_NOANIMEGROUPREL = (CookieGet('LAY_NOANIMEGROUPREL') == 'true');
var LAY_HIDEAWARDS = (CookieGet('LAY_HIDEAWARDS') == 'true');
var LAY_ETLANGFLAGS = (CookieGet('LAY_ETLANGFLAGS') == 'true');
var LAY_HIDEFILES = (CookieGet('LAY_HIDEFILES') == 'true');
var LAY_HIDERAWS = (CookieGet('LAY_HIDERAWS') == 'true');
var LAY_HIDEGENERICFILES = (CookieGet('LAY_HIDEGENERICFILES') == 'true');
var LAY_HIDEPARODYEPS = (CookieGet('LAY_HIDEPARODYEPS') == 'true');
var LAY_HIDEEPTITLES = (CookieGet('LAY_HIDEEPTITLES') == 'true');
var LAY_PSEUDOFILES = (CookieGet('LAY_PSEUDOFILES') == 'true');
var LAY_FORMATFILESIZE = (CookieGet('LAY_FORMATFILESIZE') == 'true');
var LAY_FILTERFILES = (CookieGet('LAY_FILTERFILES') == 'true');
var LAY_SHOWCRC = (CookieGet('LAY_SHOWCRC') == 'true');
var LAY_SHOWFID = (CookieGet('LAY_SHOWFID') == 'true');
var LAY_SHOWTRADE = (CookieGet('LAY_SHOWTRADE') == 'true');
var LAY_NOEPTB = (CookieGet('LAY_NOEPTB') == 'true');
var LAY_FILEMODE = CookieGet('LAY_FILEMODE') || 0;
var search_logic = 'OR';
var base_url = 'http://anidb.net/perl-bin/animedb.pl';

function changeOptionValue() {
	var name = (this.name == this.id ? this.name : this.id);
	var value = (this.type == 'checkbox' ? this.checked : this.value);
	setCookie(name,value);
}
