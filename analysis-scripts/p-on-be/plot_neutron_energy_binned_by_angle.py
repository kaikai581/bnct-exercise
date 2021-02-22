#!/usr/bin/env python

# redirect output to avoid x11 issues
import matplotlib
matplotlib.use('Agg')

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
    parser.add_argument('--equal_angle_bin', action='store_true')
    parser.add_argument('--separate_plots', action='store_true')
    args = parser.parse_args()
    infpn = args.input_file
    
    # load data from file into a dataframe
    df_neutron = pd.read_csv(infpn)

    # angles at which data is binned
    if args.equal_angle_bin:
        angles = neutron_energy_binned_by_angle.angles_equal_angle_bin
    else:
        angles = neutron_energy_binned_by_angle.angles
    
    # select the cut variable
    cut_var = 'angle_bin_equal_cos'
    if args.equal_angle_bin: cut_var = 'angle_bin_equal_angle'
    # make plots with multiple angles in one figure
    if not args.separate_plots:
        half_idx = int(len(angles)/2)
        angles1 = angles[:half_idx]
        angles2 = angles[half_idx:]

        for i, angle_set in enumerate([angles1, angles2]):
            # add variable to select events of interest and select
            df_neutron['in_angle'] = df_neutron[cut_var].apply(low_in_angles, args=(angle_set,))
            df_angle_set = df_neutron[df_neutron.in_angle]

            algle_order = sorted(list(df_angle_set[cut_var].dropna().unique()))
            # make kinetic energy plot binned by angle
            sns_plot = sns.histplot(x='ke', data=df_angle_set, hue=cut_var, element='step', fill=False, hue_order=algle_order)
            sns_plot.set(xlabel='Neutron kinetic energy (MeV)')
            sns_plot.set(xlim=(0, 5))
            sns_plot.figure.savefig('plots/neutron_kinetic_energy_angle_bin_{}.png'.format(i))
            sns_plot.figure.clf()
    else: # one angle per figure
        df_neutron['in_angle'] = df_neutron[cut_var].apply(low_in_angles, args=(angles,))
        df_angle_set = df_neutron[df_neutron.in_angle]

        angle_ranges = df_neutron[cut_var].unique()
        for angle_range in angle_ranges:
            df_angle_range = df_angle_set[df_angle_set[cut_var] == angle_range]
            if df_angle_range.empty: continue
            sns_plot = sns.histplot(x='ke', data=df_angle_range, hue=cut_var, element='step', fill=False)
            sns_plot.set(xlabel='Neutron kinetic energy (MeV)')
            sns_plot.set(xlim=(0, 5))
            sns_plot.figure.savefig('plots/neutron_kinetic_energy_angle_bin_{}.png'.format(angle_range))
            sns_plot.figure.clf()

