/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_ANONYMOUS_H
#define UNISIM_ANONYMOUS_H

#include <usbase/model.h>

namespace UniSim{

class Anonymous : public Model
{
public:
    Anonymous(Identifier name, QObject *parent=0);
};

} //namespace


#endif
