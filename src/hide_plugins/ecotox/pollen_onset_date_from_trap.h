/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_POLLEN_ONSET_DATE
#define ECOTOX_POLLEN_ONSET_DATE
#include <usbase/data_grid.h>
#include <QDate>
#include <QMap>

namespace UniSim {
class Generator;
    class Model;
    class Random;
}

namespace ecotox {

class PollenOnsetDate
{
public: 
    PollenOnsetDate() {}
    virtual ~PollenOnsetDate() {}
    virtual QDate calculate() = 0;
};


class PollenOnsetDateFromFile : public PollenOnsetDate
{
public:
    PollenOnsetDateFromFile(QString fileName);
    QDate calculate();
private:
    UniSim::DataGrid data;
    QMap<int, int> yearsFirstLine;
    int lastYear;
};


class PollenOnsetDateFromCurve : public PollenOnsetDate
{
public:
    PollenOnsetDateFromCurve(UniSim::Model *model);
    QDate calculate();
private:
    UniSim::Model *model;
};

} //namespace
#endif
