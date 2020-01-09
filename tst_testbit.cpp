// requirement: REQFBCE-128, REQFBCE-129, REQFBCE-130, REQFBCE-205
/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
**
** Filename: tst_testbit.cpp
** File Date: 20161220
**
** Authors **
** Author: Jordan R. Higgins (JRH)
**
** Version and Traceability **
** Subversion: Version $Id: tst_testbit.cpp 1181 2017-08-01 21:04:21Z jrhiggi2 $
**
** Revision History:
**   <Date> <Name of Change Agent>
**   Description:
**     - Bulleted list of changes.
**
********************************************************************** */
#include <QtTest>

#include <bitimpl.h>
#include <builder.h>
#include <ecamssharedmemory.h>
#include <functionthread.h>
#include <mockcommands.h>
#include <mocktest.h>
#include <pbittwotest.h>

class TestBit : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(); // will be called before the first test function starts
    void init(); // will be called before each test function executes
    void cleanup(); // will be called after each test function executes
    void cleanupTestCase(); // will be called after the final test function finishes

    // requirement: REQFBCE-61
    void testCommandReceive();
    void testCommandSend();
    // end requirement

    // requirement: REQFBCE-57
    void testConstructor();
    // end requirement

    // requirement: REQFBCE-129
    void testPBitIsRunning();
    void testPBitIsRunning_data();

    void testPBitStart();

    void testPBitStop();
    void testPBitStop_data();
    // end requirement

    // requirement: REQFBCE-27
    void testReportingStart();
    void testReportingStart_data();
    void testReportingStop();
    void testReportingStop_data();
    // end requirement

    // requirement: REQFBCE-57
    void testStartThread();
    // end requirement

    // requirement: REQFBCE-128, REQFBCE-130, REQFBCE-205
    void testTestProcessingStartInvalid();
    void testTestProcessingStartQueued();
    void testTestProcessingStartValid();
    void testTestProcessingStartValid_data();

    void testTestProcessingStopActive();
    void testTestProcessingStopActive_data();
    void testTestProcessingStopInactive();
    void testTestProcessingStopInactive_data();
    // end requirement

private:
    QScopedPointer<Builder> _builder;

    QScopedPointer<BitImpl> _bit;
};

// ********************************************************************** */
void TestBit::initTestCase()
// ********************************************************************** */
{
} // end void TestBit::initTestCase()

// ********************************************************************** */
void TestBit::init()
// ********************************************************************** */
{
    _builder.reset(new Builder);
    _bit.reset(new BitImpl(_builder.data(), nullptr));

} // end void TestBit::init()

// ********************************************************************** */
void TestBit::cleanup()
// ********************************************************************** */
{
    if (!_bit.isNull())
    {
        _bit->_ourPBitTwoTest.reset();
        _bit.reset();
    }

    _builder.reset();

} // end void TestBit::cleanup()

// ********************************************************************** */
void TestBit::cleanupTestCase()
// ********************************************************************** */
{
} // end void TestBit::cleanupTestCase()

// requirement: REQFBCE-61
// ********************************************************************** */
void TestBit::testCommandReceive()
// ********************************************************************** */
{
    // Arrange
    QSharedPointer<XFcfCommand> givenCommand(new XFcfCommand(0,0));

    _bit->_ourLogger = QSharedPointer<MockHealthStatusLogger>::create();
    _bit->_ourPBitTwoTest = QSharedPointer<MockPBitTwoTest>::create(_builder.data());
    _bit->_ourSharedMemoryArea = QSharedPointer<EcamsSharedMemory>::create();

    // Act
    _bit->commands_CommandReceiveSlot(givenCommand);

    // Assert
    QCOMPARE(_bit->_ourLogger->countCalls(), 0);
    QCOMPARE(_bit->_ourPBitTwoTest->countCalls(), 0);
    QCOMPARE(_bit->_ourSharedMemoryArea->countCalls(), 0);

} // end void TestAccumulatorTest::testCommandReceive()

