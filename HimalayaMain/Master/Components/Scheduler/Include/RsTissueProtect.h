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
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler{

class SchedulerMainThreadController;

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
        RELEASE_PRESSURE
    } StateList_t;

    typedef enum
    {
        UNKNOWN,
        Fixation,
        Fixation_Overflow,
        Concentration_Dehydration,
        Concentration_Dehydration_Overflow,
        Clearing,
        Clearing_Overflow,
        Paraffin,
        Paraffin_Overflow
    } ReagentType_t;

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor of class CRsTissueProtect
     *
     *  \param SchedController = pointer to SchedulerMainThreadController
     *
     */
    /****************************************************************************/
    CRsTissueProtect(SchedulerMainThreadController* SchedController);

    /****************************************************************************/
    /*!
     *  \brief  Desctruction of class CRsTissueProtect
     */
    /****************************************************************************/
    ~CRsTissueProtect();

    /****************************************************************************/
    /*!
     *  \brief  Get current state
     *
     *  \param statesList = QSet<QAbstractState*>
     *
     *	\return Internal state list
     */
    /****************************************************************************/
    CRsTissueProtect::StateList_t GetCurrentState(QSet<QAbstractState*> statesList);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for RS_Tissue_Protect
     *
     *  \param cmdName - command name
     *  \param retCode - return code
     *
     */
    /****************************************************************************/
    void HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

signals:

    /****************************************************************************/
    /*!
     *  \brief  Signal for stopping command execution
     *
     */
    /****************************************************************************/
    void StopCmdExec();

    /****************************************************************************/
    /*!
     *  \brief  Signal for draining current reagent
     *
     */
    /****************************************************************************/
    void DrainCurReagent();

    /****************************************************************************/
    /*!
     *  \brief  Signal for moving to tube position
     *
     */
    /****************************************************************************/
    void MoveToTube();

    /****************************************************************************/
    /*!
     *  \brief  Signal for heating level sensor
     *
     */
    /****************************************************************************/
    void LevelSensorHeating();

    /****************************************************************************/
    /*!
     *  \brief  Signal for filling
     *
     */
    /****************************************************************************/
    void Filling();

    /****************************************************************************/
    /*!
     *  \brief  Signal for wait for 8 seconds;
     *
     */
    /****************************************************************************/
    void Wait8Seconds();

    /****************************************************************************/
    /*!
     *  \brief  Signal for moving to sealing position
     *
     */
    /****************************************************************************/
    void MoveToSealing();

    /****************************************************************************/
    /*!
     *  \brief  Signal for releasing pressure
     *
     */
    /****************************************************************************/
    void ReleasePressure();

    /****************************************************************************/
    /*!
     *  \brief  Signal for Tasks done
     *  \param  flag = bool ture- tasks done successfully, false - tasks failed
     *
     */
    /****************************************************************************/
    void TasksDone(bool flag);

private slots:
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

private:
    SchedulerMainThreadController* mp_SchedulerController;  //!< Pointer to SchedulerMainThreadController
    QSharedPointer<QStateMachine>   mp_StateMachine;        //!< State machine for RS_Tissue_Protect
    QSharedPointer<QState> mp_Init;                         //!< Initial state
    QSharedPointer<QState> mp_StopCmdExec;         			//!< Stop command execution state
    QSharedPointer<QState> mp_DrainCurReagent;              //!< Drain current reagent state
    QSharedPointer<QState> mp_MoveToTube;         			//!< Move to Tube position state
    QSharedPointer<QState> mp_LevelSensorHeating;         	//!< Level Sensor Heating state
    QSharedPointer<QState> mp_Filling;           			//!< Filling state
    QSharedPointer<QState> mp_Wait8S;           			//!< Wait for 8 seconds state
    QSharedPointer<QState> mp_MoveToSealing;       			//!< Move to Sealing positon state
    QSharedPointer<QState> mp_ReleasePressure;       		//!< Release pressure state

    bool m_IsLevelSensorRelated;                            //!< flag to indicate if the error is related to level sensor
    QString m_StationID;                                    //!< Station ID
    QString m_ReagentGroup;                                 //!< Reagent group
    quint8 m_DrainCurReagentSeq;                            //!< Sequnece of draining current reagent
    quint8 m_MoveToTubeSeq;                                 //!< Sequnece of Moving to tube
    quint8 m_FillSeq;                                       //!< Sequence of Filling
    quint8 m_LevelSensorSeq;                                //!< Sequence of Level sensor heating
    quint8 m_MoveToSealSeq;                                 //!< Sequnece of Moving to Sealing position
    qint64 m_StartWaitTime;                                 //!< start up time for wait
private:
    CRsTissueProtect(const CRsTissueProtect& rhs);
    CRsTissueProtect& operator=(const CRsTissueProtect& rhs);
    CRsTissueProtect::ReagentType_t GetReagentType();
    QString GetStationID();
};
}
#endif // RS_TISSUE_PROTECT_H
