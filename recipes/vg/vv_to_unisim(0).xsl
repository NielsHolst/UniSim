<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output indent="yes" encoding="ISO-8859-1"/>

<!-- RESOURCES -->

<xsl:variable name="crops" select="document('crops.xml')/Crops"/>
	
<!-- CONFIGURATION -->

<!-- 0: File output for DVV; no progress bar
     1: Screen plot output for testing; show progress bar 
	 2: File output for testing; show progress bar -->
<xsl:variable name="test-mode" select="2"/>  

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

<xsl:variable name="CoverEmissitivity" select="0.84"/>  	<!-- [0;1] -->
<xsl:variable name="CoverAbsorption" select="0.04"/>		<!-- [0;1] -->

<xsl:variable name="CoverDensity" select="2600"/>			<!-- [kg/m3] -->
<xsl:variable name="CoverHeatCapacity" select="840"/>		<!-- Specific heat capacity [J/kg/K] -->
<xsl:variable name="CoverThickness" select="4"/>			<!-- [mm] -->

<xsl:variable name="MaxCrackVentilation" select="1"/>				<!-- [h-1] (GCC, p. 148) -->
		
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

<xsl:template name="extract-screen">
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
	<parameter name="lightTransmission">
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
</xsl:template>
	
<!-- MAIN -->
		
<xsl:template match="/"> <simulation name="Virtual Greenhouse">
	<xsl:variable name="startTime" select="JobDataSet/StartTime"/>
	<xsl:variable name="stopTime" select="JobDataSet/StopTime"/>
	<xsl:variable name="startYear" select="number( substring($startTime, 1, 4) )"/>
	<xsl:variable name="stopYear" select="number( substring($stopTime, 1, 4) )"/>
	<xsl:variable name="incYear" select="$stopYear - $startYear"/>

	<integrator name="steps" type="UniSim::StepsTimeInterval">
		<!--
		<parameter name="beginDate">
			<xsl:attribute name="value">
				<xsl:value-of select="concat(2001, substring($startTime, 5, 6))"/>
			</xsl:attribute>
		</parameter>

		<parameter name="endDate">
			<xsl:attribute name="value">
				<xsl:value-of select="concat(2001 + $incYear, substring($stopTime, 5, 6))"/>
			</xsl:attribute>
		</parameter>
-->
		<parameter name="beginDate" value="2001-01-01"/>
		<parameter name="endDate" value="2001-12-31"/>

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

		<parameter name="timeStep" value="4"/>
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
		<parameter name="sample" value="1"/>
