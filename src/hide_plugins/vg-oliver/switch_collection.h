/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SWITCH_COLLECTION_H
#define VG_SWITCH_COLLECTION_H

#include <usbase/model.h>

namespace vg {

class SwitchCollection : public UniSim::Model
{
public:
    SwitchCollection(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    enum {SomeTrue, AllTrue} condition;
    QString _condition;
    bool initOn, emptyOn, on;
    QList<const bool*> switches;
};
} //namespace


#endif
