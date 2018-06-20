/****************************************************************************/
/*! \file ProtocolParser.h
 *
 *  \brief ProtocolParser class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ May 17th, 2018
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2014 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H

#include <QList>
#include <QSharedPointer>
#include "Scheduler/Include/Actions/IAction.h"
#include "Scheduler/Include/Session.h"
#include "HimalayaDataContainer/Containers/Programs/Include/ProgramStep.h"
#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"
#include "HimalayaDataManager/Include/DataManager.h"

using namespace DataManager;

namespace Scheduler {

class SchedulerCommandProcessorBase;

#define TIME_FOR_CLEANING_DRY_STEP            720    ///< seconds spending in dry step
#define TIME_FOR_HEATING_LEVEL_SENSOR         30     ///< seconds for heating level sensor
#define TIME_FOR_FILLING                      60     ///< seconds for filling
#define TIME_FOR_MOVE_SEAL                    3      ///< seconds for move seal
#define TIME_FOR_PRESSURE_CHECK               15     ///< seconds for pressure check before move tube
#define TIME_FOR_MOVE_TUBE                    5      ///< seconds for move tube
#define TIME_FOR_DRAIN                        60     ///< seconds for draing
#define TIME_FOR_MOVE_NEXT_TUBE               8      ///< seconds for move next tube
#define TIME_FOR_COOLING_DOWN                 1200   ///< seconds for cooling down
#define TIME_FOR_PURGE                        20     ///< seconds for purge

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct StationUseRecord
 */
/****************************************************************************/
typedef struct
{
    QString StationID;   ///<  Definition/Declaration of variable StationID
    int UsedTimes;       ///<  Definition/Declaration of variable UsedTimes

}StationUseRecord_t;

//typedef enum
//{
//    TYPE_UNDEFIN = 0,
//    FILLING,
//    DRAINING,
//    PURGE,
//    SOAKING
//}ActionType_t;


class ProtocolParser
{
public:
    ProtocolParser(DataManager::CDataManager* dataManager, SchedulerCommandProcessorBase* commandProcessor);
    virtual ~ProtocolParser();

    bool GenerateActionList(Session* session);
    bool GenerateActionList(Session* session, const CProgram* program, QList<QSharedPointer<IAction>>& actionList);

private:
    bool GenerateActionList(Session* session, const CProgramStep* programStep, ListOfIDs_t& unusedStationIDs,
                            QList<QSharedPointer<IAction>>& actionList, bool isLastStep);

    quint32 GetDruationTime(ActionType_t type);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SelectStationFromReagentID
     *
     *  \param ReagentID = const QString type parameter
     *  \param unusedStationIDs
     *  \param usedStations
     *  \param isLastStep
     *
     *  \return from SelectStationFromReagentID
     */
    /****************************************************************************/
    QString SelectStationFromReagentID(const QString& ReagentID,
                                      ListOfIDs_t& unusedStationIDs,
                                      QList<StationUseRecord_t>& usedStations,
                                      bool isLastStep);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SelectStationByReagent
     *
     *  \param pReagent = const DataManager::CReagent type parameter
     *  \param unusedStationIDs
     *  \param usedStations
     *  \param bFindNewestOne
     *  \param rmsMode
     *
     *  \return from SelectStationByReagent
     */
    /****************************************************************************/
    QString SelectStationByReagent(const DataManager::CReagent* pReagent,
                                                                        ListOfIDs_t& unusedStationIDs,
                                                                        QList<StationUseRecord_t>& usedStations,
                                                                        bool bFindNewestOne,
                                                                        Global::RMSOptions_t rmsMode) const;

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsCleaningReagent
     *
     *  \param ReagentID = const QString type parameter
     *
     *  \return from IsCleaningReagent
     */
    /****************************************************************************/
    bool IsCleaningReagent(const QString& ReagentID);

    IAction* CreateAction(ActionType_t type, Session* session, bool isLastStep = false);

private:
    QList<ActionType_t> m_StepActionTypes;
    DataManager::CDataManager* mp_DataManager;
    SchedulerCommandProcessorBase*  mp_SchedulerCommandProcessor;       ///<  Definition/Declaration of variable m_SchedulerCommandProcessor

};

}
#endif // PROTOCOLPARSER_H
