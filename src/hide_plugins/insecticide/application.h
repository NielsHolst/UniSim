/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_APPLICATION_H
#define INSECTICIDE_APPLICATION_H

#include <usbase/model.h>

namespace insecticide {

class Application : public UniSim::Model
{
public:
    Application(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    int day;
    double dosage;

    // Variables
    double application;

    // Data
    int daysPassed;
};

} //namespace


#endif