// ********************************************************************** */
void TestBit::testCommandSend()
// ********************************************************************** */
{
    // Arrange
    QSharedPointer<XFcfCommand> givenCommand(new XFcfCommand(0,0));

    _bit->_ourLogger = QSharedPointer<MockHealthStatusLogger>::create();
    _bit->_ourPBitTwoTest = QSharedPointer<MockPBitTwoTest>::create(_builder.data());
    _bit->_ourSharedMemoryArea = QSharedPointer<EcamsSharedMemory>::create();

    // Act
    _bit->commands_CommandSendSlot(givenCommand);

    // Assert
    QCOMPARE(_bit->_ourLogger->countCalls(), 0);
    QCOMPARE(_bit->_ourPBitTwoTest->countCalls(), 0);
    QCOMPARE(_bit->_ourSharedMemoryArea->countCalls(), 0);

} // end void TestAccumulatorTest::testCommandSend()
// end requirement

// requirement: REQFBCE-57
// ********************************************************************** */
void TestBit::testConstructor()
// ********************************************************************** */
{
    // Assert
    QVERIFY(_bit->_ourCurrentTest.isNull());
    QCOMPARE(_bit->_ourCSC, BIT);
    QVERIFY(_bit->_ourLogger.isNull());
    QVERIFY(_bit->_ourPBitTwoTest.isNull());
    QVERIFY(_bit->_ourSharedMemoryArea.isNull());

} // end void TestBit::testConstructor()
// end requirement

// requirement: REQFBCE-129
// ********************************************************************** */
void TestBit::testPBitIsRunning()
// ********************************************************************** */
{
    // Arrange
    QFETCH(bool, testCreated);
    QFETCH(bool, testRunning);

    if (testCreated)
    {
        _bit->_ourPBitTwoTest.reset(new MockPBitTwoTest(_builder.data()));
        _bit->_ourPBitTwoTest->expect("b_PbitTwoIsRunning").andReturn(testRunning);
    }

    // Act
    bool actualResult = _bit->b_PBitIsRunning();

    // Assert
    QCOMPARE(actualResult, testRunning);

} // end void TestBit::testPBitIsRunning()

// ********************************************************************** */
void TestBit::testPBitIsRunning_data()
// ********************************************************************** */
{
    QTest::addColumn<bool>("testCreated");
    QTest::addColumn<bool>("testRunning");

    QTest::newRow("Test Not Created") << false << false;
    QTest::newRow("Test Not Running") << true  << false;
    QTest::newRow("Test Running")     << true  << true;

} // end void TestBit::testPBitIsRunning_data()

// ********************************************************************** */
void TestBit::testPBitStart()
// ********************************************************************** */
{
    // Arrange
    QSharedPointer<MockPBitTwoTest> mockTest(new MockPBitTwoTest(_builder.data()));
    _builder->provide("PBitTwoTest", mockTest);

    mockTest->expect("threadable_Start");
    mockTest->expect("test_TestProcessingStartSlot", PBIT_TWO);

    QSignalSpy startedSpy(_bit.data(), SIGNAL(b_PBitStartedSignal()));

    // Act
    _bit->b_PBitStartSlot();

    // Assert
    QCOMPARE(_bit->_ourPBitTwoTest, mockTest);
    QCOMPARE(mockTest->countCalls("threadable_Start"), 1);
    QCOMPARE(mockTest->countCalls("test_TestProcessingStartSlot", PBIT_TWO), 1);

    QCOMPARE(startedSpy.size(), 1);

} // end void TestBit::testPBitStart()

