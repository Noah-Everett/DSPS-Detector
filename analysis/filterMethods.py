import numpy as np
import tqdm

def filter_r(df_hits, r_max):
    df_hits = df_hits[df_hits['r'] <= r_max].copy()
    df_hits.reset_index(drop=True, inplace=True)

    return df_hits

def filter_hits(df_hits, radius, number):
    df_hits['closeHits_count'] = 0
    relative_positions = np.vstack(df_hits['relativePosition_nBin'].values)
    photosensor_ids = df_hits['sensor_name'].values

    for index, row in tqdm.tqdm(df_hits.iterrows(), total=len(df_hits)):
        x, y = row['relativePosition_nBin']
        photosensor_id = row['sensor_name']

        same_photosensor = (photosensor_ids == photosensor_id)
        
        distances = np.max(np.abs(relative_positions[same_photosensor] - np.array([x, y])), axis=1)
        df_hits.at[index, 'closeHits_count'] = np.sum(distances <= radius) - 1

    df_hits = df_hits[df_hits['closeHits_count'] >= number].copy()
    df_hits.reset_index(drop=True, inplace=True)

    return df_hits