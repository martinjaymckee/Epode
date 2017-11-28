#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#
# File - parseCSV.py:
#
#      Basic type definitions required to implement the whole of the Epode library.  These are
#  mostly basic types (and type aliases) which are used for data forwarding and storage or are
#  available as base classes for generic functionality.
#
#
# License:
#
#      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
#  If a copy of the MPL was not distributed with this file, You can obtain one
#  at http://mozilla.org/MPL/2.0/.
#
#

import argparse
import os.path

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def series(vals, axes):
    return [vals[:,idx] for idx in axes]

def projection(axes):
    return '3d' if len(axes)==3 else None

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Process a set of csv files to produce plots.')
    parser.add_argument('--axes', type=int, nargs='+', default = [1, 2],
        help='Indices to plot in x, y, (z) order.')
    parser.add_argument('-o','--output', default=None,
        help='File to output a final image to.')
    parser.add_argument('csvs', nargs='+', type=argparse.FileType('r'),
        help='Input CSV filenames.')

    args = parser.parse_args()
    assert 2 <= len(args.axes) <= 3, "Invalid axes length of {}".format(len(args.axes))
    csvs = args.csvs
    axes = args.axes
    output = args.output

    #
    # Set Style Variables
    #
    plt.style.use('seaborn-deep')


    #
    # Parse Datafiles and produce a list of (np.array(), filename) tuples
    #
    data = [(np.genfromtxt(csv, delimiter=','), os.path.basename(csv.name)) for csv in csvs]

    #
    # Plot the results
    #

    fig = plt.figure()
    ax = fig.add_subplot(111, projection = projection(axes))
    for vals, name in data:
        ax.plot(*series(vals, axes), alpha=0.75, linewidth=2, label=name)
    ax.legend(shadow=True)
    if not args.output is None: plt.savefig(output, bbox_inches='tight')
    plt.show()


