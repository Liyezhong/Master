/****************************************************************************/
/*! \file DashboardScene.cpp
 *
 *  \brief DashboardScene implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 17-Jan-2013
 *   $Author:  $ Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Dashboard/Include/DashboardScene.h"
#include "Core/Include/DataConnector.h"
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QApplication>

namespace Dashboard {


//!< Timing interval for the water animation in milliseconds
#define ANIMATION_INTERVAL 500
/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_DataConnector = Global data container
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CDashboardScene::CDashboardScene(Core::CDataConnector *p_DataConnector,
                       QObject *p_Parent,
                       MainMenu::CMainWindow *p_MainWindow) :
                       QGraphicsScene(p_Parent),
                       mp_DataConnector(p_DataConnector),                       
                       mp_MainWindow(p_MainWindow),
                       m_CloneDashboardStationList(true),
                       m_CloneProgramList(true)

{
    QRectF Rect;

    mp_DashboardStationListClone = new DataManager::CDashboardDataStationList();
    mp_ProgramListClone = new DataManager::CDataProgramList();

    InitDashboardStationItemsPositions();
    InitDashboardStationGroups();
    InitDashboardStationIDs();
    InitDashboardStationLabels();
    InitDashboardStationConnectorsPositions();
    InitDashboardStationConnectorsWidths();
    InitDashboardStationConnectorsHeights();
    InitDashboardEndTimeWidgetPosition();
    AddDashboardStationItemsToScene();



    // update the stations whenenver the stations.xml is sent
    CONNECTSIGNALSLOT(mp_DataConnector, DashboardStationsUpdated(), this, UpdateDashboardStations());

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDashboardScene::~CDashboardScene()
{
    try {
        delete mp_DashboardStationListClone;
        delete mp_ProgramListClone;

        delete mp_DashboardStationRetort;

        for (int i = 0; i < mp_DashboardStationItems.count(); i++)
        {
            delete mp_DashboardStationItems.at(i);
        }
/**
        for(int i = 0; i < mp_DashboardStationConnectors.count(); i++)
        {
            delete mp_DashboardStationConnectors.at(i);
        }
**/
        delete mp_DashboardStationConnector;
        delete mp_DashboardEndTimeWidget;

    } catch(...) {
        // Please the PC-Lint
    }
}

/****************************************************************************/
/*!
 *  \brief Initialize Dashboard Station Item Positions on the Screen
 */
/****************************************************************************/
void CDashboardScene::InitDashboardStationItemsPositions()
{
    /**
     m_DashboardStationItemPositions << QPoint(119, 89)
                                << QPoint(119, 192) << QPoint(247, 192) << QPoint(375, 192)
                                << QPoint(119, 276) << QPoint(204, 276) << QPoint(289, 276) << QPoint(374, 276) << QPoint(459, 276) << QPoint(544, 276) << QPoint(629, 276)
                                << QPoint(119, 396) << QPoint(204, 396) << QPoint(289, 396) << QPoint(374, 396) << QPoint(459, 396) << QPoint(544, 396) ;
    **/

    m_DashboardStationItemPositions << QPoint(119, 276) << QPoint(204, 276) << QPoint(289, 276) << QPoint(374, 276) << QPoint(459, 276) << QPoint(544, 276) << QPoint(629, 276)
                                    << QPoint(119, 396) << QPoint(204, 396) << QPoint(289, 396) << QPoint(374, 396) << QPoint(459, 396) << QPoint(544, 396)
                                    << QPoint(119, 192) << QPoint(247, 192) << QPoint(375, 192)
                                    << QPoint(119, 89);


}

/****************************************************************************/
/*!
 *  \brief Initialize Dashboard Stations IDs
 */
/****************************************************************************/
void CDashboardScene::InitDashboardStationIDs()
{
    /**
     m_DashboardStationIDs << "001"
                          << "O02" << "O03" << "004"
                          << "005" << "006" << "007" << "008"
                          << "009" << "010" << "011" << "012"
                          << "013" << "014" << "015" << "016"
                          << "017";
    **/
    m_DashboardStationIDs << "S1" << "S2" << "S3" << "S4" << "S5" << "S6" <<"S7"
                          << "S8" << "S9" <<"S10" <<"S11" <<"S12" <<"S13"
                          << "P1" << "P2" << "P3";
}

/****************************************************************************/
/*!
 *  \brief Initialize Dashboard Stations Labels
 */
/****************************************************************************/
void CDashboardScene::InitDashboardStationLabels()
{
    /**
     m_DashboardStationLabels << "Retort"
                          << "Paraffin (1)" << "Paraffin (2)" << "Paraffin (3)"
                          << "1" << "2" << "3" << "4" << "5" << "6" << "7"
                          << "8" << "9" << "10" << "11" << "12" << "13";
    **/
    m_DashboardStationLabels << "1" << "2" << "3" << "4" << "5" << "6" << "7"
                             << "8" << "9" << "10" << "11" << "12" << "13"
                             << "Paraffin (1)" << "Paraffin (2)" << "Paraffin (3)";
}

