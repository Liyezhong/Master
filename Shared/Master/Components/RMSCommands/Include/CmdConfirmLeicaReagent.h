/****************************************************************************/
/*! \file CmdConfirmLeicaReagent.h
 *
 *  \brief CmdConfirmLeicaReagent command definition.
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

#ifndef MSGCLASSES_CMDCONFIRMLEICAREAGENT_H
#define MSGCLASSES_CMDCONFIRMLEICAREAGENT_H

#include <Global/Include/Commands/Command.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdConfirmLeicaReagent command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdConfirmLeicaReagent : public Global::Command {
public:
    static QString NAME;    ///< Command name.
    friend QDataStream & operator << (QDataStream &, const CmdConfirmLeicaReagent &);
    friend QDataStream & operator >> (QDataStream &, CmdConfirmLeicaReagent &);
    /****************************************************************************/
    CmdConfirmLeicaReagent(int Timeout, const QString &Status);
    CmdConfirmLeicaReagent();
    ~CmdConfirmLeicaReagent();
    virtual QString GetName() const;
    QString GetConfirmationStatus() const;
private:
    CmdConfirmLeicaReagent(const CmdConfirmLeicaReagent &);                     ///< Not implemented.
    const CmdConfirmLeicaReagent & operator = (const CmdConfirmLeicaReagent &); ///< Not implemented.
private:
    QString     m_Status;       ///< Confirmation status.
}; // end class CmdConfirmLeicaReagent
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdConfirmLeicaReagent &Cmd)
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
inline QDataStream & operator >> (QDataStream &Stream, CmdConfirmLeicaReagent &Cmd)
{
    Q_UNUSED(Cmd);
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDCONFIRMLEICAREAGENT_H
