#ifndef UNISIM_FIXED_H
#define UNISIM_FIXED_H

#include <QDate>
#include <QTime>
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
    // parameters
    QString parametersAsString;

    // value buffers
    QVector<QDate> dates;
    QVector<QTime> times;
    QVector<bool> bools;
    QVector<int> ints;
    QVector<double> doubles;
    QVector<QString> strings;
};

}
#endif
