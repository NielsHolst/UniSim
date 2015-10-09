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
<xsl:variable name="EnergyScreenOption" select="1"/>  		<!-- 1: EnergyBalanc or 2: OutsideLight --> 
<xsl:variable name="spLightEnergyScreen" select="10"/>
<xsl:variable name="spShadowThresholdScreen" select="500"/>
<xsl:variable name="Use2ndScreenWithEnergy" select="1"/>
<xsl:variable name="MinHeatingPbandRH" select="5"/>			<!-- See GHValues.Control.Pipes.PbandRH -->
<xsl:variable name="MinVentilationPbandRH" select="5"/>		<!-- See GHValues.Control.Pipes.PbandRH -->
<xsl:variable name="HeatingLightAdjustmentThreshold" select="300"/>
<xsl:variable name="HeatingLightAdjustmentThresholdBand" select="100"/>
<xsl:variable name="HeatingLightAdjustment" select="1"/>
<xsl:variable name="OutdoorsCo2" select="400"/>
<xsl:variable name="Co2MaxSupply" select="4.5"/>				<!-- g/m2/h -->
<xsl:variable name="Co2VentilationThreshold" select="6"/>	<!-- h-1 -->
<xsl:variable name="Co2VentilationThresholdBand" select="1"/> <!-- h-1 -->

<xsl:variable name="CoverEmissitivity" select="0.93"/>  		<!-- [0;1] -->
<xsl:variable name="CoverAbsorptivity" select="0.04"/>		<!-- [0;1] -->
<xsl:variable name="CoverHaze" select="0"/>  					<!-- [0;1] -->
<xsl:variable name="CoverHeatCapacity" select="8736"/>		<!-- Heat capacity [J/m2/K] -->
																<!-- density*specHeatCapacity*thickness/1000  = 2600 kg/m3 * 840 J/kg/K * 4 mm *m/mm  -->
<xsl:variable name="ScreenHaze" select="0.8"/>				<!-- [0;1] -->
<xsl:variable name="ScreenEmissivityInner" select="0.62"/>	<!-- [0;1] -->
<xsl:variable name="ScreenEmissivityOuter" select="0.06"/>	<!-- [0;1] -->
<xsl:variable name="ScreenU" select="2"/>	                  

<xsl:variable name="MaxCrackVentilation" select="4"/>			<!-- [h-1] (GCC, p. 148) -->
<xsl:variable name="Leakage" select="1"/>						<!-- [h-1] -->

<xsl:variable name="FloorUindoors" select="7.5"/>	
<xsl:variable name="FloorUsoil" select="4"/>	
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
		<xsl:when test="$cropType=22">TomatoYearRound</xsl:when>
		<xsl:when test="$cropType=5">Cucumber</xsl:when>
		<xsl:when test="$cropType=6">SweetPepper</xsl:when>
		<xsl:when test="$cropType=7">Lettuce</xsl:when>
		<xsl:when test="$cropType=8">PotPlant</xsl:when>
		<xsl:when test="$cropType=24">Euphorbia</xsl:when>
		<xsl:otherwise>PotPlant</xsl:otherwise>
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

<!--
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
-->

<xsl:template name="screen-position-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">roof1_roof2_trapez</xsl:when>
		<xsl:when test="$number=2">roof1_roof2_horizontal</xsl:when>
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
	<parameter name="emissivityInner">
		<xsl:attribute name="value">
			<xsl:value-of select="$ScreenEmissivityInner"/>
		</xsl:attribute>
	</parameter>
	<parameter name="emissivityOuter">
		<xsl:attribute name="value">
			<xsl:value-of select="$ScreenEmissivityOuter"/>
		</xsl:attribute>
	</parameter>
	<parameter name="transmissivityLight">
		<xsl:attribute name="value">
			<xsl:value-of select="Constants/Parameters[ParameterID='30']/Value div 100"/>
		</xsl:attribute>
	</parameter>
	<parameter name="transmissivityAir">
		<xsl:attribute name="value">
			<xsl:value-of select="Constants/Parameters[ParameterID='32']/Value div 100"/>
		</xsl:attribute>
	</parameter>
	<parameter name="energyLossReduction">
		<xsl:attribute name="value">
			<xsl:value-of select="Constants/Parameters[ParameterID='33']/Value div 100"/>
		</xsl:attribute>
	</parameter>
	
	<parameter name="U">
		<xsl:attribute name="value">
			<xsl:value-of select="$ScreenU"/>
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
	<xsl:param name="shelter-position"/>
	<model name="screens" type="vg::Screens">
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
				</model>
			</xsl:if>
		</xsl:for-each>					
	</model>
