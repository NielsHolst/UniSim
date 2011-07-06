#ifndef UNISIM_TEST_COMPONENT_H
#define UNISIM_TEST_COMPONENT_H

#include <QObject>
#include <QtTest/QtTest>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Component;
}
	
class TestComponent : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testSeekOne();
    void testSeekMany();
    void testSeekOneChild();
    void testSeekChildren();
    void testSeekOneSibling();
    void testSeekOneSiblingNotMe();
    void testSeekSiblings();
    void testSeekSiblingsNotMe();
    void testSeekPrecedingSibling();
    void testSeekFollowingSibling();
    void testSeekOneDescendant();
    void testSeekDescendants();
    void testSeekOneAscendant();
    void testSeekOneAscendantWithJoker();

private:
    UniSim::Component
        *animals,
        *reptiles, *snakes, *cobra, *lizards,
        *mammals, *mice[2], *elephants, *dogs, *femaleDogs, *maleDogs;
};

DECLARE_TEST(TestComponent)

#endif 
