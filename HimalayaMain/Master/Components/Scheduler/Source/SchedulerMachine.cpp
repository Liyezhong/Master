/****************************************************************************/
/*! \file SchedulerMachine.cpp
 *
 *  \brief CSchedulerStateMachine class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02.01.2014
 *   $Author:  $ Quan Zhang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include "Global/Include/Commands/Command.h"
#include "Global/Include/Utils.h"
#include "Scheduler/Include/SchedulerMachine.h"
#include "Scheduler/Include/HimalayaDeviceEventCodes.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdALAllStop.h"
#include "Scheduler/Include/RsStandby.h"
#include "Scheduler/Include/RsStandbyWithTissue.h"
#include "Scheduler/Include/RsHeatingErr30SRetry.h"
#include "Scheduler/Include/RcLevelsensorHeatingOvertime.h"
#include "Scheduler/Include/RcRestart.h"
#include "Scheduler/Include/RcReport.h"
#include "Scheduler/Include/ProgramSelfTest.h"
#include <QDebug>
#include <QDateTime>

namespace Scheduler
{
/****************************************************************************/
/*!
 *  \brief    Constructor
 */
/****************************************************************************/
CSchedulerStateMachine::CSchedulerStateMachine(SchedulerMainThreadController* SchedulerThreadController)
    :mp_SchedulerThreadController(SchedulerThreadController)
{
    qRegisterMetaType<DeviceControl::RTTempCtrlType_t>("DeviceControl::RTTempCtrlType_t");
    m_PreviousState = SM_UNDEF;
    m_CurrentState = SM_UNDEF;

    mp_SchedulerMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    // Layer one states
    mp_InitState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_IdleState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_BusyState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_ErrorState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));

    // Layer two states (for Busy state)
    mp_PssmInitState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmPreTestState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmReadyToHeatLevelSensorS1 = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmReadyToHeatLevelSensorS2 = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmReadyToTubeBefore = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmReadyToTubeAfter = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmReadyToSeal = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmReadyToFill = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmReadyToDrain = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmSoak = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmStepFinish = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmError = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmPause = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmPauseDrain = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmAborting = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmAborted = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmProgramFinish = QSharedPointer<QFinalState>(new QFinalState(mp_BusyState.data()));

    // Layer two states (for Error state)
    mp_ErrorWaitState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsStandbyWithTissueState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRcLevelSensorHeatingOvertimeState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRcRestartState= QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsHeatingErr30SRetryState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));

    // Set Initial states
    mp_SchedulerMachine->setInitialState(mp_InitState.data());
    mp_BusyState->setInitialState(mp_PssmInitState.data());
    mp_ErrorState->setInitialState(mp_ErrorWaitState.data());

    // Add transition
    mp_InitState->addTransition(this, SIGNAL(SchedulerInitComplete()), mp_IdleState.data());
    mp_IdleState->addTransition(this, SIGNAL(RunSignal()), mp_BusyState.data());
    mp_BusyState->addTransition(this, SIGNAL(RunComplete()), mp_IdleState.data());
    mp_IdleState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());
    mp_InitState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());
    mp_BusyState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());

    // Sate machines for Run handling
    mp_ProgramSelfTest = QSharedPointer<CProgramSelfTest>(new CProgramSelfTest(mp_SchedulerThreadController));

    //Program Pre-Test related logic
    mp_PssmInitState->addTransition(this, SIGNAL(RunPreTest()), mp_PssmPreTestState.data());
    mp_PssmPreTestState->addTransition(mp_ProgramSelfTest.data(), SIGNAL(TasksDone(bool)), mp_PssmInitState.data());

    // State machines for Error handling
    mp_RsRvGetOriginalPositionAgain = QSharedPointer<CRsRvGetOriginalPositionAgain>(new CRsRvGetOriginalPositionAgain(mp_SchedulerMachine.data(), mp_ErrorState.data()));
    mp_RsStandby = QSharedPointer<CRsStandby>(new CRsStandby(mp_SchedulerMachine.data(), mp_ErrorState.data()));
    mp_RsHeatingErr30SRetry = QSharedPointer<CRsHeatingErr30SRetry>(new CRsHeatingErr30SRetry(mp_SchedulerMachine.data(), mp_ErrorRsHeatingErr30SRetryState.data()));
    mp_RsStandbyWithTissue = QSharedPointer<CRsStandbyWithTissue>(new CRsStandbyWithTissue(SchedulerThreadController));
    mp_RcLevelSensorHeatingOvertime = QSharedPointer<CRcLevelSensorHeatingOvertime>(new CRcLevelSensorHeatingOvertime(mp_SchedulerMachine.data(), mp_ErrorRcLevelSensorHeatingOvertimeState.data()));
    mp_RcRestart = QSharedPointer<CRcRestart>(new CRcRestart(mp_SchedulerMachine.data(), mp_ErrorRcRestartState.data()));
    mp_RcReport = QSharedPointer<CRcReport>(new CRcReport(mp_SchedulerMachine.data(), mp_ErrorState.data()));

    //RS_Standby_WithTissue related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsStandByWithTissue()), mp_ErrorRsStandbyWithTissueState.data());
    CONNECTSIGNALSLOT(mp_RsStandbyWithTissue.data(), TasksDone(bool), this, OnTasksDone(bool));
    mp_ErrorRsStandbyWithTissueState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_HeatingErr30SRetry related logic
    mp_ErrorState->addTransition(this, SIGNAL(SigEnterRSHeatingErr30SRetry()), mp_ErrorRsHeatingErr30SRetryState.data());
    CONNECTSIGNALSLOT(mp_RsHeatingErr30SRetry.data(), TasksDone(bool), this, OnTasksDone(bool));
    mp_ErrorRsHeatingErr30SRetryState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Levelsensor_Heating_Overtime related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRcLevelsensorHeatingOvertime()), mp_ErrorRcLevelSensorHeatingOvertimeState.data());
    CONNECTSIGNALSLOT(mp_RcLevelSensorHeatingOvertime.data(), TasksDone(bool), this, OnTasksDone(bool));
    mp_ErrorRcLevelSensorHeatingOvertimeState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Restart related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRcRestart()), mp_ErrorRcRestartState.data());
    CONNECTSIGNALSLOT(mp_RcRestart.data(), TasksDone(bool), this, OnTasksDone(bool));
    mp_ErrorRcRestartState->addTransition(this, SIGNAL(sigStateChange()), mp_BusyState.data());
}


