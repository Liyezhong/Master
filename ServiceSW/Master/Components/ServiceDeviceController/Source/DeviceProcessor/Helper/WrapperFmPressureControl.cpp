/****************************************************************************/
/*! \file Tools/protoTest/Source/WrapperFmPressureControl.cpp
 *
 *  \brief wrapper file for pressure control function module.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-1-5
 *  $Author:    $ Albert Zhang
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/


#ifdef PRE_ALFA_TEST
#include "WrapperFmPressureControl.h"
#include "WrapperUtils.h"
#include <QTimer>
#include "Global/Include/SystemPaths.h"
#include <QtDebug>
#include <unistd.h>

const qint32 UNDEFINED = -1; //!< undefined value for pressure and control status
const qint32 TOLERANCE = 10; //!< tolerance value for calculating inside and outside range

/****************************************************************************/
/*!
 *  \brief Constructor of class WrapperFmPressureControl
 *
 *  \iparam   Name           The Name of the Device used for QtScript identifier.
 *  \iparam   pPressureControl   Pointer to CPressureControl
 *  \iparam   pParent        Optional pointer to parent for QObject Hierarchy.
 *
 */
/****************************************************************************/
WrapperFmPressureControl::WrapperFmPressureControl(QString Name, CPressureControl *pPressureControl, QObject *pParent):
    WrapperBase(Name, pParent), m_pPressureControl(pPressureControl)
{
    Reset();
    connect(m_pPressureControl, SIGNAL(ReportActPressure(quint32, ReturnCode_t, quint8, float)),
            this, SLOT(OnGetPressure(quint32, ReturnCode_t, quint8, float)));
    connect(m_pPressureControl, SIGNAL(ReportRefPressure(quint32, ReturnCode_t, float)),
            this, SLOT(OnSetPressure(quint32, ReturnCode_t, float)));
    connect(m_pPressureControl, SIGNAL(ReportActStatus(quint32, ReturnCode_t, PressureCtrlStatus_t, PressureCtrlMainsVoltage_t)),
            this, SLOT(OnPressureControlStatus(quint32, ReturnCode_t, PressureCtrlStatus_t, PressureCtrlMainsVoltage_t)));
    connect(m_pPressureControl, SIGNAL(ReportActOperatingMode(quint32, ReturnCode_t, PressureCtrlOperatingMode_t)),
            this, SLOT(OnSetPressureCtrlOpMode(quint32, ReturnCode_t, PressureCtrlOperatingMode_t)));
    connect(m_pPressureControl, SIGNAL(ReportResetPumpOperatingTime(quint32, ReturnCode_t, quint8)),
            this, SLOT(OnResetPumpOperatingTime(quint32, ReturnCode_t, quint8)));
    connect(m_pPressureControl, SIGNAL(ReportPumpOperatingTime(quint32, ReturnCode_t, quint8, quint32)),
            this, SLOT(OnGetPumpOperatingTime(quint32, ReturnCode_t, quint8, quint32)));
    connect(m_pPressureControl, SIGNAL(ReportFanSpeed(quint32, ReturnCode_t, quint8, quint16)),
            this, SLOT(OnGetFanSpeed(quint32, ReturnCode_t, quint8, quint16)));
    connect(m_pPressureControl, SIGNAL(ReportHardwareStatus(quint32, ReturnCode_t, quint8, quint8, quint8, quint8, quint16)),
            this, SLOT(OnGetHardwareStatus(quint32, ReturnCode_t, quint8, quint8, quint8, quint8, quint16)));
    connect(m_pPressureControl, SIGNAL(ReportPressureRange(quint32, ReturnCode_t, bool, float)),
            this, SLOT(OnPressureRange(quint32, ReturnCode_t, bool, float)));

    connect(m_pPressureControl, SIGNAL(ReportRefValveState(quint32,ReturnCode_t,quint8,quint8)),
            this, SLOT(OnSetValve(quint32,ReturnCode_t,quint8,quint8)));
    connect(m_pPressureControl, SIGNAL(ReportRefFanState(quint32,ReturnCode_t,quint8)),
            this, SLOT(OnSetFan(quint32,ReturnCode_t,quint8)));
    connect(m_pPressureControl, SIGNAL(ReportError(quint32, quint16, quint16, quint16, QDateTime)),
            this, SLOT(OnError(quint32,quint16,quint16,quint16,QDateTime)));

    m_DrainingCfgData.DariningPressure = 30;
    m_DrainingCfgData.UpLiminalPressure = 15;
    m_DrainingCfgData.DownLiminalPressure = 10;
    m_DrainingCfgData.DelayTime = 5*1000;
    m_PressureDrift = ReadPressureDrift();
    for(quint32 idx =0; idx<(sizeof(m_SuckingTime)/sizeof(quint32));idx++)
    {
        m_SuckingTime[idx] = 120;   //default time: 20 Second
    }
    m_WorkingPressurePositive = 30;
    m_WorkingPressureNegative = -30;
}


/****************************************************************************/
/*!
 *  \brief Script-API:Set reference pressure
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.StartPressureControl]
 *  \until    [PressureControl.StartPressureControl]
 *
 *  \iparam  NominalPressure   Reference pressure
 *
 *  \return true, if the set function succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::StartPressureControl(quint8 flag, float NominalPressure)
{
    return SetPressure(flag, NominalPressure);
}

/****************************************************************************/
/*!
 *  \brief  slot for set pressure
 *
 *  This slot is connected to the signal ReportRefPressure
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Pressure   Actual pressure
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::OnSetPressure(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, float Pressure)
{
    qint32 ret = 1;
    //m_CurrentPressure = Pressure;
    if (!HandleErrorCode(ReturnCode))
    {
        ret = UNDEFINED;
    }
    if (m_LoopSetPressure.isRunning())
    {
        if (qAbs(Pressure - m_TargetPressure) > 0.1 )
        {
            Log(tr("ERROR: Target pressure is not reached."));
        }
        m_TargetPressure = UNDEFINED;
        m_LoopSetPressure.exit(ret);
    }
    else
    {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  slot for set valve
 *
 *  This slot is connected to the signal ReportRefValveState
 *
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::OnSetValve(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint8 ValveIndex, quint8 ValveState)
{
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopSetValve.isRunning())
    {
        m_LoopSetValve.exit(ret);
    }
    else
    {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

void WrapperFmPressureControl::OnSetFan(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint8 FanState)
{
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopSetFan.isRunning())
    {
        m_LoopSetFan.exit(ret);
    }
    else
    {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}
/****************************************************************************/
/*!
 *  \brief Script-API:Get actual pressure
 *
 * This method performs reading of actual pressure
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.GetPressure]
 *  \until    [PressureControl.GetPressure]
 *
 *  \return actualpressure, else UNDEFINED in caseof error
 *
 */
/****************************************************************************/
float WrapperFmPressureControl::GetPressure(quint8 Index)
{

    float RetValue;
    static qint64 LastTime = 0;
    qint64 Now = QDateTime::currentMSecsSinceEpoch();

    if((Now - LastTime) > 200)
    {
        bool ok = HandleErrorCode(m_pPressureControl->ReqActPressure(Index));
        if (!ok)
        {
            RetValue = UNDEFINED;
        }
        else
        {
            qint32 ret = m_LoopGetPressure.exec();
            if (ret != 1)
            {
                RetValue = UNDEFINED;
            }
            else
            {
                RetValue = m_CurrentPressure - m_PressureDrift;
            }
        }
        LastTime = QDateTime::currentMSecsSinceEpoch();
    }
    else
    {
        RetValue = m_CurrentPressure - m_PressureDrift;
    }
    return RetValue;
}

