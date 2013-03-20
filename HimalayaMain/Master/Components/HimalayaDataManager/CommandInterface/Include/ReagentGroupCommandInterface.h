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

#include "../../../../../../Platform/Master/Components/DataManager/CommandInterface/Include/CommandInterfaceBase.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "Global/Include/GlobalDefines.h"

namespace Threads
{
    class CommandChannel;
}

namespace MsgClasses
{
    class CmdReagentGroupUpdate;
}

namespace DataManager {

/****************************************************************************/
/*!
 * \brief  Reagent group command interface definition
 */
/****************************************************************************/
class CReagentGroupCommandInterface: public CCommandInterfaceBase
{
    Q_OBJECT
public:
    CReagentGroupCommandInterface(CDataManager *p_DataManager, Threads::MasterThreadController *p_MasterThreadController, CDataContainer *p_DataContainer);

private:
    virtual void RegisterCommands(void);

    void UpdateReagentGroup(Global::tRefType,
                            const MsgClasses::CmdReagentGroupUpdate &,
                            Threads::CommandChannel &);
};

}// end of namespace DataManager

#endif // DATAMANAGER_REAGENTGROUPCOMMANDINTERFACE_H
