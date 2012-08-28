/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_optimiser_factory.h"
#include "crop.h"
#include "farm.h"
#include "field.h"
#include "landscape.h"
#include "livestock.h"

using namespace UniSim;

namespace crop_optimiser{

void CropOptimiserFactory::defineProducts() {
    addProduct<Farm>("Farm", this, "Description pending");
    addProduct<Field>("Field", this, "Description pending");
    addProduct<Crop>("Crop", this, "Description pending");
    addProduct<Landscape>("Landscape", this, "Description pending");
    addProduct<Livestock>("Livestock", this, "Description pending");
}

UniSim::Identifier CropOptimiserFactory::id() const {
    return "crop_optimiser";
}

QString CropOptimiserFactory::description() const {
    return
    "!Exclude\n"
    "Description of crop_optimiser";
}

QStringList CropOptimiserFactory::authors() const {
    return QStringList()
        << "Anna" << "Niels" ;
}

QObject* CropOptimiserFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(crop_optimiser_factory, CropOptimiserFactory)

} //namespace
