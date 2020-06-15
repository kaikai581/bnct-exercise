#!/usr/bin/env python

import argparse
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

def main(infn):
    infpn = os.path.join('data', infn)
    pLi_xsec = pd.read_csv(infpn, sep=' ', skipinitialspace=True)
    # Select only records with energy larger than 0.75 MeV.
    pLi_xsec = pLi_xsec[pLi_xsec['E(MeV)'] >= .75]

    # Determine output file pathname
    ofn = infn.rstrip('.csv') + '_xsec.png'
    plot_all_and_ratio(pLi_xsec, 'proton cross section off Li', ofn)



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-i','--input', help='Input file name', default='pLi_QGSP_BIC_HP_0-50.csv')
    args = parser.parse_args()
    main(args.input)
