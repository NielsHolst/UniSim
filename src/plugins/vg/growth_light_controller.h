/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GROWTH_LIGHT_CONTROLLER_H
#define VG_GROWTH_LIGHT_CONTROLLER_H

#include <QTime>
#include <usbase/model.h>

namespace vg {

class GrowthLightController : public UniSim::Model
{
public:
    GrowthLightController(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    const bool *periodFlag, *offFlag, *onFlag;
    const bool * getFlag(QString name);
    bool signal;
};
} //namespace


#endif
