<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" encoding="UTF-8" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd" doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN" indent="yes"/>

<xsl:param name="typeFilter" select="'*'"/>
<xsl:param name="watchedFilter" select="-1"/>
<xsl:param name="ratingFilter" select="-1"/>
<xsl:param name="completeFilter" select="-1"/>
<xsl:param name="mainLang" select="'*'"/>
<xsl:param name="maxRows" select="9999999"/>
<xsl:param name="optionsVisible" select="0"/>

<xsl:template match="AC">
		<xsl:call-template name="Options"/>
		<table width="100%" border="0">
			<colgroup width="1%">
				<col/>
				<col width="86%"/>
				<col span="13"/>
			</colgroup>
			<thead>
				<xsl:call-template name="Headers"/>
			</thead>
			<tfoot>
				<xsl:call-template name="Headers"/>
			</tfoot>
			<tbody>
				<xsl:apply-templates select="A[($typeFilter='*' or T=$typeFilter) and ($ratingFilter&lt;0 or floor(@r)=$ratingFilter) and ($watchedFilter=-1 or ($watchedFilter=0 and @we=0) or ($watchedFilter=1 and @we&gt;0) or ($watchedFilter=2 and @we&gt;0 and @w=0) or ($watchedFilter=3 and @w=1)) and ($completeFilter=-1 or (@c=$completeFilter)) and position()&lt;=$maxRows]">
					<xsl:sort select="N" data-type="text" order="ascending" />
				</xsl:apply-templates>
			</tbody>
		</table>
</xsl:template>


<xsl:template name="Options">
		<div id="optionsheader">Options</div>
		<div id="options">
			<xsl:if test="$optionsVisible = 0"><xsl:attribute name="class">invisible</xsl:attribute></xsl:if>
			<table width="100%" border="0" class="options">
				<tr>
					<td class="optionlabel">Show only type: <xsl:call-template name="typeList"/></td>
					<td class="optionlabel">Show only rating: <xsl:call-template name="ratingList"/></td>
					<td class="optionlabel">Main language: <xsl:call-template name="languageList"/></td>
					<td class="optionbutton" onclick="doFilterComplete();" title="Toggle: incomplete only / complete only / Showing all in/complete">
						<xsl:choose>
							<xsl:when test="$completeFilter=0">Showing incomplete only</xsl:when>
							<xsl:when test="$completeFilter=1">Showing complete only</xsl:when>
							<xsl:otherwise>Showing all in/complete</xsl:otherwise>
						</xsl:choose>
						<input type="hidden" id="completeFilter" value="{$completeFilter}"/>
					</td>
					<td class="optionbutton" onclick="showAllDetails(true);">Show Details</td>
					<td class="optionbutton" onclick="showAllDetails(false);">Close Details</td>
				</tr>
			</table>
			<table width="100%" border="0" class="options">
				<tr>
					<td class="optionlabel">Watched:<input type="hidden" id="watchedFilter" value="{$watchedFilter}"/></td>
					<td class="optionbutton" onclick="doFilterWatched(0)"><xsl:if test="$watchedFilter=0"><xsl:attribute name="class">optionbutton selected</xsl:attribute></xsl:if>Not yet</td>
					<td class="optionbutton" onclick="doFilterWatched(1)"><xsl:if test="$watchedFilter=1"><xsl:attribute name="class">optionbutton selected</xsl:attribute></xsl:if>At least one</td>
					<td class="optionbutton" onclick="doFilterWatched(2)"><xsl:if test="$watchedFilter=2"><xsl:attribute name="class">optionbutton selected</xsl:attribute></xsl:if>Partially</td>
					<td class="optionbutton" onclick="doFilterWatched(3)"><xsl:if test="$watchedFilter=3"><xsl:attribute name="class">optionbutton selected</xsl:attribute></xsl:if>Completely</td>
				</tr>
			</table>
		</div>
</xsl:template>

