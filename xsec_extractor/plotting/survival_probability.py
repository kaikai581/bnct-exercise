#!/usr/bin/env python

from scipy.constants import N_A
from scipy.interpolate import interp1d
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd

def get_xsec_interpolation(df):
    xsecx = df['E(MeV)'].to_numpy()
    xsecy = df['Total(b)'].to_numpy()
    return interp1d(xsecx, xsecy)

def main():
    pLi_xsec = pd.read_csv('data/pLi_QGSP_BIC_HP.csv', sep=' ', skipinitialspace=True)
    # Select only records with energy larger than 0.75 MeV.
    pLi_xsec = pLi_xsec[pLi_xsec['E(MeV)'] >= .75]

    # Interpolate cross section
    xsec_interpolated = get_xsec_interpolation(pLi_xsec)
    
    # Proton energies of interest
    proton_energies = [1, 5, 10]

    # Constants
    density = 0.534 # g/cm3
    M_A = 6.940 # g/mole
    n_density = N_A/M_A*density

    # Mean free paths
    for p_en in proton_energies:
        print(1./n_density/(xsec_interpolated(p_en)*1e-24))


if __name__ == '__main__':
    main()