// ********************************************************************** */
void TestBit::testPBitStop()
// ********************************************************************** */
{
    // Arrange
    QFETCH(bool, pbitTwoCreated);
    QFETCH(bool, pbitTwoRunning);

    if (pbitTwoCreated)
    {
        QSharedPointer<MockPBitTwoTest> mockTest(new MockPBitTwoTest(_builder.data()));
        _bit->_ourPBitTwoTest = mockTest;

        mockTest->expect("b_PbitTwoIsRunning").andReturn(pbitTwoRunning);

        if (pbitTwoRunning)
        {
            mockTest->expect("test_TestProcessingStopSlot", PBIT_TWO);
        }
    }

    QSignalSpy stoppedSpy(_bit.data(), SIGNAL(b_PBitStoppedSignal()));

    // Act
    _bit->b_PBitStopSlot();

    // Assert
    if (pbitTwoRunning)
    {
        QCOMPARE(_bit->_ourPBitTwoTest->countCalls("test_TestProcessingStopSlot", PBIT_TWO), 1);
        QCOMPARE(stoppedSpy.size(), 1);
    }
    else
    {
        if (pbitTwoCreated)
        {
            QCOMPARE(_bit->_ourPBitTwoTest->countCalls("test_TestProcessingStopSlot", PBIT_TWO), 0);
        }

        QCOMPARE(stoppedSpy.size(), 0);
    }

} // end void TestBit::testPBitStop()

// ********************************************************************** */
void TestBit::testPBitStop_data()
// ********************************************************************** */
{
    QTest::addColumn<bool>("pbitTwoCreated");
    QTest::addColumn<bool>("pbitTwoRunning");

    QTest::newRow("PBit Not Created") << false << false;
    QTest::newRow("PBit Not Running") << true  << false;
    QTest::newRow("PBit Running")     << true  << true;

} // end void TestBit::testPBitStop_data()
// end requirement

// requirement: REQFBCE-27
// ********************************************************************** */
void TestBit::testReportingStart()
// ********************************************************************** */
{
    // Arrange
    QFETCH(bool, loggerSet);
    QFETCH(bool, sharedMemorySet);

    QSharedPointer<MockCommands> mockCommands = QSharedPointer<MockCommands>::create();
    QSharedPointer<MockHealthStatusLogger> mockLogger = QSharedPointer<MockHealthStatusLogger>::create();
    QSharedPointer<MockBaseSharedMemory_A> mockSharedMemory = QSharedPointer<EcamsSharedMemory>::create();

    _builder->provide("Commands", mockCommands);

    if (loggerSet)
    {
        _bit->_ourLogger = mockLogger;
    }
    else
    {
        _builder->provide("HealthStatusLogger", mockLogger);
    }

    if (sharedMemorySet)
    {
        _bit->_ourSharedMemoryArea = mockSharedMemory;
    }
    else
    {
        _builder->provide("EcamsSharedMemory", mockSharedMemory);
    }

    mockLogger->expect("h_HealthAndStatusLogSlot", ANY, BIT, ANY, STATUS, ANY);

    // Act
    _bit->reporting_StartReporting();

    // Assert
    QCOMPARE(_bit->objectName(), QString("BIT"));

    QVERIFY(!connect(mockCommands.data(), SIGNAL(commands_CommandReceivedSignal(QSharedPointer<XFcfCommand>)),
                     _bit.data(), SLOT(commands_CommandReceiveSlot(QSharedPointer<XFcfCommand>)),
                     Qt::UniqueConnection));
    QVERIFY(!connect(_bit.data(), SIGNAL(commands_CommandSendSignal(QSharedPointer<XFcfCommand>)),
                     mockCommands.data(), SLOT(commands_CommandSendSlot(QSharedPointer<XFcfCommand>)),
                     Qt::UniqueConnection));

    QCOMPARE(_bit->_ourLogger, mockLogger);
    QCOMPARE((bool)connect(_bit.data(), SIGNAL(h_HealthAndStatusLogSignal(qint64,CSC,QString,MSG_TYPES,QString)),
                           mockLogger.data(), SLOT(h_HealthAndStatusLogSlot(qint64,CSC,QString,MSG_TYPES,QString)),
                           Qt::UniqueConnection), loggerSet);

    QCOMPARE(_bit->_ourSharedMemoryArea, mockSharedMemory);
    QCOMPARE((bool)connect(_bit.data(), SIGNAL(sm_WriteDataSignal(QByteArray,CSC,quint64)),
                           mockSharedMemory.data(), SLOT(sm_WriteDataSlot(QByteArray,CSC,quint64)),
                           Qt::UniqueConnection), sharedMemorySet);

} // end void TestBit::testReportingStart()