<xsl:template name="Headers">
				<tr>
					<th class="sortable" sortField="@i" sortType="number" sortOrder="">#</th>
					<th class="sortable" sortField="N" sortType="text" sortOrder="">Name</th>
					<th class="sortable" sortField="@y" sortType="text" sortOrder="">Year</th>
					<th class="sortable" sortField="T" sortType="text" sortOrder="">Type</th>
					<th class="sortable" sortField="@e" sortType="number" sortOrder="" title="Standard episodes">Ep</th>
					<th class="sortable" sortField="@es" sortType="number" sortOrder="" title="Special episodes">EpS</th>
					<th class="sortable" sortField="@we" sortType="number" sortOrder="" title="Watched standard episodes">W-Ep</th>
					<th class="sortable" sortField="@ws" sortType="number" sortOrder="" title="Watched special episodes">W-EpS</th>
					<th class="sortable" sortField="@c" sortType="text" sortOrder="" title="Anime is complete">C</th>
					<th class="sortable" sortField="@w" sortType="text" sortOrder="" title="Anime completely watched">W-C</th>
					<th class="sortable" colspan="2" sortField="@r" sortType="number" sortOrder="" title="Average rating and total votes">Rating</th>
					<th class="sortable" colspan="2" sortField="@rt" sortType="number" sortOrder="" title="Average temporary rating and total votes">Temp R.</th>
					<th class="sortable" colspan="2" sortField="@rr" sortType="number" sortOrder="" title="Average review ratings and total number of reviews">Reviews</th>
					<th class="sortable" sortField="count(W)" sortType="text" sortOrder="">Awards</th>
				</tr>
</xsl:template>


<xsl:template match="A">
	<xsl:param name="pos" select="position()"/>
	<xsl:param name="title">
		<xsl:choose>
			<xsl:when test="count(NO[@i=$mainLang]) &gt; 0"><xsl:value-of select="NO[@i=$mainLang]"/></xsl:when>
			<xsl:otherwise><xsl:value-of select="N"/></xsl:otherwise>
		</xsl:choose>
	</xsl:param>
				<tr id="a{@i}" animeId="{@i}">
					<xsl:choose>
						<xsl:when test="$pos mod 2 = 0"><xsl:attribute name="class">even hover toggle_ep</xsl:attribute></xsl:when>
						<xsl:otherwise><xsl:attribute name="class">odd hover toggle_ep</xsl:attribute></xsl:otherwise>
					</xsl:choose>
					<td class="right"><xsl:value-of select="$pos"/></td>
					<td class="left"><xsl:value-of select="$title"/> &#160;(<a target="anidb" href="http://anidb.net/a{@i}" onclick="event.stopPropagation();">&#35;<xsl:value-of select="@i"/></a>)</td>
					<td class="right"><xsl:value-of select="@y"/></td>
					<td><xsl:value-of select="T"/></td>
					<td class="right"><xsl:value-of select="@e"/></td>
					<td class="right"><xsl:if test="@es &gt; 0"><xsl:value-of select="@es"/></xsl:if></td>
					<td class="right"><xsl:if test="@we &gt; 0"><xsl:value-of select="@we"/></xsl:if></td>
					<td class="right"><xsl:if test="@ws &gt; 0"><xsl:value-of select="@ws"/></xsl:if></td>
					<td>
						<xsl:choose>
							<xsl:when test="@c = 0">&#160;</xsl:when>
							<xsl:otherwise>Y</xsl:otherwise>
						</xsl:choose>
					</td>
					<td>
						<xsl:choose>
							<xsl:when test="@w = 0">&#160;</xsl:when>
							<xsl:otherwise>Y</xsl:otherwise>
						</xsl:choose>
					</td>
					<td class="left"><xsl:value-of select="@r"/></td>
					<td class="right">&#160;<xsl:if test="@v &gt; 0">(<xsl:value-of select="@v"/>)</xsl:if></td>
					<td class="left"><xsl:value-of select="@rt"/></td>
					<td class="right">&#160;<xsl:if test="@vt &gt; 0">(<xsl:value-of select="@vt"/>)</xsl:if></td>
					<td class="left"><xsl:value-of select="@rr"/></td>
					<td class="right">&#160;<xsl:if test="@vr &gt; 0">(<xsl:value-of select="@vr"/>)</xsl:if></td>
					<td class="awards"><xsl:apply-templates select="W[string-length(@n) &gt; 0]"/></td>
				</tr>
				<tr id="es{@i}" class="invisible"></tr>
