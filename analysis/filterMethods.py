import numpy as np
import tqdm
from scipy.spatial.distance import cdist

def filter_r(df_hits, r_max):
    df_hits = df_hits[df_hits['r'] <= r_max].copy()
    df_hits.reset_index(drop=True, inplace=True)

    return df_hits


def filter_hits(df_hits, radius, number):
    df_hits['closeHits_count'] = 0
    relative_positions = np.vstack(df_hits['relativePosition_nBin'].to_numpy())
    photosensor_ids = df_hits['sensor_name'].to_numpy()

    # Calculate distances using scipy's cdist
    distances = cdist(relative_positions, relative_positions, metric='chebyshev')

    # Ignore the distance to the hit itself
    np.fill_diagonal(distances, np.inf)

    # Broadcast the sensor_ids to match the shape of distances
    same_sensor = (photosensor_ids[:, np.newaxis] == photosensor_ids)

    # Count the number of close hits within the specified radius on the same sensor
    close_hits_count = np.sum((distances <= radius) & same_sensor, axis=1)
    
    # Update the 'closeHits_count' column
    df_hits['closeHits_count'] = close_hits_count

    # Filter hits based on the condition
    df_hits = df_hits[df_hits['closeHits_count'] >= number].copy()
    df_hits.reset_index(drop=True, inplace=True)

    return df_hits
