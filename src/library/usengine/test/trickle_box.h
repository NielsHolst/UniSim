#ifndef TRICKLE_BOX_H
#define TRICKLE_BOX_H

#include <usbase/model.h>


class TrickleBox : public UniSim::Model
{
	Q_OBJECT
public:
	TrickleBox(QString name, QObject *parent=0);
	void initialize();
	void reset();
	void update();

private:
	// parameters
	int _capacity;

	// state
	double _contents, _inflow, _outflow;
};


#endif
