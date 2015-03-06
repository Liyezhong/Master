/****************************************************************************/
/*! \file WrapperFmStepperMotor.h
 *
 *  \brief  Definition file for class WrapperFMStepperMotor
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

#ifndef WRAPPERFMSTEPPERMOTOR_H
#define WRAPPERFMSTEPPERMOTOR_H

#include <QObject>
#include <QEventLoop>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/StepperMotor.h"
#include "DeviceControl/Include/SlaveModules/ModuleConfig.h"

//lint -e553
using namespace DeviceControl;

class CResetData;

/****************************************************************************/
/*! \brief Wrapper for function module StepperMotor.
 *
 *  This wrapper class provides a QtScript interface to the
 *  function module StepperMotor.
 *
 *  Examples:
 *   \skipline [WrapperFMStepperMotor]
 *   \until    [WrapperFMStepperMotor]
 */
/****************************************************************************/
class WrapperFmStepperMotor : public WrapperBase
{
    Q_OBJECT
    //Q_ENUMS(DeviceControl::CANFctModuleStepperMotor::RotationDir_t)
    //Q_ENUMS(DeviceControl::CANFctModuleStepperMotor::RotationType_t)
    //Q_ENUMS(DeviceControl::CANFctModuleStepperMotor::DriverType_t)


    /****************************************************************************/
    /*                                                                          */
    /* Auto-generated properties                                                */
    /*                                                                          */
    /****************************************************************************/


