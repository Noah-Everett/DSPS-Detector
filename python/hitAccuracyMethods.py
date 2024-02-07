import numpy as np
import tqdm

def make_r(df_hits, binned=True):
    if binned:
        x, y = np.array(df_hits['relativePosition_binned'].tolist()).reshape(-1,2).T
    else:
        x, y = np.array(row['relativePosition'].tolist()).reshape(-1,2).T

    r = np.sqrt(x**2 + y**2)
    df_hits['r'] = r.tolist()

    return df_hits

def make_theta(df_hits, rToTheta, binned=True):
    r = df_hits['r'].values
    theta = rToTheta(r)
    df_hits['theta'] = theta.tolist()

    return df_hits

def make_phi(df_hits, binned=True):
    if binned:
        x, y = np.array(df_hits['relativePosition_binned'].tolist()).reshape(-1,2).T
    else:
        x, y = np.array(row['relativePosition'].tolist()).reshape(-1,2).T

    phi = np.arctan2(y, x)
    df_hits['phi'] = phi.tolist()

    return df_hits

def make_relativeVector(df_hits):
    initialPositions = df_hits['initialPosition']
    sensorPositions = df_hits['sensor_position']

    df_hits['relativeVector'] = (np.array(initialPositions.to_list()) - np.array(sensorPositions.to_list())).tolist()
    df_hits['relativeVector_r'] = np.linalg.norm(df_hits['relativeVector'].to_list(), axis=1).tolist()

    return df_hits

def make_reconstructedVector_time(df_hits, C_XENON_MM_PER_NS=162.93068369565216, lensDistance=300):
    df_hits['reconstructedVector_time'] = df_hits['reconstructedVector_direction'] * (df_hits['time'] - lensDistance/C_XENON_MM_PER_NS) * C_XENON_MM_PER_NS
    
    return df_hits

def make_reconstructedPoint_time(df_hits, C_XENON_MM_PER_NS=162.93068369565216, lensDistance=300):
    if 'reconstructedVector_time' not in df_hits.columns:
        make_reconstructedVector_time(df_hits, C_XENON_MM_PER_NS, lensDistance)

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