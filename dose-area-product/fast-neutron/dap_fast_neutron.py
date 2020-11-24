#!/usr/bin/env python

# quiet down seaborn's FutureWarnings
import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

from scipy.interpolate import interp1d
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

df_nkerma = pd.read_csv('data/neutron_kerma_factor.dat', names=['energy', 'nkerma'], delim_whitespace=True)
# From Andrew's comments:
# Kerma factors given in the tables are in J/Kg (Gray) per particle per m^2. 
# To convert to rad*cm2, multiply these values by 10^6.
# So if converting to Gray*cm2, multiply by 10^4. ( 1 rad = 0.01 Gray) 
df_nkerma['nkerma'] = df_nkerma['nkerma']*1e4
# neutron sample from Ming-Feng's Monte Carlo
df_nspec = pd.read_csv('data/fast_neutron_fromuserdefinedNeutronPL0degree_10cmFe33cmMgF2_2cmTi3cmBi_BeamOn1e7_1mmdetec_1116_2020.txt', names=['energy'])

def neutron_kerma_function():
    return interp1d(df_nkerma['energy'], df_nkerma['nkerma'], kind='cubic', fill_value='extrapolate')

def neutron_spectrum_function(ax=None):
    x, y = sns.distplot(df_nspec['energy'], rug=True, hist=False, kde_kws={'clip': (0.0, float('inf'))}, ax=ax).get_lines()[0].get_data()
    ax.clear()
    return interp1d(x, y, kind='cubic', fill_value='extrapolate')

def plot_neutron_kerma(ax=None, logx=True, logy=True, xrange=None):
    ax = df_nkerma.plot(x='energy', y='nkerma', ax=ax)
    if logx:
        ax.set_xscale('log')
    if logy:
        ax.set_yscale('log')
    ax.set_xlabel('Neutron energy (MeV)')
    ax.set_ylabel('Neutron kerma\n'+r'factors (Gy-cm$^2$)')
    ax.get_legend().remove()
    # set x range
    if xrange:
        ax.set_xlim(xrange)
    # if this function is called standalone, plot it
    if not ax:
        plt.show()
        plt.close()

def plot_neutron_spectrum(ax1=None, xrange=None):
    # for samples with physical limits, here is the solution:
    # https://stackoverflow.com/questions/45911709/limit-the-range-of-x-in-seaborn-distplot-kde-estimation
    ax = sns.distplot(df_nspec['energy'], rug=True, hist=False, kde_kws={'clip': (0.0, float('inf'))}, ax=ax1)
    ax.set_title('fast neutron spectrum')
    ax.set_xlabel('Neutron energy (MeV)')
    # set x range
    if xrange:
        ax.set_xlim(xrange)
    # if this function is called standalone, plot it
    if not ax1:
        plt.show()
        plt.close()

def plot_weighted_kerma():
    '''
    This function make all informative plots.
    The neutron kerma function, the neutron spectrum, and the product of the two.
    '''
    _, axs = plt.subplots(3)
    plot_neutron_kerma(axs[0], False, False, xrange=(-0.1,4.1))
    plot_neutron_spectrum(axs[1], xrange=(-0.1,4.1))

    # plot the product function
    f_nkerma = neutron_kerma_function()
    f_nspec = neutron_spectrum_function(axs[2])
    x_prod = np.linspace(0, 4, 200)
    y_prod = f_nkerma(x_prod)*f_nspec(x_prod)
    axs[2].plot(x_prod, y_prod)
    axs[2].set_xlim((-0.1,4.1))
    axs[2].set_ylabel('Density '+r'$\times$'+'\nNeutron Kerma\n'+r'factors (Gy-cm$^2$)')

    # draw the results
    plt.tight_layout()
    plt.show()
    plt.close()

if __name__ == "__main__":
    print(df_nkerma)
    print(df_nspec)
    # plot_neutron_kerma()
    # plot_neutron_spectrum()
    plot_weighted_kerma()
