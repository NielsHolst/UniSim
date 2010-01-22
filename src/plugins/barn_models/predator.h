#ifndef BARN_PREDATOR
#define BARN_PREDATOR
#include <QObject>
#include <usbase/model.h>

namespace barn {

class Predator : public UniSim::Model
{
	Q_OBJECT
public: 
	Predator(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
	double Ninit, K, r;

	// state
	double density;

};

} //namespace
#endif
