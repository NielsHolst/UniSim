/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_POLLEN_ONSET_DATE_FROM_TRAP_OBS
#define ECOTOX_POLLEN_ONSET_DATE_FROM_TRAP_OBS
#include "pollen_onset_date.h"

namespace UniSim {
    class DataGrid;
}

namespace ecotox {

class PollenOnsetDateFromTrapObs : public PollenOnsetDate
{
public:
    PollenOnsetDateFromTrapObs(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void reset();
private:
    // parameters
    QString obsFileName;
    // data
    int colDate, colPollen;
    // methods
    int findColumn(const UniSim::DataGrid &data, QString colName) const;
    double randomFractile();
};


} //namespace
#endif
