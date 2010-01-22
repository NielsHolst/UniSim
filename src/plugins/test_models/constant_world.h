/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef TEST_CONSTANTWORLD
#define TEST_CONSTANTWORLD
#include <QObject>
#include <usbase/model.h>

namespace test{

class ConstantWorld : public UniSim::Model
{
	Q_OBJECT
public: 
	ConstantWorld(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

private:
    // parameters
    double setTemperature;

    // state
    double temperature, day;
};

} //namespace
#endif
