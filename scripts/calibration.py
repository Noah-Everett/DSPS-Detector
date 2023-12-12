grid_size = (8, 8, 8) # m
detector_wall_thickness = 0.1 # m
calorimeter_size = (0.2, 0.02, 0.5) # m
photoSensor_size = (0.2, 0.2, 0.05) # m
buffer_room = 0.0 # m
nParticles = 1

PS = photoSensor_size[0]
CS = calorimeter_size[1]
FV_size = tuple(GS*PS + (GS+1)*CS - buffer_room for GS in grid_size)

grid_space = tuple(FV / (GS-1) for FV, GS in zip(FV_size, grid_size))

with open('calibration.mac', 'w') as f:
    f.write("##########################\n")
    f.write("# Calibration macro file #\n")
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
    f.write("/gun/particle e-\n")#PhotonCreator\n")
    f.write("/gun/energy 5 MeV\n")# 24.12 eV\n")
    f.write("\n")
    f.write("/particleGun/momentum/random true\n")
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
    f.write("\n")
    f.write("/run/beamOn 300")

# with open('calibration.mac', 'w') as f:
#     f.write("##########################\n")
#     f.write("# Calibration macro file #\n")
#     f.write("##########################\n")
#     f.write("\n")
#     f.write("# Global parameters\n")
#     f.write("/run/initialize\n")
#     f.write("\n")
#     f.write("/control/verbose 0\n")
#     f.write("/run/verbose 0\n")
#     f.write("/event/verbose 0\n")
#     f.write("/tracking/verbose 0\n")
#     f.write("\n")
#     f.write("/gun/particle e-\n")#PhotonCreator\n")
#     f.write("/gun/energy 5 MeV\n")# 24.12 eV\n")
#     f.write("\n")
#     f.write("/particleGun/momentum/random true\n")
#     f.write("/particleGun/position/x/random true\n")
#     f.write("/particleGun/position/x/min -{:.5f} m\n".format(FV_size[0] / 2))
#     f.write("/particleGun/position/x/max  {:.5f} m\n".format(FV_size[0] / 2))
#     f.write("/particleGun/position/y/random true\n")
#     f.write("/particleGun/position/y/min -{:.5f} m\n".format(FV_size[1] / 2))
#     f.write("/particleGun/position/y/max  {:.5f} m\n".format(FV_size[1] / 2))
#     f.write("/particleGun/position/z/random true\n")
#     f.write("/particleGun/position/z/min -{:.5f} m\n".format(FV_size[2] / 2))
#     f.write("/particleGun/position/z/max  {:.5f} m\n".format(FV_size[2] / 2))
#     f.write("\n")
#     f.write("/analysis/setHistoDirName photoSensor_hits_histograms\n")
#     f.write("\n")
#     f.write("# Detector parameters\n")

#     for x in range(grid_size[0]):
#         for y in range(grid_size[1]):
#             for z in range(grid_size[2]):
#                 pos_x = x * grid_space[0] - FV_size[0] / 2
#                 pos_y = y * grid_space[1] - FV_size[1] / 2
#                 pos_z = z * grid_space[2] - FV_size[2] / 2
#                 f.write(f"/gun/position {pos_x:.5f} {pos_y:.5f} {pos_z:.5f} m\n")
#                 f.write(f"/analysis/setFileName ../runs/calibration/calibration_{pos_x:.5f}_{pos_y:.5f}_{pos_z:.5f}.root\n")
#                 f.write(f"/run/beamOn {nParticles}\n\n")
#                 print(f"Calibration {x}, {y}, {z} --> {pos_x:.5f}, {pos_y:.5f}, {pos_z:.5f} done")
# print("Calibration done")
# print("Total number of particles: ", nParticles * grid_size[0] * grid_size[1] * grid_size[2])