/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <time.h>

namespace MicrobialCommunity {

    class Random {

        public:
        Random();

        int getInt(int, int);
        double getDouble(double, double);

    };

} //namespace


#endif // RANDOM_H
