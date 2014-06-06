/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INTEGRATOR_H
#define UNISIM_INTEGRATOR_H

#include <QProgressDialog>
#include "model.h"

namespace UniSim{

class Exception;

class Integrator : public UniSim::Model
{
	Q_OBJECT
public:
    Integrator(Identifier name, QObject *parent=0);
	
    // standard methods
    void initialize();
    void reset();

    // special methods
    bool nextStep();
    virtual bool nextRun();
	
protected:
	// variables
    int stepNumber;
	double progress;

private:
	// variables
    int runNumber;
	// links
    Model *runIterator;
    QProgressDialog progressDialog;
	// methods
    virtual bool doNextStep() = 0;
    void setupProgressDialog();
};

} //namespace

#endif
