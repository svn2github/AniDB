<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" encoding="UTF-8" />

<xsl:variable name="complete" select="0"/>
<xsl:variable name="watched" select="0"/>
<xsl:variable name="sfv" select="0"/>

<xsl:template match="AC">

<table width="100%" border="0">
      <tr class="options">
      <td> Options: </td>

      <td style="cursor:pointer;" onclick="javascript:swWatched(this)">
      <xsl:choose>
         <xsl:when test="$watched=1"><xsl:attribute name="class">opt</xsl:attribute></xsl:when>
         <xsl:otherwise><xsl:attribute name="class"></xsl:attribute></xsl:otherwise>
      </xsl:choose> Watched </td>

      <td style="cursor:pointer;" onclick="javascript:swComplete(this)">
      <xsl:choose>
         <xsl:when test="$complete=1"><xsl:attribute name="class">opt</xsl:attribute></xsl:when>
         <xsl:otherwise><xsl:attribute name="class"></xsl:attribute></xsl:otherwise>
      </xsl:choose> Complete </td>

      <td style="cursor:pointer;" onclick="javascript:swSFV(this)">
      <xsl:choose>
         <xsl:when test="$sfv=1"><xsl:attribute name="class">opt</xsl:attribute></xsl:when>
         <xsl:otherwise><xsl:attribute name="class"></xsl:attribute></xsl:otherwise>
      </xsl:choose> Alt. filenames </td>

      </tr>
</table>

<table width="100%">

   <tr class="thead">
      <td> # </td>
      <td onClick="sort('Name', 'text')"    onMouseOver="over(this, 'sort')" onMouseOut="out(this)">
         Name </td>
      <td onClick="sort('substring(Year,1,4)', 'number')"     onMouseOver="over(this, 'sort')" onMouseOut="out(this)">
         Year </td>
      <td onClick="sort('Rate', 'number')"    onMouseOver="over(this, 'sort')" onMouseOut="out(this)">
         Rating </td>
      <td onClick="sort('Files/@number','number')"    onMouseOver="over(this, 'sort')" onMouseOut="out(this)">
         Files </td>
      <td onClick="sort('translate(Size, \'.\', \'\')', 'number')"  onMouseOver="over(this, 'sort')" onMouseOut="out(this)">
         Size </td>   
   </tr>


<xsl:apply-templates select="Anime"/>

</table>

</xsl:template>


<xsl:template match="Anime">

<xsl:variable name="selected" select="0"/>
<xsl:variable name="position" select="position()"/>

<xsl:if test="($watched=0 or @watched=1) and ($complete=0 or @complete=1)">

<tr onMouseOver="over2(this, 'high1')" onMouseOut="out2(this)">
      <xsl:choose>
         <xsl:when test="position() mod 2"><xsl:attribute name="class">even</xsl:attribute></xsl:when>
         <xsl:otherwise><xsl:attribute name="class">odd</xsl:attribute></xsl:otherwise>
      </xsl:choose>

   <td align="center" style="cursor:pointer;" onClick="select('{$position}')"> <xsl:value-of select="$position"/></td>
   <td align="left">   <a href="http://anidb.info/a{Url}" target="_blank"> <xsl:value-of select="Name" /> </a> </td>
   <td align="center"> <xsl:value-of select="Year"/> </td>
   <td align="center"> <xsl:value-of select="Rate"/> </td>
   <td align="center"> <xsl:value-of select="Files/@number"/> </td>
   <td align="right">    <xsl:value-of select="Size"/> </td>
</tr>

<xsl:if test="position()=$selected">
<xsl:for-each select="Files/Episode/File">
   <tr class="dir" onMouseOver="over2(this, 'high2');window.status='State: {State}'" onMouseOut="out2(this);window.status=''">
      
<xsl:choose>
         <xsl:when test="$sfv=0">
          <td align="center"> <xsl:value-of select="position()" /></td>
          <td colspan="3"><a href="http://anidb.info/f{@fid}" target="_blank"><xsl:value-of select="Name"/></a> </td>
         <td align="center"><a href="{ED2K}">ed2k</a></td>
         <td align="right"> <xsl:value-of select="Size"/> </td>
       </xsl:when>
         <xsl:otherwise>
          <td> </td>
          <td colspan="5">[<xsl:value-of select="translate(Group,' ','')"/>]_<xsl:value-of select="translate(../../../Name,' ','_')"/>_-_<xsl:value-of select="../@nr"/>_[<xsl:value-of select="translate(CRC,'abcdef','ABCDEF')"/>].<xsl:value-of select="Type"/></td>
      </xsl:otherwise>
</xsl:choose>
      
   </tr>
</xsl:for-each>
</xsl:if>

</xsl:if>

</xsl:template>


</xsl:stylesheet>
