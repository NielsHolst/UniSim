#include <iostream>
#include "trickle_box.h"
#include "trickle_sequence.h"

TrickleSequence::TrickleSequence(QString name, QObject *parent)
	: Model(name,parent) 
{
    setRecursionPolicy(Update, ChildrenNot);
}

void TrickleSequence::initialize()
{
	boxes.clear();
	for (int i = 0; i < children().size(); ++i) {
		TrickleBox* box = dynamic_cast<TrickleBox*>(children().at(i));
		if (box) {
			boxes.append(box);
		}
	}
}

void TrickleSequence::update()
{
    std::cout << "TrickleSequence::update() A " << "\n";
    if (boxes.size() > 0) {
        boxes[0]->pushVariable<double>("inflow", 1.);
		boxes[0]->update();
		for (int i = 1; i < boxes.size(); ++i) {
            boxes[i]->pushVariable<double>("inflow", boxes[i-1]->pullVariable<double>("outflow"));
			boxes[i]->update();
		}
	}
    std::cout << "TrickleSequence::update() Z " << "\n";
}
