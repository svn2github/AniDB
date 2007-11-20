/* *
 * @file Preferences script
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (03.07.2007)
 */

// template info
var theme = {'name':"html-fstyle",'version':"0.9.7beta",'source':"svn",'date':"20.11.2007"};
// settings
var animeTitleLang = getCookie('animeTitleLang') || 'x-jat';
var animeAltTitleLang = getCookie('animeAltTitleLang') || 'en';
var animeOfficialTitleLang = getCookie('animeOfficialTitleLang') || 'en';
var animeTitleDisplay = getCookie('animeTitleDisplay') || 2;
var episodeTitleLang = getCookie('episodeTitleLang') || 'en';
var episodeAltTitleLang = getCookie('episodeAltTitleLang') || 'x-jat';
var episodeTitleDisplay = getCookie('episodeTitleDisplay') || 2;
var entriesPerPage = getCookie('entriesPerPage') || 20;
var LAY_HEADER = (getCookie('LAY_HEADER') == 'true');
var LAY_NOANIMEGROUPREL = (getCookie('LAY_NOANIMEGROUPREL') == 'true');
var LAY_HIDEAWARDS = (getCookie('LAY_HIDEAWARDS') == 'true');
var LAY_ETLANGFLAGS = (getCookie('LAY_ETLANGFLAGS') == 'true');
var LAY_HIDEFILES = (getCookie('LAY_HIDEFILES') == 'true');
var LAY_HIDERAWS = (getCookie('LAY_HIDERAWS') == 'true');
var LAY_HIDEGENERICFILES = (getCookie('LAY_HIDEGENERICFILES') == 'true');
var LAY_HIDEPARODYEPS = (getCookie('LAY_HIDEPARODYEPS') == 'true');
var LAY_HIDEEPTITLES = (getCookie('LAY_HIDEEPTITLES') == 'true');
var LAY_PSEUDOFILES = (getCookie('LAY_PSEUDOFILES') == 'true');
var LAY_FORMATFILESIZE = (getCookie('LAY_FORMATFILESIZE') == 'true');
var LAY_FILTERFILES = (getCookie('LAY_FILTERFILES') == 'true');
var search_logic = 'OR';
var base_url = 'http://anidb.net/perl-bin/animedb.pl';

function changeOptionValue() {
	var name = (this.name == this.id ? this.name : this.id);
	var value = (this.type == 'checkbox' ? this.checked : this.value);
	setCookie(name,value);
}

function renderPage() {
	updateStatus('Rendering page...');
	var elems = document.getElementsByTagName('PREFERENCES.ANIME.TITLE.MAIN');
	while (elems.length) {
		var select = createLanguageSelect(null,'animeTitleLang','animeTitleLang',changeOptionValue,animeTitleLang);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.ANIME.TITLE.ALT');
	while (elems.length) {
		var select = createLanguageSelect(null,'animeAltTitleLang','animeAltTitleLang',changeOptionValue,animeAltTitleLang);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.ANIME.TITLE.OFFICIAL');
	while (elems.length) {
		var select = createLanguageSelect(null,'animeOfficialTitleLang','animeOfficialTitleLang',changeOptionValue,animeOfficialTitleLang);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.ANIME.TITLE.DISPLAY');
	while (elems.length) {
		var optionArray = { 0 : {"text":'Title'} , 1 : {"text":'Title (Alt. Title)'}, 2 : {"text":'Title [tooltip: Alt. Title]'}};
		var select = createSelectArray(null,'animeTitleDisplay','animeTitleDisplay',changeOptionValue,animeTitleDisplay,optionArray);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.EPISODE.TITLE.MAIN');
	while (elems.length) {
		var select = createLanguageSelect(null,'episodeTitleLang','episodeTitleLang',changeOptionValue,episodeTitleLang);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.EPISODE.TITLE.ALT');
	while (elems.length) {
		var select = createLanguageSelect(null,'episodeAltTitleLang','episodeAltTitleLang',changeOptionValue,episodeAltTitleLang);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.EPISODE.TITLE.DISPLAY');
	while (elems.length) {
		var optionArray = { 0 : {"text":'Title'} , 1 : {"text":'Title (Alt. Title)'}, 2 : {"text":'Title [tooltip: Alt. Title]'}};
		var select = createSelectArray(null,'episodeTitleDisplay','episodeTitleDisplay',changeOptionValue,episodeTitleDisplay,optionArray);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.EPP');
	while (elems.length) {
		var optionArray = {10:{"text":'10'},20:{"text":'20'},30:{"text":'30'},50:{"text":'50'},
											 100:{"text":'100'},250:{"text":'250'},500:{"text":'500'},1000:{"text":'1000'}};
		var select = createSelectArray(null,'entriesPerPage','entriesPerPage',changeOptionValue,entriesPerPage,optionArray);
		elems[0].parentNode.replaceChild(select,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_HEADER');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_HEADER',changeOptionValue,LAY_HEADER);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_NOANIMEGROUPREL');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_NOANIMEGROUPREL',changeOptionValue,LAY_NOANIMEGROUPREL);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_HIDEAWARDS');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_HIDEAWARDS',changeOptionValue,LAY_HIDEAWARDS);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_ETLANGFLAGS');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_ETLANGFLAGS',changeOptionValue,LAY_ETLANGFLAGS);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_HIDERAWS');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_HIDERAWS',changeOptionValue,LAY_HIDERAWS);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_HIDEGENERICFILES');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_HIDEGENERICFILES',changeOptionValue,LAY_HIDEGENERICFILES);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_HIDEEPTITLES');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_HIDEEPTITLES',changeOptionValue,LAY_HIDEEPTITLES);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_HIDEPARODYEPS');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_HIDEPARODYEPS',changeOptionValue,LAY_HIDEPARODYEPS);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_PSEUDOFILES');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_PSEUDOFILES',changeOptionValue,LAY_PSEUDOFILES);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_FORMATFILESIZE');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_FORMATFILESIZE',changeOptionValue,LAY_FORMATFILESIZE);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	elems = document.getElementsByTagName('PREFERENCES.DISPLAY.LAY_FILTERFILES');
	while (elems.length) {
		var ck = createCheckBox(null,'LAY','LAY_FILTERFILES',changeOptionValue,LAY_FILTERFILES);
		elems[0].parentNode.replaceChild(ck,elems[0]);
	}
	updateStatus('');
}

var uriObj = parseURI();
if (uriObj['basename'].indexOf('preferences') >= 0) addLoadEvent(renderPage);