    Q_PROPERTY(qint32 rotation_type READ GetConfigRotationType WRITE SetConfigRotationType) //!< property for ConfigRotationType
    Q_PROPERTY(quint16 rotation_steps_revolution READ GetConfigResolution WRITE SetConfigResolution) //!< property for ConfigResolution
    Q_PROPERTY(quint16 rotation_rev_hstep_count READ GetConfigRevHStepCount WRITE SetConfigRevHStepCount) //!< property for ConfigRevHStepCount
    Q_PROPERTY(qint32 rotation_direction READ GetConfigDirection WRITE SetConfigDirection) //!< property for ConfigDirection
    Q_PROPERTY(qint32 rotation_position_min READ GetConfigMinPosition WRITE SetConfigMinPosition) //!< property for ConfigMinPosition
    Q_PROPERTY(qint32 rotation_position_max READ GetConfigMaxPosition WRITE SetConfigMaxPosition) //!< property for ConfigMaxPosition
    Q_PROPERTY(qint16 rotation_speed_min READ GetConfigMinSpeed WRITE SetConfigMinSpeed) //!< property for ConfigMinSpeed
    Q_PROPERTY(qint16 rotation_speed_max READ GetConfigMaxSpeed WRITE SetConfigMaxSpeed) //!< property for ConfigMaxSpeed
    Q_PROPERTY(quint8 rotation_current_run READ GetConfigCurrentRun WRITE SetConfigCurrentRun) //!< property for ConfigCurrentRun
    Q_PROPERTY(quint8 rotation_current_stop READ GetConfigCurrentStop WRITE SetConfigCurrentStop) //!< property for ConfigCurrentStop
    Q_PROPERTY(quint16 rotation_current_delay READ GetConfigCurrentDelay WRITE SetConfigCurrentDelay) //!< property for ConfigCurrentDelay
    Q_PROPERTY(quint8 encoder_type READ GetConfigEncoderType WRITE SetConfigEncoderType) //!< property for ConfigEncoderType
    Q_PROPERTY(quint16 encoder_resolution READ GetConfigEncoderResolution WRITE SetConfigEncoderResolution) //!< property for ConfigEncoderResolution
    Q_PROPERTY(qint32 encoder_rotation READ GetConfigEncoderDir WRITE SetConfigEncoderDir) //!< property for ConfigEncoderDir
    Q_PROPERTY(qint8 reference_run_position READ GetConfigRefRunPosition WRITE SetConfigRefRunPosition) //!< property for ConfigRefRunPosition
    Q_PROPERTY(qint32 reference_run_max_distance READ GetConfigRefRunMaxDistance WRITE SetConfigRefRunMaxDistance) //!< property for ConfigRefRunMaxDistance
    Q_PROPERTY(qint16 reference_run_timeout READ GetConfigRefRunTimeout WRITE SetConfigRefRunTimeout) //!< property for ConfigRefRunTimeout
    Q_PROPERTY(qint32 reference_run_reverse_distance READ GetConfigRefRunReverseDistance WRITE SetConfigRefRunReverseDistance) //!< property for ConfigRefRunReverseDistance
    Q_PROPERTY(qint32 reference_run_refpos_offset READ GetConfigRefPosOffset WRITE SetConfigRefPosOffset) //!< property for ConfigRefPosOffset
    Q_PROPERTY(qint16 reference_run_slow_speed READ GetConfigRefRunSlowSpeed WRITE SetConfigRefRunSlowSpeed) //!< property for ConfigRefRunSlowSpeed
    Q_PROPERTY(qint16 reference_run_high_speed READ GetConfigRefRunHighSpeed WRITE SetConfigRefRunHighSpeed) //!< property for ConfigRefRunHighSpeed
    Q_PROPERTY(qint16 supervision_steploss_warn_limit READ GetConfigStepLossWarnLimit WRITE SetConfigStepLossWarnLimit) //!< property for ConfigStepLossWarnLimit
    Q_PROPERTY(qint16 supervision_steploss_error_limit READ GetConfigStepLossErrorLimit WRITE SetConfigStepLossErrorLimit) //!< property for ConfigStepLossErrorLimit
    Q_PROPERTY(qint16 supervision_current_limit READ GetConfigCurrentLimit WRITE SetConfigCurrentLimit) //!< property for ConfigCurrentLimit
    Q_PROPERTY(qint32 driver_type READ GetConfigDriverType WRITE SetConfigDriverType) //!< property for ConfigDriverType
    Q_PROPERTY(quint32 driver_reg_drvConf READ GetConfigDrvConf WRITE SetConfigDrvConf) //!< property for ConfigDrvConf
    Q_PROPERTY(quint32 driver_reg_sgcsConf READ GetConfigSgcsConf WRITE SetConfigSgcsConf) //!< property for ConfigSgcsConf
    Q_PROPERTY(quint32 driver_reg_smartEn READ GetConfigSmartEn WRITE SetConfigSmartEn) //!< property for ConfigSmartEn
    Q_PROPERTY(quint32 driver_reg_chopConf READ GetConfigChopConf WRITE SetConfigChopConf) //!< property for ConfigChopConf
    Q_PROPERTY(quint16 motion_profile0_speed_min READ GetConfigP0SpeedMin WRITE SetConfigP0SpeedMin) //!< property for ConfigP0SpeedMin
    Q_PROPERTY(quint16 motion_profile0_speed_max READ GetConfigP0SpeedMax WRITE SetConfigP0SpeedMax) //!< property for ConfigP0SpeedMax
    Q_PROPERTY(quint16 motion_profile0_acc READ GetConfigP0Acc WRITE SetConfigP0Acc) //!< property for ConfigP0Acc
    Q_PROPERTY(quint16 motion_profile0_dec READ GetConfigP0Dec WRITE SetConfigP0Dec) //!< property for ConfigP0Dec
    Q_PROPERTY(quint16 motion_profile0_acc_time READ GetConfigP0AccTime WRITE SetConfigP0AccTime) //!< property for ConfigP0AccTime
    Q_PROPERTY(quint16 motion_profile0_dec_time READ GetConfigP0DecTime WRITE SetConfigP0DecTime) //!< property for ConfigP0DecTime
    Q_PROPERTY(quint8 motion_profile0_micro_steps READ GetConfigP0MicroSteps WRITE SetConfigP0MicroSteps) //!< property for ConfigP0MicroSteps
    Q_PROPERTY(quint8 motion_profile_ramp_type READ GetConfigP0RampType WRITE SetConfigP0RampType) //!< property for ConfigP0RampType
    Q_PROPERTY(quint8 limitswitch1_exists READ GetConfigLS1Exists WRITE SetConfigLS1Exists) //!< property for ConfigLS1Exists
    Q_PROPERTY(quint8 limitswitch1_polarity READ GetConfigLS1Polarity WRITE SetConfigLS1Polarity) //!< property for ConfigLS1Polarity
    Q_PROPERTY(quint8 limitswitch1_sample_rate READ GetConfigLS1SampleRate WRITE SetConfigLS1SampleRate) //!< property for ConfigLS1SampleRate
    Q_PROPERTY(quint8 limitswitch1_debounce READ GetConfigLS1Debounce WRITE SetConfigLS1Debounce) //!< property for ConfigLS1Debounce
    Q_PROPERTY(quint8 limitswitch2_exists READ GetConfigLS2Exists WRITE SetConfigLS2Exists) //!< property for ConfigLS2Exists
    Q_PROPERTY(quint8 limitswitch2_polarity READ GetConfigLS2Polarity WRITE SetConfigLS2Polarity) //!< property for ConfigLS2Polarity
    Q_PROPERTY(quint8 limitswitch2_sample_rate READ GetConfigLS2SampleRate WRITE SetConfigLS2SampleRate) //!< property for ConfigLS2SampleRate
    Q_PROPERTY(quint8 limitswitch2_debounce READ GetConfigLS2Debounce WRITE SetConfigLS2Debounce) //!< property for ConfigLS2Debounce
    Q_PROPERTY(quint8 position_code1_value READ GetConfigPos1Value WRITE SetConfigPos1Value) //!< property for ConfigPos1Value
    Q_PROPERTY(quint8 position_code1_stop READ GetConfigPos1Stop WRITE SetConfigPos1Stop) //!< property for ConfigPos1Stop
    Q_PROPERTY(quint8 position_code1_stop_dir READ GetConfigPos1StopDir WRITE SetConfigPos1StopDir) //!< property for ConfigPos1StopDir




public:
    static const QString ROTATION;  ///< WorkingMode: rotation (default)
    static const QString LINEAR;    ///< WorkingMode: linear, requires appropriate configuration

