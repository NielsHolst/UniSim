/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DATE_INTERVAL
#define UNISIM_DATE_INTERVAL
#include <QDate>

namespace UniSim {

class DateInterval
{
public: 
    typedef enum {Daily, Yearly} Frequency;
    DateInterval(QDate fromDate, QDate toDate, Frequency frequency);
    DateInterval(QDate date, Frequency frequency);
    bool includes(QDate date);
private:
    QDate from, to;
    Frequency frequency;
};

} //namespace
#endif
