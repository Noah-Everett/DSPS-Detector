import numpy as np
import tqdm
from scipy.spatial.distance import cdist
import pandas as pd

def filter_r(df_hits, r_max):
    df_hits = df_hits[df_hits['r'] <= r_max].copy()
    df_hits.reset_index(drop=True, inplace=True)

    return df_hits

# def filter_hits(df_hits, radius, number):
#     df_hits['closeHits_count'] = 0
#     relative_positions = np.vstack(df_hits['relativePosition_nBin'].to_numpy())
#     photosensor_ids = df_hits['sensor_name'].to_numpy()

#     # Calculate distances using scipy's cdist
#     distances = cdist(relative_positions, relative_positions, metric='chebyshev')

#     # Ignore the distance to the hit itself
#     np.fill_diagonal(distances, np.inf)

#     # Broadcast the sensor_ids to match the shape of distances
#     same_sensor = (photosensor_ids[:, np.newaxis] == photosensor_ids)

#     # Count the number of close hits within the specified radius on the same sensor
#     close_hits_count = np.sum((distances <= radius) & same_sensor, axis=1)
    
#     # Update the 'closeHits_count' column
#     df_hits['closeHits_count'] = close_hits_count

#     # Filter hits based on the condition
#     df_hits = df_hits[df_hits['closeHits_count'] >= number].copy()
#     df_hits.reset_index(drop=True, inplace=True)

#     return df_hits

def filter_hits(df_hits, radius, number):
    df_hits_filtered = pd.DataFrame()

    for sensor_name, hits_group in df_hits.groupby('sensor_name'):
        relative_positions = np.array(hits_group['relativePosition_nBin'].to_list())
        relative_positions = np.atleast_2d(relative_positions)

        distances = cdist(relative_positions, relative_positions, metric='chebyshev')

        np.fill_diagonal(distances, np.inf)

        close_hits_count = np.sum((distances <= radius), axis=1)

        hits_group_filtered = hits_group[close_hits_count >= number]
        df_hits_filtered = pd.concat([df_hits_filtered, hits_group_filtered])

    df_hits_filtered.reset_index(drop=True, inplace=True)

    return df_hits_filtered


# def filter_hits(df_hits, radius, number):
#     df_hits['closeHits_count'] = 0
#     relative_positions = np.vstack(df_hits['relativePosition_nBin'].to_numpy())
#     photosensor_ids = df_hits['sensor_name'].to_numpy()

#     # Create a boolean matrix indicating whether hits are on the same sensor
#     same_sensor = (photosensor_ids[:, np.newaxis] == photosensor_ids)

#     # Calculate distances only for hits on the same sensor
#     distances = cdist(relative_positions[same_sensor], relative_positions[same_sensor], metric='chebyshev')

#     # Create a matrix to store distances for all hits (zeros for hits not on the same sensor)
#     all_distances = np.zeros_like(same_sensor, dtype=float)
#     all_distances[same_sensor] = distances.flatten()

#     # Reshape distances back to the original shape
#     distances = all_distances.reshape(same_sensor.shape)

#     # Ignore the distance to the hit itself
#     np.fill_diagonal(distances, np.inf)

#     # Count the number of close hits within the specified radius on the same sensor
#     close_hits_count = np.sum((distances <= radius) & same_sensor, axis=1)
    
#     # Update the 'closeHits_count' column
#     df_hits['closeHits_count'] = close_hits_count

#     # Filter hits based on the condition
#     df_hits = df_hits[df_hits['closeHits_count'] >= number].copy()
#     df_hits.reset_index(drop=True, inplace=True)

#     return df_hits