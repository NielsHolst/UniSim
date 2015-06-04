### Prepare R environment
rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)



f1 = function(GLOBRAD) {
	spDif = 1
	y = spDif*(1-exp((-0.01*GLOBRAD)/spDif))
	y[GLOBRAD<300] = 0
	y
}

f2 = function(GLOBRAD) {
	spDif = 1
	Pband = 100
	dRad = 3*(GLOBRAD - 300)
	y = spDif*(1-exp((-dRad/Pband)/spDif))
	y[dRad<0] = 0
	y
}

g = function(RHgh) {
	maxTHeatRH = 1
	Pband = 5
	dRH = 1.5*(RHgh - 80)
	y = maxTHeatRH*(1 - exp(-dRH/Pband/maxTHeatRH))
	y[dRH<0] = 0
	y
}

GLOBRAD=0:500
qplot(GLOBRAD, f2(GLOBRAD), geom="line")

windows()
RHgh = 60:100
qplot(RHgh, g(RHgh), geom="line")

