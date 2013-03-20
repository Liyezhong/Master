/****************************************************************************/
/*! \file CmdScanLeicaKit.h
 *
 *  \brief CmdScanLeicaKit command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02.10.2012
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

#ifndef MSGCLASSES_CMDSCANLEICAKIT_H
#define MSGCLASSES_CMDSCANLEICAKIT_H

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdScanLeicaKit command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdScanLeicaKit : public Global::Command {
public:
    static QString NAME;    //!< Command name.
    friend QDataStream & operator << (QDataStream &, const CmdScanLeicaKit &);
    friend QDataStream & operator >> (QDataStream &, CmdScanLeicaKit &);
    /****************************************************************************/
    CmdScanLeicaKit(int Timeout, const bool IsKitScanViaBlg,const QString &NeedAutoHide);
    CmdScanLeicaKit();
    ~CmdScanLeicaKit();
    virtual QString GetName() const;    //!< Command name.
    QString GetAutoHideStatus() const;  //!< Gets auto hide status
    bool GetKitScanFlag() const;        //!< Returns Kit scan sequence flag: Scan via Blg = true
private:                                               
    CmdScanLeicaKit(const CmdScanLeicaKit &);                     ///< Not implemented.
    const CmdScanLeicaKit & operator = (const CmdScanLeicaKit &); ///< Not implemented.
private:
    bool m_IsKitScanViaBlg;       //!< Kit scan sequence via BLG or Update Reagent".
    QString     m_AutoHide;       //!< Auto hide reuired or not "yes" or "no".
}; // end class CmdScanLeicaKit
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdScanLeicaKit &Cmd)
{
     // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_IsKitScanViaBlg;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdScanLeicaKit &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    Stream >> Cmd.m_IsKitScanViaBlg;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSCANLEICAKIT_H
