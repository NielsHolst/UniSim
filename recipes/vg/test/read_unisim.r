### Prepare R environment
rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)
library(plyr)
library(reshape2)

read_unisim = function(fname) { 
	folder = "C:/data/QDev/UniSim/src/applications/output"
	S = read.table(paste(folder,fname,sep="/"), header=TRUE)
	S = mutate(S, Day=(date-2001)*365, DayNumber=floor(Day), Hour=floor(24*Day))
	S
}

plot1 = function(U,from=0, to=366, cols=NA) {
	all_columns = any(is.na(cols))
	cols = if (all_columns) 2:(ncol(U)-2) else c("Day", cols)
	V = melt(U[U$Day>=from & U$Day<=to, cols], id.vars="Day")
	line_size = if (all_columns) 1 else 2
	ggp = ggplot(V) +
		  geom_line(aes(x=Day, y=value, color=variable), size=line_size)
	if (all_columns) {
		ggp = ggp +
		theme(legend.position="none") +
		facet_wrap(~variable, scales="free")
	}
	ggp
}

plot1a = function(U,from=0, to=366, cols) {
	V = melt(U[U$Day>=from & U$Day<=to, c(cols, "Day")], id.vars="Day")
	ggplot(V) +
		geom_line(aes(x=Day, y=value, color=variable))
}

plot2 = function(U1,U2,from=0, to=366, cols=NA) {
	cols = if (any(is.na(cols))) 2:(ncol(U)-2) else c("Day", cols)
	V1 = mutate(U1[U1$Day>=from & U1$Day<=to, cols], Scenario=factor("Scen-1"))
	V2 = mutate(U2[U2$Day>=from & U2$Day<=to, cols], Scenario=factor("Scen-2"))
	V12 = rbind(V1,V2)
 	W = melt(V12, id.vars=c("Day", "Scenario"))
	ggplot(W) +
		geom_line(aes(x=Day, y=value, color=Scenario)) +
		facet_wrap(~variable, scales="free")

}

U = read_unisim("dvv_unisim_0001.txt")
windows(14,10)
plot1(U,1)

windows(14,10)
plot1(U,10,15, c("outdoors_temp", "cover_temp", "top_temp", "indoors_temp"))

# windows(14,10)
# plot1(U,10,15, c("outdoors_light", "top_light", "indoors_light", "growth_light", "act_scr_en"))


# windows(14,10)
# plot1a(U,182.6,182.8, c("indoors_temp", "outdoors_temp"))

# Uh = ddply(U, .(Hour), colMeans)
# windows(14,10)
# plot1(Uh,1)

# Ud = ddply(U, .(DayNumber), colMeans)
# U2 = read_unisim("dvv_unisim_0001_0006.txt")
# U2 = mutate(U2, Day=Day/30, DayNumber=floor(Day), Hour=floor(24*Day))
# U2d = ddply(U2, .(DayNumber), colMeans)
# U2h = ddply(U2, .(Hour), colMeans)
# windows(14,10)
# plot2(Uh,U2h)
# windows(14,10)
# plot2(Ud,U2d)


