#ifndef UNISIM_TEST_MEGA_FACTORY_H
#define UNISIM_TEST_MEGA_FACTORY_H

#include <usbase/test/autotest.h>

class TestMegaFactory : public QObject
{
    Q_OBJECT
private slots:
    void testCreateModelExists();
    void testCreateModelNoClass();
    void testCreateModelWrongClass();
};

DECLARE_TEST(TestMegaFactory)


#endif
