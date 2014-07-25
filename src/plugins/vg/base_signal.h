/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BASE_SIGNAL_H
#define VG_BASE_SIGNAL_H

#include <usbase/model.h>

namespace vg {

class BaseSignal : public UniSim::Model
{
public:
    BaseSignal(UniSim::Identifier name, QObject *parent);
    void reset() final;
    void update() final;
    virtual double signal() = 0;
private:
    double signalReset, _signal;
    bool flag;
};
} //namespace


#endif
