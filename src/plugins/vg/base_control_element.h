/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BASE_CONTROL_ELEMENT_BASE_H
#define VG_BASE_CONTROL_ELEMENT_BASE_H

#include <usbase/model.h>

namespace vg {

class BaseControlElement : public UniSim::Model
{
public:
    BaseControlElement(UniSim::Identifier name, QObject *parent);
    void reset() final;
    void update() final;
    virtual void localReset() {}
protected:
    double initState, signal, timeStepSecs;
private:
    enum {Decreasing, Stable, Increasing};
    double state, fulfilment;
    int course;
    virtual double change(double error) = 0;
};
} //namespace


#endif