void CSchedulerStateMachine::OnStartDeviceTempCtrl()
{

}

void CSchedulerStateMachine::OnCheckDeviceStatus()
{

}

void CSchedulerStateMachine::OnTasksDone(bool flag)
{
    Global::EventObject::Instance().RaiseEvent(mp_SchedulerThreadController->GetEventKey(), 0, 0, flag);
    emit sigStateChange();
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CSchedulerStateMachine::~CSchedulerStateMachine()
{

}

//mp_SelfTestStateMachine
void CSchedulerStateMachine::UpdateCurrentState(SchedulerStateMachine_t currentState)
{
    if(m_CurrentState != currentState )
    {
        QString msg = QString("%1 Previous state: %2, Current State: %3").arg(QDateTime::currentDateTime().toString()).arg(m_CurrentState,0,16).arg(currentState, 0, 16);
        Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_ID_DEBUG_MESSAGE,Global::tTranslatableStringList()<<msg);
        qDebug()<<"DBG"<< QDateTime::currentDateTime()<<"Previous state is: "<<hex<<m_CurrentState<<" Current State is:" <<hex<<currentState;
        m_PreviousState = m_CurrentState;
        m_CurrentState = currentState;
    }
}

void CSchedulerStateMachine::Start()
{
    mp_SchedulerMachine->start();
}

