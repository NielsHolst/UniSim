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

plot1 = function(U, from, to, cols) {
	cols = c("Day", cols)
	V = melt(U[U$Day>=from & U$Day<=to, cols], id.vars="Day", value.name="Value", variable.name="Variable")
	ggplot(V) +
		geom_line(aes(x=Day, y=Value, color=Variable))
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

plot3 = function(U,from=0, to=366) {
	light = plot2(U,from,to, c("outdoors_light", "growth_light", "indoors_light")) 
	light2 = plot2(U,from,to, c("roof1_light", "roof2_light" ,"side1_light","side2_light","end1_light", "end2_light")) 
	screen = plot2(U,from,to, c("act_scr_en", "act_scr_sh", "act_scr_bl"))
	air = plot2(U,from,to, c("air_infilt", "air_crack", "air_top", "air_bottom"))
	grid.arrange(light, light2, screen, air, ncol=2)
}

plot4 = function(U,from=0, to=366) {
	screen = plot2(U,from,to, c("act_scr_en", "act_scr_sh", "act_scr_bl"))
	temperature = plot2(U,from,to, c("outdoors_temp", "roof1_temp", "side1_temp", "top_temp", "indoors_temp"))
	rh = plot2(U,from,to, c("outdoors_rh", "top_rh", "indoors_rh"))
	ah = plot2(U,from,to, c("outdoors_ah", "top_ah", "indoors_ah"))
	setpoint = plot2(U,from,to, c("sp_heat", "sp_vent", "sp_rh"))
	air = plot2(U,from,to, c("air_infilt", "air_crack", "air_top", "air_bottom"))
	grid.arrange(screen, temperature, rh, ah, setpoint, air, ncol=3)
}

plot5 = function(U,from=0, to=366) {
	top = plot2(U,from,to,
		c("top_energy_total","top_cond","top_air_outdoors1","top_air_outdoors2","top_air_indoors","top_air_conv","top_cover","top_light"))
	bottom1 = plot2(U,from,to,
		c("bottom_energy_total","bottom_cond","bottom_air_outdoors1","bottom_air_outdoors2","bottom_air_indoors","bottom_conv","bottom_cover","bottom_light")) 
	bottom2 = plot2(U,from,to,
		c("bottom_trans","bottom_heat","bottom_floor"))
	grid.arrange(top, bottom1, bottom2, ncol=3)
}


plot6 = function(U,from=0, to=366) {
	temperature = plot1(U,from,to, c("indoors_temp", "temp_top", "temp_mid", "temp_bot"))
	pg = plot1(U,from,to, c("Pg_top", "Pg_mid", "Pg_bot"))
	grid.arrange(temperature, pg, ncol=2)
}

plot7 = function(U,from=0, to=366) {
	p1 = plot2(U, from, to, c("temp_top","temp_mid","temp_bot"))
	p2 = plot2(U, from, to, c("flux_top","flux_mid","flux_bot"))
	p3 = plot2(U, from, to, c("abs_par_top","abs_par_mid","abs_par_bot"))
	p4 = plot2(U, from, to, c("abs_li_top","abs_li_mid","abs_li_bot"))
	p5 = plot2(U, from, to, c("abs_ir_top","abs_ir_mid","abs_ir_bot"))
	p6 = plot2(U, from, to, c("Pg_top","Pg_mid","Pg_bot"))
	p7 = plot2(U, from, to, c("indoors_temp", "energy_heat"))
	p8 = plot2(U, from, to, c("abs_top","abs_mid","abs_bot","ref_can","tra_can"))
	grid.arrange(p1,p2,p3,p4,p5,p6,p7,p8, ncol=4)
}

plot8 = function(U,from=0, to=366) {
	p1 = plot1(U, from, to, c("temp_top","temp_mid","temp_bot"))
	p2 = plot2(U, from, to, c("heat_flux", "heat_sum"))
	p3 = plot2(U, from, to, c("yield_kg_fw"))
	grid.arrange(p1,p2,p3, ncol=2)
}

plot9 = function(U,from=0, to=366) {
	p1 = plot2(U, from, to, c("roof1_temp", "temp_top","temp_mid","temp_bot"))
	p2 = plot2(U, from, to, c("abs_li_top","abs_li_mid","abs_li_bot"))
	p3 = plot2(U, from, to, c("abs_heat_top","abs_heat_mid","abs_heat_bot"))
	p4 = plot2(U, from, to, c("flux_top","flux_mid","flux_bot"))
	p5 = plot2(U, from, to, c("abs_gr_li_top","abs_gr_li_mid","abs_gr_li_bot"))
	p6 = plot2(U, from, to, c("lost_cover_top","lost_cover_mid","lost_cover_bot"))
	p7 = plot2(U, from, to, c("abs_tot_top","abs_tot_mid","abs_tot_bot"))
	p8 = plot2(U, from, to, c("outdoors_temp", "indoors_temp", "roof1_temp"))
	p9 = plot2(U, from, to, c("heat_flux", "heat_sum"))
	p10 = plot2(U, from, to, c("shl_tra_dir", "shl_tra_dif", "shl_tra_tot", "shl_abs_cov", "shl_abs_scr", "shl_haze", "shl_U", "shl_tra_air", "shl_cp_cov", "shl_cp_scr"))
	grid.arrange(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10, ncol=5)
}

plot10 = function(U,from=0, to=366) {
	p1 = plot2(U, from, to, c("shl_in_ir_abs_screens", "shl_in_ir_abs_cover", "shl_in_ir_abs_shelter", "shl_in_ir_abs", "shl_out_ir_abs"))
	p2 = plot2(U, from, to, c("shl_heat_out", "shl_heat_in", "shl_heat_sky"))
	p3 = plot2(U, from, to, c("shl_sun_cover", "shl_sun_scr"))
	p4 = plot2(U, from, to, c("shl_crop_top", "shl_crop_mid", "shl_crop_bot"))
	p5 = plot2(U, from, to, c("shl_cov_temp", "roof1_temp", "indoors_temp"))
	p6 = plot2(U, from, to, c("shl_abs_cov", "shl_abs_scr", "shl_cp_cov", "shl_cp_scr"))
	grid.arrange(p1,p2,p3,p4,p5,p6, ncol=3)
}
 
U = read_unisim("dvv_unisim_0001.txt")
windows(14,10)
plot10(U)

windows(14,10)
plot9(U)
