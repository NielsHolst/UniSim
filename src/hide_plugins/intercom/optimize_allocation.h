/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_OPTIMIZE_ALLOCATION
#define INTERCOM_OPTIMIZE_ALLOCATION
#include <QList>
#include <QPair>

namespace intercom {

    namespace OptimizeAllocation {
        typedef QPair<double,int> SolutionEntry;
        typedef QList<SolutionEntry > Solution;

        void optimize(Solution &solution);
    }
}

#endif
