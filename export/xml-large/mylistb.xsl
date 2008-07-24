<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" encoding="UTF-8"/>
  <xsl:param name="sortNodeName" select="'type'"/>
  <xsl:param name="sortDirection" select="'descending'"/>
  <xsl:param name="sortDataType" select="'text'"/>
  <xsl:param name="showOVA" select="'true'"/>
  <xsl:param name="showTV" select="'true'"/>
  <xsl:param name="showMovie" select="'true'"/>
  <xsl:param name="showOther" select="'true'"/>
  <xsl:param name="showIncomplete" select="'true'"/>
  <xsl:param name="mainLanguage" select="'romanji'"/>
  <xsl:template match="animeList">
    <xsl:variable name="oppositeLanguage">
      <xsl:call-template name="FlipLanguage"/>
    </xsl:variable>
    <table width="100%">
      <thead>
        <tr class="thead">
          <th>#</th>
          <xsl:call-template name="CreateHeaderCell">
            <xsl:with-param name="displayName">
              <div class="headerRight"></div>
              <div class="headerLeft switch" 
                   onClick="unlinkParentEvent(this);myListRenderer.update({{'mainLanguage': '{$oppositeLanguage}'}});">
                <span class="sub">
                  Switch to <xsl:call-template name="LanguageChoice">
                    <xsl:with-param name="flip" select="'true'"/>
                  </xsl:call-template>
                </span>
              </div>
              <div class="headerCenter">
                Name&#160;&#160;&#160;<span class="sub">
                  <xsl:call-template name="LanguageChoice"/>
                </span>
              </div>
            </xsl:with-param>
            <xsl:with-param name="paramName" select="'name'"/>
            <xsl:with-param name="paramDataType" select="'text'"/>
          </xsl:call-template>
          <xsl:call-template name="CreateHeaderCell">
            <xsl:with-param name="displayName" select="'Type'"/>
            <xsl:with-param name="paramName" select="'type'"/>
            <xsl:with-param name="paramDataType" select="'text'"/>
          </xsl:call-template>
          <xsl:call-template name="CreateHeaderCell">
            <xsl:with-param name="displayName" select="'Broadcast Date'"/>
            <xsl:with-param name="paramName" select="'broadcastDate'"/>
            <xsl:with-param name="paramDataType" select="'text'"/>
          </xsl:call-template>
          <xsl:call-template name="CreateHeaderCell">
            <xsl:with-param name="displayName" select="'Anidb Rating'"/>
            <xsl:with-param name="paramName" select="'rating'"/>
            <xsl:with-param name="paramDataType" select="'number'"/>
          </xsl:call-template>
          <xsl:call-template name="CreateHeaderCell">
            <xsl:with-param name="displayName" select="'Files'"/>
            <xsl:with-param name="paramName" select="'ownEpisodes'"/>
            <xsl:with-param name="paramDataType" select="'number'"/>
          </xsl:call-template>
          <xsl:call-template name="CreateHeaderCell">
            <xsl:with-param name="displayName" select="'Last Addition Date'"/>
            <xsl:with-param name="paramName" select="'lastAdditionDate'"/>
            <xsl:with-param name="paramDataType" select="'text'"/>
          </xsl:call-template>
        </tr>
      </thead>
      <tbody>
        <xsl:apply-templates select="anime[@type[(. != 'OVA' or $showOVA = 'true') and
                    (. != 'TV Series' or $showTV = 'true') and (. != 'Movie' or $showMovie = 'true') and
                    (. = 'TV Series' or . = 'OVA' or . = 'Movie' or $showOther = 'true')] and
                    (@status = 'complete' or $showIncomplete = 'true')]">
          <xsl:sort select="(romanjiName[$mainLanguage = 'romanji' or ../englishName =  ''] | 
                    englishName[$mainLanguage = 'english' and . !=  ''])['name' = $sortNodeName] | 
                    (*|@*)[name() = $sortNodeName]" data-type="{$sortDataType}" 
                    order="{$sortDirection}"/>
        </xsl:apply-templates>
      </tbody>
    </table>
  </xsl:template>

  <xsl:template name="LanguageChoice">
    <xsl:param name="flip"/>    
    <xsl:choose>
      <xsl:when test="($flip and $mainLanguage != 'romanji') or (not($flip) and $mainLanguage  = 'romanji')">
        Romanji&#160;&#160;(<i>English</i>)
      </xsl:when>
      <xsl:otherwise>
        English&#160;&#160;(<i>Romanji</i>)
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="FlipLanguage">
    <xsl:choose>
      <xsl:when test="$mainLanguage = 'romanji'">
        <xsl:value-of select="'english'"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="'romanji'"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="CreateHeaderCell">
    <xsl:param name="displayName"/>
    <xsl:param name="paramName"/>
    <xsl:param name="paramDataType"/>
    <xsl:variable name="sortOrder">
      <xsl:choose>
        <xsl:when test="$paramName != $sortNodeName">
          <xsl:value-of select="$sortDirection"/>
        </xsl:when>
        <xsl:when test="$sortDirection = 'descending'">ascending</xsl:when>
        <xsl:otherwise>descending</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="cssClass">
      <xsl:if test="$paramName = $sortNodeName"><xsl:value-of select="$sortOrder"/></xsl:if>
    </xsl:variable>
    <th onMouseOver="over(this, 'sort {$cssClass}')" onMouseOut="out(this)" class="{$cssClass}" 
        onClick="myListRenderer.update({{'sortNodeName': '{$paramName}', 'sortDataType': '{$paramDataType}','sortDirection': '{$sortOrder}'}})">
      <xsl:copy-of select="$displayName"/>
       </th>
  </xsl:template>

  <xsl:template match="*[@totalEpisodes and @ownEpisodes]" mode="fileCount">
    <xsl:value-of select="@ownEpisodes"/>
    /
    <xsl:value-of select="@totalEpisodes"/>
  </xsl:template>

  <xsl:template match="anime">
    <xsl:variable name="position" select="position()"/>
    <tr onMouseOver="overChangeClass(this, 'high1')" onMouseOut="outChangeClass(this)">
      <xsl:choose>
        <xsl:when test="$position mod 2">
          <xsl:attribute name="class">even</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="class">odd</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <td align="center" onClick="myListRenderer.selectShow('a{@id}')" class="clickable">
        <xsl:value-of select="$position"/>
      </td>
      <td align="left" onClick="myListRenderer.selectShow('a{@id}')" class="clickable">
        <xsl:choose>
          <xsl:when test="englishName =  '' or englishName = romanjiName">
            <xsl:value-of select="romanjiName"/>
          </xsl:when>
          <xsl:when test="$mainLanguage = 'romanji'">
            <xsl:value-of select="romanjiName"/>&#160;&#160;(<i><xsl:value-of select="englishName"/></i>)
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="englishName"/>&#160;&#160;(<i>
              <xsl:value-of select="romanjiName"/>
            </i>)
          </xsl:otherwise>
        </xsl:choose>
        -
        <xsl:value-of select="@completed = 'completedByPreferredGroup'"/>
        <xsl:value-of select="@completed"/>
      </td>
      <td align="center">
        <xsl:value-of select="@type"/>
      </td>
      <td align="center">
        <xsl:value-of select="@broadcastDate"/>
      </td>
      <td align="center">
        <xsl:value-of select="@rating"/>
      </td>
      <td align="center">
          <xsl:if test="@status = 'complete'">
              <xsl:attribute name="class">complete</xsl:attribute>
          </xsl:if>
        <b>
          <xsl:apply-templates select="." mode="fileCount"/>
        </b> + 
        <xsl:choose>
          <xsl:when test="specialEpisodes/@totalEpisodes != 0">
            <i>
              <xsl:apply-templates select="specialEpisodes" mode="fileCount"/>
            </i>
          </xsl:when>
          <xsl:otherwise>
            <i>0 / 0</i>
          </xsl:otherwise>
        </xsl:choose>
      </td>
      <td align="center">
        <xsl:value-of select="@lastAdditionDate"/>
      </td>
    </tr>
    <tr >
      <td onClick="myListRenderer.selectShow('a{@id}')" class="clickable close"></td>
      <td id="r2-a{@id}" colSpan="6"/>
    </tr>
    <tr>
      <td id="r3-a{@id}" colSpan="7"  onClick="myListRenderer.selectShow('a{@id}')" class="clickable close"/> 
    </tr>
  </xsl:template>
</xsl:stylesheet>