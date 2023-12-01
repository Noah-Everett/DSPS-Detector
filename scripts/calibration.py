grid_size = (5, 5, 5) # m
world_size = (2.5, 2.5, 2.5) # m
detector_wall_thickness = 0.1 # m
calorimeter_height = 0.2 # m
buffer_room = 0.1 # m
nParticles = 1000

FV_size = tuple(WS - 2 * detector_wall_thickness - 2 * calorimeter_height - 2 * buffer_room for WS in world_size)
grid_space = tuple(FV / (GS-1) for FV, GS in zip(FV_size, grid_size))

with open('calibration.mac', 'w') as f:
    f.write("##########################\n")
    f.write("# Calibration macro file #\n")
    f.write("##########################\n")
    f.write("\n")
    f.write("# Global parameters\n")
    f.write("/run/initialize\n")
    f.write("/control/verbose 0\n")
    f.write("/run/verbose 0\n")
    f.write("/event/verbose 0\n")
    f.write("/tracking/verbose 0\n")
    f.write("/gun/particle PhotonCreator\n")
    f.write("/gun/energy 24.12 eV\n")
    f.write("/analysis/setHistoDirName photoSensor_hits_histograms\n")
    f.write("\n")
    f.write("# Detector parameters\n")

    for x in range(grid_size[0]):
        for y in range(grid_size[1]):
            for z in range(grid_size[2]):
                pos_x = x * grid_space[0] - FV_size[0] / 2
                pos_y = y * grid_space[1] - FV_size[1] / 2
                pos_z = z * grid_space[2] - FV_size[2] / 2
                f.write(f"/gun/position {pos_x:.5f} {pos_y:.5f} {pos_z:.5f} m\n")
                f.write(f"/analysis/setFileName ../runs/calibration/calibration_{pos_x:.5f}_{pos_y:.5f}_{pos_z:.5f}.root\n")
                f.write(f"/run/beamOn {nParticles}\n\n")
                print(f"Calibration {x}, {y}, {z} --> {pos_x:.5f}, {pos_y:.5f}, {pos_z:.5f} done")
print("Calibration done")
print("Total number of particles: ", nParticles * grid_size[0] * grid_size[1] * grid_size[2])