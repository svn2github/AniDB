<!-- global vars -->
var sel_text;
var begin,selection,end;
var myArea, hiddenArea;
var asel_text;
var tag1, tag2;
var usageMode;
var intCC;

<!-- toggle flags -->

var isBr = false;
var isP = false;
var isBold = false;
var isItalic = false;
var isUnderline = false;
var isStrikeout = false;
var isuList = false;
var isoList = false;
var isiList = false;
var isImg = false;
var isLink = false;
var isQuote = false;
var isCode = false;
var isAnime = false;
var isEp = false;
var isFile = false;
var isGroup = false;
var isReview = false;
var isCompany = false;
var isUser = false;
var isVotes = false;
var isCreq = false;
var isHTML = false;
var isConvert = false;
var allowOut = false;
var isSel = false;

<!-- Help line messages -->
html_d_help = "Line Break: &lt;br \/&gt; (alt+d)";
html_e_help = "Paragraph: &lt;p&gt;text&lt;\/p&gt; (alt+e)";
html_b_help = "Bold text: &lt;b&gt;text&lt;\/b&gt; (alt+b)";
html_i_help = "Italic text: &lt;i&gt;text&lt;\/i&gt; (alt+i)";
html_u_help = "Underline text: &lt;span style=\"text-decoration: underline\"&gt;text&lt;\/span&gt; (alt+u)";
html_ul_help = "List: &lt;ul&gt;list&lt;\/ul&gt; (alt+l)";
html_ol_help = "Ordered list: &lt;ol&gt;list&lt;\/ol&gt; (alt+l)";
html_li_help = "List item: &lt;li&gt;element&lt;\/li&gt; (alt+k)";
html_p_help = "Insert Image: &lt;img src=\"http:\/\/url\" alt=\"\"&gt; (alt+p)";
html_w_help = "Insert URL: &lt;a href=\"http:\/\/url\"&gt;text&lt;\/a&gt; (alt+w)";
html_q_help = "Quote text: &lt;span style=\"class: quote\"&gt;text&lt;\/span&gt;  (alt+q)";
html_c_help = "Code display: &lt;span style=\"class: code\"&gt;text&lt;\/span&gt;  (alt+c)";
html_s_help = "Font color: &lt;span style=\"color: red\"&gt;text&lt;\/span&gt;  Tip: you can also use color: #FF0000";
html_f_help = "Font size: &lt;span style=\"font-size: x-small\"&gt;small text&lt;\/span&gt;";

bb_d_help = "Line Break: not used (alt+d)";
//bb_e_help = "Paragraph: not used (alt+e)";
bb_b_help = "Bold text: [b]text[/b]  (alt+b)";
bb_i_help = "Italic text: [i]text[/i]  (alt+i)";
bb_u_help = "Underline text: [u]text[/u]  (alt+u)";
bb_u_help = "Line-through text: [s]text[/s]  (alt+h)";
bb_ul_help = "List: [ul]text[/ul]  (alt+l)";
bb_ol_help = "Ordered list: [ol]text[/ol]  (alt+o)";
bb_li_help = "List item: [li]element[/li]  (alt+k)";
bb_p_help = "Insert image: [img]http://image_url[/img]  (alt+p)";
bb_url_help = "Insert URL: [url]http://url[/url] or [url=http://url]text[/url]  (alt+t)";
bb_q_help = "Quote text: [quote]text[/quote]  (alt+q)";
bb_w_help = "Code display: [code]code[/code]  (alt+w)";
bb_s_help = "Font color: [color=red]text[/color] or [color=#FF0000]text[/color]";
bb_f_help = "Font size: [size=x-small]small text[/size]";
bb_anime_help = "Link an anime: [anime=AID]text[/anime] (alt+a)";
bb_ep_help = "Link an episode: [ep=EID]text[/ep] (alt+e)";
bb_file_help = "Link a file: [file=FID]text[/file] (alt+f)";
bb_group_help = "Link a group: [group=GID]text[/group] (alt+g)";
bb_company_help = "Link a company: [company=PRID]text[/company] (alt+c)";
bb_user_help = "Link to a user mylist: [user=UID]text[/user] (alt+y)";
bb_votes_help = "Link to a user votes: [votes=UID]text[/votes] (alt+v)";
bb_review_help = "Link a review: [review=AID#REVIEWID]text[/review] (alt+r)";
bb_creq_help = "Link a creq: [creq=CREQID]text[/creq] (alt+d)"; 

<!-- general functions -->

