setwd("C:/data/QDev/UniSim/doc/spatial")

head = "<html><body><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"
tail = "</svg></body></html>"
colours = c("#FF416E","#3760D5","#FFC741","#34C1E8","#F7FF41","#A135D0","#39DE52")
nColours = length(colours)

attrib = function(name,value) {
	paste(name, "=\"", value, "\"", sep="")
}

square = function(x,y,size,colour) {
	paste("<rect", attrib("x",x), attrib("y",y), attrib("width",size), attrib("height",size),
		attrib("style",paste("fill:",colour,sep="")),
		"/>")
}

svg = function(rows,cols,size) {
	all ="<g>"
	for (r in 1:rows) {
		for (c in 1:cols) {
			ixColour = sample(1:nColours,1)
			value = square(c*size,r*size,size,colours[ixColour])
			all =  c(all,value)
		}
	}
	all = c(all,"</g>")
	all
}

f = file("grid.html")
writeLines(c(head,svg(70,70,5),tail),f)
close(f)
