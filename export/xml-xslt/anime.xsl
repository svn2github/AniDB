<!--
An XSLT for anime info page
© 2005 by Siddhi Pittayachawan
Version 0.1
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="anime">
	<html>
		<head>
			<title><xsl:value-of select="name/romanji"/></title>
			<link rel="stylesheet" type="text/css" href="../style.css"/>
			<script type="text/javascript">
				function display(targetId) {
					var target = document.getElementById(targetId)
					if(target.className == "hiddencontent") {
						target.className = "showcontent"
					} else {
						target.className = "hiddencontent"
					}
				}
				
				function displayall() {
					var target = document.getElementsByName("fdetail")
					for(i = 1; i != target.length+1; i++) {
						display("ep"+i)
					}
				}
			</script>
		</head>
		<body>
			<p>&#60; Anime Info &#62; &#60; <a href="../mylist.xml" onmouseover="this.className='hover'" onmouseout="this.className=''">Back to My List</a> &#62; &#60; Link : <a href="http://anidb.info/perl-bin/animedb.pl?show=anime&#38;aid={id}"><img src="http://www.anidb.net/favicon.ico" width="16" height="16" alt="AniDB icon" title="Info at AniDB"/></a>
			<xsl:if test="url/anidb != ''">
				<a href="{url/anidb}"><img src="http://www.animenfo.com/favicon.ico" width="16" height="16" alt="AnimeNfo icon" title="Info at AnimeNfo"/></a>
			</xsl:if>
			<xsl:if test="url/animeplanet != ''">
				<a href="{url/animeplanet}"><img src="http://www.anime-planet.com/favicon.ico" width="16" height="16" alt="Anime-Planet icon" title="Recommended similar anime at Anime-Planet"/></a>
			</xsl:if>
			&#62;</p>
			<table id="animeinfo">
				<xsl:apply-templates select="name"/>
				<xsl:apply-templates select="genres"/>
				<tr>
					<td>Type:</td>
					<td><xsl:value-of select="type"/></td>
				</tr>
				<xsl:apply-templates select="epnumber"/>
				<xsl:apply-templates select="broadcast"/>
				<tr>
					<td>Producer:</td>
					<td><xsl:value-of select="producer"/></td>
				</tr>
				<xsl:apply-templates select="feedback"/>
				<xsl:apply-templates select="review"/>
				<tr>
					<td>Synopsis:</td>
					<td>
						<xsl:value-of select="synopsis"/>
					</td>
				</tr>
				<xsl:apply-templates select="award"/>
			</table><br/>
			<div>
				<p class="link" onmouseover="this.className='hover'" onmouseout="this.className='link'" onclick="displayall()">Show / Hide All Files</p>
			</div><br/>
			<xsl:apply-templates select="episode"/>
			<p>List created with <a href="http://www.anidb.info">AniDB</a> export feature</p>
		</body>
	</html>
</xsl:template>

<xsl:template match="name">
	<tr>
		<td>Title:</td>
		<td><xsl:value-of select="romanji"/></td>
	</tr>
	<tr>
		<td>Jap. Kanji:</td>
		<td>
			<xsl:choose>
				<xsl:when test="kanji != ''">
					<xsl:value-of select="kanji"/>
				</xsl:when>
				<xsl:otherwise>-</xsl:otherwise>
			</xsl:choose>
		</td>
	</tr>
	<tr>
		<td>English:</td>
		<td>
			<xsl:choose>
				<xsl:when test="english != ''">
					<xsl:value-of select="english"/>
				</xsl:when>
				<xsl:otherwise>-</xsl:otherwise>
			</xsl:choose>
		</td>
	</tr>
	<xsl:apply-templates select="synonym"/>
	<xsl:apply-templates select="shorts"/>
</xsl:template>

<xsl:template match="synonym">
	<tr>
		<td>Synonyms:</td>
		<td>
			<xsl:choose>
				<xsl:when test="alias != ''">
					<xsl:for-each select="alias">
						<xsl:value-of select="."/>
						<xsl:if test="position() != last()">, </xsl:if>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>-</xsl:otherwise>
			</xsl:choose>
		</td>
	</tr>	
</xsl:template>

<xsl:template match="shorts">
	<tr>
		<td>Short:</td>
		<td>
			<xsl:choose>
				<xsl:when test="short != ''">
					<xsl:for-each select="short">
						<xsl:value-of select="."/>
						<xsl:if test="position() != last()">, </xsl:if>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>-</xsl:otherwise>
			</xsl:choose>
		</td>
	</tr>	
</xsl:template>

<xsl:template match="genres">
	<tr>
		<td>Genre:</td>
		<td>
			<xsl:for-each select="genre">
				<xsl:value-of select="."/>
				<xsl:if test="position() != last()">, </xsl:if>
			</xsl:for-each>
		</td>
	</tr>
</xsl:template>

