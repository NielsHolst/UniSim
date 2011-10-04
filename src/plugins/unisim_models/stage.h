/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STAGE_H
#define UNISIM_STAGE_H

#include <QObject>
#include <QVector>
#include <usbase/model.h>

namespace UniSim {

class Stage : public UniSim::Model
{
	Q_OBJECT
public:
    Stage(Identifier name, QObject *parent=0);
    // standard methods
	void initialize();
	void reset();
	void update();

    // special methods
    QVector<double>* getBuffer();

private:
	// parameters
    double _L, initialInflow;
    int k;

    // pull variables
    QObject *ageClassesPtr;
    double _dt;
    mutable bool _dirtySum, firstUpdate;
    mutable double _sum, _input, _output, _inputTotal, _outputTotal, _prevInflow;

    // push variables
    double _inflow, _instantMortality;

    // push-pull variables
    double _fgr;

    // links
    Model *time;

	// methods
	double sum() const;
    void applyInstantMortality();

    // data
    QVector<double> buffer;
    double *x;
};

}
#endif
