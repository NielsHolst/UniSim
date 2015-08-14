<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output indent="yes" encoding="ISO-8859-1"/>

<!-- RESOURCES -->

<xsl:variable name="crops" select="document('crops.xml')/Crops"/>
	
<!-- CONFIGURATION -->

<!-- 0: File output for DVV; no progress bar
     1: Screen plot output for testing; show progress bar 
	 2: File output for testing; show progress bar -->
<xsl:variable name="test-mode" select="2"/>  

<!-- Simulation period used when test-mode > 0 -->
<xsl:variable name="BeginDate" select="'2001-01-01'"/>
<xsl:variable name="EndDate" select="'2001-12-31'"/>

<!-- Parameters missing in DVV Online must be set here -->
<xsl:variable name="EnergyScreenOption" select="1"/>  <!-- 1: EnergyBalanc or 2: OutsideLight --> 
<xsl:variable name="spLightEnergyScreen" select="10"/>
<xsl:variable name="spShadowThresholdScreen" select="500"/>
<xsl:variable name="Use2ndScreenWithEnergy" select="1"/>
<xsl:variable name="MinHeatingPbandRH" select="5"/>	<!-- See GHValues.Control.Pipes.PbandRH -->
<xsl:variable name="MinVentilationPbandRH" select="5"/>	<!-- See GHValues.Control.Pipes.PbandRH -->
<xsl:variable name="HeatingLightAdjustmentThreshold" select="300"/>
<xsl:variable name="HeatingLightAdjustmentThresholdBand" select="100"/>
<xsl:variable name="HeatingLightAdjustment" select="1"/>
<xsl:variable name="OutdoorsCo2" select="750"/>

<xsl:variable name="CoverEmissitivity" select="0.93"/>  	<!-- [0;1] -->
<xsl:variable name="CoverAbsorptivity" select="0.04"/>	<!-- [0;1] -->
<xsl:variable name="CoverHaze" select="0"/>  				<!-- [0;1] -->
<xsl:variable name="CoverDensity" select="2600"/>			<!-- [kg/m3] -->
<xsl:variable name="CoverHeatCapacity" select="840"/>	<!-- Specific heat capacity [J/kg/K] -->
<xsl:variable name="CoverThickness" select="4"/>			<!-- [mm] -->
<xsl:variable name="ScreenU50" select="2.5"/>				<!-- [W/m2/K] -->
<xsl:variable name="ScreenHaze" select="0"/>				<!-- [0;1] -->

<xsl:variable name="MaxCrackVentilation" select="4"/>		<!-- [h-1] (GCC, p. 148) -->
<xsl:variable name="Leakage" select="1"/>					<!-- [h-1] -->

<xsl:variable name="FloorU" select="7.5"/>	
<xsl:variable name="FloorHeatCapacity" select="42000."/>	

		
<!-- GLOBAL VARIABLES -->

<!-- no. of spans * span width * length -->
<xsl:variable name="greenhouse-area" 
	select=
	"JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='9']/Value *  	
	 JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='11']/Value *	
	 JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='223']/Value">
</xsl:variable>

<xsl:variable name="crop-name">
	<xsl:variable name="cropType" select="//Cultures/Culture/Id"/>
	<xsl:choose>
		<xsl:when test="$cropType=1">Roses</xsl:when>
		<xsl:when test="$cropType=2">Chrysanthemum</xsl:when>
		<xsl:when test="$cropType=3">Pansy</xsl:when>
		<xsl:when test="$cropType=4">Tomato</xsl:when>
		<xsl:when test="$cropType=5">Cucumber</xsl:when>
		<xsl:when test="$cropType=6">SweetPepper</xsl:when>
		<xsl:when test="$cropType=7">Lettuce</xsl:when>
		<xsl:otherwise>Tomato</xsl:otherwise>
	</xsl:choose>
</xsl:variable>

<!-- GLOBAL METHODS -->

<xsl:template name="extract-crop-model">
	<xsl:param name="modelName"/>
	<xsl:comment>
		<xsl:value-of select="concat($crop-name, ' ', $modelName)"/>
	</xsl:comment>
	<xsl:copy-of select="$crops/*[name()=$crop-name]/model[@name=$modelName]"/>
</xsl:template>

<xsl:template name="extract-crop-parameters">
	<xsl:copy-of select="$crops/*[name()=$crop-name]/model[@name='crop']//parameter"/>
</xsl:template>

<xsl:template name="extract-period">
	<xsl:param name="circadian"/>
	<parameter name="beginDay">
		<xsl:attribute name="value">
			<xsl:value-of select="FromDay"/>
		</xsl:attribute>
	</parameter>
	<parameter name="endDay">
		<xsl:attribute name="value">
			<xsl:value-of select="ToDay"/>
		</xsl:attribute>
	</parameter>
	<parameter name="beginTime">
		<xsl:attribute name="value">
			<xsl:value-of select="concat(floor(FromTime div 60), ':', FromTime mod 60)"/>
		</xsl:attribute>
	</parameter>
	<parameter name="endTime">
		<xsl:attribute name="value">
			<xsl:value-of select="concat(floor(ToTime div 60), ':', ToTime mod 60)"/>
		</xsl:attribute>
	</parameter>
	<parameter name="circadian">
		<xsl:attribute name="value">
			<xsl:value-of select="$circadian"/>
		</xsl:attribute>
	</parameter>
</xsl:template>

<xsl:template name="cover-position-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">roof1</xsl:when>
		<xsl:when test="$number=2">roof2</xsl:when>
		<xsl:when test="$number=3">side1</xsl:when>
		<xsl:when test="$number=4">side2</xsl:when>
		<xsl:when test="$number=5">end1</xsl:when>
		<xsl:when test="$number=6">end2</xsl:when>
		<xsl:when test="$number=7">floor</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="cover-compartment-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">indoors/top</xsl:when>
		<xsl:when test="$number=2">indoors/top</xsl:when>
		<xsl:when test="$number=3">indoors</xsl:when>
		<xsl:when test="$number=4">indoors</xsl:when>
		<xsl:when test="$number=5">indoors</xsl:when>
		<xsl:when test="$number=6">indoors</xsl:when>
		<xsl:when test="$number=7">indoors</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="cover-average-height">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">roofAverageHeight</xsl:when>
		<xsl:when test="$number=2">roofAverageHeight</xsl:when>
		<xsl:when test="$number=3">averageHeight</xsl:when>
		<xsl:when test="$number=4">averageHeight</xsl:when>
		<xsl:when test="$number=5">averageHeight</xsl:when>
		<xsl:when test="$number=6">averageHeight</xsl:when>
		<xsl:when test="$number=7">averageHeight</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="screen-position-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">roof1_roof2_trapez</xsl:when>
		<xsl:when test="$number=2">horizontal</xsl:when>
		<xsl:when test="$number=3">roof1</xsl:when>
		<xsl:when test="$number=4">roof2</xsl:when>
		<xsl:when test="$number=5">side1</xsl:when>
		<xsl:when test="$number=6">side2</xsl:when>
		<xsl:when test="$number=7">end1</xsl:when>
		<xsl:when test="$number=8">end2</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="screen-layer-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">outer</xsl:when>
		<xsl:when test="$number=2">mid</xsl:when>
		<xsl:when test="$number=3">inner</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="screen-type-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">shade</xsl:when>
		<xsl:when test="$number=2">energy</xsl:when>
		<xsl:when test="$number=3">blackout</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="lamp-type">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">HPSL</xsl:when>
		<xsl:when test="$number=2">LED</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="extract-screen-parameters">
	<xsl:param name="screen-type"/>
	<parameter name="position">
		<xsl:attribute name="value">
			<xsl:call-template name="screen-position-name">
				<xsl:with-param name="number" select="Position"/>
			</xsl:call-template>
		</xsl:attribute>
	</parameter>
	<parameter name="layer">
		<xsl:attribute name="value">
			<xsl:call-template name="screen-layer-name">
				<xsl:with-param name="number" select="Layer"/>
			</xsl:call-template>
		</xsl:attribute>
	</parameter>
	<parameter name="lightTransmissivity">
		<xsl:attribute name="value">
			<xsl:value-of select="Constants/Parameters[ParameterID='30']/Value div 100"/>
		</xsl:attribute>
	</parameter>
	<parameter name="airTransmission">
		<xsl:attribute name="value">
			<xsl:value-of select="Constants/Parameters[ParameterID='32']/Value div 100"/>
		</xsl:attribute>
	</parameter>
	<parameter name="energyLossReduction">
		<xsl:attribute name="value">
			<xsl:value-of select="Constants/Parameters[ParameterID='33']/Value div 100"/>
		</xsl:attribute>
	</parameter>
	<parameter name="U50">
		<xsl:attribute name="value">
			<xsl:value-of select="$ScreenU50"/>
		</xsl:attribute>
	</parameter>
	<parameter name="haze">
		<xsl:attribute name="value">
			<xsl:value-of select="$ScreenHaze"/>
		</xsl:attribute>
	</parameter>
	<parameter name="state">
		<xsl:attribute name="ref">
			<xsl:value-of select="concat('actuators/screens/', $screen-type, '/control[state]')"/>
		</xsl:attribute>
	</parameter>
</xsl:template>

