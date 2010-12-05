<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:template match="filters">
    <div id="menu">
      <ul>
        <li>
          Options :
        </li>
      </ul>
      <xsl:apply-templates select="dropDownFilters"/>
      <xsl:apply-templates select="checkBoxFilters/checkBoxFilter"/>
    </div>
  </xsl:template>

  <xsl:template match="dropDownFilters">
    <ul>
      <li>
        Filters <span>&#8595;</span>
        <ul>
          <xsl:apply-templates select="dropDownFilter"/>
        </ul>
      </li>
    </ul>
  </xsl:template>

  <xsl:template match="checkBoxFilter">
    <ul>
      <li>
        <xsl:variable name="idString">
          <xsl:apply-templates select="." mode="idString"/>
        </xsl:variable>
        <input type="checkbox" id="{$idString}" onclick="myListRenderer.update({{'{$idString}': this.checked.toString()}})" checked="checked"/>
        <label for="{$idString}">
          <xsl:value-of select="."/>
        </label>
      </li>
    </ul>
  </xsl:template>

  <xsl:template match="dropDownFilter">
    <li>
      <xsl:variable name="idString">
        <xsl:apply-templates select="." mode="idString"/>
      </xsl:variable>
      <input type="radio" id="{$idString}" name="filter" onclick="myListRenderer.update({{'filter': '{$idString}'}})">
        <xsl:if test="@checked">
          <xsl:attribute name="checked">checked</xsl:attribute>
        </xsl:if>
      </input>
      <label for="{$idString}">
        <xsl:value-of select="."/>
      </label>
    </li>
  </xsl:template>

  <xsl:template match="*" mode="idString">
    <xsl:variable name="withoutSpaces" select="translate(.,' ','')"/>
    <xsl:variable name="firstChar" select="translate(substring($withoutSpaces,1,1),'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')"/>
    <xsl:value-of select="concat($firstChar,substring($withoutSpaces,2))"/>
  </xsl:template>
</xsl:stylesheet>