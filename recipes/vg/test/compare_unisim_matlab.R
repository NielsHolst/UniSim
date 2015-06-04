
pairs = list(
			c("Day", "Day"),
			c("cover_temp", "Tcovi"), 
			c("indoors_temp", "Tghi"), 
			c("indoors_rh","RHghi"),
			c("act_vent_supply", "alphaleei"),
			c("sp_heat", "spTheat"),
			c("sp_vent", "spTvent"),
			c("heat_sup", "HeatTransPipei")
		)

Ucols = unlist( llply(pairs, function(x) x[1]) )
Mcols = unlist( llply(pairs, function(x) x[2]) )

n = min(nrow(M),nrow(U))
U2 = U[1:n,Ucols]
M2 = M[1:n,Mcols]
colnames(M2) = colnames(U2)

U2$act_vent_supply = U2$act_vent_supply/20*100

time_plot = function(from=0, to=366) {
	U3 = mutate([U2$Day>=from & U2$Day<=to,], Model=factor("UniSim"))
	M3 = mutate([M2$Day>=from & M2$Day<=to,], Model=factor("Matlab"))
	UM = rbind(U3,M3)
 	UMm = melt(UM, id.vars=c("Day", "Model"))
	ggplot(UMm) +
		geom_line(aes(x=Day, y=value, color=Model)) +
		facet_wrap(~variable, scales="free")
}

corr_plot = function() {
	U3 = melt(U2[,-1])
	M3 = melt(M2[,-1])
	colnames(U3) = c("Variable", "UniSim")
	colnames(M3) = c("Variable", "Matlab")
	UM = cbind(U3, M3$Matlab)
	colnames(UM)[3] = "Matlab"
	ggplot(UM) +
		geom_point(aes(x=UniSim, y=Matlab, color=Variable)) +
		geom_abline(intercept=0, slope=1) +
		facet_wrap(~Variable, scales="free")
}
	
windows(14,10)
time_plot()

# windows(14,10)
# corr_plot()	


