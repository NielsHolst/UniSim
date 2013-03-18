/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SKY_H
#define VG_SKY_H

#include <usbase/model.h>

namespace vg {

class Sky : public UniSim::Model
{
public:
    Sky(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters
    QString method;
    bool override;

    // Variables
    double temperature, emissivity;

    // Links
    UniSim::Model *records;
    double *TSkyRecords, *Tair, *windspeed;

    // Data
};
} //namespace


#endif
