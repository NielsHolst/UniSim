/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/file_locations.h>
#include "constant_world.h"
#include <iostream>

using namespace UniSim;

namespace test{

ConstantWorld::ConstantWorld(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
	setState("temperature", &temperature);
	setState("day", &day);
    QDir dir = FileLocations::location(FileLocations::Plugins);
    Q_ASSERT(dir.absolutePath().contains("plugins"));
}

void ConstantWorld::initialize()
{
	 setParameter("temperature", &setTemperature, 30.);
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

