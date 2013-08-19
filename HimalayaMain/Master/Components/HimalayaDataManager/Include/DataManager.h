/****************************************************************************/
/*! \file DataManager.h
 *
 *  \brief Definition file for class CDataManager.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2011-07-20, 2013-03-17
 *  $Author:    $ F. Toth
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

#ifndef DATAMANAGER_OBSOLETE_DATAMANAGER_H
#define DATAMANAGER_OBSOLETE_DATAMANAGER_H

#include <QObject>
#include <QString>
#include <QIODevice>
#include "../Include/DataContainer.h"
#include "DataManager/Include/DataManagerBase.h"

namespace DataManager {
class CProgramCommandInterface;
class CReagentCommandInterface;
class CStationCommandInterface;
class CReagentGroupCommandInterface;
class CUserSettingsCommandInterface;

class CDataManager : public CDataManagerBase
{
    Q_OBJECT
private:
    bool m_UpdateList;
    CDataContainer* mp_DataContainer;
    CStationCommandInterface *mp_StationCommandInterface; //!< handles commands related to  Station container
    CReagentCommandInterface *mp_ReagentCommandInterface; //!< handles commands related to reagent container
    CReagentGroupCommandInterface *mp_ReagentGroupCommandInterface;
protected:
    virtual bool DeinitializeDataContainer();

public:
    CDataManager(Threads::MasterThreadController *p_HimalayaMasterThreadController);
    CDataManager(Threads::MasterThreadController *p_HimalayaMasterThreadController, QString Path);
    virtual ~CDataManager();

    quint32 InitializeDataContainer();
    const CDataContainer* GetDataContainer() { return mp_DataContainer; }

    CDataProgramList* GetProgramList();
    bool GetProgramList(CDataProgramList *p_ProgramList);

    CProgramSettings* GetProgramSettings();
    bool GetProgramSettings(CProgramSettings *p_ProgramList);

    CDataReagentGroupList* GetReagentGroupList();
    bool GetReagentGroupList(CDataReagentGroupList *p_ReagentGroupList);

    CReagentGroupColorList* GetReagentGroupColorList();

    bool GetStationList(CDashboardDataStationList *p_StationList);
    CDashboardDataStationList* GetStationList();

    bool GetReagentList(CDataReagentList *p_ReagentList);
    CDataReagentList* GetReagentList();

    CUserSettingsInterface* GetUserSettingsInterface();
    CHimalayaUserSettings* GetUserSettings();
    CDeviceConfigurationInterface* GetDeviceConfigurationInterface();
    CProgramCommandInterface *mp_ProgramCommandInterface; //!< Handles commands related to program container


signals:
    void StationUpdate(CDashboardStation &Station);
    void StationContainerUpdate();
    void ReagentUpdate(CReagent &Reargent);
    void ReagentContainerUpdate(CDataReagentList &);

};
}// namespace DataManager

#endif // DATAMANAGER_OBSOLETE_DATAMANAGER_H


