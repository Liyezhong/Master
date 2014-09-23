/****************************************************************************/
/*! \file ServiceDeviceProcess.cpp
 *
 *  \brief Implementation file for class ServiceDeviceProcess.
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
#include "Global/Include/Utils.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "Core/Include/ServiceUtils.h"
#include <QTimer>

namespace Diagnostics {

#define RV_MOVE_OK      1
#define RESPONSE_TIMEOUT        0

ServiceDeviceProcess* ServiceDeviceProcess::mp_Instance = NULL;

ServiceDeviceProcess* ServiceDeviceProcess::Instance()
{
    if (mp_Instance==NULL) {
        mp_Instance = new ServiceDeviceProcess();
    }
    return mp_Instance;
}


void ServiceDeviceProcess::Destroy()
{
    if (mp_Instance) {
        delete mp_Instance;
    }
    mp_Instance = NULL;
}

/****************************************************************************/
ServiceDeviceProcess::ServiceDeviceProcess()
{
    m_IsInitialized = false;
}

/****************************************************************************/
void ServiceDeviceProcess::Initialize()
{
    m_IsInitialized = true;
    m_EventLoopMap.clear();
    m_ResultsMap.clear();
}


/****************************************************************************/
bool ServiceDeviceProcess::IsInitialized()
{
    return m_IsInitialized;
}

int ServiceDeviceProcess::MainRelaySetOnOff(bool OnFlag)
{
    QString ReqName = "MainRelaySetOnOff";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(OnFlag));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;

    return Ret;
}

int ServiceDeviceProcess::AlarmSetOnOff(int LocalRemote, bool OnFlag)
{
    QString ReqName = "AlarmSetOnOff";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(LocalRemote));
    Params.append(QString("%1").arg(OnFlag));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;

    return Ret;
}

int ServiceDeviceProcess::AlarmGetState(int LocalRemote, qint32 *RetState)
{
    QString ReqName = "AlarmGetState";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(LocalRemote));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {  
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetState = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::MainControlGetCurrent(quint8 SlaveType, quint16 *RetCurrent)
{
    QString ReqName = "MainControlGetCurrent";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(SlaveType));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetCurrent = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::MainControlGetVoltage(quint8 SlaveType, quint16 *RetVoltage)
{
    QString ReqName = "MainControlGetVoltage";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(SlaveType));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetVoltage = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::OvenStartHeating(qreal TargetTempTop, qreal TargetTempBottom)
{
    QString ReqName = "OvenStartHeating";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(TargetTempTop));
    Params.append(QString("%1").arg(TargetTempBottom));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;

    return Ret;
}

int ServiceDeviceProcess::OvenStopHeating()
{
    QString ReqName = "OvenStopHeating";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;

    return Ret;
}

