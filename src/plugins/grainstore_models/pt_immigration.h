/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GRAINSTORE_PT_IMMIGRATION
#define GRAINSTORE_PT_IMMIGRATION
#include <QObject>
#include <usbase/model.h>

namespace grainstore{

class PtImmigration : public UniSim::Model
{
	Q_OBJECT
public: 
    PtImmigration(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double initial, continuous;

	// state
    double current;
        bool useInitial;

};

} //namespace
#endif
