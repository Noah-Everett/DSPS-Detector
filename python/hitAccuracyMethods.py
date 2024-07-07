import numpy as np
import tqdm
from constants import *
import pickle as pkl
import pandas as pd

def make_r(df_hits, binned=True, x=None, y=None):
    if x is None or y is None:
        if binned:
            x, y = np.array(df_hits['relativePosition_binned'].tolist()).reshape(-1,2).T
        else:
            x, y = np.array(df_hits['relativePosition'].tolist()).reshape(-1,3).T[:2]

    r = np.sqrt(x**2 + y**2)
    df_hits['r'] = r.tolist()

    return df_hits

def make_theta(df_hits, rToTheta, binned=True):
    r = df_hits['r'].values
    theta = rToTheta(r)
    df_hits['theta'] = theta.tolist()

    return df_hits

def boundAngle(angle):
    angle[angle > np.pi] -= 2 * np.pi
    angle[angle < -np.pi] += 2 * np.pi

    return angle

def make_phi(df_hits, binned=True, x=None, y=None):
    if x is None or y is None:
        if binned:
            x, y = np.array(df_hits['relativePosition_binned'].tolist()).reshape(-1,2).T
        else:
            x, y = np.array(df_hits['relativePosition'].tolist()).reshape(-1,3).T[:2]

    phi = np.arctan2(y, x)
    # phi += np.pi
    # phi = boundAngle(phi)

    df_hits['phi'] = phi.tolist()

    return df_hits

def make_recoDir_num(df_hits, PDFfileName, x=None, y=None, numSamples=1000, method=np.mean):
    if x is None or y is None:
        x, y = np.array(df_hits['relativePosition_binned'].tolist()).reshape(-1,2).T.astype(int)

    PDFs = pkl.load(open(PDFfileName, 'rb'))

    recoDirs = []
    for xi, yi in tqdm.tqdm(zip(x, y), total=len(x)):
        try:
            PDF        = PDFs[(xi, yi)][0]
            thetaEdges = PDFs[(xi, yi)][1]
            phiEdges   = PDFs[(xi, yi)][2]

            PDF_flat = PDF.flatten()
            PDF_flat /= np.sum(PDF_flat)

            samples_ind = np.random.choice(np.arange(len(PDF_flat)), numSamples, p=PDF_flat)
            samples_ind = np.unravel_index(samples_ind, PDF.shape)
            samples_ind = np.array(samples_ind).T

            samples_dir = np.array([
                thetaEdges[samples_ind[:,0], samples_ind[:,1]][0],
                phiEdges  [samples_ind[:,0], samples_ind[:,1]][0]
            ]).T
            samples_dir = np.reshape(samples_dir, (-1, 2))

            recoDir = method(samples_dir, axis=0)
            recoDirs.append(np.reshape(recoDir, -1))
        except:
            recoDirs.append([np.nan, np.nan])

    df_hits['theta_num'] = [rd[0] for rd in recoDirs]
    df_hits['phi_num']   = [rd[1] for rd in recoDirs]

    return df_hits

def make_recoDir_ref(df_hits):
    df_hits['theta_ref'] = 0
    df_hits['phi_ref']   = 0

    return df_hits

def changePhiSignWithTheta(phis, thetas):
    phis = np.array(phis)
    thetas = np.array(thetas)

    phis  [thetas < 0] *= -1
    thetas[thetas < 0] *= -1

    return phis, thetas

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
    
def get_rotationMatrix(vector, target_direction):
    vector = vector / np.linalg.norm(vector)
    target_direction = target_direction / np.linalg.norm(target_direction)

    axis_of_rotation = np.cross(vector, target_direction)

    angle = np.arccos(np.dot(vector, target_direction))

    rotation_matrix = np.array([
        [
            np.cos(angle) + axis_of_rotation[0]**2 * (1 - np.cos(angle)),
            axis_of_rotation[0] * axis_of_rotation[1] * (1 - np.cos(angle)) - axis_of_rotation[2] * np.sin(angle),
            axis_of_rotation[0] * axis_of_rotation[2] * (1 - np.cos(angle)) + axis_of_rotation[1] * np.sin(angle)
        ],[
            axis_of_rotation[1] * axis_of_rotation[0] * (1 - np.cos(angle)) + axis_of_rotation[2] * np.sin(angle),
            np.cos(angle) + axis_of_rotation[1]**2 * (1 - np.cos(angle)),
            axis_of_rotation[1] * axis_of_rotation[2] * (1 - np.cos(angle)) - axis_of_rotation[0] * np.sin(angle)
        ],[
            axis_of_rotation[2] * axis_of_rotation[0] * (1 - np.cos(angle)) - axis_of_rotation[1] * np.sin(angle),
            axis_of_rotation[2] * axis_of_rotation[1] * (1 - np.cos(angle)) + axis_of_rotation[0] * np.sin(angle),
            np.cos(angle) + axis_of_rotation[2]**2 * (1 - np.cos(angle))
        ]
    ])

    return rotation_matrix
