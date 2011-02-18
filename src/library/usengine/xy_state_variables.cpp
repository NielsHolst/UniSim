/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usbase/utilities.h>
#include "xy_state_variables.h"

namespace UniSim{

void XYStateVariables::Variable::reset()
{
	data.clear();
}

void XYStateVariables::Variable::update()
{
	data.append(*value);
}
	
XYStateVariables::XYStateVariables(QString name, QObject *parent)
	: QObject(parent), decoded(false)
{
}

void XYStateVariables::reset()
{
	decode();
	_x[0].reset();
	for (int i=0; i< _y.size(); ++i)
		_y[i].reset();
}

void XYStateVariables::update()
{
	decode();
	_x[0].update();
	for (int i=0; i< _y.size(); ++i)
		_y[i].update();
}

void XYStateVariables::appendRawVariable(QString xy, QString variableName, QString modelPath, QString stateName)
{
	Q_ASSERT(xy=="x" || xy=="y");
	
	RawVariable v = {variableName, modelPath, stateName};
	
	if (xy=="x") 
		_xRaw.append(v);
	else
		_yRaw.append(v);
	decoded = false;
}

const XYStateVariables::Variables& XYStateVariables::xVariables() const
{
	const_cast<XYStateVariables*>(this)->decode();
	return _x;
}

const XYStateVariables::Variables& XYStateVariables::yVariables() const
{
	const_cast<XYStateVariables*>(this)->decode();
	return _y;
}

void XYStateVariables::decode()
{
	if (decoded) return;
	_x.clear();
	_y.clear();
	decode(&_x, &_xRaw);
	decode(&_y, &_yRaw);

	if (_x.size() > 1)
		throw Exception("Only one X variable allowed for output, found " 
		      + QString::number(_x.size()) + "in:\n" + rawVariablesAsText(&_xRaw));
			
	decoded = true;
}

void XYStateVariables::decode(Variables *var, RawVariables *raw)
{
    for (RawVariables::iterator ra = raw->begin(); ra != raw->end(); ++ra) {
        Models models = find<Model*>(ra->modelPath);
		
		int noOfVariables = 0;
        for (Models::iterator mo = models.begin(); mo != models.end(); ++mo) {
			if (ra->stateName == "*") {
				for (Model::States::const_iterator st = (*mo)->states().begin(); st != (*mo)->states().end(); ++st) {
                    Identifier stateId = st.key();
                    Variable  v = {ra->variableName, stateId.key(), *mo, st.value()};
					var->append(v);
					++noOfVariables;
				}
			}
			else {
				const double *statePtr = (*mo)->statePtr(ra->stateName);
				if (statePtr) {
					Variable v = {ra->variableName, ra->stateName, *mo, statePtr};
					var->append(v);
					++noOfVariables;
				}
			}
		}
		if (noOfVariables == 0)
			throw Exception("Could not find variable: " + rawVariableAsText(*ra));
	}
}

QString XYStateVariables::rawVariableAsText(RawVariable raw) const
{
	return
		"Variable: '" + raw.variableName + 
		"' in model '" + raw.modelPath + 
		"' state: '" + raw.stateName + "'\n";
}

QString XYStateVariables::rawVariablesAsText(const RawVariables *raw) const
{
	QString res;
	for (RawVariables::const_iterator ra = raw->begin(); ra != raw->end(); ++ra)
		res += rawVariableAsText(*ra);
	return res;	
}

bool XYStateVariables::isYNameUnique(QString yName) const {
    int count = 0;
    for (int i = 0; i < _y.size(); ++i)
        if (_y[i].label == yName) ++count;
    return count == 1;
}

QStringList XYStateVariables::uniqueYLabels() const {
    QStringList labels;
    if (_y.size() == 1)
        labels << _y[0].label;
    else {
        for (int i = 0; i < _y.size(); ++i) {
            if (isYNameUnique(_y[i].label))
                labels << _y[i].label;
            else
                labels << _y[i].model->objectName() + ":" + _y[i].label;
        }
    }
    Q_ASSERT(labels.size() == _y.size());
    return labels;
}

} // namespace