<xsl:template name="extract-screens">
	<xsl:param name="model-name"/>
	<xsl:param name="shelter-position"/>
	<xsl:param name="inner-temperature"/>
	<xsl:param name="outer-temperature"/>
	<model>
		<xsl:attribute name="name">
			<xsl:value-of select="$model-name"/>
		</xsl:attribute>
		<xsl:attribute name="type">
			<xsl:value-of select="'vg::Screens'"/>
		</xsl:attribute>
		<xsl:for-each select="/JobDataSet/Greenhouse/zone/Screens//Screen">
			<xsl:variable name="screen-type">
				<xsl:call-template name="screen-type-name">
					<xsl:with-param name="number" select="Constants/Parameters[ParameterID='573']/Value"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="screen-position">
				<xsl:call-template name="screen-position-name">
					<xsl:with-param name="number" select="Position"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:if test="contains($screen-position, $shelter-position)">
				<model>
					<xsl:attribute name="name">
						<xsl:value-of select="$screen-type"/>
					</xsl:attribute>
					<xsl:attribute name="type">
						<xsl:value-of select="'vg::Screen'"/>
					</xsl:attribute>
					<xsl:call-template name="extract-screen-parameters">
						<xsl:with-param name="screen-type" select="$screen-type"/>
					</xsl:call-template>
					<!--
					<model name="temperature" type="vg::ScreenTemperature">
						<parameter name="innerTemperature">
							<xsl:attribute name="ref">
								<xsl:value-of select="$inner-temperature"/>
							</xsl:attribute>
						</parameter>
						<parameter name="outerTemperature">
							<xsl:attribute name="ref">
								<xsl:value-of select="$outer-temperature"/>
							</xsl:attribute>
						</parameter>
					</model>
					-->
				</model>
			</xsl:if>
		</xsl:for-each>					
	</model>
</xsl:template>
	
	
<!-- MAIN -->
		
<xsl:template match="/"> <simulation name="Virtual Greenhouse">
	<xsl:variable name="startTime" select="JobDataSet/StartTime"/>
	<xsl:variable name="stopTime" select="JobDataSet/StopTime"/>
	<xsl:variable name="startYear" select="number( substring($startTime, 1, 4) )"/>
	<xsl:variable name="stopYear" select="number( substring($stopTime, 1, 4) )"/>
	<xsl:variable name="incYear" select="$stopYear - $startYear"/>

	<integrator name="steps" type="UniSim::StepsTimeInterval">
		
		<parameter name="beginDate">
			<xsl:attribute name="value">
				<xsl:choose>
					<xsl:when test="$test-mode=0">
						<xsl:value-of select="concat(2001, substring($startTime, 5, 6))"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$BeginDate"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</parameter>

		<parameter name="endDate">
			<xsl:attribute name="value">
				<xsl:choose>
					<xsl:when test="$test-mode=0">
						<xsl:value-of select="concat(2001 + $incYear, substring($stopTime, 5, 6))"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$EndDate"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</parameter>

		<parameter name="beginTime">
			<xsl:attribute name="value">
				<xsl:value-of select="substring($startTime, 12, 5)"/>
			</xsl:attribute>
		</parameter>
		<parameter name="endTime">
			<xsl:attribute name="value">
				<xsl:value-of select="substring($stopTime, 12, 5)"/>
			</xsl:attribute>
		</parameter>

		<parameter name="timeStep" value="5"/>
		<!--
		<parameter name="timeStep">
			<xsl:attribute name="value">
				<xsl:value-of select="JobDataSet/SimulationTimeStep"/>
			</xsl:attribute>
		</parameter>
		-->
		<parameter name="timeUnit" value="m"/>
		<xsl:choose>
			<xsl:when test="$test-mode=0">
				<parameter name="indicator" value="1"/>
			</xsl:when>
			<xsl:when test="$test-mode>0">
				<parameter name="indicator" value="2"/>
			</xsl:when>
		</xsl:choose>
	</integrator>

	<model name="calendar" type="UniSim::Calendar">
		<xsl:variable name="simTimeStep" select="JobDataSet/SimulationTimeStep"/>
		<xsl:variable name="outTimeStep" select="JobDataSet/OutputTimeStep"/>
		<parameter name="initialDate" ref="steps[beginDate]"/>
		<parameter name="initialTimeOfDay" ref="steps[beginTime]"/>
		<parameter name="timeStep" ref="steps[timeStep]"/>
		<parameter name="timeUnit" ref="steps[timeUnit]"/>
		<parameter name="sample" value="12"/>
<!--	<parameter name="sample">
			<xsl:choose>
				<xsl:when test="string-length($simTimeStep)=0 or string-length($simTimeStep)=0">
					<xsl:attribute name="value" select="12"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="value" select="$outTimeStep div $simTimeStep"/>
				</xsl:otherwise>
			</xsl:choose>
		</parameter> -->
		<parameter name="latitude">
			<xsl:attribute name="value" select="JobDataSet/Latitude"/>
		</parameter>
		<parameter name="longitude">
			<xsl:attribute name="value" select="JobDataSet/Longitude"/>
		</parameter>
		<parameter name="timeZone">
			<xsl:attribute name="value">
				<xsl:value-of select="substring($startTime, 21, 2)"/>
			</xsl:attribute>
		</parameter>
	</model>

	<model name="outdoors" type="vg::Outdoors">
		<parameter name="co2">
			<xsl:attribute name="value">
				<xsl:value-of select="$OutdoorsCo2"/>
			</xsl:attribute>
		</parameter>
		<model name="records" type="UniSim::Records">
			<parameter name="fileName" value="sel_dk.txt"/>
			<parameter name="imposeInitialDateTime" value="false"/>
		</model>
	</model>

	<model name="construction">
		<model name="geometry" type="vg::ConstructionGeometry">
			<parameter name="spanWidth">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='11']/Value"/>
				</xsl:attribute>
			</parameter>
			<parameter name="numSpans">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='9']/Value"/>
				</xsl:attribute>
			</parameter>
			<parameter name="height">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='13']/Value"/>
				</xsl:attribute>
			</parameter>
			<parameter name="length">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='223']/Value"/>
				</xsl:attribute>
			</parameter>
			<parameter name="roofPitch">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='14']/Value"/>
				</xsl:attribute>
			</parameter>
			<parameter name="shade">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='445']/Value div 100"/>
				</xsl:attribute>
			</parameter>
		</model>

		<model name="greenhouseShelter">
			<model name="top" type="vg::GreenhouseShelter">
				<parameter name="shelters" value="(greenhouseShelter/roof1 greenhouseShelter/roof2)"/>
			</model>
			<model name="bottom" type="vg::GreenhouseShelter">
				<parameter name="shelters" value="(greenhouseShelter/side1 greenhouseShelter/side2 greenhouseShelter/end1 greenhouseShelter/end2)"/>
			</model>
			<model name="total" type="vg::GreenhouseShelter">
				<parameter name="shelters" value="(greenhouseShelter/roof1 greenhouseShelter/roof2 greenhouseShelter/side1 greenhouseShelter/side2 greenhouseShelter/end1 greenhouseShelter/end2)"/>
			</model>
			<xsl:for-each select="JobDataSet/Greenhouse/zone/Panes//Pane[Position &lt; 7]">
				<model>
					<xsl:variable name="shelter-position">
						<xsl:call-template name="cover-position-name">
							<xsl:with-param name="number" select="Position"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:variable name="compartment-name">
						<xsl:call-template name="cover-compartment-name">
							<xsl:with-param name="number" select="Position"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:variable name="average-height">
						<xsl:call-template name="cover-average-height">
							<xsl:with-param name="number" select="Position"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:attribute name="name">
						<xsl:value-of select="$shelter-position"/>
					</xsl:attribute>
					<xsl:attribute name="type">
						<xsl:value-of select="'vg::Shelter'"/>
					</xsl:attribute>
					<model name="cover" type="vg::Cover">
						<parameter name="U4">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='310']/Value"/>
							</xsl:attribute>
						</parameter>
						<parameter name="transmissivity">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='308']/Value div 100"/>
							</xsl:attribute>
						</parameter>
						<parameter name="emissivity">
							<xsl:attribute name="value">
								<xsl:value-of select="$CoverEmissitivity"/>
							</xsl:attribute>
						</parameter>
						<parameter name="absorptivity">
							<xsl:attribute name="value">
								<xsl:value-of select="$CoverAbsorptivity"/>
							</xsl:attribute>
						</parameter>
						<parameter name="density">
							<xsl:attribute name="value">
								<xsl:value-of select="$CoverDensity"/>
							</xsl:attribute>
						</parameter>
						<parameter name="heatCapacity">
							<xsl:attribute name="value">
								<xsl:value-of select="$CoverHeatCapacity"/>
							</xsl:attribute>
						</parameter>
						<parameter name="thickness">
							<xsl:attribute name="value">
								<xsl:value-of select="$CoverThickness"/>
							</xsl:attribute>
						</parameter>
						<parameter name="haze">
							<xsl:attribute name="value">
								<xsl:value-of select="$CoverHaze"/>
							</xsl:attribute>
						</parameter>
						<model name="energyFlux" type="vg::EnergyFluxCover">
							<parameter name="indoorsTemperature">
								<xsl:attribute name="ref">
									<xsl:value-of select="concat($compartment-name, '/temperature[value]')"/>
								</xsl:attribute>
							</parameter>
							<parameter name="averageHeight">
								<xsl:attribute name="ref">
									<xsl:value-of select="concat('construction/geometry[', $average-height, ']')"/>
								</xsl:attribute>
							</parameter>
						</model>
						<model name="condensation" type="vg::CoverCondensation"/>
						<!--						
						<model name="condensation" type="vg::CoverCondensation"> 
							<parameter name="indoorsTemperature">
								<xsl:attribute name="ref">
									<xsl:value-of select="concat($compartment-name, '/temperature[value]')"/>
								</xsl:attribute>
							</parameter>
							<parameter name="indoorsAh">
								<xsl:attribute name="ref">
									<xsl:value-of select="concat($compartment-name, '/humidity[ah]')"/>
								</xsl:attribute>
							</parameter>
						</model>
						-->
					</model>
					<xsl:call-template name="extract-screens">
						<xsl:with-param name="model-name" select="'screens'"/>
						<xsl:with-param name="shelter-position" select="$shelter-position"/>
						<xsl:with-param name="outer-temperature" select="'../../../cover/energyFlux[temperature]'"/>
						<xsl:with-param name="inner-temperature" select="concat('indoors/', $compartment-name, '/temperature[value]')"/>
					</xsl:call-template>
				</model>
			</xsl:for-each>		

		</model>

		<xsl:call-template name="extract-screens">
			<xsl:with-param name="model-name" select="'horizontalScreen'"/>
			<xsl:with-param name="shelter-position" select="'horizontal'"/>
			<xsl:with-param name="outer-temperature" select="'indoors/top/temperature[value]'"/>
			<xsl:with-param name="inner-temperature" select="'indoors/bottom/temperature[value]'"/>
		</xsl:call-template>

