#ifndef UNISIM_TEST_COMPONENT_H
#define UNISIM_TEST_COMPONENT_H

#include <QObject>
#include <QtTest/QtTest>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Component;
    class Simulation;
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

    void testParameterRef();
    void testParameterRefError();
private:
    UniSim::Component
        *animals,
        *reptiles, *snakes, *cobra, *lizards,
        *mammals, *mice[2], *elephants, *dogs, *femaleDogs, *maleDogs;

    UniSim::Simulation *sim;
    void createSimulation(QString fileName, bool isOk);
    QString filePath(QString fileName) const;
    QString outputPath(QString fileName) const;

};


DECLARE_TEST(TestComponent)

#endif 
