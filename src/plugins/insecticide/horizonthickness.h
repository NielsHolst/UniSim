/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_HORIZONTHICKNESS_H
#define INSECTICIDE_HORIZONTHICKNESS_H

#include <usbase/model.h>

namespace UniSim {
    class DataGrid;
}

namespace insecticide {

class horizonthickness : public UniSim::Model
{
public:
    horizonthickness(UniSim::Identifier name, QObject *parent);
    void reset();
    void initialize();
    void update();

private:
    // Input
    double penetrationDepth;
    QString soilFileName, soilType;

    // Translated input
    double start[11], pH[10], clay[10], sand[10], silt[10], foc[10], bulkDensity;
    UniSim::DataGrid *soilTable;

    // Output
    double Htc[11];
};

} //namespace


#endif
