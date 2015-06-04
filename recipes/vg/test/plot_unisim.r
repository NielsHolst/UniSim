### Prepare R environment
rm(list=ls(all=TRUE))
graphics.off()
library(fields)
library(ggplot2)
library(plyr)
library(reshape2)
library(RColorBrewer)
library(R.matlab)
setwd("C:/data/QDev/UniSim/recipes/vg/test")  # Change this according to your working folder

reorder_levels = function(x, new_order) {
	factor(x,levels(x)[new_order])
}

read_matlab = function(test_num) {
	col_df = function(i) {
		y = M[i,1,1]
		as.data.frame(y)
	}
	M = readMat(paste0("output/OKO", test_num, ".dat"))
	output_ix = length(M)
	M = M[[output_ix]]
	n = dim(M)[1]
	Mw = data.frame(Day=(1:(365*24))/24, Scenario=factor(test_num))
	for (i in 1:n) Mw = cbind(Mw, col_df(i))
	Mw = mutate(Mw, Day=as.numeric(rownames(Mw))/24)
	Mw
}

read_unisim = function(fname) { 
	folder = "C:/data/QDev/UniSim/src/applications/output"
	S = read.table(paste(folder,fname,sep="/"), header=TRUE)
	S = mutate(S, Day=as.numeric(rownames(S))/24)
	S
}

add_season = function(X) {
	Y = mutate(X, Month=factor(pmin(Day%/%30,11)+1), Season=Month)
	levels(Y$Season) = c(rep("Winter",2), rep("Spring",3), rep("Summer",3), rep("Autumn",3), "Winter")
	Y$Season = reorder_levels(Y$Season, c(2,3,4,1))
	Y
}

melt_both = function(df1, df2, name1, name2, cols=NA) {
	the_rows = 1:min(nrow(df1), nrow(df2))
	the_cols1 = intersect(colnames(df1), colnames(df2))
	the_cols2 = the_cols1
	if (!any(is.na(cols))) {
		the_cols1 = intersect(colnames(df1), cols)
		the_cols2 = intersect(colnames(df2), cols)
	}
	the_cols1 = c("Day", the_cols1)
	the_cols2 = c("Day", the_cols2)
	df1 = df1[the_rows, the_cols1]
	df2 = df2[the_rows, the_cols2]
	df1m = melt(df1, id.vars="Day")
	df2m = melt(df2, id.vars="Day")
	# df12m = cbind(df1m,df2m)[,c(-4,-5)]
	# df12m = join(df1m, df2m, by=c("Day","variable"), type="right")
	df12m = merge(df1m, df2m, by=c("Day","variable"), all=TRUE)
	
	colnames(df12m) = c("Day", "Variable", name1, name2)
	df12m = add_season(df12m)
	df12m
}

comp_1to1 = function(df, cols=c(3,4)) {
	x_name = colnames(df)[cols[1]]
	y_name = colnames(df)[cols[2]]
	ggplot(df[complete.cases(df),], aes_string(x=x_name, y=y_name, color="Season")) +
		geom_abline(intercept=0, slope=1, color="darkgrey", size=2) +
		geom_point(size=1.3) +
		scale_colour_manual(values=brewer.pal(5, "YlOrRd")[2:5]) +
		facet_wrap(~Variable, scales="free")
}

comp_trend = function(df, cols=c(3,4)) {
	dfm = melt(df, measure.vars=colnames(df)[cols], variable.name="Model", value.name="Value")
	ggplot(dfm, aes(x=Day, y=Value, color=Model)) +
		geom_line() +
		# scale_colour_manual(values=brewer.pal(5, "YlOrRd")[2:5]) +
		facet_wrap(~Variable, scales="free")
}

comp_corr = function(df, pairs, cols=c(3,4)) {
	name1 = colnames(df)[cols[1]]
	name2 = colnames(df)[cols[2]]
	df1 = df[,c("Day","Variable", name1)]
	df2 = df[,c("Day","Variable", name2)]
	colnames(df1)[3] = "Value"
	colnames(df2)[3] = "Value"
	df1$Scenario = name1
	df2$Scenario = name2
	df12 = rbind(df1,df2)
	pairs = list(c("alphaleei","VentVindui"), c("Tghi","VentVindui"))

	create_pair_df = function(i) {
		p = pairs[[i]]
		co = c("Day", "Scenario", "Value")
		dfp1 = df12[df12$Variable==p[1],co]
		colnames(dfp1)[3] = p[1]
		dfp2 = df12[df12$Variable==p[2],co]
		colnames(dfp2)[3] = p[2]
		dfpair = cbind(dfp1, dfp2[,3])
		colnames(dfpair)[4] = p[2]
		dfpair$PlotNumber = i
		dfpair
	}	
	n = length(pairs)
	df_pairs = lapply(1:n, create_pair_df)
	
	ggp = ggplot()
	for (i in 1:n) {
		x_name = colnames(df_pairs[[i]])[cols[1]]
		y_name = colnames(df_pairs[[i]])[cols[2]]
		ggp = ggp + geom_point(data=df_pairs[[i]],aes_string(x=x_name, y=y_name,colour="Scenario"))
	}
	ggp + facet_wrap(Scenario~PlotNumber, scales="free")
}

columns = c("radiIni","spTheat", "spTvent", "Tghi", "Tcovi", "Tcropi", "RHghi", "VentVindui", "alphaleei", "alphawindi", 
	"outdoorsDirectRadiation", "outdoorsDiffuseRadiation", "growthLights",
	"RadiationRatio", "DWyieldi", "Pghi", "Pnci.1", "Pnci.2","Pnci.3","CO2airi", "Pgc", "Tpipe1i", "Tpipe2i",
	"cropConductance", "cropVapourFlux", "ventConductance", "ventVapourFlux", "condConductance", "condVapourFlux"
)
# columns = c("RatioLightTransDifi", "RatioLightTransDiri")
columns = c("Tghi", "RHghi", "spTheat", "spTvent", 
	"pass_vent", "pass_vap_trans", "pass_vap_evap" , "pass_vap_cond", "pass_vap_vent" )

# M = read_matlab(1)
U = read_unisim("dvv_unisim_0001.txt")
U = mutate(U, RadiationRatio=radiIni/(outdoorsDirectRadiation+outdoorsDiffuseRadiation))

# U2 = read_unisim("dvv_unisim_0001_flowrate=2.txt")
# UU = melt_both(U1,U2,"Default","Flowrate2",columns)

# colnames(U)[39] = "RatioLightTransDifi"
# colnames(U)[40] = "RatioLightTransDiri"
U = mutate(U, Tdiff=Tghi-spTheat)

# M$RatioLightTransDifi = 1 - M$RatioLightTransDifi
# M$RatioLightTransDiri = 1 - M$RatioLightTransDiri
plot_U = function() {
	V = melt(mutate(U, Day=Day/30), id.vars="Day")
	ggplot(V) +
		geom_line(aes(x=Day, y=value, color=variable)) +
		facet_wrap(~variable, scales="free")
}

windows(14,10)
plot_U(U, columns)

# MU = melt_both(M,U,"Matlab","Unisim",columns)

# windows(14,10)
# comp_trend(MU)

# windows(14,10)

# co = c("Tsky", "Toutdoors", "Tcovi", "Tcropi.1", "Tghi", "spTheat", "spTvent")
# Um = melt(U, id.vars="Day")
# ggplot(Um[Um$variable %in% co,], aes(x=Day, y=value, colour=variable)) +
	# geom_line(size=2)
