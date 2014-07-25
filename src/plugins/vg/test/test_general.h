#ifndef TEST_VG_GENERAL_H
#define TEST_VG_GENERAL_H

#include <usbase/model.h>
#include <usbase/test/autotest.h>

class TestGeneral : public QObject
{
    Q_OBJECT
private slots:

    void testCo2();
};

DECLARE_TEST(TestGeneral)


#endif