/****************************************************************************/
/*!
 *  \brief Helper function, to set the pressure of Oven
 *
 *  \iparam NominalPressure Pressure to be set
 *
 *  \return true, if the pressure set succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::SetPressure(quint8 flag, float NominalPressure)
{
    //m_TargetPressure = NominalPressure;
    m_TargetPressure = NominalPressure + m_PressureDrift;
    bool ok = HandleErrorCode(m_pPressureControl->SetPressure(flag, m_TargetPressure));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetPressure.exec();
    return (ret == 1);
}

bool WrapperFmPressureControl::SetFan(quint8 State)
{
    bool ok = HandleErrorCode(m_pPressureControl->SetFan(State));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetFan.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief Helper function, to set the state of Valve
 *
 *  \iparam ValveIndex: which valve
            ValveState: the new state 1: open
                                      0: close(default, no power)
 *
 *  \return true, if the pressure set succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::SetValve(quint8 ValveIndex, quint8 ValveState)
{
    bool ok = HandleErrorCode(m_pPressureControl->SetValve(ValveIndex, ValveState));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetValve.exec();
    return (ret == 1);
}
bool WrapperFmPressureControl::SetCalibration(bool Enable)
{
    bool ok = HandleErrorCode(m_pPressureControl->SetCalibration(Enable));
    return ok;
}

/****************************************************************************/
/*!
 *  \brief Script-API: set the target pressure.
 *
 *  \iparam pressure: target pressure in 1 kpa step
 *
 *  \return true, if the pressure set succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::SetTargetPressure(quint8 flag, float pressure)
{
    if(pressure > 0)
    {
        if((flag != 1)&&(flag != 17))
        {
            Log(tr("ERROR: invlaid flag: %1 for pressure: %2").arg(flag).arg(pressure));
            return false;
        }
        //close valve 1
        SetValve(0, 0);
        //open valve 2
        SetValve(1, 1);
        (void)usleep(200*1000);//should sleep for 0.2 sec
        SetPressure(flag, pressure);
    }
    else if(pressure < 0)
    {
        if((flag != 9)&&(flag != 25))
        {
            Log(tr("ERROR: invlaid flag: %1 for pressure: %2").arg(flag).arg(pressure));
            return false;
        }
        //open valve 1
        SetValve(0, 1);
        //close valve 2
        SetValve(1, 0);
        (void)usleep(200*1000);//should sleep for 0.2 sec
        SetPressure(flag, pressure);
    }
    return true;
}


#define DRAINGING_TARGET_THRESHOLD_PRESSURE  (10) //15
#define DRAINGING_TARGET_FINISHED_PRESSURE   (8)  //10
#define DRAINGING_SETUP_WARNING_TIME    (120*1000) //Td_Err
#define DRAINGING_MAX_SETUP_TIME        (240*1000) //Td_Err
#define DRAINGING_POLLING_TIME          (1000)
#define DRAINGING_RET_OK                (1)
#define DRAINGING_RET_GENERAL_ERR       (-1)
#define DRAINGING_RET_TIMEOUT           (-2)
/****************************************************************************/
/*!
 *  \brief Script-API: Drain the reagent in the retort
 *
 *   This method drain the reagent in the retort by setting up pressure. The
 *   program is determined finished by detecting the pressure goes down to
 *   certain threshold, otherwise will time out.
 *
 *  \iparam DelayTime Time to delay after reagent has been drained.
 *  \iparam TubePosition which bottle is draining to.
 *
 *  \return 1 if succeeded, -1 error happend, -2 timeout happend.
 *
 */
/****************************************************************************/
qint32 WrapperFmPressureControl::Draining(quint32 DelayTime, quint32 TubePosition)
{

    bool stop = false;
    qint32 RetValue = DRAINGING_RET_OK;
    QTimer timer;
    QDateTime beforeDraining = QDateTime::currentDateTime();
    float CurrentPressure = 0;
    bool PressureHasBeenSetup = false;
    qint32 counter = 0;
    qint32 TimeSlotPassed = 0;
    bool WarnShowed = false;

    connect(&timer, SIGNAL(timeout()), this, SLOT(DrainingTimerCB()));
    Log(tr("Start Drainging procedure."));
    //release pressure
    if(!ReleasePressure())
    {
        RetValue = DRAINGING_RET_GENERAL_ERR;
        goto SORTIE;
    }

    SetTargetPressure(17, m_WorkingPressurePositive);
    SetFan(1);
    Log(tr("Set target pressure finished."));
    timer.setSingleShot(false);
    timer.start(DRAINGING_POLLING_TIME);

    while(!stop)
    {
        if (m_LoopDrainingTimer.exec()== (-1))
        {
             Log(tr("Current procedure has been interrupted, exit now."));
             RetValue = DRAINGING_RET_GENERAL_ERR;
             goto SORTIE;
        }
        CurrentPressure = GetPressure(0);

        if(!PressureHasBeenSetup)
        {
            if(CurrentPressure >= DRAINGING_TARGET_THRESHOLD_PRESSURE)
            {
                PressureHasBeenSetup = true;
                Log(tr("Pressure has been set up"));
            }
        }
        else
        {
            if(CurrentPressure < DRAINGING_TARGET_FINISHED_PRESSURE)
            {
                counter++;
                if(counter > 6)
                {
                    Log(tr("Drain finished!"));
                    stop = true;

                    if(TubePosition != 0)
                    {
                        QDateTime now = QDateTime::currentDateTime();
                        quint32 seconds = beforeDraining.secsTo(now);
                        Log(tr("Draining time is: %1 seconds").arg(seconds));
                    }
                }
            }
            else
            {
                counter = 0;
            }
        }
        TimeSlotPassed++;
        if(((TimeSlotPassed * DRAINGING_POLLING_TIME) > DRAINGING_SETUP_WARNING_TIME) && (!WarnShowed))
        {
              Log(tr("Warning: Draining do not finished in expected time(%1 seconds)!").arg(DRAINGING_SETUP_WARNING_TIME / 1000));
              WarnShowed = true;
        }
        if((TimeSlotPassed * DRAINGING_POLLING_TIME) > DRAINGING_MAX_SETUP_TIME)
        {
            Log(tr("Warning: Draining exceed maximum setup time(%1 seconds), exit!").arg(DRAINGING_MAX_SETUP_TIME / 1000));
            RetValue = DRAINGING_RET_TIMEOUT;
            goto SORTIE;
        }
    }

    //waiting for some time
    if(DelayTime > 0)
    {
        Log(tr("Finished. start hold for %1 millisecond.").arg(DelayTime));
        timer.setSingleShot(true);
        timer.start(DelayTime);
        if (m_LoopDrainingTimer.exec()== (-1))
        {
             Log(tr("Current procedure has been interrupted, exit now."));
             RetValue = DRAINGING_RET_GENERAL_ERR;
             goto SORTIE;
        }
    }

SORTIE:
    if(timer.isActive())
    {
        timer.stop();
    }
    //stop compressor
    StopCompressor();
    SetFan(0);
    //close both valve
    SetValve(0,0);
    SetValve(1,0);

    return RetValue;
}

