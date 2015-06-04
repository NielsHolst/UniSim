/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LIPAPHIS_LEAF_NITROGEN_H
#define LIPAPHIS_LEAF_NITROGEN_H

#include <usbase/model.h>

namespace lipaphis {

class LeafNitrogen : public UniSim::Model
{
public:
    LeafNitrogen(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    int time;
    double fertilisation,
        percentage;
};

} //namespace


#endif