<!--		
		<parameter name="sample">
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

		<model name="greenhouseSurface" type="vg::GreenhouseSurface">
			<xsl:for-each select="JobDataSet/Greenhouse/zone/Panes//Pane[Position &lt; 7]">
				<model type="vg::Surface">
					<xsl:variable name="surface-name">
						<xsl:call-template name="cover-position-name">
							<xsl:with-param name="number" select="Position"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:attribute name="name">
						<xsl:value-of select="$surface-name"/>
					</xsl:attribute>
					<model name="cover" type="vg::Cover">
						<parameter name="U4">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='310']/Value"/>
							</xsl:attribute>
						</parameter>
						<parameter name="emissivity">
							<xsl:attribute name="value">
								<xsl:value-of select="$CoverEmissitivity"/>
							</xsl:attribute>
						</parameter>
						<parameter name="absorption">
							<xsl:attribute name="value">
								<xsl:value-of select="$CoverAbsorption"/>
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
						<parameter name="diffuseTransmission">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='308']/Value div 100"/>
							</xsl:attribute>
						</parameter>
						<parameter name="haze" value="0"/>
						<parameter name="antiReflection" value="no"/>
						<model name="energyFlux" type="vg::EnergyFluxCover"/>
						<model name="condensation" type="vg::CoverCondensation"/> 
					</model>
					<model name="screens" type="vg::Screens">
						<xsl:for-each select="/JobDataSet/Greenhouse/zone/Screens//Screen">
							<xsl:variable name="screen-type">
								<xsl:call-template name="screen-type-name">
									<xsl:with-param name="number" select="Constants/Parameters[ParameterID='573']/Value"/>
								</xsl:call-template>
							</xsl:variable>
							<xsl:variable name="screen-name">
								<xsl:call-template name="screen-position-name">
									<xsl:with-param name="number" select="Position"/>
								</xsl:call-template>
							</xsl:variable>
							<xsl:if test="contains($screen-name, $surface-name)">
								<model type="vg::Screen">
									<xsl:attribute name="name">
										<xsl:value-of select="$screen-type"/>
									</xsl:attribute>
									<model name="temperature" type="vg::ScreenTemperature"/>
									<xsl:call-template name="extract-screen"/>
									<parameter name="state">
										<xsl:attribute name="ref">
											<xsl:value-of select="concat('actuators/screens/', $screen-type, '/control[state]')"/>
										</xsl:attribute>
									</parameter>
								</model>
							</xsl:if>
						</xsl:for-each>					
					</model>
				</model>
			</xsl:for-each>					
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
		
		<model name="floor">
			<model name="temperature" type="vg::FloorTemperature">
				<parameter name="U">
					<xsl:attribute name="value">
						<xsl:value-of select="JobDataSet/Greenhouse/zone/Panes//Pane[Position=7]/Constants/Parameters[ParameterID='310']/Value"/>
					</xsl:attribute>
				</parameter>
				<parameter name="initValue" value="18"/>
			</model>
		</model>
	</model>

	<model name="indoors">
		<model name="radiation" type="vg::IndoorsRadiation"/> 

		<model name="passive"> 
			<model name="ventilation" type="Unisim::Sum">
				<parameter name="toAdd" value="(./infiltration[value] ./crack[value])"/>
				<model name="infiltration" type="vg::AirInfiltration">
					<parameter name="leakage" value="1"/>
				</model>
				<model name="crack" type="vg::PidControlElement">
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
			</model>
			<model name="vapourFlux" type="vg::VapourFluxSum"> 
				<model name="transpiration" type="vg::VapourFluxTranspiration"/>
				<model name="evaporation" type="vg::VapourFluxSum"/>  <!-- None -->
				<model name="condensation" type="vg::VapourFluxSumGreenhouseCondensation"/> 
				<model name="ventilation"  type="vg::VapourFluxVentilation">
					<parameter name="ventilationSupply" ref="indoors/passive/ventilation[value]"/>
				</model>
			</model>
			<model name="energyFlux" type="vg::EnergyFluxSum"> 
				<model name="cover" type="vg::EnergyFluxCoverSum"/> 
				<model name="floor" type="vg::EnergyFluxFloor"/> 
				<model name="transpiration" type="vg::EnergyFluxTranspiration"/> 
				<model name="condensation" type="vg::EnergyFluxCondensation"/> 
				<model name="shortWave" type="vg::EnergyFluxShortWave"/> 
				<model name="growthLights" type="vg::PidControlElement">
					<parameter name="Kprop" value="0.5"/>
					<parameter name="signal" ref="./growthLights[value]"/>
					<model name="growthLights" type="vg::EnergyFluxGrowthLights"/>
				</model>
				<model name="ventilation" type="vg::EnergyFluxAir"> 
					<parameter name="ventilationSupply" ref="indoors/passive/ventilation[value]"/>
				</model>
			</model>		
		</model>		
		
		<model name="active">
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
						<parameter name="toAdd" value="(passive/energyFlux[value] active/energyFlux/heating/supply[value])"/>
					</model>
					<parameter name="energyFlux" ref="./energyFlux[value]"/>
				</model>
				
				<model name="cooling"> 
					<model name="demand" type="vg::EnergyFluxCoolingDemand"/>
					<model name="supply" type="vg::PidControlElement">
						<parameter name="Kprop" value="0.5"/>
						<parameter name="signal" ref="./supply[value]"/>
						<model name="supply" type="vg::EnergyFluxCoolingSupply">
							<model name="byWind" type="vg::VentilationByWind">
								<parameter name="baseRate" value="30"/>
							</model>
							<model name="byTemp" type="vg::VentilationByTemp"/>
						</model>
					</model>
				</model>				
			</model>
			<model name="ventilation" type="vg::VentilationByCooling"/>
			<model name="vapourFlux" type="vg::VapourFluxVentilation">
				<parameter name="ventilationSupply" ref="../ventilation[value]"/>
			</model>
		</model>		
		
		<model name="total">
			<model name="energyFlux" type="Unisim::Sum">
				<parameter name="toAdd" value="(passive/energyFlux[value] active/energyFlux/heating/supply[value] active/energyFlux/cooling/supply[value])"/>
			</model>
			<model name="ventilation" type="Unisim::Sum">
				<parameter name="toAdd" value="(passive/ventilation[value] active/ventilation[value])"/>
			</model>
			<model name="vapourFlux" type="vg::VapourFluxSum">
				<parameter name="toAdd" value="(passive/vapourFlux active/vapourFlux)"/>
			</model>
		</model>

		<model name="temperature" type="vg::IndoorsTemperature">
			<parameter name="initValue">
				<xsl:attribute name="value">
					<xsl:value-of select="//Setpoint[ParameterId='2']//SetpointValue"/>
				</xsl:attribute>
			</parameter> 
			<parameter name="energyFlux" ref="../total/energyFlux[value]"/>
		</model>
		<model name="humidity" type="vg::IndoorsHumidity"/>
		<model name="co2" type="vg::IndoorsCo2"/>
			</model>

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

	<xsl:choose>
		<xsl:when test="$test-mode=0 or $test-mode=2">
			<model name="fixed" type="UniSim::Fixed">
				<parameter name="parameters"  
					value="
				(
				(HeatEnergy 99.)
				(EnergyUseLight 99.)
				(EffectAgam 0.)
				(Tgh 99.)
				(Tgh_natural 99.)
				(RHgh 99.)
				(CO2air 99.)
				(Tcov 99.)
				(Tscreen 99.)
				(TrLight 99.)
				(VentVindu 99.)
				(Leakage_heat 99.)
				(Leakage_hum 99.)
				(alphawind 99.)
				(alphalee 99.)
				(DWyield 99.)
				(FWyield 99.)
				(NumberPlants 99.)
				(Tcrop 99.)
				(VPDcrop_air 99.)
				(LE 99.)
				(Rn 99.)
				(Bn 99.)
				(Pnc 99.)
				(radiIn 99.)
				(Tpipe1 99.)
				(Tpipe2 99.)
				(spTheat 99.)
				(spTvent 99.)
				(spRHgh 99.)
				(H2O_dehum 0.)
				(Dew 99.)
				(CO2supply 99.)
				)"/>
			</model>
			
			<output name="output" type="table">
				<parameter name="fileName" value="dvv_unisim_0001.txt"/>
	<!--
				<trace label="HeatEnergyi" value="actuators/heating/pipes[sumEnergy]"/>
				<trace label="EnergyUseLight" value="actuators/growthLights[energyUse]"/>
				<trace label="ElUseDehumidifieri" value="fixed[EffectAgam]"/>
				<trace label="Tghi" value="indoors/temperature[value]"/>
				<trace label="Tgh_naturali" value="fixed[Tgh_natural]"/>
				<trace label="RHghi" value="indoors/humidity[rh]"/>
				<trace label="CO2airi" value="indoors/co2[ppm]"/>
				<trace label="Tcovi" value="greenhouseSurface[temperature]"/>
				<trace label="Tscreeni" value="greenhouseSurface[temperature]"/>
				<trace label="TrLighti" value="cover/transmission[light]"/>
				<trace label="VentVindui" value="indoors/ventilation[rate]"/>
				<trace label="Leakage_heati" value="fixed[Leakage_heat]"/>
				<trace label="Leakage_humi" value="fixed[Leakage_hum]"/>
				<trace label="alphawindi" value="actuators/vents/windside[state]"/>
				<trace label="alphaleei" value="actuators/vents/leeside[state]"/>
				<trace label="DWyieldi" value="crop/yield[dryWeightRate]"/>
				<trace label="FWyieldi" value="crop/yield[freshWeightRate]"/>
				<trace label="NumberPlantsi" value="crop[density]"/>
				<trace label="Tcropi.1" value="crop/layers/top/temperature[value]"/>
				<trace label="Tcropi.2" value="crop/layers/middle/temperature[value]"/>
				<trace label="Tcropi.3" value="crop/layers/bottom/temperature[value]"/>
				<trace label="VPDcrop_airi" value="fixed[VPDcrop_air]"/>
				<trace label="LEi" value="fixed[LE]"/>
				<trace label="Rni" value="fixed[Rn]"/>
				<trace label="Bni" value="fixed[Bn]"/>
				<trace label="Pnci.1" value="crop/layers/top/photosynthesis[Pn]"/>
				<trace label="Pnci.2" value="crop/layers/middle/photosynthesis[Pn]"/>
				<trace label="Pnci.3" value="crop/layers/bottom/photosynthesis[Pn]"/>
				<trace label="radiIni" value="indoors/radiation[total]"/>
				<trace label="Tpipe1i" value="heating/pipes/pipe1[temperature]"/>
				<trace label="Tpipe2i" value="heating/pipes/pipe2[temperature]"/>
				<trace label="spTheat" value="setpoints/temperature/heating[value]"/>
				<trace label="spTvent" value="setpoints/temperature/ventilation[value]"/>
				<trace label="spRHghi" value="setpoints/humidity/maximumRh[signal]"/>
				<trace label="H2O_dehumi" value="fixed[H2O_dehum]"/>
				<trace label="Dewi" value="fixed[Dew]"/>
				<trace label="CO2supplyi" value="fixed[CO2supply]"/>
				<- Outputs from UniSim version ->
				<trace label="Tsky" value="outdoors[skyTemperature]"/>
				<trace label="Toutdoors" value="outdoors[temperature]"/>
				<trace label="outdoorsDirectRadiation" value="outdoors[directRadiation]"/>
				<trace label="outdoorsDiffuseRadiation" value="outdoors[diffuseRadiation]"/>
				<trace label="diffuseTransmission" value="indoors/screens/transmission[diffuse]"/>
				<trace label="directTransmissionAsDirect" value="indoors/screens/transmission[directAsDirect]"/>
				<trace label="directTransmissionAsDiffuse" value="indoors/screens/transmission[directAsDiffuse]"/>
				<trace label="growthLights" value="actuators/growthLights[shortWaveEmission]"/>
				<trace label="Pgc" value="crop/growth/Pgc[value]"/>
				<trace label="condConductance" value="indoors/humidity/condensation/greenhouse[conductance]"/>
				<trace label="condVapourFlux" value="indoors/humidity/condensation/greenhouse[vapourFlux]"/>
				<trace label="condGain" value="indoors/humidity/condensation/greenhouse[gain]"/>
				<trace label="cropConductance" value="indoors/humidity/transpiration/crop[conductance]"/>
				<trace label="cropVapourFlux" value="indoors/humidity/transpiration/crop[vapourFlux]"/>
				<trace label="cropGain" value="indoors/humidity/transpiration/crop[gain]"/>
				<trace label="ventConductance" value="indoors/humidity/ventilation/total[conductance]"/>
				<trace label="ventVapourFlux" value="indoors/humidity/ventilation/total[vapourFlux]"/>
				<trace label="ventGain" value="indoors/humidity/ventilation/total[gain]"/>
				<trace label="screensAirTransmission" value="indoors/screens/transmission[air]"/>
				<trace label="screensNetU" value="indoors/screens/transmission[netU]"/>
				<trace label="pipe1Inflow" value="heating/pipes/pipe1[inflowTemperature]"/>
				<trace label="TdiffTarget" value="controllers/heating/temperature[TdiffTarget]"/>
				-->

		<trace label="date" value="calendar[dateAsReal]"/>
		<trace label="energy_light" value="actuators/growthLights[energyUsed]"/>
		<trace label="energy_heat" value="active/energyFlux/heating/supply/supply[energyUsed]"/>
		<trace label="yield_dw" value="crop/yield[dryWeight]"/>
		
		<trace label="windspeed" value="outdoors[windspeed]"/>
		<trace label="outdoors_temp" value="outdoors[temperature]"/>
		<trace label="floor_temp" value="floor/temperature[value]"/>
		<trace label="cover_temp" value="greenhouseSurface[temperature]"/>
		<trace label="indoors_temp" value="indoors/temperature[value]"/>
		<trace label="indoors_rh" value="indoors/humidity[rh]"/>
		<trace label="sp_heat" value="setpoints/temperature/heating[value]"/>
		<trace label="sp_vent" value="setpoints/temperature/ventilation[value]"/>
		<trace label="sp_rh" value="setpoints/humidity/maximumRh[signal]"/>
		<trace label="leakage" value="passive/ventilation/infiltration[value]"/>
		<trace label="crack_vent" value="passive/ventilation/crack[value]"/>
