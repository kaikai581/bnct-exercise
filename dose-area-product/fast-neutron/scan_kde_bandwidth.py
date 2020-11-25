#!/usr/bin/env python

from dap_fast_neutron import *
import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':
    bandwidths = np.logspace(-4, 0, num=10)
    ratios = [fast_neutron_ratio(use_kde=True, kde_bw=bw) for bw in bandwidths]
    plt.close()
    plt.scatter(bandwidths, ratios)
    plt.xscale('log')
    plt.yscale('log')
    plt.show()