# print('+y', np.dot(get_rotationMatrix([0,0,1], [ 0,-1, 0]), [1,1,1])) # = [ 1,-1, 1]
# print('-x', np.dot(get_rotationMatrix([0,0,1], [ 1, 0, 0]), [1,1,1])) # = [ 1, 1,-1]
# print('-z', np.dot(get_rotationMatrix([0,0,1], [ 0, 0, 1]), [1,1,1])) # = [ 1, 1, 1]
# print('+x', np.dot(get_rotationMatrix([0,0,1], [-1, 0, 0]), [1,1,1])) # = [-1, 1, 1]
# print('+z', np.dot(get_rotationMatrix([0,0,1], [ 0, 0,-1]), [1,1,1])) # = [-1,-1,-1]
# print('-y', np.dot(get_rotationMatrix([0,0,1], [ 0, 1, 0]), [1,1,1])) # = [ 1, 1,-1]

def make_reconstructedVector_direction(df_hits, thetaName='theta', phiName='phi', outputName='reconstructedVector_direction'):
    timeStart = pd.Timestamp.now()
    df_hits_old = df_hits.copy()
    output_vectors = []
    for _, row in df_hits_old.iterrows():
        theta = row[thetaName]
        phi = row[phiName]
        sensor_direction = row['sensor_direction']
        sensor_wall = row['sensor_wall']
        
        output_vector = [np.sin(theta) * np.cos(phi), np.sin(theta) * np.sin(phi), np.cos(theta)]
        relativeSensorXYZ_sign  = RELATIVE_SENSOR_POSITIONS_XYZ_SIGN [sensor_wall]
        relativeSensorXYZ_index = RELATIVE_SENSOR_POSITIONS_XYZ_INDEX[sensor_wall]
        rotated_vector = np.array([
            -output_vector[0] * relativeSensorXYZ_sign[0],
            -output_vector[1] * relativeSensorXYZ_sign[1],
            output_vector[2] * relativeSensorXYZ_sign[2]
        ])
        rotated_vector = np.array([
            rotated_vector[relativeSensorXYZ_index[0]],
            rotated_vector[relativeSensorXYZ_index[1]],
            rotated_vector[relativeSensorXYZ_index[2]]
        ])
        
        # rotationMatrix = get_rotationMatrix([0, 0, 1], sensor_direction)
        # rotated_vector = np.dot(rotationMatrix, output_vector)
        
        output_vectors.append(rotated_vector)

    df_hits_old[outputName] = output_vectors
    
    # return df_hits
    timeEnd = pd.Timestamp.now()
    oldTime = timeEnd - timeStart

    timeStart = pd.Timestamp.now()
    df_hits_new = df_hits.copy()

    # Calculate sin and cos for theta and phi
    sin_theta = np.sin(df_hits_new[thetaName])
    cos_theta = np.cos(df_hits_new[thetaName])
    sin_phi = np.sin(df_hits_new[phiName])
    cos_phi = np.cos(df_hits_new[phiName])
    
    # Create the output vectors using broadcasting
    output_vectors = np.column_stack((sin_theta * cos_phi, sin_theta * sin_phi, cos_theta))
    
    # Apply the relative sensor positions and signs
    def rotate_vector(row):
        relativeSensorXYZ_sign = RELATIVE_SENSOR_POSITIONS_XYZ_SIGN[row['sensor_wall']]
        relativeSensorXYZ_index = RELATIVE_SENSOR_POSITIONS_XYZ_INDEX[row['sensor_wall']]
        rotated_vector = np.array([
            -row['x'] * relativeSensorXYZ_sign[0],
            -row['y'] * relativeSensorXYZ_sign[1],
            row['z'] * relativeSensorXYZ_sign[2]
        ])
        # return rotated_vector[relativeSensorXYZ_index]
        return np.array([
            rotated_vector[relativeSensorXYZ_index[0]],
            rotated_vector[relativeSensorXYZ_index[1]],
            rotated_vector[relativeSensorXYZ_index[2]]
        ])

    # Split output_vectors into separate columns
    df_temp = pd.DataFrame(output_vectors, columns=['x', 'y', 'z'], index=df_hits_new.index)
    df_hits_new = pd.concat([df_hits_new, df_temp], axis=1)
    
    # Apply the rotation to each row
    df_hits_new[outputName] = df_hits_new.apply(rotate_vector, axis=1)

    # Drop temporary columns
    df_hits_new.drop(columns=['x', 'y', 'z'], inplace=True)
    
    timeEnd = pd.Timestamp.now()
    newTime = timeEnd - timeStart

    print('Old time:', oldTime)
    print('New time:', newTime)
    print()

    for new, old in zip(df_hits_new[outputName].values, df_hits_old[outputName].values):
        if not np.allclose(new, old):
            raise ValueError('Error in make_reconstructedVector_direction')
    

    df_hits = df_hits_new
    
    return df_hits_new
        

