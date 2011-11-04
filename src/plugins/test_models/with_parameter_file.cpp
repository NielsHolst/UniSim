/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include "with_parameter_file.h"
#include "constant_world.h"

using namespace UniSim;

namespace test{

WithParameterFile::WithParameterFile(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    crops = new CropParameters("crops", "model_with_parameter_file_input.txt", this, "Description...");
}

void WithParameterFile::initialize() {
    // Example on how to access crops
    // int water = crops->value<int>("barley","sandy","water");
}

WithParameterFile::CropParameters* WithParameterFile::theCrops() {
    return crops;
}


} //namespace

