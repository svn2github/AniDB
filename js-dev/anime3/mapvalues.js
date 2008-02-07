/**
 * @file MapValues interface
 * @author fahrenheit (alka.setzer@gmail.com)
 *         Based on code from PetriW's work at anidb
 * @version 2.1 (22.03.2007)
 */
 
 // CORE Maps //
 
 /**
  * Function that maps file states to textual values
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

/**
 * Function to convert a mylistEntry status id to a status name
 * @param statId Status id to convert to text
 * @return String containing status name
 */
function mapMEStatusName(statusName) {
  if (statusName.indexOf('internal') >= 0) return ('onhdd');
  else if (statusName.indexOf('external') >= 0) return ('oncd');
  else if (statusName.indexOf('deleted') >= 0) return ('deleted');
  else if (statusName.indexOf('unknown') >= 0) return ("unknown");
}

var languageMap = {'x-unk':{"name":'unknown',"classname":'unknown'},
                   'ja':{"name":'japanese',"classname":'japanese'},
                   'zh':{"name":'chinise (unspecified)',"classname":'chinese'},
                   'en':{"name":'english',"classname":'english'},
                   'de':{"name":'german',"classname":'german'},
                   'fr':{"name":'french',"classname":'french'},
                   'it':{"name":'italian',"classname":'italian'},
                   'es':{"name":'spanish',"classname":'spanish'},
                   'ru':{"name":'russian',"classname":'russian'},
                   'ko':{"name":'korean',"classname":'korean'},
                   'pl':{"name":'polish',"classname":'polish'},
                   'sv':{"name":'swedish',"classname":'swedish'},
                   'ar':{"name":'arabic',"classname":'arabic'},
                   'pt':{"name":'portuguese',"classname":'portuguese'},
                   'no':{"name":'norwegian',"classname":'norwegian'},
                   'pt-BR':{"name":'brazilian portuguese',"classname":'brasilian'},
                   'ca':{"name":'catalan',"classname":'catalan'},
                   'cs':{"name":'czech',"classname":'czech'},
                   'da':{"name":'danish',"classname":'danish'},
                   'nl':{"name":'dutch',"classname":'dutch'},
                   'fi':{"name":'finnish',"classname":'finnish'},
                   'el':{"name":'greek',"classname":'greek'},
                   'he':{"name":'hebrew',"classname":'hebrew'},
                   'hu':{"name":'hungarian',"classname":'hungarian'},
                   'x-in':{"name":'instrumental',"classname":'instrumental'},
                   'lt':{"name":'lithuanian',"classname":'lithuanian'},
                   'th':{"name":'thai',"classname":'thai'},
                   'tr':{"name":'turkish',"classname":'turkish'},
                   'zh-x-cmn':{"name":'chinese (mandarin)',"classname":'mandarin'},
                   'zh-x-yue':{"name":'chinese (cantonese)',"classname":'cantonese'},
                   'zh-x-nan':{"name":'chinese (taiwanese)',"classname":'taiwanese'},
                   'x-other':{"name":'other',"classname":'unknown'},
                   'ro':{"name":'romanian',"classname":'romanian'},
                   'my':{"name":'malay',"classname":'malay'},
                   'sk':{"name":'slovak',"classname":'slovak'},
                   'sl':{"name":'slovenian',"classname":'slovenian'},
                   'tt':{"name":'tatar',"classname":'tartar'},
                   'lv':{"name":'latvian',"classname":'latvian'},
                   'ka':{"name":'georgian',"classname":'georgian'},
                   'la':{"name":'latin',"classname":'latin'},
                   'vi':{"name":'vietnamese',"classname":'vietnamese'},
                   'id':{"name":'indonesian',"classname":'indonesian'},
                   'ta':{"name":'tamil',"classname":'tamil'},
                   'bg':{"name":'bulgarian',"classname":'bulgarian'},
                   'x-jat':{"name":'japanese (transcription)',"classname":'japanese-transcription'},
                   'et':{"name":'estonian',"classname":'estonian'},
                   'uk':{"name":'ukrainian',"classname":'ukrainian'},
                   'hr':{"name":'croatian',"classname":'croatian'},
                   'sr':{"name":'serbian',"classname":'serbian'},
                   'zh-Hant':{"name":'chinese (traditional)',"classname":'traditional'},
                   'zh-Hans':{"name":'chinese (simplified)',"classname":'simplified'},
                   'is':{"name":'icelandic',"classname":'icelandic'}};

/**
 * Function that maps a LANGCODE to a language
 * @param langCode The language code to convert
 * @return Name of the language
 */
function mapLanguage(langCode) {
  if (languageMap[langCode]) return (languageMap[langCode]['name']);
  else return ('unknown');
}
/**
 * Function that maps a LANGCODE to a language used in the icons
 * @param langCode The language code to convert
 * @return Name of the language
 */
function mapClassLanguage(langCode) {
  if (languageMap[langCode]) return (languageMap[langCode]['classname']);
  else return ('unknown');
}

/**
 * Converts a subtitle stream flags in the textual relation
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

/**
 * Converts a video stream flags in the textual relation
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

/**
 * Converts a subtitle types in the textual relation
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

/**
 * Converts a audioChannel id in the textual relation
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

/**
 * Converts numeric audio types in a textual relation
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

/**
 * This should not be needed, and wasn't needed till *someone* changed the system...
 * @param qual Quality
 * @return Quality className
 */
function mapQualityClassName(qual) {
  switch (qual) {
    case 'veryhigh': return ("vhigh");
    case 'medium': return ("med");
    case 'low': return ("low");
    case 'verylow': return ("vlow");
    default: return (qual);
  }
  return ("unknown");
}

/**
 * This should not be needed, and wasn't needed till *someone* changed the system...
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

/**
 * Function that returns the anime type
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
  }
  return ('unknown');
}