// ********************************************************************** */
void TestBit::testReportingStart_data()
// ********************************************************************** */
{
    QTest::addColumn<bool>("loggerSet");
    QTest::addColumn<bool>("sharedMemorySet");

    QTest::newRow("Clean") << false << false;
    QTest::newRow("With Logger") << true << false;
    QTest::newRow("With Shared Memory") << false << true;
    QTest::newRow("With Logger and Shared Memory") << true << true;

} // end void TestBit::testReportingStart_data()

// ********************************************************************** */
void TestBit::testReportingStop()
// ********************************************************************** */
{
    // Arrange
    QFETCH(bool, loggerSet);
    QFETCH(bool, pbitTwoCreated);
    QFETCH(bool, pbitTwoRunning);

    if (loggerSet)
    {
        _bit->_ourLogger = QSharedPointer<MockHealthStatusLogger>::create();
    }

    if (pbitTwoCreated)
    {
        _bit->_ourPBitTwoTest.reset(new MockPBitTwoTest(_builder.data()));
        _bit->_ourPBitTwoTest->expect("b_PbitTwoIsRunning").andReturn(pbitTwoRunning);

        if (pbitTwoRunning)
        {
            _bit->_ourPBitTwoTest->expect("test_TestProcessingStopSlot", PBIT_TWO);
        }
    }

    QSignalSpy stoppedSpy(_bit.data(), SIGNAL(b_PBitStoppedSignal()));

    // Act
    _bit->reporting_StopReporting();

    // Assert
    QVERIFY(_bit->_ourLogger.isNull());

    int stopped;
    if (pbitTwoRunning)
    {
        stopped = 1;
    }
    else
    {
        stopped = 0;
    }

    if(pbitTwoCreated)
    {
        QCOMPARE(_bit->_ourPBitTwoTest->countCalls("test_TestProcessingStopSlot", PBIT_TWO), stopped);
    }

    QCOMPARE(stoppedSpy.size(), stopped);

} // end void TestBit::testReportingStop()

// ********************************************************************** */
void TestBit::testReportingStop_data()
// ********************************************************************** */
{
    QTest::addColumn<bool>("loggerSet");
    QTest::addColumn<bool>("pbitTwoCreated");
    QTest::addColumn<bool>("pbitTwoRunning");

    QTest::newRow("Logger Set")       << true  << false << false;
    QTest::newRow("PBit Not Created") << false << false << false;
    QTest::newRow("PBit Not Running") << false << true  << false;
    QTest::newRow("PBit Running")     << false << true  << true;

} // end void TestBit::testReportingStop_data()
// end requirement

// requirement: REQFBCE-57
// ********************************************************************** */
void TestBit::testStartThread()
// ********************************************************************** */
{
    // Arrange
    _bit->expect("threadPriority_PriorityInitialize");

    QSignalSpy logSpy(_bit.data(), SIGNAL(h_HealthAndStatusLogSignal(qint64,CSC,QString,MSG_TYPES,QString)));

    // Act
    qint64 beforeTime = CommonUtils::currentUSecsSinceEpoch();
    _bit->threadPriority_StartThreadSlot();
    qint64 afterTime = CommonUtils::currentUSecsSinceEpoch();

    // Assert
    QCOMPARE(_bit->countCalls("threadPriority_PriorityInitialize"), 1);

    QCOMPARE(logSpy.size(), 1);
    QVERIFY( logSpy[0][0].toLongLong() >= beforeTime);
    QVERIFY( logSpy[0][0].toLongLong() <= afterTime);
    QCOMPARE(logSpy[0][1].value<CSC>(), BIT);
    QCOMPARE(logSpy[0][2].toString(), QString("threadPriority_StartThreadSlot"));
    QCOMPARE(logSpy[0][3].value<MSG_TYPES>(), STATUS);

} // end void TestBit::testStartThread()
// end requirement