    WrapperFmStepperMotor(QString Name, CStepperMotor *pStepperMotor, QObject *pParent);
    ~WrapperFmStepperMotor();

public slots:
    bool DoReferenceRun();
    bool DriveSpeed(qint16 Speed);
    bool MoveToPosition(qint32 Position, qint32 Timeout = -1);
    bool MoveFromPosition(qint32 Distance, qint32 Timeout = -1);
    QString GetPosition();
    QString GetLimitSwitchCode();
    qint16 GetSpeed();
    bool SetProfile(qint8 Profile);
    qint8 GetProfile();
    bool Wait(qint32 Timeout = -1);
    void Reset();
    bool ReqDataReset();
    bool Enable();
    bool Disable();
    bool ApplyNewParameterSet();
    void RestoreLastValidParameterSet();
    QString GetOperationTime();
    QString GetRevolutionCount();
    QString GetDirChangeCount();
#ifdef PRE_ALFA_TEST
    void ApplyNewParameterSetTimerCB();
#endif
    /*
    SetDirection(cw|ccw)
    qint32 GetDirection()
    GetEncoderPosition
    GetMotorCurrent
    GetLifeCycleData
    HIGH|LOW|ERROR GetLimitSwitchState(index)
    SetScaleFactor(float)
    GetScaleFactor(float)
    */


    /****************************************************************************/
    /*                                                                          */
    /* Auto-generated configuration getters/setters                             */
    /*                                                                          */
    /****************************************************************************/


