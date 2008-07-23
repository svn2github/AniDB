<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" encoding="UTF-8"/>
  <xsl:key name="companiesByType" match="company" use="@typeId"/>
  <xsl:template match="anime">
    <h3>
      <xsl:value-of select="seriesInfo/name/romanji"/>
    </h3>
    <xsl:apply-templates select="seriesInfo"/>
    <xsl:apply-templates select="episodes"/>
  </xsl:template>
  <xsl:template match="seriesInfo">
    <dl class="seriesInfo">
      <dt>Series Info</dt>
      <dd>
        <xsl:apply-templates select="name"/>
        <xsl:apply-templates select="genres"/>
        <xsl:apply-templates select="airingDate"/>
        <xsl:apply-templates select="type"/>
        <xsl:apply-templates select="rating"/>
        <xsl:apply-templates select="awards"/>
        <xsl:apply-templates select="companies"/>
        <xsl:apply-templates select="synopsis"/>
      </dd>
    </dl>
  </xsl:template>

  <xsl:template match="episodes">
    <table width="100%">
      <tr onMouseOver="overChangeClass(this, 'high1')" onMouseOut="outChangeClass(this)"
          onClick="myListRenderer.toggle('broadcastDate', 'text')">
      </tr>
    </table>
  </xsl:template>

  <xsl:template match="name[other != '' or count(synonym/alias) > 0]" mode="otherNames">
    <dt>Other Names</dt>
    <dd>
      <ul>
        <xsl:apply-templates select="other" mode="nonEmptyElementInLi"/>
        <xsl:apply-templates select="synonym/alias" mode="nonEmptyElementInLi"/>
      </ul>
    </dd>
  </xsl:template>

  <xsl:template match="companies">
    <dl>

    </dl>
  </xsl:template>

  <xsl:template match="name[count(shorts/short) > 0]" mode="shortNames">
    <dt>Short Names</dt>
    <dd>
      <ul>
        <xsl:apply-templates select="shorts/short" mode="nonEmptyElementInLi"/>
      </ul>
    </dd>
  </xsl:template>

  <xsl:template match="name">
    <dl class="name">
      <xsl:apply-templates select="romanji" mode="nonEmptyElementInDDWithDT">
        <xsl:with-param name="DTContent">Romanji Name</xsl:with-param>
      </xsl:apply-templates>
      <xsl:apply-templates select="english" mode="nonEmptyElementInDDWithDT">
        <xsl:with-param name="DTContent">English Name</xsl:with-param>
      </xsl:apply-templates>
      <xsl:apply-templates select="kanji" mode="nonEmptyElementInDDWithDT">
        <xsl:with-param name="DTContent">Kanji Name</xsl:with-param>
      </xsl:apply-templates>
      <xsl:apply-templates select="." mode="otherNames"/>
      <xsl:apply-templates select="." mode="shortNames"/>
    </dl>
  </xsl:template>
  <xsl:template match="node()[. != '']" mode="nonEmptyElementInDDWithDT">
    <xsl:param name="DTContent"/>
    <dt>
      <xsl:copy-of select="$DTContent"/>
    </dt>
    <xsl:apply-templates select="." mode="elementInDD"/>
  </xsl:template>
  <xsl:template match="node()[. != '']" mode="nonEmptyElementInLi">
    <li>
      <xsl:value-of select="."/>
    </li>
  </xsl:template>
  <xsl:template match="node()" mode="elementInDD">
    <dd>
      <xsl:copy-of select="."/>
    </dd>
  </xsl:template>
  <xsl:template match="genres"/>
  <xsl:template match="airingDate">
    <dl class="airingDate">
      <dt>Aired From</dt>
      <dd>
        <xsl:value-of select="@start"/>
        to
        <xsl:value-of select="@end"/>
      </dd>
    </dl>
  </xsl:template>
  <xsl:template match="type">
    <dl class="type">
      <dt>Anime Type</dt>
      <xsl:apply-templates select="." mode="elementInDD"/>
    </dl>
  </xsl:template>
  <xsl:template match="rating">
    <dl class="rating">
      <dt>Anidb Rating</dt>
      <xsl:apply-templates select="." mode="elementInDD"/>
    </dl>
  </xsl:template>
  <xsl:template match="awards">
    <span>This anime has won the following awards</span>
    <dl>
      <xsl:apply-templates select="awardTypes/awardType"/>
    </dl>
  </xsl:template>
  <xsl:template match="awardType">
    <dt>
      <a href="{normalize-space(ancestor::awards/award[@awardTypeId = current()/@id]/awardURL)}">
        <xsl:copy-of select="."/>
      </a>
    </dt>
    <dd>
      <ul>
        <xsl:apply-templates select="ancestor::awards/award[@awardTypeId = ./@id]"/>
      </ul>
    </dd>
  </xsl:template>

  <xsl:template match="award">
    <xsl:apply-templates select="awardName" mode="nonEmptyElementInLi"/>
  </xsl:template>

  <xsl:template match="companies">
    <span>The following companies were involved in the production</span>
    <dl>
      <xsl:apply-templates select="company[count(. | key('companiesByType', @typeId)[1]) = 1]"/>
    </dl>
  </xsl:template>

  <xsl:template match="company">
    <dt>
      <xsl:value-of select="type"/>
    </dt>
    <dd>
      <ul>
        <xsl:apply-templates select="key('companiesByType',@typeId)/name" mode="nonEmptyElementInLi"/>
      </ul>
    </dd>
  </xsl:template>

  <xsl:template match="synopsis">
    <dl class="synopsis">
      <dt>Synopsis</dt>
      <dd>
        <xsl:apply-templates select="." mode="blah"/>
      </dd>
    </dl>
  </xsl:template>

  <xsl:template match="text()" mode="blah">
    <xsl:value-of disable-output-escaping="yes" select="."/>
  </xsl:template>

  <xsl:template priority="-1"
                match="@* | * | text() | processing-instruction() |
comment()" mode="blah">
    <!-- Identity transformation. -->
    <xsl:copy>
      <xsl:apply-templates
           select="@* | * | text() | processing-instruction() | comment()" mode="blah"/>
    </xsl:copy>
  </xsl:template>
  
</xsl:stylesheet>
