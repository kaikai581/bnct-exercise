#!/usr/bin/env python

from dap_fast_neutron import *
import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':
    bandwidths = np.logspace(-4, 0, num=41)
    ratios = [fast_neutron_ratio(use_kde=True, kde_bw=bw) for bw in bandwidths]
    
    # decorate the output figure
    plt.close()
    plt.scatter(bandwidths, ratios)
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('KDE bandwidth (MeV)')
    plt.ylabel(r'Weighted ratio (Gy-cm$^2$)')
    plt.title('Kerma factor-weighted fast to epithermal neutron flux ratio\nvs. neutron flux KDE bandwidth')
    plt.grid(True)
    
    # save plot
    out_dir = 'plots'
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    out_fpn = os.path.join(out_dir, 'kde_bandwidth_scan.png')
    plt.savefig(out_fpn)
