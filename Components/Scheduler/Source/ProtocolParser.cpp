#include "Scheduler/Include/ProtocolParser.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"

namespace Scheduler{

ProtocolParser::ProtocolParser(DataManager::CDataManager* dataManager, SchedulerCommandProcessorBase* commandProcessor):
    mp_DataManager(dataManager), mp_SchedulerCommandProcessor(commandProcessor)
{

}

ProtocolParser::~ProtocolParser()
{

}

void ProtocolParser::GenerateActionList(Session *session)
{
    if (!session)
    {
        return;
    }

    GenerateActionList(session->GetRetortID(), session->GetProgram(), session->GetActionList());
}

void ProtocolParser::GenerateActionList(const QString& retortID, const CProgram* program, QList<QSharedPointer<IAction>>& actionList)
{
    if (!program)
    {
        return;
    }

    int stepNum = program->GetNumberOfSteps();
    if (0 == stepNum)
    {
        return;
    }

    actionList.clear();
    for (int i = 0; i < stepNum; ++i)
    {
        const CProgramStep* step = program->GetProgramStep(i);
        GenerateActionList(retortID, step, actionList);
    }

}

 void ProtocolParser::GenerateActionList(const QString& retortID, const CProgramStep* programStep, QList<QSharedPointer<IAction>>& actionList)
 {
    if (!programStep)
    {
        return;
    }

    CDashboardDataStationList* pDashboardDataStationList = mp_DataManager->GetStationList();
    if (!pDashboardDataStationList)
    {
        Q_ASSERT(false);
        return;
    }

    bool isLastStep = false;

    ListOfIDs_t unusedStationIDs = pDashboardDataStationList->GetOrderedListOfDashboardStationIDs();
    QList<StationUseRecord_t> usedStations;

    for (int i = (int)FILLING; i < ACTION_SUM; ++i)
    {
        ActionType_t type = (ActionType_t)i;
        QSharedPointer<IAction> action = QSharedPointer<IAction>(new IAction(mp_SchedulerCommandProcessor, retortID, type));
        QString reagentID = programStep->GetReagentID();
        QString stationID = SelectStationFromReagentID(reagentID, unusedStationIDs, usedStations, isLastStep);

        action.data()->SetReagentID(reagentID);
        action.data()->SetStationID(stationID);
        quint32 duration = 0;
        if (SOAKING == type)
        {
            duration = programStep->GetDuration().toUInt();
        }
        else
        {
            duration = GetDruationTime(type);
        }
        action.data()->SetDuration(duration);
        actionList.append(action);
    }
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

     for (int i = 0; i < unusedStationIDs.count(); i++)
     {
         CDashboardStation* pDashboardStation = pDashboardDataStationList->GetDashboardStation(unusedStationIDs.at(i));
         if (pDashboardStation->GetDashboardReagentID() == ReagentID)
         {
                 if (!isCleaningReagent)
                     unusedStationIDs.removeAt(i);
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

}
