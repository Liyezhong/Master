/****************************************************************************/
/*! \file StationCommandInterface.h
 *
 *  \brief Station Command Interface definition
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
#ifndef STATIONCOMMANDINTERFACE_H
#define STATIONCOMMANDINTERFACE_H

#include "../../../../../../Platform/Master/Components/DataManager/CommandInterface/Include/CommandInterfaceBase.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdStationChangeReagent.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdStationResetData.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdStationSetAsFull.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdStationSetAsEmpty.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdUpdateStationReagentStatus.h"

namespace DataManager {
/****************************************************************************/
/**
 * \brief  Station container command interface definition
 */
/****************************************************************************/
class CStationCommandInterface : public CCommandInterfaceBase
{
    Q_OBJECT
public:
    CStationCommandInterface(CDataManager *p_DataManager, Threads::MasterThreadController *p_MasterThreadController, CDataContainer *p_DataContainer);

private:
    void RegisterCommands();

    void ChangeReagentInStation(Global::tRefType,
                                 const MsgClasses::CmdStationChangeReagent &,
                                 Threads::CommandChannel &);

     void ResetStationData(Global::tRefType,
                           const MsgClasses::CmdStationResetData &,
                           Threads::CommandChannel &);

     void SetStationAsFull(Global::tRefType,
                           const MsgClasses::CmdStationSetAsFull &,
                           Threads::CommandChannel &);

     void SetStationAsEmpty(Global::tRefType,
                            const MsgClasses::CmdStationSetAsEmpty &Cmd,
                            Threads::CommandChannel &);

     void UpdateStationReagentStatus(Global::tRefType,
                            const MsgClasses::CmdUpdateStationReagentStatus& Cmd,
                            Threads::CommandChannel &);

};

}// end of namespace DataManager

#endif // STATIONCOMMANDINTERFACE_H
