#!/usr/bin/env python

import matplotlib.pyplot as plt
import os
import pandas as pd

def plot_all_and_ratio(df_xsec, desc, fn):
    fig, axex = plt.subplots(2, sharex=True)
    axex[0] = df_xsec.plot(x='E(MeV)', y='Total(b)', alpha=0.4, c='magenta', lw=4, ax=axex[0])
    df_xsec.plot(x='E(MeV)', y=['Elastic(b)','Inelastic(b)'], ax=axex[0])
    axex[0].set_ylabel(r'cross section (barn, $10^{-24}$cm$^2$)')
    axex[0].set_title(desc)
    axex[0].grid(axis='y')

    # Make a ratio data frame
    df_frac = pd.DataFrame()
    df_frac['E(MeV)'] = df_xsec['E(MeV)']
    df_frac['elastic/inelastic'] = df_xsec['Elastic(b)'].divide(df_xsec['Inelastic(b)'])
    axex[1] = df_frac.plot(x='E(MeV)', y='elastic/inelastic', alpha=0.8, ax=axex[1])
    axex[1].set_xlabel('proton kinetic energy (MeV)')
    axex[1].set_ylabel('elastic to inelastic ratio')
    axex[1].set_yscale('log')
    axex[1].grid(axis='y')
    plt.savefig(os.path.join('plots', fn))

def main():
    pLi_xsec = pd.read_csv('pLi_QGSP_BIC_HP.csv', sep=' ', skipinitialspace=True)
    # Select only records with energy larger than 0.75 MeV.
    pLi_xsec = pLi_xsec[pLi_xsec['E(MeV)'] >= .75]
    plot_all_and_ratio(pLi_xsec, 'proton cross section off Li', 'pLi_xsec.png')
    # plot_weighted_mean(nC_xsec, nH_xsec, 'weighted_mean.png')


if __name__ == '__main__':
    main()
