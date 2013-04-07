// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Source/SchedulerCommandProcessor.cpp
 *
 *  @brief
 *  Implementation of class Scheduler::SchedulerCommandProcessor.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-10
 *  $Author:    $ L. Yang
 *
 *  @b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 * ================================================================================================
*/


#include "../Include/SchedulerCommandProcessor.h"
#include "../Include/SchedulerMainThreadController.h"

namespace Scheduler{


SchedulerCommandProcessor::SchedulerCommandProcessor(SchedulerMainThreadController *controller) :
    mp_SchedulerThreadController(controller)
{

    qRegisterMetaType<ReturnCode_t>("ReturnCode_t");
    qRegisterMetaType<DevInstanceID_t>("DevInstanceID_t");

    CONNECTSIGNALSLOT(&m_IDeviceProcessing, ReportInitializationFinished(DevInstanceID_t, ReturnCode_t),
                      this, DevProcInitialisationAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(&m_IDeviceProcessing, ReportConfigurationFinished(DevInstanceID_t, ReturnCode_t),
                      this, DevProcConfigurationAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(&m_IDeviceProcessing, ReportStartNormalOperationMode(DevInstanceID_t, ReturnCode_t),
                      this, DevProcStartNormalOpModeAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(&m_IDeviceProcessing, ReportError(DevInstanceID_t, quint16, quint16, quint16, const QDateTime &),
                      this, ThrowError(DevInstanceID_t, quint16, quint16, quint16, const QDateTime &));
    CONNECTSIGNALSLOT(&m_IDeviceProcessing, ReportDestroyFinished(), this, DevProcDestroyAckn());

}

void SchedulerCommandProcessor::run()
{
    while(true)
    {
        if(newCmdComing())
        {
            QString cmdname = m_currentCmd->GetPointerToUserData()->GetName();
            bool result = false;
            //TBD
            emit onCmdFinished(m_currentCmd,result);
        }
    }

}

void SchedulerCommandProcessor::pushCmd(Global::CommandShPtr_t *cmd)
{
    m_CmdMutex.lock();
    m_Cmds.push_front(cmd);
    m_CmdMutex.unlock();
}

bool SchedulerCommandProcessor::newCmdComing()
{
    bool ret = false;
    m_CmdMutex.lock();
    if(!m_Cmds.isEmpty())
    {
        ret = true;
       m_currentCmd =  m_Cmds.dequeue();
    }
    m_CmdMutex.unlock();
    return ret;
}
void SchedulerCommandProcessor::DevProcInitialisationAckn(DevInstanceID_t instanceID, ReturnCode_t configResult)
{
    // interface implementation runs in DeviceControl-Thread
    ReturnCode_t retCode;

    qDebug()<< "Configurating DeviceControl Layer...";

    qDebug() << "  CApplicationControl::DevProcInitialisationAckn" << " " << instanceID << " " << configResult;

    QString SerialNo;
    if (IDeviceProcessing::GetSerialNumber(SerialNo)) {
        qDebug() << "  Serial number is " << SerialNo;
    } else {
        qDebug() << "  Error: getting serial number failed.";
    }

    retCode = m_IDeviceProcessing.StartConfigurationService();
    if(retCode != DCL_ERR_FCT_CALL_SUCCESS)
    {
        qDebug() << "  Error starting configuration service: " << retCode;
    }
}
void SchedulerCommandProcessor::DevProcConfigurationAckn(DevInstanceID_t instanceID, ReturnCode_t hdlInfo)
{
    // interface implementation runs in DeviceControl-Thread
    qDebug() << "  SchedulerCommandProcessor::DevProcConfigurationAckn" << instanceID << "ReturnCode" << hdlInfo;
    if((hdlInfo == DCL_ERR_FCT_CALL_SUCCESS)|| (hdlInfo == DCL_ERR_TIMEOUT))
    {
        emit DCLConfigurationFinished(hdlInfo, &m_IDeviceProcessing);
    }
}
void SchedulerCommandProcessor::DevProcStartNormalOpModeAckn(DevInstanceID_t instanceID, ReturnCode_t hdlInfo)
{
    qDebug() << "  SchedulerCommandProcessor::DevProcStartNormalOpModeAckn " << instanceID << " " << hdlInfo;
}
void SchedulerCommandProcessor::ThrowError(DevInstanceID_t instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp)
{
    // Platform/Master/Components/DeviceControl/Include/Global/
    //   DeviceControlError.h
    //   DeviceControlGlobal.h
    //WriteToLog(
     //                       QString() + "Thrown DCL Error:" +
     //                       "  Module:" +  QString().setNum(instanceID, 16) +
     //                       ", Group:"  +  QString().setNum(usErrorGroup, 16) +
     //                       ", ID:"     +  QString().setNum(usErrorID, 16) +
     //                       ", Data:"   +  QString().setNum(usErrorData, 16) +
     //                       ", TimeStamp:" + TimeStamp.toString()
     //                       );
}
void SchedulerCommandProcessor::DevProcDestroyAckn()
{
}
}// end of namespace Scheduler
