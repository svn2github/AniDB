<?xml version="1.0" encoding="utf-8"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" encoding="UTF-8" /> 

<xsl:template match="/">

<html lang="en">
<head>

<title>
<xsl:value-of select="AC/@u"/>'s animelist: 
<xsl:value-of select="AC/@n"/> animes in 
<xsl:value-of select="AC/@m"/> files taking
<xsl:value-of select="AC/@s"/>. Updated 
<xsl:value-of select="AC/@t"/>
</title>

<link rel="stylesheet" type="text/css" href="mylist.css" />

<script type="text/javascript" src="mylista.js"></script>

</head>

<body>

<div id="resultDiv"></div>

<noscript><p>
This page uses scripted XSLT transformations. Ya need IE/Win, Mozilla or Netscape 7.1.
</p></noscript>

</body>
</html>
</xsl:template>
</xsl:stylesheet>