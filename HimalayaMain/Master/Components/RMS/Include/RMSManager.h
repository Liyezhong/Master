/****************************************************************************/
/*! \file RMSManager.h
 *
 *  \brief Definition file for class CRMSManager.
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

#ifndef RMSMANAGER_RMSMANAGER_H
#define RMSMANAGER_RMSMANAGER_H

#include <QObject>
#include <QString>
#include <QIODevice>
#include "RMSHandler.h"
#include "RMS/Include/RMSCommandInterface.h"

namespace Himalaya {
    class HimalayaMasterThreadController;
}
namespace HimalayaRMS {
class CRMSCommandInterface;

class CRMSManager : public QObject
{
    Q_OBJECT
private:
    CRMSHandler *mp_RMSHandler;   //!< RMS handler
    CRMSCommandInterface *mp_RMSCommandInterface; //!< Handles commands related to RMs

public:

    CRMSManager(Himalaya::HimalayaMasterThreadController *p_HimalayaMasterThreadController, DataManager::CDataManager *p_DataManager);
    void ReadConsumablesRFID(Global::tRefType Ref, const QByteArray *RfidData);
    void PollReagentExpiryStatus();
    void AbortLeicaConsumablesScanProcess();
    ~CRMSManager();
};
}// namespace HimalayaRMS

#endif // DATAMANAGER_DATAMANAGER_H


