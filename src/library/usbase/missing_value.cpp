/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <limits>
#include "missing_value.h"

using std::numeric_limits;

namespace UniSim {

template<> QString missingValue() {
    return "NA";
}

template<> double missingValue() {
    return numeric_limits<double>::quiet_NaN();
}

template<> int missingValue() {
    return numeric_limits<int>::max();
}

template<> char missingValue() {
    return char(0);
}

template<> QDate missingValue() {
    return QDate(0,0,0);
}

template<> QTime missingValue() {
    return QTime(-1,-1);
}

template<> bool isMissingValue(double value) {
    return std::isnan(value);
}

template<> bool isMissingValue(bool) {
    return false;
}

}
