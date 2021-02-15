#!/usr/bin/env python
'''
From the script "energy_double_peak_clustring_comparison.py", it is found that spectral embedding without data preprocessing works best for this dataset.
This script trains a spectral embedding model and save labels back to the original dataset.
'''

from energy_double_peak_clustering_comparison import prepare_neutron_data
from sklearn import manifold
from sklearn.cluster import SpectralClustering
import argparse
import data_utilities
import os
import pandas as pd

if __name__ == '__main__':
    # command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_file', type=str, default=os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/neutron_preview.csv'))
    args = parser.parse_args()
    infpn = args.input_file

    # preprocess data
    X = prepare_neutron_data(infpn)

    # prepare model and write results back
    clusters = SpectralClustering(n_clusters=2, n_neighbors=10, random_state=0).fit_predict(X)
    df = pd.read_csv(infpn)
    df['cluster_babel_sc'] = clusters
    print(df)