void CSchedulerStateMachine::Stop()
{
    mp_SchedulerMachine->stop();
}

void CSchedulerStateMachine::SendSchedulerInitComplete()
{
    emit SchedulerInitComplete();
}

void CSchedulerStateMachine::SendRunSignal()
{
    emit RunSignal();
}

void CSchedulerStateMachine::SendRunComplete()
{
    emit RunComplete();
}

void CSchedulerStateMachine::SendErrorSignal()
{
    emit ErrorSignal();
}

SchedulerStateMachine_t CSchedulerStateMachine::GetCurrentState()
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(mp_SchedulerMachine->configuration().contains(mp_InitState.data()))
    {
        currentState = SM_INIT;
    }
    else if(mp_SchedulerMachine->configuration().contains(mp_IdleState.data()))
    {
        currentState = SM_IDLE;
    }
    else if(mp_SchedulerMachine->configuration().contains(mp_ErrorState.data()))
    {
        if(mp_SchedulerMachine->configuration().contains(mp_ErrorWaitState.data()))
        {
            return SM_ERR_WAIT;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsStandbyWithTissueState.data()))
        {
            return SM_ERR_RS_STANDBY_WITH_TISSUE;
        }

        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsHeatingErr30SRetryState.data()))
        {
            return SM_ERR_RS_HEATINGERR30SRETRY;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRcLevelSensorHeatingOvertimeState.data()))
        {
            return SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRcRestartState.data()))
        {
            return SM_ERR_RC_RESTART;
        }
    }
    else if(mp_SchedulerMachine->configuration().contains(mp_BusyState.data()))
    {
        if(mp_SchedulerMachine->configuration().contains(mp_PssmInitState.data()))
        {
            return PSSM_INIT;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmPreTestState.data()))
        {
            return PSSM_PRETEST;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmReadyToHeatLevelSensorS1.data()))
        {
            return PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmReadyToHeatLevelSensorS2.data()))
        {
            return PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmReadyToTubeBefore.data()))
        {
            return PSSM_READY_TO_TUBE_BEFORE;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmReadyToTubeAfter.data()))
        {
            return PSSM_READY_TO_TUBE_AFTER;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmReadyToFill.data()))
        {
            return PSSM_READY_TO_FILL;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmReadyToSeal.data()))
        {
            return PSSM_READY_TO_SEAL;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmSoak.data()))
        {
            return PSSM_SOAK;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmReadyToDrain.data()))
        {
            return PSSM_READY_TO_DRAIN;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmStepFinish.data()))
        {
            return PSSM_STEP_FINISH;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmProgramFinish.data()))
        {
            return PSSM_PROGRAM_FINISH;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmPause.data()))
        {
            return PSSM_PAUSE;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmPauseDrain.data()))
        {
            return PSSM_PAUSE_DRAIN;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmAborting.data()))
        {
            return PSSM_ABORTING;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmAborted.data()))
        {
            return PSSM_ABORTED;
        }
    }

    return currentState;
}

SchedulerStateMachine_t CSchedulerStateMachine::GetPreviousState()
{
    return m_PreviousState;
}

void CSchedulerStateMachine::NotifyStInitOK()
{
    emit sigStInitOK();
}

void CSchedulerStateMachine::NotifyStTempOK()
{
    emit sigStTempOK();
}

void CSchedulerStateMachine::NotifyStCurrentOK()
{
    emit sigStCurrentOK();
}

void CSchedulerStateMachine::NotifyStVoltageOK()
{
    emit sigStVoltageOK();
}

void CSchedulerStateMachine::NotifyStRVPositionOK()
{
    emit sigStRVPositionOK();
}

void CSchedulerStateMachine::NotifyStPressureOK()
{
    emit sigStPressureOK();
}

void CSchedulerStateMachine::NotifyStSealingOK()
{
    emit sigStSealingOK();
}

