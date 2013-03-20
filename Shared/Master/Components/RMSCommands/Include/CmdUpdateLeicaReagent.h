/****************************************************************************/
/*! \file CmdUpdateLeicaReagent.h
 *
 *  \brief CmdUpdateLeicaReagent command definition.
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

#ifndef MSGCLASSES_CMDUPDATELEICAREAGENT_H
#define MSGCLASSES_CMDUPDATELEICAREAGENT_H

#include <Global/Include/Commands/Command.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdConfirmBathLayout command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdUpdateLeicaReagent : public Global::Command {
public:
    static QString NAME;    ///< Command name.
    friend QDataStream & operator << (QDataStream &, const CmdUpdateLeicaReagent &);
    friend QDataStream & operator >> (QDataStream &, CmdUpdateLeicaReagent &);
    /****************************************************************************/
    CmdUpdateLeicaReagent(int Timeout, const QString &LeicaConsumables);
     CmdUpdateLeicaReagent();
    ~CmdUpdateLeicaReagent();
    virtual QString GetName() const;
    QString GetLeicaConsumablesStatus() const;
private:

    CmdUpdateLeicaReagent(const CmdUpdateLeicaReagent &);                     ///< Not implemented.
    const CmdUpdateLeicaReagent & operator = (const CmdUpdateLeicaReagent &); ///< Not implemented.
private:
    QString     m_LeicaConsumables;       ///< Either "Bottle" or "Kit".
}; // end class CmdUpdateLeicaReagent
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdUpdateLeicaReagent &Cmd)
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
inline QDataStream & operator >> (QDataStream &Stream, CmdUpdateLeicaReagent &Cmd)
{
    Q_UNUSED(Cmd);
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDUPDATELEICAREAGENT_H
