<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" encoding="UTF-8" cdata-section-elements="romanji english"/>
    <xsl:template match="myList">
        <myList>
            <xsl:copy-of select="@user"/>
            <xsl:copy-of select="@creationDate"/>
            <xsl:apply-templates select="animeList"/>

        </myList>
    </xsl:template>
    <xsl:template match="animeList">
        <animeList>
            <xsl:copy-of select="@count"/>
            <xsl:copy-of select="@episodes"/>
            <xsl:copy-of select="@files"/>
            <xsl:copy-of select="@size"/>
            <xsl:apply-templates select="anime"/>
        </animeList>
    </xsl:template>
    <xsl:template match="anime">
        <anime>
            <xsl:copy-of select="@id"/>
            <xsl:apply-templates select="seriesInfo"/>
            <xsl:apply-templates select="episodes"/>
        </anime>
    </xsl:template>
    <xsl:template match="episodes">
        <episodes>
            <count>
                <xsl:copy-of select="count/@Total"/>
                <own>
                    <xsl:copy-of select="count/own/@Total"/>
                </own>
            </count>
            <xsl:apply-templates select="episode"/>
        </episodes>
    </xsl:template>
    <xsl:template match="episode">
        <episode>
            <xsl:copy-of select="@id"/>
            <files>
                <xsl:apply-templates select="files/file"/>
            </files>
        </episode>
    </xsl:template>
    <xsl:template match="file">
        <file>
            <xsl:copy-of select="@id"/>
            <dates>
                <xsl:copy-of select="dates/@additionDate"/>
            </dates>
        </file>
    </xsl:template>
    <xsl:template match="seriesInfo">
        <seriesInfo>
            <xsl:apply-templates select="name"/>
            <xsl:copy-of select="type"/>
            <xsl:copy-of select="rating"/>
            <airingDate>
                <xsl:copy-of select="airingDate/start"/>
            </airingDate>
        </seriesInfo>
    </xsl:template>
    <xsl:template match="name">
        <name>
            <xsl:copy-of select="romanji"/>
            <xsl:copy-of select="english"/>
        </name>
    </xsl:template>

</xsl:stylesheet>