<!--		<trace label="act_cool_dem_W" value="cooling/demand[value]"/>-->

		<trace label="pass_en_cover" value="passive/energyflux/cover[value]"/>
		<trace label="pass_en_floor" value="passive/energyflux/floor[value]"/>
		<trace label="pass_en_trans" value="passive/energyflux/transpiration[value]"/>
		<trace label="pass_en_cond" value="passive/energyflux/condensation[value]"/>
		<trace label="pass_en_shwav" value="passive/energyflux/shortwave[value]"/>
		<trace label="pass_en_lights" value="passive/energyflux/growthlights[value]"/>
		<trace label="pass_en_vent" value="passive/energyflux/ventilation[value]"/>
		<trace label="pass_en_total" value="passive/energyFlux[value]"/>
		<trace label="act_heat_D" value="active/energyFlux/heating/supply/supply[demand]"/>
		<trace label="act_heat" value="active/energyFlux/heating/supply[state]"/>
		<trace label="act_heat_I" value="active/energyFlux/heating/supply[errorIntegral]"/>
		<trace label="act_cool" value="active/energyFlux/cooling/supply[value]"/>
		<trace label="act_cool_I" value="active/energyFlux/cooling/supply[errorIntegral]"/>
		<trace label="en_flux_total" value="total/energyFlux/[value]"/>
		
		<trace label="scr_max" value="controllers/screens/maxDrawn[value]"/>
		<!--
		<trace label="scr_en" value="controllers/screens/energy/control[signal]"/>
		<trace label="scr_sh" value="controllers/screens/shade/control[signal]"/>
		<trace label="scr_bl" value="controllers/screens/blackout/control[signal]"/>
		-->
		<trace label="act_scr_en" value="actuators/screens/energy/control[state]"/>
		<trace label="act_scr_sh" value="actuators/screens/shade/control[state]"/>
		<trace label="act_scr_bl" value="actuators/screens/blackout/control[state]"/>

		<trace label="light_on" ref="controllers/growthLight[signal]"/>
		
		
