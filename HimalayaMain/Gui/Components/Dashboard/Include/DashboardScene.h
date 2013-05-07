/****************************************************************************/
/*! \file DashboardScene.h
 *
 *  \brief DashboardScene definition.
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

#ifndef DASHBOARD_DASHBOARDSCENE_H
#define DASHBOARD_DASHBOARDSCENE_H

#include "Core/Include/ReagentRMSModel.h"
#include "Core/Include/DashboardStationItem.h"
#include "Dashboard/Include/DashboardStationConnector.h"
#include "Dashboard/Include/DashboardEndTimeWidget.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QGraphicsItem>
#include <QPainter>
#include <HimalayaDataContainer/Helper/Include/Global.h>

namespace Core {
    class CDataConnector;
}

namespace Dashboard {

/****************************************************************************/
/**
 * \brief This class is the graphics scene for the bath layout.
 */
/****************************************************************************/
class CDashboardScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit CDashboardScene(Core::CDataConnector *p_DataConnector,
                        QObject *p_Parent = NULL,
                        MainMenu::CMainWindow *p_MainWindow = NULL);
    virtual ~CDashboardScene();
    void UpdateRetortStatus(DataManager::RetortStatusType_t retortStatusType);
    const QTime& GetStepRemainingTime();
    const QTime GetProgramRemainingTime();
    const QString GetEndDateTime();

protected:

private:
    Core::CDataConnector *mp_DataConnector;                                          //!< Global data container
    MainMenu::CMainWindow *mp_MainWindow;                                            //!< Reference to main window.
    QList<Core::CDashboardStationItem *> mp_DashboardStationItems;                   //!< list of station items
    Core::CDashboardStationItem *mp_DashboardStationRetort;
    QList<Dashboard::CDashboardStationConnector *> mp_DashboardStationConnectors;    //!< list of station connectors
    QList<QPointF> m_DashboardStationItemPositions;                                  //!< list of station item positions
    QList<QPointF> m_DashboardStationConnectorsPositions;                             //!< list of station connectors positions
    QList<int> m_DashboardStationConnectorsWidths;                                    //!< list of station connectors (pipe) width
    QList<int> m_DashboardStationConnectorsHeights;                                   //!< list of station connectors (pipe) height
    QPoint    m_DashboardEndTimeWidgetPos;
    QHash<QString, DataManager::CDashboardStation*> m_DashboardStationList;          //!< Hash table of Stations
    QStringList m_DashboardStationIDs;                                               //!< StationIds list
    QList<StationGroupType_t> m_DashboardStationGroup;                               //!< StationGroup list
    QList<QString> m_DashboardStationLabels;
    Dashboard::CDashboardStationConnector *mp_DashboardStationConnector;
    Dashboard::CDashboardEndTimeWidget *mp_DashboardEndTimeWidget;
    QGraphicsProxyWidget *mp_GraphicsProxyWidget;

    DataManager::CDashboardDataStationList *mp_DashboardStationListClone;            //!< Cloned DataManager StationList
    bool m_CloneDashboardStationList;                                                //!< True if DataManager StationList has to be cloned.
    DataManager::CDataProgramList *mp_ProgramListClone;                              //!< Cloned DataManager ProgramList
    bool m_CloneProgramList;                                                         //!< True if DataManager StationList has to be cloned.


    void InitDashboardStationIDs();
    void InitDashboardStationItemsPositions();
    void InitDashboardStationGroups();
    void InitDashboardStationLabels();
    void InitDashboardStationConnectorsPositions();
    void InitDashboardStationConnectorsWidths();
    void InitDashboardStationConnectorsHeights();
    void InitDashboardEndTimeWidgetPosition();
    void AddDashboardStationItemsToScene();
    void AddGraphicsProxyWidgetsToScene();


private slots:
    void UpdateDashboardStations();
    void UpdateDashboardSceneReagentsForProgram(QString&, int);

signals:
    void OnSelectDateTime(const QDateTime &);
    void ProgramActionStarted(DataManager::ProgramActionType_t, int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume);
    void ProgramActionStopped(DataManager::ProgramActionType_t);

};

} // end namespace Dashboard

#endif // DASHBOARD_DASHBOARDSCENE_H
