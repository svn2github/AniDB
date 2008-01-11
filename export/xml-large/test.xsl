<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" encoding="UTF-8"/>
  <xsl:template match="/root">
    <dl>
      <xsl:apply-templates select="item">
        <xsl:sort select="number[not(current()/number &gt; .)]" data-type="number"/>
      </xsl:apply-templates>
    </dl>
  </xsl:template>

  <xsl:template match="item">
    <dt>
      <xsl:value-of select="@name"/>
      &#160; whose max value is &#160;
      <xsl:value-of select="number[not(current()/number &gt; .)]"/>
    </dt>
    <xsl:apply-templates select="number"/>
  </xsl:template>

  <xsl:template match="number">
    <dd>
      <xsl:value-of select="."/>
    </dd>
  </xsl:template>
</xsl:stylesheet>
