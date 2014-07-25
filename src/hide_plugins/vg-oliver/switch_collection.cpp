/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "publish.h"
#include "switch_collection.h"

using namespace UniSim;

namespace vg {

PUBLISH(SwitchCollection)

/*! \class SwitchCollection
 * \brief Finds whether any contained model is on
 *
 * Inputs
 * ------
 * - _condition_ tells whether some or all children muist be true ["someTrue", "allTrue"]
 * - _initOn_ is the initial on value [true/false]
 * - _emptyOn_ is the value if there are no child models [true/false]
 *
 * Outputs
 * ------
 * - _on_ is true if some or all children are true, depending on _condition_ [true/false]

 * Optional dependencies
 * ------------
 * - many (0..n) child models with a port [true/false] named either:
 *   + _on_ or
 *   + _timeOn_
 * - only such child models are allowed
 */

SwitchCollection::SwitchCollection(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input2(QString, _condition, condition, "someTrue");
    Input(bool, initOn, false);
    Input(bool, emptyOn, true);
    Output(bool, on);
}

void SwitchCollection::initialize() {
    switches.clear();
    auto children = seekChildren<Model*>("*");
    for (auto child : children) {
        const bool *p = child->peekValuePtr<bool>("on");
        if (!p)
            p = child->peekValuePtr<bool>("timeOn");
        if (!p) {
            QString msg{"Only child models with a boolean variable named 'on' or 'timeOn' are allowed; not this child '%1'"};
            throw Exception(msg.arg(child->id().label()), this);
        }
        switches << p;
    }

    if (_condition.toLower() == "sometrue")
        condition = SomeTrue;
    else if (_condition.toLower() == "alltrue")
        condition = AllTrue;
    else {
        QString msg{"Input 'condition' must be 'someTrue' or 'allTrue', not '%1'"};
        throw Exception(msg.arg(_condition), this);
    }
}

void SwitchCollection::reset() {
    on = initOn;
}

void SwitchCollection::update() {
    if (switches.isEmpty())
        on = emptyOn;
    else {
        bool allTrue{true}, someTrue {false};
        for (auto sw : switches) {
            allTrue = allTrue && *sw;
            someTrue = someTrue || *sw;
        }
        on = (condition==AllTrue && allTrue) ||
             (condition==SomeTrue && someTrue);
    }
}


} //namespace

