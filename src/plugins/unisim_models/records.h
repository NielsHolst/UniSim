#ifndef UNISIM_RECORDS_H
#define UNISIM_RECORDS_H

namespace UniSim {

class Records : public Model {
	Records();
	~Records();

private:
	QFile file;
	QDateTime curTime, time[2];
	QList<Identifier> names;
	typedef QList<PullVariableBase*> Variables;
	typedef Variables* VariablesPtr;
	Variables curValues;
	VariablesPtr variables[2];
	
};

}
#endif