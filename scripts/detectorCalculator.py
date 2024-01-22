# grid_size = (10, 10, 10)
# detector_wall_thickness = 0.1 # m
# calorimeter_size = (0.2, 0.02, 0.5) # m
# photoSensor_size = (0.2, 0.2, 0.05) # m
grid_size = (15, 15, 15)
detector_wall_thickness = 0.1 # cm
calorimeter_size = (14, 2, 67) # cm
photoSensor_size = (14, 14, 5) # cm

calorimeter_size = tuple(size / 100 for size in calorimeter_size)
photoSensor_size = tuple(size / 100 for size in photoSensor_size)

PS = photoSensor_size[0]
CS = calorimeter_size[1]

size_woCalThickness = tuple(GS*PS + (GS+1)*CS for GS in grid_size)
print("side size = ", size_woCalThickness)

volume = size_woCalThickness[0] * size_woCalThickness[1] * size_woCalThickness[2]
print("volume = ", volume)

density = 2950 # kg/m^3
mass = density * volume
print("mass [t] = ", mass/1000)

# CS = calorimeter_size[1] + detector_wall_thickness
# size = tuple(SS + 2 * CS for SS in size_woCalThickness)
# print("side size = ", size)