/****************************************************************************/
/*!
 *  \brief Callback function for timer in Draining
 *
 *  \return void
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::DrainingTimerCB(void)
{

    if(m_LoopDrainingTimer.isRunning())
    {
        m_LoopDrainingTimer.exit();
    }
}
#define AGITATION_MAX_TIME         (40 * 1000)
/****************************************************************************/
/*!
 *  \brief Script-API: Agitation the reagent
 *
 *   This method agitate the reagent in the retort by draining the reagent for
 *   certain time and then sucking back.
 *
 *  \iparam TubePosition which bottle is draining to.
 *
 *  \return true, if Agitation is successed, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::Agitation(quint32 TubePosition)
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(AgitationTimerCB()));
    Log(tr("Start Agitation."));
    //release pressure
    if(!ReleasePressure())
    {
        return false;
    }
    if(0 == TubePosition)
    {
         Log(tr("Invalid position"));
        return false;
    }

    timer.setSingleShot(true);
    quint32 draintime = m_SuckingTime[TubePosition] * 333; // set to 1/3 of sucking time in milliseconds
    if(draintime > AGITATION_MAX_TIME)
    {
        draintime = AGITATION_MAX_TIME;
    }
    Log(tr("Start Draining for %1 millseconds.").arg(draintime));
    timer.start(draintime);
    SetTargetPressure(17, m_WorkingPressurePositive);

    Log(tr("Start timer, please wait."));

    if(m_LoopAgitationTimer.exec() == (-1))
    {
        Log(tr("Current procedure has been interrupted, exit now."));
        timer.stop();
        return false;
    }

    Log(tr("Draining finished. Now suck"));

    return ReleasePressure();
    //return Sucking();
}

/****************************************************************************/
/*!
 *  \brief Callback function for timer in Agitation
 *
 *  \return void
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::AgitationTimerCB(void)
{

    if(m_LoopAgitationTimer.isRunning())
    {
        m_LoopAgitationTimer.exit();
    }
}


#define SUCKING_TARGET_PRESSURE           (-30)
#define SUCKING_OVERFLOW_PRESSURE         (-25)
#define SUCKING_INSUFFICIENT_PRESSURE     (-20)
#define SUCKING_OVERFLOW_4SAMPLE_DELTASUM (-2.5)
#define SUCKING_INSUFFICIENT_4SAMPLE_DELTASUM (1)

#define SUCKING_MAX_DELAY_TIME        (10000)
#define SUCKING_MAX_DELAY_TIME_FOR_OVERFLOW (70*1000)
#define SUCKING_POOLING_TIME          (400)
#define SUCKING_SETUP_WARNING_TIME    (120*1000)
#define SUCKING_MAX_SETUP_TIME        (240*1000)
#define SUCKING_OVERFLOW_SAMPLE_SIZE    (4) //used to be 4 with old pump
#define SUCKING_OVERFLOW_TOLERANCE      (2)
#define SUCKING_OVERFLOW_WINDOW_SIZE    (3)
#define SUCKING_OVERFLOW_THRESHOLD      (10)
#define SUCKING_RET_OK                  (1)
#define SUCKING_RET_GENERAL_FAIL        (-1)
#define SUCKING_RET_TIMEOUT             (-2)
#define SUCKING_RET_OVERFLOW            (-3)
#define SUCKING_RET_INSUFFICIENT        (-4)
/****************************************************************************/
/*!
 *  \brief Script-API: Sucking the reagent
 *
 *   This method suck the reagent to the retort from certain station by set up
 *   vaccum. The sucking will finished by detecting level sensor state, otherwise
 *   will time out.
 *
 *  \iparam DelayTime Time to delay when get level sensor signal.
 *  \iparam TubePosition which bottle is sucking from.
 *  \iparam IsAgitation If this function is used in Agitation program.
 *
 *  \return 1 if succeeded, -1 error happend, -2 timeout, -3 overflow.
 *
 */
