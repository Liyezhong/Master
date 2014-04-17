/****************************************************************************/
/*! \file CmdQuitAppShutdownReply.h
 *
 *  \brief After some processes have done for "quit Application" or "shutdown", scheduler will notify GUI by this command.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-06-27
 *  $Author:    $ Abe Yang
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

#ifndef MSGCLASSES_CMDQUITSHUTDOWNREPLY_H
#define MSGCLASSES_CMDQUITSHUTDOWNREPLY_H

#include "Global/Include/Commands/Command.h"
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdQuitAppShutdownReply
 */
/****************************************************************************/
class CmdQuitAppShutdownReply : public Global::Command
{
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of friend operator<<
     */
    /****************************************************************************/
    friend QDataStream & operator << (QDataStream &, const CmdQuitAppShutdownReply &);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of friend operator>>
     */
    /****************************************************************************/
    friend QDataStream & operator >> (QDataStream &, CmdQuitAppShutdownReply &);

public:
    CmdQuitAppShutdownReply(int TimeOut, DataManager::QuitAppShutdownActionType_t quitAppShutdownActionType);
    CmdQuitAppShutdownReply(void);

    ~CmdQuitAppShutdownReply(void);

    virtual QString GetName(void) const;

    static QString NAME; ///< Command name.
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function QuitAppShutdownActionType
     *
     *  \return from QuitAppShutdownActionType
     */
    /****************************************************************************/
    inline DataManager::QuitAppShutdownActionType_t QuitAppShutdownActionType() const {return m_quitAppShutdownActionType;}

private:
    CmdQuitAppShutdownReply(const CmdQuitAppShutdownReply &); ///< Not implemented.
    const CmdQuitAppShutdownReply &operator = (const CmdQuitAppShutdownReply &); ///< Not implemented.
    DataManager::QuitAppShutdownActionType_t m_quitAppShutdownActionType;       ///<  Definition/Declaration of variable m_quitAppShutdownActionType

 }; // end class CmdQuitAppShutdownReply

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function operator<<
 *
 *  \return from operator<<
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdQuitAppShutdownReply &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    Stream << (int)Cmd.QuitAppShutdownActionType();
    return Stream;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function operator>>
 *
 *  \return from operator>>
 */
/****************************************************************************/
inline QDataStream & operator >> (QDataStream &Stream, CmdQuitAppShutdownReply &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    int temp;
    Stream >> temp;
    Cmd.m_quitAppShutdownActionType = (DataManager::QuitAppShutdownActionType_t)temp;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDQUITSHUTDOWNREPLY_H
