/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SEPARATION_SEPARATOR
#define SEPARATION_SEPARATOR
#include <QObject>
#include <usbase/model.h>

namespace separation {

class Separator : public UniSim::Model
{
	Q_OBJECT
public: 
	Separator(UniSim::Identifier name, QObject *parent=0);
	// standard methods
        void initialize();
	void reset();
	void update();

private:
	// parameters
        double slurryTAN, sepindexTAN,sepindexVol, slurryTotal;
        int periode;

        UniSim::Model *liquid;
        UniSim::Model *solid;

        int days;
        void load();

        // pull variables


};

} //namespace
#endif