function change_mode(mode){
	if (!mode)
		mode = document.getElementById('sel_mode').value;
	if (mode == 'basic'){ // Basic mode
		document.getElementById('bold').className = '';
		document.getElementById('italic').className = '';
		document.getElementById('underline').className = '';
		document.getElementById('strikeout').className = '';
		document.getElementById('ilist').className = '';
		document.getElementById('ulist').className = '';
		document.getElementById('olist').className = '';
	} else if (mode == 'full') { // Extended mode 
		document.getElementById('br').className = '';
		document.getElementById('pr').className = '';
		document.getElementById('olist').className = '';
		document.getElementById('quote').className = '';
		document.getElementById('code').className = '';
		document.getElementById('font_controls').className = '';
		document.getElementById('link').className = '';
		document.getElementById('img').className = '';
	} else if (mode == 'pm') { // pm mode
		change_mode('basic');
		document.getElementById('quote').className = '';
		document.getElementById('code').className = '';
		document.getElementById('anime').className = '';
		document.getElementById('file').className = '';
		document.getElementById('ep').className = '';
		document.getElementById('group').className = '';
		document.getElementById('review').className = '';
		document.getElementById('creq').className = '';
		document.getElementById('user').className = '';
		document.getElementById('votes').className = '';
		document.getElementById('company').className = '';
	} else if (mode == 'review') { // review mode
		change_mode('basic');
		document.getElementById('anime').className = '';
		document.getElementById('review').className = '';
		document.getElementById('company').className = '';
	} else if (mode == 'news') { // news mode
		change_mode('basic');
		document.getElementById('font_controls').className = '';
		document.getElementById('quote').className = '';
		document.getElementById('code').className = '';
		document.getElementById('anime').className = '';
		document.getElementById('file').className = '';
		document.getElementById('ep').className = '';
		document.getElementById('group').className = '';
		document.getElementById('link').className = '';
		document.getElementById('company').className = '';
	} else if (mode == 'anime') { // anime mode
		change_mode('basic');
	} else if (mode == 'company') { // company mode
		change_mode('basic');
		document.getElementById('link').className = '';
		document.getElementById('company').className = '';
	}
}

function change_layout()
{
	layout = document.getElementById('sel_layout').value;
	if (layout == 'bb'){
		isHTML = false;
	} else {
		isHTML = true;
	}
}

function change_convert()
{
	convert = document.getElementById('sel_convert').value;
	if (convert == '1'){
		isConvert = true;
	} else {
		isConvert = false;
	}
}

function show_help(text)
{
	var mode;
	if (isHTML){ mode = 'html'; } else { mode = 'bb' }
	document.getElementById('help_line').innerHTML = eval(mode+'_'+text);
}

function insertAtCursor(myField, myValue) {
	if (document.selection) {
		//IE support
		myField.focus();
		sel = document.selection.createRange();
		sel.text = myValue;
		myField.focus();
	} else if (myField.selectionStart || myField.selectionStart == '0') {
		//MOZILLA/NETSCAPE support
		var startPos = myField.selectionStart;
		var endPos = myField.selectionEnd;
		myField.value = myField.value.substring(0, startPos)
		+ myValue
		+ myField.value.substring(endPos, myField.value.length);
		myField.focus();
		myField.selectionStart = startPos + myValue.length;
		myField.selectionEnd = startPos + myValue.length;
	} else {
		myField.value += myValue;
		myField.focus();
	}
}

function insertTagsAtSelection(myField, myValue, tag1, tag2) {
	if (document.selection) {
		//IE support
		myField.focus();
		sel = document.selection.createRange();
		sel.text = tag1+myValue+tag2;
		myField.focus();
	} else if (myField.selectionStart || myField.selectionStart == '0') {
		//MOZILLA/NETSCAPE support
		var startPos = myField.selectionStart;
		var endPos = myField.selectionEnd;
		myField.value = myField.value.substring(0, startPos)
		+ tag1+ myValue + tag2
		+ myField.value.substring(endPos, myField.value.length);
		myField.focus();
		myField.selectionStart = startPos + tag1.length + myValue.length + tag2.length;
		myField.selectionEnd = startPos + tag1.length + myValue.length + tag2.length;
	} else {
		myField.value += tag1+myValue+tag2;
		myField.focus();
	}
}

