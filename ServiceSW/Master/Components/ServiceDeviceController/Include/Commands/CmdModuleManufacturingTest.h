/****************************************************************************/
/*! \file CmdModuleEndTest.h
 *
 *  \brief Definition file for class CmdModuleManufacturingTest.
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
#ifndef DEVICECOMMANDPROCESSOR_CMDMODULEMANUFACTURINGTEST_H
#define DEVICECOMMANDPROCESSOR_CMDMODULEMANUFACTURINGTEST_H

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
class CmdModuleManufacturingTest : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdModuleManufacturingTest();                                                    ///< Not implemented.
    CmdModuleManufacturingTest(const CmdModuleManufacturingTest &);                       ///< Not implemented.

    /****************************************************************************/
    /**
      * \brief Constructor - Not implemented
      * \iparam ModuleManufacturingTest = CmdModuleManufacturingTest object
      * \return
      */
    /****************************************************************************/
    const CmdModuleManufacturingTest & operator = (const CmdModuleManufacturingTest &ModuleManufacturingTest);   ///< Not implemented.

protected:
public:
    static QString  NAME;                                   ///< Command name.

    Service::ModuleTestCaseID m_CommandType;              ///< Type of end test requested
    Service::ModuleTestCaseID m_AbortTestCaseId;
    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \iparam CmdType = Command type
     */
    /****************************************************************************/
    CmdModuleManufacturingTest(Service::ModuleTestCaseID CmdType, Service::ModuleTestCaseID AbortId);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdModuleManufacturingTest();
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
}; // end class CmdModuleManufacturingTest

} // end namespace DeviceCommandProcessor
#endif // DEVICECOMMANDPROCESSOR_CMDMODULEMANUFACTURINGTEST_H