    bool SetConfigRotationType(qint32 rotationType);
    qint32 GetConfigRotationType();
    bool SetConfigResolution(quint16 sResolution);
    quint16 GetConfigResolution();
    bool SetConfigRevHStepCount(quint16 sRevHStepCount);
    quint16 GetConfigRevHStepCount();
    bool SetConfigDirection(qint32 bDirection);
    qint32 GetConfigDirection();
    bool SetConfigMinPosition(qint32 lMinPosition);
    qint32 GetConfigMinPosition();
    bool SetConfigMaxPosition(qint32 lMaxPosition);
    qint32 GetConfigMaxPosition();
    bool SetConfigMinSpeed(qint16 sMinSpeed);
    qint16 GetConfigMinSpeed();
    bool SetConfigMaxSpeed(qint16 sMaxSpeed);
    qint16 GetConfigMaxSpeed();
    bool SetConfigCurrentRun(quint8 runCurrentScale);
    quint8 GetConfigCurrentRun();
    bool SetConfigCurrentStop(quint8 stopCurrentScale);
    quint8 GetConfigCurrentStop();
    bool SetConfigCurrentDelay(quint16 stopCurrentDelay);
    quint16 GetConfigCurrentDelay();
    bool SetConfigEncoderType(quint8 bEncoderType);
    quint8 GetConfigEncoderType();
    bool SetConfigEncoderResolution(quint16 sEncoderResolution);
    quint16 GetConfigEncoderResolution();
    bool SetConfigEncoderDir(qint32 bEncoderDir);
    qint32 GetConfigEncoderDir();
    bool SetConfigRefRunPosition(qint8 refRunRefPos);
    qint8 GetConfigRefRunPosition();
    bool SetConfigRefRunMaxDistance(qint32 lRefRunMaxDistance);
    qint32 GetConfigRefRunMaxDistance();
#ifndef PRE_ALFA_TEST
qint16 GetConfigRefRunTimeout();
bool SetConfigRefRunTimeout(qint16 sRefRunTimeout);
#else
quint16 GetConfigRefRunTimeout();
bool SetConfigRefRunTimeout(quint16 sRefRunTimeout);
#endif
    bool SetConfigRefRunReverseDistance(qint32 lRefRunReverseDistance);
    qint32 GetConfigRefRunReverseDistance();
    bool SetConfigRefPosOffset(qint32 lRefPosOffset);
    qint32 GetConfigRefPosOffset();
    bool SetConfigRefRunSlowSpeed(qint16 sRefRunSlowSpeed);
    qint16 GetConfigRefRunSlowSpeed();
    bool SetConfigRefRunHighSpeed(qint16 sRefRunHighSpeed);
    qint16 GetConfigRefRunHighSpeed();
    bool SetConfigStepLossWarnLimit(qint16 sStepLossWarnLimit);
    qint16 GetConfigStepLossWarnLimit();
    bool SetConfigStepLossErrorLimit(qint16 sStepLossErrorLimit);
    qint16 GetConfigStepLossErrorLimit();
    bool SetConfigCurrentLimit(qint16 sCurrentLimit);
    qint16 GetConfigCurrentLimit();
    bool SetConfigDriverType(qint32 driverType);
    qint32 GetConfigDriverType();
    bool SetConfigDrvConf(quint32 drvConf);
    quint32 GetConfigDrvConf();
    bool SetConfigSgcsConf(quint32 sgcsConf);
    quint32 GetConfigSgcsConf();
    bool SetConfigSmartEn(quint32 smartEn);
    quint32 GetConfigSmartEn();
    bool SetConfigChopConf(quint32 chopConf);
    quint32 GetConfigChopConf();
    bool SetConfigP0SpeedMin(quint16 sSpeedMin);
    quint16 GetConfigP0SpeedMin();
    bool SetConfigP0SpeedMax(quint16 sSpeedMax);
    quint16 GetConfigP0SpeedMax();
    bool SetConfigP0Acc(quint16 sAcc);
    quint16 GetConfigP0Acc();
    bool SetConfigP0Dec(quint16 sDec);
    quint16 GetConfigP0Dec();
    bool SetConfigP0AccTime(quint16 sAccTime);
    quint16 GetConfigP0AccTime();
    bool SetConfigP0DecTime(quint16 sDecTime);
    quint16 GetConfigP0DecTime();
    bool SetConfigP0MicroSteps(quint8 bMicroSteps);
    quint8 GetConfigP0MicroSteps();
    bool SetConfigP0RampType(quint8 bRampType);
    quint8 GetConfigP0RampType();
    bool SetConfigLS1Exists(quint8 bExists);
    quint8 GetConfigLS1Exists();
    bool SetConfigLS1Polarity(quint8 bPolarity);
    quint8 GetConfigLS1Polarity();
    bool SetConfigLS1SampleRate(quint8 bSampleRate);
    quint8 GetConfigLS1SampleRate();
    bool SetConfigLS1Debounce(quint8 bDebounce);
    quint8 GetConfigLS1Debounce();
    bool SetConfigLS2Exists(quint8 bExists);
    quint8 GetConfigLS2Exists();
    bool SetConfigLS2Polarity(quint8 bPolarity);
    quint8 GetConfigLS2Polarity();
    bool SetConfigLS2SampleRate(quint8 bSampleRate);
    quint8 GetConfigLS2SampleRate();
    bool SetConfigLS2Debounce(quint8 bDebounce);
    quint8 GetConfigLS2Debounce();
    bool SetConfigPos1Value(quint8 bValid);
    quint8 GetConfigPos1Value();
    bool SetConfigPos1Stop(quint8 bStop);
    quint8 GetConfigPos1Stop();
    bool SetConfigPos1StopDir(quint8 bStopDir);
    quint8 GetConfigPos1StopDir();

#if PRE_ALFA_TEST
    /*
    bool SetConfigRefRunRefPosSkip(quint8 refRunRefPosSkip);
    quint8 GetConfigRefRunRefPosSkip();
    */
    bool SetConfigPos1HitSkip(quint8 hitSkip);
    quint8 GetConfigPos1HitSkip();
    bool SetConfigPos2HitSkip(quint8 hitSkip);
    quint8 GetConfigPos2HitSkip();
    bool SetConfigPos3HitSkip(quint8 hitSkip);
    quint8 GetConfigPos3HitSkip();
    quint8 GetConfigPos2Stop();
    bool SetConfigPos2Stop(quint8 bStop);
    bool SetConfigPos3Stop(quint8 bStop);
    quint8 GetConfigPos3Stop();
    bool SetConfigPos3StopDir(quint8 bStopDir);
    quint8 GetConfigPos3StopDir();