function getSel(myArea)
{
	var myField = eval(myArea);
	var local_set_text = '';
	var selText = false;
	if (window.getSelection) {
		local_sel_text = window.getSelection();
		selText = (local_sel_text != ''); // if true we have text selected outside of myArea
		if (!selText && myArea.selectionStart!= undefined) {  
			begin = myField.value.substr(0, myField.selectionStart);
			local_sel_text = myField.value.substr(myField.selectionStart, myField.selectionEnd - myField.selectionStart);
			end = myField.value.substr(myField.selectionEnd);
		}
	} else if (document.getSelection) {
		local_sel_text = document.getSelection();
		selText = (local_sel_text != ''); // if true we have text selected outside of myArea
		if (!selText && myArea.selectionStart!= undefined) {  
			begin = myField.value.substr(0, myField.selectionStart);
			local_sel_text = myField.value.substr(myField.selectionStart, myField.selectionEnd - myField.selectionStart);
			end = myField.value.substr(myField.selectionEnd);
		}
	} else if (document.selection) {
		local_sel_text = document.selection.createRange().text;
	}
	else return;
	sel_text = local_sel_text;
	if (selText && allowOut){
		isSel = true;
		if (isHTML){
			tag1 = '<span class="quote">';
			tag2 = '</span>';
		} else {
			tag1 = '[quote=""]';
			tag2 = '[/quote]';
		}
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else { isSel = false; }
	myArea.focus();
}

function escapeVal(textarea,replaceWith){
	textarea.value = escape(textarea.value)
	for (i=0; i<textarea.value.length; i++){
		if (textarea.value.indexOf("%0D%0A") > -1) {
			textarea.value=textarea.value.replace("%0D%0A",replaceWith)
		} else if (textarea.value.indexOf("%0A") > -1) {
			textarea.value=textarea.value.replace("%0A",replaceWith)
		} else if (textarea.value.indexOf("%0D") > -1) {
			textarea.value=textarea.value.replace("%0D",replaceWith)
		}
	}
	textarea.value=unescape(textarea.value);
}

function convertHTMLText(textarea) {
	var li_regExp = /\<br \/\>\<li\>(.+?)\<\/li\>/mgi;
	var ul_regExp = /\<ul\>(.+?)\\<br \/\><\/ul\>/mgi;
	var ol_regExp = /\<ol\>(.+?)\<br \/\>\<\/ol\>/mgi;
	textarea.value = textarea.value.replace(li_regExp,"<li>$1</li>");
	textarea.value = textarea.value.replace(ul_regExp,"<ul>$1</ul>");
	textarea.value = textarea.value.replace(ol_regExp,"<ol>$1</ol>");
}

function convertBBText(textarea) {
	var b_regExp = /\[b\](.+?)\[\/b\]/mgi;
	var i_regExp = /\[i\](.+?)\[\/i\]/mgi;
	var u_regExp = /\[u\](.+?)\[\/u\]/mgi;
	var s_regExp = /\[s\](.+?)\[\/s\]/mgi;
	var ul_regExp1 = /\[ul\](.+?)\[\/ul\]/mgi;
	var ul_regExp2 = /((^\*.*(\n)?)+)/mgi;
	var ol_regExp1 = /\[ol\](.+?)\[\/ol\]/mgi;
	//var ol_regExp2 = /((^\#.*(\n)?)+)/mgi;
	var li_regExp0 = /\<br \/\>\[li\](.+?)\[\/li\]/mgi;
	var li_regExp1 = /\[li\](.+?)\[\/li\]/mgi;
	var li_regExp2 = /\* *(.*)$/mgi;
	//var li_regExp3 = /\# *(.*)$/mgi;
	var quote_regExp = /\[quote\](.+?)\[\/quote\]/mgi;
	var quoteUsr_regExp = /\[quote=([^[]*)\](.+?)\[\/quote\]/mgi;
	var code_regExp = /\[code\](.+?)\[\/code\]/mgi;
	var anime_regExp = /\[anime=(\d+)\](.+?)\[\/anime\]/mgi;
	var ep_regExp = /\[ep=(\d+)\](.+?)\[\/ep\]/mgi;
	var file_regExp = /\[file=(\d+)\](.+?)\[\/file\]/mgi;
	var group_regExp = /\[group=(\d+)\](.+?)\[\/group\]/mgi;
	var company_regExp = /\[company=(\d+)\](.+?)\[\/company\]/mgi;
	var user_regExp = /\[user=(\d+)\](.+?)\[\/user\]/mgi;
	var votes_regExp = /\[votes=(\d+)\](.+?)\[\/votes\]/mgi;
	var review_regExp = /\[review=(\d+\#?\d*)\](.+?)\[\/review\]/mgi;
	var creq_regExp = /\[creq=(\d+)\](.+?)\[\/creq\]/mgi;
	var url_regExp = /\[url=(.+?)\](.+?)\[\/url\]/mgi;
	var urlLink_regExp = /\[url\](.+?)\[\/url\]/mgi;
	var size_regExp = /\[size=(.+?)\](.+?)\[\/size\]/mgi;
	var color_regExp = /\[color=(.+?)\](.+?)\[\/color\]/mgi;
	textarea.value = textarea.value.replace(b_regExp,"<b>$1</b>");
	textarea.value = textarea.value.replace(i_regExp,"<i>$1</i>");
	textarea.value = textarea.value.replace(u_regExp,"<span style=\"text-decoration: underline;\">$1</span>");
	textarea.value = textarea.value.replace(s_regExp,"<span style=\"text-decoration: line-through;\">$1</span>");
	textarea.value = textarea.value.replace(ul_regExp1,"<ul>$1</ul>");
	textarea.value = textarea.value.replace(ul_regExp2,"<ul>$1</ul>");
	textarea.value = textarea.value.replace(ol_regExp1,"<ol>$1</ol>");
	//textarea.value = textarea.value.replace(ol_regExp2,"<ol>$1</ol>");
	textarea.value = textarea.value.replace(li_regExp0,"<li>$1</li>");
	textarea.value = textarea.value.replace(li_regExp1,"<li>$1</li>");
	textarea.value = textarea.value.replace(li_regExp2,"<li>$1</li>");
	//textarea.value = textarea.value.replace(li_regExp3,"<li>$1</li>");
	textarea.value = textarea.value.replace(quote_regExp,"<div class=\"quotebody\"><div class=\"quoteof\">Quote:</div><div class=\"quote\">$1</div></div>");
	textarea.value = textarea.value.replace(quoteUsr_regExp, "<div class=\"quotebody\"><div class=\"quoteof\">$1 wrote:</div><div class=\"quote\">$2</div></div>");
	textarea.value = textarea.value.replace(code_regExp,"<div class=\"codebody\"><div class=\"codeof\">Code:</div><div class=\"code\">$1</div></div>");
	textarea.value = textarea.value.replace(anime_regExp,"<a href=\"animedb.pl?show=anime&aid=$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(ep_regExp,"<a href=\"animedb.pl?show=ep&eid=$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(file_regExp,"<a href=\"animedb.pl?show=file&fid=$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(group_regExp,"<a href=\"animedb.pl?show=group&gid=$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(company_regExp,"<a href=\"animedb.pl?show=producer&prid=$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(user_regExp,"<a href=\"animedb.pl?show=mylist&uid=$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(votes_regExp,"<a href=\"animedb.pl?show=myvotes&uid=$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(review_regExp,"<a href=\"animedb.pl?show=animeatt&aid=$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(creq_regExp,"<a href=\"animedb.pl?show=creq&do.showhist=1&creq.id=$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(url_regExp,"<a href=\"$1\" target=\"blank\">$2</a>");
	textarea.value = textarea.value.replace(urlLink_regExp,"<a href=\"$1\" target=\"blank\">$1</a>");
	textarea.value = textarea.value.replace(color_regExp,"<span style=\"color: $1;\">$2</span>");
	textarea.value = textarea.value.replace(size_regExp,"<span style=\"font-size: $1;\">$2</span>");
}

function preview_output() {
	hiddenArea.value = myArea.value;
	if (!isHTML) // convert bb code to html to display
		convertBBText(hiddenArea);
	escapeVal(hiddenArea,'<br \/>');
	convertHTMLText(hiddenArea);
	if (!isHTML) // convert bb code to html to display
		convertBBText(hiddenArea);
	convertHTMLText(hiddenArea);
	if (!document.getElementById('preview')) {
		var table;
		if (usageMode == 'pm')
			table = document.getElementById('msg.send').parentNode.parentNode.parentNode;
		if (usageMode == 'review')
			table = document.getElementById('review.add').parentNode.parentNode.parentNode;
		if (usageMode == 'news')
			table = document.getElementById('addn.add').parentNode.parentNode.parentNode;
		if (usageMode == 'anime')
			table = document.getElementById('anime.add').parentNode.parentNode.parentNode;
		if (usageMode == 'company')
			table = document.getElementById('company.add').parentNode.parentNode.parentNode;
		var nRow = document.createElement('TR');
		var nCol = document.createElement('TD');
		nCol.vAlign = 'top';
		nCol.appendChild(document.createTextNode(' Preview: '));
		nRow.appendChild(nCol);
		nCol = document.createElement('TD');
		var nSpan = document.createElement('SPAN');
		nSpan.id = "preview";
		nCol.appendChild(nSpan);
		nRow.appendChild(nCol);
		table.appendChild(nRow);
	}
	var out;
	if (usageMode == 'pm')
		out = '<table class="msg_preview">'+
				'<tbody><tr><td><b>To:</b></td><td><a href="#">'+document.getElementById('msg.to').value+'</a> (uid)</td></tr>'+
				'<tr><td><b>Date:</b></td><td>'+new Date()+'</td></tr>'+
				'<tr><td><b>Title:</b></td><td>'+document.getElementById('msg.title').value+'</td></tr>'+
				'<tr><td valign="top"><b>Body:</b></td><td>'+hiddenArea.value+'</td></tr>'+
				'</tbody></table>';
	else
		out = hiddenArea.value;
	document.getElementById('preview').innerHTML = out;
}

<!-- control functions -->

function make_br() {
	allowOut = false;
	if (isHTML){
		tag1 = '<br />';
		tag2 = '';
	} else {
		tag1 = '';
		tag2 = '';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	}
}

function make_p() {
	allowOut = false;
	if (isHTML){
		tag1 = '<p>';
		tag2 = '</p>';
	} else {
		tag1 = '';
		tag2 = '';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else {
		if (!isP) { 
			insertAtCursor(myArea,tag1);
			isP = true;
			document.getElementById('pr').value = ' p* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isP = false;
			document.getElementById('pr').value = ' p ';
		}
	}
}

function make_bold() {
	allowOut = false;
	if (isHTML){
		tag1 = '<b>';
		tag2 = '</b>';
	} else {
		tag1 = '[b]';
		tag2 = '[/b]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else {
		if (!isBold) { 
			insertAtCursor(myArea,tag1);
			isBold = true;
			document.getElementById('bold').value = ' B* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isBold = false;
			document.getElementById('bold').value = ' B ';
		}
	}
}

function make_italic() {
	allowOut = false;
	if (isHTML){
		tag1 = '<i>';
		tag2 = '</i>';
	} else {
		tag1 = '[i]';
		tag2 = '[/i]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else {
		if (!isItalic) { 
			insertAtCursor(myArea,tag1);
			isItalic = true;
			document.getElementById('italic').value = ' i* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isItalic = false;
			document.getElementById('italic').value = ' i ';
		}
	}
}

function make_underline() {
	allowOut = false;
	if (isHTML){
		tag1 = '<span style="text-decoration: underline;">';
		tag2 = '</span>';
	} else {
		tag1 = '[u]';
		tag2 = '[/u]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else {
		if (!isUnderline) { 
			insertAtCursor(myArea,tag1);
			isUnderline = true;
			document.getElementById('underline').value = ' u* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isUnderline = false;
			document.getElementById('underline').value = ' u ';
		}
	}
}

function make_strikeout() {
	allowOut = false;
	if (isHTML){
		tag1 = '<span style="text-decoration: line-through;">';
		tag2 = '</span>';
	} else {
		tag1 = '[s]';
		tag2 = '[/s]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else {
		if (!isStrikeout) { 
			insertAtCursor(myArea,tag1);
			isStrikeout = true;
			document.getElementById('strikeout').value = ' s* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isStrikeout = false;
			document.getElementById('strikeout').value = ' s ';
		}
	}
}

function make_quote() {
	allowOut = true;
	if (isHTML){
		tag1 = '<span class="quote">';
		tag2 = '</span>';
	} else {
		tag1 = '[quote]';
		tag2 = '[/quote]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isQuote) { 
				insertAtCursor(myArea,tag1);
				isQuote = true;
				document.getElementById('quote').value = ' quote* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isQuote = false;
				document.getElementById('quote').value = ' quote ';
			}
		}
	}
}

function make_code() {
	allowOut = false;
	if (isHTML){
		tag1 = '<span class="code">';
		tag2 = '</span>';
	} else {
		tag1 = '[code]';
		tag2 = '[/code]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else {
		if (!isCode) { 
			insertAtCursor(myArea,tag1);
			isCode = true;
			document.getElementById('code').value = ' code* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isCode = false;
			document.getElementById('code').value = ' code ';
		}
	}
}

function make_ulist() {
	allowOut = false;
	if (isHTML){
		tag1 = '<ul>';
		tag2 = '</ul>';
	} else {
		tag1 = '[ul]';
		tag2 = '[/ul]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else {
		if (!isuList) { 
			insertAtCursor(myArea,tag1);
			isuList = true;
			document.getElementById('ulist').value = ' ul* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isuList = false;
			document.getElementById('ulist').value = ' ul ';
		}
	}
}

function make_olist() {
	allowOut = false;
	if (isHTML){
		tag1 = '<ol>';
		tag2 = '</ol>';
	} else {
		tag1 = '[ol]';
		tag2 = '[/ol]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else {
		if (!isoList) { 
			insertAtCursor(myArea,tag1);
			isoList = true;
			document.getElementById('olist').value = ' ol* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isoList = false;
			document.getElementById('olist').value = ' ol ';
		}
	}
}

function make_ilist() {
	allowOut = false;
	if (isHTML){
		tag1 = '<li>';
		tag2 = '</li>';
	} else {
		tag1 = '[li]';
		tag2 = '[/li]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	} else {
		if (!isiList) { 
			insertAtCursor(myArea,tag1);
			isiList = true;
			document.getElementById('ilist').value = ' li* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isiList = false;
			document.getElementById('ilist').value = ' li ';
		}
	}
}

function make_img() {
	allowOut = false;
	if (isHTML){
		tag1 = '<img src=\"';
		tag2 = '\" alt=\"\" />';
	} else {
		tag1 = '[img]';
		tag2 = '[/img]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		insertTagsAtSelection(myArea, sel_text, tag1, tag2);
	}
}

function make_link() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href=\"';
		tag1a = '\">';
		tag2 = '</a>';
	} else {
		tag1 = '[url=';
		tag1a = ']';
		tag2 = '[/url]';
	}
	getSel(myArea);
	if (sel_text.length > 0){ // we have text
		var link = prompt("Please enter the url", "http://");
		insertTagsAtSelection(myArea, sel_text, (tag1 + link + tag1a), tag2);
	} else {
		if (!isLink) { 
			insertAtCursor(myArea,(tag1 + tag1a));
			isLink = true;
			document.getElementById('link').value = ' url* ';
		} else {
		 	insertAtCursor(myArea,tag2);
			isLink = false;
			document.getElementById('link').value = ' url ';
		}
	}
}

function make_color() {
	allowOut = false;
	if (isHTML){
		tag1 = '<span style="color: ';
		tag1a = ';\">';
		tag2 = '</span>';
	} else {
		tag1 = '[color=';
		tag1a = ']';
		tag2 = '[/color]';
	}
	color = document.getElementById('sel_color').value;
	getSel(myArea);
	insertTagsAtSelection(myArea, sel_text, (tag1 + color + tag1a), tag2);
}

function make_size() {
	allowOut = false;
	if (isHTML){
		tag1 = '<span style="font-size: ';
		tag1a = ';\">';
		tag2 = '</span>';
	} else {
		tag1 = '[size=';
		tag1a = ']';
		tag2 = '[/size]';
	}
	size = document.getElementById('sel_size').value;
	getSel(myArea);
	insertTagsAtSelection(myArea, sel_text, (tag1 + size + tag1a), tag2);
}

function make_anime() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href="animedb.pl?show=anime&aid=" target="_blank">';
		tag2 = '</a>';
	} else {
		tag1 = '[anime=]';
		tag2 = '[/anime]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isAnime) { 
				insertAtCursor(myArea,tag1);
				isAnime = true;
				document.getElementById('anime').value = ' anime* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isAnime = false;
				document.getElementById('anime').value = ' anime ';
			}
		}
	}
}

