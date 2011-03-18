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
    void initialize();

private:
    //methods
    void initParameters();
    void initPullVariables();
    void initPushVariables();

    // parameters
    QString parametersAsString, pullVariablesAsString, pushVariablesAsString;

    // pull variables
    QVector<double> parameters, pullVariables, pushVariables;
};

}
#endif
