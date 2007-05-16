<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	<xsl:output method="html" indent="no"/>

	<!-- the page -->
	<xsl:template match="anime">
		<html>
			<head>
				<link rel="stylesheet" type="text/css" href="../anime.css" />
			</head>
			<body>
				<p>
					<h2>Details</h2>
					<xsl:call-template name="details"/>
				</p>
				<p>
					<h2>Episodes</h2>
					<xsl:call-template name="episodelist"/>
				</p>
			</body>
		</html>
	</xsl:template>

	<!-- details -->
	<xsl:template name="details">
		<table>
			<thead>
				<tr><td>Attribute</td><td class="wide">Value</td></tr>
			</thead>
			<tbody>
				<tr><td>AniDB ID</td><td><xsl:value-of select="id"/></td></tr>
				<tr><td>Title(s)</td><td>
					<xsl:for-each select="name | title_alias | title_short">
						<xsl:value-of select="."/>
						<xsl:if test="position() &lt; last()"><b> | </b></xsl:if>
					</xsl:for-each>
				</td></tr>

				<tr><td>Genre(s)</td><td>
					<xsl:for-each select="genre">
						<xsl:value-of select="."/>
						<xsl:if test="position() &lt; last()"><b> | </b></xsl:if>
					</xsl:for-each>
				</td></tr>

				<tr><td>Producer</td><td><xsl:value-of select="producer"/></td></tr>
				<tr><td>URL</td><td><xsl:value-of select="url"/></td></tr>
				<!--<tr><td>Other</td><td class="wrap"><xsl:value-of select="other"/></td></tr>-->
				<tr><td>Type</td><td><xsl:value-of select="type_name"/></td></tr>
				<tr><td>Rating</td><td><xsl:value-of select="rating"/> (<xsl:value-of select="votes"/> votes)</td></tr>
				<tr><td>Review rating</td><td><xsl:value-of select="reviewrating"/> (<xsl:value-of select="reviews"/> reviews)</td></tr>
				<tr><td>Episodes</td><td title="watched / completed / total"><xsl:value-of select="my_watchedeps"/> / <xsl:value-of select="my_eps"/> / <xsl:value-of select="eps"/></td></tr>
				<tr><td>Downloaded</td><td><xsl:value-of select="my_size_h"/></td></tr>
			</tbody>
		</table>
	</xsl:template>

	<!-- episode table -->
	<xsl:template name="episodelist">
		<table>
			<thead>
				<tr>
					<td class="wide">Name</td>
					<td>Length</td>
				</tr>
			</thead>

			<tbody>
				<xsl:for-each select="episode">
					<tr>
						<xsl:choose>
							<xsl:when test="@hasfile!='' and @iswatched!=''">
								<xsl:attribute name="class">complete_watched</xsl:attribute>
							</xsl:when>
							<xsl:when test="@hasfile!=''">
								<xsl:attribute name="class">complete</xsl:attribute>
							</xsl:when>
							<xsl:when test="@iswatched!=''">
								<xsl:attribute name="class">watched</xsl:attribute>
							</xsl:when>
							<xsl:otherwise>
								<xsl:attribute name="class">attention</xsl:attribute>
							</xsl:otherwise>
						</xsl:choose>
						<td>
							<a href="#" target="_blank">
								<xsl:value-of select="name"/>
							</a>
						</td>
						<td>
							<xsl:value-of select="length"/>
							<xsl:if test="aired=''">*</xsl:if>
						</td>
					</tr>

					<xsl:for-each select="file">
						<tr title="File ID = {fid}">
							<xsl:choose>
								<xsl:when test="@iswatched!=''">
									<xsl:attribute name="class">complete_watched</xsl:attribute>
								</xsl:when>
								<xsl:otherwise>
								</xsl:otherwise>
							</xsl:choose>

							<td colspan="100" title="Group ID = {group_id}">
								<xsl:text disable-output-escaping="yes">&amp;nbsp;&amp;nbsp;</xsl:text>

								<a href="{ed2k_link}">
									Size: <xsl:value-of select="size"/>, Quality: <xsl:value-of select="qual_name"/>
									<xsl:if test="state_crcok">CRC-OK, </xsl:if>
									<xsl:if test="state_crcfailed">CRC-Failed, </xsl:if>
								</a>

								<xsl:if test="group_id>0">
									<a href="{/*/global/animedburl}?show=group&amp;gid={group_id}" target="_blank" title="{group_name}">
										<b> | </b>[<xsl:value-of select="group_shortname"/>]
									</a>
								</xsl:if>

								<xsl:if test="source!=''"><b> | </b>Source: <xsl:value-of select="source"/></xsl:if>
								<xsl:if test="storage!=''"><b> | </b>Storage: <xsl:value-of select="storage"/></xsl:if>
								<b> | </b>My state: <xsl:value-of select="mystate_string"/>
								(<xsl:value-of select="mystate_icon" disable-output-escaping="yes"/>)
							</td>
						</tr>
					</xsl:for-each>
				</xsl:for-each>
			</tbody>

			<tfoot>
				<tr>
					<td colspan="100">
						<xsl:value-of select="my_size_h"/>
					</td>
				</tr>
			</tfoot>
		</table>
	</xsl:template>

</xsl:stylesheet>