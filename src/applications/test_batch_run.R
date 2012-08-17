folder = "C:/data/QDev/UniSim/src/applications"
prog = "UniSim-1-37d.exe"
xml = "C:/data/QDev/UniSim/src/plugins/unisim_models/test/input/test_fixed.xml"
cmd = paste(folder,"/",prog," -i \"",xml,"\"",sep="")
shell(cmd)

