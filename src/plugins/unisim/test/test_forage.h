#ifndef TEST_UNISIM_FORAGE_H
#define TEST_UNISIM_FORAGE_H
#include <QVector>
#include <usbase/test/autotest.h>

class TestForage : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testParts();
    void testFirstLevel();
    void testFinalLevel();
    void testOwners();
    void testForage();

private:
    QVector<double> p1, p2, p3;
    bool compareVectors(QVector<double> v1, QVector<double> v2);
};

DECLARE_TEST(TestForage)


#endif
