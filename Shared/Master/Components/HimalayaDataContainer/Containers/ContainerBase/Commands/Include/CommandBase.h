/****************************************************************************/
/*! \file CommandBase.h
 *
 *  \brief Command base definition.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-02
 *  $Author:    $ R. Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MSGCLASSES_COMMANDBASE_H
#define MSGCLASSES_COMMANDBASE_H

#include <QByteArray>

#include "Global/Include/Commands/Command.h"

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  Reagent command base.
 */
/****************************************************************************/
class CommandBase : public Global::Command
{
public:
    CommandBase(int, const QDataStream &);
    CommandBase(void);

    ~CommandBase(void);

    const QByteArray &GetData(void) const;

    friend QDataStream & operator << (QDataStream &, const CommandBase &);
    friend QDataStream & operator >> (QDataStream &, CommandBase &);

protected:
    QByteArray m_ByteArray;

private:
    CommandBase(const CommandBase &);
    const CommandBase &operator = (const CommandBase &);

}; // end class CommandBase

} // end namespace MsgClasses

#endif // MSGCLASSES_COMMANDBASE_H
