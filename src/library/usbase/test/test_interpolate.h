#ifndef UNISIM_TEST_USBASE_INTERPOLATE_H
#define UNISIM_TEST_USBASE_INTERPOLATE_H

#include <QDir>
#include <QObject>
#include <usbase/test/autotest.h>


class TestInterpolate : public QObject
{
    Q_OBJECT
private slots:
    void testBracketEmpty();
    void testBracketOne();
    void testBracketTwo();
    void testBracketEqual();
    void testBracketBetween();
    void testBracketBelow();
    void testBracketAbove();

    void testInterpolateTwoPointsIncreasing();
    void testInterpolateTwoPointsDecreasing();
    void testInterpolateTwoPointsEqual();
    void testInterpolateTwoPointsBelow();
    void testInterpolateTwoPointsAbove();

    void testInterpolateManyEmpty();
    void testInterpolateManyOne();
    void testInterpolateManyTwo();
    void testInterpolateManyEqual();
    void testInterpolateManyBetween();
    void testInterpolateManyBelow();
    void testInterpolateManyAbove();
    void testInterpolatePlane2x2();
    void testInterpolatePlane5x4();
private:
    QString filePath(QString fileName);
    QDir inputFolder();
};

DECLARE_TEST(TestInterpolate)


#endif
