var xmlURL = "mylist.xml";
var xslURL = "mylistb.xsl";
var xslAnimeURL = "mylistc.xsl";
var resultElementId = "result";

const XSLNS = "http://www.w3.org/1999/XSL/Transform";
var xml_document;
var xsl_document;
var xsl_anime_document;


function loadXMLDoc(fname) {
	var xmlDoc;
	if (document.implementation && document.implementation.createDocument) {
		xmlDoc = document.implementation.createDocument("", "", null);
	}	else {
		return false;
	}

	xmlDoc.async = false;
	xmlDoc.load(fname);

	return xmlDoc;
}

function transformation(setParamsFunc) {
	if (!document.implementation && document.implementation.createDocument) {
		alert('Your browser cannot handle XSLT transforming using web standards!');
		return false;
	}

	// initialization
	if (!xml_document)
		xml_document = loadXMLDoc(xmlURL);
	if (!xsl_document)
		xsl_document = loadXMLDoc(xslURL);

	if (!xml_document || !xsl_document) {
		alert('Your browser cannot handle XSLT transforming using web standards!');
		return false;
	}

	$("body").addClass("waiting");

	// render
	var xsltProcessor = new XSLTProcessor();
	xsltProcessor.importStylesheet(xsl_document);
	transformationParameters(xsltProcessor);

	if (typeof setParamsFunc != "undefined")
		setParamsFunc(xsltProcessor);

	// show result
	$("#" + resultElementId).empty().append(xsltProcessor.transformToFragment(xml_document, document));

	afterTransformation();

	$("body").removeClass("waiting");

	return true;
}

function transformationParameters(xsltProcessor) {
	// FF does not tolerate undefined parameter values!
	if (typeof $("#typeList").val() == "undefined")
		xsltProcessor.setParameter(null, "typeFilter", "*");
	else
		xsltProcessor.setParameter(null, "typeFilter", $("#typeList").val());

	if (typeof $("#watchedFilter").val() == "undefined")
		xsltProcessor.setParameter(null, "watchedFilter", -1);
	else
		xsltProcessor.setParameter(null, "watchedFilter", $("#watchedFilter").val());

	if (typeof $("#ratingList").val() == "undefined")
		xsltProcessor.setParameter(null, "ratingFilter", -1);
	else
		xsltProcessor.setParameter(null, "ratingFilter", $("#ratingList").val());

	if (typeof $("#completeFilter").val() == "undefined")
		xsltProcessor.setParameter(null, "completeFilter", -1);
	else
		xsltProcessor.setParameter(null, "completeFilter", $("#completeFilter").val());

	if (typeof $("#languageList").val() == "undefined")
		xsltProcessor.setParameter(null, "mainLang", "*");
	else
		xsltProcessor.setParameter(null, "mainLang", $("#languageList").val());

	if (typeof $("#options").get(0) == "undefined")
		xsltProcessor.setParameter(null, "optionsVisible", 0);
	else
		xsltProcessor.setParameter(null, "optionsVisible", $("#options").hasClass("invisible") ? 0 : 1);
}

