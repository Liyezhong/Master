/****************************************************************************/
/*! \file RsTissueProtect.h
 *
 *  \brief RsTissueProtect class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ Auguest 8th, 2014
 *   $Author:  $ Songtao Yu
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
#ifndef RS_TISSUE_PROTECT_H
#define RS_TISSUE_PROTECT_H
#include <QStateMachine>
#include <QSharedPointer>
#include <QCoreApplication>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler{

class SchedulerMainThreadController;
class CSchedulerStateMachine;

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_Tissue_Protect_* patterns
 */
/****************************************************************************/
class  CRsTissueProtect : public QObject
{
    Q_OBJECT

    //!< state list of the state machine
    typedef enum
    {
        UNDEF,
        INIT,
        STOP_CMDEXEC,
        DRAIN_CUR_REAGENT,
        MOVE_TO_TUBE,
        LEVELSENSOR_HEATING,
        FILLING,
        WAIT_8S,
        MOVE_TO_SEALING,
        RELEASE_PRESSURE,
        PROCESSING_SAFE_REAGENT,
        DRAIN_SAFE_REAGENT,
        SAFE_REAGENT_FINISHED
    } StateList_t;

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor of class CRsTissueProtect
     *
     *  \param SchedController = pointer to SchedulerMainThreadController
     *  \param StateMachine = pointer to CSchedulerStateMachine
     *
     */
    /****************************************************************************/
    CRsTissueProtect(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine);

    /****************************************************************************/
    /*!
     *  \brief  Desctruction of class CRsTissueProtect
     */
    /****************************************************************************/
    ~CRsTissueProtect();

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for RS_Tissue_Protect
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *  \param ctrlCmd - controlCommandType_t
     *
     */
    /****************************************************************************/
    void HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode, ControlCommandType_t ctrlCmd);


    /****************************************************************************/
    /*!
     *  \brief start up state machine
     */
    /****************************************************************************/
    void Start();

private:
    /****************************************************************************/
    /*!
     *  \brief  slot for moving to safe reagent tube position
     *  \return void
     */
    /****************************************************************************/
    void OnMoveToTube();

    /****************************************************************************/
    /*!
     *  \brief  slot for moving to safe reagent sealing position
     *  \param  void
     *  \return void
     *
     */
    /****************************************************************************/
    void OnMoveToSeal();

    /****************************************************************************/
    /*!
     *  \brief  slot for releasing pressure
     *  \param  void
     *  \return void
     *
     */
    /****************************************************************************/
    void OnReleasePressure();

    /****************************************************************************/
    /*!
     *  \brief  Tasks done
     *  \param  flag = bool ture- tasks done successfully, false - tasks failed
     *
     */
    /****************************************************************************/
    void SendTasksDoneSig(bool flag);

    /****************************************************************************/
    /*!
     *  \brief  try to suck from next station of safe reagent
     *  \return indicates whether execute the filling or not
     *
     */
    /****************************************************************************/
    bool TryNextSafeReagent();

private:
    SchedulerMainThreadController* mp_SchedulerController;  //!< Pointer to SchedulerMainThreadController
    CSchedulerStateMachine*        mp_StateMachine;         //!< Pointer to CSchedulerStateMachine
    bool m_IsLevelSensorRelated;                            //!< flag to indicate if the error is related to level sensor
    QString m_StationID;                                    //!< Station ID
    QString m_ReagentGroup;                                 //!< Reagent group
    quint8 m_DrainCurReagentSeq;                            //!< Sequnece of draining current reagent
    quint8 m_MoveToTubeSeq;                                 //!< Sequnece of Moving to tube
    quint8 m_FillSeq;                                       //!< Sequence of Filling
    quint8 m_LevelSensorSeq;                                //!< Sequence of Level sensor heating
    quint8 m_MoveToSealSeq;                                 //!< Sequnece of Moving to Sealing position
    quint8 m_ReleasePressure;                               //!< Sequnece of release pressure
    quint8 m_DrainSafeReagent;                              //!< Sequnece of drain safe reagent
    quint8 m_ProcessingSafeReagent;                         //!< Sequnece of processing safe reagent
    qint64 m_StartWaitTime;                                 //!< start up time for wait
    bool   m_IsSafeReagentSuccessful;                       //!< flag to indicate if safe reagent is successful or not
    bool   m_retryNextStation;                              //!< flag to indicate whether system will suck the next station safe reagent or not
    StateList_t m_CurrentStep;                              //!< current step
    QList<QString> m_safeReagentStations;                   //!< hold all safe reagent
    int    m_CurSafeReagentIndex;                           //!< which one is being used in the safe reagent

private:
    CRsTissueProtect(const CRsTissueProtect& rhs);
    CRsTissueProtect& operator=(const CRsTissueProtect& rhs);
    QString GetCurReagentType(const QString& lastReagentGroup);
    QString GetStationID();
};
}
#endif // RS_TISSUE_PROTECT_H
