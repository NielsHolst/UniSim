/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREEN_DEW_H
#define VG_SCREEN_DEW_H

#include <usbase/model.h>

namespace vg {

class ScreenDew : public UniSim::Model
{
public:
    ScreenDew(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double Tscreen, mcIndoors, state, timeStepSecs;
    int numTimeSteps;

    // Variables
    double value;
};
} //namespace


#endif
