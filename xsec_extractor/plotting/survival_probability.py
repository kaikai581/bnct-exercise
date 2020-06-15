#!/usr/bin/env python

from scipy.constants import N_A
from scipy.interpolate import interp1d
import math
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd

def get_xsec_interpolation(df):
    xsecx = df['E(MeV)'].to_numpy()
    xsecy = df['Total(b)'].to_numpy()*1e-24
    return interp1d(xsecx, xsecy)

def mean_free_path(xsec):
    return 1./n_density/xsec

def plot_mean_free_path(xsec_interpolated):
    x = np.linspace(1, 10, 91)
    xsec = np.array(list(map(xsec_interpolated, x)))
    y = np.array(list(map(mean_free_path, xsec)))
    plt.figure(0)
    plt.xlabel('proton kinetic energy (MeV)')
    plt.ylabel('mean free path (cm)')
    plt.plot(x, y, '-')
    plt.savefig('plots/mean_free_path.png')
    return

def plot_survival_probability(xsec_interpolated):
    l = np.logspace(-6, 2, 81)

    plt.figure(1)
    plt.xlim(1e-7, 1e3)
    plt.xlabel('target thickness (cm)')
    plt.ylabel('interaction probability')
    plt.xscale('log')
    plt.yscale('log')
    plt.grid()
    for p_en in [1, 5, 10]:
        xsec = xsec_interpolated(p_en)
        y = np.array([1-survival_probability(xsec, th) for th in list(l)])
        plt.plot(l, y, '-', label=r'p $E_k$ {} MeV'.format(p_en))
    plt.legend()
    plt.savefig('plots/interaction_probability.png')
    return

def survival_probability(xsec, thickness):
    return math.exp(-n_density*xsec*thickness)

def main():
    pLi_xsec = pd.read_csv('data/pLi_QGSP_BIC_HP.csv', sep=' ', skipinitialspace=True)
    # Select only records with energy larger than 0.75 MeV.
    pLi_xsec = pLi_xsec[pLi_xsec['E(MeV)'] >= .75]

    # Interpolate cross section
    xsec_interpolated = get_xsec_interpolation(pLi_xsec)
    
    # Proton energies of interest
    proton_energies = [1, 5, 10]

    # Mean free paths
    # for p_en in proton_energies:
    #     print(1./n_density/xsec_interpolated(p_en))
    plot_mean_free_path(xsec_interpolated)

    # Survival probability
    plot_survival_probability(xsec_interpolated)


if __name__ == '__main__':
    # File scale constants
    density = 0.534 # g/cm3
    M_A = 6.940 # g/mole
    n_density = N_A/M_A*density

    main()
