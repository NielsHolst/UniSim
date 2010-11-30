/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RUN_ITERATOR_FIXED
#define UNISIM_RUN_ITERATOR_FIXED
#include <QObject>
#include <usbase/model.h>


namespace UniSim{

class RunIteratorFixed : public Model
{
	Q_OBJECT
public: 
    RunIteratorFixed(UniSim::Identifier name, QObject *parent=0);

    // standard methods
    void initialize();
    void cleanup();

private:
    // parameters
    int numIterations;

    // pull variables
    bool value;
    int iteration;
};

} //namespace
#endif
