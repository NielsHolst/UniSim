
T0 = 273.15
P = 101325 #Pa

# Absolute concentration [g/m3]
ac = function(temp, ppm, vol) {
	R = 8.314 # J/mol/K
	T = temp + T0 # K
	n = P*vol*ppm*10^-6/R/T # mol
	M = 44.01 #g/mol
	n*M
}

ac(25, 380, 1)