<xsl:template match="epnumber">
	<tr>
		<td>Episodes:</td>
		<td>
			<xsl:value-of select="total"/>
			(have <xsl:value-of select="own"/>
			<xsl:if test="ownsp &#62; 0">
				+ <xsl:value-of select="ownsp"/>
			</xsl:if>,
			watched <xsl:value-of select="watch"/>
			<xsl:if test="watchsp &#62; 0">
				+ <xsl:value-of select="watchsp"/>
			</xsl:if>)
		</td>
	</tr>
</xsl:template>

<xsl:template match="broadcast">
<tr>
	<td>Year:</td>
	<td>
		<xsl:value-of select="year"/>
		<xsl:choose>
			<xsl:when test="start = end">
				(<xsl:value-of select="translate(start,'.','/')"/>)
			</xsl:when>
			<xsl:otherwise>
				(<xsl:value-of select="translate(start,'.','/')"/> till <xsl:value-of select="translate(end,'.','/')"/>)
			</xsl:otherwise>
		</xsl:choose>
	</td>
</tr>
</xsl:template>

<xsl:template match="feedback">
	<tr>
		<td>Rating:</td>
		<td>
			<xsl:value-of select="rating"/> (<xsl:value-of select="vote"/> votes), My Vote:
			<xsl:choose>
				<xsl:when test="myvote &#62; 0">
					<xsl:value-of select="myvote"/>
				</xsl:when>
				<xsl:otherwise>
					-
				</xsl:otherwise>
			</xsl:choose>
		</td>
	</tr>
	<tr>
		<td>Temp Rating:</td>
		<td>
			<xsl:value-of select="tmprating"/> (<xsl:value-of select="tmpvote"/> votes), My Temp Vote:
			<xsl:choose>
				<xsl:when test="mytmpvote &#62; 0">
					<xsl:value-of select="mytmpvote"/>
				</xsl:when>
				<xsl:otherwise>
					-
				</xsl:otherwise>
			</xsl:choose>
		</td>
	</tr>
</xsl:template>

<xsl:template match="review">
	<tr>
		<td>Reviews:</td>
		<td><xsl:value-of select="reviewer"/> review(s), Avg Review Rating: <xsl:value-of select="rating"/></td>
	</tr>
</xsl:template>

<xsl:template match="award">
	<tr>
		<td>Awards:</td>
		<td>
			<xsl:choose>
				<xsl:when test="type != ''">
					<xsl:for-each select="type">
						<img src="http://img2.anidb.info/pics/anime/{pic}" width="300" height="50" alt="{name}" title="{name}"/>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>-</xsl:otherwise>
			</xsl:choose>
		</td>
	</tr>
</xsl:template>

<xsl:template match="episode">
	<table id="episode">
		<tr>
			<th id="ep_ep">Ep</th>
			<th>Title</th>
			<th id="ep_length">Length</th>
			<th id="ep_date">Date</th>
		</tr>
		<xsl:for-each select="epdetail">
			<tr onmouseover="this.className='hover'" onmouseout="this.className=''" onclick="display('ep{position()}')">
				<td><a href="http://anidb.info/perl-bin/animedb.pl?show=ep&#38;eid={id}&#38;nonav=1"><xsl:value-of select="number"/></a></td>
				<td><xsl:value-of select="name"/></td>
				<td><xsl:value-of select="length"/> min</td>
				<td><xsl:value-of select="translate(date,'.','/')"/></td>
			</tr>
			<tr>
				<td colspan="4">
					<blockquote id="ep{position()}" name="fdetail" class="hiddencontent">
						<xsl:apply-templates select="files"/>
					</blockquote>
				</td>
			</tr>
		</xsl:for-each>
	</table>
</xsl:template>

<xsl:template match="files">
	<table>
		<tr>
			<th id="file_col_id">ID</th>
			<th id="file_col_size">Size</th>
			<th>Language / Subtitle</th>
			<th id="file_col_group">Group</th>
			<th id="file_col_qual">Quality</th>
			<th id="file_col_res">Resolution</th>
			<th id="file_col_source">Source</th>
			<th id="file_col_ed2k">ed2k</th>
		</tr>
		<xsl:for-each select="file">
			<tr onmouseover="this.className='hover'" onmouseout="this.className=''">
				<td><a href="http://anidb.info/perl-bin/animedb.pl?show=file&#38;fid={id}&#38;nonav=1"><xsl:value-of select="id"/></a></td>
				<td><xsl:value-of select="translate(size,'.',',')"/></td>
				<td><xsl:value-of select="lang"/> / <xsl:value-of select="sub"/></td>
				<td><xsl:value-of select="groupname"/></td>
				<td><xsl:value-of select="quality"/></td>
				<td><xsl:value-of select="resolution"/></td>
				<td><xsl:value-of select="source"/></td>
				<td><a class="linklist" href="{ed2klink}" title="{ed2kname}">Link</a></td>
			</tr>
		</xsl:for-each>
	</table>
</xsl:template>

</xsl:stylesheet>
