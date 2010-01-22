#include "prey.h"

using namespace UniSim;


namespace barn {

Prey::Prey(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
	setState("N", &density);
}

void Prey::initialize() {
	setParameter("Ninit", &Ninit, 1.);
	setParameter("K", &K, 1000.);
	setParameter("r", &r, 1.2);
}

void Prey::reset() {
	density = Ninit;
}

void Prey::update() {
	density += (K<=0) ? 0 : density*r*(K-density)/K;
}

} //namespace