function make_ep() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href="animedb.pl?show=ep&eid=" target="_blank">';
		tag2 = '</a>';
	} else {
		tag1 = '[ep=]';
		tag2 = '[/ep]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isEp) { 
				insertAtCursor(myArea,tag1);
				isEp = true;
				document.getElementById('ep').value = ' ep* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isEp = false;
				document.getElementById('ep').value = ' ep ';
			}
		}
	}
}

function make_file() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href="animedb.pl?show=file&fid=" target="_blank">';
		tag2 = '</a>';
	} else {
		tag1 = '[file=]';
		tag2 = '[/file]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isFile) { 
				insertAtCursor(myArea,tag1);
				isFile = true;
				document.getElementById('file').value = ' file* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isFile = false;
				document.getElementById('file').value = ' file ';
			}
		}
	}
}

function make_group() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href="animedb.pl?show=group&gid=" target="_blank">';
		tag2 = '</a>';
	} else {
		tag1 = '[group=]';
		tag2 = '[/group]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isGroup) { 
				insertAtCursor(myArea,tag1);
				isGroup = true;
				document.getElementById('group').value = ' group* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isGroup = false;
				document.getElementById('group').value = ' group ';
			}
		}
	}
}

function make_company() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href="animedb.pl?show=producer&prid=" target="_blank">';
		tag2 = '</a>';
	} else {
		tag1 = '[company=]';
		tag2 = '[/company]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isCompany) { 
				insertAtCursor(myArea,tag1);
				isCompany = true;
				document.getElementById('company').value = ' company* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isCompany = false;
				document.getElementById('company').value = ' company ';
			}
		}
	}
}

