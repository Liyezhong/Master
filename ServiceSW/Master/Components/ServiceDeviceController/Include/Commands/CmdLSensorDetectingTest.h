/****************************************************************************/
/*! \file CmdLSensorDetectingTest.h
 *
 *  \brief Definition file for class CmdLSensorDetectingTest.
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
#ifndef DEVICECOMMANDPROCESSOR_CMDLSENSORDETECTINGTEST_H
#define DEVICECOMMANDPROCESSOR_CMDLSENSORDETECTINGTEST_H

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
class CmdLSensorDetectingTest : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdLSensorDetectingTest();                                                 ///< Not implemented.
    CmdLSensorDetectingTest(const CmdLSensorDetectingTest &);                       ///< Not implemented.
    const CmdLSensorDetectingTest & operator = (const CmdLSensorDetectingTest &);   ///< Not implemented.

protected:
public:
    static QString  NAME;                                   ///< Command name.

    DeviceControl::DevInstanceID_t m_DevInstanceID;        ///< Instance ID of the target device
    qint32  m_Position;                                    ///< Tube position

    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \param[in]   DevInstanceID       Instance ID of the concerned device.
     */
    /****************************************************************************/
    CmdLSensorDetectingTest(DeviceControl::DevInstanceID_t DevInstanceID, qint32 Position);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdLSensorDetectingTest();
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
}; // end class CmdLSensorDetectingTest

} // end namespace DeviceCommandProcessor
#endif // DEVICECOMMANDPROCESSOR_CMDLSENSORDETECTINGTEST_H