/****************************************************************************/
qint32 WrapperFmPressureControl::Sucking(quint32 DelayTime, quint32 TubePosition, bool IsAgitation)
{
    qint32 RetValue = SUCKING_RET_OK;
    QTimer timer;
    quint32 counter = 0;
    qint64 StopTimeMs = 0;
    QList<float> PressureBuf;
    int levelSensorState = 0xFF;
    bool stop = false;
    bool WarnShowed = false;
    bool StopInsufficientCheck =false;
    QDateTime beforeSucking = QDateTime::currentDateTime();

    connect(&timer, SIGNAL(timeout()), this, SLOT(SuckingTimerCB()));
    Log(tr("Start Sucking procedure."));

    //release pressure
    if(!ReleasePressure())
    {
        RetValue = SUCKING_RET_GENERAL_FAIL;
        goto SORTIE;
    }
    Log(tr("Start Sucking now."));
    SetTargetPressure(25, m_WorkingPressureNegative);
    SetFan(1);

    //set timeout to 2 minutes
    timer.setSingleShot(false);
    timer.start(SUCKING_POOLING_TIME);

    while(!stop)
    {
        levelSensorState = m_LoopSuckingLevelSensor.exec();
        counter++;
        qint64 nowMs = QDateTime::currentDateTime().toMSecsSinceEpoch();
        if(levelSensorState == 1)
        {
            if(StopTimeMs == 0)
            {
                Log(tr("Hit target level. Sucking Finished."));
                if(DelayTime > 0)
                {
                     //waiting for some time
#if 0  // changed by Sunny on July, 15, 2014 to test overflow for manufacturing.
                     if(DelayTime < SUCKING_MAX_DELAY_TIME)
                     {
                          Log(tr("Delay for %1 milliseconds.").arg(DelayTime));
                          StopTimeMs = nowMs + DelayTime;
                     }
                    else
                    {
                        Log(tr("Delay for %1 milliseconds.").arg(SUCKING_MAX_DELAY_TIME));
                        StopTimeMs = nowMs + SUCKING_MAX_DELAY_TIME;
                    }
#else
                    if(DelayTime < SUCKING_MAX_DELAY_TIME_FOR_OVERFLOW)
                    {
                         Log(tr("Delay for %1 milliseconds.").arg(DelayTime));
                         StopTimeMs = nowMs + DelayTime;
                    }
                   else
                   {
                       Log(tr("Delay for %1 milliseconds.").arg(SUCKING_MAX_DELAY_TIME_FOR_OVERFLOW));
                       StopTimeMs = nowMs + SUCKING_MAX_DELAY_TIME_FOR_OVERFLOW;
                   }
#endif //
                }
                else
                {
                    stop = true;
                }
                if((TubePosition != 0)&&(!IsAgitation))
                {
                    QDateTime now = QDateTime::currentDateTime();
                    quint32 seconds = beforeSucking.secsTo(now);
                    m_SuckingTime[TubePosition] = seconds;
                    Log(tr("Sucking time is: %1 seconds").arg(seconds));
                }
            }
            StopInsufficientCheck = true;
        }
        else if(levelSensorState == 0)
        {
            Log(tr("Received 0"));
        }
        else if (levelSensorState == (-1))
        {
            Log(tr("Current procedure has been interrupted, exit now."));
            RetValue = SUCKING_RET_GENERAL_FAIL;
            goto SORTIE;
        }
        else if(levelSensorState == 3)
        {

#if 0  // changed by Sunny on Jul, 15, 2014. For manufacturing and service, no warning report, but report timeout in 2 minutes.
            if((counter > (SUCKING_SETUP_WARNING_TIME / SUCKING_POOLING_TIME)) && ( !WarnShowed ))
            {
                Log(tr("Warning! Do not get level sensor data in %1 seconds!").arg(SUCKING_SETUP_WARNING_TIME / 1000));
                WarnShowed = true;
            }
            if(counter > (SUCKING_MAX_SETUP_TIME / SUCKING_POOLING_TIME))
            {
                Log(tr("Do not get level sensor data in %1 seconds, Time out! Exit!").arg(SUCKING_MAX_SETUP_TIME / 1000));
                RetValue = SUCKING_RET_TIMEOUT;
                goto SORTIE;
            }
#else
            if(counter > (SUCKING_SETUP_WARNING_TIME / SUCKING_POOLING_TIME))
            {
                Log(tr("Do not get level sensor data in %1 seconds, Time out! Exit!").arg(SUCKING_SETUP_WARNING_TIME / 1000));
                RetValue = SUCKING_RET_TIMEOUT;
                goto SORTIE;
            }
#endif  //
            if((StopTimeMs!=0)&&(nowMs >= StopTimeMs))
            {
                Log(tr("Delay finished."));
                goto SORTIE;
            }
            //check pressure here
            float CurrentPressure = GetPressure(0);
            if(CurrentPressure != (-1))
            {
                PressureBuf.append(CurrentPressure);
            }
            if(PressureBuf.length() >= SUCKING_OVERFLOW_SAMPLE_SIZE)
            {
#if 1
                //with 4 wire pump
                float Sum = 0;
                float DeltaSum = 0;
                float lastValue = PressureBuf.at(0);
                for(qint32 i = 0; i < PressureBuf.length(); i++)
                {
                     Sum += PressureBuf.at(i);
                     DeltaSum += PressureBuf.at(i) - lastValue;
                     lastValue = PressureBuf.at(i);
                }

                if(((Sum/ PressureBuf.length()) < SUCKING_OVERFLOW_PRESSURE)&&(DeltaSum < SUCKING_OVERFLOW_4SAMPLE_DELTASUM))
                {
                    Log(tr("Overflow occured! Exit now"));
                    RetValue = SUCKING_RET_OVERFLOW;
                    for(qint32 i = 0; i < PressureBuf.length(); i++)
                    {
                         Log(tr("Over Flow Pressure %1 is: %2").arg(i).arg(PressureBuf.at(i)));
                    }
                    goto SORTIE;
                }
                else if(((Sum/ PressureBuf.length()) < SUCKING_INSUFFICIENT_PRESSURE)&&(DeltaSum > SUCKING_INSUFFICIENT_4SAMPLE_DELTASUM)&&(!StopInsufficientCheck))
                {
                    if((TubePosition > 13) && (TubePosition <=16))
                    {
                        Log(tr("Insufficient reagent in the station! Exit now"));
                        for(qint32 i = 0; i < PressureBuf.length(); i++)
                        {
                             Log(tr("Insufficient Pressure %1 is: %2").arg(i).arg(PressureBuf.at(i)));
                        }
                        RetValue = SUCKING_RET_INSUFFICIENT;
                        goto SORTIE;
                    }
                    else
                    {
                        Log(tr("Insufficient reagent in the station!"));
                    }
                }
                PressureBuf.pop_front();
#elif 0
                //after 2012.11.15, algorithm: at least one sample bigger than target, and two sample bigger than 3 samples' mean.
                 PressureBuf.pop_front();
                 quint32 ExceptPointNum = 0;
                 bool biggerThanTarget = false;
                 for(qint32 i = 0; i < PressureBuf.length(); i++)
                 {
                     biggerThanTarget |= qAbs(PressureBuf.at(i)) > qAbs(SUCKING_TARGET_PRESSURE);

                    if(i < SUCKING_OVERFLOW_WINDOW_SIZE)
                    {   continue;   }
                    float sum = 0;
                    for(qint32 j = (i - SUCKING_OVERFLOW_WINDOW_SIZE); j < i ; j++)
                    {
                        sum += qAbs(PressureBuf.at(j));
                    }
                    float average = sum/SUCKING_OVERFLOW_WINDOW_SIZE;
                    if(( average + SUCKING_OVERFLOW_TOLERANCE) <= qAbs(PressureBuf.at(i)))
                    {
                         ExceptPointNum++;
                    }
                 }

                 if((ExceptPointNum > 1)&&(biggerThanTarget))
                 {
                     Log(tr("Overflow occured! Exit now"));
                     RetValue = SUCKING_RET_OVERFLOW;
                     goto SORTIE;
                 }
#else
                //before 2012.11.15, algorithm: latest samples bigger than THRESHOLD, and two sample bigger than 3 samples' mean.
                PressureBuf.pop_front();
                quint32 ExceptPointNum = 0;

                for(qint32 i = 0; i < PressureBuf.length(); i++)
                {
                   if(i < SUCKING_OVERFLOW_WINDOW_SIZE)
                   {   continue;   }
                   float sum = 0;
                   for(qint32 j = (i - SUCKING_OVERFLOW_WINDOW_SIZE); j < i ; j++)
                   {
                       sum += qAbs(PressureBuf.at(j));
                   }
                   float average = sum/SUCKING_OVERFLOW_WINDOW_SIZE;
                   if((( average + SUCKING_OVERFLOW_TOLERANCE) <= qAbs(PressureBuf.at(i)))&& average > SUCKING_OVERFLOW_THRESHOLD)
                   {
                        ExceptPointNum++;
                   }
                }

                if(ExceptPointNum > 1)
                {
                    stop = true;
                    Log(tr("Overflow occured! Exit now"));
                    RetValue = false;
                }
#endif
              }
        }
        else
        {
            Log(tr("Unexpected level sensor state value: %1").arg(levelSensorState));
        }
    }


SORTIE:
    if(timer.isActive())
    {
        timer.stop();
    }
    //ReleasePressure();
    StopCompressor();
    SetFan(0);
    return RetValue;
}
/****************************************************************************/
/*!
 *  \brief Callback function for timer in Sucking
 *
 *  \return void
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::SuckingTimerCB(void)
{

    if(m_LoopSuckingLevelSensor.isRunning())
    {
         m_LoopSuckingLevelSensor.exit(3);
    }
}

#define PRESSURE_STATIC_DIFFERENCE     (2)
#define PRESSURE_MAX_SETUP_TIME        (120*1000)  //Tp_Rrr
#define PRESSURE_HOLD_TIME             (5*1000)    //Tp_Delay
#define PRESSURE_POLLING_TIME          (500)       //Polling time
#define PRESSURE_PID_STEADY_NUM        (10)
/****************************************************************************/
/*!
 *  \brief Script-API: Setup a Pressure
 *
 *   This method set up a pressure environment in the restort.
 *
 *
 *  \return true if succeeded, otherwise false.
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::Pressure()
{
     Log(tr("Start set up pressure procedure"));
     QTimer timer;
     bool stop = false;
     quint32 TimeSlotPassed = 0;
     float CurrentPressure;

     connect(&timer, SIGNAL(timeout()), this, SLOT(PressureTimerCB()));

     //release pressure
     if(!ReleasePressure())
     {
         Log(tr("Release pressure failed, exit now."));
         return true;
     }

    //start compressor
    Log(tr("Start to setup pressure now."));
    SetTargetPressure(1, m_WorkingPressurePositive);
    SetFan(1);
    IsPIDDataSteady(0,  0,  0,  0, true);
    timer.start(PRESSURE_POLLING_TIME);

    while(!stop)
    {
        if (m_LoopPressureTimer.exec()== (-1))
        {
             Log(tr("Current procedure has been interrupted, exit now."));
             timer.stop();
             SetFan(0);
             return true;
        }
        TimeSlotPassed++;
        CurrentPressure = GetPressure(0);
        if (IsPIDDataSteady(m_WorkingPressurePositive,  CurrentPressure,  \
                            PRESSURE_STATIC_DIFFERENCE, PRESSURE_PID_STEADY_NUM, false))
        {
            Log(tr("Target pressure is getting steady now."));
            stop = true;
            timer.stop();
        }
        else
        {
            if((TimeSlotPassed * PRESSURE_POLLING_TIME) > PRESSURE_MAX_SETUP_TIME)
            {
                timer.stop();
                Log(tr("Warning: Pressure exceed maximum setup time, exit!"));
                //stop compressor
                StopCompressor();
                //close both valve
                SetValve(0,0);
                SetValve(1,0);
                SetFan(0);
                return true;
            }
        }
    }
    timer.start(PRESSURE_HOLD_TIME);
    Log(tr("Wait for 3 sec to get steady"));
    if (m_LoopPressureTimer.exec()== (-1))
    {
         Log(tr("Current procedure has been interrupted, exit now."));
         timer.stop();
         SetFan(0);
         return true;
    }
    timer.stop();
    //stop compressor
    //StopCompressor();
    //SetPressure(5, m_WorkingPressurePositive);//should be 1

    Log(tr("Finished, exit!"));
    SetFan(0);
    return true;
}

/****************************************************************************/
/*!
 *  \brief Callback function for timer in Pressure
 *
 *  \return void
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::PressureTimerCB(void)
{

    if(m_LoopPressureTimer.isRunning())
    {
        m_LoopPressureTimer.exit();
    }
}
/****************************************************************************/
/*!
 *  \brief Helper function for judge if a set of data is getting steady in PID
 *         control process.
 *
 *  \iparam TargetValue: Target value of the PID control
 *  \iparam CurrentValue: Current value.
 *  \iparam Tolerance: The acceptable difference between taget value and actual
                       value.
 *  \iparam Num: Number of data to make the judgement.
 *  \iparam Init: If is to start a new judgement.

 *  \return true if the data is steady, otherwise false.
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::IsPIDDataSteady(float TargetValue, float CurrentValue, float Tolerance, qint32 Num, bool Init)
{
    static QList<float> valueList;

    bool ret = false;
    if(Init)
    {
        if(!valueList.isEmpty())
        {
            valueList.clear();
        }
    }
    else
    {
        float diff = (CurrentValue > TargetValue) ? (CurrentValue - TargetValue) : (TargetValue - CurrentValue );
        valueList.append(diff);
        if(valueList.size() >= Num)
        {
            ret = true;
            for(int idx =0; idx < Num; idx++)
            {
                if(valueList.at(valueList.size() - idx - 1) > Tolerance)
                {
                    ret = false;
                    break;
                }
            }
        }
    }

    return ret;
}

#define VACCUM_STATIC_DIFFERENCE     (2)
#define VACCUM_PID_STEADY_NUM        (10)
#define VACCUM_MAX_SETUP_TIME        (120*1000)  //Tv_Rrr
#define VACCUM_HOLD_TIME             (5*1000)  //Tv_Delay
#define VACCUM_POLLING_TIME          (500)  //Polling time
/****************************************************************************/
/*!
 *  \brief Script-API: Setup a vaccum
 *
 *   This method set up a vaccum environment in the restort.
 *
 *
 *  \return true if succeeded, otherwise false.
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::Vaccum()
{
     Log(tr("Start set up vaccum procedure"));
     QTimer timer;
     quint32 TimeSlotPassed = 0;
     bool stop = false;
     float CurrentPressure;
     connect(&timer, SIGNAL(timeout()), this, SLOT(VaccumTimerCB()));

     //release pressure
     if(!ReleasePressure())
     {
         return true;
     }

    //start compressor
    Log(tr("Start to setup vaccum now."));
    SetTargetPressure(9, m_WorkingPressureNegative);
    SetFan(1);
    IsPIDDataSteady(0,  0,  0,  0, true);
    timer.start(VACCUM_POLLING_TIME);
    while(!stop)
    {
        if (m_LoopVaccumTimer.exec()== (-1))
        {
             Log(tr("Current procedure has been interrupted, exit now."));
             timer.stop();
             SetFan(0);
             return true;
        }
        TimeSlotPassed++;
        CurrentPressure = GetPressure(0);

        if ((IsPIDDataSteady(m_WorkingPressureNegative,  CurrentPressure,  \
                             VACCUM_STATIC_DIFFERENCE, VACCUM_PID_STEADY_NUM, false)))
        {
            Log(tr("Target pressure is getting steady now."));
            stop = true;
            timer.stop();
        }
        else
        {
            if((TimeSlotPassed * VACCUM_POLLING_TIME) > VACCUM_MAX_SETUP_TIME)
            {
                Log(tr("Warning: Vaccum exceed maximum setup time, exit!"));
                //stop compressor
                StopCompressor();
                //close both valve
                SetValve(0,0);
                SetValve(1,0);
                SetFan(0);
                return true;
            }
        }
    }

    timer.start(VACCUM_HOLD_TIME);
    Log(tr("Wait for 3 sec to get steady"));
    if (m_LoopVaccumTimer.exec()== (-1))
    {
         Log(tr("Current procedure has been interrupted, exit now."));
         timer.stop();
         SetFan(0);
         return true;
    }
    timer.stop();
    //stop compressor
    //StopCompressor();
    //SetPressure(13, m_WorkingPressureNegative);//should be 1
    Log(tr("Finished, exit!"));
    SetFan(0);
    return true;
}

/****************************************************************************/
/*!
 *  \brief Callback function for timer in Vaccum
 *
 *  \return void
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::VaccumTimerCB(void)
{

    if(m_LoopVaccumTimer.isRunning())
    {
        m_LoopVaccumTimer.exit();
    }
}


#define RELEASE_PRESSURE_MAX_TIME (20*1000)
#define RELEASE_PRESSURE_PROCEDURE_INTERRUPT (-1)
/****************************************************************************/
/*!
 *  \brief Script-API: Release the pressure in retort to ambient pressure
 *
 *
 *  \return true if succeeded, otherwise false.
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::ReleasePressure(void)
{
    Log(tr("Start release pressure procedure"));
    QTimer timer;
    quint32 TimeSlotPassed = 0;


    if( m_LoopDrainingTimer.isRunning())
    {
        m_LoopDrainingTimer.exit(RELEASE_PRESSURE_PROCEDURE_INTERRUPT);
    }
    if (m_LoopSuckingTimer.isRunning())
    {
        m_LoopSuckingTimer.exit(RELEASE_PRESSURE_PROCEDURE_INTERRUPT);
    }
    if(m_LoopSuckingLevelSensor.isRunning())
    {
        m_LoopSuckingLevelSensor.exit(RELEASE_PRESSURE_PROCEDURE_INTERRUPT);
    }
    if( m_LoopPressureTimer.isRunning())
    {
        m_LoopPressureTimer.exit(RELEASE_PRESSURE_PROCEDURE_INTERRUPT);
    }
    if( m_LoopVaccumTimer.isRunning())
    {
        m_LoopVaccumTimer.exit(RELEASE_PRESSURE_PROCEDURE_INTERRUPT);
    }
    if( m_LoopReleasePressureTimer.isRunning())
    {
        m_LoopReleasePressureTimer.exit(RELEASE_PRESSURE_PROCEDURE_INTERRUPT);
    }
    if(m_LoopAgitationTimer.isRunning())
    {
        m_LoopAgitationTimer.exit(RELEASE_PRESSURE_PROCEDURE_INTERRUPT);
    }
    connect(&timer, SIGNAL(timeout()), this, SLOT(ReleasePressureTimerCB()));
    //close both valve
    StopCompressor();
    SetFan(0);
    Log(tr("Close Both Valves"));
    (void)usleep(100*1000);//should sleep for 0.1 sec
    SetValve(0,0);
    SetValve(1,0);
    float CurrentPressure = GetPressure(0);

    timer.setSingleShot(false);
    timer.start(500);
    Log(tr("Wait for current pressure get to ZERO"));
    while((CurrentPressure > 2)||(CurrentPressure < (-2)))
    {
        CurrentPressure = GetPressure(0);
        if (m_LoopReleasePressureTimer.exec() == RELEASE_PRESSURE_PROCEDURE_INTERRUPT)
        {
            Log(tr("Current procedure has been interrupted, exit now."));
            timer.stop();
            return false;
        }
        TimeSlotPassed++;
        if(TimeSlotPassed*500 > RELEASE_PRESSURE_MAX_TIME)
        {
            Log(tr("Warning: Release Pressure exceed maximum setup time, exit!"));
            //stop compressor
            StopCompressor();
            //close both valve
            (void)usleep(100*1000);//should sleep for 0.1 sec
            SetValve(0,0);
            SetValve(1,0);
            return false;
        }
    }
    return true;
}

/****************************************************************************/
/*!
 *  \brief Script-API: Stop the pump immediately.
 *
 *
 *  \return void.
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::StopCompressor(void)
{
    Log(tr("Shut down compressor"));
    SetPressure(0, UNDEFINED - m_PressureDrift);
}


/****************************************************************************/
/*!
 *  \brief Callback function for timer in ReleasePressure
 *
 *  \return void
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::ReleasePressureTimerCB(void)
{
    if(m_LoopReleasePressureTimer.isRunning())
    {
        m_LoopReleasePressureTimer.exit();
    }
}

/****************************************************************************/
/*!
 *  \brief  slot for get pressure
 *
 *  This slot is connected to the signal ReportActPressure
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Pressure          Actual pressure
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::OnGetPressure(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint8 Index, float Pressure)
{
    Q_UNUSED(Index)

    qint32 ret = 1;
    m_CurrentPressure = Pressure;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopGetPressure.isRunning())
    {
            //Log(tr("Current pressure is: %1").arg(Pressure));
             m_LoopGetPressure.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Helper function, to set pressure control status
 *
 *  \return true, if set pressure control status ON has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::SetPressureCtrlON()
{
    Log(tr("SetPressureCtrlON"));
    return SetPressureControlStatus(PRESSURECTRL_STATUS_ON);
}

/****************************************************************************/
/*!
 *  \brief Script-API:Set pressure control status
 *
 *   This method sets the pressure control status OFF
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.StopPressureControl]
 *  \until    [PressureControl.StopPressureControl]
 *
 *  \return true, if set pressure control status off has succeeded, else falsel
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::StopPressureControl()
{
    Log(tr("StopPressureControl"));
    return SetPressureControlStatus(PRESSURECTRL_STATUS_OFF);
}

/****************************************************************************/
/*!
 *  \brief Helper function, to set the pressure control status
 *
 *  \iparam PressureCtrlStatus   Status of the pressure control
 *
 *  \return true, if the pressure control status setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::SetPressureControlStatus(PressureCtrlStatus_t PressureCtrlStatus)
{
    m_TargetPressureCtrlStatus = PressureCtrlStatus;
    bool ok = HandleErrorCode(m_pPressureControl->SetStatus(PressureCtrlStatus));
    if (!ok) {
        return false;
    }
    //qint32 ret = m_LoopSetPressureControl.exec();
    //return (ret == 1);
    return true;
}

/****************************************************************************/
/*!
 *  \brief  slot for pressure control status
 *
 *  This slot is connected to the signal ReportPressureCtrlStatus
 *
 *  \iparam   ReturnCode      ReturnCode of function level Layer
 *  \iparam   PressureCtrlStatus  Status of the pressure control
 *  \iparam   MainsVoltage    Mains voltage state of the heaters
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::OnPressureControlStatus(quint32 /*InstanceID*/, ReturnCode_t ReturnCode,
                                               PressureCtrlStatus_t PressureCtrlStatus, PressureCtrlMainsVoltage_t MainsVoltage)
{
    qint32 ret = 1;
    m_CurrentPressureCtrlStatus = PressureCtrlStatus;
    m_MainsVoltageStatus = MainsVoltage;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    // GetPosition can be called during asynchronous SetPosition, so
    // process GetPosition first.
    if (m_LoopGetPressureControl.isRunning()) {
        m_LoopGetPressureControl.exit(ret);
    }
    else if (m_LoopGetVoltageState.isRunning()) {
        m_LoopGetVoltageState.exit(ret);

    }
    else if (m_LoopSetPressureControl.isRunning()){
        if(m_TargetPressureCtrlStatus != m_CurrentPressureCtrlStatus) {
           Log(tr("ERROR: Target pressure control status is not reached."));
           ret = UNDEFINED;
        } else {
           Log(tr("Done."));
        }
        m_TargetPressureCtrlStatus = PRESSURECTRL_STATUS_UNDEF;
        m_LoopSetPressureControl.exit(ret);
    }
    else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Set pressure control operation mode
 *
 *  This method performs setting of pressure control operation mode as full
 *  power mode.
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.SetFullPowerMode]
 *  \until    [PressureControl.SetFullPowerMode]
 *
 *  \return true, if the mode setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::SetFullPowerMode()
{
    Log(tr("SetFullPowerMode"));
    return SetOperatingMode(PRESSURECTRL_OPMODE_FULL);
}

