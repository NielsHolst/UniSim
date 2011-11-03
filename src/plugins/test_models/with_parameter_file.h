/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef TEST_WITH_PARAMETER_FILE
#define TEST_WITH_PARAMETER_FILE
#include <QObject>
#include <usbase/model.h>
#include <usbase/parameter_file.h>

namespace test{

class WithParameterFile : public UniSim::Model
{
	Q_OBJECT
public: 
    WithParameterFile(UniSim::Identifier name, QObject *parent=0);
    void initialize();

    typedef UniSim::ParameterFile2D<QString, QString> CropParameters;
    CropParameters* theCrops();

private:
    // parameters
    CropParameters *crops;
};

} //namespace
#endif
