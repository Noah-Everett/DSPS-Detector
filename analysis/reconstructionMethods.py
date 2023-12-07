import numpy as np
import tqdm

def make_r(df_hits):
    r_list = []
    for index, row in tqdm.tqdm(df_hits.iterrows(), total=len(df_hits)):
        x, y = row['relativePosition']
        r = np.sqrt(x**2 + y**2)
        r_list.append(r)
    df_hits['r'] = r_list

    return df_hits

def make_theta(df_hits, rToTheta):
    if 'r' not in df_hits.columns:
        make_r(df_hits)

    theta_list = []
    for index, row in tqdm.tqdm(df_hits.iterrows(), total=len(df_hits)):
        x, y = row['relativePosition']
        r = row['r']
        theta = rToTheta(r)
        theta_list.append(theta)
    df_hits['theta'] = theta_list
    
    return df_hits

def make_phi(df_hits):
    phi_list = []
    for index, row in tqdm.tqdm(df_hits.iterrows(), total=len(df_hits)):
        x, y = row['relativePosition']
        phi = np.arctan(y / x)
        phi_list.append(phi)
    df_hits['phi'] = phi_list
    
    return df_hits

def make_reconstructedVector_time(df_hits, C_XENON_MM_PER_NS=162.93068369565216):
    df_hits['reconstructedVector_time'] = df_hits['reconstructedVector_direction'] * df_hits['time'] * C_XENON_MM_PER_NS
    
    return df_hits

def make_reconstructedPoint_time(df_hits):
    if 'reconstructedVector_time' not in df_hits.columns:
        make_reconstructedVector_time(df_hits)

    df_hits['reconstructedPoint_time'] = df_hits['sensor_position'] + df_hits['reconstructedVector_time']
    
    return df_hits

def make_reconstructedVector_direction(df_hits):
    def get_rotationMatrix(vector, target_direction):
        vector = vector / np.linalg.norm(vector)
        target_direction = target_direction / np.linalg.norm(target_direction)

        axis_of_rotation = np.cross(vector, target_direction)

        angle = np.arccos(np.dot(vector, target_direction))

        rotation_matrix = np.array([[np.cos(angle) + axis_of_rotation[0]**2 * (1 - np.cos(angle)),
                                    axis_of_rotation[0] * axis_of_rotation[1] * (1 - np.cos(angle)) - axis_of_rotation[2] * np.sin(angle),
                                    axis_of_rotation[0] * axis_of_rotation[2] * (1 - np.cos(angle)) + axis_of_rotation[1] * np.sin(angle)],
                                    [axis_of_rotation[1] * axis_of_rotation[0] * (1 - np.cos(angle)) + axis_of_rotation[2] * np.sin(angle),
                                    np.cos(angle) + axis_of_rotation[1]**2 * (1 - np.cos(angle)),
                                    axis_of_rotation[1] * axis_of_rotation[2] * (1 - np.cos(angle)) - axis_of_rotation[0] * np.sin(angle)],
                                    [axis_of_rotation[2] * axis_of_rotation[0] * (1 - np.cos(angle)) - axis_of_rotation[1] * np.sin(angle),
                                    axis_of_rotation[2] * axis_of_rotation[1] * (1 - np.cos(angle)) + axis_of_rotation[0] * np.sin(angle),
                                    np.cos(angle) + axis_of_rotation[2]**2 * (1 - np.cos(angle))]])
        return rotation_matrix

    output_vectors = []
    for _, row in df_hits.iterrows():
        theta = row['theta']
        phi = row['phi']
        sensor_direction = row['sensor_direction']
        
        output_vector = [np.sin(theta) * np.cos(phi), np.sin(theta) * np.sin(phi), np.cos(theta)]
        
        rotationMatrix = get_rotationMatrix([0, 0, 1], sensor_direction)
        rotated_vector = np.dot(rotationMatrix, output_vector)
        
        output_vectors.append(rotated_vector)

    df_hits['reconstructedVector_direction'] = output_vectors
    
    return df_hits

def make_reconstructedPoint_primary_distance(df_hits, df_primary):
    closestPoints = np.full((df_hits.shape[0], 3), -1)
    minDistances = np.full((df_hits.shape[0], 1), -1)
    positions = np.array(df_primary['position'].tolist())
    for i, hit_row in tqdm.tqdm(df_hits.iterrows(), total=df_hits.shape[0]):
        recoVectorDirection = hit_row['reconstructedVector_direction']
        sensorPosition = hit_row['sensor_position']

        vectors = positions - sensorPosition

        dot_products = np.dot(vectors, recoVectorDirection)
        projections = dot_products / np.linalg.norm(recoVectorDirection)
        closest_points_on_line = sensorPosition + projections[:, np.newaxis] * recoVectorDirection
        distances = np.linalg.norm(closest_points_on_line - positions, axis=1)

        minDistanceIndex = np.argmin(distances)
        closestPoints[i] = closest_points_on_line[minDistanceIndex]
        minDistances[i] = distances[minDistanceIndex]

    df_hits['reconstructedPoint_primary_distance'] = closestPoints.tolist()
    df_hits['reconstructedPoint_primary_distance_r'] = minDistances.reshape(-1).tolist()
    
    return df_hits

def make_reconstructedPoint_initial_distance(df_hits):
    recoVectorDirections = df_hits['reconstructedVector_direction'].values
    sensorPositions = df_hits['sensor_position'].values
    initialPositions = df_hits['initialPosition'].values

    vectors = np.array([np.array(ip) - np.array(sp) for ip, sp in zip(initialPositions, sensorPositions)])

    dot_products = np.array([np.dot(v, rv) for v, rv in zip(vectors, recoVectorDirections)])
    projections = np.array([ dp / np.linalg.norm(rv) for dp, rv in zip(dot_products, recoVectorDirections)])    
    closest_points_on_line = np.array([sp + p * rvd for sp, p, rvd in zip(sensorPositions, projections, recoVectorDirections)])
    distances = np.linalg.norm(np.array([cpol - ip for cpol, ip in zip(closest_points_on_line, initialPositions)]), axis=1)

    df_hits['reconstructedPoint_initial_distance'] = closest_points_on_line.tolist()
    df_hits['reconstructedPoint_initial_distance_r'] = distances.tolist()
    
    return df_hits