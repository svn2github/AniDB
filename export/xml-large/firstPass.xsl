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
                <xsl:apply-templates select="episodes/episode/files/file/@additionDate" mode="selectFirst">
                    <xsl:sort order="descending" select="substring(normalize-space(.),7,4)" data-type="number"/>
                    <xsl:sort order="descending" select="substring(normalize-space(.),4,2)" data-type="number"/>
                    <xsl:sort order="descending" select="substring(normalize-space(.),1,2)" data-type="number"/>
                </xsl:apply-templates>
            </xsl:attribute>
            <xsl:apply-templates select="@*|node()[not(self::episodes)]"/>
        </xsl:copy>
    </xsl:template>
    <xsl:template match="@*" mode="selectFirst">
        <xsl:if test="position() = 1">
            <xsl:apply-templates select="." mode="displayXmlDate"/>
        </xsl:if>
    </xsl:template>
    <xsl:template match="@*" mode="displayXmlDate">
        <xsl:value-of select="substring(normalize-space(.),7,4)"/>.<xsl:value-of select="substring(normalize-space(.),4,2)"/>.<xsl:value-of select="substring(normalize-space(.),1,2)"/>
    </xsl:template>
</xsl:stylesheet>