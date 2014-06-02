rm(list=ls(all=TRUE))

# From test_geometry.cpp
roofHeight = 10./sqrt(3.)
roofWidth = 2*roofHeight
roofArea = 3*2*roofWidth*50
gablesArea = 3*roofHeight*20
gablesArea
coverAreaTotal-gablesArea


coverAreaTotal = (100+120)*4 + 6*(roofHeight*10 + roofWidth*50)
coverAreaTotal
coverAreaTotal-gablesArea
f = roofArea/coverAreaTotal
f

coverK = 7.5
screenReduction = 0.4
withScreenK = coverK*(1-screenReduction)
commonK = f*withScreenK + (1-f)*coverK

coverK
withScreenK
commonK