/****************************************************************************/
/*!
 *  \brief Script-API:Set pressure control operation mode
 *
 *  This method performs setting of pressure control operation mode as full
 *  power save mode.
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.SetPowerSaveMode]
 *  \until    [PressureControl.SetPowerSaveMode]
 *
 *  \return true, if the mode setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::SetPowerSaveMode()
{
    Log(tr("SetPowerSaveMode"));
    return SetOperatingMode(PRESSURECTRL_OPMODE_HOLD);
}

/****************************************************************************/
/*!
 *  \brief Helper function to set the operation mode
 *
 *  \iparam PressureCtrlOpMode   Operation mode
 *
 *  \return true, if the mode setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::SetOperatingMode(PressureCtrlOperatingMode_t PressureCtrlOpMode)
{
    m_TargetOperatingMode = PressureCtrlOpMode;
    bool ok = HandleErrorCode(m_pPressureControl->SetOperatingMode(PressureCtrlOpMode));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetOperatingMode.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  slot for set pressure control operation mode
 *
 *  This slot is connected to the signal ReportPressureCtrlOpMode
 *
 *  \iparam   ReturnCode      ReturnCode of function level Layer
 *  \iparam   PressureCtrlOpMode  Operation mode
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::OnSetPressureCtrlOpMode(quint32, ReturnCode_t ReturnCode, PressureCtrlOperatingMode_t PressureCtrlOpMode)
{
    qint32 ret = 1;
    m_CurrentOperatingMode = PressureCtrlOpMode;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopSetOperatingMode.isRunning()) {
        if (m_TargetOperatingMode != m_CurrentOperatingMode) {
            Log(tr("ERROR: Target pressure ctrl operating mode not set."));
            ret = UNDEFINED;
        }
        m_TargetOperatingMode = PRESSURECTRL_OPMODE_UNDEF;
        m_LoopSetOperatingMode.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Reset heater operating time
 *
 *  This method performs resetting of the operating time of a single heater
 *  back to null.
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.ResetHeaterOperatingTime]
 *  \until    [PressureControl.ResetHeaterOperatingTime]
 *
 *  \iparam Index = Index number of the heating element
 *
 *  \return true, if the mode setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::ResetPumpOperatingTime(quint8 Index)
{
    bool ok = HandleErrorCode(m_pPressureControl->ResetPumpOperatingTime(Index));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopResetHeaterOperatingTime.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  slot for reseting the heater operating time
 *
 *  This slot is connected to the signal ReportResetHeaterOperatingTime
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Index         Index number of the heating element
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::OnResetPumpOperatingTime(quint32, ReturnCode_t ReturnCode, quint8 Index)
{
    Q_UNUSED(Index)
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopResetHeaterOperatingTime.isRunning()) {
        m_LoopResetHeaterOperatingTime.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Get heater operating time
 *
 *  This method performs getting of the operating time of a single heater in
 *  seconds.
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.GetHeaterOperatingTime]
 *  \until    [PressureControl.GetHeaterOperatingTime]
 *
 *  \iparam Index = Index number of the heating element
 *
 *  \return Heater operating time in seconds
 */
