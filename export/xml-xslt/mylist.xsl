<!--
An XSLT for my list page
© 2005 by Siddhi Pittayachawan
Version 0.1
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="list">
	<html>
		<head>
			<title><xsl:value-of select="userinfo/name"/>'s anime list</title>
			<link rel="stylesheet" type="text/css" href="style.css"/>
			<script type="text/javascript">
				function display(targetId) {
					var target = document.getElementById(targetId)
					if(targetId == "animelist") {
						document.getElementById("userinfo").className = "hiddencontent"
					} else if(targetId == "userinfo") {
						document.getElementById("animelist").className = "hiddencontent"
					}
					target.className = "showcontent"
				}
			</script>
		</head>
		<body>
			<br/><div>&lt;
				<p class="link" onmouseover="this.className='hover'" onmouseout="this.className='link'" onclick="display('animelist')">My List</p> &gt; &lt; <p class="link" onmouseover="this.className='hover'" onmouseout="this.className='link'" onclick="display('userinfo')">My Info</p> &gt;
			</div><br/>
			<xsl:apply-templates select="userinfo"/>
			<div id="animelist" class="showcontent">
				<table>
					<colgroup/>
					<colgroup class="columnspace" span="6"/>
					<tr>
						<th>Title</th>
						<th>Year</th>
						<th>Episode</th>
						<th>Watched</th>
						<th>Rating</th>
						<th>Review</th>
						<th>Size</th>
						<th>State</th>
					</tr>
					<xsl:for-each select="anime">
						<xsl:variable name="progress">
							<xsl:choose>
								<xsl:when test="epnumber/own=epnumber/total">complete</xsl:when>
								<xsl:otherwise>incomplete</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>
						<tr class="{$progress}" onmouseover="this.className='hover'" onmouseout="this.className='{$progress}'">
							<td><a class="listlink" href="anime/a{id}.xml"><xsl:value-of select="name"/></a></td>
							<td><xsl:value-of select="year"/></td>
							<xsl:apply-templates select="epnumber"/>
							<xsl:apply-templates select="vote"/>
							<xsl:apply-templates select="review"/>
							<td><xsl:value-of select="size"/></td>
							<td >
								<xsl:choose>
									<xsl:when test="state=0">
										<img src="http://www.anidb.net/pics/anidb_state_unknown.gif" alt="state: unknown" title="state: unknown" width="15" height="13"/>
									</xsl:when>
									<xsl:when test="state=1">
										<img src="http://www.anidb.net/pics/anidb_state_onhdd.gif" alt="state: on hdd" title="state: on hdd" width="15" height="13"/>
									</xsl:when>
									<xsl:when test="state=2">
										<img src="http://www.anidb.net/pics/anidb_state_oncd.gif" alt="state: on cd" title="state: on cd" width="15" height="13"/>
									</xsl:when>
									<xsl:when test="state=3">
										<img src="http://www.anidb.net/pics/anidb_state_deleted.gif" alt="state: deleted" title="state: deleted" width="15" height="13"/>
									</xsl:when>
									<xsl:when test="state=4">
										<img src="http://www.anidb.net/pics/anidb_state_shared.gif" alt="state: shared" title="state: shared" width="15" height="13"/>
									</xsl:when>
									<xsl:when test="state=5">
										<img src="http://www.anidb.net/pics/anidb_state_release.gif" alt="state: release" title="state: release" width="15" height="13"/>
									</xsl:when>
								</xsl:choose>
								<xsl:apply-templates select="award"/>
							</td>
						</tr>
					</xsl:for-each>
				</table>
			</div>
			<p>List created with <a href="http://www.anidb.info" onmouseover="this.className='hover'" onmouseout="this.className=''">AniDB</a> export feature</p>
		</body>
	</html>
</xsl:template>

<xsl:template match="userinfo">
<div id="userinfo" class="hiddencontent">
	<table>
		<tr>
			<td>User:</td>
			<td><xsl:value-of select="name"/></td>
		</tr>
		<tr>
			<td>Created:</td>
			<td><xsl:value-of select="translate(export,'.','/')"/></td>
		</tr>
		<tr>
			<td>Anime:</td>
			<td><xsl:value-of select="anime"/></td>
		</tr>
		<tr>
			<td>Episodes:</td>
			<td><xsl:value-of select="ep"/></td>
		</tr>
		<tr>
			<td>Files:</td>
			<td><xsl:value-of select="file"/></td>
		</tr>
		<tr>
			<td>Size:</td>
			<td><xsl:value-of select="size"/></td>
		</tr>
	</table>
</div>
</xsl:template>

<xsl:template match="award">
	<xsl:if test="name = 'AniDB Top 10'">
		<img src="http://www.anidb.net/pics/anidb_rate_star.gif" width="15" height="13" alt="AniDB Top 10" title="AniDB Top 10"/>
	</xsl:if>
	<xsl:if test="translate(name,'0123456789','') = 'ARC Awards '">
		<img src="http://www.anidb.net/pics/anidb_araward.gif" width="15" height="13" alt="ARC Awards" title="ARC Awards"/>
	</xsl:if>
</xsl:template>

<xsl:template match="epnumber">
	<td>
		<xsl:value-of select="own"/>/<xsl:value-of select="total"/>
		<xsl:if test="ownsp &gt; 0">
			+ <xsl:value-of select="ownsp"/>
		</xsl:if>
	</td>
	<td>
		<xsl:value-of select="watch"/>/<xsl:value-of select="own"/>
		<xsl:if test="ownsp &gt; 0">
			+ <xsl:value-of select="watchsp"/>/<xsl:value-of select="ownsp"/>
		</xsl:if>
	</td>
</xsl:template>

<xsl:template match="vote">
	<td>
		<xsl:value-of select="rating"/> (<xsl:value-of select="voter"/>)
	</td>
</xsl:template>

<xsl:template match="review">
	<td>
		<xsl:value-of select="rating"/> (<xsl:value-of select="reviewer"/>)
	</td>
</xsl:template>

</xsl:stylesheet>
