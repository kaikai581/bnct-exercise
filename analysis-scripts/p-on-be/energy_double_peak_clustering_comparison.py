#!/usr/bin/env python
'''
In the neutron kinetic energy spectrum, there is 2-peak structure below 2 MeV.
This two peak structure is due to processes involving Be8.
This script employs unsupervised learning methods to see if anything can be found.
'''

# template script: sklearn's official website
# https://scikit-learn.org/stable/auto_examples/manifold/plot_compare_methods.html#sphx-glr-auto-examples-manifold-plot-compare-methods-py

from collections import OrderedDict
from functools import partial
from matplotlib.ticker import NullFormatter
from sklearn import manifold
from sklearn.preprocessing import StandardScaler
from time import time
import argparse
import data_utilities
import matplotlib.pyplot as plt
import os
import pandas as pd

def prepare_neutron_data(infpn, output_preview=False):
    # load the neutron dataframe
    ndata = pd.read_csv(infpn)

    # select only neutrons produced by the interaction
    # proton + Be9 --> proton + neutron + Be8
    ndata = ndata[(ndata.particle_name == 'neutron') & (ndata.equation == 'proton + Be9 --> proton + neutron + Be8')]
    
    if output_preview:
        ndata.to_csv(os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/be8_neutrons.csv'))
    
    # track_id is categorical. Might be useful, but skip for now.
    # Be9 is also categorical and skipped for now.
    sel_cols = ['posx', 'posy', 'posz', 'px', 'py', 'pz', 'ke']

    return ndata[sel_cols]

if __name__ == '__main__':
    # command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_file', type=str, default=os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/neutron_preview.csv'))
    args = parser.parse_args()
    infpn = args.input_file

    # preprocess data
    X = prepare_neutron_data(infpn, output_preview=True)
    print('Before standardization')
    print(X)
    standarized_X = StandardScaler().fit_transform(X)
    print('After standardization')
    print(standarized_X)

    # Create figure
    fig = plt.figure(figsize=(12, 8))
    fig.suptitle('Manifold Learning comparison', fontsize=14)

    # Set-up manifold methods
    n_neighbors = 10
    n_components = 2
    LLE = partial(manifold.LocallyLinearEmbedding, n_neighbors=n_neighbors, n_components=n_components, eigen_solver='auto')

    methods = OrderedDict()
    # With Hessian, I got the following error. As a result, I added the dense option.
    '''
    ValueError: Error in determining null-space with ARPACK. Error message: 'Factor is exactly singular'. Note that eigen_solver='arpack' can fail when the weight matrix is singular or otherwise ill-behaved. In that case, eigen_solver='dense' is recommended. See online documentation for more information.
    '''
    methods['LLE'] = LLE(method='standard', eigen_solver='dense')
    methods['LTSA'] = LLE(method='ltsa', eigen_solver='dense')
    methods['Hessian LLE'] = LLE(method='hessian', eigen_solver='dense')
    methods['Modified LLE'] = LLE(method='modified')
    methods['Isomap'] = manifold.Isomap(n_neighbors=n_neighbors, n_components=n_components)
    methods['MDS'] = manifold.MDS(n_components, max_iter=100, n_init=1)
    methods['SE'] = manifold.SpectralEmbedding(n_components=n_components,
                                               n_neighbors=n_neighbors)
    methods['t-SNE'] = manifold.TSNE(n_components=n_components, init='pca',
                                     random_state=0)
    
    # Plot results
    datasets = dict()
    datasets['no_data_preprocessing'] = X
    datasets['data_standardized'] = standarized_X
    for preprocessing, dataset in datasets.items():
        for i, (label, method) in enumerate(methods.items()):
            t0 = time()
            Y = method.fit_transform(dataset)
            t1 = time()
            print("%s: %.2g sec" % (label, t1 - t0))
            ax = fig.add_subplot(2, 4, 1 + i)
            ax.scatter(Y[:, 0], Y[:, 1], cmap=plt.cm.Spectral, s=1)
            ax.set_title("%s (%.2g sec)" % (label, t1 - t0))
            ax.xaxis.set_major_formatter(NullFormatter())
            ax.yaxis.set_major_formatter(NullFormatter())
            ax.axis('tight')

        plt.savefig('plots/clustering_visualization_' + preprocessing + '.png')
        plt.clf()