void CSchedulerStateMachine::NotifyStGetStationcheckResult()
{
    emit sigStGetStationcheckResult();
}

void CSchedulerStateMachine::NotifyStStationLeft()
{
    emit sigStStationLeft();
}

void CSchedulerStateMachine::NotifyStStationOK()
{
    emit sigStStationOK();
}

void CSchedulerStateMachine::NotifyStDone()
{
    emit sigStDone();
}
void CSchedulerStateMachine::NotifyTempsReady()
{
    emit sigTempsReady();
}

void CSchedulerStateMachine::NotifyLevelSensorTempS1Ready()
{
    emit sigLevelSensorTempS1Ready();
}

void CSchedulerStateMachine::NotifyLevelSensorTempS2Ready()
{
    emit sigLevelSensorTempS2Ready();
}

void CSchedulerStateMachine::NotifyHitTubeBefore()
{
    emit sigHitTubeBefore();
}

void CSchedulerStateMachine::NotifyFillFinished()
{
    emit sigFillFinished();
}

void CSchedulerStateMachine::NotifyHitSeal()
{
    emit sigHitSeal();
}

void CSchedulerStateMachine::NotifySoakFinished()
{
    emit sigSoakFinished();
}

void CSchedulerStateMachine::NotifyHitTubeAfter()
{
    emit sigHitTubeAfter();
}

void CSchedulerStateMachine::NotifyDrainFinished()
{
    emit sigDrainFinished();
}

void CSchedulerStateMachine::NotifyStepFinished()
{
    emit sigStepFinished();
}

void CSchedulerStateMachine::NotifyProgramFinished()
{
    emit sigProgramFinished();
}

void CSchedulerStateMachine::NotifyError()
{
    emit sigError();
}


void CSchedulerStateMachine::NotifyPause(SchedulerStateMachine_t PreviousState)
{
    Q_UNUSED(PreviousState);
    emit sigPause();
}

void CSchedulerStateMachine::NotifyAbort()
{
    emit sigAbort();
}

void CSchedulerStateMachine::NotifyRsRvMoveToInitPositionFinished()
{
    emit sigRsRvMoveToInitPositionFinished();
}

void CSchedulerStateMachine::NotifyRsRvMoveToInitPosition()
{
    emit sigRsRvMoveToInitPosition();
}

void CSchedulerStateMachine::NotifyRcReport()
{
    emit sigRcReport();
}

void CSchedulerStateMachine::NotifyRsReleasePressure()
{
    emit sigReleasePressure();
}

void CSchedulerStateMachine::NotifyRsShutdownFailedHeater()
{
    emit sigShutdownFailedHeater();
}

void CSchedulerStateMachine::NotifyRsShutdownFailedHeaterFinished()
{
    emit sigShutdownFailedHeaterFinished();
}

void CSchedulerStateMachine::OnNotifyResume()
{
    if( this->GetPreviousState() == (PSSM_INIT))
    {
           emit sigResumeToInit();
    }
    else if((this->GetPreviousState() & 0xFFFF) == PSSM_PRETEST)
    {
        emit sigResumeToPreTest();
    }
    else if( this->GetPreviousState()== (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1))
    {
        emit sigResumeToHeatLevelSensorS1();
    }
    else if( this->GetPreviousState() == (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2))
    {
        emit sigResumeToHeatLevelSensorS2();
    }
    else if( this->GetPreviousState() == (PSSM_READY_TO_TUBE_BEFORE))
    {
        if(((this->GetCurrentState())&0xFF) == (SM_ERROR))
        {
            emit sigResumeFromErrorToBegin();
        }
        else
        {
            emit sigResumeToReadyToFill();
        }
    }
    else if( this->GetPreviousState() == (PSSM_READY_TO_FILL))
    {
        emit sigResumeToReadyToFill();
    }
    else if( this->GetPreviousState() == (PSSM_READY_TO_SEAL))
    {

        if(((this->GetCurrentState())&0xFF) == (SM_ERROR))
        {
            emit sigResumeFromErrorToBegin();
        }
        else
        {
            emit sigResumeToSoak();
        }
    }
    else if( this->GetPreviousState() == (PSSM_SOAK))
    {
        emit sigResumeToSoak();
    }
    else if( this->GetPreviousState() == (PSSM_STEP_FINISH))
    {
        emit sigResumeToStepFinished();
    }
    else
    {
        //should not enter here
    }
}

