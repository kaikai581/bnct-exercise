import matplotlib.pyplot as plt
import pandas as pd

def compare_all(df_xsec, desc, fn):
    fig, axex = plt.subplots(2, sharex=True)
    axex[0] = df_xsec.plot(x='E(MeV)', y='Total(b)', alpha=0.4, c='magenta', lw=4, ax=axex[0])
    df_xsec.plot(x='E(MeV)', y=['Elastic(b)','Inelastic(b)','Capture(b)'], ax=axex[0])
    axex[0].set_ylabel(r'cross section (barn, $10^{-24}$cm$^2$)')
    axex[0].set_title(desc)
    axex[0].grid(axis='y')

    # Make a ratio data frame
    df_frac = pd.DataFrame()
    df_frac['E(MeV)'] = df_xsec['E(MeV)']
    for xn in ['Elastic(b)','Inelastic(b)','Capture(b)']:
        df_frac[xn] = df_xsec[xn]/df_xsec['Total(b)']
    axex[1] = df_frac.plot(x='E(MeV)', y=['Elastic(b)','Inelastic(b)','Capture(b)'], alpha=0.8, ax=axex[1])
    axex[1].set_xlabel('neutron kinetic energy (MeV)')
    axex[1].set_ylabel('fraction of total')
    axex[1].grid(axis='y')
    plt.savefig(fn)

def plot_weighted_mean(df_C, df_H, fn):
    # weight fractions
    wf_C = 0.922577329294378
    wf_H = 0.0774226707056217
    # number density ratio
    nd_C_over_nd_H = (wf_C/wf_H)*(1./12)
    # number density fraction
    nf_C = nd_C_over_nd_H/(nd_C_over_nd_H+1)
    nf_H = 1/(nd_C_over_nd_H+1)
    
    # make a data frame storing the weighted mean
    wm_xsec = pd.DataFrame()
    wm_xsec['E(MeV)'] = df_C['E(MeV)']
    wm_xsec['H_comp(b)'] = df_H['Total(b)']*nf_H
    wm_xsec['C_comp(b)'] = df_C['Total(b)']*nf_C
    wm_xsec['Total(b)'] = wm_xsec['H_comp(b)']+wm_xsec['C_comp(b)']
    # Make a ratio data frame
    df_frac = pd.DataFrame()
    df_frac['E(MeV)'] = wm_xsec['E(MeV)']
    for xn in ['H_comp(b)','C_comp(b)']:
        df_frac[xn] = wm_xsec[xn]/wm_xsec['Total(b)']

    # make plot
    fig, axex = plt.subplots(2, sharex=True)
    axex[0] = wm_xsec.plot(x='E(MeV)', y='Total(b)', alpha=0.4, c='magenta', lw=4, ax=axex[0])
    wm_xsec.plot(x='E(MeV)', y=['H_comp(b)', 'C_comp(b)'], ax=axex[0])
    axex[0].set_ylabel(r'cross section (barn, $10^{-24}$cm$^2$)')
    axex[0].set_title(r'{:.4f}$\sigma_H$+{:.4f}$\sigma_C$'.format(nf_H, nf_C))
    axex[0].grid(axis='y')

    axex[1] = df_frac.plot(x='E(MeV)', y=['H_comp(b)', 'C_comp(b)'], alpha=0.8, ax=axex[1])
    axex[1].set_xlabel('neutron kinetic energy (MeV)')
    axex[1].set_ylabel('fraction of total')
    axex[1].grid(axis='y')
    plt.savefig(fn)


def main():
    nC_xsec = pd.read_csv('nC_FTFP_BERT.csv', sep=' ', skipinitialspace=True)
    # Select only records with energy larger than 10 MeV.
    nC_xsec = nC_xsec[nC_xsec['E(MeV)'] >= 10]
    nH_xsec = pd.read_csv('nH_FTFP_BERT.csv', sep=' ', skipinitialspace=True)
    nH_xsec = nH_xsec[nH_xsec['E(MeV)'] >= 10]
    compare_all(nH_xsec, 'neutron cross section off H', 'nH_xsec.png')
    compare_all(nC_xsec, 'neutron cross section off C', 'nC_xsec.png')
    plot_weighted_mean(nC_xsec, nH_xsec, 'weighted_mean.png')


if __name__ == '__main__':
    main()