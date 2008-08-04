/* file MapValues interface
 * @author fahrenheit (alka.setzer@gmail.com)
 *         Based on code from PetriW's work at anidb
 * @version 2.1 (22.03.2007)
 */
 
 // CORE Maps //
 
 /* Function that maps file states to textual values
  * @param fstate The fstate to convert
  * @return String of the textual converted value
  */
function mapFState(fstate) {
	var stClass = 'i_filestate_'+fstate.split(' ').join('');
	if (fstate.indexOf('/') >= 0) stClass = 'i_filestate_'+fstate.split(' ')[0]; // special cases
	if (fstate == 'corrupted version/invalid crc') stClass = 'i_filestate_corrupted';
	if (fstate == 'self edited') stClass = 'i_filestate_edited';
	return stClass;
}

/* Function to convert a mylistEntry status id to a status name
 * @param statId Status id to convert to text
 * @return String containing status name
 */
function mapMEStatusName(statusName) {
	if (statusName.indexOf('internal') >= 0) return ('onhdd');
	else if (statusName.indexOf('external') >= 0) return ('oncd');
	else if (statusName.indexOf('deleted') >= 0) return ('deleted');
	else if (statusName.indexOf('unknown') >= 0) return ("unknown");
}

var languageMap = {'x-unk':{"name":'unknown',"classname":'unknown',"id":'1'},
                   'ja':{"name":'japanese',"classname":'japanese',"id":'2'},
                   'zh':{"name":'chinese (unspecified)',"classname":'chinese',"id":'3'},
                   'en':{"name":'english',"classname":'english',"id":'4'},
                   'de':{"name":'german',"classname":'german',"id":'5'},
                   'fr':{"name":'french',"classname":'french',"id":'6'},
                   'it':{"name":'italian',"classname":'italian',"id":'8'},
                   'es':{"name":'spanish',"classname":'spanish',"id":'9'},
                   'ru':{"name":'russian',"classname":'russian',"id":'15'},
                   'ko':{"name":'korean',"classname":'korean',"id":'17'},
                   'pl':{"name":'polish',"classname":'polish',"id":'18'},
                   'sv':{"name":'swedish',"classname":'swedish',"id":'19'},
                   'ar':{"name":'arabic',"classname":'arabic',"id":'20'},
                   'pt':{"name":'portuguese',"classname":'portuguese',"id":'21'},
                   'no':{"name":'norwegian',"classname":'norwegian',"id":'23'},
                   'pt-BR':{"name":'brazilian portuguese',"classname":'brasilian',"id":'25'},
                   'ca':{"name":'catalan',"classname":'catalan',"id":'26'},
                   'cs':{"name":'czech',"classname":'czech',"id":'27'},
                   'da':{"name":'danish',"classname":'danish',"id":'28'},
                   'nl':{"name":'dutch',"classname":'dutch',"id":'29'},
                   'fi':{"name":'finnish',"classname":'finnish',"id":'30'},
                   'el':{"name":'greek',"classname":'greek',"id":'31'},
                   'he':{"name":'hebrew',"classname":'hebrew',"id":'32'},
                   'hu':{"name":'hungarian',"classname":'hungarian',"id":'33'},
                   'x-in':{"name":'instrumental',"classname":'instrumental',"id":'34'},
                   'lt':{"name":'lithuanian',"classname":'lithuanian',"id":'35'},
                   'th':{"name":'thai',"classname":'thai',"id":'37'},
                   'tr':{"name":'turkish',"classname":'turkish',"id":'38'},
                   'zh-x-cmn':{"name":'chinese (mandarin)',"classname":'mandarin',"id":'41'},
                   'zh-x-yue':{"name":'chinese (cantonese)',"classname":'cantonese',"id":'42'},
                   'zh-x-nan':{"name":'chinese (taiwanese)',"classname":'taiwanese',"id":'43'},
                   'x-other':{"name":'other',"classname":'unknown',"id":'58'},
                   'ro':{"name":'romanian',"classname":'romanian',"id":'61'},
                   'my':{"name":'malay',"classname":'malay',"id":'62'},
                   'sk':{"name":'slovak',"classname":'slovak',"id":'63'},
                   'sl':{"name":'slovenian',"classname":'slovenian',"id":'64'},
                   'tt':{"name":'tatar',"classname":'tartar',"id":'65'},
                   'lv':{"name":'latvian',"classname":'latvian',"id":'67'},
                   'ka':{"name":'georgian',"classname":'georgian',"id":'68'},
                   'la':{"name":'latin',"classname":'latin',"id":'69'},
                   'vi':{"name":'vietnamese',"classname":'vietnamese',"id":'70'},
                   'id':{"name":'indonesian',"classname":'indonesian',"id":'71'},
                   'ta':{"name":'tamil',"classname":'tamil',"id":'73'},
                   'bg':{"name":'bulgarian',"classname":'bulgarian',"id":'74'},
                   'x-jat':{"name":'japanese (transcription)',"classname":'japanese-transcription',"id":'75'},
                   'et':{"name":'estonian',"classname":'estonian',"id":'76'},
                   'uk':{"name":'ukrainian',"classname":'ukrainian',"id":'77'},
                   'hr':{"name":'croatian',"classname":'croatian',"id":'78'},
                   'sr':{"name":'serbian',"classname":'serbian',"id":'79'},
                   'zh-Hant':{"name":'chinese (traditional)',"classname":'traditional',"id":'80'},
                   'zh-Hans':{"name":'chinese (simplified)',"classname":'simplified',"id":'81'},
                   'is':{"name":'icelandic',"classname":'icelandic',"id":'84'},
                   'al':{"name":'albanian',"classname":'albanian',"id":'85'}};

