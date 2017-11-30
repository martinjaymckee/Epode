#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#
# File - parseCSV.py:
#
#      A script that allows for the plotting of a number of CSV files on a single plot.  The column
#   to be used for each axis is configurable in the '--axes' argument.  An output file may be
#   specified with the '-o' '--output' argument if an image file is to be created.  Otherwise, the
#   plot is simply shown on the screen.  Any number of .csv filenames or patterns may be passed
#   into the script and the data from each of the found files will be plotted with the assumption
#   that the columns all have the same meaning.
#
#      Pattern matching is implemented with pythons fnmatch
#
#      This script is absolutely NOT intended to be any kind of highly functional data exploration
#   application.  It is intended solely for use in producing graphics from the tabular results of
#   the example applications shipped with the Epode library.
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
import fnmatch
import os.path

import matplotlib as mpl
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def series(vals, axes):
    return [vals[:,idx] for idx in axes]

def projection(axes):
    return '3d' if len(axes)==3 else None

def fileConfig(file):
    config = {}
    for line in file:
        idx_start = line.find("#")
        if not idx_start == -1:
            comment = line[idx_start+1:]
            key, _, value = comment.partition(':')
            key = key.strip()
            value = value.strip()
            if value:
                config[key] = value
    return config


if __name__ == '__main__':
    #
    # Construct argument parser
    #
    parser = argparse.ArgumentParser(description='Process a set of csv files to produce plots.')
    parser.add_argument('-D', '--directory', type=str, default = '.',
        help='Base directory for input .csv files')
    parser.add_argument('--axes', type=int, nargs='+', default = [1, 2],
        help='Indices to plot in x, y, (z) order.')
    parser.add_argument('--labels', type=str, nargs='+', default = None,
        help='Labels to use in the legend.')
    parser.add_argument('-o','--output', default=None,
        help='File to output a final image to.')
    parser.add_argument('csvs', nargs='+', type=str,
        help='Input .csv filenames or simple filename patterns.')

    #
    # Parse Arguments, validate and extract values into variables
    #
    args = parser.parse_args()
    assert 2 <= len(args.axes) <= 3, "Invalid axes length of {}".format(len(args.axes))
    axes = args.axes
    output = args.output

    pattern_csvs = args.csvs
    csvs = set()
    for file in os.listdir(args.directory):
        for pattern in pattern_csvs:
            if fnmatch.fnmatch(file, pattern):
                new_csv = open(os.path.join(args.directory, file), 'r')
                csvs.add(new_csv)
    if not args.labels is None:
        assert len(args.labels) >= len(csvs), "Not all files have matching explicit labels!"
    csvs = list(csvs)
    csvs.sort(key=lambda x: x.name)

    #
    # Set Style Variables
    #
    plt.style.use('seaborn-white')
    mpl.rc('lines', linewidth=2)
    cm = mpl.cm.viridis
    colors = [cm(x) for x in np.linspace(0, 1, len(csvs)) ]

    #
    # Parse Datafiles and produce a list of (np.array(), filename) tuples
    #
    data = []
    for idx, csv in enumerate(csvs):
        new_data = np.genfromtxt(csv, delimiter=',', comments="#")
        csv.seek(0)
        new_config = fileConfig(csv)
        new_label = new_config.get('label', os.path.basename(csv.name))
        if not args.labels is None:
            new_label = args.labels[idx]
        data.append( (new_data, new_label) )

    #
    # Plot the results
    #
    fig = plt.figure()
    ax = fig.add_subplot(111, projection = projection(axes))
    for idx, (vals, label) in enumerate(data):
        ax.plot(*series(vals, axes), alpha=0.75, color=colors[idx], label=label)
    ax.legend(shadow=True)
    if not args.output is None: plt.savefig(output, bbox_inches='tight')
    plt.show()


