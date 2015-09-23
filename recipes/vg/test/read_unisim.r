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
	p1 = plot2(U, from, to, c("shl_cov_temp", "temp_top","temp_mid","temp_bot"))
	p2 = plot2(U, from, to, c("abs_li_top","abs_li_mid","abs_li_bot"))
	p3 = plot2(U, from, to, c("abs_heat_top","abs_heat_mid","abs_heat_bot"))
	p4 = plot2(U, from, to, c("flux_top","flux_mid","flux_bot"))
	p5 = plot2(U, from, to, c("abs_gr_li_top","abs_gr_li_mid","abs_gr_li_bot"))
	p6 = plot2(U, from, to, c("abs_tot_top","abs_tot_mid","abs_tot_bot"))
	p7 = plot2(U, from, to, c("outdoors_temp", "indoors_temp"))
	p8 = plot2(U, from, to, c("heat_flux", "heat_sum"))
	p9 = plot2(U, from, to, c("shl_tra_dir", "shl_tra_dif", "shl_tra_tot", "shl_abs_cov", "shl_abs_scr", "shl_haze", "shl_U", "shl_tra_air", "shl_cp_cov", "shl_cp_scr"))
	grid.arrange(p1,p2,p3,p4,p5,p6,p7,p8,p9, ncol=5)
}

plot10 = function(U,from=0, to=366) {
	p1 = plot2(U, from, to, c("shl_in_ir_abs_screens", "shl_in_ir_abs_cover", "shl_in_ir_abs_shelter", "shl_in_ir_abs", "shl_out_ir_abs"))
	p2 = plot2(U, from, to, c("shl_heat_out", "shl_heat_in", "shl_heat_sky"))
	p3 = plot2(U, from, to, c("shl_sun_cover", "shl_sun_scr"))
	p4 = plot2(U, from, to, c("shl_crop_top", "shl_crop_mid", "shl_crop_bot"))
	p5 = plot2(U, from, to, c("shl_cov_temp", "indoors_temp", "sp_heat", "sp_vent"))
	p6 = plot2(U, from, to, c("shl_abs_cov", "shl_abs_scr", "shl_cp_cov", "shl_cp_scr"))
	grid.arrange(p1,p2,p3,p4,p5,p6, ncol=3)
}

plot11 = function(U,from=0, to=366) {
	grid.arrange(
		plot2(U, from, to, c("giv_eflux_sum", "giv_eflux_trans", "giv_eflux_cond_cov", "giv_eflux_cond_scr")),
		plot2(U, from, to, c("giv_eflux_lamp", "giv_eflux_shelter", "giv_eflux_floor")),
		plot2(U, from, to, c("outdoors_temp", "indoors_temp", "indoors_rh")),
		plot2(U, from, to, c("heated_temp", "sp_heat", "sp_vent")),
		plot2(U, from, to, c("heat_flux", "heat_sum")),
		plot2(U, from, to, c("air_giv_infilt", "air_giv_crack", "air_giv_grav", "air_giv_tot")),
		# plot2(U, from, to, c("leaf_tra_cond_top", "leaf_tra_cond_mid", "leaf_tra_cond_bot")),
		# plot2(U, from, to, c("leaf_tra_lah_top", "leaf_tra_lah_mid", "leaf_tra_lah_bot")),
		# plot2(U, from, to, c("leaf_tra_iah_top", "leaf_tra_iah_mid", "leaf_tra_iah_bot")),
		plot2(U, from, to, c("en_cool_demand", "en_cool_air_sup_max", "air_cooling", "en_cool_air_sup")),
        plot2(U, from, to, c("Pg_top", "Pg_mid", "Pg_bot")),
		plot2(U, from, to, c("abs_tot_top", "abs_tot_mid", "abs_tot_bot")),
		plot2(U, from, to, c("temp_top", "temp_mid", "temp_bot")),
		# plot2(U, from, to, c("co2_ppm", "co2_assim", "co2_inject", "co2_inject_sum")),
		ncol=5
	)	
}


plot12 = function(U,from=0, to=366) {
	windows(14,10)
	grid.arrange(
		plot2(U, from, to, c("indoorsTemperature", "indoors_rh")),
		plot2(U, from, to, c("heatingEnergyFlux", "heatingEnergyTotal")),
			plot2(U, from, to, c("heated_temp", "sp_heat", "sp_vent")),
		plot2(U, from, to, c("heating_supply", "heating_pred", "heating_slope")),
		plot2(U, from, to, c("leaf_cond_top", "leaf_cond_mid", "leaf_cond_bot")),
		ncol=3
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
		plot2(U, from, to, c("top_Pn", "mid_Pn", "bot_Pn")),
		plot2(U, from, to, c("top_Pg", "mid_Pg", "bot_Pg")),
		plot2(U, from, to, c("top_rb_H2o", "mid_rb_H2o", "bot_rb_H2o")),
		plot2(U, from, to, c("top_rb_Co2", "mid_rb_Co2", "bot_rb_Co2")),
		plot2(U, from, to, c("top_rs_H2o", "mid_rs_H2o", "bot_rs_H2o")),
		plot2(U, from, to, c("top_rs_Co2", "mid_rs_Co2", "bot_rs_Co2")),
		plot2(U, from, to, c("top_LUE", "mid_LUE", "bot_LUE")),
		plot2(U, from, to, c("top_Pnmax", "mid_Pnmax", "bot_Pnmax")),
		plot2(U, from, to, c("top_Pgmax", "mid_Pgmax", "bot_Pgmax")),
		plot2(U, from, to, c("indoorsTemperature", "indoorsRh", "indoorsCo2", "airFluxTotal")),
		ncol=5
	)	
}
 
U = read_unisim("dvv_unisim_0001.txt")
production(U,1)
photosynthesis(U,1)

 