</xsl:template>

<xsl:template name="extract-shelter">
	<model>
		<xsl:variable name="shelter-position">
			<xsl:call-template name="cover-position-name">
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
			<parameter name="specificHeatCapacity">
				<xsl:attribute name="value">
					<xsl:value-of select="$CoverHeatCapacity"/>
				</xsl:attribute>
			</parameter>
			<parameter name="haze">
				<xsl:attribute name="value">
					<xsl:value-of select="$CoverHaze"/>
				</xsl:attribute>
			</parameter>
		</model> <!-- cover -->

		<xsl:call-template name="extract-screens">
			<xsl:with-param name="shelter-position" select="$shelter-position"/>
		</xsl:call-template>
	</model>
</xsl:template>

<xsl:template name="crop-layer">
	<model name="windSpeed" type="vg::LeafWindSpeed">
		<parameter name="k" value="0.6"/>
	</model>
	<xsl:call-template name="extract-crop-model">
		<xsl:with-param name="modelName">rs</xsl:with-param>
	</xsl:call-template>
	<model name="rb" type="vg::BoundaryLayerResistanceStanghellini"/>
	<model name="radiationAbsorbed" type="vg::LeafRadiationAbsorbed"/>
	<model name="transpiration" type="vg::LeafTranspiration"/>
	<!--
	<model name="condensation">
		<model name="vapourFlux" type="vg::VapourFluxCondensation">
			<parameter name="surfaceAreaPerGroundArea" ref="./lai[value]"/>
			<parameter name="surfaceTemperature" ref="../../temperature[value]"/>
			<model name="lai" type="vg::LeafLai"/>
		</model>
		<model name="energyFlux" type="vg::EnergyFluxCondensation">
			<parameter name="vapourFlux" ref="../vapourflux[vapourFlux]"/>
		</model>
	</model>
	-->
	<model name="temperature" type="vg::LeafTemperature"/>
	<model name="photosynthesis" type="vg::LeafPhotosynthesis">
		<model name="lightResponse" type="vg::LeafLightResponse"/>	
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

		<model name="shelters" type="vg::Shelters">
			<xsl:for-each select="JobDataSet/Greenhouse/zone/Panes//Pane[Position &lt; 7]">
				<xsl:call-template name="extract-shelter"/>
			</xsl:for-each>		
			<!--
			<model name="roof" type="vg::Shelters">
				<xsl:for-each select="JobDataSet/Greenhouse/zone/Panes//Pane[Position &lt; 3]">
					<xsl:call-template name="extract-shelter"/>
				</xsl:for-each>		
			</model>
			<model name="walls" type="vg::Shelters">
				<xsl:for-each select="JobDataSet/Greenhouse/zone/Panes//Pane[Position > 2 and Position &lt; 7]">
					<xsl:call-template name="extract-shelter"/>
				</xsl:for-each>		
			</model>
			-->
		</model>


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
	</model>
	
	<model name="indoors">
		<model name="given">
			<model name="airflux" type="AirFluxGiven">
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
				
				<model name="gravitation" type="AirFluxGravitation"/>
			</model> <!-- airflux -->

			<model name="vapourFlux" type="vg::VapourFluxSum"> 
				<model name="transpiration" type="vg::VapourFluxTranspiration"/>
				<model name="condensationCover" type="vg::VapourFluxCondensation">
					<parameter name="surfaceAreaPerGroundArea" ref="geometry[coverPerGroundArea]"/>
					<parameter name="surfaceTemperature" ref="energyFlux/shelter[coverTemperature]"/>
				</model>
				<model name="condensationScreens" type="vg::VapourFluxCondensation">
					<parameter name="surfaceAreaPerGroundArea" ref="construction/shelters[screensPerGroundArea]"/>
					<parameter name="surfaceTemperature" ref="energyFlux/shelter[screensTemperature]"/>
				</model>
				<model name="airFluxOutdoors"  type="vg::VapourFluxAir">
					<parameter name="airFlux" ref="given/airflux[value]"/>
				</model>
			</model>
			
			<model name="energyFlux" type="vg::EnergyFluxSum"> 
				<model name="light" type="UniSim::Sum">
					<parameter name="toAdd" value="(indoors/light[total])"/>
				</model>
				<model name="condensationCover" type="vg::EnergyFluxCondensation">
					<parameter name="vapourFlux" ref="../../vapourflux/condensationCover[vapourFlux]"/>
				</model>
				<model name="condensationScreens" type="vg::EnergyFluxCondensation">
					<parameter name="vapourFlux" ref="../../vapourflux/condensationScreens[vapourFlux]"/>
				</model>
				<model name="airFlux" type="vg::EnergyFluxAir">
					<parameter name="airFlux" ref="given/airflux[value]"/>
				</model>
				<model name="growthLights" type="vg::PidControlElement">
					<parameter name="Kprop" value="0.5"/>
					<parameter name="signal" ref="./growthLights[value]"/>
					<model name="growthLights" type="vg::EnergyFluxGrowthLights"/>
				</model>
				<model name="shelter" type="vg::EnergyFluxShelters"/>
				<model name="floor" type="vg::EnergyFluxFloor"> 
					<model name="radiationAbsorbed" type="vg::FloorRadiationAbsorbed"/>
					<parameter name="Uindoors">
						<xsl:attribute name="value">
							<xsl:value-of select="$FloorUindoors"/>
						</xsl:attribute>
					</parameter>
					<parameter name="Usoil">
						<xsl:attribute name="value">
							<xsl:value-of select="$FloorUsoil"/>
						</xsl:attribute>
					</parameter>
					<parameter name="heatCapacity">
						<xsl:attribute name="value">
							<xsl:value-of select="$FloorHeatCapacity"/>
						</xsl:attribute>
					</parameter>
				</model>
				
				<model name="transpiration" type="vg::EnergyFluxTranspiration"/> 
			</model>		
		</model> <!-- given -->		

		<model name="controlled">
			<model name="energyFlux"> 
				<model name="heating">
					<model name="minSupply" type="Unisim::Sum">
						<parameter name="toAdd" value="(actuators/heating/pipes/*[nextEnergyFluxMin])"/>
					</model>
					<model name="maxSupply" type="Unisim::Sum">
						<parameter name="toAdd" value="(actuators/heating/pipes/*[nextEnergyFluxMax])"/>
					</model>
					<model name="demand" type="vg::EnergyFluxHeatingDemand"/>
					<model name="supply" type="vg::PidControlElement">
						<parameter name="Kprop" value="0.6"/>
						<parameter name="Kint" value="0.01"/>
						<parameter name="minimum" ref="../minSupply[value]"/>
						<parameter name="maximum" ref="../maxSupply[value]"/>
						<parameter name="signal" ref="../demand[value]"/>
					</model>				
				</model>		
				
				<model name="temperature" type="vg::IndoorsTemperature">
					<model name="energyFlux" type="Unisim::Sum">
						<parameter name="toAdd" value="(given/energyFlux[value] energyFlux/heating/supply[value])"/>
					</model>
					<parameter name="energyFlux" ref="./energyFlux[value]"/>
					<parameter name="baseTemperature" ref="indoors/temperature[value]"/>
				</model>

				<model name="cooling"> 
					<model name="demand" type="vg::EnergyFluxCoolingDemand"/>
					<model name="airSupplyMax" type="vg::AirFluxCoolingSupplyMax">
						<model name="byWind" type="vg::VentilationByWind">
							<parameter name="baseRate" value="10"/>
						</model>
						<model name="byTemp" type="vg::VentilationByTemp">
							<parameter name="dischargeCoefficient" value="0.3"/>
						</model>
					</model>
					<model name="supply" type="vg::PidControlElement">
						<parameter name="Kprop" value="0.5"/>
						<parameter name="maximum" value="0"/>
						<parameter name="maxSlope" value="1"/>
						<parameter name="minSlope" value="-1"/>
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
					<parameter name="airFlux" ref="cooling/airFlux[value]"/>
				</model>
				<model name="energyFlux" type="vg::EnergyFluxAir">
					<parameter name="airFlux" ref="cooling/airFlux[value]"/>
				</model>
			</model>
		</model> <!-- controlled -->
		
		<model name="total">
			<model name="airFlux" type="Unisim::Sum">
				<parameter name="toAdd" value="(given/airFlux[value] cooling/airFlux[value])"/>
			</model>
			<model name="vapourFlux" type="vg::VapourFluxSum">
				<parameter name="toAdd" value="(given/vapourFlux cooling/vapourFlux)"/>
			</model>
			<model name="energyFlux" type="Unisim::Sum">
				<parameter name="toAdd" value="(given/energyFlux[value] energyFlux/heating/supply[value] cooling/energyFlux[value])"/>
			</model>
		</model> <!-- total -->

		<model name="light" type="vg::IndoorsLight"/> 
		<model name="temperature" type="vg::IndoorsTemperature">
			<parameter name="resetValue">
				<xsl:attribute name="value">
					<xsl:value-of select="//Setpoint[ParameterId='2']//SetpointValue"/>
				</xsl:attribute>
			</parameter> 
			<parameter name="energyFlux" ref="total/energyFlux[value]"/>
		</model>
		<model name="humidity" type="vg::IndoorsHumidity"/>
		<model name="co2" type="vg::IndoorsCo2"/>
		<model name="windSpeed" type="vg::IndoorsWindSpeed"/>
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
				<parameter name="signalReset" value="1500"/>
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
		
		<model name="CO2" type="vg::CO2Controller">
			<parameter name="injectionRate" ref="./injectionRate[signal]"/>
			<model name="injectionRate" type="vg::ProportionalSignal">
				<parameter name="input" ref="indoors/total/airFlux[value]"/>
				<parameter name="threshold">
					<xsl:attribute name="value">
						<xsl:value-of select="$Co2VentilationThreshold"/>
					</xsl:attribute>
				</parameter>
				<parameter name="thresholdBand">
					<xsl:attribute name="value">
						<xsl:value-of select="$Co2VentilationThresholdBand"/>
					</xsl:attribute>
				</parameter>
				<parameter name="increasingSignal" value="false"/>
				<parameter name="minSignal" value="0"/>
				<parameter name="maxSignal">
					<xsl:attribute name="value">
						<xsl:value-of select="$Co2MaxSupply"/>
					</xsl:attribute>
				</parameter>
			</model>
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
		-->
		<model name="heating">
			<model name="pipes"> 
				<xsl:for-each select="JobDataSet/Greenhouse/zone/Heatpipes/Heatpipe">
					<model>
						<xsl:attribute name="name">
							<xsl:value-of select="concat('pipe', position())"/>
						</xsl:attribute>
						<xsl:attribute name="type">
							<xsl:value-of select="'vg::Pipe'"/>
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
			<model name="top" type="vg::LeafLayer">
				<parameter name="xGaussUpperside"  value="0.1127"/>
				<parameter name="wGaussUpperside"  value="0.2778"/>
				<parameter name="xGaussLowerside"  value="0.8873"/>
				<parameter name="wGaussLowerside"  value="0.2778"/>
				<xsl:call-template name="crop-layer"/>
			</model>
			<model name="middle" type="vg::LeafLayer">
				<parameter name="xGaussUpperside"  value="0.5"/>
				<parameter name="wGaussUpperside"  value="0.4444"/>
				<parameter name="xGaussLowerside"  value="0.5"/>
				<parameter name="wGaussLowerside"  value="0.4444"/>
				<xsl:call-template name="crop-layer"/>
			</model>
			<model name="bottom" type="vg::LeafLayer">
				<parameter name="xGaussUpperside"  value="0.8873"/>
				<parameter name="wGaussUpperside"  value="0.2778"/>
				<parameter name="xGaussLowerside"  value="0.1127"/>
				<parameter name="wGaussLowerside"  value="0.2778"/>
				<xsl:call-template name="crop-layer"/>
			</model>
		</model>
		
		<model name="radiation" type="vg::CropRadiation"/>

		<model name="temperature" type="Unisim::Average">
			<parameter name="inputs"  value="(layers/top/temperature[value] layers/middle/temperature[value] layers/bottom/temperature[value])"/> 
		</model>	

		<model name="lightAbsorbed" type="Unisim::Sum">
			<parameter name="toAdd" value="(layers/top/radiationAbsorbed[lightAbsorbed] layers/middle/radiationAbsorbed[lightAbsorbed] layers/bottom/radiationAbsorbed[lightAbsorbed])"/>
		</model>

		<model name="heatingAbsorbed" type="Unisim::Sum">
			<parameter name="toAdd" value="(layers/top/radiationAbsorbed[heatingAbsorbed] layers/middle/radiationAbsorbed[heatingAbsorbed] layers/bottom/radiationAbsorbed[heatingAbsorbed])"/>
		</model>

		<model name="growthLightLwAbsorbed" type="Unisim::Sum">
			<parameter name="toAdd" value="(layers/top/radiationAbsorbed[growthLightLwAbsorbed] layers/middle/radiationAbsorbed[growthLightLwAbsorbed] layers/bottom/radiationAbsorbed[growthLightLwAbsorbed])"/>
		</model>

		<model name="radiationAbsorbed" type="Unisim::Sum">
			<parameter name="toAdd" value="(layers/top/radiationAbsorbed[lightAbsorbed] layers/middle/radiationAbsorbed[lightAbsorbed] layers/bottom/radiationAbsorbed[lightAbsorbed])"/>
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
			<model name="Pg" type="Unisim::Sum">
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

	<model name="budget" type="vg::Budget"/>
	
	<output name="output" type="table">
		<parameter name="fileName" value="dvv_unisim_0001.txt"/>
		<!-- Time axis -->
		<trace label="date" value="calendar[dateAsReal]"/>
		
		<!-- Climate -->
		<trace label="outdoorsWindspeed" value="outdoors[windspeed]"/>
		<trace label="indoorsWindspeed" value="indoors/windspeed[value]"/>
		<trace label="indoorsCo2" ref="indoors/co2[value]"/>
		
		<trace label="outdoorsRh" value="outdoors[rh]"/>
		<trace label="indoorsRh" value="indoors/humidity[rh]"/>
		<trace label="outdoorsAh" value="outdoors[ah]"/>
		<trace label="indoorsAh" value="indoors/humidity[ah]"/>

		<trace label="indoorsTemperature" value="indoors/temperature[value]"/>
		<trace label="coverTemperature" value="given/energyFlux/shelter[coverTemperature]"/>
		<trace label="screenTemperature" value="given/energyFlux/shelter[screensTemperature]"/>
		<trace label="floorTemperature" value="energyFlux/floor[temperature]"/>

		<trace label="topTemperature" ref="layers/top/temperature[value]"/>
		<trace label="midTemperature" ref="layers/middle/temperature[value]"/>
		<trace label="botTemperature" ref="layers/bottom/temperature[value]"/>

		<trace label="topLightAbsorbed" ref="layers/top/radiationAbsorbed[lightAbsorbed]"/>
		<trace label="midLightAbsorbed" ref="layers/middle/radiationAbsorbed[lightAbsorbed]"/>
		<trace label="botLightAbsorbed" ref="layers/bottom/radiationAbsorbed[lightAbsorbed]"/>

		<trace label="topGrowthLightLwAbsorbed" ref="layers/top/radiationAbsorbed[growthLightLwAbsorbed]"/>
		<trace label="midGrowthLightLwAbsorbed" ref="layers/middle/radiationAbsorbed[growthLightLwAbsorbed]"/>
		<trace label="botGrowthLightLwAbsorbed" ref="layers/bottom/radiationAbsorbed[growthLightLwAbsorbed]"/>

		<trace label="topFloorLwAbsorbed" ref="layers/top/radiationAbsorbed[floorLwAbsorbed]"/>
		<trace label="midFloorLwAbsorbed" ref="layers/middle/radiationAbsorbed[floorLwAbsorbed]"/>
		<trace label="botFloorLwAbsorbed" ref="layers/bottom/radiationAbsorbed[floorLwAbsorbed]"/>

		<trace label="topShelterLoss" ref="layers/top/radiationAbsorbed[shelterLoss]"/>
		<trace label="midShelterLoss" ref="layers/middle/radiationAbsorbed[shelterLoss]"/>
		<trace label="botShelterLoss" ref="layers/bottom/radiationAbsorbed[shelterLoss]"/>

		<trace label="topHeatingAbsorbed" ref="layers/top/radiationAbsorbed[heatingAbsorbed]"/>
		<trace label="midHeatingAbsorbed" ref="layers/middle/radiationAbsorbed[heatingAbsorbed]"/>
		<trace label="botHeatingAbsorbed" ref="layers/bottom/radiationAbsorbed[heatingAbsorbed]"/>

		<!-- Controlled -->
		<trace label="airInfiltration" value="given/airFlux/infiltration[value]"/>
		<trace label="airHumCrack" value="given/airFlux/crackVentilation[value]"/>
		<trace label="airGravity" value="given/airFlux/gravitation[value]"/>
		<trace label="airGiven" value="given/airFlux[value]"/>

		<trace label="airCoolingMax" ref="cooling/airSupplyMax[value]"/>
		<trace label="airCoolingMaxByWind" ref="cooling/airSupplyMax/byWind[value]"/>
		<trace label="airCoolingMaxByTemp" ref="cooling/airSupplyMax/byTemp[value]"/>

		<trace label="energyCoolingDemand" value="energyflux/cooling/demand[value]"/>
		<trace label="energyCoolingSupply" value="energyflux/cooling/supply[value]"/>
		<trace label="airCoolingSupply" ref="cooling/airFlux[value]"/>
		<trace label="airCoolingSupplySlope" ref="cooling/supply[slope]"/>

		<trace label="spHeating" value="setpoints/temperature/heating[value]"/>
		<trace label="spVentilation" value="setpoints/temperature/ventilation[value]"/>
		<trace label="spRh" value="setpoints/humidity/maximumRh[signal]"/>
		<trace label="heatingSupply" ref="heating/supply[value]"/>

		<!-- Production -->
		<trace label="heatingEnergyFlux" value="budget[heatingEnergyFlux]"/>
		<trace label="heatingEnergyTotal" value="budget[heatingEnergyTotal]"/>
	
		<trace label="growthLightsEnergyFlux" value="budget[growthLightsEnergyFlux]"/>
		<trace label="growthLightsEnergyTotal" value="budget[growthLightsEnergyTotal]"/>
		<trace label="growthLightsPar" value="growthLights[parEmission]"/>

		<trace label="co2Flux" ref="budget[co2Flux]"/>
		<trace label="co2Total" ref="budget[co2Total]"/>

		<trace label="assimilation" ref="crop/growth/Pg[value]"/>
		<trace label="fruitGrowthRate" value="crop/mass[fruitGrowthRate]"/>
		<trace label="yieldFreshWeight" value="crop/yield[freshWeight]"/>

		<trace label="pipe1Temperature" value="pipes/pipe1[temperature]"/>
		<trace label="pipe2Temperature" value="pipes/pipe2[temperature]"/>
		<trace label="pipe1NextTempMin" value="pipes/pipe1[nextTemperatureMin]"/>
		<trace label="pipe2NextTempMin" value="pipes/pipe2[nextTemperatureMin]"/>
		<trace label="pipe1NextTempMax" value="pipes/pipe1[nextTemperatureMax]"/>
		<trace label="pipe2NextTempMax" value="pipes/pipe2[nextTemperatureMax]"/>
		<trace label="pipe1NextEnergyMin" value="pipes/pipe1[nextEnergyFluxMin]"/>
		<trace label="pipe2NextEnergyMin" value="pipes/pipe2[nextEnergyFluxMin]"/>
		<trace label="pipe1NextEnergyMax" value="pipes/pipe1[nextEnergyFluxMax]"/>
		<trace label="pipe2NextEnergyMax" value="pipes/pipe2[nextEnergyFluxMax]"/>

		<!-- Photosynthesis -->
		<trace label="top_Pn" ref="top/photosynthesis[Pn]"/>
		<trace label="mid_Pn" ref="middle/photosynthesis[Pn]"/>
		<trace label="bot_Pn" ref="bottom/photosynthesis[Pn]"/>

		<trace label="top_Pg" ref="top/photosynthesis[Pg]"/>
		<trace label="mid_Pg" ref="middle/photosynthesis[Pg]"/>
		<trace label="bot_Pg" ref="bottom/photosynthesis[Pg]"/>

		<trace label="top_rb_H2o" ref="top/rb[rbH2o]"/>
		<trace label="mid_rb_H2o" ref="middle/rb[rbH2o]"/>
		<trace label="bot_rb_H2o" ref="bottom/rb[rbH2o]"/>

		<trace label="top_rb_Co2" ref="top/rb[rbCo2]"/>
		<trace label="mid_rb_Co2" ref="middle/rb[rbCo2]"/>
		<trace label="bot_rb_Co2" ref="bottom/rb[rbCo2]"/>

		<trace label="top_rs_H2o" ref="top/rs[rsH2o]"/>
		<trace label="mid_rs_H2o" ref="middle/rs[rsH2o]"/>
		<trace label="bot_rs_H2o" ref="bottom/rs[rsH2o]"/>

		<trace label="top_rs_Co2" ref="top/rs[rsCo2]"/>
		<trace label="mid_rs_Co2" ref="middle/rs[rsCo2]"/>
		<trace label="bot_rs_Co2" ref="bottom/rs[rsCo2]"/>
		
		<trace label="top_LUE" ref="top/photosynthesis/lightResponse[LUE]"/>
		<trace label="mid_LUE" ref="middle/photosynthesis/lightResponse[LUE]"/>
		<trace label="bot_LUE" ref="bottom/photosynthesis/lightResponse[LUE]"/>

		<trace label="top_Pnmax" ref="top/photosynthesis/lightResponse[Pnmax]"/>
		<trace label="mid_Pnmax" ref="middle/photosynthesis/lightResponse[Pnmax]"/>
		<trace label="bot_Pnmax" ref="bottom/photosynthesis/lightResponse[Pnmax]"/>

		<trace label="top_Pgmax" ref="top/photosynthesis/lightResponse[Pgmax]"/>
		<trace label="mid_Pgmax" ref="middle/photosynthesis/lightResponse[Pgmax]"/>
		<trace label="bot_Pgmax" ref="bottom/photosynthesis/lightResponse[Pgmax]"/>

		<trace label="top_Rd" ref="top/photosynthesis/lightResponse[Rd]"/>
		<trace label="mid_Rd" ref="middle/photosynthesis/lightResponse[Rd]"/>
		<trace label="bot_Rd" ref="bottom/photosynthesis/lightResponse[Rd]"/>

		<trace label="airFluxTotal" ref="indoors/total/airflux[value]"/>
		
		<!-- Wageningen (2015) poster -->
		<trace label="topRtCO2" ref="top/photosynthesis/lightResponse[rtCO2]"/>
		<trace label="midRtCO2" ref="middle/photosynthesis/lightResponse[rtCO2]"/>
		<trace label="botRtCO2" ref="bottom/photosynthesis/lightResponse[rtCO2]"/>
	</output>
	
		
</simulation> </xsl:template>

</xsl:stylesheet>
