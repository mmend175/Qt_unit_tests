// requirement: REQFBCE-130, REQFBCE-205
/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
**
** Filename: tst_testcorioliswaterflowtest.cpp
** File Date: 20170117
**
** Authors **
** Author: Jordan R. Higgins (JRH)
**
** Version and Traceability **
** Subversion: Version $Id: tst_testcorioliswaterflowtest.cpp 1046 2017-05-23 13:53:59Z jrhiggi2 $
**
** Revision History:
**   <Date> <Name of Change Agent>
**   Description:
**     - Bulleted list of changes.
**
********************************************************************** */
#include <QtTest>

#include <builder.h>

#include <corioliswaterflowtest.h>
#include <functionthread.h>
#include <mockhealthstatuslogger.h>
#include <mocksensoreffector_i.h>

class TestCoriolisWaterFlowTest : public QObject
{
    Q_OBJECT

public:
    TestCoriolisWaterFlowTest();
    ~TestCoriolisWaterFlowTest();

private slots:
    void initTestCase(); // will be called before the first test function starts
    void init(); // will be called before each test function executes
    void cleanup(); // will be called after each test function executes
    void cleanupTestCase(); // will be called after the final test function finishes

    void testConstructor();

    void testStartWrongTest();
    void testStartWrongTest_data();

    void testStopWrongTest();
    void testStopWrongTest_data();

    // requirement: REQFBCE-61
    void testCommandReceive();
    void testCommandSend();
    // end requirement

private:
    Builder _builder;
    QSharedPointer<MockHealthStatusLogger> _logger;
    QSharedPointer<MockSensorEffector_I> _sensorEffector;

    QScopedPointer<QSignalSpy> _commandReceivedSpy;
    QScopedPointer<QSignalSpy> _commandSendSpy;
    QScopedPointer<QSignalSpy> _completionSpy;
    QScopedPointer<CoriolisWaterFlowTest> _test;
};

// ********************************************************************** */
TestCoriolisWaterFlowTest::TestCoriolisWaterFlowTest()
// ********************************************************************** */
{
    QCoreApplication::setApplicationName("TestCoriolisWaterFlowTest");

} // end TestCoriolisWaterFlowTest::TestCoriolisWaterFlowTest()

// ********************************************************************** */
TestCoriolisWaterFlowTest::~TestCoriolisWaterFlowTest()
// ********************************************************************** */
{
} // end TestCoriolisWaterFlowTest::~TestCoriolisWaterFlowTest()

// ********************************************************************** */
void TestCoriolisWaterFlowTest::initTestCase()
// ********************************************************************** */
{
    _logger.reset(new MockHealthStatusLogger);
    _builder.provide("HealthStatusLogger", _logger);

    _sensorEffector.reset(new MockSensorEffector_I(&_builder));
    _builder.provide("SensorEffector_I", _sensorEffector);

} // end void TestCoriolisWaterFlowTest::initTestCase()

// ********************************************************************** */
void TestCoriolisWaterFlowTest::init()
// ********************************************************************** */
{
    _logger->clearCalls();
    _logger->clearExpectations();
    _logger->expect("h_HealthAndStatusLogSlot", ANY, CSC::BIT, ANY, ANY, ANY);

    _sensorEffector->clearCalls();
    _sensorEffector->clearExpectations();

    _test.reset(new CoriolisWaterFlowTest(&_builder));
    _commandReceivedSpy.reset(new QSignalSpy(_test.data(),
                                            SIGNAL(commands_CommandReceivedSignal(QByteArray))));
    _commandSendSpy.reset(new QSignalSpy(_test.data(),
                                         SIGNAL(commands_CommandSendSignal(QByteArray))));
    _completionSpy.reset(new QSignalSpy(_test.data(),
                                        SIGNAL(test_TestProcessingCompleteSignal(TEST,bool))));

} // end void TestCoriolisWaterFlowTest::init()

// ********************************************************************** */
void TestCoriolisWaterFlowTest::cleanup()
// ********************************************************************** */
{
} // end void TestCoriolisWaterFlowTest::cleanup()