/****************************************************************************/
quint32 WrapperFmPressureControl::GetPumpOperatingTime(quint8 Index)
{
    bool ok = HandleErrorCode(m_pPressureControl->GetPumpOperatingTime(Index));
    if (!ok) {
        return UNDEFINED;
    }
    qint32 ret = m_LoopGetHeaterOperatingTime.exec();
    if (ret != 1) {
        return UNDEFINED;
    }
    return m_HeaterOperatingTime;
}

/****************************************************************************/
/*!
 *  \brief  slot for getting the heater operating time
 *
 *  This slot is connected to the signal ReportHeaterOperatingTime
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Index         Index number of the heating element
 *  \iparam   OperatingTime Heater operating time in seconds
 */
/****************************************************************************/
void WrapperFmPressureControl::OnGetPumpOperatingTime(quint32, ReturnCode_t ReturnCode, quint8 Index, quint32 OperatingTime)
{
    Q_UNUSED(Index)
    m_HeaterOperatingTime = OperatingTime;
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopGetHeaterOperatingTime.isRunning()) {
        m_LoopGetHeaterOperatingTime.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Get fan speed
 *
 *  This method performs getting of the speed of a single ventilation fan in
 *  revelations per second.
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.GetFanSpeed]
 *  \until    [PressureControl.GetFanSpeed]
 *
 *  \iparam Index = Index number of the fan
 *
 *  \return Ventilation fan speed in revelations per second
 */