function make_user() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href="animedb.pl?show=mylist&uid=" target="_blank">';
		tag2 = '</a>';
	} else {
		tag1 = '[user=]';
		tag2 = '[/user]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isUser) { 
				insertAtCursor(myArea,tag1);
				isUser = true;
				document.getElementById('user').value = ' user* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isUser = false;
				document.getElementById('user').value = ' user ';
			}
		}
	}
}

function make_votes() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href="animedb.pl?show=myvotes&uid=" target="_blank">';
		tag2 = '</a>';
	} else {
		tag1 = '[votes=]';
		tag2 = '[/votes]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isVotes) { 
				insertAtCursor(myArea,tag1);
				isVotes = true;
				document.getElementById('votes').value = ' votes* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isVotes = false;
				document.getElementById('votes').value = ' votes ';
			}
		}
	}
}

function make_review() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href="animedb.pl?show=animeatt&aid=" target="_blank">';
		tag2 = '</a>';
	} else {
		tag1 = '[review=]';
		tag2 = '[/review]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isReview) { 
				insertAtCursor(myArea,tag1);
				isReview = true;
				document.getElementById('review').value = ' review* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isReview = false;
				document.getElementById('review').value = ' review ';
			}
		}
	}
}

function make_creq() {
	allowOut = false;
	if (isHTML){
		tag1 = '<a href="animedb.pl?show=creq&do.showhist=1&creq.id=" target="_blank">';
		tag2 = '</a>';
	} else {
		tag1 = '[creq=]';
		tag2 = '[/creq]';
	}
	getSel(myArea);
	if (!isSel) {
		if (sel_text.length > 0){ // we have text
			insertTagsAtSelection(myArea, sel_text, tag1, tag2);
		} else {
			if (!isCreq) { 
				insertAtCursor(myArea,tag1);
				isCreq = true;
				document.getElementById('creq').value = ' creq* ';
			} else {
			 	insertAtCursor(myArea,tag2);
				isCreq = false;
				document.getElementById('creq').value = ' creq ';
			}
		}
	}
}

