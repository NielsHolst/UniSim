### Prepare R environment
rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)
library(gridExtra)
library(plyr)
library(reshape2)

read_unisim = function(fname) { 
	folder = "C:/data/QDev/UniSim/src/applications/output"
	S = read.table(paste(folder,fname,sep="/"), header=TRUE)
	S = mutate(S, Day=(date-2001)*365, DayNumber=floor(Day), Hour=floor(24*Day))
	S
}

check = function(U,x) {
	missing_cols = setdiff(x, intersect(x, colnames(U)))
	if (length(missing_cols>0)) {
		msg = paste("Missing columns:", paste(missing_cols, collapse=" "))
		stop(msg)
	} 
}

plot1 = function(U, from, to, cols) {
	cols = c("Day", cols)
	check(U,cols)
	V = melt(U[U$Day>=from & U$Day<=to, cols], id.vars="Day", value.name="Value", variable.name="Variable")
	ggplot(V) +
		geom_line(aes(x=Day, y=Value, color=Variable))
}

plot2 = function(U, from, to, cols) {
	cols = c("Day", cols)
	check(U,cols)
	V = melt(U[U$Day>=from & U$Day<=to, cols], id.vars="Day", value.name="Value", variable.name="Variable")
	ggplot(V) +
		geom_line(aes(x=Day, y=Value, color=Variable), size=1.1) +
		theme(legend.position="none") +
		xlab("") + ylab("") +
		facet_wrap(~Variable, ncol=1, scales="free_y")
}

climate = function(U,from=0, to=366) {
	windows(14,10)
	grid.arrange( 
		plot2(U, from, to, c("outdoorsWindspeed", "indoorsWindspeed", "indoorsCo2")),
		plot2(U, from, to, c("outdoorsRh", "indoorsRh", "outdoorsAh", "indoorsAh")),
		plot2(U, from, to, c("indoorsTemperature", "coverTemperature", "screenTemperature", "floorTemperature")),
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "Temperature")),
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "LightAbsorbed")),
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "GrowthLightLwAbsorbed")),
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "FloorLwAbsorbed")),
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "ShelterLoss")),
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "HeatingAbsorbed")),
		ncol=5
	)	
}

controlled = function(U,from=0, to=366) {
	windows(14,10)
	grid.arrange( 
		plot2(U, from, to, c("airInfiltration", "airHumCrack", "airGravity", "airGiven")),
		plot2(U, from, to, c("airCoolingMax", "airCoolingMaxByWind", "airCoolingMaxByTemp")),
		plot2(U, from, to, c("energyCoolingDemand", "energyCoolingSupply", "airCoolingSupply", "airCoolingSupplySlope")),
		plot2(U, from, to, c("spHeating", "spVentilation", "spRh", "heatingSupply")),
		plot2(U, from, to, c("indoorsTemperature", "indoorsRh", "indoorsCo2", "airFluxTotal")),
		plot2(U, from, to, c("pipe1Temperature", "pipe2Temperature")),
		ncol=3
	)	
}

check_pipes = function(U,from=0, to=366) {
	windows(14,10)
	grid.arrange( 
		plot1(U, from, to, c("pipe1NextTempMin", "pipe1Temperature", "pipe1NextTempMax")),
		plot1(U, from, to, c("pipe2NextTempMin", "pipe2Temperature", "pipe2NextTempMax")),
		ncol=1
	)
}

production = function(U,from=0, to=366) {
	windows(14,10)
	grid.arrange( 
		plot2(U, from, to, c("heatingEnergyFlux", "heatingEnergyTotal", "indoorsTemperature")),
		plot2(U, from, to, c("growthLightsEnergyFlux", "growthLightsEnergyTotal", "growthLightsPar")),
		plot2(U, from, to, c("co2Flux", "co2Total", "indoorsCo2")),
		plot2(U, from, to, c("assimilation", "fruitGrowthRate", "yieldFreshWeight")),
		ncol=2
	)	
}

photosynthesis = function(U,from=0, to=366) {
	windows(14,10)
	grid.arrange( 
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "_Pn")),
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "_Pg")),
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "_rb_H2o")), 
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "_rb_Co2")), 
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "_rs_H2o")), 
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "_rs_Co2")), 
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "_LUE")), 
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "_Pnmax")),
		plot2(U, from, to, paste0(c("top", "mid", "bot"), "_Pgmax")),
		plot2(U, from, to, c("indoorsTemperature", "indoorsRh", "indoorsCo2", "airFluxTotal")),
		ncol=5
	)	
}
 
U = read_unisim("dvv_unisim_0001.txt")
climate(U)
controlled(U)
production(U)
photosynthesis(U)
# hist(U$pipe1NextTempMax-)

# windows(14,10)
# grid.arrange( 
	# ggplot(data=data.frame(cbind(U0=U0$topTemperature, U=U$topTemperature)), aes(x=U0,y=U)) + geom_point() + geom_abline(size=1.5, color="red"),
	# ggplot(data=data.frame(cbind(U0=U0$midTemperature, U=U$midTemperature)), aes(x=U0,y=U)) + geom_point() + geom_abline(size=1.5, color="red"),
	# ggplot(data=data.frame(cbind(U0=U0$botTemperature, U=U$botTemperature)), aes(x=U0,y=U)) + geom_point() + geom_abline(size=1.5, color="red"),
	# ncol=2
# )	



 