/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LIFE_STAGE_H
#define LIFE_STAGE_H

#include "../../base/model.h"


class LifeStage : public UniSim::Model
{
	//Q_OBJECT
public:
	LifeStage(QString name, QObject *parent=0);
	~LifeStage();
	void initialize();
	void reset();
	void update();

	void fill(double value);
	double sum() const;

private:
	// parameters
	double _L;
	int _k;
    double _initialInput;

	// state
	double *_x;
	double _fgr, _prevDel;
	mutable bool _dirtySum, _firstUpdate;
	mutable double _sum, _input, _output, _inputTotal, _outputTotal;
};


#endif
