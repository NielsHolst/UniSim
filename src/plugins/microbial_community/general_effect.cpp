/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include <usbase/exception.h>
#include "general_effect.h"
#include "random.h"

using namespace UniSim;

namespace MicrobialCommunity {

    GeneralEffect::GeneralEffect(UniSim::Identifier name, QObject *parent) : Effect(name, parent) {
        new Parameter<QString>("TargetParameter", &targetParameter, "", this, "The parameter of Population model that is affected by this factor");
        new Parameter<QString>("EffectRatio", &effectRatio, "", this, "The effect ratio of this factor");
    }

    void GeneralEffect::initialize() {

        // Call initialize() from base class first to generate a list of affected populations.
        Effect::initialize();

        if(enableEffect) {

            effectRatio = effectRatio.trimmed().toLower();

            if(effectRatio.startsWith("uniform:{")) {

                QRegExp match("\\{([\\d.]+)\\}"); // Matches text like "{0.5}" and captures the text within {} into a group.
                match.indexIn(effectRatio);

                if(match.matchedLength() > 0) {
                    bool isDouble = false;
                    ratio = match.capturedTexts()[1].trimmed().toDouble(&isDouble);
                    if(!isDouble) ratio = 1.;
                } //if

                else {
                    throw Exception("\"" + effectRatio + "\" is not a valid value for parameter EffectRatio. A valid value should be like \"Uniform:{0.5}\".", this);
                }

            } //if "Uniform"

            else if(effectRatio.startsWith("specified:{")) {

                QRegExp match("\\{([\\d\\s.-,]+)\\}"); // Matches text like "{0.5, 0.6, 0.7-0.8, 0.9}" and captures the text within {} into a group.
                match.indexIn(effectRatio);

                if(match.matchedLength() > 0) {
                    QStringList valueList = match.capturedTexts()[1].split(",", QString::SkipEmptyParts);

                    if(valueList.size() != affectedPopulationList.size()) {
                        throw Exception("The number of specified values in parameter EffectRatio does not match the number of Population models in parameter TargetPopulations. These two parameters must contain the same number of items.", this);
                    }

                    for(int i = 0; i < valueList.size(); i++) {
                        effectRatioList[i] = valueList[i].trimmed();
                    } //for

                } //if

            } //if "Specified"

            else if(effectRatio.startsWith("range:{")) {

                QRegExp match("\\{([\\d.]+)\\s*,\\s*([\\d.]+)\\}"); // Matches text like "{0.5, 0.8}" and captures each number into a separate group.
                match.indexIn(effectRatio);

                if(match.matchedLength() > 0) {
                    bool isDouble = false;

                    minRatio = match.capturedTexts()[1].toDouble(&isDouble);
                    if(!isDouble) minRatio = 1.;

                    maxRatio = match.capturedTexts()[2].toDouble(&isDouble);
                    if(!isDouble) maxRatio = 1.;

                    if(minRatio > maxRatio) {
                        int temp = minRatio;
                        minRatio = maxRatio;
                        maxRatio = temp;
                    }

                    ratio = random.getDouble(minRatio, maxRatio);
                } //if

                else {
                    throw Exception("\"" + effectRatio + "\" is not a valid value for parameter EffectRatio. A valid value should be like \"Random:{0.5, 0.8}\".", this);
                }

            } //if "Range"

            else {
                throw Exception("\"" + effectRatio + "\" is not a valid value for parameter EffectRatio. Refer to plug-in documentation for correct parameter values.", this);
            }

        } //if(enableEffect)

    } //initialize()

    void GeneralEffect::reset() {

        // Call reset() from base class first to generate a list of affected populations.
        Effect::reset();

        if(enableEffect) {

            for(int i = 0; i < affectedPopulationList.size(); i++) {

                if(effectRatio.startsWith("specified:{")) {

                    QRegExp match("([\\d.]+)-([\\d.]+)"); // Matches text like "0.5-0.8" and captures each number into a separate group.
                    match.indexIn(effectRatioList[i]);

                    bool isDouble = false;

                    if(match.matchedLength() > 0) {

                        minRatio = match.capturedTexts()[1].toDouble(&isDouble);
                        if(!isDouble) minRatio = 1.;

                        maxRatio = match.capturedTexts()[2].toDouble(&isDouble);
                        if(!isDouble) maxRatio = 1.;

                        if(minRatio > maxRatio) {
                            int temp = minRatio;
                            minRatio = maxRatio;
                            maxRatio = temp;
                        }

                        ratio = random.getDouble(minRatio, maxRatio);
                    }
                    else {
                        ratio = effectRatioList[i].toDouble(&isDouble);
                        if(!isDouble) ratio = 1.;
                    }
                } //if "Specified"

                else if(effectRatio.startsWith("range:{")) {
                    ratio = random.getDouble(minRatio, maxRatio);
                } //if "Range"

                affectedPopulationList[i]->pushValue<double>(targetParameter, affectedPopulationList[i]->pullValue<double>(targetParameter) * ratio);

            } //for

        } //if(enableEffect)

    } //reset()

} //namespace
