#ifndef UNISIM_TEST_NAME_LIST_H
#define UNISIM_TEST_NAME_LIST_H

#include <QObject>
#include <QtTest/QtTest>
#include <usbase/test/autotest.h>

	
class TestNameList : public QObject
{
    Q_OBJECT
    
private slots:
    void testEmpty();
    void testOne();
    void testMany();
};

DECLARE_TEST(TestNameList)

#endif 
