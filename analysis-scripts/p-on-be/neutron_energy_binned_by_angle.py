#!/usr/bin/env python

# my own module
import data_utilities

import argparse
import numpy as np
import os
import pandas as pd

# angles at which data is binned
angles = [0., 20., 40., 60., 80., 90., 100., 120.]

def get_angle_edges(angles):
    '''
    Given engle values as one of the bin edges,
    return angle bin edges based on the angles of which the cosines have the same bin width.
    '''
    angles = np.array(sorted(angles))*np.pi/180.
    cosines = np.cos(angles)
    min_cos_interval = min([abs(cosines[i+1]-cosines[i]) for i in range(len(cosines)-1)])
    # since angle is in ascending order, cos is in decending order
    cosine_edges = list(cosines)
    for cos in list(cosines):
        edge = cos-min_cos_interval
        if not edge in cosine_edges:
            cosine_edges.append(cos-min_cos_interval)
    return sorted(np.round(np.arccos(np.array(cosine_edges))*180/np.pi, 4))

if __name__ == '__main__':
    # command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_file', type=str, default=os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/neutron_preview.csv'))
    args = parser.parse_args()
    infpn = args.input_file

    # load data from file into a dataframe
    df_neutron = pd.read_csv(infpn)
    df_neutron = df_neutron[df_neutron.particle_name == 'neutron']
    df_neutron['theta_p_degree'] = df_neutron.theta_p*180/np.pi

    angle_edges = get_angle_edges(angles)
    
    # add binning field
    df_neutron['angle_bin'] = pd.cut(df_neutron.theta_p_degree, angle_edges)
    df_neutron.to_csv(os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/neutron_only_angle_bin.csv'))