/****************************************************************************/
/*! \file CmdScanLeicaBottle.h
 *
 *  \brief CmdScanLeicaBottle command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 21.08.2012
 *   $Author:  $ Vikram MK
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

#ifndef MSGCLASSES_CMDSCANLEICABOTTLE_H
#define MSGCLASSES_CMDSCANLEICABOTTLE_H

#include <Global/Include/Commands/Command.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdScanLeicaBottle command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdScanLeicaBottle : public Global::Command {
public:
    static QString NAME;    ///< Command name.
    friend QDataStream & operator << (QDataStream &, const CmdScanLeicaBottle &);
    friend QDataStream & operator >> (QDataStream &, CmdScanLeicaBottle &);
    /****************************************************************************/
    CmdScanLeicaBottle(int Timeout, const QString &NeedAutoHide);
    CmdScanLeicaBottle();
    ~CmdScanLeicaBottle();
    virtual QString GetName() const;
    QString GetAutoHideStatus() const;
private:    
    CmdScanLeicaBottle(const CmdScanLeicaBottle &);                     ///< Not implemented.
    const CmdScanLeicaBottle & operator = (const CmdScanLeicaBottle &); ///< Not implemented.
private:
    QString     m_AutoHide;       ///< Auto hide reuired or not "yes" or "no".
}; // end class CmdScanLeicaBottle
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdScanLeicaBottle &Cmd)
{
    Q_UNUSED(Cmd);
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
inline QDataStream & operator >> (QDataStream &Stream, CmdScanLeicaBottle &Cmd)
{
    Q_UNUSED(Cmd);
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSCANLEICABOTTLE_H
