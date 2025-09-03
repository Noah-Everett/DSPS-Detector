import numpy as np
from scipy.spatial import cKDTree

def _ensure_array(x, dtype=np.float32):
    return np.asarray(x, dtype=dtype)

def _safe_mean(arr):
    return np.nan if arr.size == 0 else float(np.mean(arr))

def _eps_from_topk(pred, eps, optimize, minEntries):
    """
    If optimize=False: return eps.

    If optimize=True:
      - Take the top-`minEntries` values of `pred` (no full sort).
      - Let t = min(topk). Use eps_eff = min(eps, nextafter(t, -inf)) so that
        (pred > eps_eff) definitely includes all of those top-k values while
        preserving strict '>' semantics.
      - If there are fewer than k elements, use global min(pred) similarly,
        which effectively includes all elements.
    """
    if not optimize:
        return float(eps)

    p = np.ravel(pred)
    n = p.size
    if n == 0:
        return max(float(eps), 0.0)

    k = int(minEntries)
    if k <= 0:
        return max(float(eps), 0.0)

    if n >= k:
        # indices of the k largest elements (argpartition is O(n))
        idx = np.argpartition(p, n - k)[n - k:]
        t = float(np.min(p[idx]))
    else:
        # fewer than k entries: include everything
        t = float(np.min(p))

    # nudge down to keep strict '>' while including t
    t_open = np.nextafter(t, -np.inf)
    eps_eff = min(float(eps), t_open)
    return max(eps_eff, 0.0)

def sparseLoss(true, pred, threshold_true=1.0):
    """
    Two-region MSE normalized by region sizes (empty regions contribute 0).
    """
    true = _ensure_array(true)
    pred = _ensure_array(pred)

    se   = (true - pred) ** 2
    mask = (true > threshold_true)
    invm = ~mask

    n_in, n_out = int(mask.sum()), int(invm.sum())

    loss_in  = se[mask].sum() / n_in  if n_in  > 0 else 0.0
    loss_out = se[invm].sum() / n_out if n_out > 0 else 0.0
    return float(loss_in + loss_out)

def _min_dists_from_A_to_B_indices(A_inds, B_inds, pos):
    if A_inds.size == 0 or B_inds.size == 0:
        return np.zeros((0,), dtype=np.float32)
    tree = cKDTree(pos[B_inds])
    dists, _ = tree.query(pos[A_inds], k=1, workers=-1)  # parallel if SciPy>=1.6
    return dists.astype(np.float32, copy=False)

def distancesFromRecoToTrue(true, pred, eps=8, optimize=False, minEntries=20, grid_pos=None):
    """
    For each voxel with pred > eps* (after optional top-k adaptation), distance to nearest true>0 voxel.
    """
    true = _ensure_array(true)
    pred = _ensure_array(pred)
    pos  = grid_pos if grid_pos is not None else globals()["grid_pos"]

    true_1d = true.reshape(-1)
    pred_1d = pred.reshape(-1)

    eps_eff = _eps_from_topk(pred_1d, float(eps), bool(optimize), int(minEntries))

    A_inds = np.nonzero(pred_1d > eps_eff)[0]
    B_inds = np.nonzero(true_1d > 0.0)[0]

    return _min_dists_from_A_to_B_indices(A_inds, B_inds, pos)

def distancesFromTrueToReco(true, pred, eps=8, optimize=False, minEntries=20, grid_pos=None):
    return distancesFromRecoToTrue(pred, true, eps=eps, optimize=optimize,
                                   minEntries=minEntries, grid_pos=grid_pos)

def meanDistanceFromRecoToTrue(true, pred, eps=8, optimize=False, minEntries=20, grid_pos=None):
    d = distancesFromRecoToTrue(true, pred, eps=eps, optimize=optimize,
                                minEntries=minEntries, grid_pos=grid_pos)
    return _safe_mean(d)

def meanDistanceFromTrueToReco(true, pred, eps=8, optimize=False, minEntries=20, grid_pos=None):
    d = distancesFromTrueToReco(true, pred, eps=eps, optimize=optimize,
                                minEntries=minEntries, grid_pos=grid_pos)
    return _safe_mean(d)

def meanAndDistancesFromRecoToTrue(true, pred, eps=8, optimize=False, minEntries=20, grid_pos=None):
    d = distancesFromRecoToTrue(true, pred, eps=eps, optimize=optimize,
                                minEntries=minEntries, grid_pos=grid_pos)
    return _safe_mean(d), d

def meanAndDistancesFromTrueToReco(true, pred, eps=8, optimize=False, minEntries=20, grid_pos=None):
    d = distancesFromTrueToReco(true, pred, eps=eps, optimize=optimize,
                                minEntries=minEntries, grid_pos=grid_pos)
    return _safe_mean(d), d

def totalLoss(true, pred, eps=15, optimize=True, minEntries=15,
              alpha=0.10, beta=0.10, grid_pos=None, threshold_true=1.0):
    sl  = sparseLoss(true, pred, threshold_true=threshold_true)
    m_rt = meanDistanceFromRecoToTrue(true, pred, eps=eps, optimize=optimize,
                                      minEntries=minEntries, grid_pos=grid_pos)
    m_tr = meanDistanceFromTrueToReco(true, pred, eps=eps, optimize=optimize,
                                      minEntries=minEntries, grid_pos=grid_pos)
    m_rt = 0.0 if np.isnan(m_rt) else float(m_rt)
    m_tr = 0.0 if np.isnan(m_tr) else float(m_tr)
    return float(sl + alpha * m_rt + beta * m_tr)