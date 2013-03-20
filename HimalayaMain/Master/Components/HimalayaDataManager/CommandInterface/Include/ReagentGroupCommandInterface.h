/****************************************************************************/
/*! \file ReagentGroupCommandInterface.h
 *
 *  \brief Reagent group Command Interface definition
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
#ifndef DATAMANAGER_REAGENTGROUPCOMMANDINTERFACE_H
#define DATAMANAGER_REAGENTGROUPCOMMANDINTERFACE_H

#include "CommandInterfaceBase.h"

#include "Global/Include/GlobalDefines.h"

namespace Threads
{
    class CommandChannel;
}

namespace MsgClassesNew
{
    class CmdReagentGroupUpdate;
}

namespace DataManager {

/****************************************************************************/
/*!
 * \brief  Reagent group command interface definition
 */
/****************************************************************************/
class ReagentGroupCommandInterface:public CommandInterfaceBase
{
    Q_OBJECT
public:
    explicit ReagentGroupCommandInterface(Threads::MasterThreadController &,
                                     DataContainer &);

private:
    virtual void RegisterCommands(void);

    void UpdateReagentGroup(Global::tRefType,
                            const MsgClassesNew::CmdReagentGroupUpdate &,
                            Threads::CommandChannel &);
};

}// end of namespace DataManager

#endif // DATAMANAGER_REAGENTGROUPCOMMANDINTERFACE_H
