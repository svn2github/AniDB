<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="html" encoding="UTF-8"/>
    <xsl:param name="sortNodeName" select="'type'"/>
    <xsl:param name="sortDirection" select="'descending'"/>
    <xsl:param name="sortDataType" select="'text'"/>
    <xsl:param name="showOVA" select="'true'"/>
    <xsl:param name="showTv" select="'true'"/>
    <xsl:param name="showMovie" select="'true'"/>
    <xsl:param name="showOther" select="'true'"/>
    <xsl:param name="showIncomplete" select="'true'"/>
    <xsl:template match="animeList">

        <table width="100%">
            <tr class="thead">
                <td>#</td>
                <td onClick="sortTable('romanjiName', 'text')" onMouseOver="over(this, 'sort')"
                    onMouseOut="out(this)">
                    Name
                </td>
                <td onClick="sortTable('type', 'text')" onMouseOver="over(this, 'sort')"
                    onMouseOut="out(this)">
                    Type
                </td>
                <td onClick="sortTable('broadcastDate', 'text')" onMouseOver="over(this, 'sort')"
                    onMouseOut="out(this)">
                    Broadcast Date
                </td>
                <td onClick="sortTable('rating', 'number')" onMouseOver="over(this, 'sort')"
                    onMouseOut="out(this)">
                    Anidb Rating
                </td>
                <td onClick="sortTable('files', 'number')" onMouseOver="over(this, 'sort')"
                    onMouseOut="out(this)">
                    Files
                </td>
                <td onClick="sortTable('additionDate', 'text')" onMouseOver="over(this, 'sort')"
                    onMouseOut="out(this)">
                    Last Addition Date
                </td>
            </tr>
            <tbody>
                <xsl:apply-templates select="anime[@type[(. != 'OVA' or $showOVA = 'true') and
                    (. != 'TV Series' or $showTv = 'true') and (. != 'Movie' or $showMovie = 'true') and
                    (. = 'TV Series' or . = 'OVA' or . = 'Movie' or $showOther = 'true')] and
                    (@status = 'complete' or $showIncomplete = 'true')]">
                    <xsl:sort select="romanjiName[$sortNodeName = 'romanjiName'] | @type[$sortNodeName = 'type'] | @broadcastDate[$sortNodeName = 'broadcastDate']
                    | @rating[$sortNodeName = 'rating'] | episodeCount/normal/@ownEpisodes[$sortNodeName = 'files'] | @lastAdditionDate[$sortNodeName = 'additionDate']"
                              data-type="{$sortDataType}" order="{$sortDirection}"/>
                </xsl:apply-templates>
            </tbody>
        </table>
    </xsl:template>

    <xsl:template match="episodeCount">
        <td align="center">
            <b>
                <xsl:apply-templates select="normal"/>
            </b>
            <xsl:if test="special/@totalEpisodes != 0">
                +
                <i>
                    <xsl:apply-templates select="special"/>
                </i>
            </xsl:if>
        </td>
    </xsl:template>

    <xsl:template match="*[@totalEpisodes and @ownEpisodes]">
        <xsl:value-of select="@ownEpisodes"/>
        /
        <xsl:value-of select="@totalEpisodes"/>
    </xsl:template>

    <xsl:template match="anime">
        <xsl:variable name="selected" select="0"/>
        <xsl:variable name="position" select="position()"/>

        <tr onMouseOver="over2(this, 'high1')" onMouseOut="out2(this)">
            <xsl:choose>
                <xsl:when test="position() mod 2">
                    <xsl:attribute name="class">even</xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:attribute name="class">odd</xsl:attribute>
                </xsl:otherwise>
            </xsl:choose>

            <td align="center" style="cursor:pointer;">
                <xsl:value-of select="$position"/>
            </td>
            <td align="left">
                <xsl:value-of select="romanjiName"/>
                <xsl:if test="englishName !=  ''">
                    /
                    <xsl:value-of select="englishName"/>
                </xsl:if>
            </td>
            <td align="center">
                <xsl:value-of select="@type"/>
            </td>
            <td align="center">
                <xsl:value-of select="@broadcastDate"/>
            </td>
            <td align="center">
                <xsl:value-of select="@rating"/>
            </td>
            <xsl:apply-templates select="episodeCount"/>
            <td align="center">
                <xsl:value-of select="@lastAdditionDate"/>
            </td>
        </tr>
        <xsl:if test="position()=$selected">
            <xsl:for-each select="D/E/F">
                <tr class="dir" onMouseOver="over2(this, 'high2');window.status='State: {S}'"
                    onMouseOut="out2(this);window.status=''">
                    <td align="center">
                        <xsl:value-of select="position()"/>
                    </td>
                    <td colspan="3">
                        <a href="http://anidb.info/f{@i}" target="_blank">
                            <xsl:value-of select="N"/>
                        </a>
                    </td>
                    <td align="center">
                        <a href="ed2k://|file|{N}|{@l}|{D}|/">ed2k</a>
                    </td>
                    <td align="right">
                        <xsl:value-of select="@l"/>
                    </td>
                </tr>
            </xsl:for-each>
        </xsl:if>

    </xsl:template>
</xsl:stylesheet>