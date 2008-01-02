<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="html" encoding="UTF-8"/>

    <xsl:template match="myList">

        <html lang="en">
            <head>
                <title>
                    <xsl:value-of select="@user"/>
                    's animelist: Created on
                    <xsl:value-of select="@creationDate"/>
                    consisting of
                    <xsl:apply-templates select="animeList"/>
                </title>
                <link rel="stylesheet" type="text/css" href="mylist.css"/>
                <script type="text/javascript" src="helper.js"></script>
                 <script type="text/javascript" src="mylista.js"></script>
                 <script type="text/javascript" src="xsltTools.js"></script>
            </head>
            <body>
                <table width="100%" border="0">
                    <tr class="options">
                        <td>Options:</td>
                        <td>
                            Sort Direction
                            <span id="sortDirection"></span>
                        </td>
                    </tr>
                </table>
                <div id="resultDiv"></div>
                <noscript>
                    <p>
                        This page uses scripted XSLT transformations. Ya need IE/Win, Mozilla or Netscape 7.1.
                    </p>
                </noscript>
            </body>
        </html>
    </xsl:template>
    <xsl:template match="animeList">
        <xsl:value-of select="@count"/>
        animes in
        <xsl:value-of select="@files"/>
        files taking
        <xsl:value-of select="@size"/>
        .
    </xsl:template>
    <xsl:template name="CheckBoxInTD">
        <xsl:param name="checkBoxText"/>
        <xsl:param name="checkBoxId"/>
        <td>
            <input type="checkbox" id="{$checkBoxId}" onclick="javascript:toggleElements(this,'mainRow')"/>
            <xsl:value-of select="$checkBoxText"/>
        </td>
    </xsl:template>
</xsl:stylesheet>