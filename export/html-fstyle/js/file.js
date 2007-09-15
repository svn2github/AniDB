/* *
 * @file FilePage interface 
 * @author fahrenheit (alka.setzer@gmail.com)
 * @version 1.0 (10.09.2007)
 */
 
// GLOBALs //

var seeDebug = false;
var seeTimes = false;
var cloneTbody = null;
var vidRow = audRow = subRow = null;

/* *
 * Calls data handlers for xml data
 * @param xmldoc the xml document retreived by xmlhttprequest
 */
function parseData(xmldoc) {
  updateStatus('parsing Custom node');
  var root = xmldoc.getElementsByTagName('root').item(0);
  base_url = root.getAttribute('anidb');
  var t1 = new Date();
  parseCustom(root.getElementsByTagName('custom').item(0));
  var parseCustomNode = (new Date()) - t1;
  replaceGlobals(document.body);
  updateStatus('Processing anime...');
  t1 = new Date();
  parseAnimes(root.getElementsByTagName('animes'));
  var parseAnimeNode = (new Date()) - t1;
  updateStatus('Processing user information...');
  if (seeTimes) { alert('Processing...'+
        '\n\tanime: '+parseAnimeNode+'ms'+
        '\n\tcustom: '+parseCustomNode+' ms'+
        '\nTotal: '+(parseAnimeNode+parseCustomNode)+' ms'); }
  updateStatus('');
  renderPage();
}

