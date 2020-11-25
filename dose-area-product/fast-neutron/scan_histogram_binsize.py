#!/usr/bin/env python

from dap_fast_neutron import *
import matplotlib.pyplot as plt
import numpy as np
import os

if __name__ == '__main__':
    binsizes = np.logspace(-4, 0, num=41)
    # First several bin sizes lead to error. Remove them.
    binsizes = binsizes[6:]
    ratios = [fast_neutron_ratio(use_kde=False, hist_bs=bs) for bs in binsizes]

    # decorate the output figure
    plt.close()
    plt.scatter(binsizes, ratios)
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Bin size (MeV)')
    plt.ylabel(r'Weighted ratio (Gy-cm$^2$)')
    plt.title('Kerma factor-weighted fast to epithermal neutron flux ratio\nvs. neutron flux bin size')
    plt.grid(True)
    
    # save plot
    out_dir = 'plots'
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    out_fpn = os.path.join(out_dir, 'histogram_binsize_scan.png')
    plt.savefig(out_fpn)
