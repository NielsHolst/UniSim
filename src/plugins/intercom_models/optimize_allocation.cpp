/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QtAlgorithms>
#include "optimize_allocation.h"

namespace intercom {

    namespace OptimizeAllocation {

        void renorm(Solution &solution, int from, int n) {
            double sum = 0.;
            for (int i = from; i < n; ++i)
                sum += solution[i].first;

            double dv = (1. - sum)/(n-from);
            for (int i = from; i < n; ++i)
                solution[i].first += dv;

        }

        void optimize(Solution &solution) {
            qSort(solution);
            int n = solution.size();
            int i = 0;
            while (i < n-1) {
                while (i < n-1 && solution[i].first >= 0) ++i;
                if (solution[i].first < 0) {
                    solution[i].first = 0;
                    renorm(solution, i+1, n);
                }
            }
        }


    }



} //namespace