var codecMap = {'unk':{"name":'unknown'},
				'other':{"name":'Other (non-ASP)'},
				'avc':{"name":'H264/AVC'},
				'xvid':{"name":'XviD'},
				'asp':{"name":'ASP Other'},
				'divxu':{"name":'DivX UNK'},
				'divx3':{"name":'DivX3'},
				'divx4':{"name":'DivX4'},
				'divx5':{"name":'DivX5 (also DivX6)'},
				'mpeg1':{"name":'MPEG-1'},
				'mpeg2':{"name":'MPEG-2'},
				'mp4x':{"name":'MS MP4x (also WMV1/2)'},
				'rvo':{"name":'RealVideo Other'},
				'rv40':{"name":'RealVideo 9/10 (also RV40)'},
				'wmv9':{"name":'WMV9 (also WMV3)'},
				'aac':{"name":'AAC'},
				'ac3':{"name":'AC3'},
				'dts':{"name":'DTS'},
				'mp2':{"name":'MP2'},
				'mp3cbr':{"name":'MP3 CBR'},
				'mp3vbr':{"name":'MP3 VBR'},
				'msaud':{"name":'MSAudio'},
				'pcm':{"name":'PCM'},
				'vorbis':{"name":'Vorbis (Ogg Vorbis)'},
				'wma':{"name":'WMA (also DivX Audio)'}};

/* Function that maps a LANGCODE to a language
 * @param langCode The language code to convert
 * @return Name of the language
 */
function mapLanguage(langCode) {
	if (languageMap[langCode]) return (languageMap[langCode]['name']);
	else return ('unknown');
}
/* Function that maps a CODEC sname to a long version
 * @param langCode The language code to convert
 * @return Name of the language
 */
function mapCodec(codecSNAME) {
	if (codecMap[codecSNAME]) return (codecMap[codecSNAME]['name']);
	else return ('unknown');
}
/* Function that maps a LANGCODE to a language used in the icons
 * @param langCode The language code to convert
 * @return Name of the language
 */
function mapClassLanguage(langCode) {
	if (languageMap[langCode]) return (languageMap[langCode]['classname']);
	else return ('unknown');
}

/* Converts a subtitle stream flags in the textual relation
 * @param subFlags Int representing subtitle flags
 * @return String giving text based subtitle flags
 */
function mapSubFlagData(subFlags) {
	if (!subFlags) return '';
	var subData = new Array();
	if (subFlags & 1) subData.push("karaoke subbed");
	if (subFlags & 2) subData.push("signs translated");
	if (subFlags & 4) subData.push("additional explanations");
	if (subFlags & 8) subData.push("dubsubbed");
	if (subFlags & 16) subData.push("hearing impaired subs");
	if (subFlags & 32) subData.push("subs for commentary audio stream");
	if (subFlags & 64) subData.push("styled subs (ASS/SSA)");
	if (subFlags & 128) subData.push("image subs (VOBSUB)");
	if (subFlags & 256) subData.push("unstyled subs (SRT)");
	return subData.join(', ');
}

/* Converts a video stream flags in the textual relation
 * @param videoFlags Int representing video flags
 * @return String giving text based video flags
 */
function mapVideoFlagsInfo(videoFlags) {
	if (!videoFlags) return '';
		var videoInfo = new Array();
		if (videoFlags & 1) videoInfo.push("anamorphic");
		if (videoFlags & 2) videoInfo.push("wrong aspect ratio");
		if (videoFlags & 4) videoInfo.push("variable frame rate");
		if (videoFlags & 8) videoInfo.push("clean video (no hardsubs...)");
	return videoInfo.join(', ');
}