// requirement: REQFBCE-128, REQFBCE-130, REQFBCE-205
// ********************************************************************** */
void TestBit::testTestProcessingStartInvalid()
// ********************************************************************** */
{
    // Arrange
    QSignalSpy completeSpy(_bit.data(), SIGNAL(test_TestProcessingCompleteSignal(TEST,bool)));
    QSignalSpy logSpy(_bit.data(), SIGNAL(h_HealthAndStatusLogSignal(qint64, CSC, QString, MSG_TYPES, QString)));

    // Act
    qint64 beforeTime = CommonUtils::currentUSecsSinceEpoch();
    _bit->test_TestProcessingStartSlot(NO_TEST);
    qint64 afterTime = CommonUtils::currentUSecsSinceEpoch();

    // Assert
    QCOMPARE(completeSpy.size(), 1);
    QCOMPARE(completeSpy[0][0], QVariant::fromValue(NO_TEST));
    QCOMPARE(completeSpy[0][1], QVariant::fromValue(false));

    int numErrors = 0;
    for (auto &log : logSpy)
    {
        qint64 logTime = log[0].value<qint64>();
        QVERIFY(beforeTime <= logTime && logTime <= afterTime);
        QCOMPARE(log[1], QVariant::fromValue(BIT));
        QCOMPARE(log[2], QVariant::fromValue(QString("test_TestProcessingStartSlot")));

        if(log[3] == QVariant::fromValue(ERROR))
        {
            numErrors++;
        }
    }
    QCOMPARE(numErrors, 1);

} // end void TestBit::testTestProcessingStartInvalid()

// ********************************************************************** */
void TestBit::testTestProcessingStartQueued()
// ********************************************************************** */
{
    // Arrange
    QThread bitThread;
    _bit->moveToThread(&bitThread);

    FunctionThread callThread([this]()
    {
        _bit->test_TestProcessingStartSlot(NO_TEST);
    });

    QSignalSpy completeSpy(_bit.data(), SIGNAL(test_TestProcessingCompleteSignal(TEST,bool)));

    // Act/Assert
    callThread.start();
    QVERIFY(!callThread.wait(50));
    QCOMPARE(completeSpy.size(), 0);

    bitThread.start();
    QVERIFY(callThread.wait(50));
    QCOMPARE(completeSpy.size(), 1);
    QCOMPARE(completeSpy[0][0], QVariant::fromValue(NO_TEST));
    QCOMPARE(completeSpy[0][1], QVariant::fromValue(false));

    _bit.reset();

    bitThread.quit();
    bitThread.wait();

} // end void TestBit::testTestProcessingStartQueued()