/****************************************************************************/
/*!
 *  \brief Initialize Dashboard Station Groups
 */
/****************************************************************************/
void CDashboardScene::InitDashboardStationGroups()
{
   /**
    m_DashboardStationGroup << STATIONS_GROUP_RETORT
                             << STATIONS_GROUP_PARAFFINBATH << STATIONS_GROUP_PARAFFINBATH << STATIONS_GROUP_PARAFFINBATH
                             << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE
                             << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE
                             << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE
                             << STATIONS_GROUP_BOTTLE;
   **/
    m_DashboardStationGroup << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE
                            << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE
                            << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE
                            << STATIONS_GROUP_BOTTLE
                            << STATIONS_GROUP_PARAFFINBATH << STATIONS_GROUP_PARAFFINBATH << STATIONS_GROUP_PARAFFINBATH;
}

/****************************************************************************/
/*!
 *  \brief Initialize Dashboard Station Connector Positions on the Screen
 */
/****************************************************************************/
void CDashboardScene::InitDashboardStationConnectorsPositions()
{
    m_DashboardStationConnectorsPositions << QPoint(175, 167)
                                          << QPoint(161, 186) << QPoint(286, 186) << QPoint(417, 186)
                                          << QPoint(142, 270) << QPoint(226, 270) << QPoint(311, 270) << QPoint(396, 270) << QPoint(481, 270) << QPoint(565, 270) << QPoint(651, 270)
                                          << QPoint(142, 390) << QPoint(226, 390) << QPoint(311, 390) << QPoint(396, 390) << QPoint(481, 390) << QPoint(565, 390)
                                          << QPoint(161, 178)
                                          << QPoint(502, 178)
                                          << QPoint(142, 262)
                                          << QPoint(716, 262)
                                          << QPoint(142, 382);

}

/****************************************************************************/
/*!
 *  \brief Initialize Dashboard Station Connectors (Pipe) Width on the Screen
 */
/****************************************************************************/
void CDashboardScene::InitDashboardStationConnectorsWidths()
{
    m_DashboardStationConnectorsWidths << 9
                                       << 9 << 9 << 9
                                       << 9 << 9 << 9 << 9 << 9 << 9 << 9
                                       << 9 << 9 << 9 << 9 << 9 << 9
                                       << 349
                                       << 8
                                       << 582
                                       << 8
                                       << 582;

}

/****************************************************************************/
/*!
 *  \brief Initialize Dashboard Station Connectors (Pipe) Height on the Screen
 */
/****************************************************************************/
void CDashboardScene::InitDashboardStationConnectorsHeights()
{
    m_DashboardStationConnectorsHeights << 11
                                        << 6 << 6 << 6
                                        << 6 << 6 << 6 << 6 << 6 << 6 << 6
                                        << 6 << 6 << 6 << 6 << 6 << 6
                                        << 8
                                        << 84
                                        << 8
                                        << 120
                                        << 8;
}


/****************************************************************************/
/*!
 *  \brief Initialize Dashboard Station End Time Widget Position on the Scene
 */
/****************************************************************************/
void CDashboardScene::InitDashboardEndTimeWidgetPosition()
{
    m_DashboardEndTimeWidgetPos = QPoint(520, 78);
}


/****************************************************************************/
/*!
 *  \brief Add Dashboard Station Items to Scene
 */