/* Converts a subtitle types in the textual relation
 * @param subType Int representing sub type
 * @return String giving text based sub type
 */
function mapSubTypeData(subType) {
	switch (subType) {
		case 'hard': return ("hard sub");
		case 'soft': return ("soft sub");
		case 'sup. soft': return ("supplementary soft sub");
		default: return (subType);
	}
	return (subType);
}

/* Converts a audioChannel id in the textual relation
 * @param audioChannels Int representing stream aud chan
 * @return String giving text based channel type
 */
function mapAudioChannels(audioChannels) {
	switch (audioChannels) {
		case '1.0': return ("mono");
		case '2.0': return ("stereo");
		case '4.0': return ("4.0 - dolby surround");
		case '5.1': return ("5.1 - surround");
		case '6.1': return ("6.1 - surround");
		case '7.1': return ("7.1 - surround");
		default: return (audioChannels);
	}
	return (audioChannels);
}

/* Converts numeric audio types in a textual relation
 * @param type Type of audio file
 * @return String containing text based audio type
 */
function mapAudioType(type) {
	switch (type) {
		case 'normal': return ("normal audio");
		case 'alt. voiceover': return ("alternative voiceover");
		default: return (type);
	}
	return (type);     
}

/* This should not be needed, and wasn't needed till *someone* changed the system...
 * @param qual Quality
 * @return Quality className
 */
function mapQualityClassName(qual) {
	switch (qual) {
		case 'veryhigh': return ("vhigh");
		case 'medium': return ("med");
		case 'verylow': return ("vlow");
		default: return (qual);
	}
	return ("unknown");
}

/* This should not be needed, and wasn't needed till *someone* changed the system...
 * @param qual Quality
 * @return Quality className
 */
function mapQuality(qualClassName) {
	switch (qualClassName) {
		case 'very high': return (8);
		case 'high': return (7);
		case 'med': return (6);
		case 'low': return (5);
		case 'very low': return (4);
		case 'corrupted': return (3);
		case 'eyecancer': return (2);
		case 'unknown': return (1);
		}
	return (1);
}

/* Function that returns the anime type
 * @param type Anime type id
 * @return String with anime type
 */
function mapAnimeType(type) {
	switch (type) {
		case 1: return ("unknown");
		case 2: return ("TV Series");
		case 3: return ("OVA");
		case 4: return ("Movie");
		case 5: return ("Other");
		case 6: return ("Web");
		case 7: return ("TV Special");
		case 8: return ("Music Video");
	}
	return ('unknown');
}

/* Function that returns an absolute episode number
 * @param epno String with episode number
 * @return Int value of episode
 */
function mapEpisodeNumber(epno) {
	var firstChar = String(epno);
	var result = 0;
	switch (firstChar[0]) {
		case 'S': result = '1' + epno.substring(1,epno.length); break;
		case 'C': result = '2' + epno.substring(1,epno.length); break;
		case 'T': result = '3' + epno.substring(1,epno.length); break;
		case 'P': result = '4' + epno.substring(1,epno.length); break;
		case 'O': result = '10' + epno.substring(1,epno.length); break;
		default: result = epno;
	}
	return (!isNaN(Number(result)) ? Number(result) : 0);
}

/* Function that returns the textual version of a numeral epno
 * @param epno Int with numeral episode number
 * @return String vale of episode number
 */
function mapReverseEpisodeNumber(epno) {
	var nepno = Number(epno);
	if (isNaN(nepno)) return '';
	epno = String(epno);
	if (nepno < 1000) return Number(epno);
	if (nepno >= 1000 && nepno < 2000) return ('S' + Number(epno.substring(1,epno.length)));
	if (nepno >= 2000 && nepno < 3000) return ('C' + Number(epno.substring(1,epno.length)));
	if (nepno >= 3000 && nepno < 4000) return ('T' + Number(epno.substring(1,epno.length)));
	if (nepno >= 4000 && nepno < 10000) return ('P' + Number(epno.substring(1,epno.length)));
	if (nepno > 10000) return ('O' + Number(epno.substring(1,epno.length)));
}

/* Function that returns a string with the mylist state
 *@param stateId Id of the state
 *@return String with mylist state
 */
function mapMylistState(stateId) {
	var stateId = Number(stateId);
	switch (stateId) {
		case 1: return ("internal storage (hdd)"); break;
		case 2: return ("external storage (cd/dvd/...)"); break;
		case 3: return ("deleted"); break;
		default: return("unknown"); break;
	}
	return "unknown";
}

