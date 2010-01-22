/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_XY_STATE_VARIABLES_H
#define UniSim_XY_STATE_VARIABLES_H

#include <QList>
#include <QObject>
#include <QStringList>
#include <QVector>

namespace UniSim{
	
class Model;
	
//! \addtogroup UniSim
//@{
//! The %XYStateVariables class holds a collection of state variables
/*!
	The %XYStateVariables class is used by Presentation to hold pointers to models and state variables
	for the purpose of showing them in Output.
	
	%XYStateVariables objects are created by SimulationMaker when parsing UniSim files.
*/
class XYStateVariables : public QObject
{
	Q_OBJECT
public:
	//! The %Variable refers to one model and one of its state variables
	struct Variable {
        QString label, 			//!< Label to represent variable
			stateName;			//!< State name (identifier inside its parent model)
		const Model *model;		//!< Pointer to the model holding the state variable
		const double *value;	//!< Pointer to the value of the state variable
		QVector<double> data;	//!< Series of collected values
		void reset();
		void update();
	};
	// Collection of variables
	typedef QList<Variable> Variables;

	//! Constructor
    /*!
      \param name of this object.
      \param parent of this object.
    */
	XYStateVariables(QString name, QObject *parent=0);
	
	void reset();
	void update();
	
    //! Appends variable(s) identified by model and state names
	/*!
		The function will append any number (zero, one or many) of variables that match the modelPath and
		stateName. 
		
		Models are matched by Simulation::findModels().
		
		State variables are matched within each of the matching models
		
		\param xy				must be "x" or "y".
		\param variableName		name given to the variable(s) appended
		\param modelPath		path to find model(s) inside current Simulation
		\param stateName		name to identify state variable(s) inside model(s)
	*/
	void appendRawVariable(QString xy, QString variableName, QString modelPath, QString stateName);
	
	//! Decodes raw variables 
	void decode();
	
	//! Returns list of x variables
	const Variables& xVariables() const;
	
	//! Returns list of y variables
	const Variables& yVariables() const;

    QStringList uniqueYLabels() const;

private:
	struct RawVariable {
		QString variableName, modelPath, stateName;
	};
	typedef QList<RawVariable> RawVariables;
	
	void decode(Variables *var, RawVariables *raw);
	QString rawVariableAsText(RawVariable raw) const;
	QString rawVariablesAsText(const RawVariables *raw) const;
    bool isYNameUnique(QString yName) const;

	RawVariables _xRaw, _yRaw;
	Variables _x, _y;
	bool decoded;
};
//@}
} // namespace

#endif
