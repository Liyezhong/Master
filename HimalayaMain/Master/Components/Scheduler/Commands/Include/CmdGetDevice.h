/****************************************************************************/
/*! \file CmdGetDevice.h
 *
 *  \brief CmdGetDevice command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 29.03.2013
 *   $Author:  $ L. Yang
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


#ifndef CMDGETDEVICE_H
#define CMDGETDEVICE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdGetDevice : public CmdSchedulerCommandBase
{
public:
    CmdGetDevice(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdGetDevice();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::CBaseDevice* GetResult()const{return m_result;}
	bool GetResult(DeviceControl::CBaseDevice*& result) const{result = m_result; return true;}
	DeviceControl::DevInstanceID_t GetInstanceID(){return m_InstanceID;}
	void SetInstanceID(DeviceControl::DevInstanceID_t InstanceID){m_InstanceID = InstanceID;}
	

    void Execute();

private:
    CmdGetDevice();                                                    ///< Not implemented.
    CmdGetDevice(const CmdGetDevice &);                     ///< Not implemented.
    const CmdGetDevice & operator = (const CmdGetDevice &); ///< Not implemented.


	mutable DeviceControl::CBaseDevice* m_result;
	mutable DeviceControl::DevInstanceID_t m_InstanceID;
	
};



}

#endif // CMDGETDEVICE_H
