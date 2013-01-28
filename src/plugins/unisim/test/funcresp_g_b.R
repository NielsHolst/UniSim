gb = function(D,X,a) {
	D*(1. - exp(-a*X/D))
}

X = 30
D = 10
a = 0.8
resp = 1
egest = 0.2

Stot = gb((D+resp)/(1-egest),X,a)
Stot
Snet = Stot*(1-egest) - resp
Snet

gb(10,20,0.8047)

