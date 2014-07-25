#ifndef TEST_ARGS_H
#define TEST_ARGS_H

#include <QObject>

#include <usbase/test/autotest.h>


class TestArgs : public QObject
{
    Q_OBJECT
private slots:
    void testPluginsDefault();
    void testPluginsRelative();
    void testPluginsAbsolute();

    void testTempDefault();
    void testTempRelative();
    void testTempinsAbsolute();

    void testInputDirNoneFileBare();
    void testInputDirNoneFileRelative();
    void testInputDirNoneFileAbsolute();

    void testInputDirRelativeFileBare();
    void testInputDirRelativeFileRelative();
    void testInputDirRelativeFileAbsolute();

    void testInputDirAbsoluteFileBare();
    void testInputDirAbsoluteFileRelative();
    void testInputDirAbsoluteFileAbsolute();
};

DECLARE_TEST(TestArgs)


#endif
