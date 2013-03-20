/****************************************************************************/
/*! \file RMSManager.cpp
 *
 *  \brief Implementation file for class CRMSManager.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-08-15 05-10-2012
 *  $Author:    $ Vikram MK, Ningu
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

#include <QDebug>
#include <Global/Include/SystemPaths.h>
#include "RMS/Include/RMSManager.h"
#include "RMS/Include/RMSHandler.h"

namespace HimalayaRMS {

/****************************************************************************/
/*!
 *  \brief Parameterized Constructor for RMS Manager class.
*/
/****************************************************************************/
CRMSManager::CRMSManager(Himalaya::HimalayaMasterThreadController *p_HimalayaMasterThreadController , DataManager::CDataManager *p_DataManager) :QObject(0)
{
    mp_RMSHandler = new CRMSHandler(p_DataManager,p_HimalayaMasterThreadController);
    mp_RMSCommandInterface = new CRMSCommandInterface(p_DataManager, p_HimalayaMasterThreadController, mp_RMSHandler);
}
/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CRMSManager::~CRMSManager()
{
    delete mp_RMSHandler;
    delete mp_RMSCommandInterface;
}
/****************************************************************************/
/*!
 *  \brief Read Kit/Bottle RFID information
 *  \iparam Ref = Reference type
 *  \iparam RfidData = Leica consumables data
 *  \return true - success
 */
/****************************************************************************/
void CRMSManager::ReadConsumablesRFID(Global::tRefType Ref, const QByteArray *RfidData)
{
    mp_RMSCommandInterface->ReadLeicaConsumables(Ref,RfidData);
}
/****************************************************************************/
/*!
 *  \brief Calls OnPollRMSStatusExpiry() for expiry status once after GUI bootup
 *  \return void
 */
/****************************************************************************/
void CRMSManager::PollReagentExpiryStatus()
{
    mp_RMSHandler->RmsPollReagentExpiryStatus();
}
/****************************************************************************/
/*!
 *  \brief Aborts Leica consumables scan process
 *  \return void
 */
/****************************************************************************/
void CRMSManager::AbortLeicaConsumablesScanProcess()
{
    mp_RMSCommandInterface->AbortLeicaScanProcess();
}
/****************************************************************************/
} // namespace HimalayaRMS