    bool SetConfigPos2Value(quint8 bValid);
    quint8 GetConfigPos2Value();
    bool SetConfigPos3Value(quint8 bValid);
    quint8 GetConfigPos3Value();

    bool SetRotationDirection(quint8 direction);
    quint8 GetRotationDirection();
    qint32 MoveToInitialPosition(bool TubeFlag = false, quint32 Position=0);

    qint32 MoveToTubePosition(quint32 Position);
    qint32 MoveToSealPosition(quint32 Position);

    qint32 MoveCW();
    qint32 MoveCCW();
    qint32 MoveBackOnePort();
#endif

private slots:
    void OnReferenceRun(quint32 InstanceID, ReturnCode_t ReturnCode, qint32 Position);
    void OnSetPosition(quint32 InstanceID, ReturnCode_t ReturnCode, qint32 Position, qint16 Speed);
    void OnGetPosition(quint32 InstanceID, ReturnCode_t ReturnCode, qint32 Position, qint8 PosCode);
    void OnSetState(quint32 InstanceID, ReturnCode_t ReturnCode);
    void OnSetConfiguration(quint32 InstanceID, ReturnCode_t ReturnCode);
    void OnGetSpeed(quint32 InstanceID, ReturnCode_t ReturnCode, qint16 Speed);
    void OnGetLifeCycleData(quint32 InstanceID, ReturnCode_t ReturnCode, quint32 ld1, quint32 DirChangeCount, quint32 OperationTime);
#ifdef PRE_ALFA_TEST
    void ReferenceRunTimerCB();
    void OnError(quint32 InstanceID, quint16 ErrorGroup, quint16 ErrorCode, quint16 ErrorData, QDateTime ErrorTime);
#endif
private:
    bool SetPosition(qint32 Position, qint32 Timeout);
    bool SetState(bool flag);
#ifdef PRE_ALFA_TEST
    qint32 ReferenceRunWithTimeout(quint32 LowerLimit, quint32 UpperLimit);
    qint32 DoReferenceRunWithStepCheck(quint32 LowerLimit, quint32 UpperLimit);
    qint32 MoveToNextPortCW();
    qint32 MoveToNextPortCCW();
    qint32 MoveToNextPort(bool changeParameter, quint32 LowerLimit, quint32 UpperLimit);
    qint32 MoveToEDPosition(quint32 Position);
    quint32 GetEDPosition();
    void SetPrevEDPosition(quint32 position);
    quint32 GetPrevEDPosition();
    void SetEDPosition(quint32 position);
    void InitEDPosition();
    QString TranslateFromEDPosition(quint32 EDPosition);
    inline quint32 GetUpperLimit(quint32 CurrentEDPosition, DeviceControl::CANFctModuleStepperMotor::RotationDir_t Direction, bool ChangeDirection);
    inline quint32 GetLowerLimit(quint32 CurrentEDPosition, DeviceControl::CANFctModuleStepperMotor::RotationDir_t Direction, bool ChangeDirection);
    float m_EncoderDiskPosition;
    float m_PrevEncoderDiskPosition;
    qint32 m_RefRunRetCode;
    QEventLoop m_LoopApplyNewParameter;
#endif

