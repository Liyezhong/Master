/****************************************************************************/
/*! \file WrapperFmStepperMotor.cpp
 *
 *  \brief  Implementation file for class WrapperFMStepperMotor
 *
 *  \version  0.1
 *  \date     2012-04-10
 *  \author   T.Scheck
 *
 *  \b Description:
 *
 *   N/A
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2012 by Leica Biosystems Nussloch GmbH. All rights reserved
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 */
/****************************************************************************/

#include <QTimer>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/ResetData.h"
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmStepperMotor.h"
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperUtils.h"
#include <unistd.h>

const QString WrapperFmStepperMotor::ROTATION = QString("rotation");
const QString WrapperFmStepperMotor::LINEAR = QString("linear");

/****************************************************************************/
/*!
 *  \brief Constructor of class WrapperFMStepperMotor
 *
 *  \iparam   Name           The Name of the Device used for QtScript identifier.
 *  \iparam   pStepperMotor  Pointer to StepperMotor control instance
 *  \iparam   pParent        Optional pointer to parent for QObject Hierarchy.
 *
 */
/****************************************************************************/
WrapperFmStepperMotor::WrapperFmStepperMotor(QString Name,
    CStepperMotor *pStepperMotor,
    QObject *pParent)
    : WrapperBase(Name, pParent), m_pStepperMotor(pStepperMotor)
{ 
    Reset();

    mp_ResetData = new CResetData(*pStepperMotor, this);

    (void)connect(
        m_pStepperMotor, 
        SIGNAL(ReportReferenceMovementAckn(quint32, ReturnCode_t, qint32)),
        this, 
        SLOT(OnReferenceRun(quint32, ReturnCode_t, qint32))
    );
    (void)connect(
        m_pStepperMotor, 
        SIGNAL(ReportMovementAckn(quint32, ReturnCode_t, qint32, qint16)), 
        this, 
        SLOT(OnSetPosition(quint32, ReturnCode_t, qint32, qint16))
    );
    (void)connect(
        m_pStepperMotor, 
        SIGNAL(ReportPosition(quint32, ReturnCode_t, qint32, qint8)),
        this, 
        SLOT(OnGetPosition(quint32, ReturnCode_t, qint32, qint8))
    );
    (void)connect(
        m_pStepperMotor,
        SIGNAL(ReportSetStateAckn(quint32, ReturnCode_t)),
        this,
        SLOT(OnSetState(quint32, ReturnCode_t))
    );
    (void)connect(
        m_pStepperMotor,
        SIGNAL(ReportConfigureDone(quint32, ReturnCode_t)),
        this,
        SLOT(OnSetConfiguration(quint32, ReturnCode_t))
    );
    (void)connect(
        m_pStepperMotor,
        SIGNAL(ReportSpeed(quint32,ReturnCode_t,qint16)),
        this,
        SLOT(OnGetSpeed(quint32,ReturnCode_t,qint16))
    );
    (void)connect(
        m_pStepperMotor,
        SIGNAL(ReportLifeCycleData(quint32, ReturnCode_t, quint32, quint32, quint32)),
        this,
        SLOT(OnGetLifeCycleData(quint32, ReturnCode_t, quint32, quint32, quint32))
    );
    (void)connect(m_pStepperMotor, SIGNAL(ReportError(quint32, quint16, quint16, quint16, QDateTime)),
            this, SLOT(OnError(quint32,quint16,quint16,quint16,QDateTime)));
#ifdef PRE_ALFA_TEST
    InitEDPosition();
#endif

}

/****************************************************************************/
/*!
 *  \brief Destructor of class WrapperFmStepperMotor
 */
/****************************************************************************/
WrapperFmStepperMotor::~WrapperFmStepperMotor()
{
    try {
        delete mp_ResetData;
    }
    catch(...) {}
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Perform reference run of StepperMotor.
 *
 *  This method performs the reference run. It also implies enabling the module.
 *  It needs to be run, before any other actions can be initiated.
 *
 *  A reference run consists of several steps:
 *    - move fast in a predefined direction until assigned limit switch gets activated within the configured time and distance limits
 *    - move in reverse direction for a predefined distance
 *    - move slowly in original direction for up to 150% of reverse distance until limit switch is hit again
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmStepperMotor::DoReferenceRun()
{
    // enable function module before first use
    if (!SetState(true)) {
        return false;
    }

    bool ok = HandleErrorCode(m_pStepperMotor->ExecReferenceMovement());
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopReferenceRun.exec();

    return (ret == 1);
}


#ifdef PRE_ALFA_TEST

#define REFER_RUN_OK                  (1)
#define REFER_RUN_DCL_ERR             (-1)
#define REFER_RUN_TO_UPPER_LIMIT_ERR  (-2)
#define REFER_RUN_TO_LOWER_LIMIT_ERR  (-3)
#define REFER_RUN_UNEXPECTED_POSITION (-4)
#define REFER_RUN_NOT_INITIALIZED     (-5)
#define REFER_RUN_USER_ERROR          (-6)
#define REFER_RUN_LOWER_LIMIT         (600)
#define REFER_RUN_TIMEROUT_IN_MS      (1100)
#define REFER_RUN_RETRY_TIME          (20)
qint32 WrapperFmStepperMotor::ReferenceRunWithTimeout(quint32 LowerLimit, quint32 UpperLimit)
{
    qint32 ret = REFER_RUN_OK;
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(ReferenceRunTimerCB()));
    timer.setSingleShot(true);

    bool stop = false;
    quint8 retry = 0;
    while(!stop)
    {
        stop = true;
        timer.start(UpperLimit);
        m_RefRunRetCode = REFER_RUN_OK;
        qint64 TBefore = QDateTime::currentMSecsSinceEpoch();

        DoReferenceRun();
        qint64 TAfter = QDateTime::currentMSecsSinceEpoch();
#if 1
        Log(tr("cost time: %1").arg(TAfter - TBefore));
        bool ok;
        qint32 position = GetPosition().toInt(&ok);

        Log(tr("steps is: %1").arg(position));
#endif
        if((TAfter - TBefore) < LowerLimit)
        {
            if((retry++) < REFER_RUN_RETRY_TIME)
            {
                stop = false;
                Log(tr("Motor running lower limit exception: retry time: %1, retry now.").arg(retry));
            }
            else
            {
                Log(tr("Motor moving retry time exceed %1, may be stucked!").arg(REFER_RUN_RETRY_TIME));
                ret = REFER_RUN_TO_LOWER_LIMIT_ERR;
            }
        }
        else if(m_RefRunRetCode == REFER_RUN_TO_UPPER_LIMIT_ERR)
        {
            ret = REFER_RUN_TO_UPPER_LIMIT_ERR;
        }
        timer.stop();
    }
#if 1
    if(ret != REFER_RUN_OK)
    {
        Log(tr("Reference run with timeout Error CODE: %1").arg(ret));
    }
#endif
    return ret;
}

