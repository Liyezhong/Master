#include <QString>
#include <QtTest>
#include "Scheduler/Include/InstrumentManager.h"
#include "HimalayaDataContainer/Helper/Include/Global.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include <QSignalSpy>
#include "Scheduler/Include/EventDispatcher.h"
#include "Scheduler/Include/TPEvent.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"
#include "Scheduler/Commands/Include/CmdRVSetTemperatureSwitchState.h"
#include "Scheduler/Include/SessionManager.h"
#include "Scheduler/Include/TPExecutor.h"

#ifndef Q_OS_WIN
#include <sys/time.h>
#endif

namespace Scheduler{
#define QCOREAPPLICATION_EXEC(TIMEOUT) \
{ \
    QTimer timer; \
    timer.setSingleShot(true); \
    timer.setInterval(TIMEOUT); \
    timer.start(); \
    connect(&timer, SIGNAL(timeout()), QCoreApplication::instance(), SLOT(quit())); \
    QCoreApplication::exec(); \
}

#define SLEEP(TIMEOUT, CONDITIONS) \
{ \
    while (CONDITIONS) \
    { \
        QCoreApplication::processEvents(QEventLoop::AllEvents, TIMEOUT); \
    } \
}

class TestInstrumentManager : public QObject
{
    Q_OBJECT

public:
    TestInstrumentManager();
    ~TestInstrumentManager();

protected Q_SLOTS:
    void OnIdleEntered();
    void OnSchedulingEntered();
    void OnBusyEntered();
    void OnDCLConfigurationDone();
private Q_SLOTS:
    void testCase1();
    void utHandleGuiCmd();
    void utInstrumentSelfTestDone();
    void utInstrumentHandleLoadRequest();
    void utInstrumentHandleStartRequest();
    void utTPExecutorHandleStartRequest();
    void utTPExecutorProtocolCompleted();


private:
    InstrumentManager* m_pInstrumentManager;
    EventDispatcher* m_pEventDispatcher;
    QThread* m_pTestThread;
    bool IdleEntered;
    bool SchedulingEntered;
    bool BusyEntered;
    bool configed;

