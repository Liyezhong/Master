/****************************************************************************/
/*! \file CmdConfirmBathLayout.h
 *
 *  \brief CmdConfirmBathLayout command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 17.03.2011, 21.08.2012
 *   $Author:  $ Y.Novak, Vikram MK
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

#ifndef MSGCLASSES_CMDCONFIRMBATHLAYOUT_H
#define MSGCLASSES_CMDCONFIRMBATHLAYOUT_H

#include <Global/Include/Commands/Command.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdConfirmBathLayout command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdConfirmBathLayout : public Global::Command {
public:
    static QString NAME;    ///< Command name.
    friend QDataStream & operator << (QDataStream &, const CmdConfirmBathLayout &);
    friend QDataStream & operator >> (QDataStream &, CmdConfirmBathLayout &);
    /****************************************************************************/
    CmdConfirmBathLayout(int Timeout, const QString &Status);
    CmdConfirmBathLayout();
    ~CmdConfirmBathLayout();
    virtual QString GetName() const;
    QString GetConfirmationStatus() const;
private:
    CmdConfirmBathLayout(const CmdConfirmBathLayout &);                     ///< Not implemented.
    const CmdConfirmBathLayout & operator = (const CmdConfirmBathLayout &); ///< Not implemented.
private:
    QString     m_Status;       ///< Confirmation status.
}; // end class CmdConfirmBathLayout
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdConfirmBathLayout &Cmd)
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
inline QDataStream & operator >> (QDataStream &Stream, CmdConfirmBathLayout &Cmd)
{
    Q_UNUSED(Cmd);
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDCONFIRMBATHLAYOUT_H
