/****************************************************************************/
/*! \file RsStandbyWithTissue.h
 *
 *  \brief RsStandby class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ April 22nd, 014
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
#ifndef RSSTANDBY_WITH_TISSUE_H
#define RSSTANDBY_WITH_TISSUE_H
#include <QStateMachine>
#include <QSharedPointer>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Scheduler{

class SchedulerMainThreadController;

/****************************************************************************/
/*!
 * \brief Error Handling State machine for RS_Standby_WithTissue
 */
/****************************************************************************/
class  CRsStandbyWithTissue : public QObject
{
    Q_OBJECT

    //!< state list of the state machine
    typedef enum
    {
        UNDEF,
        SHUTDOWN_FAILD_HEATER,
        RTBOTTOM_STOP_TEMPCTRL,
        RTSIDE_STOP_TEMPCTRL,
        CHECK_TEMPMODULE_CURRENT,
        RELEASE_PRESSURE
    } StateList_t;

    typedef enum
    {
        UNKNOWN,
        LEVELSENSOR,
        LATUBE1,
        LATUBE2,
        RETORT,
        OVEN,
        RV
    } HeaterType_t;
public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor of class CRsStandbyWithTissue
     *
     *  \param SchedController = pointer to SchedulerMainThreadController
     *
     */
    /****************************************************************************/
    CRsStandbyWithTissue (SchedulerMainThreadController* SchedController);

    /****************************************************************************/
    /*!
     *  \brief  Desctruction of class CRsStandbyWithTissue
     */
    /****************************************************************************/
    ~CRsStandbyWithTissue();

    /****************************************************************************/
    /*!
     *  \brief  Get current state
     *
     *  \param statesList = QSet<QAbstractState*>
     *
     *	\return Internal state list
     */
    /****************************************************************************/
    CRsStandbyWithTissue::StateList_t GetCurrentState(QSet<QAbstractState*> statesList);

    /****************************************************************************/
    /*!
     *  \brief Handle the whole work flow for RS_Standby_WithTissue
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
     *  \brief  Signal for stopping RTBottom temperature control
     *
     */
    /****************************************************************************/
    void StopRTBottomTempCtrl();

    /****************************************************************************/
    /*!
     *  \brief  Signal for stopping RTSide temperature control
     *
     */
    /****************************************************************************/
    void StopRTSideTempCtrl();

    /****************************************************************************/
    /*!
     *  \brief  Signal for checking RTBottom temperature
     *
     */
    /****************************************************************************/
    void CheckTempModuleCurrernt();

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
     *  \param  bool ture- tasks done successfully, false - tasks failed
     *
     */
    /****************************************************************************/
    void TasksDone(bool);
private slots:

    /****************************************************************************/
    /*!
     *  \brief  Slot for releasing pressure
     *  \param  void
     *  \return void
     *
     */
    /****************************************************************************/
    void OnReleasePressure();

    /****************************************************************************/
    /*!
     *  \brief  Slot for entering state of checking temperature module status
     *  \param  void
     *  \return void
     *
     */
    /****************************************************************************/
    void OnEnterCheckingTempModuleCurrent() { m_StartCheckingTime = QDateTime::currentMSecsSinceEpoch(); }


    /****************************************************************************/
    /*!
     *  \brief  Slot for the time of shutting down heaters
     *  \param  void
     *  \return void
     *
     */
    /****************************************************************************/
    void OnShutdownHeaters() { m_ShutdownHeatersTime = QDateTime::currentMSecsSinceEpoch(); }

private:
    SchedulerMainThreadController* mp_SchedulerController;  //!< Pointer to SchedulerMainThreadController
    QSharedPointer<QStateMachine>   mp_StateMachine;        //!< State machine for RS_Standby_WithTissue
    QSharedPointer<QState> mp_ShutdownFailedHeater;         //!< Shutdown failed heater state
    QSharedPointer<QState> mp_RTBottomStopTempCtrl;         //!< RT Bottom stop tempature control state
    QSharedPointer<QState> mp_RTSideStopTempCtrl;           //!< RT Top stop tempatrue control state
    QSharedPointer<QState> mp_CheckTempModuleCurrent;       //!< Check the current of temperature module status (if error message was raised not not)
    QSharedPointer<QState> mp_ReleasePressure;              //!< Release pressure
    qint64                m_ShutdownHeatersTime;           //!< Time when the failed heaters have been down
    qint64                m_StartCheckingTime;             //!< start-up time for checking temperature module current status
private:
    /****************************************************************************/
    /*!
     *  \brief  Get the tpe of failed heater
     *  \param  void
     *  \return Heater type
     *
     */
    /****************************************************************************/
    CRsStandbyWithTissue::HeaterType_t GetFailerHeaterType();

    /****************************************************************************/
    /*!
     *  \brief  Shutting down failed heaters
     *  \param  void
     *  \return bool, ture - success, false -failure
     *
     */
    /****************************************************************************/
    bool ShutdownHeaters();


    /****************************************************************************/
    /*!
     *  \brief  Check temperature modules current
     *  \param  void
     *  \return bool, ture - success, false -failure
     *
     */
    /****************************************************************************/
    bool CheckTempModulesCurrent();
};
}
#endif // RSSTANDBY_WITH_TISSUE_H
