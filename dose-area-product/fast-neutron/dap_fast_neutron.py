#!/usr/bin/env python

# quiet down seaborn's FutureWarnings
import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

from scipy.interpolate import interp1d
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
import scipy.integrate as integrate
import seaborn as sns

# load data
df_nkerma = pd.read_csv('data/neutron_kerma_factor.dat', names=['energy', 'nkerma'], delim_whitespace=True)
# From Andrew's comments:
# Kerma factors given in the tables are in J/Kg (Gray) per particle per m^2. 
# To convert to rad*cm2, multiply these values by 10^6.
# So if converting to Gray*cm2, multiply by 10^4. ( 1 rad = 0.01 Gray) 
df_nkerma['nkerma'] = df_nkerma['nkerma']*1e4
# fast neutron sample from Ming-Feng's Monte Carlo
df_fast_nspec = pd.read_csv('data/fast_neutron_fromuserdefinedNeutronPL0degree_10cmFe33cmMgF2_2cmTi3cmBi_BeamOn1e7_1mmdetec_1116_2020.txt', names=['energy'])
# all neutron sample from Ming-Feng's Monte Carlo
# the third column in the file is the neutron spectrum
df_nspec = pd.read_csv('data/neutron_fromuserdefinedNeutronPL0degree_10cmFe33cmMgF2_2cmTi3cmBi_BeamOn1e7_1mmdetec_1116_2020.txt', delim_whitespace=True, header=None)
df_nspec = df_nspec.iloc[:,[2]]
df_nspec.columns = ['energy']

# constant parameters
kde_bandwidth = 1e-1
xmax = max(list(df_nspec['energy'])) + kde_bandwidth

def fast_neutron_ratio(use_kde=True, kde_bw=kde_bandwidth):
    '''
    Calculate the fast neutron to epithermal neutron ratio, where the fast neutron flux is weighted by the kerma factor.
    Ref: p.18 equation in the paper https://core.ac.uk/download/pdf/82172536.pdf.
    '''
    f_nkerma = neutron_kerma_function()
    f_nspec = neutron_spectrum_function(use_kde=use_kde, kde_bw=kde_bw)
    f_prod = lambda x: f_nkerma(x)*f_nspec(x)

    # upper limit of integration
    xmax = max(list(df_nspec['energy'])) + kde_bw

    # integrate and divide
    num = integrate.quad(f_prod, 1e-2, min(xmax, 30))
    denum = integrate.quad(f_nspec, 5e-7, 1e-2)
    result = num[0]/denum[0]
    print('Kerma factor weighted fast neutron to epithermal neutron ratio: {:.2e}'.format(num[0]/denum[0]))

    return result


def hist_spec(x, hist, bin_edges):
    for i in range(len(hist)):
        if (x >= bin_edges[i]) and (x < bin_edges[i+1]):
            return hist[i]
    return 0

def neutron_kerma_function():
    return interp1d(df_nkerma['energy'], df_nkerma['nkerma'], kind='cubic', fill_value='extrapolate')

def neutron_spectrum_function(df=df_nspec, ax=None, use_kde=True, kde_bw=kde_bandwidth):
    if use_kde:
        x, y = sns.distplot(df['energy'], rug=True, hist=False, kde_kws={'clip': (0.0, float('inf')), 'bw_method': kde_bw}, ax=ax).get_lines()[0].get_data()
        if ax:
            ax.clear()
        return interp1d(x, y, kind='cubic', fill_value='extrapolate')
    # histogram method
    hist, bin_edges = np.histogram(df['energy'], bins=300, density=1)

    return lambda x: hist_spec(x, hist, bin_edges)

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

def plot_neutron_spectrum(df=df_nspec, ax=None, xrange=None, use_kde=True, logy=False, kde_bw=kde_bandwidth):
    # for samples with physical limits, here is the solution:
    # https://stackoverflow.com/questions/45911709/limit-the-range-of-x-in-seaborn-distplot-kde-estimation
    if use_kde:
        ax1 = sns.distplot(df['energy'], rug=True, hist=False, kde_kws={'clip': (0.0, float('inf')), 'bw_method': kde_bw}, ax=ax)
    else:
        ax1 = df.plot.hist(bins=300, density=1, histtype='step', ax=ax)
    ax1.set_title('fast neutron spectrum')
    ax1.set_ylabel('Density')
    ax1.set_xlabel('Neutron energy (MeV)')
    if ax1.get_legend():
        ax1.get_legend().remove()
    # set x range
    if xrange:
        ax1.set_xlim(xrange)
    if logy:
        ax1.set_yscale('log')
    # if this function is called standalone, plot it
    if not ax:
        plt.show()
        plt.close()

def plot_weighted_kerma(use_kde=False, logy=True):
    '''
    This function makes all informative plots.
    The neutron kerma function, the neutron spectrum, and the product of the two.
    '''
    _, axs = plt.subplots(2, 2)
    plot_neutron_kerma(ax=axs[0,0], logx=False, logy=False, xrange=(-0.1,xmax))
    plot_neutron_spectrum(ax=axs[0,1], xrange=(-0.1,xmax), use_kde=use_kde, logy=True)

    # plot the product function
    f_nkerma = neutron_kerma_function()
    f_nspec = neutron_spectrum_function(use_kde=use_kde)
    x_prod = np.linspace(0, xmax, 1001)
    y_prod = f_nkerma(x_prod)*np.array([f_nspec(xi) for xi in x_prod])
    axs[1,0].plot(x_prod, y_prod)
    axs[1,0].set_xlim((-0.1,xmax))
    axs[1,0].set_ylabel('Density '+r'$\times$'+'\nNeutron Kerma\n'+r'factors (Gy-cm$^2$)')
    if logy:
        axs[1,0].set_yscale('log')
    axs[1,0].set_xlabel('Neutron energy (MeV)')

    # Writing results to the last subplot
    form_str = r'$\frac{\int_{10keV}^{30MeV} F(E)K(E) \,dE}{\int_{0.5eV}^{10keV} F(E) \,dE}$'
    ratio = fast_neutron_ratio(use_kde=use_kde)
    axs[1,1].clear()
    axs[1,1].text(.3, .4, form_str+'\n={:.2e}'.format(ratio))

    # draw the results
    out_dir = 'plots'
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    out_fpn = os.path.join(out_dir, 'kerma+flux+product_kde.png' if use_kde else 'kerma+flux+product_no_kde.png')
    plt.tight_layout()
    plt.savefig(out_fpn)
    plt.close()

if __name__ == "__main__":
    plot_weighted_kerma(use_kde=False)
    plot_weighted_kerma(use_kde=True)
