/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "pollen_onset_date.h"
#include <usengine/simulation.h>

using namespace UniSim;

namespace ecotox {

PollenOnsetDate::PollenOnsetDate(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    addVariable<QDate>(Name(date), "Date when pollen deposition begins");
    addVariable<double>(Name(dayOfYear), "Julian day of @F date");
    addVariable<double>(Name(year), "The year of @F date");
}

int PollenOnsetDate::currentYear() const {
    Model *calendar = simulation()->seekOne<Model*>("calendar");
    QDate date = calendar->pullValue<QDate>("date");
    int year = date.year();
    if (date.month() == 12)
        ++year;
    return year;
}

} //namespace

