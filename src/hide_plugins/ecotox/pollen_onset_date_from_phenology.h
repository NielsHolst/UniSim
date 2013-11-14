/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_POLLEN_ONSET_DATE_FROM_PHENOLOGY
#define ECOTOX_POLLEN_ONSET_DATE_FROM_PHENOLOGY
#include <QDate>
#include <QMap>
#include <usbase/model.h>

namespace UniSim {
    class DataGrid;
}

namespace ecotox {

class PollenOnsetDateFromPhenology : public UniSim::Model
{
public: 
    PollenOnsetDateFromPhenology(UniSim::Identifier name, QObject *parent=0);
    ~PollenOnsetDateFromPhenology();
    // standard methods
    void amend();
    void initialize();
    void reset();
private:
    // parameters
    QString fileName;
    // variables
    QDate value;
    // links
    const QDate *date;
    // data
    DataGrid *data;
    QMap<int, int> yearsFirstRow;
};

} //namespace
#endif