function updateCharCount() {
	var charCount = document.getElementById('charCount');
	var wordCount = document.getElementById('wordCount');
	var limit;
	if (usageMode == 'news') {
		cc = charCount.value = myArea.value.length;
	} else if (usageMode == 'review') {
		limit = 50000;
		if (myArea.value.length > limit) myArea.value = myArea.value.substring(0,limit);
		wc = wordCount.value = myArea.value.split(' ').length + myArea.value.split('\n').length - 2;
		cc = charCount.value = myArea.value.length;
		charCount.style.backgroundColor = "";		
		if (cc <= 500)
			charCount.style.color = "grey";
		if (cc > 45000 && cc <= 49990)
			charCount.style.color = "red";
		if (cc > 49990) {
			charCount.style.backgroundColor = "red";
			charCount.style.color = "white";
		}
		if (wc <= 500)
			wordCount.style.color = "green";
		if (wc > 500 && wc <= 1250)
			wordCount.style.color = "orange";
		if (wc > 1250)
			wordCount.style.color = "red";
	} else {
		limit = 5000;
		if (myArea.value.length > limit) myArea.value = myArea.value.substring(0,limit);		
		cc = charCount.value = myArea.value.length;
		charCount.style.backgroundColor = "";
		if (cc <= 4500)
			charCount.style.color = "grey";
		if (cc > 4500 && cc <= 4990)
			charCount.style.color = "red";
		if (cc > 4990) {
			charCount.style.backgroundColor = "red";
			charCount.style.color = "white";
		}
	}
}

function span_mode_controls()
{
	var output = "";
	output += '<span style="font-size: small;">';
	output += '&nbsp;Edit mode: <select class="" id="sel_mode" onchange="change_mode()">';
	output += '<option value="basic">Basic</option>';
    output += '<option value="pm">Private Message</option>';
	output += '<option value="review">Review</option>';
	output += '<option value="full">Full</option>';
    output += '</select>';
    output += '&nbsp;Layout: <select class="" id="sel_layout" onchange="change_layout()">';
    output += '<option value="bb">BB</option>';
	output += '<option value="html">HTML</option>';
    output += '</select>';
    output += '&nbsp;Convert: <select class="" id="sel_convert" onchange="change_convert()">';
    output += '<option value="0">none</option>';
    output += '<option value="1">Line breaks to &lt;br /&gt;</option>';
    output += '</select></span>';
    document.getElementById('mode_controls').innerHTML = output;
}

