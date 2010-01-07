<?xml version="1.0" encoding="utf-8" ?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method="html"/>

	<xsl:template match="class">

		<h1><xsl:value-of select="name"/></h1>

		<p><h3>Defined in: </h3><xsl:value-of select="filename"/></p>

		<p>
			<h3>Dependencies:</h3>

			<xsl:for-each select="depends/library">
				<div>
					external - 
					<xsl:value-of select="."/>
					&#160; (<xsl:value-of select="./@partof"/>)
				</div>
			</xsl:for-each>

			<xsl:for-each select="depends/module">
				<div>
					internal - 
					<a>
						<xsl:attribute name="href">
							<xsl:value-of select="./@href"/>
						</xsl:attribute>
						<xsl:value-of select="."/>
					</a>
				</div>
			</xsl:for-each>
		</p>

		<p>
			<h3>Derived from:</h3>

			<xsl:for-each select="derived/parent">
				<div>
					<a>
						<xsl:attribute name="href"><xsl:value-of select="./@href"/></xsl:attribute>
						<xsl:value-of select="."/>
					</a>
				</div>
			</xsl:for-each>
		</p>

		<xsl:choose>
			<xsl:when test="@istemplate='1'">
				<p>
					<h3>Template Parameter:</h3>
	
					<xsl:for-each select="template/parameter">
						<div>
							<xsl:value-of select="./@type"/> &#160; <xsl:value-of select="."/>
						</div>
					</xsl:for-each>
	
				</p>
			</xsl:when>
		</xsl:choose>

		<p>
			<h3>Attributes:</h3>

			<xsl:for-each select="attribute">
				<div>
					<xsl:value-of select="type"/> &#160; <xsl:value-of select="name"/>
				</div>
			</xsl:for-each>
		</p>

		<p>
			<h3>Methods:</h3>

			<xsl:for-each select="method">
				<div><a>
					<xsl:attribute name="href">
						#<xsl:value-of select="name"/>
					</xsl:attribute>
					<xsl:value-of select="name"/>
				</a></div>
			</xsl:for-each>
		</p>

		<p>
			<h3>Details:</h3>

			<xsl:for-each select="method">
				<p>
					<h6>
						<xsl:attribute name="name"><xsl:value-of select="name"/></xsl:attribute>
						<xsl:value-of select="name"/>
					</h6>

					<b>Returns:</b><xsl:value-of select="returns"/><br/>

					<xsl:for-each select="parameter">
						<b>Parameter:</b><xsl:value-of select="."/>,&#160;<br/>
					</xsl:for-each>

					<b>Info:</b><xsl:value-of select="info"/><br/>
				</p>

				<hr/>

			</xsl:for-each>
		</p>

	</xsl:template>

</xsl:stylesheet>
