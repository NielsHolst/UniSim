/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_DIRECTED_CONTROL_H
#define VG_DIRECTED_CONTROL_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class DirectedControl : public UniSim::Model
{
public:
    DirectedControl(UniSim::Identifier name, QObject *parent);
    void initialize();
    typedef enum{Floor, Ceiling} Direction;
    Direction direction() const;

private:
    // Parameters
    QString strDirection;
    // Data
    UniSim::StringMap<Direction> directions;
    Direction _direction;
};
} //namespace


#endif
