#ifndef TRICKLE_SEQUENCE_H
#define TRICKLE_SEQUENCE_H

#include <usbase/model.h>

class TrickleBox;

class TrickleSequence : public UniSim::Model
{
	//Q_OBJECT
public:
	TrickleSequence(QString name, QObject *parent=0);
	void initialize();
    void update();
private:
	QList<TrickleBox*> boxes;
};


#endif