def make_reconstructedVector_direction_num(df_hits, thetaName='theta_num', phiName='phi_num', outputName='reconstructedVector_direction_num'):
    return make_reconstructedVector_direction(df_hits, thetaName, phiName, outputName)

def make_reconstructedVector_direction_ref(df_hits, thetaName='theta_ref', phiName='phi_ref', outputName='reconstructedVector_direction_ref'):
    return make_reconstructedVector_direction(df_hits, thetaName, phiName, outputName)

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

def make_reconstructedPoint_initial_distance(df_hits, recoVectorDirectionName='reconstructedVector_direction', outputNames=['reconstructedPoint_initial_distance', 'reconstructedPoint_initial_distance_r']):
    recoVectorDirections = df_hits[recoVectorDirectionName].values
    sensorPositions = df_hits['sensor_position'].values
    initialPositions = df_hits['initialPosition'].values

    vectors = np.array([np.array(ip) - np.array(sp) for ip, sp in zip(initialPositions, sensorPositions)])

    dot_products = np.array([np.dot(v, rv) for v, rv in zip(vectors, recoVectorDirections)])
    projections = np.array([ dp / np.linalg.norm(rv) for dp, rv in zip(dot_products, recoVectorDirections)])    
    closest_points_on_line = np.array([sp + p * rvd for sp, p, rvd in zip(sensorPositions, projections, recoVectorDirections)])
    distances = np.linalg.norm(np.array([cpol - ip for cpol, ip in zip(closest_points_on_line, initialPositions)]), axis=1)

    df_hits[outputNames[0]] = closest_points_on_line.tolist()
    df_hits[outputNames[1]] = distances.tolist()
    
    return df_hits

def make_reconstructedPoint_initial_distance_num(df_hits, recoVectorDirectionName='reconstructedVector_direction_num', outputNames=['reconstructedPoint_initial_distance_num', 'reconstructedPoint_initial_distance_r_num']):
    return make_reconstructedPoint_initial_distance(df_hits, recoVectorDirectionName, outputNames)

def make_reconstructedPoint_initial_distance_ref(df_hits, recoVectorDirectionName='reconstructedVector_direction_ref', outputNames=['reconstructedPoint_initial_distance_ref', 'reconstructedPoint_initial_distance_r_ref']):
    return make_reconstructedPoint_initial_distance(df_hits, recoVectorDirectionName, outputNames)

def sph2cart(theta, phi):
    x = np.sin(theta) * np.cos(phi)
    y = np.sin(theta) * np.sin(phi)
    z = np.cos(theta)
    return np.array([x, y, z])

def angle_between_vectors(v1, v2):
    # if len(v1.shape) == 1:
    #     v1 = v1[np.newaxis, :]
    # if len(v2.shape) == 1:
    #     v2 = v2[np.newaxis, :]

    # angles = []
    # for v1_i, v2_i in zip(v1, v2):
    #     v1_i = v1_i/np.linalg.norm(v1_i)
    #     v2_i = v2_i/np.linalg.norm(v2_i)

    #     angle = np.arccos(np.clip(np.dot(v1_i, v2_i), -1.0, 1.0))
    #     angles.append(angle)

    # return angles

    dot_product = np.sum(v1 * v2, axis=1)
    norms = np.linalg.norm(v1, axis=1) * np.linalg.norm(v2, axis=1)
    cos_theta = dot_product / norms
    angle_radians = np.arccos(np.clip(cos_theta, -1.0, 1.0))
    angle_degrees = np.degrees(angle_radians)

    return angle_radians

def make_direction_cartesian(df_hits, thetaName='theta', phiName='phi', outputName='direction_cartesian'):
    df_hits[outputName] = df_hits.apply(lambda row: sph2cart(row[thetaName], row[phiName]), axis=1)
    return df_hits