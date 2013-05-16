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

struct PipeRectAndOrientation
{
    PipeRectAndOrientation()
    {

    }

    PipeRectAndOrientation(QRectF rect, QString orientation, QPoint brushOrigin)
    {
        m_rect = rect;
        m_orientation = orientation;
        m_brushOrigin = brushOrigin;
    }

    QRectF m_rect;
    QString m_orientation;
    QPoint m_brushOrigin;
};

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
    void UpdateRetortStatus(DataManager::ContainerStatusType_t retortStatusType);
    const QTime& GetStepRemainingTime();
    const QTime GetProgramRemainingTime();
    const QString GetEndDateTime();
    void OnStationSuckDrain(const QString& StationId, bool IsStart, bool IsSuck);
    bool HaveExpiredReagent();
protected:

private:
    Core::CDataConnector *mp_DataConnector;                                          //!< Global data container
    MainMenu::CMainWindow *mp_MainWindow;                                            //!< Reference to main window.
    QList<Core::CDashboardStationItem *> mp_DashboardStationItems;                   //!< list of station items
    Core::CDashboardStationItem *mp_DashboardStationRetort;
    QList<QPointF> m_DashboardStationItemPositions;                                  //!< list of station item positions
    QPoint    m_DashboardEndTimeWidgetPos;
    QHash<QString, DataManager::CDashboardStation*> m_DashboardStationList;          //!< Hash table of Stations
    QHash<QString, QPointF> m_StationJointList;          //!< Hash table of Station joints
    QHash<QString, PipeRectAndOrientation> m_PipeRectList;

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
    QString m_CurrentReagentColorValue;
    QStringList m_PipeListP1;
    QStringList m_PipeListP2;
    QStringList m_PipeListP3;

    QStringList m_PipeOrientationList;

    QStringList m_PipeListS1;
    QStringList m_PipeListS2;
    QStringList m_PipeListS3;
    QStringList m_PipeListS4;
    QStringList m_PipeListS5;
    QStringList m_PipeListS6;

    QStringList m_PipeListS7;
    QStringList m_PipeListS8;
    QStringList m_PipeListS9;
    QStringList m_PipeListS10;
    QStringList m_PipeListS11;
    QStringList m_PipeListS12;
    QStringList m_PipeListS13;
    bool m_IsSuck;
    QTimer* m_pPipeAnimationTimer;
    QGraphicsPathItem* m_pWorkingPipeGraphicsRectItem;
    QList<QGraphicsPathItem*> m_pGraphicsPathItemPipeList;
    QGraphicsPathItem* m_WholePipeGraphicsRectItem;
    int m_currentTimerOrder;
    void InitDashboardStationIDs();
    void InitDashboardStationItemsPositions();
    void InitStationConnectedPipeList();
    void InitDashboardStationGroups();
    void InitDashboardStationLabels();
    void InitDashboardEndTimeWidgetPosition();
    void AddDashboardStationItemsToScene();
    void AddGraphicsProxyWidgetsToScene();
    QRectF MakeHorizontalPipeRect(const QString& stationID, const QString& OtherStationID);
    QRectF MakeHorizontalBinaryPipeRect(const QString& stationID, const QString& OtherStationID,
                                        const QString& MidTopStationID, bool IsReturnLeftOne);//IsReturnLeftOne is false ,it will return the right one
    void CollectPipeRect();
    void CreateAllPipe();
    void RepresentCurrentWorkingPipe(const QString& StationID);
    void ClearCurrentWorkingPipe();
private slots:
    void UpdateDashboardStations();
    void UpdateDashboardSceneReagentsForProgram(QString&, int, QList<QString>&);
    void PipeSuckDrainAnimation();

signals:
    void OnSelectDateTime(const QDateTime &);
    void ProgramActionStarted(DataManager::ProgramActionType_t, int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume);
    void ProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType);

};

} // end namespace Dashboard

#endif // DASHBOARD_DASHBOARDSCENE_H
