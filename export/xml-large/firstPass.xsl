<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" version="1.0" omit-xml-declaration="no"/>
  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="@broadcastDate">
    <xsl:attribute name="broadcastDate">
      <xsl:apply-templates select="." mode="displayXmlDate"/>
    </xsl:attribute>
  </xsl:template>

  <xsl:template match="anime">
    <xsl:copy>
      <xsl:attribute name="lastAdditionDate">
        <xsl:apply-templates select="episodes/episode/files/file/@additionDate" mode="selectFirstDate">
          <xsl:sort order="descending" select="substring(normalize-space(.),7,4)" data-type="number"/>
          <xsl:sort order="descending" select="substring(normalize-space(.),4,2)" data-type="number"/>
          <xsl:sort order="descending" select="substring(normalize-space(.),1,2)" data-type="number"/>
        </xsl:apply-templates>
      </xsl:attribute>
      <xsl:variable name="completenessStatus">
        <xsl:choose>
          <xsl:when test="@status='incomplete'">
            <xsl:apply-templates select="." mode="incompleteStatus"/>
          </xsl:when>
          <xsl:otherwise>
            allCompleteShows
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:attribute name="completenessStatus"><xsl:value-of select="normalize-space($completenessStatus)"/></xsl:attribute>
      <xsl:apply-templates select="@*|node()[not(self::episodes) and not(self::groups)]"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="anime" mode="incompleteStatus">
    <xsl:choose>
      <xsl:when test="groups/group[@state = 'complete' or @state = 'finished']">
        <xsl:apply-templates select="." mode="incompleteWithCompletedGroupStatus"/>
      </xsl:when>
      <xsl:otherwise>
        incompletableShows
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="anime" mode="incompleteWithCompletedGroupStatus">
    <xsl:variable name="completedGroups" select="groups/group[@state = 'complete' or @state = 'finished']"/>
    <xsl:variable name="lastEpisode">
      <xsl:apply-templates select="episodes/episode/@number[boolean(number(.))]" mode="selectFirstEpisode">
        <xsl:sort order="descending" data-type="number"/>
      </xsl:apply-templates>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="episodes/episode[@number = $lastEpisode]/files/file[@releasedBy = $completedGroups/@id]">
        showsCompletableByPreferredGroup
      </xsl:when>
      <xsl:otherwise>
        showsCompletableByGroupInMyList
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="@*" mode="selectFirstEpisode">
    <xsl:if test="position() = 1">
      <xsl:value-of select="."/>
    </xsl:if>
  </xsl:template>

  <xsl:template match="@*" mode="selectFirstDate">
    <xsl:if test="position() = 1">
      <xsl:apply-templates select="." mode="displayXmlDate"/>
    </xsl:if>
  </xsl:template>

  <xsl:template match="@*" mode="displayXmlDate">
    <xsl:value-of select="substring(normalize-space(.),7,4)"/>.<xsl:value-of select="substring(normalize-space(.),4,2)"/>.<xsl:value-of select="substring(normalize-space(.),1,2)"/>
  </xsl:template>
</xsl:stylesheet>