// ********************************************************************** */
void TestBit::testTestProcessingStartValid()
// ********************************************************************** */
{
    // Arrange
    QFETCH(bool, expectedResult);
    QFETCH(TEST, expectedTest);
    QFETCH(TEST, givenTest);

    QObject scope;

    bool commandReceivedConnected = false;
    bool commandSendConnected = false;
    bool mockCreated = false;
    bool mockDestroyed = false;

    connect(&MockTest::monitor, &MockMonitor::create,
            &scope, [this, &commandReceivedConnected, &commandSendConnected, &mockCreated, expectedResult, expectedTest, givenTest](BaseMock *mock)
    {
        MockTest *test = (MockTest *)mock;
        mockCreated = true;

        QCOMPARE(test->_test, expectedTest);
        test->expect("test_TestProcessingStartSlot", givenTest).andDo([this, &commandReceivedConnected, &commandSendConnected, expectedResult, givenTest, test](QVariantList)
        {
            commandReceivedConnected = !connect(_bit.data(), SIGNAL(commands_CommandReceivedSignal(QSharedPointer<XFcfCommand>)),
                                                test, SLOT(commands_CommandReceiveSlot(QSharedPointer<XFcfCommand>)),
                                                Qt::UniqueConnection);
            commandSendConnected = !connect(test, SIGNAL(commands_CommandSendSignal(QSharedPointer<XFcfCommand>)),
                                            _bit.data(), SLOT(commands_CommandSendSlot(QSharedPointer<XFcfCommand>)),
                                            Qt::UniqueConnection);

            emit test->test_TestProcessingCompleteSignal(givenTest, expectedResult);
            return QVariant();
        });
    });

    connect(&MockTest::monitor, &MockMonitor::destroy,
            &scope, [givenTest, &mockDestroyed](BaseMock *mock)
    {
        MockTest *test = (MockTest *)mock;
        mockDestroyed = true;

        QCOMPARE(test->countCalls("test_TestProcessingStartSlot", givenTest), 1);
    });

    QSignalSpy completeSpy(_bit.data(), SIGNAL(test_TestProcessingCompleteSignal(TEST,bool)));

    // Act
    _bit->test_TestProcessingStartSlot(givenTest);

    // Assert
    QVERIFY(commandReceivedConnected);
    QVERIFY(commandSendConnected);

    QVERIFY(mockCreated);
    QVERIFY(mockDestroyed);

    QCOMPARE(completeSpy.size(), 1);
    QCOMPARE(completeSpy[0][0], QVariant::fromValue(givenTest));
    QCOMPARE(completeSpy[0][1], QVariant::fromValue(expectedResult));

} // end void TestBit::testTestProcessingStartValid()

