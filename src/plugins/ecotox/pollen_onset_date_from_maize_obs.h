/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_POLLEN_ONSET_DATE_FROM_MAIZE_OBS
#define ECOTOX_POLLEN_ONSET_DATE_FROM_MAIZE_OBS
#include <QMap>
#include "pollen_onset_date.h"

namespace UniSim {
    class DataGrid;
}

namespace ecotox {

class PollenOnsetDateFromMaizeObs : public PollenOnsetDate
{
public:
    PollenOnsetDateFromMaizeObs(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void amend();
    void reset();
private:
    // parameters
    QString obsFileName;
    // data
    UniSim::DataGrid *data;
    QMap<int, int> yearsFirstLine;
    int lastYear;
    // methods
    int sampleInterval(int from, int to);
};



} //namespace
#endif