// ********************************************************************** */
void TestCoriolisWaterFlowTest::cleanupTestCase()
// ********************************************************************** */
{
} // end void TestCoriolisWaterFlowTest::cleanupTestCase()

// ********************************************************************** */
void TestCoriolisWaterFlowTest::testConstructor()
// ********************************************************************** */
{
    // Action done by init()

    // Assert
    QCOMPARE(_test->_theLogger, _logger);
    QCOMPARE(_test->_theSensorEffector, _sensorEffector);

} // end void TestCoriolisWaterFlowTest::testConstructor()

// requirement: REQFBCE-61
// ********************************************************************** */
void TestCoriolisWaterFlowTest::testCommandReceive()
// ********************************************************************** */
{
    // Arrange
    const QSharedPointer<XFcfCommand> givenCommand(new XFcfCommand(0,0));

    // Act
    _test->commands_CommandReceiveSlot(givenCommand);

    // Assert
    QCOMPARE(_logger->countCalls(), 0);
    QCOMPARE(_sensorEffector->countCalls(), 0);

} // end void TestCoriolisWaterFlowTest::testCommandReceive()

// ********************************************************************** */
void TestCoriolisWaterFlowTest::testCommandSend()
// ********************************************************************** */
{
    // Arrange
    const QSharedPointer<XFcfCommand> givenCommand(new XFcfCommand(0,0));

    // Act
    _test->commands_CommandSendSlot(givenCommand);

    // Assert
    QCOMPARE(_logger->countCalls(), 0);
    QCOMPARE(_sensorEffector->countCalls(), 0);

} // end void TestCoriolisWaterFlowTest::testCommandSend()
// end requirement

// ********************************************************************** */
void TestCoriolisWaterFlowTest::testStartWrongTest()
// ********************************************************************** */
{
    // Arrange
    QFETCH(TEST, givenTest);

    // Act
    _test->test_TestProcessingStartSlot(givenTest);

    // Assert
    QCOMPARE(_completionSpy->size(), 1);
    QCOMPARE((*_completionSpy)[0][0], QVariant::fromValue(givenTest));
    QCOMPARE((*_completionSpy)[0][1], QVariant::fromValue(false));

    QList<BaseMock::Call> calls = _logger->calls();
    QCOMPARE(calls.size(), 1);
    QVERIFY(calls[0].matches("h_HealthAndStatusLogSlot", ANY, BIT, QString("test_TestProcessingStartSlot"), ERROR, ANY));

} // end void TestCoriolisWaterFlowTest::testStartWrongTest()

// ********************************************************************** */
void TestCoriolisWaterFlowTest::testStartWrongTest_data()
// ********************************************************************** */
{
    QTest::addColumn<TEST>("givenTest");

    QTest::newRow("FTEST-002") << FTEST_TWO;
    QTest::newRow("IBIT-001") << IBIT_ONE;
    QTest::newRow("MBIT-002") << MBIT_TWO;

} // end void TestCoriolisWaterFlowTest::testStartWrongTest_data()

// ********************************************************************** */
void TestCoriolisWaterFlowTest::testStopWrongTest()
// ********************************************************************** */
{
    // Arrange
    QFETCH(TEST, givenTest);

    // Act
    _test->test_TestProcessingStopSlot(givenTest);

    // Assert
    QList<BaseMock::Call> calls = _logger->calls();
    QCOMPARE(calls.size(), 1);
    QVERIFY(calls[0].matches("h_HealthAndStatusLogSlot", ANY, BIT, QString("test_TestProcessingStopSlot"), ERROR, ANY));

} // end void TestCoriolisWaterFlowTest::testStopWrongTest()

// ********************************************************************** */
void TestCoriolisWaterFlowTest::testStopWrongTest_data()
// ********************************************************************** */
{
    QTest::addColumn<TEST>("givenTest");

    QTest::newRow("FTEST-002") << FTEST_TWO;
    QTest::newRow("IBIT-001") << IBIT_ONE;
    QTest::newRow("MBIT-002") << MBIT_TWO;

} // end void TestCoriolisWaterFlowTest::testStopWrongTest_data()

QTEST_GUILESS_MAIN(TestCoriolisWaterFlowTest)

#include "tst_testcorioliswaterflowtest.moc"
// end requirement
