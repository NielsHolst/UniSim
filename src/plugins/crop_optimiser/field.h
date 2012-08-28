/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CROP_OPTIMISER_FIELD
#define CROP_OPTIMISER_FIELD
#include <QObject>
#include <usbase/model.h>


namespace crop_optimiser {

class Field : public UniSim::Model
{
	Q_OBJECT
public: 
	Field(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void reset();
	void update();
        //typedef UniSim::ParameterFile1D<QString> CropParameters;

private:
	// parameters
        double area;
        QString cropIdentifier;
        //CropParameters *crops;

	// pull variables
        double price;
            //double density;

};

} //namespace
#endif