    Himalaya::HimalayaMasterThreadController* mp_HMThreadController;
    SchedulerMainThreadController* m_pController;
    DataManager::CDataManager* m_pDataManager;
    SessionManager* m_pSessionManager;
};

TestInstrumentManager::TestInstrumentManager()
{
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
    m_pController = new Scheduler::SchedulerMainThreadController(0x0103);
    mp_HMThreadController = new Himalaya::HimalayaMasterThreadController();
    m_pDataManager = new DataManager::CDataManager(mp_HMThreadController);
    m_pController->DataManager(m_pDataManager);
    m_pController->CreateAndInitializeObjects();
    m_pTestThread = new QThread();
    m_pEventDispatcher = m_pController->m_pEventDispatcher;
//    m_pEventDispatcher->moveToThread(m_pTestThread);
    m_pTestThread->start();
    m_pSessionManager = new SessionManager(m_pDataManager);
    m_pInstrumentManager = new InstrumentManager("Common", m_pEventDispatcher, m_pController, m_pSessionManager);
    IdleEntered = false;
    SchedulingEntered = false;
    BusyEntered = false;
    configed = false;
}

void TestInstrumentManager::testCase1()
{
    // arrange
    QList<QString> retorts;
    retorts<<"Retort_A";
    retorts<<"Retort_B";

    // act
    m_pInstrumentManager->Initialize(retorts);

    // assert
    QVERIFY2(m_pInstrumentManager->GetTPExecutor("Retort_B") != nullptr, "Failure");

}

void TestInstrumentManager::utHandleGuiCmd()
{
    // arrange

    auto event = TPEventArgs<Global::CommandShPtr_t>::CreateEvent(
                Scheduler::SchedulerCommandShPtr_t(new Scheduler::CmdRVSetTemperatureSwitchState(500, "Retort_A")));
    m_pEventDispatcher->IncomingEvent(event);

    // act
    m_pEventDispatcher->OnTickTimer();

    // assert
     QVERIFY2(m_pInstrumentManager->GetTPExecutor("B") != nullptr, "Failure");
}

void TestInstrumentManager::utInstrumentSelfTestDone()
{
    // arrange

    QObject::connect(m_pInstrumentManager->m_pIdle, SIGNAL(entered()), this, SLOT(OnIdleEntered()));

    // act
    while (1)
    {
#ifdef Q_OS_WIN
    Sleep(uint(100));
#else
    struct timespec ts = { 100 / 1000, (100 % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
        QCoreApplication::processEvents();
        if (!m_pController->GetHeatingStrategy().isNull())
        {
            break;
        }
    }
    m_pInstrumentManager->Start();


    while (!IdleEntered || !m_pController->IsSelfTestDone())
    {

#ifdef Q_OS_WIN
    Sleep(uint(100));
#else
    struct timespec ts = { 1000 / 1000, (100 % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif

        QCoreApplication::processEvents(QEventLoop::AllEvents, 500);
    }

    // assert
    QVERIFY2(m_pInstrumentManager->GetTPExecutor("B") != nullptr, "Failure");
}

void TestInstrumentManager::utInstrumentHandleLoadRequest()
{
    // act
    QObject::connect(m_pInstrumentManager->m_pScheduling, SIGNAL(entered()), this, SLOT(OnSchedulingEntered()));
//    auto req = new TPCmdEvent<Global::CommandShPtr_t>("Common", Global::CommandShPtr_t(new MsgClasses::CmdProgramSelected("Common", 500, "L01", 0)));

    m_pEventDispatcher->IncomingEvent(TPEventArgs<Global::CommandShPtr_t>::CreateEvent("Common", Global::CommandShPtr_t(new MsgClasses::CmdProgramSelected("Common", 500, "L01", 0))));

    while (!SchedulingEntered)
    {

#ifdef Q_OS_WIN
    Sleep(uint(100));
#else
    struct timespec ts = { 1000 / 1000, (100 % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif

        QCoreApplication::processEvents(QEventLoop::AllEvents, 500);
    }

    // assert
    QAbstractState* pScheduling = nullptr;
    foreach(auto p, m_pInstrumentManager->m_pStateMachine->configuration())
    {
        qDebug() << p->objectName();
        pScheduling = p;
    }

    QVERIFY2(pScheduling->objectName() == "Instrument_Scheduling_State", "Failure");
}

void TestInstrumentManager::utInstrumentHandleStartRequest()
{
    // act
    QObject::connect(m_pInstrumentManager->m_pBusy, SIGNAL(entered()), this, SLOT(OnBusyEntered()));
    m_pEventDispatcher->IncomingEvent(TPEventArgs<Global::CommandShPtr_t>
                                      ::CreateEvent("Common", Global::CommandShPtr_t(
                                                        new MsgClasses::CmdProgramAction("Retort_A", 500, "L01", DataManager::ProgramActionType_t::PROGRAM_START, 500, 6000, ""))));

    while (!BusyEntered)
    {

#ifdef Q_OS_WIN
    Sleep(uint(100));
#else
    struct timespec ts = { 1000 / 1000, (100 % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif

        QCoreApplication::processEvents(QEventLoop::AllEvents, 500);
    }

    // assert
    QAbstractState* pBusy = nullptr;
    foreach(auto p, m_pInstrumentManager->m_pStateMachine->configuration())
    {
        qDebug() << p->objectName();
        pBusy = p;
    }

    QVERIFY2(pBusy->objectName() == "Instrument_Busy_State", "Failure");
}

void TestInstrumentManager::utTPExecutorHandleStartRequest()
{
    // arrange
    BusyEntered = false;

    // act
    QObject::connect(m_pInstrumentManager->GetTPExecutor("Retort_A")->m_pBusy, SIGNAL(entered()), this, SLOT(OnBusyEntered()));
    m_pEventDispatcher->IncomingEvent(TPEventArgs<Global::CommandShPtr_t>
                                      ::CreateEvent("Common", Global::CommandShPtr_t(
                                                        new MsgClasses::CmdProgramAction("Retort_A", 500, "L01", DataManager::ProgramActionType_t::PROGRAM_START, 500, 6000, ""))));

    SLEEP(500, !BusyEntered)

    // assert
    QAbstractState* pBusy = nullptr;
    foreach(auto p, m_pInstrumentManager->GetTPExecutor("Retort_A")->m_pStateMachine->configuration())
    {
        qDebug() << p->objectName();
        pBusy = p;
    }

    QVERIFY2(pBusy->objectName() == "TPExecutor_NextAction_State", "Failure");
}

void TestInstrumentManager::utTPExecutorProtocolCompleted()
{
    // arrange
    IdleEntered = false;

    // act
    QObject::connect(m_pInstrumentManager->GetTPExecutor("Retort_A")->m_pIdle, SIGNAL(entered()), this, SLOT(OnIdleEntered()));

    SLEEP(500, !IdleEntered)

    // assert
    QAbstractState* pIdle = nullptr;
    foreach(auto p, m_pInstrumentManager->GetTPExecutor("Retort_A")->m_pStateMachine->configuration())
    {
        qDebug() << p->objectName();
        pIdle = p;
    }

    QVERIFY2(pIdle->objectName() == "TPExecutor_Idle_State", "Failure");
}


void TestInstrumentManager::OnIdleEntered()
{
    IdleEntered = true;
}

void TestInstrumentManager::OnSchedulingEntered()
{
    SchedulingEntered = true;
}

void TestInstrumentManager::OnBusyEntered()
{
    BusyEntered = true;
}

void TestInstrumentManager::OnDCLConfigurationDone()
{
    configed = true;
}

TestInstrumentManager::~TestInstrumentManager()
{
    delete m_pController;
    delete mp_HMThreadController;
    delete m_pDataManager;
    delete m_pInstrumentManager;
}
}

//QTEST_MAIN(TestInstrumentManager)
int main(int argc, char *argv[]) \
{ \
    QCoreApplication app(argc, argv); \
    app.setAttribute(Qt::AA_Use96Dpi, true); \
    Scheduler::TestInstrumentManager tc; \
    QTEST_SET_MAIN_SOURCE_PATH \
    return QTest::qExec(&tc, argc, argv); \
}

#include "TestInstrumentManager.moc"
