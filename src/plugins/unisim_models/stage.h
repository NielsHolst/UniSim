/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STAGE_H
#define UNISIM_STAGE_H

#include <QObject>
#include <usbase/model.h>

namespace UniSim {

class Stage : public UniSim::Model
{
	Q_OBJECT
public:
    Stage(Identifier name, QObject *parent=0);
    ~Stage();
    // standard methods
	void initialize();
	void reset();
	void update();

    // special methods
    const double* ageClasses() const;

private:
	// parameters
	double _L;
	int _k;

    // pull variables
	double *_x;
    double _dt, _prevDel;
	mutable bool _dirtySum, _firstUpdate;
	mutable double _sum, _input, _output, _inputTotal, _outputTotal;
    QObject *ageClassesPtr;

    // push variables
    double _inflow, _instantMortality;

    // push-pull variables
    double _fgr;

    // models
    Model *time;

	// methods
    void deleteVariable(QString name);
    void fill(double value);
	double sum() const;
    void applyInstantMortality();;
};

}
#endif