function span_controls()
{
	var output = '';
	output += '<input class="hidden" value=" br " id="br" style="width: 30px;" type="button" onclick="make_br()" onMouseOver="show_help(\'d_help\')" />';
	output += '<input class="hidden" value=" p " id="pr" style="width: 30px;" type="button" onclick="make_p()" onMouseOver="show_help(\'e_help\')" />';
	output += '&nbsp;<input accesskey="b" class="hidden" value=" B " id="bold" style="font-weight: bold; width: 30px;" type="button" onclick="make_bold()" onMouseOver="show_help(\'b_help\')" />';
	output += '&nbsp;<input accesskey="i" class="hidden" value=" i " id="italic" style="font-style: italic; width: 30px;" type="button" onclick="make_italic()" onMouseOver="show_help(\'i_help\')" />';
	output += '&nbsp;<input accesskey="u" class="hidden" value=" u " id="underline" style="text-decoration: underline; width: 30px;" type="button" onclick="make_underline()" onMouseOver="show_help(\'u_help\')" />';
	output += '&nbsp;<input accesskey="h" class="hidden" value=" s " id="strikeout" style="text-decoration: line-through; width: 30px;" type="button" onclick="make_strikeout()" onMouseOver="show_help(\'s_help\')" />';
	output += '&nbsp;<input accesskey="l" class="hidden" value=" ul " id="ulist" style="width: 30px" type="button" onclick="make_ulist()" onMouseOver="show_help(\'ul_help\')" />';
	output += '&nbsp;<input accesskey="o" class="hidden" value=" ol " id="olist" style="width: 30px" type="button" onclick="make_olist()" onMouseOver="show_help(\'ol_help\')" />';
	output += '&nbsp;<input accesskey="k" class="hidden" value=" li " id="ilist" style="width: 30px" type="button" onclick="make_ilist()" onMouseOver="show_help(\'li_help\')" />';
	output += '&nbsp;<input accesskey="q" class="hidden" value=" quote " id="quote" style="width: 55px;" type="button" onclick="make_quote()" onMouseOver="show_help(\'q_help\')" />';
	output += '&nbsp;<input accesskey="w" class="hidden" value=" code " id="code" style="width: 55px;" type="button" onclick="make_code()" onMouseOver="show_help(\'w_help\')" />';
	output += '&nbsp;<input class="hidden" value=" img " id="img" style="width: 40px" type="button" onclick="make_img()" onMouseOver="show_help(\'p_help\')" />';
	output += '&nbsp;<input accesskey="t" class="hidden" value=" url " id="link" style="text-decoration: underline; width: 40px" type="button" onclick="make_link()" onMouseOver="show_help(\'url_help\')" />';
	if (usageMode != 'anime') {
		output += '<br /><br />';
		output += '&nbsp;<input accesskey="a" class="hidden" value=" anime " id="anime" style="width: 65px;" type="button" onclick="make_anime()" onMouseOver="show_help(\'anime_help\')" />';
		output += '&nbsp;<input accesskey="e" class="hidden" value=" ep " id="ep" style="width: 45px;" type="button" onclick="make_ep()" onMouseOver="show_help(\'ep_help\')" />';
		output += '&nbsp;<input accesskey="f" class="hidden" value=" file " id="file" style="width: 55px;" type="button" onclick="make_file()" onMouseOver="show_help(\'file_help\')" />';
		output += '&nbsp;<input accesskey="g" class="hidden" value=" group " id="group" style="width: 65px;" type="button" onclick="make_group()" onMouseOver="show_help(\'group_help\')" />';
		output += '&nbsp;<input accesskey="c" class="hidden" value=" company " id="company" style="width: 85px;" type="button" onclick="make_company()" onMouseOver="show_help(\'company_help\')" />';
		output += '&nbsp;<input accesskey="y" class="hidden" value=" user " id="user" style="width: 55px;" type="button" onclick="make_user()" onMouseOver="show_help(\'user_help\')" />';
		output += '&nbsp;<input accesskey="v" class="hidden" value=" votes " id="votes" style="width: 65px;" type="button" onclick="make_votes()" onMouseOver="show_help(\'votes_help\')" />';
		output += '&nbsp;<input accesskey="r" class="hidden" value=" review " id="review" style="width: 75px;" type="button" onclick="make_review()" onMouseOver="show_help(\'review_help\')" />';
		output += '&nbsp;<input accesskey="d" class="hidden" value=" creq " id="creq" style="width: 55px;" type="button" onclick="make_creq()" onMouseOver="show_help(\'creq_help\')" />';
	}
	document.getElementById('controls').innerHTML = output;
}

function span_font_controls() {
	if (!document.getElementById('font_controls')) return;
	var output = '';
	output += '<span style="font-size: small;">';
    output += '&nbsp;Font color: <select class="" id="sel_color" onMouseOver="show_help(\'s_help\')" onchange="make_color()">';
    output += '<option disabled selected="selected">-----</option>';
	output += '<option style="color:black;" value="">Default</option>';
	output += '<option style="color:darkred;" value="darkred">Dark Red</option>';
	output += '<option style="color:red;" value="red">Red</option>';
	output += '<option style="color:orange;" value="orange">Orange</option>';
	output += '<option style="color:brown;" value="brown">Brown</option>';
	output += '<option style="color:yellow;" value="yellow">Yellow</option>';
	output += '<option style="color:green;" value="green">Green</option>';
	output += '<option style="color:olive;" value="olive">Olive</option>';
	output += '<option style="color:cyan;" value="cyan">Cyan</option>';
	output += '<option style="color:blue;" value="blue">Blue</option>';
	output += '<option style="color:darkblue;" value="darkblue">Dark Blue</option>';
	output += '<option style="color:indigo;" value="indigo">Indigo</option>';
	output += '<option style="color:violet;" value="violet">Violet</option>';
	output += '<option style="color:white;" value="white">White</option>';
	output += '<option style="color:black;" value="black">Black</option>';
	output += '</select>';
	output += '&nbsp;Font size: <select class="" id="sel_size" onMouseOver="show_help(\'f_help\')" onchange="make_size()">';
	output += '<option disabled selected="selected">-----</option>';
	output += '<option>xx-small</option>';
	output += '<option>x-small</option>';
	output += '<option>small</option>';
  	output += '<option>medium</option>';
	output += '<option>large</option>';
  	output += '<option>x-large</option>';
	output += '<option>xx-large</option>';
	output += '</select>';
    output += '</span>';
	document.getElementById('font_controls').innerHTML = output;
}