// ********************************************************************** */
void TestBit::testTestProcessingStartValid_data()
// ********************************************************************** */
{
    QTest::addColumn<TEST>("givenTest");
    QTest::addColumn<TEST>("expectedTest");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("IBIT-001 (Failure)" ) << IBIT_ONE    << IBIT_ONE    << false;
    QTest::newRow("IBIT-001 (Success)" ) << IBIT_ONE    << IBIT_ONE    << true;
    QTest::newRow("IBIT-002 (Failure)" ) << IBIT_TWO    << IBIT_TWO    << false;
    QTest::newRow("IBIT-002 (Success)" ) << IBIT_TWO    << IBIT_TWO    << true;
    QTest::newRow("IBIT-003 (Failure)" ) << IBIT_THREE  << IBIT_THREE  << false;
    QTest::newRow("IBIT-003 (Success)" ) << IBIT_THREE  << IBIT_THREE  << true;
    QTest::newRow("IBIT-004 (Failure)" ) << IBIT_FOUR   << IBIT_FOUR   << false;
    QTest::newRow("IBIT-004 (Success)" ) << IBIT_FOUR   << IBIT_FOUR   << true;
    QTest::newRow("IBIT-005 (Failure)" ) << IBIT_FIVE   << IBIT_FIVE   << false;
    QTest::newRow("IBIT-005 (Success)" ) << IBIT_FIVE   << IBIT_FIVE   << true;
    QTest::newRow("FTEST-001 (Failure)") << FTEST_ONE   << FTEST_ONE   << false;
    QTest::newRow("FTEST-001 (Success)") << FTEST_ONE   << FTEST_ONE   << true;
    QTest::newRow("FTEST-002 (Failure)") << FTEST_TWO   << FTEST_TWO   << false;
    QTest::newRow("FTEST-002 (Success)") << FTEST_TWO   << FTEST_TWO   << true;
    QTest::newRow("FTEST-003 (Failure)") << FTEST_THREE << FTEST_THREE << false;
    QTest::newRow("FTEST-003 (Success)") << FTEST_THREE << FTEST_THREE << true;
    QTest::newRow("FTEST-004 (Failure)") << FTEST_FOUR  << FTEST_FOUR  << false;
    QTest::newRow("FTEST-004 (Success)") << FTEST_FOUR  << FTEST_FOUR  << true;
    QTest::newRow("FTEST-005 (Failure)") << FTEST_FIVE  << FTEST_FIVE  << false;
    QTest::newRow("FTEST-005 (Success)") << FTEST_FIVE  << FTEST_FIVE  << true;
    QTest::newRow("FTEST-006 (Failure)") << FTEST_SIX   << FTEST_SIX   << false;
    QTest::newRow("FTEST-006 (Success)") << FTEST_SIX   << FTEST_SIX   << true;
    QTest::newRow("MBIT-001 (Failure)")  << MBIT_ONE    << FTEST_ONE   << false;
    QTest::newRow("MBIT-001 (Success)")  << MBIT_ONE    << FTEST_ONE   << true;
    QTest::newRow("MBIT-002 (Failure)")  << MBIT_TWO    << MBIT_TWO    << false;
    QTest::newRow("MBIT-002 (Success)")  << MBIT_TWO    << MBIT_TWO    << true;
    QTest::newRow("MBIT-003 (Failure)")  << MBIT_THREE  << IBIT_THREE  << false;
    QTest::newRow("MBIT-003 (Success)")  << MBIT_THREE  << IBIT_THREE  << true;
    QTest::newRow("MBIT-004 (Failure)")  << MBIT_FOUR   << FTEST_TWO   << false;
    QTest::newRow("MBIT-004 (Success)")  << MBIT_FOUR   << FTEST_TWO   << true;
    QTest::newRow("MBIT-005 (Failure)")  << MBIT_FIVE   << FTEST_THREE << false;
    QTest::newRow("MBIT-005 (Success)")  << MBIT_FIVE   << FTEST_THREE << true;
    QTest::newRow("MBIT-006 (Failure)")  << MBIT_SIX    << FTEST_FOUR  << false;
    QTest::newRow("MBIT-006 (Success)")  << MBIT_SIX    << FTEST_FOUR  << true;
    QTest::newRow("MBIT-007 (Failure)")  << MBIT_SEVEN  << MBIT_SEVEN  << false;
    QTest::newRow("MBIT-007 (Success)")  << MBIT_SEVEN  << MBIT_SEVEN  << true;
    QTest::newRow("MBIT-008 (Failure)")  << MBIT_EIGHT  << IBIT_FIVE   << false;
    QTest::newRow("MBIT-008 (Success)")  << MBIT_EIGHT  << IBIT_FIVE   << true;
    QTest::newRow("MBIT-009 (Failure)")  << MBIT_NINE   << IBIT_FOUR   << false;
    QTest::newRow("MBIT-009 (Success)")  << MBIT_NINE   << IBIT_FOUR   << true;
    QTest::newRow("MBIT-010 (Failure)")  << MBIT_TEN    << MBIT_TEN    << false;
    QTest::newRow("MBIT-010 (Success)")  << MBIT_TEN    << MBIT_TEN    << true;

} // end void TestBit::testTestProcessingStartValid_data()

// ********************************************************************** */
void TestBit::testTestProcessingStopActive()
// ********************************************************************** */
{
    // Arrange
    QFETCH(TEST, givenTest);

    QScopedPointer<MockTest> mockTest(new MockTest);
    mockTest->expect("test_TestProcessingStopSlot", givenTest);
    _bit->_ourCurrentTest.reset(mockTest.take());

    // Act
    _bit->test_TestProcessingStopSlot(givenTest);

    // Assert
    mockTest.reset((MockTest *)_bit->_ourCurrentTest.take());
    QCOMPARE(mockTest->countCalls("test_TestProcessingStopSlot", givenTest), 1);

} // end void TestBit::testTestProcessingStopActive()

