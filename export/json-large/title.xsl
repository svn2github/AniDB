<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" version="1.0" omit-xml-declaration="no"/>
  <xsl:template match="myList">
    <title>
      <xsl:value-of select="@user"/>'s animelist: <xsl:value-of select="animeList/@count"/> anime titles with <xsl:value-of select="animeList/@episodes"/> episodes. Updated <xsl:value-of select="@creationDate"/>
    </title>
  </xsl:template>
</xsl:stylesheet>