<!--	<model name="horizontalScreen" type="vg::HorizontalScreen">
			<xsl:variable name="horizontal-screens-count">
				<xsl:value-of select="count(/JobDataSet/Greenhouse/zone/Screens//Screen[Position=2])"/>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$horizontal-screens-count=0">
					<model name="screens" type="vg::Screens"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="extract-screens">
						<xsl:with-param name="shelter-position" select="'horizontal'"/>
					<xsl:with-param name="outer-temperature" select="'indoors/top/temperature[value]'"/>
					<xsl:with-param name="inner-temperature" select="'indoors/bottom/temperature[value]'"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</model> -->

	
		<model name="vents" type="vg::Vents">
			<xsl:for-each select="//JobDataSet/Greenhouse/zone/Vents/Vent">
				<model type="vg::Vent">
					<xsl:attribute name="name">
						<xsl:value-of select="concat('vent', position())"/>
					</xsl:attribute>
					<!--<parameter name="maxOpening">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='52']/Value"/>
						</xsl:attribute>
					</parameter>-->
					<parameter name="length">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='456']/Value"/>
						</xsl:attribute>
					</parameter>
					<parameter name="height">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='457']/Value"/>
						</xsl:attribute>
					</parameter>
					<parameter name="number">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='49']/Value"/>
						</xsl:attribute>
					</parameter>
					<parameter name="porosity">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='51']/Value div 100"/>
						</xsl:attribute>
					</parameter>
				</model>
			</xsl:for-each>
		</model>
		
		<model name="floor">
			<model name="energyFlux" type="vg::EnergyFluxFloor"> 
				<parameter name="U">
					<xsl:attribute name="value">
						<xsl:value-of select="$FloorU"/>
					</xsl:attribute>
				</parameter>
				<parameter name="heatCapacity">
					<xsl:attribute name="value">
						<xsl:value-of select="$FloorHeatCapacity"/>
					</xsl:attribute>
				</parameter>
			</model>
		</model>
	</model>

	<model name="indoors">
		<model name="given">
			<model name="total">
				<model name="airflux">
					<model name="infiltration" type="vg::AirFluxInfiltration">
						<parameter name="leakage">
							<xsl:attribute name="value">
								<xsl:value-of select="$Leakage"/>
							</xsl:attribute>
						</parameter>
					</model>
					<model name="crackVentilation" type="vg::PidControlElement">
						<parameter name="signal" ref="./target[signal]"/>
						<parameter name="Kprop" value="0.1"/>
						<model name="target" type="vg::ProportionalSignal">
							<model name="coldFactor" type="vg::ProportionalSignal"> 
								<parameter name="input" ref ="outdoors[temperature]"/>
								<parameter name="threshold" value ="-5"/>    	<!-- sp.VentsspFrostProtection_alpha=-5  -->
								<parameter name="thresholdBand" value="1"/>
								<parameter name="increasingSignal" value="true"/>
								<parameter name="maxSignal">
									<xsl:attribute name="value">
										<xsl:value-of select="$MaxCrackVentilation"/>
									</xsl:attribute>
								</parameter>
								<parameter name="minSignal" value="0"/>
							</model>
							
							<parameter name="input" ref="indoors/humidity[rh]"/>
							<parameter name="threshold" ref="setpoints/humidity/maximumRh[signal]"/>
							<parameter name="thresholdBand">
								<xsl:attribute name="value">
									<xsl:value-of select="max(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='492']/Value)"/> <!--Hack-->
								</xsl:attribute>
							</parameter>
							<parameter name="increasingSignal" value="true"/>
							<parameter name="maxSignal" ref="./coldFactor[signal]"/>  
							<parameter name="minSignal" value="0"/>
						</model>
					</model>
					<model name="gravitation" type="vg::AirFluxGravitation"/>
				</model> <!-- airflux -->
			</model>
		
			<model name="top">
				<model name="airFlux">
					<model name="outdoors" type="AirFluxOutdoors">
						<parameter name="infiltration" ref="total/airFlux/infiltration[value]"/>
						<parameter name="ventilation" ref="total/airFlux/crackVentilation[value]"/>
						<parameter name="volumeProportion" ref="construction/geometry[volumeProportionTop]"/>
						<parameter name="gap" ref="horizontalScreen[airTransmissionNot]"/>
					</model>
					<model name="indoors" type="AirFluxIndoors">
						<parameter name="receiverVolume" ref="construction/geometry[volumeTop]"/>
					</model>
				</model>
				<model name="vapourFlux" type="vg::VapourFluxSum"> 
					<model name="condensation" type="vg::VapourFluxSum">
						<parameter name="toAdd" value="(roof1/cover/condensation roof2/cover/condensation)"/>
					</model>
					<model name="airFluxOutdoors"  type="vg::VapourFluxAir">
						<parameter name="airFlux" ref="../../airflux/outdoors[value]"/>
						<parameter name="receiverAh" ref="indoors/top/humidity[ah]"/> 
						<parameter name="donorAh" ref="outdoors[ah]"/> 
						<parameter name="averageHeight" ref="construction/geometry[roofAverageHeight]"/>
					</model>
					<model name="airFluxIndoors"  type="vg::VapourFluxAir">
						<parameter name="airFlux" ref="top/airFlux/indoors[value]"/>
						<parameter name="receiverAh" ref="indoors/top/humidity[ah]"/> 
						<parameter name="donorAh" ref="indoors/humidity[ah]"/> 
						<parameter name="averageHeight" ref="construction/geometry[roofAverageHeight]"/>
					</model>
				</model>
				<model name="energyFlux" type="vg::EnergyFluxSum"> 
					<model name="condensation" type="vg::EnergyFluxCondensation"/> 
					<model name="airFluxOutdoors" type="vg::EnergyFluxAir">
						<parameter name="airFlux" ref="top/airFlux/outdoors[value]"/>
						<parameter name="receiverTemperature" ref="indoors/top/temperature[value]"/> 
						<parameter name="donorTemperature" ref="outdoors[temperature]"/>
						<parameter name="receiverHeight" ref="construction/geometry[roofAverageHeight]"/>
					</model>
					<model name="airFluxIndoors" type="vg::EnergyFluxAir">
						<parameter name="airFlux" ref="top/airFlux/indoors[value]"/>
						<parameter name="receiverTemperature" ref="indoors/top/temperature[value]"/> 
						<parameter name="donorTemperature" ref="indoors/temperature[value]"/>
						<parameter name="receiverHeight" ref="construction/geometry[roofAverageHeight]"/>
						<parameter name="donorHeight" ref="construction/geometry[height]"/>
					</model>
					<model name="convection" type="vg::EnergyFluxHorizontal">
						<parameter name="receiverTemperature" ref="indoors/top/temperature[value]"/> 
						<parameter name="donorTemperature" ref="indoors/temperature[value]"/>
					</model>
					<model name="cover" type="vg::EnergyFluxCoverSum"> 
						<parameter name="toAdd" value="(roof1/cover/energyFlux roof2/cover/energyFlux)"/>
					</model>					
					<model name="light" type="UniSim::Sum">
						<parameter name="toAdd" value="(top/light[total])"/>
					</model>					
				</model>		
			</model> <!-- top -->
			
			<model name="bottom"> 
				<model name="airFlux">
					<model name="outdoors" type="AirFluxOutdoors">
						<parameter name="infiltration" ref="total/airFlux/infiltration[value]"/>
						<parameter name="ventilation" value="0"/>
						<parameter name="volumeProportion" ref="construction/geometry[volumeProportionIndoors]"/>
						<parameter name="gap" ref="horizontalScreen[airTransmission]"/>
					</model>
					<model name="indoors" type="AirFluxIndoors">
						<parameter name="receiverVolume" ref="construction/geometry[volumeIndoors]"/>
					</model>
				</model>
				<model name="vapourFlux" type="vg::VapourFluxSum"> 
					<model name="transpiration" type="vg::VapourFluxTranspiration"/>
					<model name="condensation" type="vg::VapourFluxSum">
						<parameter name="toAdd" value="(side1/cover/condensation side2/cover/condensation end1/cover/condensation end2/cover/condensation)"/>
					</model>
					<model name="airFluxOutdoors"  type="vg::VapourFluxAir">
						<parameter name="airFlux" ref="bottom/airflux/outdoors[value]"/>
						<parameter name="receiverAh" ref="indoors/humidity[ah]"/> 
						<parameter name="donorAh" ref="outdoors[ah]"/> 
						<parameter name="averageHeight" ref="construction/geometry[height]"/>
					</model>
					<model name="airFluxIndoors"  type="vg::VapourFluxAir">
						<parameter name="airFlux" ref="bottom/airFlux/indoors[value]"/>
						<parameter name="receiverAh" ref="indoors/humidity[ah]"/> 
						<parameter name="donorAh" ref="indoors/top/humidity[ah]"/> 
						<parameter name="averageHeight" ref="construction/geometry[height]"/>
					</model>
				</model>
				
				<model name="energyFlux" type="vg::EnergyFluxSum"> 
					<model name="transpiration" type="vg::EnergyFluxTranspiration"/> 
					<model name="condensation" type="vg::EnergyFluxCondensation"/> 
					<model name="growthLights" type="vg::PidControlElement">
						<parameter name="Kprop" value="0.5"/>
						<parameter name="signal" ref="./growthLights[value]"/>
						<model name="growthLights" type="vg::EnergyFluxGrowthLights"/>
					</model>
					<model name="airFluxOutdoors" type="vg::EnergyFluxAir">
						<parameter name="airFlux" ref="bottom/airflux/outdoors[value]"/>
						<parameter name="receiverTemperature" ref="indoors/temperature[value]"/> 
						<parameter name="donorTemperature" ref="outdoors[temperature]"/>
						<parameter name="receiverHeight" ref="construction/geometry[height]"/>
					</model>
					<model name="airFluxIndoors" type="vg::EnergyFluxAir">
						<parameter name="airFlux" ref="bottom/airFlux/indoors[value]"/>
						<parameter name="receiverTemperature" ref="indoors/temperature[value]"/> 
						<parameter name="donorTemperature" ref="indoors/top/temperature[value]"/>
						<parameter name="receiverHeight" ref="construction/geometry[height]"/>
						<parameter name="donorHeight" ref="construction/geometry[roofAverageHeight]"/>
					</model>
					<model name="convection" type="vg::EnergyFluxHorizontal">
						<parameter name="receiverTemperature" ref="indoors/temperature[value]"/> 
						<parameter name="donorTemperature" ref="indoors/top/temperature[value]"/>
					</model>
					<model name="cover" type="vg::EnergyFluxCoverSum"> 
						<parameter name="toAdd" value="(side1/cover/energyFlux side2/cover/energyFlux end1/cover/energyFlux end2/cover/energyFlux)"/>
					</model>					
					<model name="light" type="UniSim::Sum">
						<parameter name="toAdd" value="(indoors/light[total])"/>
					</model>					
					<model name="floor" type="UniSim::Sum">
						<parameter name="toAdd" value="(construction/floor/energyFlux[value])"/>
					</model>
				</model>		
			</model>
		</model> <!-- given -->		

		<model name="controlled">
			<model name="bottom">
				<model name="energyFlux"> 
					<model name="heating">
						<model name="demand" type="vg::EnergyFluxHeatingDemand"/>
						<model name="supply" type="vg::PidControlElement">
							<parameter name="Kprop" value="0.6"/>
							<parameter name="Kint" value="0.01"/>
							<parameter name="signal" ref="./supply[value]"/>
							<model name="supply" type="vg::EnergyFluxHeatingSupply">
								<parameter name="maxHeating" value="10000"/>
							</model>
						</model>				
					</model>		
					
					<model name="temperature" type="vg::IndoorsTemperature">
						<model name="energyFlux" type="Unisim::Sum">
							<parameter name="toAdd" value="(given/bottom/energyFlux[value] energyFlux/heating/supply[value])"/>
						</model>
						<parameter name="energyFlux" ref="./energyFlux[value]"/>
						<parameter name="baseTemperature" ref="indoors/temperature[value]"/>
						<parameter name="averageHeight" ref="construction/geometry[height]"/> 
					</model>

					<model name="cooling"> 
						<model name="demand" type="vg::EnergyFluxCoolingDemand"/>
						<model name="airSupplyMax" type="vg::AirFluxCoolingSupplyMax">
							<model name="byWind" type="vg::VentilationByWind">
								<parameter name="baseRate" value="30"/>
							</model>
							<model name="byTemp" type="vg::VentilationByTemp"/>
						</model>
						<model name="supply" type="vg::PidControlElement">
							<parameter name="Kprop" value="0.5"/>
							<parameter name="maximum" value="0"/>
							<parameter name="signal" ref="./target[value]"/>
							<model name="target" type="vg::EnergyFluxCoolingSupply">
								<parameter name="airSupplyMax" ref="cooling/airSupplyMax[value]"/>
							</model>
						</model>
					</model>
					
				</model> <!-- energyFlux -->

				<model name="cooling">
					<model name="airFlux" type="vg::AirFluxCoolingSupply"/>
					<model name="vapourFlux" type="vg::VapourFluxAir">
						<parameter name="airFlux" ref="cooling/airFlux[fromOutdoorsToIndoors]"/>
						<parameter name="receiverAh" ref="indoors/humidity[ah]"/>
						<parameter name="donorAh" ref="outdoors[ah]"/>
						<parameter name="averageHeight" ref="construction/geometry[height]"/>
					</model>
					<model name="energyFlux" type="vg::EnergyFluxAir">
						<parameter name="airFlux" ref="cooling/airFlux[fromOutdoorsToIndoors]"/>
						<parameter name="receiverTemperature" ref="indoors/temperature[value]"/>
						<parameter name="donorTemperature" ref="outdoors[temperature]"/>
						<parameter name="receiverHeight" ref="construction/geometry[height]"/>
					</model>
				</model>
			</model> <!-- bottom -->
			
			<model name="top">
				<model name="vapourFlux" type="vg::VapourFluxAir">
					<parameter name="airFlux" ref="cooling/airFlux[fromOutdoorsToTop]"/>
					<parameter name="receiverAh" ref="indoors/top/humidity[ah]"/>
					<parameter name="donorAh" ref="outdoors[ah]"/>
					<parameter name="averageHeight" ref="construction/geometry[roofAverageHeight]"/>
				</model>
				<model name="energyFlux" type="vg::EnergyFluxAir">
					<parameter name="airFlux" ref="cooling/airFlux[fromOutdoorsToTop]"/>
					<parameter name="receiverTemperature" ref="indoors/top/temperature[value]"/>
					<parameter name="donorTemperature" ref="outdoors[temperature]"/>
					<parameter name="receiverHeight" ref="construction/geometry[roofAverageHeight]"/>
				</model>
			</model> <!-- top -->
		</model> <!-- controlled -->
		
		<model name="total">
			<model name="bottom">
				<model name="airFlux" type="Unisim::Sum">
					<parameter name="toAdd" value="(given/bottom/airFlux/outdoors[value] bottom/cooling/airFlux[fromOutdoorsToIndoors])"/>
				</model>
				<model name="vapourFlux" type="vg::VapourFluxSum">
					<parameter name="toAdd" value="(given/bottom/vapourFlux bottom/cooling/vapourFlux)"/>
				</model>
				<model name="energyFlux" type="Unisim::Sum">
					<parameter name="toAdd" value="(given/bottom/energyFlux[value] bottom/energyFlux/heating/supply[value] bottom/cooling/energyFlux[value])"/>
				</model>
			</model>
			<model name="top">
				<model name="airFlux" type="Unisim::Sum">
					<parameter name="toAdd" value="(given/top/airFlux/outdoors[value] bottom/cooling/airFlux[fromOutdoorsToTop])"/>
				</model>
				<model name="vapourFlux" type="vg::VapourFluxSum">
					<parameter name="toAdd" value="(given/top/vapourFlux controlled/top/vapourFlux)"/>
				</model>
				<model name="energyFlux" type="Unisim::Sum">
					<parameter name="toAdd" value="(given/top/energyFlux[value] controlled/top/energyFlux[value])"/>
				</model>
			</model>
		</model> <!-- total -->

		<model name="top">
			<model name="light" type="vg::IndoorsTopLight"/> <!-- FIX -->
			<model name="temperature" type="vg::IndoorsTemperature">
				<parameter name="initValue">
					<xsl:attribute name="value">
						<xsl:value-of select="//Setpoint[ParameterId='2']//SetpointValue"/>
					</xsl:attribute>
				</parameter> 
				<parameter name="energyFlux" ref="total/top/energyFlux[value]"/> <!-- FIX -->
				<parameter name="averageHeight" ref="construction/geometry[roofAverageHeight]"/>
			</model>
			<model name="humidity" type="vg::IndoorsHumidity">
				<parameter name="conductance" ref="total/top/vapourFlux[conductance]"/>
				<parameter name="vapourFlux" ref="total/top/vapourFlux[vapourFlux]"/>
				<parameter name="gain" ref="total/top/vapourFlux[gain]"/>
				<parameter name="temperature" ref="../temperature[value]"/>
				<parameter name="averageHeight" ref="construction/geometry[roofAverageHeight]"/>
			</model>
			<model name="co2"/>
		</model> <!-- top -->

		<model name="light" type="vg::IndoorsBottomLight"/> <!-- FIX -->
		<model name="temperature" type="vg::IndoorsTemperature">
			<parameter name="initValue">
				<xsl:attribute name="value">
					<xsl:value-of select="//Setpoint[ParameterId='2']//SetpointValue"/>
				</xsl:attribute>
			</parameter> 
			<parameter name="energyFlux" ref="total/bottom/energyFlux[value]"/>
			<parameter name="averageHeight" ref="construction/geometry[height]"/> 
		</model>
		<model name="humidity" type="vg::IndoorsHumidity">
			<parameter name="conductance" ref="total/bottom/vapourFlux[conductance]"/>
			<parameter name="vapourFlux" ref="total/bottom/vapourFlux[vapourFlux]"/>
			<parameter name="gain" ref="total/bottom/vapourFlux[gain]"/>
			<parameter name="temperature" ref="../temperature[value]"/>
			<parameter name="averageHeight" ref="construction/geometry[height]"/>
		</model>
		<model name="co2" type="vg::IndoorsCo2"/>
	</model> <!-- indoors -->

	<model name="setpoints">
		<model name="isDay" type="vg::ProportionalSignal">
			<parameter name="threshold" >
				<xsl:attribute name="value">
					<xsl:value-of select="//Greenhouse/zone/Constants/Parameters[ParameterID='436']/Value"/>
				</xsl:attribute>
			</parameter>
			<parameter name="input" ref="outdoors/records[GlobRad]"/>
			<parameter name="thresholdBand" value="0"/>
			<parameter name="increasingSignal" value="true"/>
			<parameter name="minSignal" value="0"/>
			<parameter name="maxSignal" value="1"/>
		</model>
		<model name="humidity">
			<!-- Day/Night setpoints not implemented in MatLab nor UniSim model -->
			<!-- DeltaX setpoint not implemented in UniSim model -->
			<model name="maximumRh" type="vg::SignalCollection">
				<parameter name="rule" value="min"/>
				<parameter name="signalReset" value="100"/>
				<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='16']/SetpointTimes//SetpointTime">
					<model type="vg::DateTimeSignal">
						<parameter name="signalOutside" value="100"/>
						<parameter name="signalOutsideTimeOnly" value="100"/>
						<parameter name="signalInside">
							<xsl:attribute name="value">
								<xsl:value-of select="SetpointValue"/>
							</xsl:attribute>
						</parameter>
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'true'"/>
						</xsl:call-template>
					</model>
				</xsl:for-each>
			</model>
		</model>
		
		<model name="temperature">
			<model name="ventilation" type="vg::PidControlElement">
				<parameter name="initState" ref="./target[signalReset]"/>
				<parameter name="signal" ref="./target[signal]"/>
				<parameter name="Kprop" value="0.1"/>
				<model name="target" type="vg::SignalCollection">
					<parameter name="rule" value="sum"/>
					<parameter name="signalReset">
						<xsl:attribute name="value">
							<xsl:value-of select="//Setpoint[ParameterId='3']//SetpointValue"/>
						</xsl:attribute>
					</parameter> 

					<model name="setpoints" type="vg::SignalCollection">
						<parameter name="rule" value="min"/>
						<parameter name="signalReset" ref="..[signalReset]"/>
						<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='3']/SetpointTimes//SetpointTime">
							<model name="setpoint" type="vg::DateTimeSignal">
								<xsl:call-template name="extract-period">
									<xsl:with-param name="circadian" select="'true'"/>
								</xsl:call-template>
								<parameter name="signalOutside" value="100"/>
								<parameter name="signalOutsideTimeOnly" value="100"/>
								<parameter name="signalInside">
									<xsl:attribute name="value">
										<xsl:value-of select="SetpointValue"/>
									</xsl:attribute>
								</parameter>
							</model>
						</xsl:for-each>
					</model>
					
					<!-- Decrease ventilation set point temperature at high humidity -->
					<model name="adjustments" type="vg::SignalCollection">
						<parameter name="rule" value="min"/>
						<parameter name="signalReset" value="0"/>
						<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='40']/SetpointTimes//SetpointTime">

							<model type="vg::DateTimeSignal">
								<xsl:call-template name="extract-period">
									<xsl:with-param name="circadian" select="'true'"/>
								</xsl:call-template>
								<parameter name="signalOutside" value="0"/>
								<parameter name="signalOutsideTimeOnly" value="0"/>
								<parameter name="signalInside" ref="./adjustment[signal]"/>
					
								<model name="adjustment" type="vg::ProportionalSignal">
									<parameter name="input" ref="indoors/humidity[rh]"/>
									<parameter name="threshold" ref="setpoints/humidity/maximumRh[signal]"/>
									<parameter name="thresholdBand">
										<xsl:variable name="user-sp" select="max(//Vents/Vent/Constants/Parameters[ParameterID='489']/Value)"/>
										<xsl:attribute name="value">
											<xsl:if test="$user-sp &lt; $MinVentilationPbandRH">
												<xsl:value-of select="$MinVentilationPbandRH"/>
											</xsl:if>
											<xsl:if test="$user-sp &gt;= $MinVentilationPbandRH">
												<xsl:value-of select="$user-sp"/>
											</xsl:if>
										</xsl:attribute>
									</parameter>
									<parameter name="increasingSignal" value="false"/>
									<parameter name="minSignal">
										<xsl:attribute name="value">
											<xsl:value-of select="-SetpointValue"/>
										</xsl:attribute>
									</parameter>
									<parameter name="maxSignal" value="0"/>
								</model>
							</model>
						</xsl:for-each>
					</model>
				</model>
			</model>
		
			<!-- Day/Night setpoints not implemented in MatLab nor UniSim model -->
			<!-- spTheatmax to avoid cold-fall from screens not implemented in MatLab nor UniSim model-->
			<model name="heating" type="vg::PidControlElement">
				<parameter name="initState" ref="./target[signalReset]"/>
				<parameter name="signal" ref="./target[signal]"/>
				<parameter name="Kprop" value="0.1"/>
				<model name="target" type="vg::SignalCollection">
					<parameter name="rule" value="sum"/>
					<parameter name="signalReset">
						<xsl:attribute name="value">
							<xsl:value-of select="//Setpoint[ParameterId='2']//SetpointValue"/>
						</xsl:attribute>
					</parameter> 
					<model name="setpoints" type="vg::SignalCollection">
						<parameter name="rule" value="max"/>
						<parameter name="signalReset" ref="..[signalReset]"/>
						<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='2']/SetpointTimes//SetpointTime">
						
							<model name="setpoint" type="vg::DateTimeSignal">
								<xsl:call-template name="extract-period">
									<xsl:with-param name="circadian" select="'true'"/>
								</xsl:call-template>
								<parameter name="signalOutside" value="0"/>
								<parameter name="signalOutsideTimeOnly" value="0"/>
								<parameter name="signalInside">
									<xsl:attribute name="value">
										<xsl:value-of select="SetpointValue"/>
									</xsl:attribute>
								</parameter>
							</model>
						</xsl:for-each>
					</model>
					
					<!-- Increase heating set point at high humidity -->
					<model name="adjustmentsRh" type="vg::SignalCollection">
						<parameter name="signalReset" value="0"/>
						<parameter name="rule" value="max"/>
						<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='22']/SetpointTimes//SetpointTime">
						
							<model type="vg::DateTimeSignal">
								<xsl:call-template name="extract-period">
									<xsl:with-param name="circadian" select="'true'"/>
								</xsl:call-template>
								<parameter name="signalReset" value="0"/>
								<parameter name="signalOutside" value="0"/>
								<parameter name="signalOutsideTimeOnly" value="0"/>
								<parameter name="signalInside" ref="./adjustment[signal]"/>
					
								<model name="adjustment" type="vg::ProportionalSignal">
									<parameter name="input" ref="indoors/humidity[rh]"/>
									<parameter name="threshold" ref="setpoints/humidity/maximumRh[signal]"/>
									<parameter name="thresholdBand">
										<xsl:variable name="user-sp" select="max(//Heatpipes/Heatpipe/Constants/Parameters[ParameterID='540']/Value)"/>
										<xsl:attribute name="value">
											<xsl:if test="$user-sp &lt; $MinHeatingPbandRH">
												<xsl:value-of select="$MinHeatingPbandRH"/>
											</xsl:if>
											<xsl:if test="$user-sp &gt;= $MinHeatingPbandRH">
												<xsl:value-of select="$user-sp"/>
											</xsl:if>
										</xsl:attribute>
									</parameter>
									<parameter name="increasingSignal" value="true"/>
									<parameter name="minSignal" value="0"/>
									<parameter name="maxSignal">
										<xsl:attribute name="value">
											<xsl:value-of select="SetpointValue"/>
										</xsl:attribute>
									</parameter>
								</model>
							</model>
						</xsl:for-each>
					</model>

					<!-- Increase heating set point at high light -->
					<model name="adjustmentLight" type="vg::ProportionalSignal">
						<parameter name="input" ref="outdoors[radiation]"/>
						<parameter name="threshold">
							<xsl:attribute name="value">
								<xsl:value-of select="$HeatingLightAdjustmentThreshold"/>
							</xsl:attribute>
						</parameter>
						<parameter name="thresholdBand">
							<xsl:attribute name="value">
								<xsl:value-of select="$HeatingLightAdjustmentThresholdBand"/>
							</xsl:attribute>
						</parameter>
						<parameter name="increasingSignal" value="true"/>
						<parameter name="minSignal" value="0"/>
						<parameter name="maxSignal">
							<xsl:attribute name="value">
								<xsl:value-of select="$HeatingLightAdjustment"/>
							</xsl:attribute>
						</parameter>
					</model>
				</model>
			</model>
		</model>

		<model name="CO2">
			<model name="minimum" type="vg::SignalCollection">
				<parameter name="rule" value="max"/>
				<parameter name="signalReset" value="0"/>
				<parameter name="rule" value="max"/>
				<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='28']/SetpointTimes//SetpointTime">
					<model type="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'true'"/>
						</xsl:call-template>
						<parameter name="signalOutside" value="0"/>
						<parameter name="signalOutsideTimeOnly" value="0"/>
						<parameter name="signalInside">
							<xsl:attribute name="value">
								<xsl:value-of select="SetpointValue"/>
							</xsl:attribute>
						</parameter>
					</model>
				</xsl:for-each>
			</model>

			<model name="maximum" type="vg::SignalCollection">
				<parameter name="rule" value="min"/>
				<parameter name="signalReset" value="2000"/>
				<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='27']/SetpointTimes//SetpointTime">
					<model type="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'true'"/>
						</xsl:call-template>
						<parameter name="signalOutside" value="0"/>
						<parameter name="signalOutsideTimeOnly" value="0"/>
						<parameter name="signalInside">
							<xsl:attribute name="value">
								<xsl:value-of select="SetpointValue"/>
							</xsl:attribute>
						</parameter>
					</model>
				</xsl:for-each>
			</model>
		</model>
	</model>

	<model name="controllers">
		<model name="screens">
			<!-- When too humid, do not draw screens completely -->
			<model name="maxDrawn" type="vg::PidControlElement"> 
				<parameter name="initState" value="1"/>
				<parameter name="signal" ref="./target[signal]"/>
				<parameter name="Kprop" value="0.1"/>
				<model name="target" type="vg::ThresholdSignal"> 
					<parameter name="input" ref ="indoors/humidity[rh]"/>
					<parameter name="threshold" ref ="setpoints/humidity/maximumRh[signal]"/>
					<parameter name="signalBelow" value ="1"/>
					<parameter name="signalAbove">
						<xsl:attribute name="value">
							<xsl:value-of select="1 - max(//Screens/Screen/Constants/Parameters[ParameterID='522']/Value) div 100"/>
						</xsl:attribute>
					</parameter>
				</model>
			</model>

			<model name="energy">
				<xsl:choose>
					<xsl:when test="$EnergyScreenOption=1">
						<model name="control" type="vg::SignalCollection">
							<parameter name="rule" value="min"/>
							<parameter name="signalReset" value ="0"/>
							<!-- Draw when it is dark -->
							<model type="vg::ThresholdSignal">
								<parameter name="input" ref ="outdoors[radiation]"/>
								<parameter name="threshold">
									<xsl:attribute name="value">
										<xsl:value-of select="$spLightEnergyScreen"/>
									</xsl:attribute>
								</parameter>
								<parameter name="signalBelow" ref ="controllers/screens/maxDrawn[value]"/>
								<parameter name="signalAbove" value ="0"/>
							</model>
							<!-- And it is cool inside -->
							<model type="vg::ThresholdSignal">
								<parameter name="input" ref ="indoors/temperature[value]"/>
								<parameter name="threshold" ref ="setpoints/temperature/ventilation[value]"/>
								<parameter name="signalBelow" value ="1"/>
								<parameter name="signalAbove" value ="0"/>
							</model>
							<!-- And the energy balance without screens is negative -->
							<model type="vg::EnergyScreenBalanceSignal"/>
						</model>
					</xsl:when>
					<xsl:when test="$EnergyScreenOption=2">
						<model name="control" type="vg::SignalCollection">
							<parameter name="rule" value="min"/>
							<parameter name="signalReset" value ="0"/>
							<!-- Draw when it is dark -->
							<model type="vg::ThresholdSignal">
								<parameter name="input" ref ="outdoors[radiation]"/>
								<parameter name="threshold">
									<xsl:attribute name="value">
										<xsl:value-of select="$spLightEnergyScreen"/>
									</xsl:attribute>
								</parameter>
								<parameter name="signalBelow" ref ="controllers/screens/maxDrawn[value]"/>
								<parameter name="signalAbove" value ="0"/>
							</model>
							<!-- And it is cool inside -->
							<model type="vg::ThresholdSignal">
								<parameter name="input" ref ="indoors/temperature[value]"/>
								<parameter name="threshold" ref ="setpoints/temperature/ventilation[signal]"/>
								<parameter name="signalBelow" value ="1"/>
								<parameter name="signalAbove" value ="0"/>
							</model>
						</model>
					</xsl:when>
				</xsl:choose>
			</model>
			
			<model name="shade">
				<model name="control" type="vg::SignalCollection">
					<parameter name="rule" value="max"/>
					<parameter name="signalReset" value ="0"/>
					<!-- Draw when it is too light  -->
					<model type="vg::ThresholdSignal">
						<parameter name="input" ref ="outdoors[radiation]"/>
						<parameter name="threshold">
							<xsl:attribute name="value">
								<xsl:value-of select="$spShadowThresholdScreen"/>
							</xsl:attribute>
						</parameter>
						<parameter name="signalBelow" value ="0"/>
						<parameter name="signalAbove" ref ="controllers/screens/maxDrawn[value]"/>
					</model>
					<!-- Or when energy screen is drawn -->
					<xsl:if test="$Use2ndScreenWithEnergy=1">
						<model type="vg::FixedSignal">
							<parameter name="input" ref="controllers/screens/energy/control[signal]"/>
						</model>
					</xsl:if>
				</model>
			</model>

			<model name="blackout">
				<!-- Draw if there is any light  -->
				<model name="light" type="vg::ThresholdSignal">
					<parameter name="input" ref="outdoors[radiation]"/>
					<parameter name="threshold" value="0"/>
					<parameter name="signalBelow" value="0"/>
					<parameter name="signalAbove" ref ="controllers/screens/maxDrawn[value]"/>
				</model>
				<model name="control" type="vg::SignalCollection">
					<parameter name="rule" value="max"/>
					<parameter name="signalReset" value="0"/>
					<model type="vg::SignalCollection">
						<parameter name="rule" value="max"/>
						<parameter name="signalReset" value="0"/>
						<!-- Draw at night time  -->
						<model type="vg::DateTimeSignal">
							<parameter name="beginDay" value="1"/>
							<parameter name="endDay" value="365"/>
							<parameter name="beginTime" value="18:00"/>
							<parameter name="endTime" value="6:00"/>
							<parameter name="signalInside" value="1"/>
							<parameter name="signalOutside" value="0"/>
							<parameter name="signalOutsideTimeOnly" value="0"/>
						</model>
					</model>
					<!-- Or when energy screen is drawn -->
					<xsl:if test="$Use2ndScreenWithEnergy=1">
						<model type="vg::FixedSignal">
							<parameter name="input" ref="controllers/screens/energy/control[signal]"/>
						</model>
					</xsl:if>
				</model>
			</model>
		</model>