void WrapperFmStepperMotor::ReferenceRunTimerCB()
{
    if(m_LoopReferenceRun.isRunning())
    {
        m_RefRunRetCode = REFER_RUN_TO_UPPER_LIMIT_ERR;
        m_LoopReferenceRun.exit( -1 );
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Perform reference run of StepperMotor with step check.
 *
 *  This method performs the reference run with step check, that is to say,
 *  one single move must cost step in target range, otherwise there may be a
 *  stuck or other error.
 *
 *  It also implies enabling the module.
 *  It needs to be run, before any other actions can be initiated.
 *
 *  A reference run with step check consists of several steps:
 *    - move fast in a predefined direction until assigned limit switch gets activated within the configured time and distance limits
 *    - Get moving steps. Judge if the step is in the ecpected range.
 *    - If the step is smaller than lower limit, retry the previous step till succeed or exceed the max retry time.
 *    - if the step is bigger than the upper limit, log a warining.
 *  \return true, if the reference run succeeded, else false
 *
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::DoReferenceRunWithStepCheck(quint32 LowerLimit, quint32 UpperLimit)
{
    qint32 ret = REFER_RUN_OK;

    bool stop = false;
    quint8 retry = 0;
    quint8 GetPositionRetry = 0;
    quint32 Step = 0;
    while(!stop)
    {
        stop = true;
        m_RefRunRetCode = REFER_RUN_OK;
#if 1
        qint64 TBefore = QDateTime::currentMSecsSinceEpoch();
#endif
        DoReferenceRun();
#if 1
        qint64 TAfter = QDateTime::currentMSecsSinceEpoch();
        Log(tr("cost time: %1").arg(TAfter - TBefore));
#endif
        while((GetPositionRetry++) <5)
        {
            bool ok;
            Step = qAbs(GetPosition().toInt(&ok));
            Log(tr("Steps is: %1").arg(Step));
            if(Step > 0)
            {
               break;
            }
            WrapperUtils* Utils = new WrapperUtils(this);
            Log(tr("Warning: Get position error, wait 0.5 sec to read again."));
            Utils->Pause(500);
        }
        if(GetPositionRetry == 5)
        {
            ret = REFER_RUN_RETRY_TIME;
        }
        else
        {
            GetPositionRetry = 0;
        }
        if(Step < LowerLimit)
        {
            if((retry++) < REFER_RUN_RETRY_TIME)
            {
                stop = false;
                Log(tr("Motor running lower limit exception: retry time: %1, retry now.").arg(retry));
            }
            else
            {
                Log(tr("Motor moving retry time exceed %1, may be stucked!").arg(REFER_RUN_RETRY_TIME));
                ret = REFER_RUN_TO_LOWER_LIMIT_ERR;
            }
        }
        else if(Step > UpperLimit)
        {
            Log(tr("Warning: Motor moving steps exceed upper limit: %1!").arg(UpperLimit));
            ret = REFER_RUN_TO_UPPER_LIMIT_ERR;
        }

    }

    return ret;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Let the rotary valve move to initial position.
 *
 *  This method let the rotary valve move to its initial position.
 *  To achieve this, this function will set both limit switch 1 and limit swicth
 *  2 as active, then use the reference run function to let the motor move.
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
qint32 WrapperFmStepperMotor::MoveToInitialPosition(bool TubeFlag, quint32 Position)
{
    QString lsCode;
    qint32 RetValue = REFER_RUN_OK;
    bool ParaChange = false;

    // added by Sunny on Oct, 14, 2014
    if (Position != 0) {
        quint32 EDPosition = 0;
        if (TubeFlag) {
            EDPosition = Position*2-1;
        }
        else {
            EDPosition = Position*2;
        }
        Log(QString( "INFO: Set the current RV position:%1, TubeFlag:%d").arg(Position).arg(TubeFlag));
        SetEDPosition(EDPosition);
        SetPrevEDPosition(EDPosition);
        return REFER_RUN_OK;
    }
    ////

//reconfig for original position
    if(GetConfigLS2Exists()==0)
    {
      SetConfigLS2Exists(1);
      SetConfigPos2Value(1);
      SetConfigPos3Value(1);
      ParaChange |= true;
    }
    if(GetConfigRefRunPosition() != 3)
    {
        SetConfigRefRunPosition(3);
        ParaChange |= true;
    }
    if(GetConfigPos1Stop() != 0)
    {
        SetConfigPos1Stop(0);
        ParaChange |= true;
    }
    if(GetConfigPos3Stop() != 1)
    {
        SetConfigPos3Stop(1);
        ParaChange |= true;
    }
    if(ParaChange)
    {
        ApplyNewParameterSet();
    }
    lsCode = GetLimitSwitchCode();
    if(lsCode == "3")
    {
        Log(tr("Already At Initial Position, No Need To Move!"));
        SetEDPosition(1);
        return REFER_RUN_OK;
    }
#if 1
    //RetValue = ReferenceRunWithTimeout(REFER_RUN_LOWER_LIMIT, 65000);
    RetValue = DoReferenceRunWithStepCheck(50, 7000);
#else
    RetValue = DoReferenceRun();
#endif
    lsCode = GetLimitSwitchCode();
    if((lsCode == "3")&&(RetValue == REFER_RUN_OK))
    {
        Log(tr("Hit Initial Position!"));
        SetEDPosition(1);
    }
    else
    {
        SetEDPosition(0);
        Log(tr("Hit unexpected position, please retry!"));
        Log(tr("The Limit Switch code is: %1").arg(lsCode));
    }
    return RetValue;
}
/****************************************************************************/
/*!
 *  \brief Helper function, return the current encoder disk's position.
 *
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
quint32 WrapperFmStepperMotor::GetEDPosition()
{
    if(m_EncoderDiskPosition > 32)
    {
        m_EncoderDiskPosition = 0;
    }

    return m_EncoderDiskPosition;
}
/****************************************************************************/
/*!
 *  \brief Helper function, set the current encoder disk's position.
 *
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
void WrapperFmStepperMotor::SetEDPosition(quint32 position)
{

    if((position <33) &&( position >0))
    {
        m_EncoderDiskPosition = position;
    }
    else
    {
        m_EncoderDiskPosition = 0;
    }
}

/****************************************************************************/
/*!
 *  \brief  Helper function: Get previous encoder disk's position.
 *
 *  \return Encoder disk position.
 */
/****************************************************************************/
quint32 WrapperFmStepperMotor::GetPrevEDPosition()
{
    if(m_PrevEncoderDiskPosition > 32)
    {
        m_PrevEncoderDiskPosition = 0;
    }

    return m_PrevEncoderDiskPosition;
}

/****************************************************************************/
/*!
 *  \brief  Helper function: Set previous encoder disk's position.
 *
 *  \iparam position = Encoder disk position.
 */
/****************************************************************************/
void WrapperFmStepperMotor::SetPrevEDPosition(quint32 position)
{

    if((position < 33) && (position > 0))
    {
        m_PrevEncoderDiskPosition = position;
    }
    else
    {
        m_PrevEncoderDiskPosition = 0;
    }
}

/****************************************************************************/
/*!
 *  \brief Helper function, initialize the current encoder disk's position.
 *
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
void WrapperFmStepperMotor::InitEDPosition()
{
    m_EncoderDiskPosition = 0;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Let the rotary valve move to certain tube position.
 *
 *  This method let the rotary valve move to certain tube position.
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
qint32 WrapperFmStepperMotor::MoveToTubePosition(quint32 Position)
{
    if((Position < 1)||(Position > 16))
    {
        Log(tr("The Tube No You Input: %1 is Invalid").arg(Position));
        return REFER_RUN_USER_ERROR;
    }
    return MoveToEDPosition(Position*2 - 1);
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Let the rotary valve move to certain seal position.
 *
 *  This method let the rotary valve move to certain seal position.
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
qint32 WrapperFmStepperMotor::MoveToSealPosition(quint32 Position)
{
    if((Position < 1)||(Position > 16))
    {
        Log(tr("The Tube No You Input: %1 is Invalid").arg(Position));
        return REFER_RUN_USER_ERROR;
    }
    return MoveToEDPosition(Position*2);
}

/****************************************************************************/
/*!
 *  \brief  Helper function: Let the rotary valve move to certain encoder disk position.
 *
 *  This method let the rotary valve move to certain encoder disk position.
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
qint32 WrapperFmStepperMotor::MoveToEDPosition(quint32 Position)
{
    qint32 ret = REFER_RUN_OK;
    quint32 EDPosition = GetEDPosition();
    float MoveSteps = 0;
    bool cw = false;

    QString lsCode, lsCodeDup;
    quint8 retry = 0;

    if(0 == EDPosition)
    {
        lsCode = GetLimitSwitchCode();
        lsCodeDup = GetLimitSwitchCode();
        while(lsCode != lsCodeDup)
        {
            lsCode = GetLimitSwitchCode();
            lsCodeDup = GetLimitSwitchCode();
            Log(tr("Limit Switch code are not stable! Retry time: %1").arg(retry));
            if(retry++ > 3)
            {
                Log(tr("Please run Move To Initial position first!"));
                return REFER_RUN_UNEXPECTED_POSITION;
            }
        }
        if(lsCode == "3")
        {
            SetEDPosition(1);
        }
        else
        {
            Log(tr("Can't find current position, please run MoveToInitialPosition first!"));
            return REFER_RUN_NOT_INITIALIZED;
        }
    }

    if((Position < 1)||(Position > 32))
    {
        Log(tr("The Tube No You Input: %1 is Invalid").arg(Position));
        return REFER_RUN_USER_ERROR;
    }

    if(EDPosition == Position)
    {
        Log(tr("Already At Target Position, No Need To Move!").arg(EDPosition));
        return REFER_RUN_OK;
    }
    else
    {
        MoveSteps = ((Position > EDPosition)?(Position - EDPosition):(EDPosition - Position));
        if(MoveSteps > 15)
        {
            MoveSteps = 32 - MoveSteps;
            cw = (Position > EDPosition ? true : false);
        }
        else
        {
            cw = (Position > EDPosition ? false : true);
        }
    }
    if(cw)
    {
        for(int i =0;i<MoveSteps;i++)
        {
            ret = MoveToNextPortCW();
            if(REFER_RUN_OK != ret)
            {
                return ret;
            }
        }
    }
    else
    {
        for(int i =0;i<MoveSteps;i++)
        {
            ret = MoveToNextPortCCW();
            if ( REFER_RUN_OK != ret)
            {
                return ret;
            }
        }
    }
    return ret;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Let the rotary valve move to next port clockwisly.
 *
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
qint32 WrapperFmStepperMotor::MoveCW()
{
    qint32 ret;
    bool ParaChange = false;
    if(GetConfigLS2Exists()==1)
    {
        //EnableLs2(0);
        SetConfigLS2Exists(0);
        SetConfigPos2Value(0);
        SetConfigPos3Value(0);
        ParaChange |= true;
    }
    if(GetConfigRefRunPosition() != 1)
    {
        SetConfigRefRunPosition(1);
        ParaChange |= true;
    }
    if(GetConfigPos1Stop() != 1)
    {
        SetConfigPos1Stop(1);
        ParaChange |= true;
    }
    if(GetConfigPos3Stop() != 0)
    {
        SetConfigPos3Stop(0);
        ParaChange |= true;
    }
    if(ParaChange)
    {
        ApplyNewParameterSet();
    }

    if( DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW == GetRotationDirection())
    {
        SetRotationDirection(DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW);
        ret = MoveToNextPort(true, \
                             GetLowerLimit(0, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, true), \
                             GetUpperLimit(0, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, true));

    }
    else
    {
        ret = MoveToNextPort(false, \
                             GetLowerLimit(0, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, false), \
                             GetUpperLimit(0, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, false));
    }
    return ret;
}

qint32 WrapperFmStepperMotor::MoveBackOnePort()
{
    quint8 dir = GetRotationDirection();
    if(dir == DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW)
    {
        return MoveCW();
    }
    else
    {
        return MoveCCW();
    }
}
/****************************************************************************/
/*!
 *  \brief  Helper function: Get the upper limit for movment at certain encoder disk position .
 *
 *
 *  \return the upper limit
 *
 */
inline quint32 WrapperFmStepperMotor::GetUpperLimit(quint32 CurrentEDPosition, DeviceControl::CANFctModuleStepperMotor::RotationDir_t Direction, bool ChangeDirection)
{
    quint32 UpperLimit = 500;
    if(CurrentEDPosition != 0)
    {
    if( DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW == Direction)
    {
        if(3 == CurrentEDPosition)
        {
            UpperLimit = 194;
        }
        else if(2 == CurrentEDPosition)
        {
            UpperLimit = 350;
        }
        else if(24 == CurrentEDPosition)
        {
            UpperLimit = 496;
        }
        else if(26 == CurrentEDPosition)
        {
            UpperLimit = 350;
        }
        else if(99 == CurrentEDPosition)
        {
            UpperLimit = 130;
        }
        else
        {
            UpperLimit = 350;
        }
    }
    else if(DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW == Direction)
    {
        if(2 == CurrentEDPosition)
        {
            UpperLimit = 130;
        }
        else if(1 == CurrentEDPosition)
        {
            UpperLimit = 350;
        }
        else if(23 == CurrentEDPosition)
        {
            UpperLimit = 496;
        }
        else if(25 == CurrentEDPosition)
        {
            UpperLimit = 350;
        }
        else if(99 == CurrentEDPosition)
        {
            UpperLimit = 194;
        }
        else
        {
            UpperLimit = 350;
        }
    }
    }
    if(ChangeDirection)
    {
        UpperLimit += 50;
    }
    return UpperLimit;
}

/****************************************************************************/
/*!
 *  \brief  Helper function: Get the lower limit for movment at certain encoder disk position .
 *
 *
 *  \return the lower limit
 *
 */
inline quint32 WrapperFmStepperMotor::GetLowerLimit(quint32 CurrentEDPosition, DeviceControl::CANFctModuleStepperMotor::RotationDir_t Direction, bool ChangeDirection)
{
    quint32 LowerLimit = 100;
    if(CurrentEDPosition != 0)
    {
    if( DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW == Direction)
    {
        if(99 == CurrentEDPosition)
        {
            LowerLimit = 30;
        }
        else
        {
            LowerLimit = 100;
        }
    }
    else if(DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW == Direction)
    {
        if( 2 == CurrentEDPosition)
        {
            LowerLimit = 30;
        }
        else
        {
            LowerLimit = 100;
        }
    }
    }
    if(ChangeDirection)
    {
        LowerLimit += 50;
    }
    return LowerLimit;
}
/****************************************************************************/
/*!
 *  \brief  Script-API: Let the rotary valve move to next port counter-clockwisly.
 *
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
qint32 WrapperFmStepperMotor::MoveCCW()
{
    qint32 ret;
    bool ParaChange = false;
    if(GetConfigLS2Exists()==1)
    {
        //EnableLs2(0);
        SetConfigLS2Exists(0);
        SetConfigPos2Value(0);
        SetConfigPos3Value(0);
        ParaChange |= true;
    }
    if(GetConfigRefRunPosition() != 1)
    {
        SetConfigRefRunPosition(1);
        ParaChange |= true;
    }
    if(GetConfigPos1Stop() != 1)
    {
        SetConfigPos1Stop(1);
        ParaChange |= true;
    }
    if(GetConfigPos3Stop() != 0)
    {
        SetConfigPos3Stop(0);
        ParaChange |= true;
    }
    if(ParaChange)
    {
        ApplyNewParameterSet();
    }

    if( DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW == GetRotationDirection())
    {
        SetRotationDirection(DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW );
        ret = MoveToNextPort(true, \
                             GetLowerLimit(0, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, true), \
                             GetUpperLimit(0, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, true));
    }
    else
    {
        ret = MoveToNextPort(false, \
                             GetLowerLimit(0, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, false), \
                             GetUpperLimit(0, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, false));
    }
    return ret;
}
/****************************************************************************/
/*!
 *  \brief  Helper function: Let the rotary valve move to next port clockwisly.
 *
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
qint32 WrapperFmStepperMotor::MoveToNextPortCW()
{
    qint32 ret = REFER_RUN_OK;
    quint32 EDPosition = GetEDPosition();

    if(0 == EDPosition)
    {
        Log(tr("Can't find current position, please run MoveToInitialPosition first!"));
        return REFER_RUN_NOT_INITIALIZED;
    }

    if( DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW == GetRotationDirection())
    {
        SetRotationDirection(DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW );
        ret = MoveToNextPort(true, \
                             GetLowerLimit(EDPosition, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, true), \
                             GetUpperLimit(EDPosition, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, true));
    }
    else
    {
        ret = MoveToNextPort(false, \
                             GetLowerLimit(EDPosition, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, false), \
                             GetUpperLimit(EDPosition, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, false));
    }

    if((ret == REFER_RUN_OK)&&(3 == EDPosition))
    {
        ret = MoveToNextPort(false, \
                             GetLowerLimit(99, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, false), \
                             GetUpperLimit(99, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW, false));
    }

    if((ret == REFER_RUN_OK)&&(EDPosition == GetEDPosition()))
    {
        EDPosition -= 1;
        if(EDPosition == 0)
        {
            EDPosition = 32;
        }
        SetEDPosition(EDPosition);
        Log(tr("CW Hit Position: %1").arg(TranslateFromEDPosition(EDPosition)));
    }
    else
    {
        Log(tr("Unknown error happened, lost current position, please run MoveToInitialPosition"));
        SetEDPosition(0);
    }
    return ret;
}
/****************************************************************************/
/*!
 *  \brief  Helper function: Get the name of certain encoder disk postion.
 *
 *
 *  \return The name of certain position.
 *
 */
QString WrapperFmStepperMotor::TranslateFromEDPosition(quint32 EDPosition)
{
    QString RetValue = "Invalid position";
    if((EDPosition > 0)&&(EDPosition < 33))
    {
        quint32 divide = EDPosition / 2;
        if((divide*2) == EDPosition)
        {
            RetValue = tr("Seal %1").arg(divide);
        }
        else
        {
            RetValue = tr("Tube %1").arg(divide + 1);
        }
    }
    return RetValue;
}
/****************************************************************************/
/*!
 *  \brief  Helper function: Let the rotary valve move to next port counter-clockwisly.
 *
 *
 *  \return true, if the reference run succeeded, else false
 *
 */
qint32 WrapperFmStepperMotor::MoveToNextPortCCW()
{
    qint32 ret = REFER_RUN_OK;
    quint32 EDPosition = GetEDPosition();

    if(0 == EDPosition)
    {
        Log(tr("Can't find current position, please run MoveToInitialPosition first!"));
        return REFER_RUN_NOT_INITIALIZED;
    }

    if( DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW == GetRotationDirection())
    {
        SetRotationDirection(DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW );
        ret = MoveToNextPort(true, \
                             GetLowerLimit(EDPosition, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, true), \
                             GetUpperLimit(EDPosition, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, true));
    }
    else
    {
        ret = MoveToNextPort(false, \
                             GetLowerLimit(EDPosition, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, false), \
                             GetUpperLimit(EDPosition, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, false));
    }
    if(2 == EDPosition)
    {
        ret = MoveToNextPort(false, \
                             GetLowerLimit(99, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, false), \
                             GetUpperLimit(99, DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW, false));
    }

    if((ret == REFER_RUN_OK)&&(EDPosition == GetEDPosition()))
    {
        EDPosition += 1;
        if(EDPosition == 33)
        {
            EDPosition = 1;
        }
        SetEDPosition(EDPosition);
        Log(tr("CCW Hit Position: %1").arg(TranslateFromEDPosition(EDPosition)));
    }
    else
    {
        Log(tr("Unknown error happened, lost current position, please run MoveToInitialPosition"));
        SetEDPosition(0);
    }
    return ret;
}

/****************************************************************************/
/*!
 *  \brief  Helper function: Move to next port without change previous
 *                           direction configuration.
 *  \iparam changeParameter If to send can msg to change configuration.
 *  \iparam LowerLimit lower limit for the reference run
 *  \iparam UpperLimit upper limit for the reference run
 *
 *  \return The name of certain position.
 *
 */
qint32 WrapperFmStepperMotor::MoveToNextPort(bool changeParameter, quint32 LowerLimit, quint32 UpperLimit)
{
    bool ParaChange = changeParameter;
    QString lsCode;
    qint32 RetValue;
    if(GetConfigLS2Exists()==1)
    {
        //EnableLs2(0);
        SetConfigLS2Exists(0);
        SetConfigPos2Value(0);
        SetConfigPos3Value(0);
        ParaChange |= true;
    }
    if(GetConfigRefRunPosition() != 1)
    {
        SetConfigRefRunPosition(1);
        ParaChange |= true;
    }
    if(GetConfigPos1Stop() != 1)
    {
        SetConfigPos1Stop(1);
        ParaChange |= true;
    }
    if(GetConfigPos3Stop() != 0)
    {
        SetConfigPos3Stop(0);
        ParaChange |= true;
    }
    if(ParaChange)
    {
        ApplyNewParameterSet();
    }
#if 1
    //static quint32 LastED = 0;
    quint32 ED = GetEDPosition();
    Log(tr("last ED is: %1, lower limit is: %2, upper limit is %3 ").arg(ED).arg(LowerLimit).arg(UpperLimit));
    //RetValue = ReferenceRunWithTimeout(LowerLimit, UpperLimit);
    RetValue = DoReferenceRunWithStepCheck(LowerLimit, UpperLimit);
#else
    RetValue = DoReferenceRun();
#endif
    lsCode = GetLimitSwitchCode();
    if(lsCode != "1")
    {
        quint32 Retry = 0;
        //while(((Retry++) < 3)&&(lsCode != "1"))  // 2013.3.8 Frank's request
        while(((Retry++) < 30)&&(lsCode != "1"))
        {
            Log(tr("Warning: Get unexpected LS Code: %1, wait 0.5 sec to read again. Retry time: %2").arg(lsCode).arg(Retry));
            (void)usleep(500*1000);
            lsCode = GetLimitSwitchCode();
        }
        if(Retry >= 30) // 2013.3.8 Frank's request
        {
            Log(tr("Hit unexpected position! Already read LS code for 30 times"));
            //RetValue = REFER_RUN_UNEXPECTED_POSITION; // 2013.3.8 Frank's request, add LS examine
                                                        // 2013.5.30 Frank's request, ignore LS return
        }
    }
    return RetValue;
}
#endif
/****************************************************************************/
/*!
 *  \brief   slot for reference run
 *
 *  \iparam InstanceID  ID of the FM (currently not needed)
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *  \iparam Position    absolute position in half-steps
 *
 */
/****************************************************************************/
void WrapperFmStepperMotor::OnReferenceRun(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, qint32 Position)
{
    // exit from eventloop: 1 success, 0 timeout, -1 failure
    qint32 ret = 1;
    m_CurrentPosition = Position;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    } else {
        m_CurrentPositionKnown = true;
    }
    if (m_LoopReferenceRun.isRunning()) {
        m_LoopReferenceRun.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for ReferenceRun (p: %1).").arg(m_CurrentPosition));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set speed for a speed movement.
 *
 *  Examples:
 *  \skipline [Start/DriveSpeed]
 *  \until    [Start/DriveSpeed]
 *
 *  \iparam Speed speed in half-steps per second
 *          (negative speed rotates in opposite direction)
 *
 *  \return true, if the action succeeded, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::DriveSpeed(qint16 Speed)
{
    m_TargetSpeed = Speed;
    bool ok = HandleErrorCode(m_pStepperMotor->DriveSpeed(m_TargetSpeed, m_Profile));
    if (!ok) {
        return false;
    }

    qint32 ret = m_LoopRotation.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Move to an absolute position.
 *
 *  This action is only allowed in linear working mode.
 *
 *  For a detailed description of the Timeout parameter, please see \ref TimeoutIntro .
 *
 *  Examples:
 *  \skipline [MoveToPosition]
 *  \until    [MoveToPosition]
 *
 *  \iparam Position absolute position in half-steps
 *  \iparam Timeout  timeout in ms for the movement
 *
 *  \return true, if the action succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmStepperMotor::MoveToPosition(qint32 Position, qint32 Timeout)
{
    Log(tr("MoveToPosition %1, Profile %2").arg(Position).arg(m_Profile));
    return SetPosition(Position, Timeout);
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Relative movement to cover a given distance
 *
 *  This method is only valid in linear working mode.
 *
 *  For a detailed description of the Timeout parameter, please see \ref TimeoutIntro .
 *
 *  Examples:
 *  \skipline [MoveFromPosition]
 *  \until    [MoveFromPosition]
 *
 *  \iparam Distance distance in half-steps
 *  \iparam Timeout  timeout in ms for the movement
 *
 *  \return true, if the action succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmStepperMotor::MoveFromPosition(qint32 Distance, qint32 Timeout)
{
    Log(tr("MoveFromPosition %1, Profile %2").arg(Distance).arg(m_Profile));
    // Normally a ReferenceRun must be executed as the first action, so an
    // initial absolute position will be available. But you never know...
    if (!m_CurrentPositionKnown) {
        bool ok;
        GetPosition().toLong(&ok);
        if (!ok) {
            Log(tr("ERROR: Can't get current position for relative movement."));
            return false;
        }
    }
    qint32 Position = m_CurrentPosition + Distance;
    Log(tr("(moving from %1 to %2)").arg(m_CurrentPosition).arg(Position));
    return SetPosition(Position, Timeout);
}

/****************************************************************************/
/*!
 *  \brief  Helper function, sets a given absolute position
 *
 *  \iparam Position absolute position in half-steps
 *  \iparam Timeout  timeout in ms for the movement
 *
 *  \todo adapt DCL for case if limit switch is activated during movement
 *  \return true, if the action succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetPosition(qint32 Position, qint32 Timeout)
{
    if (!m_SetPositionActive) {
        m_SetPositionActive = true;
        m_TargetPosition = Position;
        bool ok = HandleErrorCode(
            m_pStepperMotor->DriveToPosition(Position, m_Profile)
        );
        if (!ok) {
           return false;
        }
    } else {
        Log(tr("ERROR: Task in progress."));
        return false;
    }

    if (Timeout == 0) {
        Log(tr("(called asynchronously)"));
        return true;
    }

    QTimer timer;
    if (Timeout > 0) {
        timer.setSingleShot(true);
        timer.setInterval(Timeout);
        timer.start();
        // quit via timer is the same as exit(0)
        (void)connect(&timer, SIGNAL(timeout()), &m_LoopSetPosition, SLOT(quit()));
    }

    qint32 ret = m_LoopSetPosition.exec();
    if (ret == 0) {
        Log(tr("(timeout expired: %1 ms)").arg(Timeout));
    }
    return (ret == 1);
}


/****************************************************************************/
/*!
 *  \brief   slot for position reached
 *
 *  This slot is called for acknowlegdement of ReqDriveSpeed and ReqDriveToPosition
 *
 *  \iparam InstanceID  ID of the FM (currently not needed)
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *  \iparam Position    absolute position in half-steps
 *  \iparam Speed       speed in half-steps per second (only in rotation mode)
 *
 */
/****************************************************************************/
void WrapperFmStepperMotor::OnSetPosition(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, qint32 Position, qint16 Speed)
{
    // exit from eventloop: 1 success, 0 timeout, -1 failure
    qint32 ret = 1;
    // TODO slaves negate values
    m_CurrentPosition = Position;
    m_CurrentSpeed = Speed;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    } else {
        m_CurrentPositionKnown = true;
    }

    if (m_LoopSetPosition.isRunning() || m_LoopWait.isRunning() || m_SetPositionActive) {
        // Handling of linear movement
        if (m_SetPositionActive) {
            m_SetPositionActive = false;
            if (m_TargetPosition != m_CurrentPosition) {
                Log(tr("ERROR: Actual value (%1) differs from target value (%2).")
                    .arg(m_CurrentPosition).arg(m_TargetPosition));
                ret = -1;
            } else {
                Log(tr("Done."));
            }
            if (m_LoopSetPosition.isRunning()) {
                m_LoopSetPosition.exit(ret);
            }
        } else {
            Log(tr("NOTICE: Unexpected action acknowledgement for linear position (p: %1 s: %2).").arg(m_CurrentPosition).arg(m_CurrentSpeed));
        }

        // finish wait loop if needed
        if (m_LoopWait.isRunning()) {
            if (!m_SetPositionActive) {
                m_LoopWait.exit(1);
            }
        }
    }

    if (m_LoopRotation.isRunning()) {
        // Handling of rotation
        if (m_TargetSpeed != m_CurrentSpeed) {
            Log(tr("ERROR: Actual value (%1) differs from target value (%2).")
                .arg(m_CurrentSpeed).arg(m_TargetSpeed));
            ret = -1;
        }
        if (m_LoopRotation.isRunning()) {
            m_LoopRotation.exit(ret);
        } else {
            Log(tr("NOTICE: Unexpected action acknowledgement for rotation position (p: %1 s: %2).").arg(m_CurrentPosition).arg(m_CurrentSpeed));
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Wait for all pending asynchronous actions
 *  of a class instance to complete.
 *
 *  For a detailed description of the Timeout parameter, please see \ref TimeoutIntro .
 *
 *  Examples:
 *  \skipline [Loader.Open/Close]
 *  \until    [Loader.Open/Close]
 *
 *  \iparam Timeout timeout in ms for finishing all pending actions
 *
 *  \return true, if all actions are finished (independent from success), false on timeout
 *
 */
/****************************************************************************/
bool WrapperFmStepperMotor::Wait(qint32 Timeout)
{
    if (!m_SetPositionActive) {
       return true;
    }

    if (Timeout == 0) {
        return true;
    }

    QTimer timer;
    if (Timeout > 0) {
        timer.setSingleShot(true);
        timer.setInterval(Timeout);
        timer.start();
        // quit via timer is the same as exit(0)
        (void)connect(&timer, SIGNAL(timeout()), &m_LoopWait, SLOT(quit()));
    }

    qint32 ret = m_LoopWait.exec();

    if (ret == 0) {
        Log(tr("(timeout expired: %1 ms)").arg(Timeout));
    }

    return (ret != 0);

}


/****************************************************************************/
/*!
 *  \brief  Script-API: Get the actual position
 *
 *  Examples:
 *  \skipline [MoveFromPosition]
 *  \until    [MoveFromPosition]
 *
 *  \return Position absolute position in half-steps
 *
 */
/****************************************************************************/
QString WrapperFmStepperMotor::GetPosition()
{
    bool ok = HandleErrorCode(m_pStepperMotor->ReqActPosition());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }
    qint32 ret = m_LoopGetPosition.exec();

    QString position = "error";
    if (ret == 1) {
        (void)position.setNum(m_CurrentPosition);
    }
    return position;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get the current limit switch code
 *
 *  Examples:
 *  \skipline [GetLimitSwitchCode]
 *  \until    [GetLimitSwitchCode]
 *
 *  \return Code Current limit switch code (0: none; 1: LS1 active; 2: LS2 active; 3: both active)
 *
 */
/****************************************************************************/
QString WrapperFmStepperMotor::GetLimitSwitchCode()
{
    bool ok = HandleErrorCode(m_pStepperMotor->ReqActPosition());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }
    qint32 ret = m_LoopGetPosition.exec();

    QString code = "error";
    if (ret == 1) {
        (void)code.setNum(m_CurrentLimitSwitchCode);
    }
    return code;
}

/****************************************************************************/
/*!
 *  \brief   slot for position reached
 *
 *  \iparam InstanceID  ID of the FM (currently not needed)
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *  \iparam Position    absolute position in half-steps
 *  \iparam PosCode     Actual limit switches position code
 *
 */
/****************************************************************************/
void WrapperFmStepperMotor::OnGetPosition(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, qint32 Position, qint8 PosCode)
{
    qint32 ret = 1;
    m_CurrentPosition = Position;
    m_CurrentLimitSwitchCode = PosCode;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    } else {
        m_CurrentPositionKnown = true;
    }

    if (m_LoopGetPosition.isRunning()) {
        m_LoopGetPosition.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for GetPosition (p: %1, LS code: %2).").arg(m_CurrentPosition).arg(PosCode));
    }
}
/****************************************************************************/
/*!
 *  \brief  Script-API: Get the actual operation time of motor
 *
 *  \return OperationTime absolute operation time in hours
 *
 */
/****************************************************************************/
QString WrapperFmStepperMotor::GetOperationTime()
{
    bool ok = HandleErrorCode(m_pStepperMotor->RequestOperationTimeData());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }
    qint32 ret = m_LoopGetLifeCycleData.exec();

    QString operationTime = "error";
    if (ret == 1) {
        operationTime = operationTime.setNum(m_CurrentOperationTime);
    }
    return operationTime;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get the actual revolution count of motor
 *
 *  \return OperationTime absolute revolution count
 *
 */
/****************************************************************************/
QString WrapperFmStepperMotor::GetRevolutionCount()
{
    bool ok = HandleErrorCode(m_pStepperMotor->RequestRevolutionCountData());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }
    qint32 ret = m_LoopGetLifeCycleData.exec();

    QString revolutionCount = "error";
    if (ret == 1) {
        revolutionCount = revolutionCount.setNum(m_CurrentRevolutionCount);
    }
    return revolutionCount;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get the actual direction change count of motor
 *
 *  \return OperationTime absolute direction change count
 *
 */
/****************************************************************************/
QString WrapperFmStepperMotor::GetDirChangeCount()
{
    bool ok = HandleErrorCode(m_pStepperMotor->RequestDirChangeCountData());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }
    qint32 ret = m_LoopGetLifeCycleData.exec();

    QString dirChangeCount = "error";
    if (ret == 1) {
        dirChangeCount = dirChangeCount.setNum(m_CurrentDirChangeCount);
    }
    return dirChangeCount;
}
/****************************************************************************/
/*!
 *  \brief  Slot for operation time
 *
 *  \iparam InstanceID = ID of the FM (currently not needed)
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 *  \iparam RevolutionCount = Number of revisions
 *  \iparam DirChangeCount = Number of trips (cw-to-ccw and ccw-to-cw transitions)
 *  \iparam OperationTime = Total on-times (minutes)
 */
/****************************************************************************/
void WrapperFmStepperMotor::OnGetLifeCycleData(quint32 /*InstanceID*/, ReturnCode_t ReturnCode,quint32 RevolutionCount,
                                               quint32 DirChangeCount, quint32 OperationTime)
{
    qint32 ret = 1;
    m_CurrentRevolutionCount = RevolutionCount;
    m_CurrentOperationTime = OperationTime;
    m_CurrentDirChangeCount = DirChangeCount;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }

    if (m_LoopGetLifeCycleData.isRunning()) {
        m_LoopGetLifeCycleData.exit(ret);
    } else {
        Log( tr("NOTICE: Unexpected action acknowledgement for OnGetLfeCycleData (revolution count: %1, operation time: %2).")
                    .arg(RevolutionCount).arg(OperationTime));
    }
}
/****************************************************************************/
/*!
 *  \brief  Script-API: Set profile for newly initiated movements.
 *
 *   The default profile index is 0.
 *   The firmware supports up to 5 profile entries (index 0 to 4).
 *   \todo no DeviceControl error on unknown profile index
 *
 *  \iparam Profile profile index as configured for the stepper motor
 *
 *  Examples:
 *  \skipline [SetProfile]
 *  \until    [SetProfile]
 *
 *  \return true, if the action succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetProfile(qint8 Profile)
{
    if (Profile < 0 || Profile > 4) {
        return false;
    } else {
        m_Profile = Profile;
        return true;
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get currently active profile.
 *
 *  \return currently active profile index
 *
 */
/****************************************************************************/
qint8 WrapperFmStepperMotor::GetProfile()
{
    return m_Profile;
}


/****************************************************************************/
/*!
 *  \brief  Script-API: Get the actual speed
 *
 *  Examples:
 *  \skipline [GetSpeed]
 *  \until    [GetSpeed]
 *
 *  \return Speed absolute speed
 *
 */
/****************************************************************************/
qint16 WrapperFmStepperMotor::GetSpeed()
{
    Log(tr("GetSpeed"));
    bool ok = HandleErrorCode(m_pStepperMotor->ReqActSpeed());
    if (!ok) {
        return -1;
    }
    qint32 ret = m_LoopGetSpeed.exec();
    if (ret != 1) {
        return -1;
    }
    return m_CurrentSpeed;
}

/****************************************************************************/
/*!
 *  \brief  slot to read speed
 *
 *  This slot is connected to the signal, DeviceLoaderRFID and
 *  DeviceUnloaderRFID
 *
 *  \iparam InstanceID  ID of the FM (currently not needed)
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *  \iparam Speed       absolute speed
 *
 */
/****************************************************************************/
void WrapperFmStepperMotor::OnGetSpeed(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, qint16 Speed)
{
    qint32 ret = 1;
     m_CurrentSpeed = Speed;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopGetSpeed.isRunning()) {
        m_LoopGetSpeed.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for GetSpeed."));
    }
}


/****************************************************************************/
/*!
 *  \brief  Script-API: Enables module.
 *
 *  If the module is disabled, this call will initialize the module.
 *  A reference run enables the module automatically.
 *  A reconfiguration disables and enables the module automatically.
 *
 *  \return true, if the module could be enabled, else false
 *
 */
/****************************************************************************/
bool WrapperFmStepperMotor::Enable()
{
    Log(tr("Enable Stepper Motor"));
    return SetState(true);
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Disables module.
 *
 *  If the module is enabled, this call will deactivate the steppermotor.
 *  A reconfiguration disables and enables the module automatically.
 *
 *  \return true, if the module could be disabled, else false
 *
 */
/****************************************************************************/
bool WrapperFmStepperMotor::Disable()
{
    Log(tr("Disable Stepper Motor"));
    return SetState(false);
}

/****************************************************************************/
/*!
 *  \brief  Helper function, sets a given state
 *
 *  \iparam flag: true enables; false disables module
 *
 *  \return true, if the action succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetState(bool flag)
{
    bool ok = HandleErrorCode(m_pStepperMotor->SetMotorState(flag));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetState.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief   slot for SetState
 *
 *  \iparam InstanceID  ID of the FM (currently not needed)
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *
 */
/****************************************************************************/
void WrapperFmStepperMotor::OnSetState(quint32 /*InstanceID*/, ReturnCode_t ReturnCode)
{
    // exit from eventloop: 1 success, 0 timeout, -1 failure
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopSetState.isRunning()) {
        m_LoopSetState.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for SetState."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request a data reset
 *
 *      This method resets the operation time of stepper motor
 *
 *
 *  Examples:
 *  \skipline [DigitalOut.ReqDataReset]
 *  \until    [DigitalOut.ReqDataReset]
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::ReqDataReset()
{
    return HandleErrorCode(m_pStepperMotor->ReqDataReset());
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
void WrapperFmStepperMotor::Reset()
{
        RestoreLastValidParameterSet();

        m_SetPositionActive = false;
        m_RotationOn = false;
        m_CurrentPositionKnown = false;

        m_CurrentPosition = 0;
        m_TargetPosition  = 0;
        m_CurrentLimitSwitchCode = 0;
        m_CurrentSpeed = 0;
        m_TargetSpeed  = 0;
        // m_Speed  = 400;
        m_Speed  = m_Config.listMotionProfiles[0].sSpeedMax;
        m_Profile = 0;

}

/****************************************************************************/
/*!
 *  \brief  Script-API: Apply new parameter set
 *
 *  Verifies local parameter set and activates it, if it is valid.
 *  It also implies disabling and re-enabling the module.
 *
 *  Beware: This requires another DoReferenceRun() unless in .
 *
 *  Quick&Dirty: Profile and Limit switch configuration are directly changed
 *  => get rid of pointers or write assign op/copy constructor or even better:
 *  \todo use new data container
 *
 *
 *  Examples:
 *  \skipline [ApplyNewParameterSet]
 *  \until    [ApplyNewParameterSet]
 *
 *  \return true if activation of parameter set succeeded, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::ApplyNewParameterSet()
{
#ifndef PRE_ALFA_TEST
    Log(tr("ApplyNewParameterSet"));
#endif

    // disable function module before reconfiguration
    if (!SetState(false)) {
        return false;
    }

    bool ok = HandleErrorCode(m_pStepperMotor->DoReconfiguration(&m_Config));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetConfiguration.exec();

#ifdef PRE_ALFA_TEST
    /*TODO: This is a work around. Delay 1 second before set state to ON will avoid can error, whose root cause is unknown now. */
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(ApplyNewParameterSetTimerCB()));
    timer->start(1000);
    if(m_LoopApplyNewParameter.isRunning())
    {
        m_LoopApplyNewParameter.exit();
    }
    else
    {
        m_LoopApplyNewParameter.exec();
    }
    delete timer;
#endif

    if (!SetState(true)) {
        return false;
    }

    return (ret == 1);
}
#ifdef PRE_ALFA_TEST
void WrapperFmStepperMotor::ApplyNewParameterSetTimerCB()
{
    if(m_LoopApplyNewParameter.isRunning())
    {
        m_LoopApplyNewParameter.exit();
    }
}
#endif

/****************************************************************************/
/*!
 *  \brief   slot for SetConfiguration
 *
 *  \iparam InstanceID  ID of the FM (currently not needed)
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *
 */
/****************************************************************************/
void WrapperFmStepperMotor::OnSetConfiguration(quint32 /*InstanceID*/, ReturnCode_t ReturnCode)
{
    // exit from eventloop: 1 success, 0 timeout, -1 failure
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopSetConfiguration.isRunning()) {
        m_LoopSetConfiguration.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for SetConfiguration."));
    }
}


/****************************************************************************/
/*!
 *  \brief  Script-API: Restore last valid parameter set
 *
 *  After a failed ApplyNewParameters() call the local parameter set can be
 *  restored to the last valid and still active configuration parameters.
 */
/****************************************************************************/
void WrapperFmStepperMotor::RestoreLastValidParameterSet()
{
    m_Config = *m_pStepperMotor->GetConfiguration();
}










/****************************************************************************/
/*                                                                          */
/* Auto-generated configuration getters/setters                             */
/*                                                                          */
/* ./steppermotor_config_create.sh steppermotor_config_data.txt 1 | xclip   */
/*                                                                          */
/****************************************************************************/



/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of rotationType; Property: rotation_type
 *  in local parameter set.
 *
 *  rotation type: 1 linear 2 rotation
 *
 *  \todo verify parameter set.
 *
 *  \iparam rotationType   configuration element ConfigRotationType
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigRotationType(qint32 rotationType)
{
    m_Config.rotationType = (DeviceControl::CANFctModuleStepperMotor::RotationType_t) rotationType;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of rotationType; Property: rotation_type
 *  in local parameter set.
 *
 *  rotation type: 1 linear 2 rotation
 *
 *  \return value of rotationType
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::GetConfigRotationType()
{
    return m_Config.rotationType;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sResolution; Property: rotation_steps_revolution
 *  in local parameter set.
 *
 *  motor resolution (half-steps per revolution, 1.8 degree per step => 400 half-steps per revolution)
 *
 *  \todo verify parameter set.
 *
 *  \iparam sResolution   configuration element ConfigResolution
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigResolution(quint16 sResolution)
{
    m_Config.sResolution =  sResolution;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sResolution; Property: rotation_steps_revolution
 *  in local parameter set.
 *
 *  motor resolution (half-steps per revolution, 1.8 degree per step => 400 half-steps per revolution)
 *
 *  \return value of sResolution
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigResolution()
{
    return m_Config.sResolution;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sRevHStepCount; Property: rotation_rev_hstep_count
 *  in local parameter set.
 *
 *  count of half-steps for one revolution (for internal degree calculations involving transmission)
 *
 *  \todo verify parameter set.
 *
 *  \iparam sRevHStepCount   configuration element ConfigRevHStepCount
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigRevHStepCount(quint16 sRevHStepCount)
{
    m_Config.sResetPosition =  sRevHStepCount;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sRevHStepCount; Property: rotation_rev_hstep_count
 *  in local parameter set.
 *
 *  count of half-steps for one revolution (for internal degree calculations involving transmission)
 *
 *  \return value of sRevHStepCount
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigRevHStepCount()
{
    return m_Config.sResetPosition;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bDirection; Property: rotation_direction
 *  in local parameter set.
 *
 *  rotation direction: 1 cw 2 ccw
 *
 *  \todo verify parameter set.
 *
 *  \iparam bDirection   configuration element ConfigDirection
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigDirection(qint32 bDirection)
{
    m_Config.bDirection = (DeviceControl::CANFctModuleStepperMotor::RotationDir_t) bDirection;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bDirection; Property: rotation_direction
 *  in local parameter set.
 *
 *  rotation direction: 1 cw 2 ccw
 *
 *  \return value of bDirection
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::GetConfigDirection()
{
    return m_Config.bDirection;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of lMinPosition; Property: rotation_position_min
 *  in local parameter set.
 *
 *  minimum motor position [half-steps]
 *
 *  \todo verify parameter set.
 *
 *  \iparam lMinPosition   configuration element ConfigMinPosition
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigMinPosition(qint32 lMinPosition)
{
    m_Config.lMinPosition =  lMinPosition;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of lMinPosition; Property: rotation_position_min
 *  in local parameter set.
 *
 *  minimum motor position [half-steps]
 *
 *  \return value of lMinPosition
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::GetConfigMinPosition()
{
    return m_Config.lMinPosition;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of lMaxPosition; Property: rotation_position_max
 *  in local parameter set.
 *
 *  maximum motor position [half-steps]
 *
 *  \todo verify parameter set.
 *
 *  \iparam lMaxPosition   configuration element ConfigMaxPosition
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigMaxPosition(qint32 lMaxPosition)
{
    m_Config.lMaxPosition =  lMaxPosition;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of lMaxPosition; Property: rotation_position_max
 *  in local parameter set.
 *
 *  maximum motor position [half-steps]
 *
 *  \return value of lMaxPosition
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::GetConfigMaxPosition()
{
    return m_Config.lMaxPosition;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sMinSpeed; Property: rotation_speed_min
 *  in local parameter set.
 *
 *  minimal speed [half-steps/sec]
 *
 *  \todo verify parameter set.
 *
 *  \iparam sMinSpeed   configuration element ConfigMinSpeed
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigMinSpeed(qint16 sMinSpeed)
{
    m_Config.sMinSpeed =  sMinSpeed;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sMinSpeed; Property: rotation_speed_min
 *  in local parameter set.
 *
 *  minimal speed [half-steps/sec]
 *
 *  \return value of sMinSpeed
 */
/****************************************************************************/
qint16 WrapperFmStepperMotor::GetConfigMinSpeed()
{
    return m_Config.sMinSpeed;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sMaxSpeed; Property: rotation_speed_max
 *  in local parameter set.
 *
 *  maximal speed [half-steps/sec]
 *
 *  \todo verify parameter set.
 *
 *  \iparam sMaxSpeed   configuration element ConfigMaxSpeed
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigMaxSpeed(qint16 sMaxSpeed)
{
    m_Config.sMaxSpeed =  sMaxSpeed;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sMaxSpeed; Property: rotation_speed_max
 *  in local parameter set.
 *
 *  maximal speed [half-steps/sec]
 *
 *  \return value of sMaxSpeed
 */
/****************************************************************************/
qint16 WrapperFmStepperMotor::GetConfigMaxSpeed()
{
    return m_Config.sMaxSpeed;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of runCurrentScale; Property: rotation_current_run
 *  in local parameter set.
 *
 *  motor current scale used when motor is moving (range 0-32); CS = (Irms*Rsense*1.41*32/Vfs)-1 = (Irms*0.05*1.41*32/0.165)-1; e.g. for 2A motor: 12.7; recommended: 80% = 10.1 ^= 10
 *
 *  \todo verify parameter set.
 *
 *  \iparam runCurrentScale   configuration element ConfigCurrentRun
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigCurrentRun(quint8 runCurrentScale)
{
    m_Config.runCurrentScale =  runCurrentScale;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of runCurrentScale; Property: rotation_current_run
 *  in local parameter set.
 *
 *  motor current scale used when motor is moving (range 0-32); CS = (Irms*Rsense*1.41*32/Vfs)-1 = (Irms*0.05*1.41*32/0.165)-1; e.g. for 2A motor: 12.7; recommended: 80% = 10.1 ^= 10
 *
 *  \return value of runCurrentScale
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigCurrentRun()
{
    return m_Config.runCurrentScale;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of stopCurrentScale; Property: rotation_current_stop
 *  in local parameter set.
 *
 *  motor current scale used when motor is standing still (range 0-32); a sensible "holding current" needs to be determined for the given task; use a value of 3 as a starting point
 *
 *  \todo verify parameter set.
 *
 *  \iparam stopCurrentScale   configuration element ConfigCurrentStop
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigCurrentStop(quint8 stopCurrentScale)
{
    m_Config.stopCurrentScale =  stopCurrentScale;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of stopCurrentScale; Property: rotation_current_stop
 *  in local parameter set.
 *
 *  motor current scale used when motor is standing still (range 0-32); a sensible "holding current" needs to be determined for the given task; use a value of 3 as a starting point
 *
 *  \return value of stopCurrentScale
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigCurrentStop()
{
    return m_Config.stopCurrentScale;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of stopCurrentDelay; Property: rotation_current_delay
 *  in local parameter set.
 *
 *  delay in ms from motor standing still to applying stop current; 0 deactivates "stop current" feature, i.e. the "run current" is permanently active
 *
 *  \todo verify parameter set.
 *
 *  \iparam stopCurrentDelay   configuration element ConfigCurrentDelay
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigCurrentDelay(quint16 stopCurrentDelay)
{
    m_Config.stopCurrentDelay =  stopCurrentDelay;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of stopCurrentDelay; Property: rotation_current_delay
 *  in local parameter set.
 *
 *  delay in ms from motor standing still to applying stop current; 0 deactivates "stop current" feature, i.e. the "run current" is permanently active
 *
 *  \return value of stopCurrentDelay
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigCurrentDelay()
{
    return m_Config.stopCurrentDelay;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bEncoderType; Property: encoder_type
 *  in local parameter set.
 *
 *  encoder type
 *
 *  \todo verify parameter set.
 *
 *  \iparam bEncoderType   configuration element ConfigEncoderType
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigEncoderType(quint8 bEncoderType)
{
    m_Config.bEncoderType =  bEncoderType;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bEncoderType; Property: encoder_type
 *  in local parameter set.
 *
 *  encoder type
 *
 *  \return value of bEncoderType
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigEncoderType()
{
    return m_Config.bEncoderType;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sEncoderResolution; Property: encoder_resolution
 *  in local parameter set.
 *
 *  encoder resolution
 *
 *  \todo verify parameter set.
 *
 *  \iparam sEncoderResolution   configuration element ConfigEncoderResolution
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigEncoderResolution(quint16 sEncoderResolution)
{
    m_Config.sEncoderResolution =  sEncoderResolution;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sEncoderResolution; Property: encoder_resolution
 *  in local parameter set.
 *
 *  encoder resolution
 *
 *  \return value of sEncoderResolution
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigEncoderResolution()
{
    return m_Config.sEncoderResolution;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bEncoderDir; Property: encoder_rotation
 *  in local parameter set.
 *
 *  encoder direction: 1 cw 2 ccw
 *
 *  \todo verify parameter set.
 *
 *  \iparam bEncoderDir   configuration element ConfigEncoderDir
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigEncoderDir(qint32 bEncoderDir)
{
    m_Config.bEncoderDir = (DeviceControl::CANFctModuleStepperMotor::RotationDir_t) bEncoderDir;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bEncoderDir; Property: encoder_rotation
 *  in local parameter set.
 *
 *  encoder direction: 1 cw 2 ccw
 *
 *  \return value of bEncoderDir
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::GetConfigEncoderDir()
{
    return m_Config.bEncoderDir;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of refRunRefPos; Property: reference_run_position
 *  in local parameter set.
 *
 *  the limit switch position code used as reference position; 1: LS1, 2: LS2, 3: LS1+LS2, 0: no LS
 *
 *  \todo verify parameter set.
 *
 *  \iparam refRunRefPos   configuration element ConfigRefRunPosition
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigRefRunPosition(qint8 refRunRefPos)
{
    m_Config.refRunRefPos =  refRunRefPos;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of refRunRefPos; Property: reference_run_position
 *  in local parameter set.
 *
 *  the limit switch position code used as reference position; 1: LS1, 2: LS2, 3: LS1+LS2, 0: no LS
 *
 *  \return value of refRunRefPos
 */
/****************************************************************************/
qint8 WrapperFmStepperMotor::GetConfigRefRunPosition()
{
    return m_Config.refRunRefPos;
}

#ifdef PRE_ALFA_TEST

/*
bool WrapperFmStepperMotor::SetConfigRefRunRefPosSkip(quint8 refRunRefPosSkip)
{
    m_Config.refRunRefPosSkip =  refRunRefPosSkip;
    return true;
}

quint8 WrapperFmStepperMotor::GetConfigRefRunRefPosSkip()
{
    return m_Config.refRunRefPosSkip;
}
*/

bool WrapperFmStepperMotor::SetConfigPos1HitSkip(quint8 hitSkip)
{
    m_Config.PosCode1.hitSkip =  hitSkip;
    return true;
}

quint8 WrapperFmStepperMotor::GetConfigPos1HitSkip()
{
    return m_Config.PosCode1.hitSkip;
}

bool WrapperFmStepperMotor::SetConfigPos2HitSkip(quint8 hitSkip)
{
    m_Config.PosCode2.hitSkip =  hitSkip;
    return true;
}

quint8 WrapperFmStepperMotor::GetConfigPos2HitSkip()
{
    return m_Config.PosCode2.hitSkip;
}

bool WrapperFmStepperMotor::SetConfigPos3HitSkip(quint8 hitSkip)
{
    m_Config.PosCode3.hitSkip =  hitSkip;
    return true;
}

quint8 WrapperFmStepperMotor::GetConfigPos3HitSkip()
{
    return m_Config.PosCode3.hitSkip;
}



#endif

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of lRefRunMaxDistance; Property: reference_run_max_distance
 *  in local parameter set.
 *
 *  maximum number of half-steps during reference run
 *
 *  \todo verify parameter set.
 *
 *  \iparam lRefRunMaxDistance   configuration element ConfigRefRunMaxDistance
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigRefRunMaxDistance(qint32 lRefRunMaxDistance)
{
    m_Config.lRefRunMaxDistance =  lRefRunMaxDistance;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of lRefRunMaxDistance; Property: reference_run_max_distance
 *  in local parameter set.
 *
 *  maximum number of half-steps during reference run
 *
 *  \return value of lRefRunMaxDistance
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::GetConfigRefRunMaxDistance()
{
    return m_Config.lRefRunMaxDistance;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sRefRunTimeout; Property: reference_run_timeout
 *  in local parameter set.
 *
 *  maximum duration in ms to perform each movement during reference run
 *
 *  \todo verify parameter set.
 *
 *  \iparam sRefRunTimeout   configuration element ConfigRefRunTimeout
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
#ifndef PRE_ALFA_TEST
bool WrapperFmStepperMotor::SetConfigRefRunTimeout(qint16 sRefRunTimeout)
#else
bool WrapperFmStepperMotor::SetConfigRefRunTimeout(quint16 sRefRunTimeout)
#endif
{
    m_Config.sRefRunTimeout =  sRefRunTimeout;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sRefRunTimeout; Property: reference_run_timeout
 *  in local parameter set.
 *
 *  maximum duration in ms to perform each movement during reference run
 *
 *  \return value of sRefRunTimeout
 */
/****************************************************************************/
#ifndef PRE_ALFA_TEST
qint16 WrapperFmStepperMotor::GetConfigRefRunTimeout()
#else
quint16 WrapperFmStepperMotor::GetConfigRefRunTimeout()
#endif
{
    return m_Config.sRefRunTimeout;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of lRefRunReverseDistance; Property: reference_run_reverse_distance
 *  in local parameter set.
 *
 *  distance for reverse move between high and low speed cycle
 *
 *  \todo verify parameter set.
 *
 *  \iparam lRefRunReverseDistance   configuration element ConfigRefRunReverseDistance
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigRefRunReverseDistance(qint32 lRefRunReverseDistance)
{
    m_Config.lRefRunReverseDistance =  lRefRunReverseDistance;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of lRefRunReverseDistance; Property: reference_run_reverse_distance
 *  in local parameter set.
 *
 *  distance for reverse move between high and low speed cycle
 *
 *  \return value of lRefRunReverseDistance
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::GetConfigRefRunReverseDistance()
{
    return m_Config.lRefRunReverseDistance;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of lRefPosOffset; Property: reference_run_refpos_offset
 *  in local parameter set.
 *
 *  offset between the reference position and internal position system in half-steps; after reference run the current position is set to "offset value".
 *
 *  \todo verify parameter set.
 *
 *  \iparam lRefPosOffset   configuration element ConfigRefPosOffset
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigRefPosOffset(qint32 lRefPosOffset)
{
    m_Config.lRefPosOffset =  lRefPosOffset;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of lRefPosOffset; Property: reference_run_refpos_offset
 *  in local parameter set.
 *
 *  offset between the reference position and internal position system in half-steps; after reference run the current position is set to "offset value".
 *
 *  \return value of lRefPosOffset
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::GetConfigRefPosOffset()
{
    return m_Config.lRefPosOffset;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sRefRunSlowSpeed; Property: reference_run_slow_speed
 *  in local parameter set.
 *
 *  second approach of limit switch is done with low speed for more accurate position [half-steps/sec]
 *
 *  \todo verify parameter set.
 *
 *  \iparam sRefRunSlowSpeed   configuration element ConfigRefRunSlowSpeed
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigRefRunSlowSpeed(qint16 sRefRunSlowSpeed)
{
    m_Config.sRefRunSlowSpeed =  sRefRunSlowSpeed;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sRefRunSlowSpeed; Property: reference_run_slow_speed
 *  in local parameter set.
 *
 *  second approach of limit switch is done with low speed for more accurate position [half-steps/sec]
 *
 *  \return value of sRefRunSlowSpeed
 */
/****************************************************************************/
qint16 WrapperFmStepperMotor::GetConfigRefRunSlowSpeed()
{
    return m_Config.sRefRunSlowSpeed;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sRefRunHighSpeed; Property: reference_run_high_speed
 *  in local parameter set.
 *
 *  first approach of limit switch is done with high speed [half-steps/sec]
 *
 *  \todo verify parameter set.
 *
 *  \iparam sRefRunHighSpeed   configuration element ConfigRefRunHighSpeed
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigRefRunHighSpeed(qint16 sRefRunHighSpeed)
{
    m_Config.sRefRunHighSpeed =  sRefRunHighSpeed;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sRefRunHighSpeed; Property: reference_run_high_speed
 *  in local parameter set.
 *
 *  first approach of limit switch is done with high speed [half-steps/sec]
 *
 *  \return value of sRefRunHighSpeed
 */
/****************************************************************************/
qint16 WrapperFmStepperMotor::GetConfigRefRunHighSpeed()
{
    return m_Config.sRefRunHighSpeed;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sStepLossWarnLimit; Property: supervision_steploss_warn_limit
 *  in local parameter set.
 *
 *  warning limit for step loss errors
 *
 *  \todo verify parameter set.
 *
 *  \iparam sStepLossWarnLimit   configuration element ConfigStepLossWarnLimit
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigStepLossWarnLimit(qint16 sStepLossWarnLimit)
{
    m_Config.sStepLossWarnLimit =  sStepLossWarnLimit;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sStepLossWarnLimit; Property: supervision_steploss_warn_limit
 *  in local parameter set.
 *
 *  warning limit for step loss errors
 *
 *  \return value of sStepLossWarnLimit
 */
/****************************************************************************/
qint16 WrapperFmStepperMotor::GetConfigStepLossWarnLimit()
{
    return m_Config.sStepLossWarnLimit;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sStepLossErrorLimit; Property: supervision_steploss_error_limit
 *  in local parameter set.
 *
 *  error limit for step loss errors
 *
 *  \todo verify parameter set.
 *
 *  \iparam sStepLossErrorLimit   configuration element ConfigStepLossErrorLimit
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigStepLossErrorLimit(qint16 sStepLossErrorLimit)
{
    m_Config.sStepLossErrorLimit =  sStepLossErrorLimit;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sStepLossErrorLimit; Property: supervision_steploss_error_limit
 *  in local parameter set.
 *
 *  error limit for step loss errors
 *
 *  \return value of sStepLossErrorLimit
 */
/****************************************************************************/
qint16 WrapperFmStepperMotor::GetConfigStepLossErrorLimit()
{
    return m_Config.sStepLossErrorLimit;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sCurrentLimit; Property: supervision_current_limit
 *  in local parameter set.
 *
 *  limit for current supervision
 *
 *  \todo verify parameter set.
 *
 *  \iparam sCurrentLimit   configuration element ConfigCurrentLimit
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigCurrentLimit(qint16 sCurrentLimit)
{
    m_Config.sCurrentLimit =  sCurrentLimit;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sCurrentLimit; Property: supervision_current_limit
 *  in local parameter set.
 *
 *  limit for current supervision
 *
 *  \return value of sCurrentLimit
 */
/****************************************************************************/
qint16 WrapperFmStepperMotor::GetConfigCurrentLimit()
{
    return m_Config.sCurrentLimit;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of driverType; Property: driver_type
 *  in local parameter set.
 *
 *  0 default; 1 DRIVER_TMC26X: trinamic tmc26x configuration register
 *
 *  \todo verify parameter set.
 *
 *  \iparam driverType   configuration element ConfigDriverType
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigDriverType(qint32 driverType)
{
    m_Config.driverType = (DeviceControl::CANFctModuleStepperMotor::DriverType_t) driverType;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of driverType; Property: driver_type
 *  in local parameter set.
 *
 *  0 default; 1 DRIVER_TMC26X: trinamic tmc26x configuration register
 *
 *  \return value of driverType
 */
/****************************************************************************/
qint32 WrapperFmStepperMotor::GetConfigDriverType()
{
    return m_Config.driverType;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of drvConf; Property: driver_reg_drvConf
 *  in local parameter set.
 *
 *  e.g. "901B4"
 *
 *  \todo verify parameter set.
 *
 *  \iparam drvConf   configuration element ConfigDrvConf
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigDrvConf(quint32 drvConf)
{
    m_Config.tmc26x.drvConf =  drvConf;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of drvConf; Property: driver_reg_drvConf
 *  in local parameter set.
 *
 *  e.g. "901B4"
 *
 *  \return value of drvConf
 */
/****************************************************************************/
quint32 WrapperFmStepperMotor::GetConfigDrvConf()
{
    return m_Config.tmc26x.drvConf;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sgcsConf; Property: driver_reg_sgcsConf
 *  in local parameter set.
 *
 *  e.g. "D400A"; last character changes motor current; CS = (Irms*Rsense*1.41*32/Vfs)-1 = (Irms*0.05*1.41*32/0.165)-1; e.g. for 2A motor: 12.7; recommended: 80% = 10.1 ^= A
 *
 *  \todo verify parameter set.
 *
 *  \iparam sgcsConf   configuration element ConfigSgcsConf
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigSgcsConf(quint32 sgcsConf)
{
    m_Config.tmc26x.sgcsConf =  sgcsConf;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sgcsConf; Property: driver_reg_sgcsConf
 *  in local parameter set.
 *
 *  e.g. "D400A"; last character changes motor current; CS = (Irms*Rsense*1.41*32/Vfs)-1 = (Irms*0.05*1.41*32/0.165)-1; e.g. for 2A motor: 12.7; recommended: 80% = 10.1 ^= A
 *
 *  \return value of sgcsConf
 */
/****************************************************************************/
quint32 WrapperFmStepperMotor::GetConfigSgcsConf()
{
    return m_Config.tmc26x.sgcsConf;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of smartEn; Property: driver_reg_smartEn
 *  in local parameter set.
 *
 *  e.g. "E02D0"
 *
 *  \todo verify parameter set.
 *
 *  \iparam smartEn   configuration element ConfigSmartEn
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigSmartEn(quint32 smartEn)
{
    m_Config.tmc26x.smartEn =  smartEn;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of smartEn; Property: driver_reg_smartEn
 *  in local parameter set.
 *
 *  e.g. "E02D0"
 *
 *  \return value of smartEn
 */
/****************************************************************************/
quint32 WrapperFmStepperMotor::GetConfigSmartEn()
{
    return m_Config.tmc26x.smartEn;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of chopConf; Property: driver_reg_chopConf
 *  in local parameter set.
 *
 *  e.g. "A8202"
 *
 *  \todo verify parameter set.
 *
 *  \iparam chopConf   configuration element ConfigChopConf
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigChopConf(quint32 chopConf)
{
    m_Config.tmc26x.chopConf =  chopConf;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of chopConf; Property: driver_reg_chopConf
 *  in local parameter set.
 *
 *  e.g. "A8202"
 *
 *  \return value of chopConf
 */
/****************************************************************************/
quint32 WrapperFmStepperMotor::GetConfigChopConf()
{
    return m_Config.tmc26x.chopConf;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sSpeedMin; Property: motion_profile0_speed_min
 *  in local parameter set.
 *
 *  Profile 0: minimal speed [half steps/sec], due to a maximal interrupt interval of 5ms, there is a lower limit for the minimal speed depending on the number of micro steps; 50 half steps/sec can be managed with down to 8 micro steps per full step (double this limit by dividing he micro steps by 2)
 *
 *  \todo verify parameter set.
 *
 *  \iparam sSpeedMin   configuration element ConfigP0SpeedMin
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigP0SpeedMin(quint16 sSpeedMin)
{
    m_Config.listMotionProfiles[0].sSpeedMin = sSpeedMin;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sSpeedMin; Property: motion_profile0_speed_min
 *  in local parameter set.
 *
 *  Profile 0: minimal speed [half steps/sec], due to a maximal interrupt interval of 5ms, there is a lower limit for the minimal speed depending on the number of micro steps; 50 half steps/sec can be managed with down to 8 micro steps per full step (double this limit by dividing he micro steps by 2)
 *
 *  \return value of sSpeedMin
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigP0SpeedMin()
{
    return m_Config.listMotionProfiles[0].sSpeedMin;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sSpeedMax; Property: motion_profile0_speed_max
 *  in local parameter set.
 *
 *  Profile 0: target speed [half steps/sec], if this speed is reached depends on profile and distance; the non-optimized control algorithm manages up to 1500 half steps/sec with 32 micro steps (double this limit by dividing the micro steps by 2)
 *
 *  \todo verify parameter set.
 *
 *  \iparam sSpeedMax   configuration element ConfigP0SpeedMax
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigP0SpeedMax(quint16 sSpeedMax)
{
    m_Config.listMotionProfiles[0].sSpeedMax = sSpeedMax;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sSpeedMax; Property: motion_profile0_speed_max
 *  in local parameter set.
 *
 *  Profile 0: target speed [half steps/sec], if this speed is reached depends on profile and distance; the non-optimized control algorithm manages up to 1500 half steps/sec with 32 micro steps (double this limit by dividing the micro steps by 2)
 *
 *  \return value of sSpeedMax
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigP0SpeedMax()
{
    return m_Config.listMotionProfiles[0].sSpeedMax;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sAcc; Property: motion_profile0_acc
 *  in local parameter set.
 *
 *  Profile 0: max acceleration [half steps/sec^2]
 *
 *  \todo verify parameter set.
 *
 *  \iparam sAcc   configuration element ConfigP0Acc
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigP0Acc(quint16 sAcc)
{
    m_Config.listMotionProfiles[0].sAcc = sAcc;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sAcc; Property: motion_profile0_acc
 *  in local parameter set.
 *
 *  Profile 0: max acceleration [half steps/sec^2]
 *
 *  \return value of sAcc
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigP0Acc()
{
    return m_Config.listMotionProfiles[0].sAcc;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sDec; Property: motion_profile0_dec
 *  in local parameter set.
 *
 *  Profile 0: max deceleration [half steps/sec^2]
 *
 *  \todo verify parameter set.
 *
 *  \iparam sDec   configuration element ConfigP0Dec
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigP0Dec(quint16 sDec)
{
    m_Config.listMotionProfiles[0].sDec = sDec;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sDec; Property: motion_profile0_dec
 *  in local parameter set.
 *
 *  Profile 0: max deceleration [half steps/sec^2]
 *
 *  \return value of sDec
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigP0Dec()
{
    return m_Config.listMotionProfiles[0].sDec;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sAccTime; Property: motion_profile0_acc_time
 *  in local parameter set.
 *
 *  Profile 0: duration of acceleration ramp (jerk) [ms];
 *
 *  \todo verify parameter set.
 *
 *  \iparam sAccTime   configuration element ConfigP0AccTime
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigP0AccTime(quint16 sAccTime)
{
    m_Config.listMotionProfiles[0].sAccTime = sAccTime;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sAccTime; Property: motion_profile0_acc_time
 *  in local parameter set.
 *
 *  Profile 0: duration of acceleration ramp (jerk) [ms];
 *
 *  \return value of sAccTime
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigP0AccTime()
{
    return m_Config.listMotionProfiles[0].sAccTime;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of sDecTime; Property: motion_profile0_dec_time
 *  in local parameter set.
 *
 *  Profile 0: duration of deceleration ramp (jerk) [ms]
 *
 *  \todo verify parameter set.
 *
 *  \iparam sDecTime   configuration element ConfigP0DecTime
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigP0DecTime(quint16 sDecTime)
{
    m_Config.listMotionProfiles[0].sDecTime = sDecTime;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of sDecTime; Property: motion_profile0_dec_time
 *  in local parameter set.
 *
 *  Profile 0: duration of deceleration ramp (jerk) [ms]
 *
 *  \return value of sDecTime
 */
/****************************************************************************/
quint16 WrapperFmStepperMotor::GetConfigP0DecTime()
{
    return m_Config.listMotionProfiles[0].sDecTime;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bMicroSteps; Property: motion_profile0_micro_steps
 *  in local parameter set.
 *
 *  Profile 0: full-step is divided by the given number of micro steps (64, 32, 16, 8, 4, 2); a high value offers a tighter control loop, but might cause an interrupt storm at high speeds (motor hangs); with a value of 8 the motor is already quite noisy
 *
 *  \todo verify parameter set.
 *
 *  \iparam bMicroSteps   configuration element ConfigP0MicroSteps
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigP0MicroSteps(quint8 bMicroSteps)
{
    m_Config.listMotionProfiles[0].bMicroSteps = bMicroSteps;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bMicroSteps; Property: motion_profile0_micro_steps
 *  in local parameter set.
 *
 *  Profile 0: full-step is divided by the given number of micro steps (64, 32, 16, 8, 4, 2); a high value offers a tighter control loop, but might cause an interrupt storm at high speeds (motor hangs); with a value of 8 the motor is already quite noisy
 *
 *  \return value of bMicroSteps
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigP0MicroSteps()
{
    return m_Config.listMotionProfiles[0].bMicroSteps;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bRampType; Property: motion_profile_ramp_type
 *  in local parameter set.
 *
 *  Profile 0: ramp type (s-curve)
 *
 *  \todo verify parameter set.
 *
 *  \iparam bRampType   configuration element ConfigP0RampType
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigP0RampType(quint8 bRampType)
{
    m_Config.listMotionProfiles[0].bRampType = bRampType;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bRampType; Property: motion_profile_ramp_type
 *  in local parameter set.
 *
 *  Profile 0: ramp type (s-curve)
 *
 *  \return value of bRampType
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigP0RampType()
{
    return m_Config.listMotionProfiles[0].bRampType;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bExists; Property: limitswitch1_exists
 *  in local parameter set.
 *
 *  Limit Switch 1: flag to mark it as exists
 *
 *  \todo verify parameter set.
 *
 *  \iparam bExists   configuration element ConfigLS1Exists
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigLS1Exists(quint8 bExists)
{
    m_Config.LimitSwitch1.bExists = bExists;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bExists; Property: limitswitch1_exists
 *  in local parameter set.
 *
 *  Limit Switch 1: flag to mark it as exists
 *
 *  \return value of bExists
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigLS1Exists()
{
    return m_Config.LimitSwitch1.bExists;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bPolarity; Property: limitswitch1_polarity
 *  in local parameter set.
 *
 *  Limit Switch 1: input polarity 0: regular, 1: inverted
 *
 *  \todo verify parameter set.
 *
 *  \iparam bPolarity   configuration element ConfigLS1Polarity
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigLS1Polarity(quint8 bPolarity)
{
    m_Config.LimitSwitch1.bPolarity = bPolarity;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bPolarity; Property: limitswitch1_polarity
 *  in local parameter set.
 *
 *  Limit Switch 1: input polarity 0: regular, 1: inverted
 *
 *  \return value of bPolarity
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigLS1Polarity()
{
    return m_Config.LimitSwitch1.bPolarity;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bSampleRate; Property: limitswitch1_sample_rate
 *  in local parameter set.
 *
 *  Limit Switch 1: sample rate in ms when motor stands still; during movement a value is read every half-step
 *
 *  \todo verify parameter set.
 *
 *  \iparam bSampleRate   configuration element ConfigLS1SampleRate
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigLS1SampleRate(quint8 bSampleRate)
{
    m_Config.LimitSwitch1.bSampleRate = bSampleRate;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bSampleRate; Property: limitswitch1_sample_rate
 *  in local parameter set.
 *
 *  Limit Switch 1: sample rate in ms when motor stands still; during movement a value is read every half-step
 *
 *  \return value of bSampleRate
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigLS1SampleRate()
{
    return m_Config.LimitSwitch1.bSampleRate;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bDebounce; Property: limitswitch1_debounce
 *  in local parameter set.
 *
 *  Limit Switch 1: debounce counter; if ReferenceRun skips slow movement, the limit switch bounces; debounce needs to be increased
 *
 *  \todo verify parameter set.
 *
 *  \iparam bDebounce   configuration element ConfigLS1Debounce
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigLS1Debounce(quint8 bDebounce)
{
    m_Config.LimitSwitch1.bDebounce = bDebounce;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bDebounce; Property: limitswitch1_debounce
 *  in local parameter set.
 *
 *  Limit Switch 1: debounce counter; if ReferenceRun skips slow movement, the limit switch bounces; debounce needs to be increased
 *
 *  \return value of bDebounce
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigLS1Debounce()
{
    return m_Config.LimitSwitch1.bDebounce;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bExists; Property: limitswitch2_exists
 *  in local parameter set.
 *
 *  Limit Switch 2: flag to mark it as exists
 *
 *  \todo verify parameter set.
 *
 *  \iparam bExists   configuration element ConfigLS2Exists
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigLS2Exists(quint8 bExists)
{
    m_Config.LimitSwitch2.bExists = bExists;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bExists; Property: limitswitch2_exists
 *  in local parameter set.
 *
 *  Limit Switch 2: flag to mark it as exists
 *
 *  \return value of bExists
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigLS2Exists()
{
    return m_Config.LimitSwitch2.bExists;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bPolarity; Property: limitswitch2_polarity
 *  in local parameter set.
 *
 *  Limit Switch 2: input polarity 0: regular, 1: inverted
 *
 *  \todo verify parameter set.
 *
 *  \iparam bPolarity   configuration element ConfigLS2Polarity
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigLS2Polarity(quint8 bPolarity)
{
    m_Config.LimitSwitch2.bPolarity = bPolarity;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bPolarity; Property: limitswitch2_polarity
 *  in local parameter set.
 *
 *  Limit Switch 2: input polarity 0: regular, 1: inverted
 *
 *  \return value of bPolarity
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigLS2Polarity()
{
    return m_Config.LimitSwitch2.bPolarity;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bSampleRate; Property: limitswitch2_sample_rate
 *  in local parameter set.
 *
 *  Limit Switch 2: sample rate in ms when motor stands still; during movement a value is read every half-step
 *
 *  \todo verify parameter set.
 *
 *  \iparam bSampleRate   configuration element ConfigLS2SampleRate
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigLS2SampleRate(quint8 bSampleRate)
{
    m_Config.LimitSwitch2.bSampleRate = bSampleRate;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bSampleRate; Property: limitswitch2_sample_rate
 *  in local parameter set.
 *
 *  Limit Switch 2: sample rate in ms when motor stands still; during movement a value is read every half-step
 *
 *  \return value of bSampleRate
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigLS2SampleRate()
{
    return m_Config.LimitSwitch2.bSampleRate;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bDebounce; Property: limitswitch2_debounce
 *  in local parameter set.
 *
 *  Limit Switch 2: debounce counter; if ReferenceRun skips slow movement, the limit switch bounces; debounce needs to be increased
 *
 *  \todo verify parameter set.
 *
 *  \iparam bDebounce   configuration element ConfigLS2Debounce
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigLS2Debounce(quint8 bDebounce)
{
    m_Config.LimitSwitch2.bDebounce = bDebounce;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bDebounce; Property: limitswitch2_debounce
 *  in local parameter set.
 *
 *  Limit Switch 2: debounce counter; if ReferenceRun skips slow movement, the limit switch bounces; debounce needs to be increased
 *
 *  \return value of bDebounce
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigLS2Debounce()
{
    return m_Config.LimitSwitch2.bDebounce;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bValid; Property: position_code1_value
 *  in local parameter set.
 *
 *  Position Code 1: position code value (see also ConfigRefRunPosition) 1: LS1, 2: LS2, 3: LS1+LS2
 *
 *  \todo verify parameter set.
 *
 *  \iparam bValid   configuration element ConfigPos1Value
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigPos1Value(quint8 bValid)
{
    m_Config.PosCode1.bValid = bValid;
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bValid; Property: position_code1_value
 *  in local parameter set.
 *
 *  Position Code 1: position code value (see also ConfigRefRunPosition) 1: LS1, 2: LS2, 3: LS1+LS2
 *
 *  \return value of bValid
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigPos1Value()
{
    return m_Config.PosCode1.bValid;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bStop; Property: position_code1_stop
 *  in local parameter set.
 *
 *  Position Code 1: stop motor when this position is crossed: 0 disabled, 1 enabled
 *
 *  \todo verify parameter set.
 *
 *  \iparam bStop   configuration element ConfigPos1Stop
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigPos1Stop(quint8 bStop)
{
    m_Config.PosCode1.bStop = bStop;
    return true;
}
#ifdef PRE_ALFA_TEST
bool WrapperFmStepperMotor::SetConfigPos2Stop(quint8 bStop)
{
    m_Config.PosCode2.bStop = bStop;
    return true;
}

quint8 WrapperFmStepperMotor::GetConfigPos2Stop()
{
    return m_Config.PosCode2.bStop;
}


bool WrapperFmStepperMotor::SetConfigPos3Stop(quint8 bStop)
{
    m_Config.PosCode3.bStop = bStop;
    return true;
}

quint8 WrapperFmStepperMotor::GetConfigPos3Stop()
{
    return m_Config.PosCode3.bStop;
}

#endif
/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bStop; Property: position_code1_stop
 *  in local parameter set.
 *
 *  Position Code 1: stop motor when this position is crossed: 0 disabled, 1 enabled
 *
 *  \return value of bStop
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigPos1Stop()
{
    return m_Config.PosCode1.bStop;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set configuration value of bStopDir; Property: position_code1_stop_dir
 *  in local parameter set.
 *
 *  Position Code 1: stop motor when limit switch is crossed in this direction: 1 cw 2 ccw
 *
 *  \todo verify parameter set.
 *
 *  \iparam bStopDir   configuration element ConfigPos1StopDir
 *
 *  \return true if current parameter set is valid, else false
 */
/****************************************************************************/
bool WrapperFmStepperMotor::SetConfigPos1StopDir(quint8 bStopDir)
{
    m_Config.PosCode1.bStopDir = (DeviceControl::CANFctModuleStepperMotor::RotationDir_t) bStopDir;
#if 0
    if(bStopDir == 1)
    {
        m_Config.PosCode2.bStopDir = 2;
    }
    else if(bStopDir == 2)
    {
        m_Config.PosCode2.bStopDir = 1;
    }

#endif
    return true;
}

#ifdef PRE_ALFA_TEST
/**
  direction: 1 cw
             2 ccw
*/

bool WrapperFmStepperMotor::SetConfigPos2Value(quint8 bValid)
{
    m_Config.PosCode2.bValid = bValid;
    return true;
}

quint8 WrapperFmStepperMotor::GetConfigPos2Value()
{
    return m_Config.PosCode2.bValid;
}

bool WrapperFmStepperMotor::SetConfigPos3Value(quint8 bValid)
{
    m_Config.PosCode3.bValid = bValid;
    return true;
}

quint8 WrapperFmStepperMotor::GetConfigPos3Value()
{
    return m_Config.PosCode3.bValid;
}

quint8 WrapperFmStepperMotor::GetConfigPos3StopDir()
{
    return m_Config.PosCode3.bStopDir;
}


bool WrapperFmStepperMotor::SetConfigPos3StopDir(quint8 bStopDir)
{
    m_Config.PosCode3.bStopDir = (DeviceControl::CANFctModuleStepperMotor::RotationDir_t) bStopDir;

    return true;
}

bool WrapperFmStepperMotor::SetRotationDirection(quint8 direction)
{
    if(direction == DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CW)
    {
        m_Config.PosCode1.bStopDir = 1;
        m_Config.PosCode2.bStopDir = 2;
        //
    }
    else if(direction == DeviceControl::CANFctModuleStepperMotor::ROTATION_DIR_CCW)
    {
        m_Config.PosCode1.bStopDir = 2;
        m_Config.PosCode2.bStopDir = 1;
    }
    return true;
}
quint8 WrapperFmStepperMotor::GetRotationDirection()
{
     return m_Config.PosCode1.bStopDir;
}
#if 0
DeviceControl::CANFctModuleStepperMotor::RotationDir_t WrapperFmStepperMotor::GetRotationDirection()
{
     return (DeviceControl::CANFctModuleStepperMotor::RotationDir_t)m_Config.PosCode1.bStopDir;
}
#endif
#endif
/****************************************************************************/
/*!
 *  \brief  Script-API: Get configuration value of bStopDir; Property: position_code1_stop_dir
 *  in local parameter set.
 *
 *  Position Code 1: stop motor when limit switch is crossed in this direction: 1 cw 2 ccw
 *
 *  \return value of bStopDir
 */
/****************************************************************************/
quint8 WrapperFmStepperMotor::GetConfigPos1StopDir()
{
    return m_Config.PosCode1.bStopDir;
}

#ifdef PRE_ALFA_TEST
/****************************************************************************/
/*!
 *  \brief  Slot for error
 *  \iparam InstanceID = id of instance
 *  \iparam ErrorGroup = group of error
 *  \iparam ErrorCode  = the code of error.
 *  \iparam ErrorData  = the data time of the error.
 *  \iparam ErrorTime  = the time of the error.
 */
/****************************************************************************/
void WrapperFmStepperMotor::OnError(quint32 InstanceID, quint16 ErrorGroup, quint16 ErrorCode, quint16 ErrorData, QDateTime ErrorTime)
{
    Q_UNUSED(ErrorTime);
    Log(tr("Rotary Valve get error, Instance ID: %1, Error Group: %2, Error Code: %3, Error Data: %4").arg(InstanceID).arg(ErrorGroup).arg(ErrorCode).arg(ErrorData));
}
#endif
// vi: set ts=4 sw=4 et:
