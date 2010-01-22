#ifndef BARN_PREY
#define BARN_PREY
#include <QObject>
#include <usbase/model.h>

namespace barn {

class Prey : public UniSim::Model
{
	Q_OBJECT
public: 
	Prey(UniSim::Identifier name, QObject *parent=0);
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
