/****************************************************************************/
/*! \file CmdHeatingTest.h
 *
 *  \brief Definition file for class CmdHeatingTest.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-06-18
 *  $Author:    $ Brandon Shao
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai R&D Center.
 *
 *  (C) Copyright 2013 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef DEVICECOMMANDPROCESSOR_CMDHEATINGTEST_H
#define DEVICECOMMANDPROCESSOR_CMDHEATINGTEST_H

#include <Global/Include/Commands/Command.h>
#include <DeviceControl/Include/Global/DeviceControlGlobal.h>

namespace DeviceCommandProcessor {

/****************************************************************************/
/**
 * \brief This command would be sent when ___________________________________
 *
 * \warning This class is not thread safe!
 */
/****************************************************************************/
class CmdHeatingTest : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdHeatingTest();                                            ///< Not implemented.
    CmdHeatingTest(const CmdHeatingTest &);                       ///< Not implemented.
    const CmdHeatingTest & operator = (const CmdHeatingTest &);   ///< Not implemented.

protected:
public:
    static QString  NAME;                                   ///< Command name.

    DeviceControl::DevInstanceID_t m_DevInstanceID;        ///< Instance ID of the target device
    quint8  m_HeaterIndex;                                 ///< Heater index of certain device
    quint8  m_CmdType;                                     ///< Heating command type
    quint32 m_TargetTemp;                                  ///< Target temperature
    quint32 m_HeatingSeconds;                              ///< Heating time in seconds
    quint32 m_Tolerance;                                   ///< Temperature tolerance

#if 0
    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \param[in]   DevInstanceID       Instance ID of the concerned device.
     */
    /****************************************************************************/
    CmdHeatingTest(DeviceControl::DevInstanceID_t DevInstanceID, quint32 TargetTemp,
                  quint32 Tolerance, quint32 HeatingSeconds, quint8 HeaterIndex);
#endif
    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \param[in]   DevInstanceID       Instance ID of the concerned device.
     */
    /****************************************************************************/
    CmdHeatingTest(DeviceControl::DevInstanceID_t DevInstanceID, quint8 HeaterIndex,
                   quint8 CmdType);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdHeatingTest();
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
}; // end class CmdHeatingTest

} // end namespace DeviceCommandProcessor
#endif // DEVICECOMMANDPROCESSOR_CMDHEATINGTEST_H
