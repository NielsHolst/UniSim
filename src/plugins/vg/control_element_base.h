/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CONTROL_ELEMENT_BASE_H
#define VG_CONTROL_ELEMENT_BASE_H

#include <usbase/model.h>

namespace vg {

class ControlElementBase : public UniSim::Model
{
public:
    ControlElementBase(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
    typedef enum {Decreasing, Stable, Increasing} Course;
protected:
    // Parameters
    double initState, signal, timeStepSecs;
private:
    // Variable
    double state;
    int course;
    // Methods
    virtual double change(double error) = 0;
};
} //namespace


#endif
