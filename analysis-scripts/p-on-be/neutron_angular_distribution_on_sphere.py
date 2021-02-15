#!/usr/bin/env python
'''
This script plots the neutron directions on a unit sphere.
'''

# my own module
import data_utilities

from mpl_toolkits.mplot3d import axes3d
import argparse
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd

if __name__ == '__main__':
    # command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_file', type=str, default=os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/neutron_preview.csv'))
    args = parser.parse_args()
    infpn = args.input_file

    # load data from file into a dataframe
    df_neutron = pd.read_csv(infpn)
    df_neutron = df_neutron[df_neutron.particle_name == 'neutron']
    df_neutron = df_neutron.sample(n=1000, random_state=1)
    
    # prepare canvas
    fig, ax = plt.subplots(1, 1, subplot_kw={'projection':'3d', 'aspect':'auto'})

    # prepare globe
    phi = np.linspace(0, np.pi, 20)
    theta = np.linspace(0, 2 * np.pi, 40)
    x = np.outer(np.sin(theta), np.cos(phi))
    y = np.outer(np.sin(theta), np.sin(phi))
    z = np.outer(np.cos(theta), np.ones_like(phi))
    ax.plot_wireframe(x, y, z, color='k', rstride=1, cstride=1)

    # plot results
    ax.scatter(df_neutron['dirx'], df_neutron['diry'], df_neutron['dirz'], s=1.5, c='r', zorder=10)
    plt.savefig('plots/n_dir_sphere.png')