<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" encoding="UTF-8" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd" doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN" indent="yes"/>

<xsl:param name="animeId" select="-1"/>
<xsl:param name="mainLang" select="'*'"/>

<xsl:template match="AC">
	<xsl:for-each select="A[@i=$animeId]">
		<xsl:call-template name="Episodes"/>
	</xsl:for-each>
</xsl:template>


<xsl:template name="Episodes">
					<td>&#160;</td>
					<td colspan="16" class="left">
						<xsl:if test="count(NO) &gt; 0">
							<table>
								<xsl:if test="count(NO[@i=2]) &gt; 0">
									<xsl:apply-templates select="NO[@i=2]"/>
								</xsl:if>
								<xsl:if test="count(NO[@i=4]) &gt; 0">
									<xsl:apply-templates select="NO[@i=4]"/>
								</xsl:if>
								<xsl:if test="count(NO[@i=$mainLang and @i!=2 and @i!=4]) &gt; 0">
									<xsl:apply-templates select="NO[@i=$mainLang]"/>
								</xsl:if>
								<xsl:apply-templates select="NO[@i!=2 and @i!=4 and @i!=$mainLang]">
									<xsl:sort select="@n" data-type="text" order="ascending"/>
								</xsl:apply-templates>
							</table>
						</xsl:if>
						<table>
							<thead>
								<tr>
									<th>#</th>
									<th>Episode Title</th>
									<th title="Length in minutes">Len</th>
									<th title="Watched">W</th>
								</tr>
							</thead>
							<tfoot>
								<tr>
									<th id="es{@i}_count" title="Number of episodes"><xsl:value-of select="count(E)"/></th>
									<th id="es{@i}_comment" class="ep_comment">&#160;</th>
									<th id="es{@i}_wtime" title="Time of watched episodes"><xsl:value-of select="sum(E[@w &gt; 0]/@l)"/></th>
									<th id="es{@i}_wcount" title="Number of watched episodes"><xsl:value-of select="count(E[@w &gt; 0])"/></th>
								</tr>
							</tfoot>
							<tbody>
								<xsl:apply-templates select="E"/>
							</tbody>
						</table>
					</td>
</xsl:template>


<xsl:template match="E">
	<xsl:variable name="pos" select="position()"/>
								<tr id="e{../@i}_{@i}">
									<xsl:choose>
										<xsl:when test="$pos mod 2 = 0"><xsl:attribute name="class">ep even hover</xsl:attribute></xsl:when>
										<xsl:otherwise><xsl:attribute name="class">ep odd hover</xsl:attribute></xsl:otherwise>
									</xsl:choose>
									<td id="e{../@i}_{@i}_num" class="right"><xsl:value-of select="@i"/></td>
									<td id="e{../@i}_{@i}_name" class="left"><xsl:value-of select="text()"/></td>
									<td id="e{../@i}_{@i}_len" class="right"><xsl:value-of select="@l"/> min</td>
									<td id="e{../@i}_{@i}_w">
										<xsl:choose>
											<xsl:when test="@w = 0">&#160;</xsl:when>
											<xsl:otherwise>Y</xsl:otherwise>
										</xsl:choose>
									</td>
								</tr>
</xsl:template>


<xsl:template match="NO">
								<tr>
									<td class="left capitalize" onclick="doSetMainLanguage({@i})"><xsl:value-of select="@n"/>:</td>
									<td class="left"><xsl:value-of select="text()"/></td>
								</tr>
</xsl:template>


</xsl:stylesheet>