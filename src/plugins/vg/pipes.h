/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_PIPES_H
#define VG_PIPES_H

#include <usbase/model.h>

namespace vg {

class Pipe;

class Pipes : public UniSim::Model
{
public:
    Pipes(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Variables
    double heatTransfer, heatEnergy;

    // Links
    QList<Pipe*> pipes;
};
} //namespace


#endif
