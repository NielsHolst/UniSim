/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INTEGRATOR_H
#define UNISIM_INTEGRATOR_H

#include "model.h"

class QProgressDialog;

namespace UniSim{

class Exception;

class Integrator : public UniSim::Model
{
	Q_OBJECT
public:
    Integrator(Identifier name, QObject *parent=0);
	
    // standard methods
    virtual void initialize();
    virtual void reset();

    // special methods
    virtual bool nextRun();
    virtual bool nextStep() = 0;
    void acceptException(Exception *e);
	
protected:
    int stepNumber;

private slots:
    void doCancel();
    void closeReport();

private:
    Model *runIterator;
    int runNumber;
    bool reporting, cancelled;
    QProgressDialog *report;
    void reportProgress();
    void openReport();
    void updateReport();
    void createReport();
};

} //namespace

#endif
