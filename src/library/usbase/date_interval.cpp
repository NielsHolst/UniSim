/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "date_interval.h"

namespace UniSim {

DateInterval::DateInterval(QDate fromDate, QDate toDate, Frequency frequency_)
    : from(fromDate), to(toDate), frequency(frequency_)
{
}

DateInterval::DateInterval(QDate date, Frequency frequency_)
    : from(date), to(date), frequency(frequency_)
{
}

bool DateInterval::includes(QDate date) {
    switch (frequency) {
    case Daily:
        return from<=date && date<=to;
        break;
    case Yearly:
        if (date.year()<from.year() || date.year()>to.year())
            return false;
        QDate fromSameYear = QDate(date.year(), from.month(), from.day());
        QDate toSameYear = QDate(date.year(), to.month(), to.day());
        return fromSameYear<=date && date<=toSameYear;
        break;
    }
    return false;
}



} //namespace

