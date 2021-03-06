#!/usr/bin/env python
'''
This script takes as input preprocessed data prepared with the data_utilities.py script and make all kinds of distributions for exploratory analysis.
'''

import matplotlib
from matplotlib.pyplot import xlabel
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import argparse
import data_utilities
import os
import pandas as pd
import seaborn as sns
import sys

class neutron_from_target:
    '''
    A class for neutron data analysis.
    '''
    def __init__(self, infpn, include_other_daughters=False):
        '''
        Constructor to load data into pandas dataframes.
        '''
        fext = os.path.splitext(infpn)[1]
        if fext == '.csv':
            self.df_neutron = pd.read_csv(infpn)
        elif fext == '.h5':
            self.df_neutron = pd.read_hdf(infpn, key='secondary')
        else:
            print('File format not supported. Use only .csv or .h5.')
            sys.exit(-1)
        # include or not other daughter particles produced with neutrons
        if not include_other_daughters:
            self.df_neutron = self.df_neutron[self.df_neutron.particle_name == 'neutron']
        
        # neutrons generated by secondaries are group into one category
        self.group_equations()
    
    def group_equations(self):
        '''
        Check if neutrons belong to simplified categories and designate.
        '''
        self.df_neutron['nuclear process'] = self.df_neutron['equation'].apply(self.simplify_process_categories)
    
    def plot_1d_distribution(self, vargroupname, varlist):
        ncols = len(varlist)
        _, axs = plt.subplots(ncols=ncols, figsize=(5*ncols, 4))
        if ncols > 1:
            for i in range(ncols):
                sns.histplot(data=self.df_neutron, x=varlist[i], ax=axs[i], element='step')
                axs[i].set_xlabel(vars[varlist[i]])
                axs[i].set_ylabel('count')
        else:
            sns.histplot(data=self.df_neutron, x=varlist[0], ax=axs, element='step')
            axs.set_xlabel(vars[varlist[0]])
            axs.set_ylabel('count')
        
        outdir = 'plots/1d_vars'
        if not os.path.exists(outdir):
            os.makedirs(outdir)
        
        plt.tight_layout()
        plt.savefig(os.path.join(outdir, vargroupname + '.png'))
        plt.close()

        print(vargroupname, 'finished!')

    def simplify_process_categories(self, eqn_str):
        '''
        Since it is quite cumbersome to track processes in which neutrons are produced by secondary particles,
        they are simply grouped into one category called "neutrons produced by secondaries".
        '''
        _, daughters = data_utilities.parse_equation(eqn_str)
        if not 'neutron' in daughters:
            return 'neutrons produced by secondaries'
        return eqn_str

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_file', type=str, default=os.path.join(data_utilities.get_git_root(__file__), 'data/p_on_be/pandas/neutron_preview.csv'))
    args = parser.parse_args()
    infpn = args.input_file

    # configure pandas to print all rows
    # pd.set_option('display.max_rows', None)

    # load dataframe
    ndata = neutron_from_target(infpn)
    # print(ndata.df_neutron)

    # create output folder
    outdir = 'plots'
    if not os.path.exists(outdir):
        os.makedirs(outdir)

    # plot nuclear process vs neutron kinetic energy
    sns_plot = sns.catplot(x='ke', y='equation', kind='boxen', data=ndata.df_neutron.sort_values('equation'))
    sns_plot.set(xlabel='neutron kinetic energy (MeV)', ylabel='nuclear process')
    sns_plot.savefig('plots/neutron_full_category_vs_kinetic_energy.png')
    sns_plot = sns.catplot(x='ke', y='nuclear process', kind='boxen', data=ndata.df_neutron.sort_values('nuclear process'))
    sns_plot.set(xlabel='neutron kinetic energy (MeV)', ylabel='nuclear process')
    sns_plot.savefig('plots/neutron_simplified_category_vs_kinetic_energy.png')

    # plot neutron kinetic energy by category
    sns_plot = sns.displot(ndata.df_neutron, x='ke', hue='nuclear process', element='step')
    sns_plot.set(xlabel='neutron kinetic energy (MeV)', ylabel='counts')
    sns_plot.savefig('plots/neutron_kinetic_energy_by_simplified_category.png')

    # plot 1d distributions
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
    var_group = dict()
    var_group['track_id'] = ['track_id']
    var_group['ke'] = ['ke']
    var_group['pos'] = ['posx', 'posy']
    var_group['momentum'] = ['px', 'py', 'pz']
    var_group['direction'] = ['dirx', 'diry', 'dirz']
    var_group['theta_p'] = ['theta_p']
    var_group['costheta_p'] = ['costheta_p']
    for vargroupname, varlist in var_group.items():
        ndata.plot_1d_distribution(vargroupname, varlist)