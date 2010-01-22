#include "predator.h"

using namespace UniSim;


namespace barn {

Predator::Predator(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
	setState("N", &density);
}

void Predator::initialize() {
	setParameter("Ninit", &Ninit, 1.);
	setParameter("K", &K, 1000.);
	setParameter("r", &r, 1.2);
}

void Predator::reset() {
	density = Ninit;
}

void Predator::update() {
	density += (K<=0) ? 0 : density*r*(K-density)/K;
}

} //namespace

