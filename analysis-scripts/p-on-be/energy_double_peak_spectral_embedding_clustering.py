#!/usr/bin/env python
'''
From the script "energy_double_peak_clustring_comparison.py", it is found that spectral embedding without data preprocessing works best for this dataset.
This script trains a spectral embedding model and save labels back to the original dataset.
'''

from energy_double_peak_clustering_comparison import prepare_neutron_data
from sklearn import manifold
from sklearn.cluster import KMeans
import argparse
import data_utilities
import matplotlib.pyplot as plt
import os
import pandas as pd

if __name__ == '__main__':
    # command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_file', type=str, default=os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/neutron_preview.csv'))
    args = parser.parse_args()
    infpn = args.input_file

    # preprocess data
    X = prepare_neutron_data(infpn, output_preview=True)

    # perform spectral embedding for k-means to cluster
    X_embed = manifold.SpectralEmbedding(n_components=2, n_neighbors=10, random_state=0).fit_transform(X)

    # perform k-means clustering
    y_pred = KMeans(n_clusters=2, random_state=0).fit_predict(X_embed)

    # plot results
    plt.scatter(X_embed[:, 0], X_embed[:, 1], c=y_pred, s=1)

    plt.savefig('plots/spectral_embedding_kmeans_clustering.png')

    # write labels back to the original data
    outfpn = os.path.join(os.path.dirname(infpn), 'be8_neutrons.csv')
    df = pd.read_csv(outfpn)
    df['cluster_label_se'] = y_pred
    outfpn = os.path.splitext(outfpn)
    outfpn = outfpn[0] + '_clustered' + outfpn[1]
    df.to_csv(outfpn)