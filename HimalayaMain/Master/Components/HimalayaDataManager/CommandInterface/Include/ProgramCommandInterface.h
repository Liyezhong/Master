/****************************************************************************/
/*! \file ProgramCommandInterface.h
 *
 *  \brief Program Command Interface definition
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-07-30
 *  $Author:    $ N.Kamath
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
#ifndef PROGRAMCOMMANDINTERFACE_H
#define PROGRAMCOMMANDINTERFACE_H

#include "../../../../../../Platform/Master/Components/DataManager/CommandInterface/Include/CommandInterfaceBase.h"
#include "HimalayaDataManager/Include/DataManager.h"
//#include "HimalayaDataContainer/Containers/StainingPrograms/Commands/Include/CmdCancelProgram.h"
#include "HimalayaDataContainer/Containers/Programs/Commands/Include/CmdDeleteItem.h"
#include "HimalayaDataContainer/Containers/Programs/Commands/Include/CmdNewProgram.h"
#include "HimalayaDataContainer/Containers/Programs/Commands/Include/CmdProgramUpdate.h"

namespace DataManager {
/****************************************************************************/
/**
 * \brief  Program container command interface:Handles all the commands
 *         which work on Program container.
 */
/****************************************************************************/
class CProgramCommandInterface:public CCommandInterfaceBase
{
    Q_OBJECT
public:
    CProgramCommandInterface(CDataManager *p_DataManager, Threads::MasterThreadController *p_MasterThreadController, CDataContainer *p_DataContainer);

private:
    void RegisterCommands();
    void UpdateProgram(Global::tRefType, const MsgClasses::CmdProgramUpdate &Cmd, Threads::CommandChannel &AckCommandChannel);
    void AddProgram(Global::tRefType, const MsgClasses::CmdNewProgram &Cmd, Threads::CommandChannel &AckCommandChannel);
    void DeleteProgram(Global::tRefType, const MsgClasses::CmdDeleteItem &Cmd, Threads::CommandChannel &AckCommandChannel);

signals:
    void StartableProgramEdited(const QString &ProgramID);

};

}// end of namespace DataManager
#endif // PROGRAMCOMMANDINTERFACE_H
