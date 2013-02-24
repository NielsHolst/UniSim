/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PATCH_SQUARE_H
#define UNISIM_PATCH_SQUARE_H
#include <usbase/model.h>


namespace UniSim{

class PatchSquare : public Model
{
public: 
    PatchSquare(UniSim::Identifier name, QObject *parent=0);
private:
    // parameters
    int x, y, size;
};

} //namespace
#endif
