/****************************************************************************/
/*! \file CmdCalibrateDevice.h
 *
 *  \brief Definition file for class CmdCalibrateDevice.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-18
 *  $Author:    $ Srivathsa HH
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2011 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef DEVICECOMMANDPROCESSOR_CMDCALIBRATEDEVICE_H
#define DEVICECOMMANDPROCESSOR_CMDCALIBRATEDEVICE_H

#include <Global/Include/Commands/Command.h>
//#include <DeviceControl/Include/Global/DeviceControl.h>
#include "Core/Include/ServiceDefines.h"

namespace DeviceCommandProcessor {

/****************************************************************************/
/**
 * \brief This command would be sent when ___________________________________
 *
 * \warning This class is not thread safe!
 */
/****************************************************************************/
class CmdCalibrateDevice : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdCalibrateDevice();                                                    ///< Not implemented.
    CmdCalibrateDevice(const CmdCalibrateDevice &);                          ///< Not implemented.

    /****************************************************************************/
    /**
      * \brief Constructor - Not implemented
      * \iparam   CalibrateDevice = CmdCalibrateDevice object
      * \return
      */
    /****************************************************************************/
    const CmdCalibrateDevice & operator = (const CmdCalibrateDevice &CalibrateDevice);      ///< Not implemented.

protected:
public:
    static QString  NAME;                                   ///< Command name.

    Service::DeviceCalibrationCmdType m_CommandType;        ///< Instance ID of the target device
    quint32 m_RackId;                                       ///< Rack RFID
    quint32 m_RackUserData;                                 ///< Rack user data providing information such as color
    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \iparam  DeviceCommand = Device command
     */
    /****************************************************************************/
    CmdCalibrateDevice(Service::DeviceCalibrationCmdType DeviceCommand);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdCalibrateDevice();
    /****************************************************************************/
    /**
     * \brief Get command name.
     *
     * \return  Command name.
     */
    /****************************************************************************/
    virtual QString GetName() const {
        return NAME;
    }
}; // end class CmdCalibrateDevice

} // end namespace DeviceCommandProcessor
#endif // DEVICECOMMANDPROCESSOR_CMDCALIBRATEDEVICE_H
