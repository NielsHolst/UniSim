### Prepare R environment
rm(list=ls(all=TRUE))
graphics.off()
library(ggplot2)
library(plyr)
library(reshape2)
library(R.matlab)

read_matlab = function(test_num) {
	col_df = function(i) {
		y = M[i,1,1]
		as.data.frame(y)
	} 

	folder = "C:/data/QDev/UniSim/recipes/vg/test"
	fname = paste0("output/OKO", test_num, ".dat")
	fname = paste(folder,fname,sep="/")
	M = readMat(fname)
	output_ix = length(M)
	M = M[[output_ix]]
	n = dim(M)[1]
	Mw = data.frame(Day=(1:(365*24))/24)
	for (i in 1:n) Mw = cbind(Mw, col_df(i))
	Mw = mutate(Mw, Day=as.numeric(rownames(Mw))/24)
	Mw[,1:(ncol(Mw)-5)]
}

plot_M = function(from=0, to=366) {
	# V = melt(M[1:(24*4),], id.vars="Day")
	V = melt(M[M$Day>=from & M$Day<=to, ], id.vars="Day")
	ggplot(V) +
		geom_line(aes(x=Day, y=value, color=variable)) +
		theme(legend.position="none") +
		facet_wrap(~variable, scales="free")
}

M = read_matlab(1)
windows(14,10)
plot_M(1,30)
