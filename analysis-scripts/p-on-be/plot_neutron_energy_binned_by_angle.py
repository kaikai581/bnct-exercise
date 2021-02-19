#!/usr/bin/env python

# my own module

import data_utilities
import neutron_energy_binned_by_angle

from numpy import NaN
import argparse
import os
import pandas as pd
import seaborn as sns

def low_in_angles(range_str, angles):
    if range_str is NaN: return False
    low = range_str.strip('(').strip(']').split(',')[0]
    if float(low) in angles:
        return True
    return False

if __name__ == '__main__':
    # command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_file', type=str, default=os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/neutron_only_angle_bin.csv'))
    args = parser.parse_args()
    infpn = args.input_file
    
    # load data from file into a dataframe
    df_neutron = pd.read_csv(infpn)

    # angles at which data is binned
    angles = neutron_energy_binned_by_angle.angles
    half_idx = int(len(angles)/2)
    angles1 = angles[:half_idx]
    angles2 = angles[half_idx:]

    for i, angle_set in enumerate([angles1, angles2]):
        # add variable to select events of interest and select
        df_neutron['in_angle'] = df_neutron['angle_bin'].apply(low_in_angles, args=(angle_set,))
        df_angle_set = df_neutron[df_neutron.in_angle]

        algle_order = sorted(list(df_angle_set.angle_bin.dropna().unique()))
        # make kinetic energy plot binned by angle
        sns_plot = sns.histplot(x='ke', data=df_angle_set, hue='angle_bin', element='step', fill=False, hue_order=algle_order)
        sns_plot.figure.savefig('plots/neutron_kinetic_energy_angle_bin_{}.png'.format(i))
        sns_plot.figure.clf()