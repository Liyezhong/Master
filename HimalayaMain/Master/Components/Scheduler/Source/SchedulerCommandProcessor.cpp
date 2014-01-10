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
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"

namespace Scheduler{


SchedulerCommandProcessor::SchedulerCommandProcessor(SchedulerMainThreadController *controller) :
    mp_SchedulerThreadController(controller)
{


}

void SchedulerCommandProcessor::run()
{
    mp_IDeviceProcessing = new IDeviceProcessing();
    qRegisterMetaType<ReturnCode_t>("ReturnCode_t");
    qRegisterMetaType<DevInstanceID_t>("DevInstanceID_t");

    //connect(mp_IDeviceProcessing, SIGNAL(ReportInitializationFinished(DevInstanceID_t, ReturnCode_t)), this, SLOT(DevProcInitialisationAckn(DevInstanceID_t, ReturnCode_t)), Qt::QueuedConnection);
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportInitializationFinished(DevInstanceID_t, ReturnCode_t),
                      this, DevProcInitialisationAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportConfigurationFinished(DevInstanceID_t, ReturnCode_t),
                      this, DevProcConfigurationAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportStartNormalOperationMode(DevInstanceID_t, ReturnCode_t),
                      this, DevProcStartNormalOpModeAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportError(DevInstanceID_t, quint16, quint16, quint16, const QDateTime &),
                      this, ThrowError(DevInstanceID_t, quint16, quint16, quint16, const QDateTime &));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportDestroyFinished(), this, DevProcDestroyAckn());

    CONNECTSIGNALSLOT(this, NewCmdAdded(), this, OnNewCmdAdded());
    /*CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportConfigurationFinished(DevInstanceID_t, ReturnCode_t),
                      this, DevProcConfigurationAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportStartNormalOperationMode(DevInstanceID_t, ReturnCode_t),
                      this, DevProcStartNormalOpModeAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportError(DevInstanceID_t, quint16, quint16, quint16, const QDateTime &),
                      this, ThrowError(DevInstanceID_t, quint16, quint16, quint16, const QDateTime &));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportDestroyFinished(), this, DevProcDestroyAckn());
    */


    /*
        while(true)
        {
    qDebug()<< "this is second thread, id is: "<<QThread::currentThreadId();
            if(newCmdComing())
            {
        qDebug()<< "sec thread got msg! current thread id is: "<<QThread::currentThreadId();
                CmdSchedulerCommandBase *scmd = dynamic_cast<CmdSchedulerCommandBase *> (m_currentCmd.GetPointerToUserData());
                scmd->Execute();
    //            (&m_currentCmd)->Execute();
            }
        usleep(200*1000);
        }
    */
}
void SchedulerCommandProcessor::OnNewCmdAdded()
{
    if(newCmdComing())
    {
        //qDebug()<< "sec thread got msg! current thread id is: "<<QThread::currentThreadId();
//        LOG_STR_ARG(STR_EXECUTE_COMMAND,Global::tTranslatableStringList()<<Global::TranslatableString(m_currentCmd->GetParameters()));
        m_currentCmd->Execute();
        mp_SchedulerThreadController->PushDeviceControlCmdQueue(m_currentCmd);
//        LOG_STR_ARG(STR_EXECUTE_COMMAND_RESULT,Global::tTranslatableStringList()<<Global::TranslatableString(m_currentCmd->GetName())
//                    <<Global::TranslatableString(m_currentCmd->GetStrResult()));
    }
}
void SchedulerCommandProcessor::pushCmd(CmdSchedulerCommandBase* cmd)
{
    m_CmdMutex.lock();
    m_Cmds.push_front(Scheduler::SchedulerCommandShPtr_t(cmd));
    m_CmdMutex.unlock();
    emit NewCmdAdded();
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

    retCode = mp_IDeviceProcessing->StartConfigurationService();
    if(retCode != DCL_ERR_FCT_CALL_SUCCESS)
    {
        qDebug() << "  Error starting configuration service: " << retCode;
    }
}

void SchedulerCommandProcessor::DevProcConfigurationAckn(DevInstanceID_t instanceID, ReturnCode_t hdlInfo)
{
    // interface implementation runs in DeviceControl-Thread
    qDebug() << "  SchedulerCommandProcessor::DevProcConfigurationAckn" << instanceID << "ReturnCode" << hdlInfo;
    if(hdlInfo == DCL_ERR_FCT_CALL_SUCCESS)
    {
      //successfully configured the devices
    }
    else
    {
        //some error happens
    }
    emit DCLConfigurationFinished(hdlInfo, mp_IDeviceProcessing);
}

void SchedulerCommandProcessor::DevProcStartNormalOpModeAckn(DevInstanceID_t instanceID, ReturnCode_t hdlInfo)
{
    qDebug() << "  SchedulerCommandProcessor::DevProcStartNormalOpModeAckn " << instanceID << " " << hdlInfo;
}

void SchedulerCommandProcessor::ThrowError(DevInstanceID_t instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp)
{
    Q_UNUSED(instanceID)
    Q_UNUSED(usErrorGroup)
    Q_UNUSED(usErrorID)
    Q_UNUSED(usErrorData)
    Q_UNUSED(TimeStamp)
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