</xsl:template>


<xsl:template match="W">
						<img src="anidb_award.gif" title="Award: {@n}"/>
</xsl:template>


<xsl:key name="key-types" match="A/T" use="text()" />

<xsl:template name="typeList">
	<select size="1" id="typeList" onchange="doFilterType(this.value)">
		<option value="*">(all)</option>		
		<xsl:for-each select="A/T[count(. | key('key-types', text())[1]) = 1]">
			<xsl:sort select="text()" data-type="text" order="ascending"/>
			<option value="{text()}">
				<xsl:if test="text() = $typeFilter"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>
			<xsl:value-of select="text()" /></option>		
		</xsl:for-each>
	</select>
</xsl:template>


<xsl:template name="ratingList">
	<select size="1" id="ratingList" onchange="doFilterRating(this.value);">
		<option value="-1">(all)</option>
		<xsl:if test="count(A[floor(@r) = 0])">
			<option value="0"><xsl:if test="$ratingFilter = 0"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>0.x</option>
		</xsl:if>				
		<xsl:if test="count(A[floor(@r) = 1])">
			<option value="1"><xsl:if test="$ratingFilter = 1"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>1.x</option>
		</xsl:if>				
		<xsl:if test="count(A[floor(@r) = 2])">
			<option value="2"><xsl:if test="$ratingFilter = 2"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>2.x</option>
		</xsl:if>				
		<xsl:if test="count(A[floor(@r) = 3])">
			<option value="3"><xsl:if test="$ratingFilter = 3"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>3.x</option>
		</xsl:if>				
		<xsl:if test="count(A[floor(@r) = 4])">
			<option value="4"><xsl:if test="$ratingFilter = 4"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>4.x</option>
		</xsl:if>				
		<xsl:if test="count(A[floor(@r) = 5])">
			<option value="5"><xsl:if test="$ratingFilter = 5"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>5.x</option>
		</xsl:if>				
		<xsl:if test="count(A[floor(@r) = 6])">
			<option value="6"><xsl:if test="$ratingFilter = 6"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>6.x</option>
		</xsl:if>				
		<xsl:if test="count(A[floor(@r) = 7])">
			<option value="7"><xsl:if test="$ratingFilter = 7"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>7.x</option>
		</xsl:if>				
		<xsl:if test="count(A[floor(@r) = 8])">
			<option value="8"><xsl:if test="$ratingFilter = 8"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>8.x</option>
		</xsl:if>				
		<xsl:if test="count(A[floor(@r) = 9])">
			<option value="9"><xsl:if test="$ratingFilter = 9"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>9.x</option>
		</xsl:if>				
		<xsl:if test="count(A[@r &gt;= 10])">
			<option value="10"><xsl:if test="$ratingFilter = 10"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>10.x</option>
		</xsl:if>				
	</select>
</xsl:template>


<xsl:key name="key-languages" match="A/NO" use="@n" />

<xsl:template name="languageList">
	<select size="1" id="languageList" onchange="doSetMainLanguage(this.value)">
		<option value="*">(default)</option>
		<xsl:for-each select="A/NO[count(. | key('key-languages', @n)[1]) = 1]">
			<xsl:sort select="@n" data-type="text" order="ascending"/>
			<option value="{@i}">
				<xsl:if test="@i = $mainLang"><xsl:attribute name="selected">selected</xsl:attribute></xsl:if>
			<xsl:value-of select="@n" /></option>		
		</xsl:for-each>
	</select>
</xsl:template>


</xsl:stylesheet>