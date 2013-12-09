/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_POLLEN_ONSET_DATE
#define ECOTOX_POLLEN_ONSET_DATE
#include <QDate>
#include <usbase/model.h>

namespace ecotox {

class PollenOnsetDate : public UniSim::Model
{
public:
    PollenOnsetDate(UniSim::Identifier name, QObject *parent=0);
protected:
    // variables
    QDate date;
    double dayOfYear, year;
    // methods
    int currentYear() const;
};

} //namespace
#endif
