/****************************************************************************/
/*! \file CmdCancelProgram.h
 *
 *  \brief CmdCancelProgram command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 17.03.2011
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

#ifndef MSGCLASSES_CMDCANCELPROGRAM_H
#define MSGCLASSES_CMDCANCELPROGRAM_H

#include <Global/Include/Commands/Command.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdCancelProgram command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdCancelProgram : public Global::Command {
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdCancelProgram(int Timeout, const QString &Action);
    ~CmdCancelProgram();
    virtual QString GetName() const;

    QString GetAction() const;
private:
    CmdCancelProgram();                                             ///< Not implemented.
    CmdCancelProgram(const CmdCancelProgram &);                     ///< Not implemented.
    const CmdCancelProgram & operator = (const CmdCancelProgram &); ///< Not implemented.
private:
    QString      m_Action;      ///< Type of action: how program has to be cancelled.
}; // end class CmdCancelProgram

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDCANCELPROGRAM_H
