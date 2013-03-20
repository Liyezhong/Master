/****************************************************************************/
/*! \file CmdUpdateRmsStatusToGui.h
 *
 *  \brief CmdUpdateRmsStatusToGui command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 05.11.2012
 *   $Author:  $ Ningu
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MSGCLASSES_CMDUPDATERMSSTATUSTOGUI_H
#define MSGCLASSES_CMDUPDATERMSSTATUSTOGUI_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {

typedef enum
{
    NOT_SCANNING,       //!< Scanning indicator
    KIT_SCANNING,       //!< Kit scanning indicator
    BOTTLE_SCANNING     //!< Bottle scanning indicator
} ConsumableScanType_t;
typedef enum
{
    CONSUMABLE_RFID_SCAN_ERROR,
    KIT_SCAN_TIMEOUT,
    CONSUMABLES_ALREADY_USED,
    KIT_BEST_BEFORE_DATE_EXPIRED,
    INVALID_KIT_PROGRAM_ID,
    KIT_DATA_ERROR,
    KIT_SCAN_STATUS_OK,
    BOTTLE_SCAN_TIMEOUT,    
    BOTTLE_DATA_ERROR,
    BOTTLE_SCAN_STATUS_OK,
    BOTTLE_SCAN_COMPLETE,
    CONFIRM_LEICA_REAGENT_STATUS_OK,
    CONFIRM_LEICA_REAGENT_FAILED,
    CONFIRM_BATHLAYOUT_STATUS_OK,
    CONFIRM_BATHLAYOUT_FAILED,   
    PROG_SEQ_BLG_TEMP_WRITE_FAILED,
    RESET_USER_RMS_OK,
    RESET_USER_RMS_FAILED,
    UPDATE_LEICA_REAGENT_STATUS_OK,
    UPDATE_LEICA_REAGENT_FAILED
} RmsStatus_t;
/****************************************************************************/
/*!
 *  \brief  This class implements a CmdUpdateRmsStatusToGui command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdUpdateRmsStatusToGui : public Global::Command {
public:
    static QString NAME;    ///< Command name.
    friend QDataStream & operator << (QDataStream &, const CmdUpdateRmsStatusToGui &);
    friend QDataStream & operator >> (QDataStream &, CmdUpdateRmsStatusToGui &);
    /****************************************************************************/
    CmdUpdateRmsStatusToGui(int Timeout,const RmsStatus_t RmsStatus,qint32 BottleCount,QString LeicaReagentId);
    CmdUpdateRmsStatusToGui();
    ~CmdUpdateRmsStatusToGui();
    virtual QString GetName() const;
    qint32 GetRmsBottleCount() const;
    RmsStatus_t GetRmsStatusMsgId() const;
    QString GetRmsLeicaReagentId() const;
private:
    CmdUpdateRmsStatusToGui(const CmdUpdateRmsStatusToGui &);                     ///< Not implemented.
    const CmdUpdateRmsStatusToGui & operator = (const CmdUpdateRmsStatusToGui &); ///< Not implemented.
private:     
     RmsStatus_t m_RmsStatus;     ///< Rms status message identifier
     qint32 m_BottleCount;        ///< Bottle count
     QString m_LeicaReagentId;    ///< Bottle Reagent Id
}; // end class CmdUpdateRmsStatusToGui
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdUpdateRmsStatusToGui &Cmd)
{

    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << (int)Cmd.m_RmsStatus <<Cmd.m_BottleCount<< Cmd.m_LeicaReagentId;
    return Stream;

}

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream from.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator >> (QDataStream &Stream, CmdUpdateRmsStatusToGui &Cmd)
{
    qint32 MessageID;
    // copy base class data
    Cmd.CopyFromStream(Stream);
    Stream >> MessageID;
    Cmd.m_RmsStatus = (RmsStatus_t)MessageID;
    Stream >> Cmd.m_BottleCount;   
    Stream >> Cmd.m_LeicaReagentId;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDUPDATERMSSTATUSTOGUI_H

