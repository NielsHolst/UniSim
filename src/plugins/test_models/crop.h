/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TEST_CROP
#define UNISIM_TEST_CROP
#include <QObject>
#include <usbase/model.h>

namespace test{

class Crop : public UniSim::Model
{
	Q_OBJECT
public: 
    Crop(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    void reset();
    void update();

private:
    // parameters
    int numFields, initArea;
    double nitrogenNorm;
    // pull variables
    int currentArea;
};

} //namespace
#endif