/****************************************************************************/
quint16 WrapperFmPressureControl::GetFanSpeed(quint8 Index)
{
    bool ok = HandleErrorCode(m_pPressureControl->GetFanSpeed(Index));
    if (!ok) {
        return UNDEFINED;
    }
    qint32 ret = m_LoopGetFanSpeed.exec();
    if (ret != 1) {
        return UNDEFINED;
    }
    return m_FanSpeed;
}

/****************************************************************************/
/*!
 *  \brief  slot for getting the ventilation fan speed
 *
 *  This slot is connected to the signal ReportFanSpeed
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Index         Index number of the heating element
 *  \iparam   FanSpeed      Fan speed in revelations per minute
 */
/****************************************************************************/
void WrapperFmPressureControl::OnGetFanSpeed(quint32, ReturnCode_t ReturnCode, quint8 Index, quint16 FanSpeed)
{
    Q_UNUSED(Index)
    m_FanSpeed = FanSpeed;
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopGetFanSpeed.isRunning()) {
        m_LoopGetFanSpeed.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Get hardware information
 *
 *  This method performs getting of the Slave's hardware information. This
 *  includes: number of pressure sensors, number of ventilation fans,
 *  number of heaters, number of PID controllers in the control loop, current
 *  through the heating circuit in milliamperes.
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.GetHardwareStatus]
 *  \until    [PressureControl.GetHardwareStatus]
 *
 *  \return Hardware information
 */
/****************************************************************************/
WrapperFmPressureControl::HardwareStatus_t *WrapperFmPressureControl::GetHardwareStatus()
{
    bool ok = HandleErrorCode(m_pPressureControl->GetHardwareStatus());
    if (!ok) {
        return NULL;
    }
    qint32 ret = m_LoopGetHardwareStatus.exec();
    if (ret != 1) {
        return NULL;
    }
    return &m_HardwareStatus;
}

/****************************************************************************/
/*!
 *  \brief  slot for getting the hardware information
 *
 *  This slot is connected to the signal ReportHardwareStatus
 *
 *  \iparam ReturnCode = ReturnCode of function level Layer
 *  \iparam Sensors = Number of pressure sensors connected to the board
 *  \iparam Fans = Number of ventilation fans connected to the board
 *  \iparam Heaters = Number of heating elements connected to the board
 *  \iparam Pids = Number of PID controllers in the control loop
 *  \iparam Current = Current through the heatinf circuit in milliamperes
 */
