/****************************************************************************/
/*! \file CmdServiceTest.h
 *
 *  \brief Definition file for class CmdServiceTest.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-09-19
 *  $Author:    $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef DEVICECOMMANDPROCESSOR_CMDSERVICETEST_H
#define DEVICECOMMANDPROCESSOR_CMDSERVICETEST_H

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
class CmdServiceTest : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdServiceTest();                                                    ///< Not implemented.
    CmdServiceTest(const CmdServiceTest &);                       ///< Not implemented.

    /****************************************************************************/
    /**
      * \brief Constructor - Not implemented
      * \iparam ModuleManufacturingTest = CmdServiceTest object
      * \return
      */
    /****************************************************************************/
    const CmdServiceTest & operator = (const CmdServiceTest &cmdServiceTest);   ///< Not implemented.

protected:
public:
    static QString  NAME;                                   ///< Command name.

    QString m_ReqName;
    QStringList m_Params;

    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \iparam CmdType = Command type
     */
    /****************************************************************************/
    CmdServiceTest(QString ReqName, QStringList Params);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdServiceTest();
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
}; // end class CmdServiceTest

} // end namespace DeviceCommandProcessor
#endif // DEVICECOMMANDPROCESSOR_CMDSERVICETEST_H
