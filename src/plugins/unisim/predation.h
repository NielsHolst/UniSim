/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PREDATION_H
#define UNISIM_PREDATION_H
#include <usbase/matrix.h>
#include <usbase/model.h>

namespace UniSim {

class DataGrid;

class Predation : public Model
{
public:
    Predation(UniSim::Identifier name, QObject *parent=0);
    ~Predation();
    // standard methods
    void amend();
    void reset();
    void update();
private:
    // parameters
    QString apparencyMatrixFileName;
    DataGrid *am;
    Matrix<double> apparencies;
    QStringList predatorNames, preyNames;
    int numPredators, numPrey;
    // variables
    Matrix<double> attacks;
    // links
    typedef QVector<const double*> Links;
    Links demands, resources;
    // methods
    void setPredators();
    void setPrey();
    void setApparencies();
    void setAttacks();
    void createLinks(Links &links, QStringList refs);
    void extractNames(QStringList &names, QStringList refs);
    void updateAttacksByPredator();
    void updateAttacksByPrey();
};

}
#endif
