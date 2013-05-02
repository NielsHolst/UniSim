/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_FUNCTIONAL_RESPONSE_MULTIWAY_H
#define UNISIM_FUNCTIONAL_RESPONSE_MULTIWAY_H
#include <QVector>
#include <usbase/matrix.h>
#include <usbase/model.h>

namespace UniSim {

class DataGrid;

class FunctionalResponseMultiway : public Model
{
public:
    FunctionalResponseMultiway(UniSim::Identifier name, QObject *parent=0);
    ~FunctionalResponseMultiway();
    // standard methods
    void amend();
    void reset();
    void update();
protected:
    // variables
    Matrix<double> attacks;
    // data
    QStringList predatorNames, preyNames;
    int numPredators, numPrey;
    // links
    typedef QVector<const double*> Links;
    Links resources;
private:
    // parameters
    QString apparencyMatrixFileName;
    DataGrid *am;
    Matrix<double> apparencies;
    // links
    Links demands;
    // methods
    virtual void createVariables() = 0;
    virtual void updateFromNumAttacks() = 0;
    void setPredators();
    void setPrey();
    void setApparencies();
    void createLinks(Links &links, QStringList refs);
    void extractNames(QStringList &names, QStringList refs);
    void updateAttacksByPredator();
    void updateAttacksByPredator(int ixPredator);
    void updateAttacksByPrey();
    void updateAttacksByPrey(int ixPrey);
};

}
#endif
