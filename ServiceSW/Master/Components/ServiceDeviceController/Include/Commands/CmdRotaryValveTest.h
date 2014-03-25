/****************************************************************************/
/*! \file CmdRotaryValveTest.h
 *
 *  \brief Definition file for class CmdRotaryValveTest.
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
#ifndef DEVICECOMMANDPROCESSOR_CMDROTARYVALVETEST_H
#define DEVICECOMMANDPROCESSOR_CMDROTARYVALVETEST_H

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
class CmdRotaryValveTest : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdRotaryValveTest();                                                 ///< Not implemented.
    CmdRotaryValveTest(const CmdRotaryValveTest &);                       ///< Not implemented.
    const CmdRotaryValveTest & operator = (const CmdRotaryValveTest &);   ///< Not implemented.

protected:
public:
    static QString  NAME;                                   ///< Command name.

    quint32 m_DevInstanceID;        ///< Instance ID of the target device
    qint32  m_Position;                                    ///< Tube/Sealing position
    quint8  m_CmdType;                                     ///< Rotary valve command type

    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \param[in]   DevInstanceID       Instance ID of the concerned device.
     */
    /****************************************************************************/
    CmdRotaryValveTest(quint32 DevInstanceID, qint32 Position,
                   quint8 CmdType);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdRotaryValveTest();
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
}; // end class CmdRotaryValveTest

} // end namespace DeviceCommandProcessor
#endif // DEVICECOMMANDPROCESSOR_CMDROTARYVALVETEST_H
