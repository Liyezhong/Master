#include <QString>
#include <QtTest>

#include "HimalayaDataContainer/Helper/Include/Global.h"
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"
#include "Scheduler/Include/IAction.h"
#include "Scheduler/Include/SessionManager.h"
#include "Scheduler/Include/Session.h"
#include "Scheduler/Include/ProtocolParser.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
//#include "Scheduler/Test/Mock/MockIDeviceProcessing.h"

namespace Scheduler{
class TestProtocolParser : public QObject
{
    Q_OBJECT

public:
    TestProtocolParser();

private Q_SLOTS:
    void testCase1();

private:
    Himalaya::HimalayaMasterThreadController* mp_MasterThreadController;
    DataManager::CDataManager* mp_DataManager;
    SessionManager* mp_SessionManager;
    ProtocolParser* mp_protocolParser;
    //MockIDeviceProcessing*                      mp_IDeviceProcessing;
    SchedulerCommandProcessorBase*  mp_SchedulerCommandProcessor;
    SchedulerMainThreadController*  mp_SchedulerMainController;
};

TestProtocolParser::TestProtocolParser()
{
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
    mp_MasterThreadController = new Himalaya::HimalayaMasterThreadController();
    mp_DataManager = new DataManager::CDataManager(mp_MasterThreadController);
    //mp_IDeviceProcessing = new MockIDeviceProcessing();
    mp_SchedulerMainController = new SchedulerMainThreadController(THREAD_ID_SCHEDULER);
    mp_SchedulerCommandProcessor = NULL;//new SchedulerCommandProcessor<IDeviceProcessing>(mp_SchedulerMainController);
    mp_SchedulerMainController->DataManager(mp_DataManager);
    //dynamic_cast<SchedulerCommandProcessor<MockIDeviceProcessing>*>(mp_SchedulerMainController->GetSchedCommandProcessor())->SetIDeviceProcessing(mp_IDeviceProcessing);
    mp_SessionManager = new SessionManager(mp_DataManager);
    mp_protocolParser = new ProtocolParser(mp_DataManager, mp_SchedulerCommandProcessor);
}

void TestProtocolParser::testCase1()
{
//    int sqn = mp_SessionManager->CreateSession("L03", "Retort_A");
//    Session* ss = mp_SessionManager->GetSessionById(sqn);
    CProgram* program = mp_DataManager->GetProgramList()->GetProgram("L03");
    qDebug()<<"******* step size:"<<program->GetNumberOfSteps();
    Session* ss = new Session("Retort_A", program);
    QVERIFY2(ss != NULL, "Failure");
    //qDebug()<<"**** retortID:"<<ss->GetRetortID();
    QVERIFY2(mp_protocolParser != nullptr, "Failure");


    bool ret = mp_protocolParser->GenerateActionList(ss);
    QVERIFY2(ret, "Failure");

    QList<QSharedPointer<IAction>> actionList = ss->GetActionList();
//    actionList.clear();
//    bool ret = mp_protocolParser->GenerateActionList("Retort_B", program, actionList);
    QVERIFY2(ret, "Failure");
    //QVERIFY2(actionList.size() == program->GetNumberOfSteps()*5, "Failure");
    qDebug()<<"***** action size:"<<actionList.size();

    foreach(QSharedPointer<IAction>action, actionList)
    {
        ActionType_t t = action.data()->GetActionType();
//        quint32 d = action.data()->GetDuration();
//        QString r = action.data()->GetReagentID();
//        QString s = action.data()->GetStationID();

//        qDebug()<<QString("**** action type:%1 station:%2, reagent:%3, duration:%4").arg(t).arg(s).arg(r).arg(d);

    }
}
}

//QTEST_APPLESS_MAIN(TestProtocolParser)
int main(int argc, char *argv[]) \
{ \
    QCoreApplication app(argc, argv); \
    app.setAttribute(Qt::AA_Use96Dpi, true); \
    Scheduler::TestProtocolParser tc; \
    QTEST_SET_MAIN_SOURCE_PATH \
    return QTest::qExec(&tc, argc, argv); \
}

#include "TestProtocolParser.moc"