function renderPage() {
	updateStatus('Rendering page...');
	var anime = animes[uriObj['aid']];
	var episode = episodes[uriObj['eid']];
	var file = files[uriObj['fid']];
	var elems = document.getElementsByTagName('ANIME.TITLE.MAIN');
	while (elems.length) {
		var a = createLink(null, anime.title, 'anime.html?show=anime&aid='+file.animeId, null, null, null, 'short_link');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	var elems = document.getElementsByTagName('EPISODE.TITLE.MAIN');
	while (elems.length) {
		var a = createLink(null, episode.epno + ' - ' + episode.getTitle(), 'episode.html?show=ep&eid='+file.episodeId+'&aid='+file.animeId, null, null, null, 'short_link');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	var elems = document.getElementsByTagName('EPISODE.NUMBER');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(episode.epno),elems[0]);
	elems = document.getElementsByTagName('FILE.ALINK');
	while (elems.length) { 
		var a = createLink(null, 'f'+file.id, 'http://anidb.net/f'+file.id, null, null, null, 'short_link');
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	elems = document.getElementsByTagName('FILE.OTHER');
	while (elems.length) {
		var text;
		if (!file.other.length) {
			text = document.createElement('I');
			text.appendChild(document.createTextNode('None'));
		} else text = document.createTextNode(file.other);
		elems[0].parentNode.replaceChild(text,elems[0]);
	}
	elems = document.getElementsByTagName('FILE.LENGTH');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(file.length),elems[0]);
	elems = document.getElementsByTagName('FILE.SIZE');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(formatFileSize(file.size,false)),elems[0]);
	elems = document.getElementsByTagName('FILE.DURATION');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(formatFileLength(file.length,'long')),elems[0]);
	elems = document.getElementsByTagName('FILE.DATE');
	while (elems.length) {
		var text = (file.dates['rel'] != '-') ? 'on ' + cTimeDate(file.dates['rel']) : 'added on ' + cTimeDate(file.dates['add']);
		elems[0].parentNode.replaceChild(document.createTextNode(text),elems[0]);
	}
	elems = document.getElementsByTagName('FILE.CENSORED');
	while (elems.length) {
		var text = 'Unknown / does not apply';
		if (file.isCensored) text = 'Censored version';
		if (file.isUncensored) text = 'Uncensored version';
		elems[0].parentNode.replaceChild(document.createTextNode(text),elems[0]);
	}
	elems = document.getElementsByTagName('FILE.GROUP.NAME');
	while (elems.length) {
		var a = createLink(null, groups[file.groupId].shortName, 'http://anidb.net/g'+file.groupId, null, null, null, null);
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	elems = document.getElementsByTagName('FILE.GROUP.RELEASE');
	while (elems.length) {
		var a = createLink(null, 'release', base_url+'show=group&gid='+file.groupId+'&aid='+file.animeId, null, null, null, null);
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	elems = document.getElementsByTagName('FILE.QUALITY');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(file.quality),elems[0]);
	elems = document.getElementsByTagName('FILE.CRC.HASH');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(file.crc32),elems[0]);
	elems = document.getElementsByTagName('FILE.CRC.STATUS');
	while (elems.length) { 
		var text = 'not checked against official crc';
		if (file.crcStatus == 'valid') text = 'matches official crc';
		else if (file.crcStatus == 'invalid') text = 'DOES NOT match official crc';
		elems[0].parentNode.replaceChild(document.createTextNode(text),elems[0]);
	}
	elems = document.getElementsByTagName('FILE.ED2K.HASH.LINK');
	while (elems.length) {
		elems[0].parentNode.onmouseover = createHashLink;
		elems[0].parentNode.parentNode.id = 'fid_'+file.id;
		var a = createLink(null, file.ed2k, "!fillme!", null, null, null, null);
		elems[0].parentNode.replaceChild(a,elems[0]);
	}
	elems = document.getElementsByTagName('FILE.MD5.HASH');
	while (elems.length) {
		var text;
		if (file.md5) text = document.createTextNode(file.md5);
		else {
			text = document.createElement('I');
			text.appendChild(document.createTextNode('Unknown'));
		}
		elems[0].parentNode.replaceChild(text,elems[0]);
	}
	elems = document.getElementsByTagName('FILE.SHA1.HASH');
	while (elems.length) {
		var text;
		if (file.sha1) text = document.createTextNode(file.sha1);
		else {
			text = document.createElement('I');
			text.appendChild(document.createTextNode('Unknown'));
		}
		elems[0].parentNode.replaceChild(text,elems[0]);
	}
	elems = document.getElementsByTagName('FILE.TTH.HASH');
	while (elems.length) {
		var text;
		if (file.tth) text = document.createTextNode(file.tth);
		else {
			text = document.createElement('I');
			text.appendChild(document.createTextNode('Unknown'));
		}
		elems[0].parentNode.replaceChild(text,elems[0]);
	}
	elems = document.getElementsByTagName('FILE.TYPE');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(file.type),elems[0]);
	elems = document.getElementsByTagName('FILE.EXTENSION');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(file.fileType),elems[0]);
	elems = document.getElementsByTagName('FILE.SOURCE');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(file.source),elems[0]);
	elems = document.getElementsByTagName('FILE.VERSION');
	while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(file.version.replace('v','Version ')),elems[0]);
	elems = document.getElementsByTagName('FILE.RESOURCES');
	while (elems.length) {
		var div = document.createElement('DIV');
		div.className = 'icons';
		createIcon(div, 'mylist', 'anime.html?show=anime&aid='+anime.id, null, 'back to anime', 'i_backtomylist');
		createIcon(div, 'anidb', 'http://anidb.net/e'+episode.id, null, 'link to episode page at anidb', 'i_anidb');
		elems[0].parentNode.replaceChild(div,elems[0]);
	}
	var div = getElementsByClassName(document.getElementsByTagName('DIV'),'track_vid',false)[0];
	if (file.videoTracks.length) { // In case there are video streams
		if (div) {
			var table = div.getElementsByTagName('TABLE')[0];
			if (table) {
				cloneTbody = document.createElement('TBODY');
				cloneTbody.appendChild(table.tBodies[0].rows[0]);
				var vidRow = cloneTbody.rows[0].cloneNode(true);
				for (var i = 0; i < file.vidCnt; i++) {
					var row = vidRow.cloneNode(true);
					var stream = file.videoTracks[i];
					elems = row.getElementsByTagName('FILE.VID.STREAM.NUM');
					while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(i+1),elems[0]);
					if (stream) {
						elems = row.getElementsByTagName('FILE.VID.STREAM.RESOLUTION');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(stream.resolution),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.AR');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(stream.ar),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.FPS');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(stream.fps),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.CODEC');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(stream.codec),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.BITRATE');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(stream.bitrate+' kBit/s'),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.FLAGS');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(mapVideoFlagsInfo(stream.flags)),elems[0]);
					} else {
						elems = row.getElementsByTagName('FILE.VID.STREAM.RESOLUTION');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.AR');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.FPS');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.CODEC');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.BITRATE');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.FLAGS');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
					}
					table.tBodies[0].appendChild(row);
				}
			}
		}
	} else if (div) div.parentNode.removeChild(div);
	div = getElementsByClassName(document.getElementsByTagName('DIV'),'track_aud',false)[0];
	if (file.audioTracks.length) { // In case there are audio streams
		// getting the row for cloning
		if (div) {
			var table = div.getElementsByTagName('TABLE')[0];
			if (table) {
				cloneTbody = document.createElement('TBODY');
				cloneTbody.appendChild(table.tBodies[0].rows[0]);
				var audRow = cloneTbody.rows[0].cloneNode(true);
				for (var i = 0; i < file.audCnt; i++) {
					var row = audRow.cloneNode(true);
					var stream = file.audioTracks[i];
					elems = row.getElementsByTagName('FILE.AUD.STREAM.NUM');
					while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(i+1),elems[0]);
					if (stream) {
						elems = row.getElementsByTagName('FILE.AUD.STREAM.CHANNELS');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(mapAudioChannels(stream.chan)),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.LANGUAGE.NAME');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(mapLanguage(stream.lang)),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.LANGUAGE.ICON');
						while (elems.length) elems[0].parentNode.replaceChild(createIcon(null, 'aud'+i+'.'+stream.lang+' ', null, null, 'audio language: '+mapLanguage(stream.lang), 'i_audio_'+stream.lang),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.CODEC');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(stream.codec),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.BITRATE');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(stream.bitrate+' kBit/s'),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.TYPE');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(stream.type),elems[0]);
					} else {
						elems = row.getElementsByTagName('FILE.AUD.STREAM.CHANNELS');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.LANGUAGE.NAME');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.LANGUAGE.ICON');
						while (elems.length) elems[0].parentNode.replaceChild(createIcon(null, 'aud'+i+'.x-unk ', null, null, 'audio language: '+mapLanguage('x-unk'), 'i_audio_x-unk'),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.CODEC');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.BITRATE');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.AUD.STREAM.TYPE');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
					}
					table.tBodies[0].appendChild(row);
				}
			}
		}
	} else if (div) div.parentNode.removeChild(div);
	div = getElementsByClassName(document.getElementsByTagName('DIV'),'track_sub',false)[0];
	if (file.subtitleTracks.length) { // In case there are sub streams
		// getting the row for cloning
		if (div) {
			var table = div.getElementsByTagName('TABLE')[0];
			if (table) {
				cloneTbody = document.createElement('TBODY');
				cloneTbody.appendChild(table.tBodies[0].rows[0]);
				var subRow = cloneTbody.rows[0].cloneNode(true);
				for (var i = 0; i < file.subCnt; i++) {
					var row = subRow.cloneNode(true);
					var stream = file.subtitleTracks[i];
					elems = row.getElementsByTagName('FILE.SUB.STREAM.NUM');
					while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(i+1),elems[0]);
					if (stream) {
						elems = row.getElementsByTagName('FILE.SUB.STREAM.LANGUAGE.NAME');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(mapLanguage(stream.lang)),elems[0]);
						elems = row.getElementsByTagName('FILE.SUB.STREAM.LANGUAGE.ICON');
						while (elems.length) elems[0].parentNode.replaceChild(createIcon(null, 'sub'+i+'.'+stream.lang+' ', null, null, 'sub language: '+mapLanguage(stream.lang), 'i_sub_'+stream.lang),elems[0]);
						elems = row.getElementsByTagName('FILE.VID.STREAM.FLAGS');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(mapSubFlagData(stream.flags)),elems[0]);
						elems = row.getElementsByTagName('FILE.SUB.STREAM.TYPE');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(stream.type),elems[0]);
					} else {
						elems = row.getElementsByTagName('FILE.SUB.STREAM.LANGUAGE.NAME');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
						elems = row.getElementsByTagName('FILE.SUB.STREAM.LANGUAGE.ICON');
						while (elems.length) elems[0].parentNode.replaceChild(createIcon(null, 'sub'+i+'.x-unk ', null, null, 'sub language: '+mapLanguage('x-unk'), 'i_sub_x-unk'),elems[0]);
						elems = row.getElementsByTagName('FILE.SUB.STREAM.FLAGS');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode(' '),elems[0]);
						elems = row.getElementsByTagName('FILE.SUB.STREAM.TYPE');
						while (elems.length) elems[0].parentNode.replaceChild(document.createTextNode('n/a'),elems[0]);
					}
					table.tBodies[0].appendChild(row);
				}
			}
		}
	} else if (div) div.parentNode.removeChild(div);
	div = getElementsByClassName(document.getElementsByTagName('DIV'),'rel_eps',false)[0];
	if (file.epRelations.length) { // In case there are related eps
		// getting the row for cloning
		if (div) {
			var table = div.getElementsByTagName('TABLE')[0];
			if (table) {
				cloneTbody = document.createElement('TBODY');
				cloneTbody.appendChild(table.tBodies[0].rows[0]);
				var subRow = cloneTbody.rows[0].cloneNode(true);
				for (var i = 0; i < file.epRelations.length; i++) {
					var row = subRow.cloneNode(true);
					var stream = file.epRelations[i];
					table.tBodies[0].appendChild(row);
				}
			}
		}
	} else if (div) div.parentNode.removeChild(div);
	div = getElementsByClassName(document.getElementsByTagName('DIV'),'rel_files',false)[0];
	if (file.fileRelations.length) { // In case there are related eps
		// getting the row for cloning
		if (div) {
			var table = div.getElementsByTagName('TABLE')[0];
			if (table) {
				cloneTbody = document.createElement('TBODY');
				cloneTbody.appendChild(table.tBodies[0].rows[0]);
				var subRow = cloneTbody.rows[0].cloneNode(true);
				for (var i = 0; i < file.fileRelations.length; i++) {
					var row = subRow.cloneNode(true);
					var stream = file.fileRelations[i];
					table.tBodies[0].appendChild(row);
				}
			}
		}
	} else if (div) div.parentNode.removeChild(div);
	updateStatus('');
}

/* *
 * Prepares the page, also fetches XML data
 */
function prepPage() {
  uriObj = parseURI();
  if (uriObj['show'] && uriObj['show'] != 'file' && !uriObj['fid'] && !uriObj['eid'] && !uriObj['aid']) return;
	initTooltips();
  loadData('anime/a'+uriObj['aid']+'.xml',parseData);
}

window.onload = prepPage;