// ********************************************************************** */
void TestBit::testTestProcessingStopActive_data()
// ********************************************************************** */
{
    QTest::addColumn<TEST>("givenTest");

    QTest::newRow("IBIT-001" ) << IBIT_ONE;
    QTest::newRow("IBIT-002" ) << IBIT_TWO;
    QTest::newRow("IBIT-003" ) << IBIT_THREE;
    QTest::newRow("IBIT-004" ) << IBIT_FOUR;
    QTest::newRow("IBIT-005" ) << IBIT_FIVE;
    QTest::newRow("FTEST-001") << FTEST_ONE;
    QTest::newRow("FTEST-002") << FTEST_TWO;
    QTest::newRow("FTEST-003") << FTEST_THREE;
    QTest::newRow("FTEST-004") << FTEST_FOUR;
    QTest::newRow("FTEST-005") << FTEST_FIVE;
    QTest::newRow("FTEST-006") << FTEST_SIX;
    QTest::newRow("MBIT-001")  << MBIT_ONE;
    QTest::newRow("MBIT-002")  << MBIT_TWO;
    QTest::newRow("MBIT-003")  << MBIT_THREE;
    QTest::newRow("MBIT-004")  << MBIT_FOUR;
    QTest::newRow("MBIT-005")  << MBIT_FIVE;
    QTest::newRow("MBIT-006")  << MBIT_SIX;
    QTest::newRow("MBIT-007")  << MBIT_SEVEN;
    QTest::newRow("MBIT-008")  << MBIT_EIGHT;
    QTest::newRow("MBIT-009")  << MBIT_NINE;
    QTest::newRow("MBIT-010")  << MBIT_TEN;

} // end void TestBit::testTestProcessingStopActive_data()

// ********************************************************************** */
void TestBit::testTestProcessingStopInactive()
// ********************************************************************** */
{
    // Arrange
    QFETCH(TEST, givenTest);

    // Act
    _bit->test_TestProcessingStopSlot(givenTest);

    // Assert
    // No specific actions are taken

} // end void TestBit::testTestProcessingStopInactive()

// ********************************************************************** */
void TestBit::testTestProcessingStopInactive_data()
// ********************************************************************** */
{
    QTest::addColumn<TEST>("givenTest");

    QTest::newRow("IBIT-001" ) << IBIT_ONE;
    QTest::newRow("IBIT-002" ) << IBIT_TWO;
    QTest::newRow("IBIT-003" ) << IBIT_THREE;
    QTest::newRow("IBIT-004" ) << IBIT_FOUR;
    QTest::newRow("IBIT-005" ) << IBIT_FIVE;
    QTest::newRow("FTEST-001") << FTEST_ONE;
    QTest::newRow("FTEST-002") << FTEST_TWO;
    QTest::newRow("FTEST-003") << FTEST_THREE;
    QTest::newRow("FTEST-004") << FTEST_FOUR;
    QTest::newRow("FTEST-005") << FTEST_FIVE;
    QTest::newRow("FTEST-006") << FTEST_SIX;
    QTest::newRow("MBIT-001")  << MBIT_ONE;
    QTest::newRow("MBIT-002")  << MBIT_TWO;
    QTest::newRow("MBIT-003")  << MBIT_THREE;
    QTest::newRow("MBIT-004")  << MBIT_FOUR;
    QTest::newRow("MBIT-005")  << MBIT_FIVE;
    QTest::newRow("MBIT-006")  << MBIT_SIX;
    QTest::newRow("MBIT-007")  << MBIT_SEVEN;
    QTest::newRow("MBIT-008")  << MBIT_EIGHT;
    QTest::newRow("MBIT-009")  << MBIT_NINE;
    QTest::newRow("MBIT-010")  << MBIT_TEN;

} // end void TestBit::testTestProcessingStopInactive_data()
// end requirement

QTEST_GUILESS_MAIN(TestBit)

#include "tst_testbit.moc"
// end requirement