int ServiceDeviceProcess::OvenGetTemp(qreal *RetTempTop, qreal *RetTempBottom1, qreal *RetTempBottom2)
{
    QString ReqName = "OvenGetTemp";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        if (RetTempTop) {
            *RetTempTop = Results.at(0).toFloat();
        }
        if (RetTempBottom1) {
            *RetTempBottom1 = Results.at(1).toFloat();
        }
        if (RetTempBottom2) {
            *RetTempBottom2 = Results.at(2).toFloat();
        }
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::OvenGetCurrent(quint16 *RetCurrentTop, quint16 *RetCurrentBottom)
{
    QString ReqName = "OvenGetCurrent";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        if (RetCurrentTop) {
            *RetCurrentTop = Results.at(0).toInt();
        }
        if (RetCurrentBottom) {
            *RetCurrentBottom = Results.at(1).toInt();
        }
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::OvenGetCoverSensorState(qint32 *RetCoverSensorState)
{
    QString ReqName = "OvenGetCoverSensorState";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetCoverSensorState = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::OvenGetSwitchType(int *RetSwitchType)
{
    QString ReqName = "OvenGetSwithType";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetSwitchType = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RetortStartHeating(qreal TargetTempSide, qreal TargetTempBottom)
{
    QString ReqName = "RetortStartHeating";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(TargetTempSide));
    Params.append(QString("%1").arg(TargetTempBottom));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RetortStopHeating()
{
    QString ReqName = "RetortStopHeating";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;

    return Ret;
}

int ServiceDeviceProcess::RetortGetTemp(qreal *RetTempSide, qreal *RetTempBottom1, qreal *RetTempBottom2)
{
    QString ReqName = "RetortGetTemp";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        if (RetTempSide) {
            *RetTempSide = Results.at(0).toFloat();
        }

        if (RetTempBottom1) {
            *RetTempBottom1 = Results.at(1).toFloat();
        }

        if (RetTempBottom2) {
            *RetTempBottom2 = Results.at(2).toFloat();
        }
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RetortGetCurrent(quint16 *RetCurrentSide, quint16 *RetCurrentBottom)
{
    QString ReqName = "RetortGetCurrent";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        if (RetCurrentSide) {
            *RetCurrentSide = Results.at(0).toInt();
        }
        if (RetCurrentBottom) {
            *RetCurrentBottom = Results.at(1).toInt();
        }
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RetortGetLidLockState(qint32 *RetLidLockState)
{
    QString ReqName = "RetortGetLidLockState";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetLidLockState = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RetortSetTemperatureSwitchState(qint8 SwitchState, qint8 AutoSwitch)
{
    QString ReqName = "RetortSetTemperatureSwitchState";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(SwitchState));
    Params.append(QString("%1").arg(AutoSwitch));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;

    return Ret;
}

int ServiceDeviceProcess::RetortGetHeaterSwitchType(quint8 *RetSwitchType)
{
    QString ReqName = "RetortGetHeaterSwitchType";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetSwitchType = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}


int ServiceDeviceProcess::LiquidTubeStartHeating(qreal TargetTemp)
{
    QString ReqName = "LiquidTubeStartHeating";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(TargetTemp));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    return Ret;
}

int ServiceDeviceProcess::LiquidTubeStopHeating()
{
    QString ReqName = "LiquidTubeStopHeating";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;

    return Ret;
}

int ServiceDeviceProcess::LiquidTubeGetTemp(qreal *RetTemp)
{
    QString ReqName = "LiquidTubeGetTemp";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetTemp = Results.at(0).toFloat();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::LiquidTubeGetCurrent(quint16 *RetCurrent)
{
    QString ReqName = "LiquidTubeGetCurrent";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetCurrent = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}


int ServiceDeviceProcess::AirTubeStartHeating(qreal TargetTemp)
{
    QString ReqName = "AirTubeStartHeating";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(TargetTemp));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    return Ret;
}

int ServiceDeviceProcess::AirTubeStopHeating()
{
    QString ReqName = "AirTubeStopHeating";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;

    return Ret;
}

int ServiceDeviceProcess::AirTubeGetTemp(qreal *RetTemp)
{
    QString ReqName = "AirTubeGetTemp";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetTemp = Results.at(0).toFloat();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::AirTubeGetCurrent(quint16 *RetCurrent)
{
    QString ReqName = "AirTubeGetCurrent";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetCurrent = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}


int ServiceDeviceProcess::RVStartHeating(qreal TargetTemp)
{
    QString ReqName = "RVStartHeating";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(TargetTemp));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RVStopHeating()
{
    QString ReqName = "RVStopHeating";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RVGetTemp(qreal *RetTempSensor1, qreal* RetTempSensor2)
{
    QString ReqName = "RVGetTemp";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        if (RetTempSensor1) {
            *RetTempSensor1 = Results.at(0).toFloat();
        }
        if (RetTempSensor2) {
            *RetTempSensor2 = Results.at(0).toFloat();
        }
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RVGetCurrent(quint16 *RetCurrent)
{
    QString ReqName = "RVGetCurrent";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetCurrent = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RVInitialize()
{
    QString ReqName = "RVInitialize";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RVMovePosition(bool TubeFlag, int Position)
{
    QString ReqName = "RVMovePosition";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(TubeFlag));
    Params.append(QString("%1").arg(Position));


    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    return Ret;
}

int ServiceDeviceProcess::RVSetTemperatureSwitchState(qint8 SwitchState, qint8 AutoSwitch)
{
    QString ReqName = "RVSetTemperatureSwitchState";
    QStringList Params;
    Params.clear();
    Params.append(QString("%1").arg(SwitchState));
    Params.append(QString("%1").arg(AutoSwitch));

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;

    return Ret;
}

int ServiceDeviceProcess::RVGetHeaterSwitchType(quint8 *RetSwitchType)
{
    QString ReqName = "RVGetHeaterSwitchType";
    QStringList Params;
    Params.clear();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName);

    qDebug()<<"Ret = "<<Ret;
    if (Ret == RESPONSE_TIMEOUT) {
        return Ret;
    }

    QStringList Results = m_ResultsMap.value(ReqName);
    qDebug()<<"Results = "<<Results;

    if (Results.size()>0) {
        *RetSwitchType = Results.at(0).toInt();
    }
    m_ResultsMap.remove(ReqName);

    return Ret;
}

int ServiceDeviceProcess::LSStartHeating(bool QuickFlag, bool WaterFlag)
{

}

int ServiceDeviceProcess::LSStopHeating()
{

}

int ServiceDeviceProcess::LSGetTemp(qreal *RetTemp)
{

}

int ServiceDeviceProcess::LSGetCurrent(qreal *RetCurrent)
{

}


int ServiceDeviceProcess::PumpBuildPressure(float TargetPressure)
{

}

int ServiceDeviceProcess::PumpReleasePressure()
{

}

int ServiceDeviceProcess::PumpGetPressure(float *RetPressure)
{

}

int ServiceDeviceProcess::PumpSetFan(bool OnFlag)
{

}

int ServiceDeviceProcess::PumpSetValve(quint8 ValveIndex, quint8 ValveState)
{

}

int ServiceDeviceProcess::PumpStopCompressor()
{

}

int ServiceDeviceProcess::PumpSucking(int DelayTime)
{

}

int ServiceDeviceProcess::PumpDraining()
{

}

int ServiceDeviceProcess::GetSlaveModuleReportError(quint8 ErrorCode, const QString &DevName, quint32 SensorName)
{
    QString ReqName = "GetSlaveModuleReportError";
    QStringList Params;
    Params.clear();

    Params.append(QString("%1").arg(ErrorCode));
    Params.append(QString("%1").arg(DevName));
    Params.append(QString("%1").arg(SensorName));

    qint64 now1 = QDateTime::currentMSecsSinceEpoch();

    emit SendServRequest(ReqName, Params);

    int Ret = GetResponse(ReqName, 20);

    qint64 now2 = QDateTime::currentMSecsSinceEpoch();

    qDebug()<<"ServiceDeviceProcess::GetSlaveModuleReportError delay="<<(now2-now1)<<"   Ret="<<Ret;

    return Ret;
}

void ServiceDeviceProcess::Pause(quint32 MilliSeconds)
{
    Core::CServiceUtils::delay(MilliSeconds);
}

void ServiceDeviceProcess::HandleServResult(QString ReqName, int Error, QStringList Results)
{
    if (Results.size()>0) {
        m_ResultsMap.insert(ReqName, Results);
    }

    qDebug()<<"ServiceDeviceProcess::HandleServResult req="<<ReqName<<" Error="<<Error;

#if 1
    QEventLoop *loop = m_EventLoopMap.value(ReqName);

    if (loop && loop->isRunning()) {
        loop->exit(Error);
        m_EventLoopMap.remove(ReqName);

    }
#else
    if (m_EventLoop.isRunning()) {
        m_EventLoop.exit(Error);
    }
#endif

}

int ServiceDeviceProcess::GetResponse(QString ReqName, int TimeoutSeconds)
{
    QTimer timer;
    qint32 ret;
    quint32 interval = 1000 * TimeoutSeconds; // 10 sec.
    timer.setSingleShot(true);
    timer.setInterval(interval);
    timer.start();

#if 1
    QEventLoop *loop = new QEventLoop();
    m_EventLoopMap.insert(ReqName, loop);
    CONNECTSIGNALSLOT(&timer, timeout(), loop, quit());
    ret = loop->exec();

    delete loop;
#else
    CONNECTSIGNALSLOT(&timer, timeout(), &m_EventLoop, quit());
    ret = m_EventLoop.exec();

    qDebug()<<"ServiceDeviceProcess::GetResponse -- ret = "<<ret;
#endif

    return ret;
}

} // end namespace Diagnostics