/****************************************************************************/
void CDashboardScene::AddDashboardStationItemsToScene()
{
    for (int i = 0; i < m_DashboardStationIDs.count(); i++)
    {
        // If station is found in the DashboardStations xmls,
        // take it from there or else
        // give the station a default value
        DataManager::CDashboardStation *p_DashboardStation = mp_DataConnector->DashboardStationList->GetDashboardStation((m_DashboardStationIDs[i]));

        if (p_DashboardStation) {
            m_DashboardStationList.insert(m_DashboardStationIDs[i], p_DashboardStation);
        } else {
            m_DashboardStationList.insert(m_DashboardStationIDs[i], p_DashboardStation);
        }

        Core::CDashboardStationItem* p_DashboardStationItem = new Core::CDashboardStationItem(mp_DataConnector,                                                                                             
                                                                                              m_DashboardStationGroup[i],
                                                                                              m_DashboardStationLabels[i],
                                                                                              true,
                                                                                              m_DashboardStationList[m_DashboardStationIDs[i]]);
        p_DashboardStationItem->setPos(m_DashboardStationItemPositions[i]);
        p_DashboardStationItem->StationSelected(false);
        mp_DashboardStationItems.append(p_DashboardStationItem);
        addItem(p_DashboardStationItem);
    }

    // For Retort, No Station Id
    mp_DashboardStationRetort = new Core::CDashboardStationItem(mp_DataConnector, STATIONS_GROUP_RETORT, "Retort", false);
    mp_DashboardStationRetort->setPos(QPoint(119, 89));
    addItem(mp_DashboardStationRetort);


    // Draw the Pipe Over here (582, 289) (width, height) of bounding rectangle
    mp_DashboardStationConnector = new Dashboard::CDashboardStationConnector(582, 289, false );
    addItem(mp_DashboardStationConnector);

    // Add the End Time Widget
    mp_DashboardEndTimeWidget = new Dashboard::CDashboardEndTimeWidget(mp_DataConnector);
    mp_DashboardEndTimeWidget->InitEndTimeWidgetItems();
    mp_GraphicsProxyWidget = this->addWidget(mp_DashboardEndTimeWidget);
    mp_GraphicsProxyWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    mp_GraphicsProxyWidget->setPos(m_DashboardEndTimeWidgetPos);
    addItem(mp_GraphicsProxyWidget);

    CONNECTSIGNALSIGNAL(mp_DashboardEndTimeWidget, OnSelectDateTime(const QDateTime &), this, OnSelectDateTime(const QDateTime &));
/**
    for(int i = 0; i < m_DashboardStationConnectorsPositions.count(); i++)
    {
        Dashboard::CDashboardStationConnector *p_DashboardStationConnector = new Dashboard::CDashboardStationConnector(m_DashboardStationConnectorsWidths[i],
                                                                                                                       m_DashboardStationConnectorsHeights[i],
                                                                                                                       false);
        p_DashboardStationConnector->setPos(m_DashboardStationConnectorsPositions[i]);
        mp_DashboardStationConnectors.append(p_DashboardStationConnector);
        addItem(p_DashboardStationConnector);
    }
**/


}

/****************************************************************************/
/*!
 *  \brief Handler when entire Stations xml file is sent
 */
/****************************************************************************/

void CDashboardScene::UpdateDashboardStations()
{

    if (m_CloneDashboardStationList) {
        *mp_DashboardStationListClone = *(mp_DataConnector->DashboardStationList);
    }

    for (int i = 0; i < mp_DashboardStationListClone->GetNumberOfDashboardStations(); i++)
    {
        DataManager::CDashboardStation *p_DashboardStation = const_cast<DataManager::CDashboardStation*>(mp_DashboardStationListClone->GetDashboardStation(i));
        m_DashboardStationList.insert(m_DashboardStationIDs[i], p_DashboardStation);

        Core::CDashboardStationItem *pListItem =
                static_cast<Core::CDashboardStationItem *>(mp_DashboardStationItems[i]);

        pListItem->SetDashboardStation(p_DashboardStation);
        pListItem->StationSelected(true);
        pListItem->update();
    }
}

void CDashboardScene::UpdateDashboardSceneReagentsForProgram(QString &ProgramId, int asapEndTime)
{
    QStringList SelectedStationIdList;
    DataManager::CProgram const *p_Program = NULL;
    QStringList ProgramReagentIdList;
    QString StationReagentId;

    if (m_CloneProgramList) {
        *mp_ProgramListClone = *(mp_DataConnector->ProgramList);
    }
    p_Program = mp_ProgramListClone->GetProgram(ProgramId);
    if(p_Program) {
        mp_DashboardEndTimeWidget->UpdateEndTimeWidgetItems(p_Program, asapEndTime);
        ProgramReagentIdList = p_Program->GetReagentIDList();
    }

    for (int i = 0; i < m_DashboardStationList.count(); i++)
    {
        DataManager::CDashboardStation *p_DashboardStation = const_cast<DataManager::CDashboardStation*>(mp_DashboardStationListClone->GetDashboardStation(i));

        StationReagentId = p_DashboardStation->GetDashboardReagentID();

        if(ProgramReagentIdList.contains(StationReagentId))

            SelectedStationIdList.append(p_DashboardStation->GetDashboardStationID());
    }

    for(int j = 0 ; j < m_DashboardStationList.count(); j++)
    {
         Core::CDashboardStationItem *pListItem =
                static_cast<Core::CDashboardStationItem *>(mp_DashboardStationItems[j]);

         if(SelectedStationIdList.contains(pListItem->GetDashboardStation()->GetDashboardStationID()))
            pListItem->StationSelected(true);
         else
            pListItem->StationSelected(false);

         pListItem->UpdateDashboardStationItemReagent();
    }
}

} // end namespace Dashboard
