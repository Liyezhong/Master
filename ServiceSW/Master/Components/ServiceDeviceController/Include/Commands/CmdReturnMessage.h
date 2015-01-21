/****************************************************************************/
/*! \file CmdReturnMessage.h
 *
 *  \brief Definition file for class CmdReturnMessage.
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
#ifndef COLORADODEVICECOMMANDPROCESSOR_CMDRETURNMESSAGE_H
#define COLORADODEVICECOMMANDPROCESSOR_CMDRETURNMESSAGE_H

#include <QStringList>
#include <Global/Include/Commands/Command.h>
#include <DeviceControl/Include/Global/DeviceControlGlobal.h>
#include "Core/Include/ServiceDefines.h"

namespace DeviceCommandProcessor {

/****************************************************************************/
/**
 * \brief This command would be sent _______________________________________
 *
 * \warning This class is not thread safe!
 */
/****************************************************************************/
class CmdReturnMessage : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdReturnMessage();                                                    ///< Not implemented.
    CmdReturnMessage(const CmdReturnMessage &);                       ///< Not implemented.
    const CmdReturnMessage & operator = (const CmdReturnMessage &);   ///< Not implemented.
protected:
public:
    static QString  NAME;               ///< Command name.

    QString m_ReturnMessage;            ///< Instance ID of the target device
    qint32  m_DeviceId;                 ///< Device ID for which module info is requested

    Service::GUIReturnMessageType  m_MessageType;   ///< Type of message

    QString m_ReadCanID;                ///< CAN ID for the module requested.
    QString m_ExpectedCanID;            ///< Expected CAN ID for the module requested.

    QString m_FirwmwareInfo;            ///< Firmware Info for the module requested.

    QString m_ReadRackRFID;             ///< Rack RFID Value.
    QString m_ExpectedRackRFID;         ///< Expected Rack RFID Value.

    QStringList m_ReadDrawerRFID;        ///< Drawer RFID Values 0, 1, 2, 3, 4.
    QStringList m_ExpectedDrawerRFID;    ///< Expected Drawer RFID Values.

    QString m_FanCurrent;               ///< Fan Current value.
    bool    m_AirFlowStatus;            ///< Air Flow status

    bool    m_HoodStatus;               ///< Hood Status

    QStringList m_KitInfo;               ///< Kit information
    QStringList m_BottleInfo;            ///< Bottle information

    QString m_LiquidLevelFull;          ///< Full liquid level information
    QString m_LiquidLevelEmpty;         ///< Empty liquid level information

    QString m_RefLiquidLevelFull;       ///< Full liquild level ref information
    QString m_RefLiquidLevelEmpty;      ///< Empty liquid level ref information

    bool m_CalibStatus; ///< calibration status
    Service::ModuleTestStatus m_Status; ///< store test status
    bool m_ModuleTestResult; ///< module test status(true/false)

    QString m_ServReqName;                ///< Service Request Name
    QStringList m_ServResults;                ///< Service Request results
    int     m_ServErrorCode;                ///< Service Request error code

    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     * \param  ReturnMessage = message
     */
    /****************************************************************************/
    CmdReturnMessage(QString ReturnMessage);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdReturnMessage();
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
}; // end class CmdReturnMessage

} // end namespace DeviceCommandProcessor
#endif // COLORADODEVICECOMMANDPROCESSOR_CMDRETURNMESSAGE_H