<!--
		<model name="ventilation" type="vg::VentilationController">
			<model name="byTemperature" type="vg::ProportionalSignal">
				<parameter name="input" ref="indoors/temperature[value]"/>
				<parameter name="threshold" ref="setpoints/temperature/ventilation[value]"/>
				<parameter name="thresholdBand">
					<xsl:attribute name="value">
						<xsl:value-of select="max(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='489']/Value)"/>  <-Hack
					</xsl:attribute>
				</parameter>
				<parameter name="increasingSignal" value="true"/>
				<parameter name="maxSignal" value="100"/>
				<parameter name="minSignal" value="0"/>
			</model>
			<model name="byHumidity" type="vg::ProportionalSignal">
				<parameter name="input" ref="indoors/humidity[rh]"/>
				<parameter name="threshold" ref="setpoints/humidity/maximumRh[signal]"/>
				<parameter name="thresholdBand">
					<xsl:attribute name="value">
						<xsl:value-of select="max(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='492']/Value)"/> <-Hack
					</xsl:attribute>
				</parameter>
				<parameter name="increasingSignal" value="true"/>
				<parameter name="maxSignal" value="5"/>  <- sp.spRH.maxVentopeningRH_day=5;  sp.spRH.maxVentopeningRH_night=5; 
				<parameter name="minSignal" value="0"/>
			</model>
			<model name="leeFactor" type="vg::ProportionalSignal">
				<parameter name="input" ref="outdoors[windspeed]"/>
				<parameter name="threshold" value="15"/>
				<parameter name="thresholdBand" value="5"/>
				<parameter name="increasingSignal" value="false"/>
				<parameter name="maxSignal" value="1"/>
				<parameter name="minSignal" value="0"/>
			</model>
			<model name="windFactor" type="vg::ProportionalSignal">
				<parameter name="input" ref="outdoors[windspeed]"/>
				<parameter name="threshold" value="10"/>
				<parameter name="thresholdBand" value="5"/>
				<parameter name="increasingSignal" value="false"/>
				<parameter name="maxSignal" value="1"/>
				<parameter name="minSignal" value="0"/>
			</model>
			<model name="coldFactor" type="vg::ProportionalSignal"> 
				<parameter name="input" ref ="outdoors[temperature]"/>
				<parameter name="threshold" value ="-5"/>    <- sp.VentsspFrostProtection_alpha=-5 
				<parameter name="thresholdBand" value="1"/>
				<parameter name="increasingSignal" value="true"/>
				<parameter name="maxSignal" value="1"/>
				<parameter name="minSignal" value="0"/>
			</model>
		</model>

		<model name="heating">
			<model name="temperature" type="vg::HeatingTemperatureController">
				<model name="Tdiff" type="vg::RunningAverage">
					<parameter name="input" ref ="..[Tdiff]"/>
					<parameter name="length" value ="30"/>
				</model>
				<- For hvert rør:
				Se CommonFlowTemperature og CommonReturnTemperature
				
				<parameter name="minimumSignal" value ="20"/>
				<parameter name="maximumSignal" value ="90"/>
			</model>
		</model>
