import vacuumms as v
#f=v.LAMMPSConfiguration("equilibrated_system.data")
c=v.Configuration("../data/fcc.gfg")
p=v.Parameters("../data/fcc.prm")
f.getSize()
f.getBoxDimensions()
f.cram()
f.isCrammed()
f
p = v.Parameters(["-n", "100", "-box", "4.24264", "4.24264", "4.24264"])
p
p.addParameter("-box  87.578857  87.578857 87.578857")
p
p.toFile("foo.prm")
p=v.Parameters("foo.prm")
p
o = v.DDX(c, p)

o = v.DDX(f, p)
o.execute()
result=o.getResult()

csdp=v.Parameters(['-width', '0.1', '-n_bins', '50'])
csd=v.CavitySizeDistribution(result, csdp)

csd
o
p
csd.writeToFile('foo.bar')

