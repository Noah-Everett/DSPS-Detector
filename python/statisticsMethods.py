import numpy as np
import tqdm
import matplotlib.pyplot as plt

def get_PDF(data, nBins=100, bins=None):
    if bins is None:
        hist, bins = np.histogram(data, bins=nBins)
    else:
        hist, bins, _ = plt.hist(data, bins=bins)
    cumulative_sum = np.cumsum(hist)
    PDF = cumulative_sum/cumulative_sum[-1]
    scaled_hist = hist / cumulative_sum[-1]
    plt.clf()

    return PDF, hist, scaled_hist, bins

def get_CSP(probability, data=None, PDF=None, bins=None, nBins=100):
    if PDF is None or bins is None:
        if data is None:
            raise ValueError('Either data or PDF and bins must be provided.')
        else:
            PDF, _ = get_PDF(data, nBins=nBins)

    if probability == '1 sigma':
        probability = 0.682689492137086
    elif probability == '2 sigma':
        probability = 0.954499736103642
    elif probability == '3 sigma':
        probability = 0.997300203936740

    return bins[np.argmin(np.abs(PDF - probability))]
