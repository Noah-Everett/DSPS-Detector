import numpy as np
import tqdm
import pandas as pd

def make_timeBins(df_hits, nBins=10, min=-1, max=1):
    if min >= 0:
        df_hits = df_hits[df_hits['time'] >= min].copy()
        df_hits.reset_index(drop=True, inplace=True)
    if max >= 0:
        df_hits = df_hits[df_hits['time'] <= max].copy()
        df_hits.reset_index(drop=True, inplace=True)

    df_hits['time_bin'] = pd.cut(df_hits['time'], bins=nBins, labels=False)
    df_hits['time_bin'] = df_hits['time_bin'].astype(int)

    return df_hits