    CStepperMotor *m_pStepperMotor;  //!< Pointer to the StepperMotor FM
    CResetData *mp_ResetData;           //!< Support for the reset data command
    CANFctModuleStepperMotor m_Config;      //!< Copy of configuration.

    QEventLoop m_LoopReferenceRun;   //!< Loop for blocking commands.
    QEventLoop m_LoopSetPosition;    //!< Loop for blocking commands.
    QEventLoop m_LoopRotation;       //!< Loop for blocking commands.
    QEventLoop m_LoopGetPosition;    //!< Loop for blocking commands.
    QEventLoop m_LoopSetState;       //!< Loop for blocking commands.
    QEventLoop m_LoopSetConfiguration;  //!< Loop for blocking commands.
    QEventLoop m_LoopWait;           //!< Loop for blocking commands.
    QEventLoop m_LoopGetSpeed;           //!< Loop for blocking commands.
    QEventLoop m_LoopGetLifeCycleData;      //!< Loop for blocking commands.

    bool m_SetPositionActive;        //!< Flag for an active asynchronous action.
    bool m_RotationOn;               //!< Flag for the rotation state.
    bool m_CurrentPositionKnown;     //!< Ensure a known absolute Position for relative movement.
    qint32 m_CurrentOperationTime;      //!< Current operation time (stored by asynchronous call)
    qint32 m_CurrentRevolutionCount;    //!< Current revolution count (stored by asynchronous call)
    qint32 m_CurrentDirChangeCount;     //!< Current direction change count (stored by asynchronous call)

    qint32 m_TargetPosition;   //!< Target position of linear movement; for verification of action result.
    qint32 m_CurrentPosition;  //!< Current position (stored by asynchronous call)
    qint32 m_CurrentLimitSwitchCode;  //!< Current limit switch code (0: none; 1: LS1 active; 2: LS2 active; 3: both active) (stored by asynchronous call)
    qint16 m_TargetSpeed;      //!< Target speed of rotation; for verification of action result.
    qint16 m_CurrentSpeed;     //!< Current speed (stored by asynchronous call)
    qint16 m_Speed;            //!< Default speed.
    qint8  m_Profile;          //!< Default profile.
};


//Q_DECLARE_METATYPE(DeviceControl::CANFctModuleStepperMotor::RotationDir_t)
//Q_DECLARE_METATYPE(DeviceControl::CANFctModuleStepperMotor::RotationType_t)
//Q_DECLARE_METATYPE(DeviceControl::CANFctModuleStepperMotor::DriverType_t)

#endif

// vi: set ts=4 sw=4 et:

