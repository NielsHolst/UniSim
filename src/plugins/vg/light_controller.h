/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LIGHT_CONTROLLER_H
#define VG_LIGHT_CONTROLLER_H

#include <QTime>
#include <usbase/model.h>

namespace vg {

class LightController : public UniSim::Model
{
public:
    LightController(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    bool signal;
    const bool *periodFlag, *offFlag, *onFlag;
    const bool * getFlag(QString name);
};
} //namespace


#endif
