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
	
	<xsl:if test="count(parameter) &gt; 0">
		<xsl:text>(</xsl:text>
		<xsl:apply-templates select="parameter"/>
		<xsl:text>)</xsl:text>
	</xsl:if>
	<xsl:text>&#xa;</xsl:text>

	<xsl:if test="count(model) &gt; 0">
		<xsl:call-template name="margin"/>
		<xsl:text>{</xsl:text>
		<xsl:text>&#xa;</xsl:text>

		<xsl:apply-templates select="model"/>

		<xsl:call-template name="margin"/>
		<xsl:text>}&#xa;</xsl:text>
	</xsl:if>
</xsl:template>

<xsl:template match="parameter">
	<xsl:value-of select="@name"/>
	<xsl:text> = </xsl:text>
	<xsl:value-of select="@value | @ref"/>
	<xsl:if test="position() != last()">
		<xsl:text>, </xsl:text>
	</xsl:if>
</xsl:template>

</xsl:stylesheet>
