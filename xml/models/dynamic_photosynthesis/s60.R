a=0.3
b = 0.005
c = 0.002
gSini = 50

S60 = function(PAR) {
	a*(1. - exp(-b*PAR))*((1. - exp(-c*gSini)))
}

PAR = 1:20

plot(PAR, S60(PAR))

