#ifndef UNISIM_FIXED_H
#define UNISIM_FIXED_H

#include <QVector>
#include <usbase/model.h>
#
namespace UniSim {

class Fixed : public Model
{
    Q_OBJECT
public:
    Fixed(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void amend();
private:
    //methods
    void ammendParameters();
    void ammendPullVariables();
    void ammendPushVariables();

    // parameters
    QString parametersAsString, pullVariablesAsString, pushVariablesAsString;

    // pull variables
    QVector<double> parameters, pullVariables, pushVariables;
};

}
#endif
