/****************************************************************************/
/*! \file CmdQuitAppShutdown.h
 *
 *  \brief Notice Scheduler that System will shutdown or quit application.
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

#ifndef MSGCLASSES_CMDQUITAPPSHUTDOWN_H
#define MSGCLASSES_CMDQUITAPPSHUTDOWN_H

#include "Global/Include/Commands/Command.h"
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace MsgClasses {

class CmdQuitAppShutdown : public Global::Command
{
    friend QDataStream & operator << (QDataStream &, const CmdQuitAppShutdown &);
    friend QDataStream & operator >> (QDataStream &, CmdQuitAppShutdown &);

public:
    CmdQuitAppShutdown(int TimeOut, DataManager::QuitAppShutdownActionType_t quitAppShutdownActionType);
    CmdQuitAppShutdown(void);

    ~CmdQuitAppShutdown(void);

    virtual QString GetName(void) const;
    inline DataManager::QuitAppShutdownActionType_t QuitAppShutdownActionType() const {return m_quitAppShutdownActionType;}

    static QString NAME; ///< Command name.
  
private:
    CmdQuitAppShutdown(const CmdQuitAppShutdown &); ///< Not implemented.
    const CmdQuitAppShutdown &operator = (const CmdQuitAppShutdown &); ///< Not implemented.
    DataManager::QuitAppShutdownActionType_t m_quitAppShutdownActionType;

 }; // end class CmdQuitAppShutdown

inline QDataStream & operator << (QDataStream &Stream, const CmdQuitAppShutdown &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    Stream << Cmd.QuitAppShutdownActionType();
    return Stream;
}

inline QDataStream & operator >> (QDataStream &Stream, CmdQuitAppShutdown &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    int temp;
    Stream >> temp;
    Cmd.m_quitAppShutdownActionType = (DataManager::QuitAppShutdownActionType_t)temp;
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDQUITAPPSHUTDOWN_H
