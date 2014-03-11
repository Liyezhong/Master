/****************************************************************************/
/*! \file CmdGetDataContainers.h
 *
 *  \brief Definition file for class CmdGetDataContainers.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-04
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
#ifndef COLORADODEVICECOMMANDPROCESSOR_CMDGETDATACONTAINERS_H
#define COLORADODEVICECOMMANDPROCESSOR_CMDGETDATACONTAINERS_H

#include <Global/Include/Commands/Command.h>
#include <DeviceControl/Include/Global/DeviceControlGlobal.h>

namespace DeviceCommandProcessor {

/****************************************************************************/
/**
 * \brief This command would be sent _______________________________________
 *
 * \warning This class is not thread safe!
 */
/****************************************************************************/
class CmdGetDataContainers : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdGetDataContainers(const CmdGetDataContainers &);                       ///< Not implemented.
    const CmdGetDataContainers & operator = (const CmdGetDataContainers &);   ///< Not implemented.
protected:
public:
    static QString  NAME;           ///< Command name.

    QString m_ReturnMessage;   ///< Instance ID of the target device
    qint32 m_DeviceId;        ///< Device ID for which module info is requested

    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     */
    /****************************************************************************/
    CmdGetDataContainers();
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdGetDataContainers();
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
}; // end class CmdGetDataContainers

} // end namespace DeviceCommandProcessor
#endif // COLORADODEVICECOMMANDPROCESSOR_CMDGETDATACONTAINERS_H
