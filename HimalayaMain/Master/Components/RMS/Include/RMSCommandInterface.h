/****************************************************************************/
/*! \file RMSCommandInterface.h
 *
 *  \brief RMSCommandInterface command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 13.08.2012 05-10-2012
 *   $Author:  $ Vikram MK, Ningu
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

#ifndef RMSCOMMANDINTERFACE_H
#define RMSCOMMANDINTERFACE_H

#include "RMSHandler.h"
#include "RMSCommands/Include/CmdLeicaConsumables.h"
#include "Threads/Include/CommandChannel.h"

#include "RMSCommands/Include/CmdConfirmLeicaReagent.h"
#include "RMSCommands/Include/CmdFillLeicaReagent.h"
#include "RMSCommands/Include/CmdResetStationRms.h"
#include "RMSCommands/Include/CmdScanLeicaBottle.h"
#include "RMSCommands/Include/CmdUpdateLeicaReagent.h"
#include "RMSCommands/Include/CmdScanLeicaKit.h"
#include "RMSCommands/Include/CmdConfirmBathLayout.h"
#include "RMSCommands/Include/CmdUpdateRmsStatusToGui.h"

namespace HimalayaRMS {

//class CRMSCommandInterface;
const int MAX_KITSCAN_TIME = (1000*60*3);       //!< Timer of 3 minutes for Kit Timeout
const int MAX_BOTTLESCAN_TIME = (1000*60*1);    //!< Timer of 1 minutes for Bottle Timeout
const int MAX_SENDREQUEST_TIME = (1000*1);     //!< Timer of 1 seconds to send request to DCL

class CRMSCommandInterface: public QObject
{
    Q_OBJECT
public:
    CRMSCommandInterface(DataManager::CDataManager *p_DataManager, Himalaya::HimalayaMasterThreadController *p_HimalayaMasterThreadController, CRMSHandler *p_RMSHandler);
    void ReadLeicaConsumables(Global::tRefType, const QByteArray *RfidData);
    void SendReadLeicaConsumablesRequest();
    bool InitiateandStartKitTimeout();
    bool InitiateandStartBottleTimeout();
    bool InitiateandStartSendRequest();
    void StopScanRequestCount();
    void AbortLeicaScanProcess();

private:
    void RegisterCommands();
    void UpdateLeicaReagent(Global::tRefType Ref, const MsgClasses::CmdUpdateLeicaReagent &Cmd, Threads::CommandChannel &AckCommandChannel);
    void ResetStationRms(Global::tRefType Ref, const MsgClasses::CmdResetStationRms &Cmd, Threads::CommandChannel &AckCommandChannel);
    void ScanLeicaBottle(Global::tRefType Ref, const MsgClasses::CmdScanLeicaBottle &Cmd, Threads::CommandChannel &AckCommandChannel);
    void ScanLeicaKit(Global::tRefType Ref, const MsgClasses::CmdScanLeicaKit &Cmd, Threads::CommandChannel &AckCommandChannel);
    void FillLeicaReagent(Global::tRefType Ref, const MsgClasses::CmdFillLeicaReagent &Cmd, Threads::CommandChannel &AckCommandChannel);
    void ConfirmBathLayout(Global::tRefType Ref , const MsgClasses::CmdConfirmBathLayout &Cmd, Threads::CommandChannel &AckCommandChannel);
    void ConfirmLeicaReagent(Global::tRefType Ref, const MsgClasses::CmdConfirmLeicaReagent &Cmd, Threads::CommandChannel &AckCommandChannel);


    //Pointer section
    DataManager::CDataManager                *mp_DataManager;                       //!< Self Explaining
    Himalaya::HimalayaMasterThreadController *mp_HimalayaMasterThreadController;    //!< Will be used to call BroadCastCommand
    CRMSHandler                              *mp_RMSHandler;                        //!< Provides access to various containers

    QTimer *mp_ScanBottleTimeout;                       //!< Timer of 1 minutes for Bottle Timeout
    QTimer *mp_ScanKitTimeout;                          //!< Timer of 3 minutes for Kit Timeout
    QTimer *mp_SendScanRequest;                         //!< Timer of 15 second to send polling messages


private slots:

    void OnScanKitTimeoutExpiry();
    void OnScanBottleTimeoutExpiry();
    void OnSendScanRequest();    
};


}// end of namespace HimalayaRMS

#endif // RMSCOMMANDINTERFACE_H
