#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#
# File - otree.py:
#
#      Implements an occupancy storing quadtree.  Provides methods for adding "occupied" regions
#   as either points or circles.  The default maximum depth of 7 should be sufficient for most
#   uses, if constructed with a larger depth, greater area resolution can be traded for a larger
#   memory footprint and longer insertion/lookup times.
#
#       It is worth noting, however, that it is the area insertion time that increases more
#   rapidly.  If only few insertions are required and many occupancy tests, there is little
#   reason to avoid higher depths.
#
# License:
#
#      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
#  If a copy of the MPL was not distributed with this file, You can obtain one
#  at http://mozilla.org/MPL/2.0/.
#
#

import matplotlib.pyplot as plt
import numpy as np
import numpy.linalg

class OccupiedQuadtree2D:
    __id_count = 0

    def __init__(self, p_ll, p_ur, depth=7, propogate=False, debug=False):
        self.__id = self.__class__.__id_count
        self.__class__.__id_count = self.__class__.__id_count + 1
        self.__p_ll = p_ll
        self.__p_ur = p_ur
        self.__default_depth = depth
        self.__width = p_ur[0] - p_ll[0]
        self.__height = p_ur[1] - p_ll[1]
        self.__p_center = (p_ur + p_ll) / 2.0
        self.__occupied = propogate
        self.__children = [None, None, None, None]
        self.__bounds = np.array([self.__p_ll, self.__p_ur])
        self.__is_leaf = True
        self.__children_full = False
        self.__debug = debug

    @property
    def id(self):
        return self.__id

    @property
    def bounds(self):
        return self.__bounds

    @property
    def center(self):
        return self.__p_center

    @property
    def width(self):
        return self.__width

    @property
    def height(self):
        return self.__height

    @property
    def full(self):
        return self.__children_full or (self.is_leaf and self.occupied)

    @property
    def is_leaf(self):
        return self.__is_leaf

    @property
    def occupied(self):
        return self.__occupied

    @occupied.setter
    def occupied(self, value):
        self.__occupied = value
        return value

    def plot(self, fig = None, ax = None, ec_o = 'k', fc_o = 'r', ec_f = 'k', fc_f = 'white', alpha = 0.1):
        if fig is None:
            fig, ax = plt.subplots()

        if self.full or self.is_leaf:
            ec, fc = (ec_o, fc_o) if self.occupied else (ec_f, fc_f)
            rect = plt.Rectangle(self.bounds[0], self.width, self.height, fc=fc, ec=ec, alpha=alpha)
            ax.add_artist(rect)
        else:
            for child in self.__children:
                if not child is None:
                    child.plot(fig, ax, ec_o, fc_o, ec_f, fc_f, alpha)
        return fig, ax

    def testPoint(self, p):
        return self.__test_point(p)

    def addPoint(self, p, depth=None):
        return self.__generic_add(self.__pointInBounds(p), depth)

    def addCircle(self, p, r, depth=None):
        def circleIntersectsBounds(bounds):
            p_ll, p_ur = bounds
            x_n = np.clip(p[0], p_ll[0], p_ur[0])
            y_n = np.clip(p[1], p_ll[1], p_ur[1])
            p_n = point(x_n, y_n)
            dist = np.linalg.norm(p_n - p)
            return dist <= r

        return self.__generic_add(circleIntersectsBounds, depth)

    def __test_point(self, p):
        if not ( (self.__p_ll[0] <= p[0] <= self.__p_ur[0]) and
                 (self.__p_ll[1] <= p[1] <= self.__p_ur[1]) ): return False
        if self.__children_full: return True
        if self.__is_leaf: return self.__occupied
        return any(child.__test_point(p) for child in self.__children)

    def __generic_test(self, func):
        if not func(self.bounds): return False
        if self.__children_full: return True
        if self.__is_leaf: return self.__occupied
        return any(child.__generic_test(func) for child in self.__children)

    def __generic_add(self, func, depth=None):
        depth = self.__default_depth if depth is None else depth
        bounds = self.bounds
        propogate = self.occupied
        if (not self.__children_full) and func(bounds):
            self.occupied = True

            if depth > 0:
                child_width = self.width / 2.0
                child_height = self.height / 2.0

                # Add Nodes
                if self.is_leaf:
                    # Process Subtree LL
                    p_ll = bounds[0]
                    p_ur = point(bounds[0][0]+child_width, bounds[0][1]+child_height)
                    self.__children[0] = self.__child(p_ll, p_ur)

                    # Process Subtree UL
                    p_ll = point(bounds[0][0], bounds[0][1]+child_height)
                    p_ur = point(bounds[0][0]+child_width, bounds[1][1])
                    self.__children[1] = self.__child(p_ll, p_ur)

                    # Process Subtree LR
                    p_ll = point(bounds[0][0]+child_width, bounds[0][1])
                    p_ur = point(bounds[1][0], bounds[0][1]+child_height)
                    self.__children[2] = self.__child(p_ll, p_ur)

                    # Process Subtree UR
                    p_ll = point(bounds[0][0]+child_width, bounds[0][1]+child_height)
                    p_ur = bounds[1]
                    self.__children[3] = self.__child(p_ll, p_ur)

                for child in self.__children:
                    child.__generic_add(func, depth-1)

                if all(child.full for child in self.__children):
                    self.__children_full = True
                    for idx in range(4):
                        self.__children[idx] = None
                else:
                    self.__children_full = False

                self.__is_leaf = all(child is None for child in self.__children)


    def __child(self, p_ll, p_ur):
        return self.__class__(p_ll, p_ur, depth=self.__default_depth, debug=self.__debug)

    def __pointInBounds(self, p):
        def func(bounds):
            p_ll, p_ur = bounds
            return (p_ll[0] <= p[0] <= p_ur[0]) and (p_ll[1] <= p[1] <= p_ur[1])
        return func
