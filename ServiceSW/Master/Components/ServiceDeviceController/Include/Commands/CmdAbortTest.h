/****************************************************************************/
/*! \file CmdAbortTest.h
 *
 *  \brief Definition file for class CmdAbortTest.
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
#ifndef DEVICECOMMANDPROCESSOR_CMDABORTTEST_H
#define DEVICECOMMANDPROCESSOR_CMDABORTTEST_H

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
class CmdAbortTest : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdAbortTest();                                            ///< Not implemented.
    CmdAbortTest(const CmdAbortTest &);                       ///< Not implemented.
    const CmdAbortTest & operator = (const CmdAbortTest &);   ///< Not implemented.

protected:
public:
    static QString  NAME;                                   ///< Command name.

    DeviceControl::DevInstanceID_t m_DevInstanceID;        ///< Instance ID of the target device

    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \param[in]   DevInstanceID       Instance ID of the concerned device.
     */
    /****************************************************************************/
    CmdAbortTest(DeviceControl::DevInstanceID_t DevInstanceID);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdAbortTest();
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
}; // end class CmdAbortTest

} // end namespace DeviceCommandProcessor
#endif // DEVICECOMMANDPROCESSOR_CMDABORTTEST_H
