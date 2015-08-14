<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text"/>

<xsl:template name="margin">
	<xsl:for-each select="1 to count(ancestor::*)">
		<xsl:text>&#x9;</xsl:text>
	</xsl:for-each>
</xsl:template>

<xsl:template match="simulation | model | integrator">
	<xsl:call-template name="margin"/>
	<xsl:choose>
		<xsl:when test="count(@type | @class) &gt; 0">
			<xsl:value-of select="@type | @class"/>
		</xsl:when>
		<xsl:otherwise>
			<xsl:text>Box</xsl:text>
		</xsl:otherwise>
	</xsl:choose>
	<xsl:text> </xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:choose>
		<xsl:when test="count(parameter)=0 and count(model)=0">
			<xsl:text> {}&#xa;</xsl:text>
		</xsl:when>
		<xsl:otherwise>
			<xsl:text> {&#xa;</xsl:text>
			<xsl:apply-templates select="parameter"/>
			<xsl:apply-templates select="model"/>
			<xsl:call-template name="margin"/>
			<xsl:text>}&#xa;</xsl:text>
		</xsl:otherwise>
	</xsl:choose>

</xsl:template>

<xsl:template match="parameter">
	<xsl:call-template name="margin"/>
	<xsl:value-of select="@name"/>
	<xsl:text>: </xsl:text>
	<xsl:value-of select="@value | @ref"/>
	<xsl:text>&#xa;</xsl:text>
</xsl:template>

</xsl:stylesheet>