function afterTransformation() {
	// add an arrow the sorted column
	var sortField = null;
	var sortOrder = null;

	// get sort parameters from main anime select (A[...])
	var sortTags = xsl_document.getElementsByTagNameNS(XSLNS, "sort");
	for (var i = 0; i < sortTags.length; i++) {
		if (sortTags[i].parentNode.getAttributeNS(null, "select").match(/^A(\[|$)/)) {
			sortField = sortTags[i].getAttributeNS(null, "select");
			sortOrder = sortTags[i].getAttributeNS(null, "order");
			break;
		}
	}

	// add up/down arrows to visualize the current sort order
	if (sortField != null) {
		$("th[sortField="+sortField+"]").remove("span").append(sortOrder != "ascending" ? " \u2191" : " \u2193").attr("title", "currently sorted " + sortOrder);
	}

	// make columns sortable
	$(".sortable").click(function() { doSort(this); });

	// show/hide options
	$("#optionsheader").click(function() { $("#options").toggleClass("invisible"); });

	// show anime details
	$(".toggle_ep").click(function() { toggleAnimeDetails(this); });

	fixAwardImages();

/*
	// font size
	$("body").change(function () {
		this.style.fontSize = x + "em";
	});
*/
};

function transformationAnimeDetails(animeId, episodesElement) {
	// initialization
	if (!xsl_anime_document)
		xsl_anime_document = loadXMLDoc(xslAnimeURL);

	if (!xsl_anime_document) {
		alert('Your browser cannot handle XSLT transforming using web standards!');
		return false;
	}

	$("body").addClass("waiting");

	// render
	var xsltProcessor = new XSLTProcessor();
	xsltProcessor.importStylesheet(xsl_anime_document);

	xsltProcessor.setParameter(null, "animeId", animeId);
	if (typeof $("#languageList").val() == "undefined")
		xsltProcessor.setParameter(null, "mainLang", "*");
	else
		xsltProcessor.setParameter(null, "mainLang", $("#languageList").val());

	// show result
	$(episodesElement).empty().append(xsltProcessor.transformToFragment(xml_document, document));

	$("body").removeClass("waiting");

	return true;
}

function doFilterType() {
	// re-build list
	transformation();
}

function doFilterRating() {
	// re-build list
	transformation();
}

function doFilterWatched(filter) {
	// filter: -1=none, 0=nothing watched, 1=at least one, 2=only partially, 3=completely
	$("#watchedFilter").val($("#watchedFilter").val() == filter ? -1 : filter);

	// re-build list
	transformation();
}

function doFilterComplete() {
	// filter: -1=none, 0=incomplete only, 1=complete only
	if ($("#completeFilter").val() == -1)
		$("#completeFilter").val(0);
	else
	if ($("#completeFilter").val() == 0)
		$("#completeFilter").val(1);
	else
		$("#completeFilter").val(-1);

	// re-build list
	transformation();
}

function doSetMainLanguage(newLangId) {
	if (newLangId)
		$("#languageList").val(newLangId);

	// re-build list
	transformation();
}

function doSort(field) {
	// check params
	var sortField = $(field).attr("sortField");
	var sortOrder = $(field).attr("sortOrder");
	var sortType = $(field).attr("sortType");

	sortOrder = (sortOrder != "descending" ? "descending" : "ascending");
	if (!field || typeof sortField == "undefinded" || typeof sortOrder == "undefinded" || typeof sortType == "undefinded") {
		alert("Sorting not possible");
		return;
	}

	// set new sort parameters directly in the apply-templates selects
	var cnt = 0;
	var sortTags = xsl_document.getElementsByTagNameNS(XSLNS, "sort");
	for (var i = 0; i < sortTags.length; i++) {
		if (sortTags[i].parentNode.getAttributeNS(null, "select").match(/^A(\[|$)/)) {
			sortTags[i].setAttributeNS(null, "select", sortField);
			sortTags[i].setAttributeNS(null, "order", sortOrder);
			sortTags[i].setAttributeNS(null, "data-type", sortType);
			cnt++;
		}
	}

	$(xsl_document).find("th[sortField="+sortField+"]").attr("sortOrder", sortOrder);

	// re-build list
	transformation();
}

function doSelectEpisode(ep) {
	$(ep).toggleClass("selected");
	showEpisodesSummary(ep);
}

function parseAnimeId(element) {
	if (typeof element == "number")
		return element;

	if (typeof element == "object") { // assume its an element with number in id
		if ($(element).attr("animeId") != null)
			return parseInt($(element).attr("animeId"));
		element = element.id;
	}

	if (element.substr(0,1) != "a" && element.substr(0,1) != "e")
		return -1;

	if (element.indexOf("_") > 0)
		element = element.substring(0, element.indexOf("_"));

	var animeId = parseInt(element.replace(/\D/g, ""));
	if (isNaN(animeId))
		return -1;
	else
		return animeId;
}

function showEpisodesSummary(ep) {
	var animeId = parseAnimeId(ep);
	if (animeId < 0) {
		alert("showEpisodesSummary: Invalid animeId");
		return;
	}

	var parentEl = $("[id^=e"+animeId+"]");
	var selCount = 0;
	var selTime = 0;
	var selCountWatched = 0;
	var selTimeWatched = 0;

	$(parentEl).filter(".selected").each(
		function() {
			selCount++;
			var epTime = $("#" + this.id + "_len").eq(0).text().replace(/\D/g, "");
			epTime = parseInt(epTime); //.text.replace(/\D/g, ""));
			selTime += epTime;
			if ($.trim($("#" + this.id + "_w").eq(0).text()) == "Y") {
				selCountWatched++;
				selTimeWatched += epTime;
			}
		}
	);

	var comment = "";
	if (selCount > 0) {
		comment += "Selected: " + selCount + " episodes with " + selTime + " minutes";
		if (selCountWatched > 0)
			comment += " (" + selCountWatched + " watched with " + selTimeWatched + " minutes)";
	}
	$("#es" + animeId + "_comment").eq(0).text(comment);
}

function doEpisodeCommentClick(el) {
	var animeId = parseAnimeId(el);
	if (animeId < 0)
		return;

	var parentEl = $("[id^=e"+animeId+"]");

	if ($(el).text().length > 0) {
		// clear all episode selections
		$(parentEl).filter(".selected").removeClass("selected");
	} else {
		// select all episodes
		$(parentEl).filter(".ep").addClass("selected");
	}

	showEpisodesSummary(el);
}

function showAllDetails(showing) {
	// toggle showing details for all animes
	$("body").addClass("waiting");
	$("[animeId]").each(function (i) {
		toggleAnimeDetails(this, showing);
	});
	$("body").removeClass("waiting");
}

function toggleAnimeDetails(animeEl, doShow) {
	var animeId = parseAnimeId(animeEl.id);
	var episodesEl = $("#es" + animeId).get(0);

	if (typeof episodesEl == "undefined" ) {
		alert("No anime selected");
		return;
	}

	if (typeof doShow == "undefined")
		doShow = 	$(episodesEl).hasClass("invisible");
	
	if (doShow) {
		// show
		if ($(episodesEl).children().length > 1) {
			// already loaded
			$(episodesEl).removeClass("invisible");
		} else {
			// load
			if (transformationAnimeDetails(animeId, episodesEl)) {
				// make episodes selectable for summary
				$(episodesEl).find(".ep").click(function() { doSelectEpisode(this) });

				// de-/select all episodes
				$(episodesEl).find(".ep_comment").click( function () { doEpisodeCommentClick(this);	} );

				$(episodesEl).removeClass("invisible");
			} else
				alert("Transformation of episodes failed!");
		}
	} else {
		// hide
		$(episodesEl).addClass("invisible");
	}
}

function fixAwardImages() {
	try {
		// workaround for award-image-tags not being XHTML and browsers who do not support "disable-output-escaping" (like FF)
		$(".awards").each(function() {
			if (this.childNodes.length == 0 || this.firstChild.nodeType != 3)
				return;

			var imageTags = this.firstChild.nodeValue.split(">");
			if (imageTags.length == 0)
				return;

			this.removeChild(this.firstChild);
			for (var i = 0; i < imageTags.length; i++) {
				imageTags[i] = imageTags[i].toLowerCase().replace(/<img | $/g,"");
				if (imageTags[i].length < 5)
					continue;

				var attributes = imageTags[i].match(/([^\s]*)="([^"]*)"/g); //(/([a-z]+)="(^["]*)"/g);
				if (attributes.length > 0) {
					var imgElement = document.createElement("img");
					for (var ia = 0; ia < attributes.length; ia++) {
						attrValues = attributes[ia].split("=");
						attrValues[1] =  attrValues[1].replace(/(^")|("$)/g, "");
						imgElement.setAttribute(attrValues[0], attrValues[1].replace(/(^")|("$)/g, ""));
					}
					this.appendChild(imgElement, this.firstChild);
				}
			}
		});
	} catch (e) {
	}
}

// -----------------------------------------------------------------------------
// on load
$().ready(function () {
	// generate HTML content from XML/XSL data
	if (!transformation())
		return false;
});
