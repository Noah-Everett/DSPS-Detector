import numpy as np

FV_size = (2, 2, 2)

nRuns = 100
nStart = 200

with open('multievent3.mac', 'w') as f:
    f.write("##########################\n")
    f.write("# Event macro file #\n")
    f.write("##########################\n")
    f.write("\n")
    f.write("# Global parameters\n")
    f.write("/run/initialize\n")
    f.write("\n")
    f.write("/control/verbose 0\n")
    f.write("/run/verbose 0\n")
    f.write("/event/verbose 0\n")
    f.write("/tracking/verbose 0\n")
    f.write("\n")
    f.write("/gun/particle mu-\n")
    f.write("/gun/energy 700 MeV\n")
    f.write("\n")
    f.write("/particleGun/momentum/random true\n")
    f.write("/particleGun/nParticles 1\n")
    f.write("\n")
    f.write("/particleGun/position/x/random true\n")
    f.write("/particleGun/position/x/min -{:.5f} m\n".format(FV_size[0] / 2))
    f.write("/particleGun/position/x/max  {:.5f} m\n".format(FV_size[0] / 2))
    f.write("/particleGun/position/y/random true\n")
    f.write("/particleGun/position/y/min -{:.5f} m\n".format(FV_size[1] / 2))
    f.write("/particleGun/position/y/max  {:.5f} m\n".format(FV_size[1] / 2))
    f.write("/particleGun/position/z/random true\n")
    f.write("/particleGun/position/z/min -{:.5f} m\n".format(FV_size[2] / 2))
    f.write("/particleGun/position/z/max  {:.5f} m\n".format(FV_size[2] / 2))
    f.write("\n")
    f.write("/analysis/setHistoDirName photoSensor_hits_histograms\n")

    for n in np.arange(nStart, nStart + nRuns):
        f.write("\n")
        f.write("/analysis/setFileName multievent_{}.root\n".format(n))
        f.write("/run/beamOn 1\n")