-->
		<model name="growthLight" type="vg::GrowthLightController">
			<model name="on" type="vg::SignalCollection">
				<parameter name="rule" value ="max"/>
				<parameter name="signalReset" value ="0"/>
				<xsl:for-each select="//Setpoint[ParameterId='33']//SetpointTime">
					<model type="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'true'"/>
						</xsl:call-template>
						<parameter name="signalInside" ref ="./on[signal]"/>
						<parameter name="signalOutside" value="0"/>
						<parameter name="signalOutsideTimeOnly" value="0"/>

						<model name="on" type="vg::ThresholdSignal">
							<parameter name="threshold">
								<xsl:attribute name="value">
									<xsl:value-of select="SetpointValue"/>
								</xsl:attribute>
							</parameter>
							<parameter name="input" ref ="outdoors[radiation]"/>
							<parameter name="signalBelow" value ="1"/>
							<parameter name="signalAbove" value="0"/>
						</model>
					</model>
				</xsl:for-each>
			</model>
			
			<model name="off" type="vg::SignalCollection">
				<parameter name="rule" value ="max"/>
				<parameter name="signalReset" value ="0"/>
				<xsl:for-each select="//Setpoint[ParameterId='34']//SetpointTime">
					<model type="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'true'"/>
						</xsl:call-template>
						<parameter name="signalInside" ref ="./off[signal]"/>
						<parameter name="signalOutside" value="0"/>
						<parameter name="signalOutsideTimeOnly" value="0"/>

						<model name="off" type="vg::ThresholdSignal">
							<parameter name="threshold">
								<xsl:attribute name="value">
									<xsl:value-of select="SetpointValue"/>
								</xsl:attribute>
							</parameter>
							<parameter name="input" ref ="outdoors[radiation]"/>
							<parameter name="signalBelow" value ="0"/>
							<parameter name="signalAbove" value="1"/>
						</model>
					</model>
				</xsl:for-each>
			</model>
			
			<model name="periods" type="vg::SignalCollection">
				<parameter name="rule" value ="max"/>
				<parameter name="signalReset" value ="0"/>
				<xsl:for-each select="//Setpoint[ParameterId='35']//SetpointTime[SetpointValue='1']">
					<model type="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'true'"/>
						</xsl:call-template>
						<parameter name="signalInside" value ="1"/>
						<parameter name="signalOutside" value="0"/>
						<parameter name="signalOutsideTimeOnly" value="0"/>
					</model>
				</xsl:for-each>
			</model>
		</model>

		<model name="chalk" type="vg::SignalCollection">
			<parameter name="rule" value ="max"/>
			<parameter name="signalReset" value ="0"/>
			<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='39']/SetpointTimes//SetpointTime">
				<model type="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'false'"/>
						</xsl:call-template>
					<parameter name="signalOutside" value="0"/>
					<parameter name="signalInside">
						<xsl:attribute name="value">
							<xsl:value-of select="SetpointValue"/>
						</xsl:attribute>
					</parameter>
				</model>
			</xsl:for-each>
		</model>
		
	</model>

	<model name="actuators">
		<model name="screens">
			<model name="energy">
				<model name="control" type="vg::PidControlElement">
					<parameter name="signal" ref="controllers/screens/energy/control[signal]"/>
					<parameter name="Kprop" value="0.3"/>
				</model>
			</model>
			<model name="shade">
				<model name="control" type="vg::PidControlElement">
					<parameter name="signal" ref="controllers/screens/shade/control[signal]"/>
					<parameter name="Kprop" value="0.3"/>
				</model>
			</model>
			<model name="blackout">
				<model name="control" type="vg::PidControlElement">
					<parameter name="signal" ref="controllers/screens/blackout/control[signal]"/>
					<parameter name="Kprop" value="0.3"/>
				</model>
			</model>
		</model>
		<!--
		<model name="vents"> 
			<model name="leeside" type="vg::LinearControlElement">
				<parameter name="signal" ref="controllers/ventilation[leewardSignal]"/>
				<parameter name="rate" value="10"/>
			</model>
			<model name="windside" type="vg::LinearControlElement">
				<parameter name="signal" ref="controllers/ventilation[windwardSignal]"/>
				<parameter name="rate" value="10"/>
			</model>
		</model>
		<model name="heating">
			<model name="pipes" type="vg::HeatPipes"> 
				<xsl:for-each select="JobDataSet/Greenhouse/zone/Heatpipes/Heatpipe">
					<model type="vg::HeatPipe">
						<xsl:attribute name="name">
							<xsl:value-of select="concat('pipe', position())"/>
						</xsl:attribute>
						<parameter name="length">
							<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='535']/Value"/>
							</xsl:attribute>
						</parameter>
						<parameter name="diameter">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='62']/Value*1000"/>
							</xsl:attribute>
						</parameter>
						<parameter name="minTemperature">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='359']/Value"/>
							</xsl:attribute>
						</parameter>
						<parameter name="maxTemperature">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='358']/Value"/>
							</xsl:attribute>
						</parameter>
					</model>
				</xsl:for-each>
			</model>
		</model>
		-->
		<model name="growthLights" type="vg::GrowthLights">
			<xsl:for-each select="JobDataSet/Greenhouse/zone/Lamps/Lamp">
				<model type="vg::GrowthLight">
					<xsl:attribute name="name">
						<xsl:value-of select="concat('growthLight', position())"/>
					</xsl:attribute>
					<parameter name="type">
						<xsl:attribute name="value">
							<xsl:call-template name="lamp-type">
								<xsl:with-param name="number" select="Constants/Parameters[ParameterID='575']/Value"/>
							</xsl:call-template>
						</xsl:attribute>
					</parameter>
					<parameter name="intensity">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='533']/Value"/>
						</xsl:attribute>
					</parameter>
					<parameter name="lifeTime">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='397']/Value"/>
						</xsl:attribute>
					</parameter>
					<parameter name="age">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='398']/Value"/>
						</xsl:attribute>
					</parameter>
				</model>
			</xsl:for-each>
		</model>
	</model>

	<xsl:comment>
		<xsl:value-of select="concat($crop-name, ' model')"/>
	</xsl:comment>

	<model name="crop" type="vg::Crop">
		<xsl:call-template name="extract-crop-parameters"/>

		<xsl:call-template name="extract-crop-model">
			<xsl:with-param name="modelName">periods</xsl:with-param>
		</xsl:call-template>

		<model name="physTime" type="UniSim::DayDegrees">
			<parameter name="T" ref="indoors/temperature[value]"/>
			<parameter name="isTicking" ref="../periods[flag]"/>
			<parameter name="doReset" ref="../periods[flagDown]"/>
		</model>

		<xsl:call-template name="extract-crop-model">
			<xsl:with-param name="modelName">lai</xsl:with-param>
		</xsl:call-template>
		
		<model name="layers">
			<model name="top" type="vg::Layer">
				<parameter name="xGauss"  value="0.1127"/>
				<parameter name="wGauss"  value="0.2778"/>
				<model name="rs" type="vg::StomatalResistance"/>
				<model name="rb" type="vg::BoundaryLayerResistance"/>
				<model name="transpiration" type="vg::LeafTranspiration"/>
				<model name="rna" type="vg::RadiationAbsorbed"/>
				<model name="temperature" type="vg::LeafTemperature"/>
				<model name="photosynthesis" type="vg::LayerPhotosynthesis">
					<model name="lightResponse" type="vg::LeafLightResponse"/>	
				</model>
			</model>
			<model name="middle" type="vg::Layer">
				<parameter name="xGauss"  value="0.5"/>
				<parameter name="wGauss"  value="0.4444"/>
				<model name="rs" type="vg::StomatalResistance"/>
				<model name="rb" type="vg::BoundaryLayerResistance"/>
				<model name="transpiration" type="vg::LeafTranspiration"/>
				<model name="rna" type="vg::RadiationAbsorbed"/>
				<model name="temperature" type="vg::LeafTemperature"/>
				<model name="photosynthesis" type="vg::LayerPhotosynthesis">
					<model name="lightResponse" type="vg::LeafLightResponse"/>	
				</model>
			</model>
			<model name="bottom" type="vg::Layer">
				<parameter name="xGauss"  value="0.8873"/>
				<parameter name="wGauss"  value="0.2778"/>
				<model name="rs" type="vg::StomatalResistance"/>
				<model name="rb" type="vg::BoundaryLayerResistance"/>
				<model name="transpiration" type="vg::LeafTranspiration"/>
				<model name="rna" type="vg::RadiationAbsorbed"/>
				<model name="temperature" type="vg::LeafTemperature"/>
				<model name="photosynthesis" type="vg::LayerPhotosynthesis">
					<model name="lightResponse" type="vg::LeafLightResponse"/>	
				</model>
			</model>
		</model>
		
		<model name="temperature" type="Unisim::Average">
			<parameter name="inputs"  value="(layers/top/temperature[value] layers/middle/temperature[value] layers/bottom/temperature[value])"/> 
		</model>	
		
		<model name="conductance" type="Unisim::Average">
			<parameter name="inputs"  value="(layers/top/transpiration[conductance] layers/middle/transpiration[conductance] layers/bottom/transpiration[conductance])"/> 
		</model>	
		<model name="vapourFlux" type="Unisim::Average">
			<parameter name="inputs"  value="(layers/top/transpiration[vapourFlux] layers/middle/transpiration[vapourFlux] layers/bottom/transpiration[vapourFlux])"/>
		</model>
		<model name="gain" type="Unisim::Average">
			<parameter name="inputs"  value="(layers/top/transpiration[gain] layers/middle/transpiration[gain] layers/bottom/transpiration[gain])"/>
		</model>

		<model name="growth" type="vg::CropGrowth">
			<model name="Pgc" type="Unisim::Sum">
				<parameter name="toAdd"  value="(layers/top/photosynthesis[Pg] layers/middle/photosynthesis[Pg] layers/bottom/photosynthesis[Pg])"/>
			</model>
		</model>
		<xsl:call-template name="extract-crop-model">
			<xsl:with-param name="modelName">mass</xsl:with-param>
		</xsl:call-template>
		<xsl:call-template name="extract-crop-model">
			<xsl:with-param name="modelName">yield</xsl:with-param>
		</xsl:call-template>

	</model> <!-- crop -->

	<output name="output" type="table">
		<parameter name="fileName" value="dvv_unisim_0001.txt"/>

		<trace label="date" value="calendar[dateAsReal]"/>
		<trace label="energy_light" value="actuators/growthLights[energyUsed]"/>
		<trace label="energy_heat" value="energyFlux/heating/supply/supply[energyUsed]"/>
		<trace label="yield_kg_fw" value="crop/yield[dryWeight]"/>
		
		<trace label="windspeed" value="outdoors[windspeed]"/>
		<trace label="outdoors_temp" value="outdoors[temperature]"/>
		<trace label="floor_temp" value="floor/temperature[value]"/>
		<trace label="roof1_temp" value="roof1/cover/energyFlux[temperature]"/>
		<trace label="side1_temp" value="side1/cover/energyFlux[temperature]"/>
		<trace label="top_temp" value="indoors/top/temperature[value]"/>
		<trace label="indoors_temp" value="indoors/temperature[value]"/>

		<trace label="outdoors_rh" value="outdoors[rh]"/>
		<trace label="top_rh" value="indoors/top/humidity[rh]"/>
		<trace label="indoors_rh" value="indoors/humidity[rh]"/>
		<trace label="outdoors_ah" value="outdoors[ah]"/>
		<trace label="top_ah" value="indoors/top/humidity[ah]"/>
		<trace label="indoors_ah" value="indoors/humidity[ah]"/>

		<trace label="sp_heat" value="setpoints/temperature/heating[value]"/>
		<trace label="sp_vent" value="setpoints/temperature/ventilation[value]"/>
		<trace label="sp_rh" value="setpoints/humidity/maximumRh[signal]"/>

		<trace label="air_infilt" value="airFlux/infiltration[value]"/>
		<trace label="air_crack" value="airFlux/crackVentilation[value]"/>
		<trace label="air_top" value="total/top/airFlux[value]"/>
		<trace label="air_bottom" value="total/bottom/airFlux[value]"/>
		<trace label="air_gravi" value="airFlux/gravitation[value]"/>

		<trace label="top_energy_total" ref="total/top/energyFlux[value]"/>
		<trace label="top_cond" ref="given/top/energyFlux/condensation[value]"/>
		<trace label="top_air_outdoors1" ref="given/top/energyFlux/airFluxOutdoors[value]"/>
		<trace label="top_air_outdoors2" ref="controlled/top/energyFlux[value]"/>
		<trace label="top_air_indoors" ref="given/top/energyFlux/airFluxIndoors[value]"/>
		<trace label="top_air_conv" ref="given/top/energyFlux/convection[value]"/>
		<trace label="top_cover" ref="given/top/energyFlux/cover[value]"/>
		<trace label="top_light" ref="given/top/energyFlux/light[value]"/>

		<trace label="bottom_energy_total" ref="total/bottom/energyFlux[value]"/>
		<trace label="bottom_cond" ref="given/bottom/energyFlux/condensation[value]"/>
		<trace label="bottom_air_outdoors1" ref="given/bottom/energyFlux/airFluxOutdoors[value]"/>
		<trace label="bottom_air_outdoors2" ref="bottom/cooling/energyFlux[value]"/>
		<trace label="bottom_air_indoors" ref="given/bottom/energyFlux/airFluxIndoors[value]"/>
		<trace label="bottom_conv" ref="given/bottom/energyFlux/convection[value]"/>
		<trace label="bottom_cover" ref="given/bottom/energyFlux/cover[value]"/>
		<trace label="bottom_light" ref="given/bottom/energyFlux/light[value]"/>

		<trace label="bottom_trans" ref="given/bottom/energyFlux/transpiration[value]"/>
		<trace label="bottom_heat" ref="bottom/energyFlux/heating/supply[value]"/>
		<trace label="bottom_floor" ref="given/bottom/energyFlux/floor[value]"/>

		<trace label="cooling_indoors" ref="cooling/airFlux[fromOutdoorsToIndoors]"/>
		<trace label="cooling_top" ref="cooling/airFlux[fromOutdoorsToTop]"/>
		<trace label="horz_air_trans" ref="horizontalScreen[airTransmission]"/>

		<trace label="top_diffuse_trans" ref="greenhouseShelter/top[diffuseLightTransmission]"/>
		<trace label="top_dir_dir_trans" ref="greenhouseShelter/top[directLightTransmissionAsDirect]"/>
		<trace label="top_dir_diff_trans" ref="greenhouseShelter/top[directLightTransmissionAsDiffuse]"/>
		<trace label="horz_light_trans" ref="horizontalScreen[lightTransmission]"/>
		
		<trace label="outdoors_light" value="outdoors[radiation]"/>
		<trace label="growth_light" value="actuators/growthLights[shortWaveEmission]"/>
		<trace label="indoors_light" value="indoors/light[total]"/>
	
		<trace label="scr_max" value="controllers/screens/maxDrawn[value]"/>
		<trace label="act_scr_en" value="actuators/screens/energy/control[state]"/>
		<trace label="act_scr_sh" value="actuators/screens/shade/control[state]"/>
		<trace label="act_scr_bl" value="actuators/screens/blackout/control[state]"/>

		<trace label="LAI" value="crop/lai[lai]"/>
		<trace label="Pg_top" ref="layers/top/photosynthesis[Pg]"/>
		<trace label="Pg_mid" ref="layers/middle/photosynthesis[Pg]"/>
		<trace label="Pg_bot" ref="layers/bottom/photosynthesis[Pg]"/>
		<trace label="stem" ref="crop/mass[stem]"/>
		<trace label="leaf" ref="crop/mass[leaf]"/>
		<trace label="fruit" ref="crop/mass[fruit]"/>
	