function expandPage(type) {
	var msgBody;
	if (type == 'pm')	
		msgBody = document.getElementById('msg.body');
	else if (type == 'review')
		msgBody = document.getElementById('review.body');
	else if (type == 'news')
		msgBody = document.getElementById('addn.body');
	else if (type == 'anime')
		msgBody = document.getElementById('anime.body');
	else if (type == 'company')
		msgBody = document.getElementById('company.body');
	if (msgBody) {
		var col = msgBody.parentNode;
		intCC = setInterval('updateCharCount()',100);
		//msgBody.onkeypress = function onkeypress(event) { updateCharCount(); }
		var hiddenTextArea = document.createElement('TEXTAREA');
		hiddenTextArea.id = "hidden_text_area";
		hiddenTextArea.style.display = "none";
		hiddenTextArea.cols = 60;
		hiddenTextArea.rows = 15;
		col.appendChild(hiddenTextArea);
		var nRow, nCol, nSpan;
		nSpan = document.createElement('SPAN');
		nSpan.id = "mode_controls";
		col.insertBefore(nSpan,msgBody);
		nSpan = document.createElement('SPAN');
		nSpan.id = "controls";
		col.insertBefore(nSpan,msgBody);
		if (type == 'news') {
			col.insertBefore(document.createElement('BR'),msgBody);
			col.insertBefore(document.createElement('BR'),msgBody);
		}
		nSpan = document.createElement('SPAN');
		nSpan.id = "font_controls"
		nSpan.className = "hidden";
		col.insertBefore(nSpan,msgBody);
		nSpan = document.createElement('SPAN');
		col.insertBefore(document.createElement('BR'),msgBody);
		nSpan.id = "help_line";
		nSpan.style.fontSize = "x-small";
		col.insertBefore(nSpan,msgBody);
		col.insertBefore(document.createElement('BR'),msgBody);
	}
	var msgSend;
	if (type == 'pm')
		msgSend = document.getElementById('msg.send');
	else if (type == 'review')
		msgSend = document.getElementById('review.add');
	else if (type == 'news')
		msgSend = document.getElementById('addn.add');
	else if (type == 'anime')
		msgSend = document.getElementById('anime.add');
	else if (type == 'company')
		msgSend = document.getElementById('company.add');
	if (msgSend) {
		var col = msgSend.parentNode;
		var row = col.parentNode;
		var table = row.parentNode;
		var previewButton = document.createElement('INPUT');
		previewButton.type = "button";
		previewButton.value = "Preview";
		previewButton.onclick = function onclick(event) { preview_output(); }
		//previewButton.setAttribute('onclick','preview_output()');
		var resetButton = document.createElement('INPUT');
		resetButton.type = "reset";
		resetButton.value = "Clear";
		var charCount = document.createElement('INPUT');
		charCount.type = "text";
		charCount.id = "charCount";
		charCount.size = 5;
		charCount.disabled = true;
		charCount.value = 0;
		var wordCount = document.createElement('INPUT');
		wordCount.type = "text";
		wordCount.id = "wordCount";
		wordCount.size = 5;
		wordCount.disabled = true;
		col.appendChild(document.createTextNode(' '));
		col.appendChild(previewButton);
		col.appendChild(document.createTextNode(' '));
		col.appendChild(resetButton);
		col.appendChild(document.createTextNode(' Char. count: '));
		col.appendChild(charCount);
		if (type == 'review') {
			col.appendChild(document.createTextNode(' Word count: '));
			col.appendChild(wordCount);
		}
	}
}

function formating_init() {
	span_controls();
	span_font_controls();
	var s_mode = document.getElementById('sel_mode');
	var s_layout = document.getElementById('sel_layout');
	var s_convert = document.getElementById('sel_convert');
	if (s_mode) {
		s_mode.value = '0';
		change_mode();
	}
	if (s_layout) {
		s_layout.value = 'bb';
		change_layout();
	}
	if (s_convert) {
		s_convert.value = '0';
		change_convert();
	}
}

function msg_formating_init() {
	usageMode = 'pm';
	myArea = document.getElementById('msg.body'); // default textarea
	expandPage('pm');
	hiddenArea = document.getElementById('hidden_text_area'); // default hidden textarea
	//span_mode_controls(); // disable this if you wish to disallow mode changes
	formating_init()
	change_mode('pm');
}

function review_formating_init() {
	usageMode = 'review';
	myArea = document.getElementById('review.body'); // default textarea
	expandPage('review');
	hiddenArea = document.getElementById('hidden_text_area'); // default hidden textarea
	//span_mode_controls(); // disable this if you wish to disallow mode changes
	formating_init()
	change_mode('review');
}

function news_formating_init() {
	usageMode = 'news';
	myArea = document.getElementById('addn.body'); // default textarea
	expandPage('news');
	hiddenArea = document.getElementById('hidden_text_area'); // default hidden textarea
	//span_mode_controls(); // disable this if you wish to disallow mode changes
	formating_init()
	change_mode('news');
}

function anime_formating_init() {
	usageMode = 'anime';
	myArea = document.getElementById('anime.body'); // default textarea
	expandPage('anime');
	hiddenArea = document.getElementById('hidden_text_area'); // default hidden textarea
	//span_mode_controls(); // disable this if you wish to disallow mode changes
	formating_init()
	change_mode('anime');
}

function company_formating_init() {
	usageMode = 'company';
	myArea = document.getElementById('company.body'); // default textarea
	expandPage('company');
	hiddenArea = document.getElementById('hidden_text_area'); // default hidden textarea
	//span_mode_controls(); // disable this if you wish to disallow mode changes
	formating_init()
	change_mode('company');
}
