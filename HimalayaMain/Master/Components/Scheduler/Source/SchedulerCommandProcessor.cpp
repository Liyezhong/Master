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


SchedulerCommandProcessor::SchedulerCommandProcessor(DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller) :
    m_IDP(IDP),
    mp_SchedulerThreadController(controller)
{
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


}// end of namespace Scheduler
