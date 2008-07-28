<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" encoding="UTF-8"/>
  <xsl:key name="companiesByType" match="company" use="@typeId"/>
  <xsl:variable name="animeId" select="//anime/@id"/>
  <xsl:template match="anime">
    <h3>
      <xsl:value-of select="seriesInfo/name/romanji"/>
    </h3>
    <xsl:apply-templates select="seriesInfo"/>
    <xsl:apply-templates select="episodes"/>
  </xsl:template>
  <xsl:template match="seriesInfo">
    <table class="seriesInfo">
      <xsl:apply-templates select="name"/>
      <xsl:apply-templates select="airingDate"/>
      <xsl:apply-templates select="type"/>
      <xsl:apply-templates select="rating"/>
      <xsl:apply-templates select="awards"/>
      <xsl:apply-templates select="companies"/>
    </table>
    <xsl:apply-templates select="synopsis"/>
  </xsl:template>

  <xsl:template match="group">
    <tr onMouseOver="overChangeClass(this, 'high2')" onMouseOut="outChangeClass(this)">
      <xsl:choose>
        <xsl:when test="(position()+1) mod 2">
          <xsl:attribute name="class">oddEpisodeTable</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="class">evenEpisodeTable</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:variable name="episodes" select="../../episode[.//releasedBy/@id = current()/@id]"/>
      <xsl:variable name="numericEpisodes" select="$episodes[boolean(number(@number))]"/>

      <td class="clickable" onClick="divDisplayer.toggleDiv('togglingDiv{$animeId}-{@id}')">
        <xsl:apply-templates select="name" mode="htmlContent"/> (<xsl:apply-templates select="shortName" mode="htmlContent"/>)
      </td>
      <td>
        <b>
          <xsl:value-of select="count($numericEpisodes)"/>/<xsl:value-of select="releasedEpisodes/@normal"/>
        </b> +
        <i>
          <xsl:value-of select="count($episodes) - count($numericEpisodes)"/>/<xsl:value-of select="releasedEpisodes/@special"/>
        </i>
      </td>
      <td>
        <xsl:call-template name="findRange">
          <xsl:with-param name="numericEpisodes" select="$numericEpisodes"/>
        </xsl:call-template> / <xsl:value-of select="releasedEpisodes/@range"/>
      </td>
      <td>
        <xsl:value-of select="@state"/>
      </td>
    </tr>
    <xsl:call-template name="episodes">
      <xsl:with-param name="files" select="../../episode/files/file[releasedBy/@id = current()/@id]"/>
      <xsl:with-param name="id" select="@id"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="episodes">
    <br/>
    <h3>Episodes Summary</h3>
    <table class="innerTable">
      <tr>
        <th>
          All Episodes
        </th>
        <th>
          Total
        </th>
        <th>Range</th>
        <th>Status</th>
      </tr>
      <tr class="oddEpisodeTable" onMouseOver="overChangeClass(this, 'high2')" onMouseOut="outChangeClass(this)">
        <td class="clickable" onClick="divDisplayer.toggleDiv('togglingDiv{$animeId}-{0}')" >
          Owned Episodes
        </td>
        <td>
          <b>
            <xsl:value-of select="episodeCount/normal/@ownEpisodes"/>/<xsl:value-of select="episodeCount/normal/@totalEpisodes"/>
          </b> +
          <i>
            <xsl:value-of select="episodeCount/special/@ownEpisodes"/>/<xsl:value-of select="episodeCount/special/@totalEpisodes"/>
          </i>
        </td>
        <td>
          <xsl:call-template name="findRange">
            <xsl:with-param name="numericEpisodes" select="episode[boolean(number(@number))]"/>
          </xsl:call-template>
        </td>
        <td>
          <xsl:value-of select="@status"/>
        </td>
      </tr>
      <xsl:call-template name="episodes">
        <xsl:with-param name="files" select="episode/files/file"/>
        <xsl:with-param name="id" select="'0'"/>
      </xsl:call-template>
      <xsl:apply-templates select="groups/group"/>
    </table>
  </xsl:template>

  <xsl:template name="findRange">
    <xsl:param name="numericEpisodes"/>
    <xsl:if test="not($numericEpisodes)">0</xsl:if>
    <xsl:variable name="range">
      <xsl:apply-templates select="$numericEpisodes[1]" mode="range">
        <xsl:with-param name="numericEpisodes" select="$numericEpisodes[position() > 1]"/>
      </xsl:apply-templates>
    </xsl:variable>
    <xsl:value-of select="translate(normalize-space($range),' ','')"/>
  </xsl:template>

  <xsl:template match="episode" mode="range">
    <xsl:param name="numericEpisodes"/>
    <xsl:param name="previousEpisodeNo"/>
    <xsl:variable name="nextEpisode" select="$numericEpisodes[1]"/>
    <xsl:choose>
      <xsl:when test="not($previousEpisodeNo)">
        <xsl:value-of select="@number"/>
      </xsl:when>
      <xsl:when test="($previousEpisodeNo + 1) &lt; @number">
        , <xsl:value-of select="@number"/>
      </xsl:when>
      <xsl:when test="not($nextEpisode) or $nextEpisode/@number &gt; @number+1">
        - <xsl:value-of select="@number"/>
      </xsl:when>
    </xsl:choose>
    <xsl:apply-templates select="$nextEpisode" mode="range">
      <xsl:with-param name="numericEpisodes" select="$numericEpisodes[position() != 1]"/>
      <xsl:with-param name="previousEpisodeNo" select="@number"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="episode" mode="terminate">
    <xsl:param name="previous_episode"/>
    <xsl:choose>
      <xsl:when test="$previous_episode">
        <xsl:value-of select="@number"/>
      </xsl:when>
      <xsl:when test="$previous_episode + 1 = @number">
        -<xsl:value-of select="@number"/>
      </xsl:when>
      <xsl:otherwise>
        ,<xsl:value-of select="@number"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="episode" mode="startRange">
    <xsl:value-of select="@number"/>
  </xsl:template>

  <xsl:template name="episodes">
    <xsl:param name="files"/>
    <xsl:param name="id"/>
    <tr>
      <td colSpan="4">
        <div class="invisible" id="togglingDiv{$animeId}-{$id}">
          <h3>Files</h3>
          <table class="innerTable">
            <thead>
              <tr>
                <th>Episode</th>
                <th>Episode Name</th>
                <th>Released On</th>
                <th>Downloaded On</th>
                <th>Released By</th>
              </tr>
            </thead>
            <xsl:apply-templates select="$files"/>
          </table>
          <br/>
          <br/>
        </div>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="file">
    <tr>
      <xsl:choose>
        <xsl:when test="position() mod 2">
          <xsl:attribute name="class">oddFileTable</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="class">evenFileTable</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <td>
        <xsl:value-of select="../../@number"/>
      </td>
      <td>
        <xsl:value-of select="../../name/english"/>
      </td>
      <td>
        <xsl:value-of select="dates/@releaseDate"/>
      </td>
      <td>
        <xsl:value-of select="dates/@additionDate"/>
      </td>
      <td>
        <xsl:apply-templates select="releasedBy"/>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="releasedBy">
    <xsl:choose>
      <xsl:when test="@id = 0">
        Unknown Group
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="name"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="name[other != '' or count(synonym/alias) > 0]" mode="otherNames">
    <tr>
      <td class="left">Other Names</td>
      <td>
        <ul>
          <xsl:apply-templates select="other" mode="elementInLi"/>
          <xsl:apply-templates select="synonym/alias" mode="elementInLi"/>
        </ul>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="name[count(shorts/short) > 0]" mode="shortNames">
    <tr>
      <td class="left">Short Names</td>
      <td>
        <ul>
          <xsl:apply-templates select="shorts/short" mode="elementInLi"/>
        </ul>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="name">
    <xsl:apply-templates select="romanji" mode="nonEmptyElementInTableRow">
      <xsl:with-param name="CellContent">Romanji Name</xsl:with-param>
    </xsl:apply-templates>
    <xsl:apply-templates select="english" mode="nonEmptyElementInTableRow">
      <xsl:with-param name="CellContent">English Name</xsl:with-param>
    </xsl:apply-templates>
    <xsl:apply-templates select="kanji" mode="nonEmptyElementInTableRow">
      <xsl:with-param name="CellContent">Kanji Name</xsl:with-param>
    </xsl:apply-templates>
    <xsl:apply-templates select="." mode="otherNames"/>
    <xsl:apply-templates select="." mode="shortNames"/>
  </xsl:template>

  <xsl:template match="node()[. != '']" mode="nonEmptyElementInTableRow">
    <xsl:param name="CellContent"/>
    <tr>
      <td class="left">
        <xsl:value-of select="$CellContent"/>
      </td>
      <td>
        <xsl:value-of select="."/>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="node()[. != '']" mode="elementInDD">
    <dd>
      <xsl:value-of select="."/>
    </dd>
  </xsl:template>

  <xsl:template match="node()[. != '']" mode="elementInLi">
    <li>
      <xsl:value-of select="."/>
    </li>
  </xsl:template>

  <xsl:template match="genres"/>

  <xsl:template match="airingDate">
    <tr>
      <td class="left">Aired From</td>
      <td>
        <xsl:value-of select="@start"/>
      </td>
    </tr>
    <tr>
      <td class="left">Aired Till</td>
      <td>
        <xsl:value-of select="@end"/>
      </td>
    </tr>
  </xsl:template>
  <xsl:template match="type">
    <tr>
      <td class="left">Anime Type</td>
      <td>
        <xsl:value-of select="."/>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="rating">
    <tr>
      <td class="left">Anidb Rating</td>
      <td>
        <xsl:value-of select="."/>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="awards">
    <tr>
      <td class="left">Awards</td>
      <td>
        <xsl:apply-templates select="awardTypes/awardType"/>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="awardType">
    <xsl:choose>
      <xsl:when test="ancestor::awards/award[@awardTypeId = current()/@id]">
        <dl>
          <dt>
            <a href="{normalize-space(ancestor::awards/award[@awardTypeId = current()/@id]/awardURL)}">
              <xsl:value-of select="."/>
            </a>
          </dt>
          <xsl:apply-templates select="ancestor::awards/award[@awardTypeId = current()/@id]"/>
        </dl>
      </xsl:when>
      <xsl:otherwise>
        <a href="{normalize-space(ancestor::awards/award[@awardTypeId = current()/@id]/awardURL)}">
          <xsl:value-of select="."/>
        </a>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="award">
    <xsl:apply-templates select="awardName" mode="elementInDD"/>
  </xsl:template>

  <xsl:template match="companies">
    <tr>
      <td class="left">Companies</td>
      <td>
        <xsl:apply-templates select="company[count(. | key('companiesByType', @typeId)[1]) = 1]"/>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="company">
    <dd>
      <dl>
        <dt>
          <xsl:value-of select="type"/>
        </dt>
        <xsl:apply-templates select="key('companiesByType',@typeId)/name" mode="elementInDD"/>
      </dl>
    </dd>
  </xsl:template>

  <xsl:template match="synopsis">
    <h3>Synopsis</h3>
    <p>
      <xsl:apply-templates select="." mode="htmlContent"/>
    </p>
  </xsl:template>

  <xsl:template match="text()" mode="htmlContent">
    <xsl:value-of disable-output-escaping="yes" select="."/>
  </xsl:template>
</xsl:stylesheet>
