<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output indent="yes" encoding="ISO-8859-1"/>

<!-- RESOURCES -->

<xsl:variable name="crops" select="document('crops.xml')/Crops"/>
	
<!-- CONFIGURATION -->

<!-- 0: File output for DVV; no progress bar
     1: Screen plot output for testing; show progress bar 
	 2: File output for testing; show progress bar -->
<xsl:variable name="test-mode" select="1"/>  

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
		<xsl:when test="$number=1">whole_roof</xsl:when>
		<xsl:when test="$number=2">flat_roof</xsl:when>
		<xsl:when test="$number=3">roof_side1</xsl:when>
		<xsl:when test="$number=4">roof_side2</xsl:when>
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

<xsl:template name="screen-name">
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
				<xsl:with-param name="number" select="../../Position"/>
			</xsl:call-template>
		</xsl:attribute>
	</parameter>
	<parameter name="layer">
		<xsl:attribute name="value">
			<xsl:call-template name="screen-layer-name">
				<xsl:with-param name="number" select="../../Layer"/>
			</xsl:call-template>
		</xsl:attribute>
	</parameter>
	<parameter name="lightTransmission">
		<xsl:attribute name="value">
			<xsl:value-of select="../Parameters[ParameterID='30']/Value div 100"/>
		</xsl:attribute>
	</parameter>
	<parameter name="airTransmission">
		<xsl:attribute name="value">
			<xsl:value-of select="../Parameters[ParameterID='32']/Value div 100"/>
		</xsl:attribute>
	</parameter>
	<parameter name="energyLossReduction">
		<xsl:attribute name="value">
			<xsl:value-of select="../Parameters[ParameterID='33']/Value div 100"/>
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
		<parameter name="beginDate">
			<xsl:attribute name="value">
				<xsl:value-of select="concat(2001, substring($startTime, 5, 6))"/>
			</xsl:attribute>
		</parameter>
		<parameter name="beginTime">
			<xsl:attribute name="value">
				<xsl:value-of select="substring($startTime, 12, 5)"/>
			</xsl:attribute>
		</parameter>
		<parameter name="endDate">
			<xsl:attribute name="value">
				<xsl:value-of select="concat(2001 + $incYear, substring($stopTime, 5, 6))"/>
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
		<parameter name="sample">
			<xsl:choose>
				<xsl:when test="string-length($simTimeStep)=0 or string-length($simTimeStep)=0">
					<xsl:attribute name="value" select="12"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="value" select="$outTimeStep div $simTimeStep"/>
				</xsl:otherwise>
			</xsl:choose>
		</parameter>
		<parameter name="latitude">
			<xsl:attribute name="value" select="JobDataSet/Latitude"/>
		</parameter>
	</model>

	<model name="outdoors" type="vg::Outdoors">
		<model name="records" type="UniSim::Records">
			<parameter name="fileName" value="sel_dk.txt"/>
			<parameter name="imposeInitialDateTime" value="false"/>
		</model>
	</model>

	<model name="greenhouse">
		<model name="construction">
			<model name="ventilation" type="vg::ConstructionVentilation">
				<parameter name="ventMaxOpening">
					<xsl:attribute name="value">
						<xsl:value-of select="min(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='52']/Value)"/>
					</xsl:attribute>
				</parameter>
				<parameter name="ventLength">
					<xsl:attribute name="value">
						<xsl:value-of select="avg(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='456']/Value)"/>
					</xsl:attribute>
				</parameter>
				<parameter name="ventWidth">
					<xsl:attribute name="value">
						<xsl:value-of select="avg(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='457']/Value)"/>
					</xsl:attribute>
				</parameter>
				<parameter name="ventDensity">
					<xsl:attribute name="value">
						<xsl:value-of select="sum(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='49']/Value) div 
						                    $greenhouse-area"/>
					</xsl:attribute>
				</parameter>
				<parameter name="efficacy">
					<xsl:attribute name="value">
						<xsl:value-of select="avg(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='51']/Value) div
											  100"/>
					</xsl:attribute>
				</parameter>
				<!--Todo: vents/vent parameters -> ventilationController -->
			</model>
			<model name="growthLights">
				<xsl:for-each select="JobDataSet/Greenhouse/zone/Lamps/Lamp">
					<model type="vg::ConstructionGrowthLight">
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
						<parameter name="capacity">
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
			</model>
			
			<xsl:for-each select="JobDataSet/Greenhouse/zone/Panes//Pane">
				<xsl:choose>
					<xsl:when test="Position &lt; 7">
						<model type="vg::Cover">
							<xsl:attribute name="name">
								<xsl:call-template name="cover-position-name">
									<xsl:with-param name="number" select="Position"/>
								</xsl:call-template>
							</xsl:attribute>
							<parameter name="type" value="User-defined"/>
							<parameter name="diffuseTransmission">
								<xsl:attribute name="value">
									<xsl:value-of select="Constants/Parameters[ParameterID='308']/Value div 100"/>
								</xsl:attribute>
							</parameter>
							<parameter name="U">
								<xsl:attribute name="value">
									<xsl:value-of select="Constants/Parameters[ParameterID='310']/Value"/>
								</xsl:attribute>
							</parameter>
							<parameter name="haze" value="0"/>
							<parameter name="antiReflection" value="no"/>
						</model>
					</xsl:when>
					<xsl:otherwise>
						<model name="floor" type="vg::ConstructionFloor">
							<parameter name="U">
								<xsl:attribute name="value">
									<xsl:value-of select="Constants/Parameters[ParameterID='310']/Value"/>
								</xsl:attribute>
							</parameter>
						</model>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>					
		</model>
	</model>

	<model name="indoors">
		<model name="floor">
			<model name="temperature" type="vg::FloorTemperature">
				<parameter name="initValue" value="17"/>
			</model>
		</model>		

		<model name="cover"> 
			<model name="transmission" type="vg::TransmissionCover"/> 
			<model name="temperature" type="vg::CoverTemperature"/> 
		</model>

		<model name="screens"> 
			<model name="transmission" type="vg::TransmissionScreens"/> 
			<model name="temperature" type="vg::ScreenTemperature"/> 
		</model>
		
		<model name="radiation" type="vg::IndoorsRadiation"/> 
		
		<model name="temperature" type="vg::IndoorsTemperature"/> 

		<model name="ventilation" type="vg::IndoorsVentilation">
			<model name="infiltration" type="vg::AirInfiltration">
				<parameter name="leakage" value="1"/>
			</model>
			<model name="vents" type="vg::VentsVentilation"/> 
		</model>		

		<model name="humidity" type="vg::IndoorsHumidity">
			<model name="transpiration">
				<model name="crop" type="vg::CropTranspiration"/>
			</model>
			<model name="evaporation">
			</model>
			<model name="condensation">
				<model name="cover" type="vg::CoverCondensation"/> 
				<model name="screens" type="vg::ScreenCondensation"/> 
				<!-- Affugtere her! -->
			</model>
			<model name="ventilation">
				<model name="total" type="vg::VentilationVapour"/>
			</model>
		</model>

		<model name="energy" type="vg::EnergyBalance"> 
			<model name="surface" type="vg::EnergyFluxSurface"/> 
			<model name="ventilation" type="vg::EnergyFluxVentilation"/> 
			<model name="floor" type="vg::EnergyFluxFloor"/> 
			<model name="transpiration" type="vg::EnergyFluxTranspiration"/> 
			<model name="condensation" type="vg::EnergyFluxCondensation"/> 
		</model>		

		<model name="co2" type="vg::IndoorsCo2"/>
	</model>

	<model name="setpoints">
		<model name="humidity">
			<!--
			<model name="minimumRh" type="vg::SwitchSetpointCollection">
				<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='15']/SetpointTimes//SetpointTime">
					<model name="setpoint" type="vg::SwitchSetpoint">
						<parameter name="setpoint" >
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
			-->
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
			<model name="ventilation" type="vg::SignalCollection">
				<parameter name="rule" value="sum"/>
				<parameter name="signalReset" value="20"/>
				
				<model name="setpoints" type="vg::SignalCollection">
					<parameter name="rule" value="min"/>
					<parameter name="signalReset" ref="..[signalReset]"/>
					<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='3']/SetpointTimes//SetpointTime">
						<model type="vg::DateTimeSignal">
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
									<xsl:attribute name="value">
										<xsl:value-of select="max(//Vents/Vent/Constants/Parameters[ParameterID='492']/Value)"/>
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
		
			<model name="heating" type="vg::SignalCollection">
				<parameter name="rule" value="sum"/>
				<parameter name="signalReset" value="18"/>
				
				<model name="setpoints" type="vg::SignalCollection">
					<parameter name="rule" value="max"/>
					<parameter name="signalReset" ref="..[signalReset]"/>
					<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='2']/SetpointTimes//SetpointTime">
					
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
				
				<model name="adjustments" type="vg::SignalCollection">
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
								<parameter name="thresholdBand">
									<xsl:attribute name="value">
										<xsl:value-of select="max(//Vents/Vent/Constants/Parameters[ParameterID='492']/Value)"/>
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
			<model name="maxDrawn" type="vg::ThresholdSignal"> <!-- When too humid, do not draw screens completely -->
				<parameter name="input" ref ="indoors/humidity[rh]"/>
				<parameter name="threshold" ref ="setpoints/humidity/maximumRh[signal]"/>
				<parameter name="signalBelow" value ="1"/>
				<parameter name="signalAbove">
					<xsl:attribute name="value">
						<xsl:value-of select="1 - max(//Screens/Screen/Constants/Parameters[ParameterID='522']/Value)"/>
					</xsl:attribute>
				</parameter>
			</model>

			<model name="energy">
				<!-- Tilføj energibalancemodel -->
				<model name="light" type="vg::ThresholdSignal">
					<parameter name="input" ref ="outdoors[radiation]"/>
					<parameter name="threshold" value ="10"/>
					<parameter name="signalBelow" ref ="../../maxDrawn[signal]"/>
					<parameter name="signalAbove" value ="0"/>
				</model>
				<!-- Separat controller for hvert eneste gardin for dets periode:
					Se InactiveFromDayInYear og InactiveToDayInYear
				-->
				<model name="control" type="vg::SignalCollection">
					<parameter name="rule" value="max"/>
					<parameter name="signalReset" value ="0"/>
					<model type="vg::SignalCollection">
						<parameter name="rule" value="max"/>
						<parameter name="signalReset" value ="0"/>
						<model type="vg::DateTimeSignal">
							<parameter name="beginDay" value="1"/>
							<parameter name="endDay" value="365"/>
							<parameter name="beginTime" value="0:0"/>
							<parameter name="endTime" value="0:0"/>
							<parameter name="signalInside" ref ="../../../light[signal]"/>
							<parameter name="signalOutside" value="0"/>
							<parameter name="signalOutsideTimeOnly" value="0"/>
						</model>
					</model>
				</model>
			</model>
			
			<model name="shade">
				<model name="light" type="vg::ThresholdSignal">
					<parameter name="input" ref ="outdoors[radiation]"/>
					<parameter name="threshold" value ="550"/>
					<parameter name="signalBelow" value ="0"/>
					<parameter name="signalAbove" ref ="../../maxDrawn[signal]"/>
				</model>
				<model name="control" type="vg::SignalCollection">
					<parameter name="rule" value="max"/>
					<parameter name="signalReset" value ="0"/>
					<model type="vg::SignalCollection">
						<parameter name="rule" value="max"/>
						<parameter name="signalReset" value ="0"/>
				<!-- Separat controller for hvert eneste gardin for dets periode:
					Se InactiveFromDayInYear og InactiveToDayInYear
				-->
						<model type="vg::DateTimeSignal">
							<parameter name="beginDay" value="1"/>
							<parameter name="endDay" value="365"/>
							<parameter name="beginTime" value="0:0"/>
							<parameter name="endTime" value="0:0"/>
							<parameter name="signalInside" ref ="../../../light[signal]"/>
							<parameter name="signalOutside" value="0"/>
							<parameter name="signalOutsideTimeOnly" value="0"/>
						</model>
					</model>
					<model type="vg::FixedSignal">
						<parameter name="input" ref="../../../energy/control[signal]"/>
					</model>
				</model>
			</model>

			<model name="blackout">
				<model name="light" type="vg::ThresholdSignal">
					<parameter name="input" ref="outdoors[radiation]"/>
					<parameter name="threshold" value="0"/>
					<parameter name="signalBelow" value="0"/>
					<parameter name="signalAbove" value="1"/>
				</model>
				<model name="control" type="vg::SignalCollection">
					<parameter name="rule" value="max"/>
					<parameter name="signalReset" value="0"/>
					<model type="vg::SignalCollection">
						<parameter name="rule" value="max"/>
						<parameter name="signalReset" value="0"/>
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
					<model type="vg::FixedSignal">
						<parameter name="input" ref="../../../energy/control[signal]"/>
					</model>
				</model>
			</model>
		</model>

		<model name="ventilation" type="vg::VentilationController">
			<model name="byTemperature" type="vg::ProportionalSignal">
				<parameter name="input" ref="indoors/temperature[value]"/>
				<parameter name="threshold" ref="setpoints/temperature/ventilation[signal]"/>
				<parameter name="thresholdBand" value="2"/>
				<parameter name="increasingSignal" value="true"/>
				<parameter name="maxSignal" value="100"/>
				<parameter name="minSignal" value="0"/>
			</model>
			<model name="byHumidity" type="vg::ProportionalSignal">
				<parameter name="input" ref="indoors/humidity[rh]"/>
				<parameter name="threshold" ref="setpoints/humidity/maximumRh[signal]"/>
				<parameter name="thresholdBand" value="5"/>
				<parameter name="increasingSignal" value="true"/>
				<parameter name="maxSignal" value="10"/>
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
				<parameter name="threshold" value ="-2"/>
				<parameter name="thresholdBand" value="2"/>
				<parameter name="increasingSignal" value="true"/>
				<parameter name="maxSignal" value="1"/>
				<parameter name="minSignal" value="0"/>
			</model>
		</model>

		<model name="heating">
			<model name="temperature" type="vg::HeatingTemperatureController">
				<!-- For hvert rør:
				Se CommonFlowTemperature og CommonReturnTemperature
				-->
				<parameter name="minimumSignal" value ="20"/>
				<parameter name="maximumSignal" value ="90"/>
			</model>
		</model>

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
			<xsl:for-each select="JobDataSet/Greenhouse/zone/Screens//Screen/Constants/Parameters[ParameterID='573']">
				<xsl:choose>
					<xsl:when test='Value=1'>
						<model name="shade" type="vg::Screen">
							<xsl:call-template name="extract-screen"/>
							<model name="control" type="vg::LinearControlElement">
								<parameter name="signal" ref="controllers/screens/shade/control[signal]"/>
								<parameter name="rate"  value="0.1"/>
							</model>
						</model>
					</xsl:when>
					<xsl:when test='Value=2'>
						<model name="energy" type="vg::Screen">
							<xsl:call-template name="extract-screen"/>
							<model name="control" type="vg::LinearControlElement">
								<parameter name="signal" ref="controllers/screens/energy/control[signal]"/>
								<parameter name="rate"  value="0.1"/>
							</model>
						</model>
					</xsl:when>
					<xsl:when test='Value=3'>
						<model name="blackout" type="vg::Screen">
							<xsl:call-template name="extract-screen"/>
							<model name="control" type="vg::LinearControlElement">
								<parameter name="signal" ref="controllers/screens/blackout/control[signal]"/>
								<parameter name="rate"  value="0.1"/>
							</model>
						</model>
					</xsl:when>
				</xsl:choose>
			</xsl:for-each>					
		</model>
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
						<parameter name="flowRate" value="20"/>
						<parameter name="inflowTemperature" ref="controllers/heating/temperature[signal]"/>
					</model>
				</xsl:for-each>
			</model>
		</model>
		<model name="growthLights" type="vg::GrowthLights">
			<xsl:for-each select="JobDataSet/Greenhouse/zone/Lamps/Lamp">
				<xsl:variable name="light-info" select="concat('construction/growthLights/growthLight', position())"/>  
				<model type="vg::GrowthLight">
					<xsl:attribute name="name">
						<xsl:value-of select="concat('growthLight', position())"/>
					</xsl:attribute>
					<parameter name="heatEmissionOn">
						<xsl:attribute name="ref">
							<xsl:value-of select="concat($light-info, '[heatEmission]')"/>
						</xsl:attribute>
					</parameter>
					<parameter name="longWaveEmissionOn">
						<xsl:attribute name="ref">
							<xsl:value-of select="concat($light-info, '[longWaveEmission]')"/>
						</xsl:attribute>
					</parameter>
					<parameter name="shortWaveEmissionOn">
						<xsl:attribute name="ref">
							<xsl:value-of select="concat($light-info, '[shortWaveEmission]')"/>
						</xsl:attribute>
					</parameter>
					<parameter name="parEmissionOn">
						<xsl:attribute name="ref">
							<xsl:value-of select="concat($light-info, '[parEmission]')"/>
						</xsl:attribute>
					</parameter>
					<parameter name="energyUseOn">
						<xsl:attribute name="ref">
							<xsl:value-of select="concat($light-info, '[energyUse]')"/>
						</xsl:attribute>
					</parameter>
					<parameter name="age">
						<xsl:attribute name="ref">
							<xsl:value-of select="concat($light-info, '[age]')"/>
						</xsl:attribute>
					</parameter>
					<parameter name="lifeTime">
						<xsl:attribute name="ref">
							<xsl:value-of select="concat($light-info, '[lifeTime]')"/>
						</xsl:attribute>
					</parameter>
					<parameter name="minPeriod">
						<xsl:attribute name="ref">
							<xsl:value-of select="concat($light-info, '[minPeriod]')"/>
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
		<xsl:when test="$test-mode=0">
			<model name="fixed" type="UniSim::Fixed">
				<parameter name="parameters"  
					value="
				(
				(HeatEnergy 99.)
				(EnergyUseLight 99.)
				(EffectAgam 99.)
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
				(H2O_dehum 99.)
				(Dew 99.)
				(CO2supply 99.)
				)"/>
			</model>
			
			<output name="output" type="table">
				<parameter name="fileName" value="dvv_unisim_0001.txt"/>
				<trace label="HeatEnergyi" value="actuators/heating/pipes[sumEnergy]"/>
				<trace label="EnergyUseLight" value="fixed[EnergyUseLight]"/>
				<trace label="ElUseDehumidifieri" value="fixed[EffectAgam]"/>
				<trace label="Tghi" value="indoors/temperature[value]"/>
				<trace label="Tgh_naturali" value="fixed[Tgh_natural]"/>
				<trace label="RHghi" value="indoors/humidity[rh]"/>
				<trace label="CO2airi" value="fixed[CO2air]"/>
				<trace label="Tcovi" value="indoors/cover/temperature[value]"/>
				<trace label="Tscreeni" value="indoors/screens/temperature[value]"/>
				<trace label="TrLighti" value="cover/transmission[light]"/>
				<trace label="VentVindui" value="indoors/ventilation[absolute]"/>
				<trace label="Leakage_heati" value="fixed[Leakage_heat]"/>
				<trace label="Leakage_humi" value="fixed[Leakage_hum]"/>
				<trace label="alphawindi" value="actuators/vents/windside[state]"/>
				<trace label="alphaleei" value="actuators/vents/leeside[state]"/>
				<trace label="DWyield" value="fixed[DWyield]"/>
				<trace label="FWyield" value="fixed[FWyield]"/>
				<trace label="NumberPlantsi" value="fixed[NumberPlants]"/>
				<trace label="Tcropi" value="crop/temperature[value]"/>
				<trace label="VPDcrop_airi" value="fixed[VPDcrop_air]"/>
				<trace label="LEi" value="fixed[LE]"/>
				<trace label="Rni" value="fixed[Rn]"/>
				<trace label="Bni" value="fixed[Bn]"/>
				<trace label="Pnci" value="fixed[Pnc]"/>
				<trace label="radiIni" value="indoors/radiation[total]"/>
				<trace label="Tpipe1i" value="heating/pipes/pipe1[temperature]"/>
				<trace label="Tpipe2i" value="heating/pipes/pipe2[temperature]"/>
				<trace label="spTheat" value="setpoints/temperature/heating[signal]"/>
				<trace label="spTvent" value="setpoints/temperature/ventilation[signal]"/>
				<trace label="spRHghi" value="setpoints/humidity/maximumRh[signal]"/>
				<trace label="H2O_dehumi" value="fixed[H2O_dehum]"/>
				<trace label="Dewi" value="fixed[Dew]"/>
				<trace label="CO2supplyi" value="fixed[CO2supply]"/>
			</output>
	
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
				<trace label="Infiltration" ref="indoors/ventilation/infiltration[rate]"/>
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
				<trace label="Pipes" ref="indoors/energy[heatPipes]"/>
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
				<trace label="Heating sp" ref="setpoints/temperature/heating[setpoint]"/>	
				<trace label="Vent sp" ref="setpoints/temperature/ventilation[setpoint]"/>	
			</output>
			<output type="plot">
				<parameter name="title" value ="Temperature sp"/>
				<trace label ="Days" type="time"/>
				<trace label="Indoors" ref="indoors/temperature[value]"/>
				<trace label="Sp heat" ref="setpoints/temperature/heating[signal]"/>
				<trace label="Sp vent" ref="setpoints/temperature/ventilation[signal]"/>
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
	

</simulation> </xsl:template>

</xsl:stylesheet>
