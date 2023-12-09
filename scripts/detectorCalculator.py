grid_size = (10, 10, 10) # m
detector_wall_thickness = 0.1 # m
calorimeter_size = (0.2, 0.02, 0.5) # m
photoSensor_size = (0.2, 0.2, 0.05) # m

PS = photoSensor_size[0]
CS = calorimeter_size[1]

size_woCalThickness = tuple(GS*PS + (GS+1)*CS for GS in grid_size)
print("side size = ", size_woCalThickness)

# CS = calorimeter_size[1] + detector_wall_thickness
# size = tuple(SS + 2 * CS for SS in size_woCalThickness)
# print("side size = ", size)