/****************************************************************************/
void WrapperFmPressureControl::OnGetHardwareStatus(quint32, ReturnCode_t ReturnCode, quint8 Sensors, quint8 Fans,
                                               quint8 Heaters, quint8 Pids, quint16 Current)
{
    m_HardwareStatus.Sensors = Sensors;
    m_HardwareStatus.Fans = Fans;
    m_HardwareStatus.Heaters = Heaters;
    m_HardwareStatus.Pids = Pids;
    m_HardwareStatus.Current = Current;
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopGetHardwareStatus.isRunning()) {
        m_LoopGetHardwareStatus.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Reset internal states
 *
 *  The internal states of the instance are reset by calling this method.
 *  This is useful for development and debugging when the wrapper layer
 *  is waiting for a signal in vain.
 *  The implementation can be expanded to reset and reconfigure slaves.
 */
/****************************************************************************/
void WrapperFmPressureControl::Reset()
{
   m_TargetPressure = UNDEFINED;
   m_CurrentPressure = UNDEFINED;
   m_TargetPressureCtrlStatus = PRESSURECTRL_STATUS_UNDEF;
   m_CurrentPressureCtrlStatus = PRESSURECTRL_STATUS_UNDEF;
   m_TargetOperatingMode = PRESSURECTRL_OPMODE_UNDEF;
   m_CurrentOperatingMode  = PRESSURECTRL_OPMODE_UNDEF;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Checks the pressure control mode
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.GetPowerOperationMode]
 *  \until    [PressureControl.GetPowerOperationMode]
 *
 *  \return string representation of the operation mode:
 *   full power mode, full safe mode, undefined
 *
 */
/****************************************************************************/
QString WrapperFmPressureControl::GetPowerOperationMode()
{
    Log(tr("GetPowerOperationMode"));
    QString OperationMode = "undefined";
    if(m_CurrentOperatingMode == PRESSURECTRL_OPMODE_FULL) {
        OperationMode = "full power mode";
    } else if(m_CurrentOperatingMode == PRESSURECTRL_OPMODE_HOLD) {
        OperationMode = "powersave mode";
    }
    return OperationMode;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Check if the pressure control is ON or not
 *
 *  This method checks if the pressure is ON
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.IsPressureControllOn]
 *  \until    [PressureControl.IsPressureControllOn]
 *
 *  \return true, if the pressure control state is InsideRange or
 *  OutsideRange else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::IsPressureControlOn()
{
   return (m_CurrentPressureCtrlStatus == PRESSURECTRL_STATUS_ON);
}

/****************************************************************************/
/*!
 *  \brief  Helper function to check if the pressure control is Off
 *
 *  This method checks if the wrapper oven pressure control state is Off.
 *
 *  \return true, if the pressure control is Off, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::IsPressureControlOff()
{
    return (m_CurrentPressureCtrlStatus == PRESSURECTRL_STATUS_OFF);
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Check the pressure control state
 *
 *  This method checks if the pressure control state is
 *  Off/IsideRange/OutSideRange
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.GetPressureControlState]
 *  \until    [PressureControl.GetPressureControlState]
 *
 *  \return string representation of pressure control state:
 *  Off, InsideRange, OutSideRange
 *
 */
/****************************************************************************/
QString WrapperFmPressureControl::GetPressureControlState()
{
    Log(tr("GetPressureControlState"));
    bool ok = HandleErrorCode(m_pPressureControl->ReqStatus());
    if (!ok) {
        m_CurrentPressureCtrlStatus = PRESSURECTRL_STATUS_UNDEF;
        return "Error";
    }
    qint32 ret = m_LoopGetPressureControl.exec();
    QString controlstate = "";
    if (ret != 1) {
        controlstate = "Error";
    } else if (IsPressureControlOn()) {
        if (IsInsideRange()) {
            controlstate = "InsideRange";
        } else if (IsOutsideRange()){
            controlstate = "OutsideRange";
        }
    } else if (IsPressureControlOff()){
        controlstate = "Off";
    }
    return controlstate;
}

/****************************************************************************/
/*!
 *  \brief  Helper function to check if the control state is in InsideRange
 *
 *  This method checks if the pressure control state is inside range.
 *
 *  \return true, if the pressure control is InsideRange, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::IsInsideRange()
{
    if(GetPressure(0) != UNDEFINED) {
        if((m_TargetPressure != UNDEFINED) || (m_CurrentPressure != UNDEFINED)) {
            if ((m_CurrentPressure > m_TargetPressure - TOLERANCE)||
                    (m_CurrentPressure < m_TargetPressure + TOLERANCE)) {
                return true;
            } else {
                return false;
            }
        }
    }
    Log(tr("Error"));
    return false;
}

/****************************************************************************/
/*!
 *  \brief  Helper function to check if the control state is in OutsideRange
 *
 *  This method checks if the pressure control state is outside range.
 *
 *  \return true, if the pressure control is OutsideRange, else false
 *
 */
/****************************************************************************/
bool WrapperFmPressureControl::IsOutsideRange()
{
    if(GetPressure(0) != UNDEFINED) {
        if((m_TargetPressure != UNDEFINED) || (m_CurrentPressure != UNDEFINED)) {
            if ((m_CurrentPressure < m_TargetPressure - TOLERANCE)||
                    (m_CurrentPressure > m_TargetPressure + TOLERANCE)) {
                return true;
            } else {
                return false;
            }
        }
    }
    Log(tr("Error"));
    return false;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Check the mains voltage state
 *
 *  This method checks if the mains voltage state is in the 220V or the 110V
 *  range
 *
 *  Examples:
 *  \dontinclude pressurecontrol.js
 *  \skipline [PressureControl.GetMainsVoltageState]
 *  \until    [PressureControl.GetMainsVoltageState]
 *
 *  \return string representation of the mains voltage state:
 *  220V, 110V
 *
 */
/****************************************************************************/
QString WrapperFmPressureControl::GetMainsVoltageState()
{
    Log(tr("GetPressureControlState"));
    bool ok = HandleErrorCode(m_pPressureControl->ReqStatus());
    if (!ok) {
        m_MainsVoltageStatus = PRESSURECTRL_VOLTAGE_UNDEF;
        return "Error";
    }
    qint32 ret = m_LoopGetVoltageState.exec();
    QString voltagestate = "";
    if (ret != 1) {
        voltagestate = "Error";
    } else if (m_MainsVoltageStatus == PRESSURECTRL_VOLTAGE_220V) {
        voltagestate = "220V";
    } else if (m_MainsVoltageStatus == PRESSURECTRL_VOLTAGE_110V){
        voltagestate = "110V";
    }
    return voltagestate;
}
/****************************************************************************/
/*!
 *  \brief  slot for getting the level sensor state
 *
 *  This slot is connected to the signal ReportFanSpeed
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   State         The current state of the level sensor
 */
/****************************************************************************/
void WrapperFmPressureControl::OnLevelSensorState(quint32, ReturnCode_t ReturnCode, quint8 State)
{
    Log(tr("New Level Sensor State is: %1").arg(State));
    qDebug()<< "WrapperFmPressureControl::OnLevelSensorState -----"<< State;
    if(State == 1)
    {
        if(m_LoopSuckingLevelSensor.isRunning())
        {
            m_LoopSuckingLevelSensor.exit(State);
        }
    }
}

void WrapperFmPressureControl::OnError(quint32 InstanceID, quint16 ErrorGroup, quint16 ErrorCode, quint16 ErrorData, QDateTime ErrorTime)
{
    //Log(tr("Pressure control get error: %1 %2 %3 %4").arg(InstanceID).arg(ErrorGroup).arg(ErrorCode).arg(ErrorData));
    Log(tr("Pressure control get error, Instance ID: %1, Error Group: %2, Error Code: %3, Error Data: %4").arg(GetNameByInstanceID(InstanceID)).arg(ErrorGroup).arg(ErrorCode).arg(ErrorData));
}
/****************************************************************************/
/*!
 *  \brief  Script-API: Set the working pressure for Pressure and Vaccum.
 *
 *  \iparam WorkingPressure: the working pressure in kpa.
 *
 *  \return void
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::SetWorkingPressure(qint32 WorkingPressure)
{
    if(WorkingPressure > 0)
    {
        m_WorkingPressurePositive = WorkingPressure;
    }
    else if(WorkingPressure < 0)
    {
        m_WorkingPressureNegative = WorkingPressure;
    }
}
/****************************************************************************/
/*!
 *  \brief  Script-API: Print the working pressure for Pressure and Vaccum.
 *
 *  \return void
 *
 */
/****************************************************************************/
void WrapperFmPressureControl::GetWorkingPressure()
{
    Log(tr("The Positive pressure is: %1").arg(m_WorkingPressurePositive));
    Log(tr("The Negative pressure is: %1").arg(m_WorkingPressureNegative));
}

float WrapperFmPressureControl::ReadPressureDrift(void)
{
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/PressureNullDrift.txt";
    float pressureDrift = 0;
    FILE* pFile;

    if ((pFile = fopen(FileName.toStdString().c_str(), "r")) == NULL)
    {
        Log(tr("Cannot open %1.\n").arg(FileName.toStdString().c_str()));
        return 0;
    }

    char Buf[200];
    memset(Buf, 0, sizeof(Buf));
    if(fread(Buf, 1, 200, pFile) > 0 )
    {
        QString Content = QString::fromAscii(Buf, -1);
        QStringList StrList = Content.split(";");
        if(StrList.size() >= 1)
        {
            pressureDrift = StrList.at(0).toDouble();
        }
    }
    fclose(pFile);
    m_PressureDrift = pressureDrift;
    return pressureDrift;
}

void WrapperFmPressureControl::WritePressureDrift(float PressureDrift)
{
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/PressureNullDrift.txt";
    QString msg = tr("%1;\n").arg(PressureDrift);
    FILE* pFile = fopen(FileName.toStdString().c_str(), "w+");
    fprintf(pFile, "%s", msg.toStdString().c_str());
    fflush(pFile);
    fclose(pFile);
    system("sync");
    m_PressureDrift = PressureDrift;
}

void WrapperFmPressureControl::GetValveOperationTime(quint8 ValveIndex)
{
    quint32 value = m_pPressureControl->GetValveLifeCycle(ValveIndex);
    Log(tr("The Valve %1's operation time is: %2").arg(ValveIndex).arg(value));
}

void WrapperFmPressureControl::ResetValveOperationTime()
{
    bool ok = HandleErrorCode(m_pPressureControl->ResetValveOperationTime());
    if (!ok)
    {
        Log(tr("Successfully reset Valves' state to 0"));
    }
    else
    {
        Log(tr("Failed reset Valves' state to 0"));
    }
}
#endif //PRE_ALFA_TEST
