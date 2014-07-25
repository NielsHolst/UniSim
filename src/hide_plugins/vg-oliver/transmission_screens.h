/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_TRANSMISSION_SCREENS_H
#define VG_TRANSMISSION_SCREENS_H

#include "transmission_base.h"

namespace vg {

class TransmissionScreens : public TransmissionBase
{
public:
    TransmissionScreens(UniSim::Identifier name, QObject *parent);
    bool applyScreens() const;
};
} //namespace


#endif
