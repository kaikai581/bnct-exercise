#!/usr/bin/env python
'''
This script takes the clustered data and make 1D histograms of each cluster.
'''

import argparse
import data_utilities
import matplotlib.pyplot as plt
import os
import pandas as pd
import seaborn as sns

def get_hist(ax):
    n, bins = [],[]
    for rect in ax.patches:
        ((x0, y0), (x1, y1)) = rect.get_bbox().get_points()
        n.append(y1-y0)
        bins.append(x0) # left edge of each bin
    bins.append(x1) # also get right edge of last bin

    return n, sorted(bins)

def plot_labeled_distribution_plain(vargroupname, varlist):
    ncols = len(varlist)
    fig, axs = plt.subplots(ncols=ncols, figsize=(5*ncols, 4))
    if ncols > 1:
        for i in range(ncols):
            _, bins, _ = axs[i].hist(df[varlist[i]], histtype='step', label='total', bins='auto', linewidth=2)
            axs[i].hist(df[df.cluster_label_se == 0][varlist[i]], histtype='stepfilled', label='cluster 0', bins=bins, alpha=.6)
            axs[i].hist(df[df.cluster_label_se == 1][varlist[i]], histtype='stepfilled', label='cluster 1', bins=bins, alpha=.6)
            axs[i].legend()
            axs[i].set_xlabel(vars[varlist[i]])
            axs[i].set_ylabel('count')
    else:
        _, bins, _ = axs.hist(df[varlist[0]], histtype='step', label='total', bins='auto', linewidth=2)
        axs.hist(df[df.cluster_label_se == 0][varlist[0]], histtype='stepfilled', label='cluster 0', bins=bins, alpha=.6)
        axs.hist(df[df.cluster_label_se == 1][varlist[0]], histtype='stepfilled', label='cluster 1', bins=bins, alpha=.6)
        axs.legend()
        axs.set_xlabel(vars[varlist[0]])
        axs.set_ylabel('count')
    
    outdir = 'plots/clustered_1d_vars'
    if not os.path.exists(outdir):
        os.makedirs(outdir)
    
    plt.tight_layout()
    plt.savefig(os.path.join(outdir, vargroupname + '.png'))
    plt.close()

    print(vargroupname, 'finished!')

def plot_labeled_distribution_with_sns(var, xlabel):
    ax = sns.histplot(x=var, data=df, hue=df.cluster_label_se)

    # get hist value and bins
    _, bins = get_hist(ax)
    plt.close()
    plt.hist(df[var], bins=bins, histtype='step', edgecolor='green', label='total')

    ax = sns.histplot(x=var, data=df, hue=df.cluster_label_se, element='step')
    ax.set(xlabel=xlabel, ylabel='counts')

    outdir = 'plots/clustered_1d_vars'
    if not os.path.exists(outdir):
        os.makedirs(outdir)
    plt.savefig(os.path.join(outdir, var + '_sns.png'))
    plt.close()

    print(var, 'finished!')

if __name__ == '__main__':
    # command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_file', type=str, default=os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/be8_neutrons_clustered.csv'))
    args = parser.parse_args()
    infpn = args.input_file

    # load the clustered data
    df = pd.read_csv(infpn)
    vars = dict()
    vars['track_id'] = 'neutron track ID'
    vars['posx'] = 'neutron vertex x-component (m)'
    vars['posy'] = 'neutron vertex y-component (m)'
    vars['px'] = 'neutron momemtum x-component (MeV)'
    vars['py'] = 'neutron momemtum y-component (MeV)'
    vars['pz'] = 'neutron momemtum z-component (MeV)'
    vars['ke'] = 'neutron kinetic energy (MeV)'
    vars['theta_p'] = 'neutron angle with respect to beam'
    vars['costheta_p'] = r'$\cos\theta_p$'
    vars['dirx'] = 'neutron direction x-component'
    vars['diry'] = 'neutron direction y-component'
    vars['dirz'] = 'neutron direction z-component'

    # posz seems to be causing memory leak!!!
    # disable it now!!!
    # vars['posz'] = 'neutron vertex z-component (m)'

    # loop through all variables and make plots
    for var, xlabel in vars.items():
        plot_labeled_distribution_with_sns(var, xlabel)
    
    var_group = dict()
    var_group['track_id'] = ['track_id']
    var_group['ke'] = ['ke']
    var_group['pos'] = ['posx', 'posy']
    var_group['momentum'] = ['px', 'py', 'pz']
    var_group['direction'] = ['dirx', 'diry', 'dirz']
    var_group['theta_p'] = ['theta_p']
    var_group['costheta_p'] = ['costheta_p']
    for vargroupname, varlist in var_group.items():
        plot_labeled_distribution_plain(vargroupname, varlist)