<!-- 			
		<trace label="LAI" value="crop/lai[lai]"/>
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
 -->			</output>
	
		</xsl:when>
		<xsl:when test="$test-mode=1">
			<output type="plot" name="plot">
				<parameter name="title" value ="Energy screen"/>
				<trace label ="Days" type="time"/>
				<trace label="Radiation" ref="outdoors[radiation]" divisor="100"/>
				<trace label="Signal" ref="controllers/screens/energy[signal]"/>
				<trace label="State" ref="actuators/screens/energy/control[state]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Blackout screen"/>
				<trace label ="Days" type="time"/>
				<trace label="Radiation" ref="outdoors[radiation]" divisor="100"/>
				<trace label="Signal" ref="controllers/screens/blackout[signal]"/>
				<trace label="State" ref="actuators/screens/blackout/control[state]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Shade screen"/>
				<trace label ="Days" type="time"/>
				<trace label="Radiation" ref="outdoors[radiation]" divisor="100"/>
				<trace label="Signal" ref="controllers/screens/shade[signal]"/>
				<trace label="State" ref="actuators/screens/shade/control[state]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Cover transmission"/>
				<trace label ="Days" type="time"/>
				<trace label="Diffuse" ref="cover/transmission[diffuse]"/>
				<trace label="DirectDirect" ref="cover/transmission[directAsDirect]"/>
				<trace label="DirectDiffuse" ref="cover/transmission[directAsDiffuse]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Screens light transmission"/>
				<trace label ="Days" type="time"/>
				<trace label="Diffuse" ref="screens/transmission[diffuse]"/>
				<trace label="DirectDirect" ref="screens/transmission[directAsDirect]"/>
				<trace label="DirectDiffuse" ref="screens/transmission[directAsDiffuse]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Screens air transmission"/>
				<trace label ="Days" type="time"/>
				<trace label="Air" ref="screens/transmission[air]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Cover and screens U"/>
				<trace label ="Days" type="time"/>
				<trace label="U" ref="screens/transmission[U]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Indoors radiation"/>
				<trace label ="Days" type="time"/>
				<trace label="Direct" ref="indoors/radiation[direct]"/>
				<trace label="Diffuse" ref="indoors/radiation[diffuse]"/>
				<trace label="Total" ref="indoors/radiation[total]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Outdoors radiation"/>
				<trace label ="Days" type="time"/>
				<trace label="Direct" ref="outdoors[directRadiation]"/>
				<trace label="Diffuse" ref="outdoors[diffuseRadiation]"/>
				<trace label="Total" ref="outdoors[radiation]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Ventilation (m3/h/m2)"/>
				<trace label ="Days" type="time"/>
				<trace label="Infiltration" ref="indoors/ventilation/infiltration[value]"/>
				<trace label="Total" ref="indoors/ventilation[rate]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Relative ventilation"/>
				<trace label ="Days" type="time"/>
				<trace label="Proportion" ref="indoors/ventilation[relative]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Weather"/>
				<trace label ="Days" type="time"/>
				<trace label="Temperature" ref="outdoors[temperature]"/>
				<trace label="Windspeed" ref="outdoors[windspeed]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Heat fluxes (W/m2)"/>
				<trace label ="Days" type="time"/>
				<trace label="Irradiation" ref="indoors/energy[irradiation]"/>
				<trace label="Pipes" ref="actuators/heating/pipes[effect]"/>
				<trace label="Growth light" ref="indoors/energy[growthLight]"/>
				<trace label="Surface" ref="indoors/energy/surface[flux]"/>
				<trace label="Floor" ref="indoors/energy/floor[flux]"/>
				<trace label="Ventilation" ref="indoors/energy/ventilation[flux]"/>
				<trace label="Transpiration" ref="indoors/energy/transpiration[flux]"/>
				<trace label="Condensation" ref="indoors/energy/condensation[flux]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Heat balance (W/m2)"/>
				<trace label ="Days" type="time"/>
				<trace label="Balance" ref="indoors/energy[value]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Temperature"/>
				<trace label ="Days" type="time"/>
				<trace label="Outdoors" ref="outdoors[temperature]"/>
				<trace label="Indoors" ref="indoors/temperature[value]"/>
				<trace label="Cover" ref="indoors/cover/temperature[value]"/>
				<trace label="Screens" ref="indoors/screens/temperature[value]"/>
				<trace label="Floor" ref="indoors/floor/temperature[value]"/>
				<trace label="Heating sp" ref="setpoints/temperature/heating[value]"/>	
				<trace label="Vent sp" ref="setpoints/temperature/ventilation[value]"/>	
			</output>
			<output type="plot">
				<parameter name="title" value ="Temperature sp"/>
				<trace label ="Days" type="time"/>
				<trace label="Indoors" ref="indoors/temperature[value]"/>
				<trace label="Sp heat" ref="setpoints/temperature/heating[value]"/>
				<trace label="Sp vent" ref="setpoints/temperature/ventilation[value]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Relative humidity"/>
				<trace label ="Days" type="time"/>
				<trace label="Outdoors" ref="outdoors[rh]"/>
				<trace label="Indoors" ref="indoors/humidity[rh]"/>
				<trace label="Vent sp" ref="setpoints/humidity/maximumRh[setpoint]"/>	
			</output>
			<output type="plot">
				<parameter name="title" value ="Absolute humidity"/>
				<trace label ="Days" type="time"/>
				<trace label="Outdoors" ref="outdoors[ah]"/>
				<trace label="Indoors" ref="indoors/humidity[ah]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Ventilation"/>
				<trace label ="Days" type="time"/>
				<trace label="Signal" ref="controllers/ventilation[signal]"/>
				<trace label="Lee side state" ref="actuators/vents/leeside[state]"/>
				<trace label="Wind side state" ref="actuators/vents/windside[state]"/>
			</output>
			<!--
			<output type="plot">
				<parameter name="title" value ="Ventilation by humidity"/>
				<trace label="Rh" ref="indoors/humidity[rh]"/>
				<trace label="Signal" ref="ventilation/byHumidity[signal]" type="symbols"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Ventilation by temperature"/>
				<trace label="Temperature" ref="indoors/temperature[value]"/>
				<trace label="Signal" ref="ventilation/byTemperature[signal]" type="symbols"/>
			</output>
			-->
			<output type="plot">
				<parameter name="title" value ="Heating"/>
				<trace label ="Days" type="time"/>
				<trace label="Tinflow1" ref="heating/pipes/pipe1[inflowTemperature]"/>
				<trace label="Tpipe1" ref="heating/pipes/pipe1[temperature]"/>
				<trace label="Max" ref="controllers/heating/temperature[maximumSignal]"/>
				<trace label="Min" ref="controllers/heating/temperature[minimumSignal]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Conductances"/>
				<trace label ="Days" type="time"/>
				<trace label="Plant" ref="transpiration/plant[conductance]"/>
				<trace label="Cover" ref="condensation/cover[conductance]"/>
				<trace label="Screen" ref="condensation/screens[conductance]"/>
				<trace label="Ventilation" ref="ventilation/total[conductance]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Vapour fluxes"/>
				<trace label ="Days" type="time"/>
				<trace label="Plant" ref="transpiration/plant[vapourFlux]"/>
				<trace label="Cover" ref="condensation/cover[vapourFlux]"/>
				<trace label="Screen" ref="condensation/screens[vapourFlux]"/>
				<trace label="Ventilation" ref="ventilation/total[vapourFlux]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Energy expenditure (MJ/m2)"/>
				<trace label ="Days" type="time"/>
				<trace label="Sum" ref="actuators/heating/pipes[sumEnergy]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="CO2 (ppm)"/>
				<trace label ="Days" type="time"/>
				<trace label="Minimum" ref="minCO2[setpoint]"/>
				<trace label="Indoors" ref="indoors/co2[ppm]"/>
				<trace label="Outdoors" ref="outdoors[co2]"/>
			</output>

			<output type="table">
				<parameter name="filename" value ="co2.txt"/>
				<trace label ="Days" type="time"/>
				<trace label="Minimum" ref="minCO2[setpoint]"/>
				<trace label="Indoors" ref="indoors/co2[ppm]"/>
				<trace label="Outdoors" ref="outdoors[co2]"/>
				<trace label="Exchange" ref="indoors/ventilation[relative]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Leaf temperature"/>
				<trace label ="Days" type="time"/>
				<trace label="Top" ref="top/temperature[value]"/>
				<trace label="Middle" ref="middle/temperature[value]"/>
				<trace label="Bottom" ref="Bottom/temperature[value]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Pg (mg CO2/m2 ground/h)"/>
				<trace label ="Days" type="time"/>
				<trace label="Top" ref="top/photosynthesis[Pg]"/>
				<trace label="Middle" ref="middle/photosynthesis[Pg]"/>
				<trace label="Bottom" ref="Bottom/photosynthesis[Pg]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="Pn (g CO2/m2 ground/h)"/>
				<trace label ="Days" type="time"/>
				<trace label="Top" ref="top/photosynthesis[Pn]"/>
				<trace label="Middle" ref="middle/photosynthesis[Pn]"/>
				<trace label="Bottom" ref="Bottom/photosynthesis[Pn]"/>
			</output>
			<output type="plot">
				<parameter name="title" value ="PgMax (g CO2/m2 leaf/s)"/>
				<trace label ="Days" type="time"/>
				<trace label="Top" ref="top/photosynthesis/lightResponse[PgMax]"/>
				<trace label="Middle" ref="middle/photosynthesis/lightResponse[PgMax]"/>
				<trace label="Bottom" ref="Bottom/photosynthesis/lightResponse[PgMax]"/>
			</output>
			<output type="plot">
				<parameter name="title" value="PnMax (mg CO2/m2 leaf/s)"/>
				<trace label="Days" type="time"/>
				<trace label="Top" ref="top/photosynthesis/lightResponse[PnMax]"/>
				<trace label="Middle" ref="middle/photosynthesis/lightResponse[PnMax]"/>
				<trace label="Bottom" ref="Bottom/photosynthesis/lightResponse[PnMax]"/>
			</output>
			<output type="plot">
				<parameter name="title" value="Rd (mg CO2/m2 leaf/s)"/>
				<trace label="Days" type="time"/>
				<trace label="Top" ref="top/photosynthesis/lightResponse[Rd]"/>
				<trace label="Middle" ref="middle/photosynthesis/lightResponse[Rd]"/>
				<trace label="Bottom" ref="Bottom/photosynthesis/lightResponse[Rd]"/>
			</output>
			<output type="plot">
				<parameter name="title" value="LUE (mg CO2/J)"/>
				<trace label="Days" type="time"/>
				<trace label="Top" ref="top/photosynthesis/lightResponse[LUE]"/>
				<trace label="Middle" ref="middle/photosynthesis/lightResponse[LUE]"/>
				<trace label="Bottom" ref="Bottom/photosynthesis/lightResponse[LUE]"/>
			</output>
			<output type="plot">
				<parameter name="title" value="Effective ventilation"/>
				<trace label="Indoors temperature" ref="indoors/temperature[value]"/>
				<trace label="Rel. vent" ref="indoors/ventilation[relative]" type="symbols"/>
			</output>
			<output type="plot">
				<parameter name="title" value="Vents opening"/>
				<trace label="Indoors temperature" ref="indoors/temperature[value]"/>
				<trace label="Leeside" ref="vents/leeside[state]" type="symbols"/>
			</output>
			<output type="plot">
				<parameter name="title" value="Growth light"/>
				<trace label="Days" type="time"/>
				<trace label="Rn" ref="actuators/growthLights[shortWaveEmission]"/>
			</output>
			<output type="plot">
				<parameter name="title" value="Growing DD"/>
				<trace label="Days" type="time"/>
				<trace label="DD" ref="crop/physTime[total]"/>
			</output>
			<output type="plot">
				<parameter name="title" value="Crop LAI"/>
				<trace label="Days" type="time"/>
				<trace label="DD" ref="crop/lai[lai]"/>
			</output>
			<output type="plot">
				<parameter name="title" value="Crop Mass"/>
				<trace label="Days" type="time"/>
				<trace label="Fruit" ref="crop/mass[fruit]"/>
				<trace label="Leaf" ref="crop/mass[leaf]"/>
				<trace label="Stem" ref="crop/mass[stem]"/>
				<trace label="Root" ref="crop/mass[root]"/>
				<trace label="Total" ref="crop/mass[total]"/>
			</output>
			<output type="plot">
				<parameter name="title" value="Yield"/>
				<trace label="Days" type="time"/>
				<trace label="Fresh wgt." ref="crop/yield[freshWeight]"/>
			</output>
		</xsl:when>
	</xsl:choose>
	
	<!--
	<output type="table">
		<parameter name="fileName" value="vent_sp.txt"/>
		<trace label="signal" ref="setpoints/temperature/ventilation/adjustments/*/adjustment[signal]"/>
		<trace label="input" ref="setpoints/temperature/ventilation/adjustments/*/adjustment[input]"/>
		<trace label="threshold" ref="setpoints/temperature/ventilation/adjustments/*/adjustment[threshold]"/>
		<trace label="thresholdBand" ref="setpoints/temperature/ventilation/adjustments/*/adjustment[thresholdBand]"/>
		<trace label="increasingSignal" ref="setpoints/temperature/ventilation/adjustments/*/adjustment[increasingSignal]"/>
		<trace label="minSignal" ref="setpoints/temperature/ventilation/adjustments/*/adjustment[minSignal]"/>
		<trace label="maxSignal" ref="setpoints/temperature/ventilation/adjustments/*/adjustment[maxSignal]"/>
	</output>
	<output type="table">
		<parameter name="fileName" value="controllers_ventilation.txt"/>
		<trace label="temperature" ref="indoors/temperature[value]"/>
		<trace label="temp sp" ref="setpoints/temperature/ventilation[signal]"/>
		<trace label="rh" ref="indoors/humidity[rh]"/>
		<trace label="byTemperature" ref="controllers/ventilation/byTemperature[signal]"/>
		<trace label="byHumidity" ref="controllers/ventilation/byHumidity[signal]"/>
		<trace label="leeFactor" ref="controllers/ventilation/leeFactor[signal]"/>
		<trace label="windFactor" ref="controllers/ventilation/windFactor[signal]"/>
		<trace label="coldFactor" ref="controllers/ventilation/coldFactor[signal]"/>
		<trace label="leewardSignal" ref="controllers/ventilation[leewardSignal]"/>
		<trace label="windwardSignal" ref="controllers/ventilation[windwardSignal]"/>
		<trace label="windspeed" ref="outdoors[windspeed]"/>	
	</output>
	<output type="table">
		<parameter name="fileName" value="indoors_radiation.txt"/>
		<trace label="radiIni" value="indoors/radiation[total]"/>
		<trace label="outdoorsDirectRadiation" value="indoors/radiation[outdoorsDirectRadiation]"/>
		<trace label="outdoorsDiffuseRadiation" value="indoors/radiation[outdoorsDiffuseRadiation]"/>
		<trace label="diffuseTransmission" value="indoors/radiation[diffuseTransmission]"/>
		<trace label="directTransmissionAsDirect" value="indoors/radiation[directTransmissionAsDirect]"/>
		<trace label="directTransmissionAsDiffuse" value="indoors/radiation[directTransmissionAsDiffuse]"/>
		<trace label="growthLightShortWaveRadiation" value="indoors/radiation[growthLightShortWaveRadiation]"/>
	</output>
	-->
		
</simulation> </xsl:template>

</xsl:stylesheet>
