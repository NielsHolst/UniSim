/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "constant_world.h"

using namespace UniSim;

namespace test{

ConstantWorld::ConstantWorld(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    new Parameter<double>("temperature", &setTemperature, 30., this, "description");
    new PullVariable<double>("temperature", &temperature, this, "description");
    new PullVariable<double>("day", &day, this, "description");
    QDir dir = FileLocations::location(FileLocationInfo::Plugins);
    Q_ASSERT(dir.absolutePath().contains("plugins"));
}

void ConstantWorld::initialize()
{
     temperature = setTemperature;
}	

void ConstantWorld::reset()
{
	day = 0;
}

void ConstantWorld::update()
{
    //cout << "ConstantWorld::update() A\n";
    day += 1;
    //cout << "ConstantWorld::update() Z\n";
}

} //namespace

