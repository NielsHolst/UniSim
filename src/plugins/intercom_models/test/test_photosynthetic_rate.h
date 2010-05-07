#ifndef INTERCOM_TEST_PHOTOSYNTHETIC_RATE
#define INTERCOM_TEST_PHOTOSYNTHETIC_RATE

#include <QList>
#include <usbase/test/autotest.h>

class TestPhotosyntheticRate : public QObject
{
    Q_OBJECT
private slots:
    void testConstructors();
    void testOperations();
};

DECLARE_TEST(TestPhotosyntheticRate)


#endif
