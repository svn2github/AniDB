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
      <xsl:if test="@status='incomplete'">
        <xsl:variable name="lastEpisode">
          <xsl:apply-templates select="episodes/episode/@number" mode="selectFirstEpisode">
            <xsl:sort order="descending" data-type="number"/>
          </xsl:apply-templates>
        </xsl:variable>
        <xsl:attribute name="completed">
          <xsl:value-of select="$lastEpisode"/> 
        </xsl:attribute> 
<!--        <xsl:if test="groups/group[@state = 'complete' or @state = 'finished']">-->
<!--          <xsl:attribute name="completeByGroupInMyList">true</xsl:attribute>-->
<!--        </xsl:if>-->
<!--        <xsl:apply-templates select="episodes/episode" mode="selectFirstEpisodesGroupStatus">-->
<!--          <xsl:sort select="@number" order="descending" data-type="number"/>-->
<!--          <xsl:with-param name="groups" select="groups"/>-->
<!--        </xsl:apply-templates>-->
<!--        <xsl:apply-templates select="episodes/episode" mode="selectFirstEpisode">-->
<!--          <xsl:sort select="@number" order="descending" data-type="number"/>-->
<!--        </xsl:apply-templates>-->
      </xsl:if>
      <xsl:apply-templates select="@*|node()[not(self::episodes) and not(self::groups)]"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="*" mode="selectFirstEpisodesGroupStatus">
    <xsl:param name="groups"/>
    <xsl:if test="(position() = 1) and files/file[@releasedBy = $groups/group/@id[../@state = 'complete' or ../@state = 'finished']]">
      <xsl:attribute name="completeByGroupWhoDidLastEp">true</xsl:attribute>
    </xsl:if>
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