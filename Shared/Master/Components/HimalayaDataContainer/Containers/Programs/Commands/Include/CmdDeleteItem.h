/****************************************************************************/
/*! \file CmdDeleteItem.h
 *
 *  \brief CmdDeleteItem command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 18.03.2011
 *   $Author:  $ Y.Novak
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

#ifndef MSGCLASSES_CMDDELETEITEM_H
#define MSGCLASSES_CMDDELETEITEM_H

#include <Global/Include/Commands/Command.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdDeleteItem command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdDeleteItem : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdDeleteItem &);
    friend QDataStream & operator >> (QDataStream &, CmdDeleteItem &);
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdDeleteItem(int Timeout, const QString &ItemId);
    CmdDeleteItem();
    ~CmdDeleteItem();
    virtual QString GetName() const;
    QString GetItemId() const;

private:
    CmdDeleteItem(const CmdDeleteItem &);                       ///< Not implemented.
    const CmdDeleteItem & operator = (const CmdDeleteItem &);   ///< Not implemented.
private:
    QString      m_ItemId;      ///< Item ID.
}; // end class CmdDeleteItem

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdDeleteItem &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_ItemId;
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
inline QDataStream & operator >> (QDataStream &Stream, CmdDeleteItem &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_ItemId;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDDELETEITEM_H
