/****************************************************************************/
/*! \file RsFillingAfterFlush.h
 *
 *  \brief CRsFillingAfterFlush class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ July 22th, 2014
 *   $Author:  $ Shufa Li
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

#ifndef RS_FILLINGAFTERFLUSH_H
#define RS_FILLINGAFTERFLUSH_H

#include <QStateMachine>
#include <QSharedPointer>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler
{

/****************************************************************************/
/*!
 * \brief Program running State machine for RsFillingAfterFlush
 */
/****************************************************************************/
class CRsFillingAfterFlush : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor of CRsFillingAfterFlush
     *
     *  \param SchedController from SchedulerMainThreadController
     *
     */
    /****************************************************************************/
    CRsFillingAfterFlush(SchedulerMainThreadController* SchedController);

    /****************************************************************************/
    /*!
     *  \brief  desctruction of CRsFillingAfterFlush
     */
    /****************************************************************************/
    ~CRsFillingAfterFlush();

    /****************************************************************************/
    /*!
     *  \brief  Handle the sub steps for RsFillingAfterFlush
     *  \param  cmdName - command name
     *  \param  retCode - return code
     *  \return void
     *
     */
    /****************************************************************************/
    void HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal for build pressure
     *
     */
    /****************************************************************************/
    void SigBuildPressure();

    /****************************************************************************/
    /*!
     *  \brief Signal for wait 30s
     *
     */
    /****************************************************************************/
    void SigWait30s();

    /****************************************************************************/
    /*!
     *  \brief Signal for heat LevelSensor
     *
     */
    /****************************************************************************/
    void SigHeatLevelSensor();

    /****************************************************************************/
    /*!
     *  \brief Signal for check LevelSensor temperature
     *
     */
    /****************************************************************************/
    void SigCheckLevelSensorTemp();

    /****************************************************************************/
    /*!
     *  \brief Signal for Filling
     *
     */
    /****************************************************************************/
    void SigFilling();

    /****************************************************************************/
    /*!
     *  \brief Signal for moving to sealing position
     *
     */
    /****************************************************************************/
    void SigMoveToSealing();

    /****************************************************************************/
    /*!
     *  \brief Signal for releasing pressure
     *
     */
    /****************************************************************************/
    void ReleasePressure();

    /****************************************************************************/
    /*!
     *  \brief Signal for tasks done
     *
     */
    /****************************************************************************/
    void TasksDone(bool);

private:
    typedef enum
    {
        RSFILLINGAFTERFLUSH_UNDEF,
        RSFILLINGAFTERFLUSH_INIT,
        RSFILLINGAFTERFLUSH_PRESSURE,
        RSFILLINGAFTERFLUSH_WAIT,
        RSFILLINGAFTERFLUSH_HEATINGLEVELSENSOR,
        RSFILLINGAFTERFLUSH_CHECKLEVELSENSORTEMP,
        RSFILLINGAFTERFLUSH_FILLING,
        RSFILLINGAFTERFLUSH_MOVETOSEALING,
        RSFILLINGAFTERFLUSH_RELEASEPRESSURE
    } StateList_t;


    /****************************************************************************/
    /*!
     *  \brief  Get current state
     *  \param  void
     *	\return StateList_t
     */
    /****************************************************************************/
    StateList_t GetCurrentState(QSet<QAbstractState*> statesList);
private slots:
    /****************************************************************************/
    /*!
     *  \brief	Slot to enter RV_Move_To_Seal state.
     *  \param	void
     *  \return	void
     */
    /****************************************************************************/
    void OnMoveToSealing() { mp_SchedulerThreadController->MoveRV(SEAL_POS);}

private:
    SchedulerMainThreadController*   mp_SchedulerThreadController;  //!< Pointer to Scheduler Thread Controller
    QSharedPointer<QStateMachine>   mp_StateMachine;                //!< State machine for Pre-Test
    QSharedPointer<QState>          mp_Initial;                     //!< Initial state
    QSharedPointer<QState>          mp_BuildPressure;               //!< Build Pressure state
    QSharedPointer<QState>          mp_Wait30s;                     //!< wait 30s state
    QSharedPointer<QState>          mp_HeatLevelSensor;             //!< heating level sensor
    QSharedPointer<QState>          mp_CheckLevelSensorTemp;        //!< check the level sensor temperature
    QSharedPointer<QState>          mp_Filling;                     //!< filling again state
    QSharedPointer<QState>          mp_MoveToSealing;               //!< move to Sealing state
    QSharedPointer<QState>          mp_ReleasePressure;             //!< Release Pressure state
    qint64                          m_StartTime;                    //!< The begin time
    quint8                          m_MoveToSealingSeq;             //!< Sequence of moving to seaing position
};

}

#endif