void CSchedulerStateMachine::NotifyResumeDrain()
{
    if(this->GetPreviousState() == (PSSM_READY_TO_TUBE_AFTER))
    {
        emit sigResumeToReadyToTubeAfter();
    }
    else if(this->GetPreviousState() == (PSSM_READY_TO_DRAIN))
    {
        emit sigResumeToReadyToTubeAfter();
    }
}

void CSchedulerStateMachine::EnterRsStandByWithTissue()
{
    emit SigEnterRsStandByWithTissue();
}

void CSchedulerStateMachine::EnterRsHeatingErr30SRetry()
{
    emit SigEnterRSHeatingErr30SRetry();
}

void CSchedulerStateMachine::HandlePssmPreTestWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    mp_ProgramSelfTest->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsStandByWithTissueWorkFlow(bool flag)
{
    mp_RsStandbyWithTissue->HandleWorkFlow(flag);
}

void CSchedulerStateMachine::HandleRsHeatingErr30SRetry(bool flag)
{
    mp_RsHeatingErr30SRetry->OnHandleWorkFlow(flag);
}


void CSchedulerStateMachine::EnterRcLevelsensorHeatingOvertime()
{
    emit SigEnterRcLevelsensorHeatingOvertime();
}


void CSchedulerStateMachine::HandleRcLevelSensorHeatingOvertimeWorkFlow(bool flag)
{
    mp_RcLevelSensorHeatingOvertime->OnHandleWorkFlow(flag);
}

void CSchedulerStateMachine::EnterRcRestart()
{
    emit SigEnterRcRestart();
}


void CSchedulerStateMachine::HandleRcRestart(bool flag)
{
    mp_RcRestart->OnHandleWorkFlow(flag);
}

void CSchedulerStateMachine::SendRunPreTest()
{
    emit RunPreTest();
}

void CSchedulerStateMachine::SendRunCleaning()
{
    emit RunCleaning();
}


QString CSchedulerStateMachine::GetDeviceName()
{
    ReturnCode_t EventID = mp_SchedulerThreadController->GetCurErrEventID();

    QString DevName = "";
    switch (EventID)
    {
    case DCL_ERR_DEV_RETORT_BOTTOM_SIDELOW_HEATING_ELEMENT_FAILED:
    case DCL_ERR_DEV_RETORT_SIDTOP_SIDEMID_HEATING_ELEMENT_FAILED:
    case DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERANGE:
    case DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL:
    case DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE:
    case DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL:
    case DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE:
    case DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL:
    case DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED:
        DevName = "Retort";
        break;
    case DCL_ERR_DEV_RETORT_LEVELSENSOR_TEMPERATURE_OVERRANGE:
        DevName = "LevelSensor";
        break;
    case DCL_ERR_DEV_WAXBATH_TOP_HEATINGPAD_CURRENT_OUTOFRANGE:
    case DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE:
    case DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE:
    case DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE:
        DevName = "Oven";
        break;
    case DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE:
    case DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE:
    case DCL_ERR_DEV_RV_HEATING_CURRENT_OUTOFRANGE:
        DevName = "Rotary Valve";
        break;
    case DCL_ERR_DEV_LA_PRESSURESENSOR_OUTOFRANGE:
    case DCL_ERR_DEV_LA_STATUS_EXHAUSTFAN:
    case DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL:
    case DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL:
    case DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE:
    case DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE:
        DevName = "LA";
        break;
    default:
        break;
    }
    return DevName;
}

}
