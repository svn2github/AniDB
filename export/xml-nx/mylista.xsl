<?xml version="1.0" encoding="utf-8"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" encoding="UTF-8" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd" doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN" indent="yes"/>

<xsl:template match="/">
<html>
<head>
	<title><xsl:value-of select="AC/@u"/>'s animelist: <xsl:value-of select="AC/@n"/> animes with <xsl:value-of select="AC/@e"/> episodes. Updated <xsl:value-of select="AC/@t"/></title>
	<link rel="stylesheet" type="text/css" href="mylist.css" />
	<script type="text/javascript" src="jquery.js"></script>
	<script type="text/javascript" src="mylist.js"></script>
</head>

<body>
	<div id="result"></div>
</body>
</html>
</xsl:template>

</xsl:stylesheet>