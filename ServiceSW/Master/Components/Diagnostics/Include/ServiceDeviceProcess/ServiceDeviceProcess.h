/****************************************************************************/
/*! \file ServiceDeviceProcess.h
 *
 *  \brief Definition file for class ServiceDeviceProcess.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-09-09
 *  $Author:    $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SERV_DEV_PROC_H
#define SERV_DEV_PROC_H

#include <QEventLoop>
#include <QMap>
#include <QStringList>
#include <Global/Include/GlobalDefines.h>
#include "Core/Include/ServiceDefines.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"


namespace Diagnostics {

typedef enum {
    RETURN_OK = 1,
    RETURN_ERR_NULL_POINTER = -1,
    RETURN_ERR_FAIL = -2
} ErrorCode_t;

/****************************************************************************/
/**
 * \brief This class is a helper to do handle device processing related commands.
 *
 * This class has thread controller and Interface for the device command
 * processing. It communicates with the real hardware with signals and slots.
 * \warning This class is not thread safe!
 */
/****************************************************************************/

class ServiceDeviceProcess : public QObject
{
    Q_OBJECT
private:
    static ServiceDeviceProcess* mp_Instance;
    ServiceDeviceProcess();

public:
    static ServiceDeviceProcess* Instance();
    static void Destroy();

public:

    /****************************************************************************/
    /**
     * \brief  To initiatize devices
     */
    /****************************************************************************/
    void Initialize();
    /****************************************************************************/
    /**
     * \brief Get Pointer to DataManager Object
     * \return True if Device Initialization is completed.
     */
    /****************************************************************************/
    bool IsInitialized();
    /****************************************************************************/


    int MainRelaySetOnOff(bool OnFlag);
    int AlarmSetOnOff(int LocalRemote, bool OnFlag);
    int AlarmGetState(int LocalRemote, qint32 *RetState);
    int MainControlGetCurrent(quint8 SlaveType, quint16 *RetCurrent);
    int MainControlGetVoltage(quint8 SlaveType, quint16 *RetVoltage);

    int OvenStartHeating(qreal TargetTempTop, qreal TargetTempBottom);
    int OvenStopHeating();
    int OvenGetTemp(qreal *RetTempTop, qreal *RetTempBottom1, qreal *RetTempBottom2);
    int OvenGetCurrent(quint16 *RetCurrentTop, quint16 *RetCurrentBottom);
    int OvenGetCoverSensorState(qint32 *RetCoverSensorState);
    int OvenGetSwitchType(int *RetSwithType);

    int RetortStartHeating(qreal TargetTempSide, qreal TargetTempBottom);
    int RetortStopHeating();
    int RetortGetTemp(qreal *RetTempSide, qreal *RetTempBottom1, qreal *RetTempBottom2);
    int RetortGetCurrent(quint16 *RetCurrentSide, quint16 *RetCurrentBottom);
    int RetortGetLidLockState(qint32 *RetLidLockState);
    int RetortSetTemperatureSwitchState(qint8 SwitchState, qint8 AutoSwitch);
    int RetortGetHeaterSwitchType(quint8 *RetSwitchType);


    int LiquidTubeStartHeating(qreal TargetTemp);
    int LiquidTubeStopHeating();
    int LiquidTubeGetTemp(qreal *RetTemp);
    int LiquidTubeGetCurrent(quint16 *RetCurrent);

    int AirTubeStartHeating(qreal TargetTemp);
    int AirTubeStopHeating();
    int AirTubeGetTemp(qreal *RetTemp);
    int AirTubeGetCurrent(quint16 *RetCurrent);

    int RVStartHeating(qreal TargetTemp);
    int RVStopHeating();
    int RVGetTemp(qreal *RetTempSensor1, qreal* RetTempSensor2);
    int RVGetCurrent(quint16 *RetCurrent);
    int RVInitialize(bool TubeFlag=false, quint32 Position=0);
    int RVMovePosition(bool TubeFlag, int Position);
    int RVSetTemperatureSwitchState(qint8 SwitchState, qint8 AutoSwitch);
    int RVGetHeaterSwitchType(quint8 *RetSwitchType);

    int LSStartHeating(bool QuickFlag, bool WaterFlag);
    int LSStopHeating();
    int LSGetTemp(qreal *RetTemp);
    int LSGetCurrent(quint16 *RetCurrent);
    int LSHeatingLevelSensor(bool WaterFlag);

    int PumpBuildPressure(float TargetPressure);
    int PumpReleasePressure();
    int PumpSetPressure(quint8 Flag, float Pressure);
    int PumpGetPressure(float *RetPressure);
    int PumpSetFan(quint8 OnFlag);
    int PumpSetValve(quint8 ValveIndex, quint8 ValveState);
    int PumpStopCompressor();
    int PumpSucking(quint32 DelayTime=0);
    int PumpDraining(quint32 DelayTime=0);
    int PumpReadPressureDrift(float *RetPressureDrift);
    int PumpWritePressureDrift(float PressureDrift);

    int GetSlaveModuleReportError(quint8 ErrorCode, const QString& DevName, quint32 SensorName);
    void Pause(quint32 MillSeconds);

private:
    QMap<QString, QEventLoop*>       m_EventLoopMap;
    QMap<QString, QStringList>      m_ResultsMap;
    bool                            m_IsInitialized;

public slots:
    void HandleServResult(QString ReqName, int Error, QStringList Results);
signals:
    void SendServRequest(QString ReqName, QStringList Params);
private:
    int GetResponse(QString ReqName, int TimeOutSeconds=10);
};

} // end namespace Diagnostics

#endif // SERV_DEV_PROC_H
