#include "Scheduler/Include/ProtocolParser.h"
#include "Scheduler/Include/Actions/FillAction.h"
//#include "Scheduler/Include/Actions/DrainAction.h"
//#include "Scheduler/Include/Actions/PurgeAction.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include <QDebug>

namespace Scheduler{

ProtocolParser::ProtocolParser(DataManager::CDataManager* dataManager, SchedulerCommandProcessorBase* commandProcessor):
    mp_DataManager(dataManager), mp_SchedulerCommandProcessor(commandProcessor)
{
    m_StepActionTypes<<FILLING<<PURGE<<SOAKING<<DRAINING<<PURGE;
}

ProtocolParser::~ProtocolParser()
{

}

bool ProtocolParser::GenerateActionList(Session *session)
{
    if (!session)
    {
        return false;
    }

    return GenerateActionList(session, session->GetProgram(), session->GetActionList());
}

bool ProtocolParser::GenerateActionList(Session* session, const CProgram* program, QList<QSharedPointer<IAction>>& actionList)
{
    if (!program)
    {
        return false;
    }

    CDashboardDataStationList* pDashboardDataStationList = mp_DataManager->GetStationList();
    if (!pDashboardDataStationList)
    {
        Q_ASSERT(false);
        return false;
    }

    int stepNum = program->GetNumberOfSteps();
    if (0 == stepNum)
    {
        return false;
    }

    actionList.clear();
    ListOfIDs_t unusedStationIDs = pDashboardDataStationList->GetOrderedListOfDashboardStationIDs();
    for (int i = 0; i < stepNum; ++i)
    {
        const CProgramStep* step = program->GetProgramStep(i);
        if (false == GenerateActionList(session, step, unusedStationIDs, actionList))
        {
            return false;
        }
    }

    session->SetStatus(Session::Ready);

    return true;
}

 bool ProtocolParser::GenerateActionList(Session* session, const CProgramStep* programStep, ListOfIDs_t& unusedStationIDs, QList<QSharedPointer<IAction>>& actionList)
 {
    if (!programStep)
    {
        return false;
    }

    bool isLastStep = false;

    QList<StationUseRecord_t> usedStations;
    QString reagentID = programStep->GetReagentID();
    QString stationID = SelectStationFromReagentID(reagentID, unusedStationIDs, usedStations, isLastStep);

//    QSharedPointer<IAction> changeReagent = QSharedPointer<IAction>(new ChangeReagentAction(mp_SchedulerCommandProcessor, retortID));
//    changeReagent->SetReagentID(reagentID);
//    changeReagent->SetStationID(stationID);
//    actionList.append(changeReagent);

    foreach (ActionType_t type, m_StepActionTypes)
    {
        QSharedPointer<IAction> action = QSharedPointer<IAction>(CreateAction(type, session));

        action.data()->SetReagentID(reagentID);
        action.data()->SetStationID(stationID);
        quint32 duration = 0;
        if (SOAKING == type)
        {
            duration = programStep->GetDurationInSeconds();
        }
        else
        {
            duration = GetDruationTime(type);
        }
        action.data()->SetDuration(duration);
        actionList.append(action);
    }
    return true;
 }

 quint32 ProtocolParser::GetDruationTime(ActionType_t type)
 {
    quint32 druation = 0;
    switch(type)
    {
    case FILLING:
        druation = TIME_FOR_HEATING_LEVEL_SENSOR + TIME_FOR_FILLING + TIME_FOR_MOVE_SEAL;
        break;
    case DRAINING:
        druation = TIME_FOR_MOVE_TUBE + TIME_FOR_DRAIN;
        break;
    case PURGE:
        druation = TIME_FOR_PURGE;
        break;
    default:
        break;
    }

    return druation;
 }

 QString ProtocolParser::SelectStationFromReagentID(const QString& ReagentID, ListOfIDs_t& unusedStationIDs,
                                                                  QList<StationUseRecord_t>& usedStations, bool isLastStep)
 {
     if (!mp_DataManager)
         return "";

     DataManager::CHimalayaUserSettings* pUserSetting = mp_DataManager->GetUserSettings();
     Global::RMSOptions_t rmsMode = Global::RMS_UNDEFINED;
     if (this->IsCleaningReagent(ReagentID))
     {
         rmsMode = pUserSetting->GetModeRMSCleaning();
     }
     else
     {
         rmsMode = pUserSetting->GetModeRMSProcessing();
     }

     if (CDataReagentList* pReagentList =  mp_DataManager->GetReagentList())
     {
         const CReagent* pReagent = pReagentList->GetReagent(ReagentID);
         return this->SelectStationByReagent(pReagent,  unusedStationIDs, usedStations, isLastStep, rmsMode);
     }

     return "";
 }

 QString ProtocolParser::SelectStationByReagent(const CReagent* pReagent,
                                                                     ListOfIDs_t& unusedStationIDs,
                                                                     QList<StationUseRecord_t>& usedStations,
                                                                     bool bFindNewestOne,
                                                                     Global::RMSOptions_t rmsMode) const
 {
     Q_UNUSED(bFindNewestOne);
     Q_UNUSED(usedStations);
     Q_UNUSED(rmsMode);

     if (!pReagent)
         return "";

     CDashboardDataStationList* pDashboardDataStationList = mp_DataManager->GetStationList();
     if (!pDashboardDataStationList)
         return "";

     CDataReagentGroupList* pDataReagentGroupList = mp_DataManager->GetReagentGroupList();
     if (!pDataReagentGroupList)
         return "";

     const QString& ReagentID = pReagent->GetReagentID();
     bool isCleaningReagent = pDataReagentGroupList->GetReagentGroup(pReagent->GetGroupID())->IsCleaningReagentGroup();

     //qDebug()<<"***** station id size:"<<unusedStationIDs.size();
     for (int i = 0; i < unusedStationIDs.count(); i++)
     {
         CDashboardStation* pDashboardStation = pDashboardDataStationList->GetDashboardStation(unusedStationIDs.at(i));
         if (pDashboardStation->GetDashboardReagentID() == ReagentID)
         {
                 if (!isCleaningReagent)
                 {
                     unusedStationIDs.removeAt(i);
                 }
                 return pDashboardStation->GetDashboardStationID();
         }
     }
     return "";
 }

 bool ProtocolParser::IsCleaningReagent(const QString& ReagentID)
 {
     if (!mp_DataManager)
     {
         return false;
         Q_ASSERT(0);
     }

     if (CDataReagentList* pReagentList =  mp_DataManager->GetReagentList())
     {
         const CReagent* pReagent = pReagentList->GetReagent(ReagentID);
         if (CReagentGroup*  reagentGroup = mp_DataManager->GetReagentGroupList()->GetReagentGroup(pReagent->GetGroupID()))
         {
             return reagentGroup->IsCleaningReagentGroup();
         }
     }
     return false;
 }

 IAction* ProtocolParser::CreateAction(ActionType_t type, Session* session, bool isLastStep)
 {
     IAction* action = NULL;
     switch (type) {
     case FILLING:
         action = new FillAction(mp_SchedulerCommandProcessor, session);
         break;
     case DRAINING:
         //action = new DrainAction(mp_SchedulerCommandProcessor, session, isLastStep);
         break;
     default:
         break;
     }

     return action;
 }

}
