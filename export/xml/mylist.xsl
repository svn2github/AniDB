<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	<xsl:output method="html" indent="no"/>

	<!-- the page -->
	<xsl:template match="mylist">
		<html>
			<head>
				<title>MyList</title>
				<link rel="stylesheet" type="text/css" href="mylist.css" />
				<base target="anime_details" />
			</head>
			<body>
				<h1>MyList of <xsl:value-of select="/*/global/user"/></h1>

				<p>
					<h2>My anime titles</h2>
					<xsl:call-template name="animelist"/>
				</p>

				<p class="footer">
					<span title="Source: {/*/global/animedburl}">Data exported on <xsl:value-of select="/*/global/date"/></span>
				</p>
			</body>
		</html>
	</xsl:template>

	<!-- the table -->
	<xsl:template name="animelist">
		<table>
			<thead>
				<tr>
					<td class="wide">Title</td>
					<td title="watched / completed / total">Episodes</td>
					<td>Size</td>
				</tr>
			</thead>

			<tbody>
				<xsl:for-each select="anime">
					<xsl:sort select="name"/>
					<tr title="AniDB ID = {id}">
						<xsl:choose>
							<xsl:when test="@complete!='' and @watched!=''">
								<xsl:attribute name="class">complete_watched</xsl:attribute>
							</xsl:when>
							<xsl:when test="@complete!=''">
								<xsl:attribute name="class">complete</xsl:attribute>
							</xsl:when>
							<xsl:when test="@watched!=''">
								<xsl:attribute name="class">watched</xsl:attribute>
							</xsl:when>
						</xsl:choose>
						<td>
							<a href="anime/a{id}.xml">
								<xsl:value-of select="name"/>
								<xsl:if test="year!=''"> (<xsl:value-of select="year"/>)</xsl:if>
							</a>
						</td>
						<td>
							<xsl:value-of select="my_watchedeps"/> / <xsl:value-of select="my_eps"/> / <xsl:value-of select="eps"/>
						</td>
						<td>
							<xsl:value-of select="my_size_h"/>
						</td>
					</tr>
				</xsl:for-each>
			</tbody>

			<tfoot>
				<tr>
					<td>
						<xsl:value-of select="/*/global/animecount"/> titles
					</td>
					<td>
						<xsl:value-of select="/*/global/epcount"/> episodes<br />
						<xsl:value-of select="/*/global/filecount"/> files
					</td>
					<td>
						<xsl:value-of select="/*/global/bytecount_h"/>
					</td>
				</tr>
			</tfoot>
		</table>
	</xsl:template>

</xsl:stylesheet>