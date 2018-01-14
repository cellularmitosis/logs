#!/usr/bin/env python
"""
Copyright (c) 2017 Muxr, http://www.eevblog.com/forum/profile/?u=105823

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
"""

import pandas
import matplotlib
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as numpy
import time as std_time
from scipy.interpolate import spline
import matplotlib.ticker as plticker
from matplotlib.ticker import FormatStrFormatter
import argparse
import sys
import locale

COLORS = ["#6e3c82", "#3498db", "#95a5a6", "#e74c3c", "#34495e", "#2ecc71"]


def format_time(ts):
    res = []
    for each in ts:
        res.append(std_time.strftime("%H:%M.%S", std_time.localtime(numpy.asscalar(numpy.int32(each)))))
    return res


def get_date_range(df):
    max_time = df.timestamp.max()
    min_time = df.timestamp.min()
    t_to = std_time.strftime("%d-%b-%Y", std_time.localtime(numpy.asscalar(numpy.int32(max_time))))
    t_from = std_time.strftime("%d-%b-%Y", std_time.localtime(numpy.asscalar(numpy.int32(min_time))))
    if t_to == t_from:
        return t_to
    return "{} - {}".format(t_from, t_to)


def time_delta(df):
    start = df.timestamp.min()
    stop = df.timestamp.max()
    d = divmod(stop-start, 86400)  # days
    h = divmod(d[1], 3600)         # hours
    m = divmod(h[1], 60)           # minutes
    s = m[1]                       # seconds

    return '{:.0f}d {:02.0f}:{:02.0f}.{:02.0f}'.format(d[0], h[0], m[0], int(s))


def y_fmt(value, _):
    # thanks to https://stackoverflow.com/a/38553110
    return '{:0,.0f}'.format(value)


def rolling(df, window):
    return df.rolling(window=window).mean()[window-1:]


def plot(options):

    sns.set(style="darkgrid")
    sns.set_palette(COLORS)

    sys.stdout.write("reading csv file...")
    sys.stdout.flush()
    df = pandas.read_csv(options.infile, delimiter=',')
    sys.stdout.write(" done.\n")
    sys.stdout.flush()

    plt.locator_params(axis='y', nticks=20)

    # by default I keep getting an error about allocating too many blocks
    # when I try to plot more than a few days worth of data.
    # this forces matplotlib to break the plot up into chunks.
    # thanks to https://stackoverflow.com/a/23361090
    matplotlib.rcParams['agg.path.chunksize'] = 100000

    sys.stdout.write("df.rolling()...")
    sys.stdout.flush()
    df = rolling(df, 360)
    sys.stdout.write(" done.\n")
    sys.stdout.flush()

    sys.stdout.write("df.set_index().plot()...")
    sys.stdout.flush()
    plot = df.set_index('timestamp').plot(figsize=(21, 9), linewidth=0.3)
    sys.stdout.write(" done.\n")
    sys.stdout.flush()

    # set labels for X and Y axis
    n = len(plot.xaxis.get_ticklabels())
    evened_out_ts = numpy.linspace(df.timestamp.min(), df.timestamp.max(), n)

    plot.set_xticklabels(format_time(evened_out_ts), rotation=-15)

    ny = len(plot.yaxis.get_ticklabels())
    plot.set_yticklabels(numpy.linspace(df.value.min(), df.value.max(), ny))
    plot.yaxis.set_major_formatter(plticker.FuncFormatter(y_fmt))


    # TODO add minor ticks
    # plot.yaxis.set_tick_params(which='minor', right='off')

    fig = plot.get_figure()

    #
    # plot the trend line
#    z = numpy.polyfit(df.timestamp, df.value, 1)
#    p = numpy.poly1d(z)
#    plt.plot(df.timestamp, p(df.timestamp), "r--", color=COLORS[0], linewidth=0.8)

    #
    # add some captions
    title = '{} ({})'.format(options.title, get_date_range(df))
    fig.text(0.40, 0.90, title, fontsize=13, fontweight='bold', color=COLORS[0])
    print title

    height = 0.265
    spacing = 0.025

    value_max = 'max: {:0,.0f}'.format(df.value.max())
    fig.text(0.905, height, value_max, fontsize=12, color=COLORS[0])
    height -= spacing
    print value_max

    value_min = 'min: {:0,.0f}'.format(df.value.min())
    fig.text(0.905, height, value_min, fontsize=12, color=COLORS[0])
    height -= spacing
    print value_min

    value_p2p = 'p-p: {:0,.0f}'.format(float(df.value.max() - df.value.min()))
    fig.text(0.905, height, value_p2p, fontsize=12, color=COLORS[0])
    height -= spacing
    print value_p2p

    value_std_dev = 'std-dev: {:0,.0f}'.format(round(df.value.std(), 9))
    fig.text(0.905, height, value_std_dev, fontsize=12, color=COLORS[0])
    height -= spacing
    print value_std_dev

    count = 'samples: {}'.format(df.value.count())
    fig.text(0.905, height, count, fontsize=12, color=COLORS[0])
    height -= spacing
    print count

    value_duration = 'duration: {}'.format(time_delta(df))
    fig.text(0.905, height, value_duration, fontsize=12, color=COLORS[0])
    height -= spacing
    print value_duration

    mean = 'mean: {:0,.1f}'.format(df.value.mean())
    fig.text(0.905, height, mean, fontsize=13, fontweight='bold', color=COLORS[0])
    height -= spacing
    print mean

    sys.stdout.write("fig.savefig()...")
    sys.stdout.flush()
    fig.savefig(options.outfile, bbox_inches='tight')
    sys.stdout.write(" done.\n")
    sys.stdout.flush()

def main():
    sys.stdout.write("starting...\n")
    sys.stdout.flush()

    parser = argparse.ArgumentParser()
    parser.add_argument('infile', nargs='?')
    parser.add_argument('outfile', nargs='?')
    parser.add_argument('-t',
                        '--title',
                        dest='title',
                        action='store',
                        help='title to be used in the chart')
    parser.add_argument('-y',
                        '--ydigits',
                        dest='ydigits',
                        action='store',
                        default="7",
                        help='Number of least significant digits in the Y labels')
    options = parser.parse_args()

    if options.infile is None:
        print "use -h for help"
        sys.exit(-1)

    if options.outfile is None:
        extensionless = options.infile.split('.')[0]
        options.outfile = extensionless + '.png'

    if options.title is None:
        options.title = options.infile

    plot(options)

if __name__ == '__main__':
    main()