<!-- 			
		<trace label="indoors_ah" value="indoors/humidity[ah]"/>
		<trace label="top_ah" value="top/transpiration[leafAh]"/>
		<trace label="top_rad" value="top/transpiration[absorbedRadiation]"/>
		<trace label="top_temp" value="top/transpiration[Tleaf]"/>
		<trace label="top_rb" value="top/transpiration[rbH2O]"/>
		<trace label="top_rs" value="top/transpiration[rsH2O]"/>

		<trace label="middle_rad" value="middle/transpiration[absorbedRadiation]"/>
		<trace label="middle_temp" value="middle/transpiration[Tleaf]"/>
		<trace label="middle_rb" value="middle/transpiration[rbH2O]"/>
		<trace label="middle_rs" value="middle/transpiration[rsH2O]"/>

		<trace label="bottom_ah" value="bottom/transpiration[leafAh]"/>
		<trace label="bottom_rad" value="bottom/transpiration[absorbedRadiation]"/>
		<trace label="bottom_temp" value="bottom/transpiration[Tleaf]"/>
		<trace label="bottom_rb" value="bottom/transpiration[rbH2O]"/>
		<trace label="bottom_rs" value="bottom/transpiration[rsH2O]"/>
 -->
	</output>
	
		
</simulation> </xsl:template>

</xsl:stylesheet>
