/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/*! \class UniSim::Model
    \brief  The %Model class provides the basic model building block.

    Classes are derived from %Model to represent simulation entities, such as animal and plant populations,
    environment and human intervention. Sub-classes of %Model are specialized  by overriding one or more of the
    transformation functions inherited from Component: initialize(), reset(), update(), cleanup() and debrief().

    %Model is derived from both Component and Parameters. The latter provides the ability to set parameter values. The initialize() function is a good place to set parameters, as well as
    establishing pointers to other %Model objects. %Model pointers can be used to communicate with other models
    during execution().

    %Model objects have an internal state represented by state variables. State parameters should be private and only
    changes via the transformation functions. But state variables can be queried by other objects (typically derived from
    %Model) via state(). If it is uncertain whether a state variables exists, it can be queried with statePtr().

    To facilitate the flow of information into %Model objects, input variables are provided which have the opposite logic
    of state variables: You cannot query a model from outside about the value of its input variables (input() is protected)
    but you can set it from the outside via setInput(),

    Both state and input variables are referred to by an Identifier.

    Models are supplied as plugins. If you derive your own %Model classes you should also put them
    in a plugin to be generally available.

    \sa ModelMakerPlugIn
*/

#include <cmath>
#include <iostream>
#include <QString>
#include "exception.h"
#include "model.h"

using namespace std;

namespace UniSim{

	
	
Model::Model(Identifier name, QObject *parent)
    : UniSim::Component(name, parent)
{
}

//! Sets up a pointer to a state variable
/*!
  State variables are typically set up during initialize(). State variables allow other models to pull
  information from this model via state().

  If called more than once with the same name, the latest call will override those before it.
  */
void Model::setState(Identifier name, double *var)
{
    States::iterator it = _states.find(name);
	Q_ASSERT(it == _states.end());
	_states[name] = var;
}

//! Returns the current value of a state variable.
/*!
  An Exception is thrown if name is not found. Use statePtr() if you want to check that name is valid.
*/
double Model::state(Identifier name) const
{
    const double *value = statePtr(name);
    if (!value)
        throw Exception("Model: " + objectName() + " has no state named: " + name.label());
	return *value;
}


//! Returns pointer to a state variable
/*!
    Null is returned in case name is not found.
*/
const double* Model::statePtr(Identifier name) const
{
    States::const_iterator it = _states.find(name);
	return it != _states.end() ? it.value() : 0;
}


//! Returns a collection of all states defined for this model
const Model::States& Model::states() const
{
	return _states;
}

//! Sets the value of an input variable
/*!
  If an input by that name does not already exits, it is created. Otherwise it's value is overwritten with the
  new value.

  This function is typically called once by the model itself, e.g. in reset(), and later on by the update() function
  in other &Model objects. Thereby other models can push information to this model.
*/
void Model::setInput(Identifier name, double value)
{
    if (isnan(value))
        throw Exception("Input variable: " + name.label() + " of " + objectName() + " set to not-a-number");
    if (isinf(value))
        throw Exception("Input variable: " + name.label() + " of " + objectName() + " set to infinite number");
    _inputs[name] = value;
}

//! Returns the value of an input variable
/*!
  An Exception is thrown if name is not found.

  Other models cannot query input variables. Only state variables allow others to pull information from
  a model.
*/
double Model::input(Identifier name) const
{
    Inputs::const_iterator it = _inputs.find(name);
    if (it == _inputs.end())
        throw Exception("Model: " + objectName() + " could not find input named: " + name.label());
    return it.value();
}

QString Model::allStates() const
{
    QString s;
    for (States::const_iterator it = _states.constBegin(); it !=_states.constEnd(); ++it) {
        s += it.key().label() + ": " + QString::number(*(it.value())) + "\n";
    }
    return s;
}

} //namespace

