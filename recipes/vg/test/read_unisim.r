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

plot1 = function(U,from=0, to=366, cols=NA) {
	all_columns = any(is.na(cols))
	cols = if (all_columns) 2:(ncol(U)-2) else c("Day", cols)
	V = melt(U[U$Day>=from & U$Day<=to, cols], id.vars="Day", value.name="Value", variable.name="Variable")
	line_size = if (all_columns) 1 else 1.1
	ggp = ggplot(V) +
		  geom_line(aes(x=Day, y=Value, color=Variable), size=line_size) 
	if (all_columns) {
		ggp = ggp +
		theme(legend.position="none") +
		facet_wrap(~Variable, scales="free")
	}
	ggp
}


plot2 = function(U, from, to, cols) {
	cols = c("Day", cols)
	V = melt(U[U$Day>=from & U$Day<=to, cols], id.vars="Day", value.name="Value", variable.name="Variable")
	ggplot(V) +
		geom_line(aes(x=Day, y=Value, color=Variable), size=1.1) +
		theme(legend.position="none") +
		xlab("") + ylab("") +
		facet_wrap(~Variable, ncol=1, scales="free_y")
}

freq1 = function(U, cols) {
	cols = c("Day", cols)
	V = melt(U[, cols], id.vars="Day", value.name="Value", variable.name="Variable")
	ggplot(V) +
		geom_density(aes(x=Value, colour=Variable, fill=Variable), alpha=0.8)
}

plot3 = function(U,from,to) {
	light = plot2(U,from,to, c("outdoors_light", "top_light", "growth_light", "indoors_light")) 
	screen = plot2(U,from,to, c("act_scr_en", "act_scr_sh", "act_scr_bl"))
	light_trans = plot2(U,from,to, c("top_diffuse_trans", "top_dir_dir_trans", "top_dir_diff_trans", "horz_light_trans"))
	temperature = plot2(U,from,to, c("outdoors_temp", "roof1_temp", "side1_temp", "top_temp", "indoors_temp"))
	rh = plot2(U,from,to, c("outdoors_rh", "top_rh", "indoors_rh"))
	ah = plot2(U,from,to, c("outdoors_ah", "top_ah", "indoors_ah"))
	setpoint = plot2(U,from,to, c("sp_heat", "sp_vent", "sp_rh"))
	air = plot2(U,from,to, c("air_infilt", "air_crack", "air_top", "air_bottom"))
	grid.arrange(light, screen, light_trans, temperature, rh, ah, setpoint, air, ncol=4)
}

plot4 = function(U,from,to) {
	top = plot2(U,from,to,
		c("top_energy_total","top_cond","top_air_outdoors1","top_air_outdoors2","top_air_indoors","top_air_conv","top_cover","top_light"))
	bottom1 = plot2(U,from,to,
		c("bottom_energy_total","bottom_cond","bottom_air_outdoors1","bottom_air_outdoors2","bottom_air_indoors","bottom_conv","bottom_cover","bottom_light")) 
	bottom2 = plot2(U,from,to,
		c("bottom_trans","bottom_heat","bottom_floor"))
	grid.arrange(top, bottom1, bottom2, ncol=3)
}


plot5 = function(U) {
	temperature = freq1(U, c("outdoors_temp", "roof1_temp", "side1_temp", "top_temp", "indoors_temp"))
	light = freq1(U, c("outdoors_light", "top_light", "growth_light", "indoors_light")) 
	ah = freq1(U, c("outdoors_ah", "top_ah", "indoors_ah"))
	rh = freq1(U, c("outdoors_rh", "top_rh", "indoors_rh"))
	grid.arrange(temperature, light, ah, rh, ncol=2)
}

U = read_unisim("dvv_unisim_0001.txt")
windows(14,10)
plot5(U)


windows(14,10)
plot3(U,30,36)
windows(14,10)
plot4(U,30,36)

windows(14,10)
plot1(U,1)

windows(14,10)
plot1(U,30,36, c("top_light", "indoors_light")) 

