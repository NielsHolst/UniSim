#include <iostream>
#include <limits>
#include <QDate>
#include <QList>
#include <QMap>
#include <QSet>
#include <QStringList>
#include <usbase/component.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include <usbase/decode_list.h>
#include "test_utilities.h"

using std::cout;
using namespace UniSim;


void TestUtilities::testDecodeSimpleListEmpty() {
    QStringList list;
    try {
        list = decodeSimpleList("");
        QVERIFY(list.isEmpty());

        list = decodeSimpleList("()");
        QVERIFY(list.isEmpty());

        list = decodeSimpleList("( )");
        QVERIFY(list.isEmpty());
    }
    catch (Exception &ex) {
        QString msg = "Wrong diagnose: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestUtilities::testDecodeSimpleListOne() {
    QStringList list;
    try {
        list = decodeSimpleList("abc");
        QCOMPARE(list.size(), 1);
        QCOMPARE(list.value(0), QString("abc"));

        list = decodeSimpleList("(abc)");
        QCOMPARE(list.size(), 1);
        QCOMPARE(list.value(0), QString("abc"));

        list = decodeSimpleList("a b c");
        QCOMPARE(list.size(), 1);
        QCOMPARE(list.value(0), QString("a b c"));
    }
    catch (Exception &ex) {
        QString msg = "Wrong diagnose: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestUtilities::testDecodeSimpleListMany() {
    QStringList list;

    try {
        list = decodeSimpleList("(a b c)");
        QCOMPARE(list.size(), 3);
        QCOMPARE(list.value(0), QString("a"));
        QCOMPARE(list.value(1), QString("b"));
        QCOMPARE(list.value(2), QString("c"));

        list = decodeSimpleList(" ( a  b  c  ) ");
        QCOMPARE(list.size(), 3);
        QCOMPARE(list.value(0), QString("a"));
        QCOMPARE(list.value(1), QString("b"));
        QCOMPARE(list.value(2), QString("c"));
    }
    catch (Exception &ex) {
        QString msg = "Wrong diagnose: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestUtilities::testDecodeSimpleListFaulty() {
    QStringList list;

    try {
        list = decodeSimpleList("(");
        QFAIL("Faulty list not diagnosed");
    }
    catch (Exception &ex) {
    }

    try {
        list = decodeSimpleList(")");
        QFAIL("Faulty list not diagnosed");
    }
    catch (Exception &ex) {
    }

    try {
        list = decodeSimpleList("(a b c");
        QFAIL("Faulty list not diagnosed");
    }
    catch (Exception &ex) {    
    }

    try {
        list = decodeSimpleList("a b c)");
        QFAIL("Faulty list not diagnosed");
    }
    catch (Exception &ex) {
    }
}

void TestUtilities::testSplitParentChildExpression() {
    QString expression = "dogs/female[size]";
    QStringList result;
    try {
        result = splitParentChildExpression(expression);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Split failed: " + ex.message()));
    }
    QCOMPARE(result[0], QString("dogs/female"));
    QCOMPARE(result[1], QString("size"));
}

void TestUtilities::testSplitAtNamespace()
{
    QString namespacePart, ownNamePart;

    splitAtNamespace("One::two", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString("One"));
    QCOMPARE(ownNamePart, QString("two"));

    splitAtNamespace("One::two::Three", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString("One::two"));
    QCOMPARE(ownNamePart, QString("Three"));

    splitAtNamespace("one", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString());
    QCOMPARE(ownNamePart, QString("one"));

    splitAtNamespace("::one", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString());
    QCOMPARE(ownNamePart, QString("one"));

    splitAtNamespace("one::", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString("one"));
    QCOMPARE(ownNamePart, QString(""));
}

namespace {
    template <class T>
    bool setEqualsList(const QSet<T*> &set, const QList<T*> &list) {
        QSet<T*> set2;
        for (int i = 0; i < list.size(); ++i)
            set2 << list.at(i);
        return set == set2;
    }
}

void TestUtilities::testQStringSplit() {
    QString str = ",a,,b,c";
    QStringList list = str.split(",");
    QCOMPARE(list.size(), 5);
    QVERIFY(list[0].isEmpty());
    QCOMPARE(list[1], QString("a"));
    QVERIFY(list[2].isEmpty());
}

void TestUtilities::testMissingLeftBracket() {
    QString expression = "femalesize]";
    bool excepted = false;
    QStringList result;
    try {
        result = splitParentChildExpression(expression);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestUtilities::testMissingRightBracket() {
    QString expression = "female[size";
    bool excepted = false;
    try {
        splitParentChildExpression(expression);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

}

void TestUtilities::testMissplacedLeftBracket() {
    QString expression = "[femalesize]";
    bool excepted = false;
    try {
        splitParentChildExpression(expression);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

}

void TestUtilities::testMissingChildName() {
    QString expression = "female[]";
    bool excepted = false;
    try {
        splitParentChildExpression(expression);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

}

void TestUtilities::testStringToValueDouble() {
    QCOMPARE(stringToValue<double>("6.023e23"), 6.023e23);
    QCOMPARE(stringToValue<double>("6."), 6.);

    bool excepted = false;
    try {
        double test = stringToValue<double>("a6z");
        std::cout << test << "\n";
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        stringToValue<double>(QString());
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestUtilities::testStringToValueInt() {
    QCOMPARE(stringToValue<int>("6023"), 6023);
    QCOMPARE(stringToValue<int>("0"), 0);

    try {
        stringToValue<int>(QString("6z"));
        QFAIL("No exception");
        stringToValue<int>(QString("6.023e23"));
        QFAIL("No exception");
        stringToValue<int>(QString("6."));
        QFAIL("No exception");
    }
    catch (Exception &ex) {
    }

    try {
        stringToValue<int>(QString());
        QFAIL("No exception");
    }
    catch (Exception &ex) {
    }
}

void TestUtilities::testStringToValueChar() {
    char ch = 'x';
    QCOMPARE(stringToValue<char>(QString(ch)), ch);

    bool excepted = false;
    try {
        stringToValue<char>(QString("ab"));
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        stringToValue<char>(QString());
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestUtilities::testStringToValueBool() {
    QCOMPARE(stringToValue<bool>("yes"), true);
    QCOMPARE(stringToValue<bool>("true"), true);
    QCOMPARE(stringToValue<bool>("no"), false);
    QCOMPARE(stringToValue<bool>("false"), false);

    bool excepted = false;
    try {
        stringToValue<bool>(QString("T"));
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        stringToValue<bool>(QString());
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestUtilities::testStringToValueDate() {
    QCOMPARE(stringToValue<QDate>(QString("24/12/2012")), QDate(2012, 12, 24));
    QCOMPARE(stringToValue<QDate>(QString("2012/12/24")), QDate(2012, 12, 24));

    bool excepted = false;
    try {
        stringToValue<QDate>("24");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        stringToValue<QDate>(QString());
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestUtilities::testStringToValueTime() {
    QCOMPARE(QTime::fromString(QString("12:30:20"), "h:m:s"), QTime(12, 30, 20));
    QCOMPARE(QTime::fromString(QString("12:30"), "h:m"), QTime(12, 30, 0));

    QCOMPARE(stringToValue<QTime>(QString("12:30:20")), QTime(12, 30, 20));
    QCOMPARE(stringToValue<QTime>(QString("12:30")), QTime(12, 30, 0));
    QCOMPARE(stringToValue<QTime>(QString("0:0:0")), QTime(0, 0, 0));
    QCOMPARE(stringToValue<QTime>(QString("0:0")), QTime(0, 0, 0));

    bool excepted = false;
    try {
        stringToValue<QTime>(QString("12"));
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

namespace {
    template<class T> void testStringToMissingValue() {
        T value;
        try {
            value = stringToValue<T>("NA");
            QVERIFY( isMissingValue(value) );
        }
        catch (Exception &ex) {
            QString msg = "Unexpected exception: " + ex.message();
            QFAIL(qPrintable(msg));

        }

    }
}

void TestUtilities::testStringToMissingValueDouble() {
    testStringToMissingValue<double>();
}

void TestUtilities::testStringToMissingValueInt() {
    testStringToMissingValue<int>();
}

void TestUtilities::testStringToMissingValueChar() {
    testStringToMissingValue<char>();
}

void TestUtilities::testStringToMissingValueBool() {
    //bool excepted = false;
    try {
        QWARN ("missingValue<bool>() won't compile?");
    }
    catch (Exception &) {
//        excepted = true;
    }
    // QVERIFY(excepted);
}

void TestUtilities::testStringToMissingValueDate() {
    testStringToMissingValue<QDate>();
}

void TestUtilities::testStringToMissingValueTime() {
    testStringToMissingValue<QTime>();
}

void TestUtilities::testDecodeListFaulty() {
    bool excepted = false;
    try {
        decodeList(QString());
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        decodeList("a");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        decodeList("a b");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    QObject *test = new QObject;
    test->setObjectName("TEST");

    excepted = false;
    try {
        decodeList("((a b) c))", test);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

}

void TestUtilities::testDecodeListEmpty() {
    QStringList list;

    list = decodeList("()");
    QCOMPARE(list.size(), 0);
}

void TestUtilities::testDecodeListEmptyItems() {
    QStringList list;
    QStringList expected;

    list = decodeList("(())");
    expected << "()";
    QCOMPARE(list, expected);

    list = decodeList("(()())");
    expected << "()";
    QCOMPARE(list, expected);
}

void TestUtilities::testDecodeListDepth1() {
    QStringList list;
    QStringList expected;

    list = decodeList("(a)");
    expected << "a";
    QCOMPARE(list, expected);

    list = decodeList("(a b)");
    expected << "b";
    QCOMPARE(list, expected);
}

void TestUtilities::testDecodeListDepth2() {
    QStringList list;
    QStringList expected;

    try {
        list = decodeList("((a))");
        expected << "(a)";
        QCOMPARE(list, expected);

        list = decodeList("((a)(b))");
        expected << "(b)";
        QCOMPARE(list, expected);

        expected.clear();
        list = decodeList("((a b))");
        expected << "(a b)";
        QCOMPARE(list, expected);

        list = decodeList("((a b)(c d))");
        expected << "(c d)";
        QCOMPARE(list, expected);

        expected.clear();
        list = decodeList("((a b) c)");
        expected << "(a b)" << "c";
        QCOMPARE(list, expected);

        expected.clear();
        list = decodeList("(a (b c))");
        expected << "a" << "(b c)";
        QCOMPARE(list, expected);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestUtilities::testDecodeListOfInts() {
    QList<int> decoded, expected;
    decoded = decodeList<int>("(12 56 -8 0)");
    expected << 12 << 56 << -8 << 0;
    QCOMPARE(decoded, expected);
}

void TestUtilities::testDecodeListOfDates() {
    QList<QDate> decoded, expected;
    decoded = decodeList<QDate>("(1/1/2010 30/6/2009 17/4/2011)");
    expected << QDate(2010,1,1) << QDate(2009,6,30) << QDate(2011,4,17);
    QCOMPARE(decoded, expected);
}

void TestUtilities::testDecodeListOfStringDoubles() {
    QMap<QString,double> decoded, expected;
    decoded = decodeList<QString,double>("( (cat 5.4) (dog 14) (mouse 0.020))");
    expected["cat"] = 5.4;
    expected["dog"] = 14;
    expected["mouse"] = 0.020;
    QCOMPARE(decoded, expected);
}

void TestUtilities::testDecodeListOfStringDoubles2() {
    QMap<QString,double> decoded, expected;
    decoded = decodeList<QString,double>("((Tmin 20)(Tmax 20)(irradiationMJ 20))");
    expected["Tmin"] = 20;
    expected["Tmax"] = 20;
    expected["irradiationMJ"] = 20;
    QCOMPARE(decoded, expected);
}

void TestUtilities::testDecodeListOfIntDates() {
    QMap<int,QDate> decoded, expected;
    decoded = decodeList<int,QDate>("( (7 1/2/2010) (-2 4/8/2008) (0 24/12/2017))");
    expected[7] = QDate(2010,2,1);
    expected[-2] = QDate(2008,8,4);
    expected[0] = QDate(2017,12,24);
    QCOMPARE(decoded, expected);
}

void TestUtilities::testDivBounded() {
    const double MAX = std::numeric_limits<double>::max();
    const double EPS = std::numeric_limits<double>::epsilon();

    QCOMPARE(divBounded(42.,0.), MAX);
    QCOMPARE(divBounded(MAX, EPS), MAX);
    QCOMPARE(divBounded(MAX, EPS, 42.), 42.);
}

void TestUtilities::testFindNearestFile() {
    QDir uniSimDir = QDir("../..");
    QDir testDir = uniSimDir;
    testDir.cd("src/library/usbase/test");
    QVERIFY(testDir.exists());

    QDir homeDir = testDir;
    homeDir.cd("test_nearest_file/test_nearest_file/test_nearest_file");
    QVERIFY(homeDir.exists());

    QFileInfo expectedFile, foundFile;

    try {
        expectedFile = homeDir.filePath("test_nearest_file_a.txt");
        foundFile = findNearestFile(homeDir, "sub", "test_nearest_file_a.txt");
        QCOMPARE(foundFile.absoluteFilePath(), expectedFile.absoluteFilePath());

        expectedFile = homeDir.filePath("sub/test_nearest_file_b.txt");
        foundFile = findNearestFile(homeDir, "sub", "test_nearest_file_b.txt");
        QCOMPARE(foundFile.absoluteFilePath(), expectedFile.absoluteFilePath());

        expectedFile = testDir.filePath("test_nearest_file/test_nearest_file/test_nearest_file_c.txt");
        foundFile = findNearestFile(homeDir, "sub", "test_nearest_file_c.txt");
        QCOMPARE(foundFile.absoluteFilePath(), expectedFile.absoluteFilePath());

        expectedFile = testDir.filePath("test_nearest_file/test_nearest_file/sub/test_nearest_file_d.txt");
        foundFile = findNearestFile(homeDir, "sub", "test_nearest_file_d.txt");
        QCOMPARE(foundFile.absoluteFilePath(), expectedFile.absoluteFilePath());

        expectedFile = testDir.filePath("test_nearest_file/test_nearest_file_e.txt");
        foundFile = findNearestFile(homeDir, "sub", "test_nearest_file_e.txt");
        QCOMPARE(foundFile.absoluteFilePath(), expectedFile.absoluteFilePath());

        expectedFile = testDir.filePath("test_nearest_file/sub/test_nearest_file_f.txt");
        foundFile = findNearestFile(homeDir, "sub", "test_nearest_file_f.txt");
        QCOMPARE(foundFile.absoluteFilePath(), expectedFile.absoluteFilePath());
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
}
