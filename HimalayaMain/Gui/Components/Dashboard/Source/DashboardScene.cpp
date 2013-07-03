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

const int PipeWidth = 10;
const int JointHeight = 17;

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
                       m_CloneProgramList(true),
                       m_pPipeAnimationTimer(NULL),
                       m_pBlinkingTimer(NULL),
                       m_currentTimerOrder(0),
                       m_SuckDrainStationId("")

{
    QRectF Rect;

    mp_DashboardStationListClone = new DataManager::CDashboardDataStationList();
    mp_ProgramListClone = new DataManager::CDataProgramList();
    m_pPipeAnimationTimer = new QTimer(this);
    CONNECTSIGNALSLOT(m_pPipeAnimationTimer, timeout(), this, PipeSuckDrainAnimation());

    m_pBlinkingTimer = new QTimer(this);
    m_pBlinkingTimer->setInterval(1000);
    CONNECTSIGNALSLOT(m_pBlinkingTimer, timeout(), this, BlinkingStation());

    InitDashboardStationItemsPositions();
    InitDashboardStationGroups();
    InitDashboardStationIDs();
    InitDashboardStationLabels();
    InitDashboardEndTimeWidgetPosition();
    InitStationConnectedPipeList();
    AddDashboardStationItemsToScene();



    // update the stations whenenver the stations.xml is sent
    CONNECTSIGNALSLOT(mp_DataConnector, DashboardStationsUpdated(), this, UpdateDashboardStations());
    CONNECTSIGNALSLOT(mp_DataConnector, UserSettingsUpdated(), this, OnUpdateUserSettings());

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

        for (int i = 0; i < m_pGraphicsPathItemPipeList.count(); i++)
        {
            delete m_pGraphicsPathItemPipeList.at(i);
        }
        delete m_WholePipeGraphicsRectItem;
        delete mp_DashboardEndTimeWidget;
        delete m_pPipeAnimationTimer;
        delete m_pBlinkingTimer;
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

    m_DashboardStationItemPositions << QPoint(119, 276) << QPoint(204, 276) << QPoint(289, 276) << QPoint(374, 276) << QPoint(459, 276) << QPoint(544, 276) << QPoint(629, 276)
                                    << QPoint(119, 396) << QPoint(204, 396) << QPoint(289, 396) << QPoint(374, 396) << QPoint(459, 396) << QPoint(544, 396)
                                    << QPoint(119, 192) << QPoint(247, 192) << QPoint(375, 192)
                                    << QPoint(119, 89);


}

void CDashboardScene::InitStationConnectedPipeList()
{
    m_PipeListP1 << "P1" << "P1P2Left" << "Retort";
    m_PipeListP2 << "P2" << "P1P2Right" << "Retort";

    m_PipeListP3 << "P3" << "P2P3" << "P1P2Right" << "Retort";

    m_PipeListS1 << "S1" << "S1S2" << "S2S3" << "S3S4" << "S4S5" << "S5S6Left" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";
    m_PipeListS2         <<   "S2" << "S2S3" << "S3S4" << "S4S5" << "S5S6Left" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";
    m_PipeListS3         <<             "S3" << "S3S4" << "S4S5" << "S5S6Left" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";
    m_PipeListS4         <<                       "S4" << "S4S5" << "S5S6Left" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";
    m_PipeListS5         <<                                 "S5" << "S5S6Left" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";
    m_PipeListS6         <<                                 "S6" << "S5S6Right" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";

    m_PipeListS7 << "S7" << "S6S7" << "S5S6Right" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";

    m_PipeListS8 << "S8" << "S8S9" << "S9S10" << "S10S11" << "S11S12" << "S12S13" << "S13S13RightPoint" << "VerticalPipe2"
                 << "S7S7RightPoint" << "S6S7" << "S5S6Right" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";


    m_PipeListS9 <<           "S9" << "S9S10" << "S10S11" << "S11S12" << "S12S13" << "S13S13RightPoint" << "VerticalPipe2"
                 << "S7S7RightPoint" << "S6S7" << "S5S6Right" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";

    m_PipeListS10 <<                    "S10" << "S10S11" << "S11S12" << "S12S13" << "S13S13RightPoint" << "VerticalPipe2"
                 << "S7S7RightPoint" << "S6S7" << "S5S6Right" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";



    m_PipeListS11 <<                                "S11" << "S11S12" << "S12S13" << "S13S13RightPoint" << "VerticalPipe2"
                 << "S7S7RightPoint" << "S6S7" << "S5S6Right" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";


    m_PipeListS12 <<                                            "S12" << "S12S13" << "S13S13RightPoint" << "VerticalPipe2"
                 << "S7S7RightPoint" << "S6S7" << "S5S6Right" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";


    m_PipeListS13 <<                                                        "S13" << "S13S13RightPoint" << "VerticalPipe2"
                 << "S7S7RightPoint" << "S6S7" << "S5S6Right" << "VerticalPipe1" << "P3P3RightPoint" << "P2P3" << "P1P2Right" << "Retort";



}

/****************************************************************************/
/*!
 *  \brief Initialize Dashboard Stations IDs
 */
/****************************************************************************/
void CDashboardScene::InitDashboardStationIDs()
{
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

    m_DashboardStationGroup << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE
                            << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE
                            << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE << STATIONS_GROUP_BOTTLE
                            << STATIONS_GROUP_BOTTLE
                            << STATIONS_GROUP_PARAFFINBATH << STATIONS_GROUP_PARAFFINBATH << STATIONS_GROUP_PARAFFINBATH;
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

QRectF CDashboardScene::MakeHorizontalPipeRect(const QString& stationID, const QString& OtherStationID)
{
    QPointF p1 = m_StationJointList[stationID];
    QPointF p2 = m_StationJointList[OtherStationID];

    return QRectF(p1.rx(), p1.ry(), p2.rx() - p1.rx(), PipeWidth);
}

QRectF CDashboardScene::MakeHorizontalBinaryPipeRect(const QString& stationID, const QString& OtherStationID,
                                    const QString& MidTopStationID, bool IsReturnLeftOne)//IsReturnLeftOne is false ,it will return the right one

{
    QPointF p1 = m_StationJointList[stationID];
    QPointF p2 = m_StationJointList[MidTopStationID];
    if (IsReturnLeftOne)
    {
        return QRectF(p1.rx(), p1.ry(), p2.rx() - p1.rx(), PipeWidth);
    }
    else
    {
         QPointF p3 = m_StationJointList[OtherStationID];
         return QRectF(p2.rx(), p1.ry(), p3.rx() - p2.rx(), PipeWidth);
    }

}

void CDashboardScene::CollectPipeRect()
{
    int yOffset = -1;
    int yOffsetP = -5;
    m_PipeRectList.insert("S8S9", PipeRectAndOrientation(MakeHorizontalPipeRect("S8", "S9"), "right", QPoint(0, yOffset)));
    m_PipeRectList.insert("S9S10", PipeRectAndOrientation(MakeHorizontalPipeRect("S9", "S10"), "right", QPoint(0, yOffset)));
    m_PipeRectList.insert("S10S11", PipeRectAndOrientation(MakeHorizontalPipeRect("S10", "S11"), "right", QPoint(0, yOffset)));
    m_PipeRectList.insert("S11S12", PipeRectAndOrientation(MakeHorizontalPipeRect("S11", "S12"), "right", QPoint(0, yOffset)));
    m_PipeRectList.insert("S12S13", PipeRectAndOrientation(MakeHorizontalPipeRect("S12", "S13"), "right", QPoint(0, yOffset)));
    m_PipeRectList.insert("S13S13RightPoint", PipeRectAndOrientation(MakeHorizontalPipeRect("S13", "S13RightPoint"), "right", QPoint(0, yOffset)));
    m_PipeRectList.insert("S1S2", PipeRectAndOrientation(MakeHorizontalPipeRect("S1", "S2"), "right", QPoint(0, yOffset)));
    m_PipeRectList.insert("S2S3", PipeRectAndOrientation(MakeHorizontalPipeRect("S2", "S3"), "right", QPoint(0, yOffset)));
    m_PipeRectList.insert("S3S4", PipeRectAndOrientation(MakeHorizontalPipeRect("S3", "S4"), "right", QPoint(0, yOffset)));
    m_PipeRectList.insert("S4S5", PipeRectAndOrientation(MakeHorizontalPipeRect("S4", "S5"), "right", QPoint(0, yOffset)));

    m_PipeRectList.insert("S5S6Right", PipeRectAndOrientation(MakeHorizontalBinaryPipeRect("S5", "S6", "P3RightPoint", false), "left", QPoint(0, yOffset)));
    m_PipeRectList.insert("S5S6Left", PipeRectAndOrientation(MakeHorizontalBinaryPipeRect("S5", "S6", "P3RightPoint", true), "right", QPoint(0, yOffset)));

    m_PipeRectList.insert("S6S7", PipeRectAndOrientation(MakeHorizontalPipeRect("S6", "S7"), "left", QPoint(0, yOffset)));
    m_PipeRectList.insert("S7S7RightPoint", PipeRectAndOrientation(MakeHorizontalPipeRect("S7", "S7RightPoint"), "left", QPoint(0, yOffset)));

    m_PipeRectList.insert("P2P3", PipeRectAndOrientation(MakeHorizontalPipeRect("P2", "P3"), "left", QPoint(0, yOffsetP)));
    m_PipeRectList.insert("P3P3RightPoint", PipeRectAndOrientation(MakeHorizontalPipeRect("P3", "P3RightPoint"), "left", QPoint(0, yOffsetP)));

    m_PipeRectList.insert("P1P2Right", PipeRectAndOrientation(MakeHorizontalBinaryPipeRect("P1", "P2", "Retort", false), "left", QPoint(0, yOffsetP)));
    m_PipeRectList.insert("P1P2Left", PipeRectAndOrientation(MakeHorizontalBinaryPipeRect("P1", "P2", "Retort", true), "right", QPoint(0, yOffsetP)));


}

void CDashboardScene::CreateAllPipe()
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    foreach (PipeRectAndOrientation rectOrientation, m_PipeRectList)
    {
        path.addRect(rectOrientation.m_rect);
    }

    m_WholePipeGraphicsRectItem = new QGraphicsPathItem(path);
    m_WholePipeGraphicsRectItem->setPen(QPen(Qt::NoPen));
    m_WholePipeGraphicsRectItem->setBrush(QBrush(QColor(136, 136, 136), Qt::SolidPattern));
    addItem(m_WholePipeGraphicsRectItem);
}

class CPipeGraphicsPathItem: public QGraphicsPathItem
{
public:
    CPipeGraphicsPathItem(const QPainterPath &path, const QPointF& brushOrigin):QGraphicsPathItem(path),
        m_BrushOrigin(brushOrigin)
    {}

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
  {
        painter->setBrushOrigin(m_BrushOrigin);
        QGraphicsPathItem::paint(painter, option, widget);
  }
private:
  const QPointF m_BrushOrigin;
};

void CDashboardScene::ClearCurrentWorkingPipe()
{
    for (int i = 0; i < m_pGraphicsPathItemPipeList.count(); i++)
    {
        delete m_pGraphicsPathItemPipeList.at(i);
    }
    m_pGraphicsPathItemPipeList.clear();
    m_PipeOrientationList.clear();
}

void CDashboardScene::RepresentCurrentWorkingPipe(const QString& StationID)
{
    //P1: P1, Retort Joint, P1P2Left

     QStringList* pipeList = NULL;

     if ("P1" == StationID)
         pipeList = &m_PipeListP1;
     else if ("P2" == StationID)
         pipeList = &m_PipeListP2;
     else if ("P3" == StationID)
         pipeList = &m_PipeListP3;
     else if ("S1" == StationID)
         pipeList = &m_PipeListS1;
     else if ("S2" == StationID)
         pipeList = &m_PipeListS2;
     else if ("S3" == StationID)
         pipeList = &m_PipeListS3;
     else if ("S4" == StationID)
         pipeList = &m_PipeListS4;
     else if ("S5" == StationID)
         pipeList = &m_PipeListS5;
     else if ("S6" == StationID)
         pipeList = &m_PipeListS6;
     else if ("S7" == StationID)
         pipeList = &m_PipeListS7;
     else if ("S8" == StationID)
         pipeList = &m_PipeListS8;
     else if ("S9" == StationID)
         pipeList = &m_PipeListS9;
     else if ("S10" == StationID)
         pipeList = &m_PipeListS10;
     else if ("S11" == StationID)
         pipeList = &m_PipeListS11;
     else if ("S12" == StationID)
         pipeList = &m_PipeListS12;
     else if ("S13" == StationID)
         pipeList = &m_PipeListS13;

     foreach (const QString &strPipeID, *pipeList)
     {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(m_PipeRectList[strPipeID].m_rect);

        QGraphicsPathItem* pWorkingPipeGraphicsRectItem = new CPipeGraphicsPathItem(path, m_PipeRectList[strPipeID].m_brushOrigin);
        pWorkingPipeGraphicsRectItem ->setPen(QPen(Qt::NoPen));
        addItem(pWorkingPipeGraphicsRectItem);

        m_PipeOrientationList.append(m_PipeRectList[strPipeID].m_orientation);
        m_pGraphicsPathItemPipeList.append(pWorkingPipeGraphicsRectItem);
     }
}


void CDashboardScene::PipeSuckDrainAnimation()
{
    QString pixmapName;
    if (0 == m_currentTimerOrder)
    {
        pixmapName = ":/HimalayaImages/Icons/Dashboard/Pipe/flow1.png";
        m_currentTimerOrder++;
    }
    else if(1 == m_currentTimerOrder)
    {
        pixmapName = ":/HimalayaImages/Icons/Dashboard/Pipe/flow2.png";
        m_currentTimerOrder++;
    }
    else if(2 == m_currentTimerOrder)
    {
        pixmapName = ":/HimalayaImages/Icons/Dashboard/Pipe/flow3.png";
        m_currentTimerOrder = 0;
    }

    int j = 0;
    for (int i = 0; i < m_pGraphicsPathItemPipeList.size(); ++i)
    {
        QString pipeOrientation = m_PipeOrientationList.at(j);
        j++;
        QPixmap pixMap;
        pixMap.load(pixmapName);

        QMatrix matrix;
        qreal angle = 0.0;
        if ("up" == pipeOrientation)
        {
            if (m_IsSuck)
            {
                angle = -90.0;
            }
            else
               angle = 90.0;
        }
        else if ("left" == pipeOrientation)
        {
            if (m_IsSuck)
             angle = 180.0;
        }
        else if ("right" == pipeOrientation)
        {
            if (!m_IsSuck)
            {
                angle = 180.0;
            }
        }

        if (abs(angle) > 0.0)
        {
            matrix.rotate(angle);
            pixMap = pixMap.transformed(matrix);
        }

        QPainter painter;
        painter.begin(&pixMap);
        QColor color(m_CurrentReagentColorValue);
        color.setAlpha(150);
        painter.fillRect(0, 0, 20, 20, color);
        painter.end();

        m_pGraphicsPathItemPipeList.at(i)->setBrush(QBrush(pixMap));
    }
}

void CDashboardScene::BlinkingStation()
{
    for (int i = 0; i < mp_DashboardStationItems.size(); i++)
    {
         Core::CDashboardStationItem* item = mp_DashboardStationItems.at(i);
         if (item)
         {
             if (item->isSelected() && item->IsReagentExpired())
                 item->DrawStationItemImage();
         }
    }
}

void CDashboardScene::OnUpdateUserSettings()
{
     if (Global::RMS_OFF == mp_DataConnector->SettingsInterface->GetUserSettings()->GetModeRMSProcessing()&&
        Global::RMS_OFF == mp_DataConnector->SettingsInterface->GetUserSettings()->GetModeRMSCleaning())
     {
        m_pBlinkingTimer->stop();
     }
     else
         m_pBlinkingTimer->start();
}

/****************************************************************************/
/*!
 *  \brief Add Dashboard Station Items to Scene
 */
/****************************************************************************/
void CDashboardScene::AddDashboardStationItemsToScene()
{
    m_StationJointList.clear();
    m_PipeRectList.clear();

    //Stations(include paraffin Bath)
    for (int i = 0; i < m_DashboardStationIDs.count(); i++)
    {
        // If station is found in the DashboardStations xmls,
        // take it from there or else
        // give the station a default value
        DataManager::CDashboardStation *p_DashboardStation = mp_DataConnector->DashboardStationList->GetDashboardStation((m_DashboardStationIDs[i]));

        if (p_DashboardStation) {
            m_DashboardStationList.insert(m_DashboardStationIDs[i], p_DashboardStation);
        }

        Core::CDashboardStationItem* p_DashboardStationItem = new Core::CDashboardStationItem(mp_DataConnector,                                                                                             
                                                                                              m_DashboardStationGroup[i],
                                                                                              m_DashboardStationIDs[i],
                                                                                              m_DashboardStationLabels[i],
                                                                                              true,
                                                                                              m_DashboardStationList[m_DashboardStationIDs[i]]);
        p_DashboardStationItem->setPos(m_DashboardStationItemPositions[i]);
        p_DashboardStationItem->StationSelected(false);
        mp_DashboardStationItems.append(p_DashboardStationItem);

        QRectF rectStation = p_DashboardStationItem->boundingRect();
        QPointF posStation = p_DashboardStationItem->pos();
        qreal x = posStation.rx() + rectStation.width()/2 - 5 - 14;
        qreal y = posStation.ry() - JointHeight;
        m_StationJointList.insert(m_DashboardStationIDs[i], QPointF(x, y));

        addItem(p_DashboardStationItem);
        int xOrigin = 0;
        const QString& strID = m_DashboardStationIDs[i];
        if ("S9" == strID || "S11" == strID || "S13" == strID || "S6" == strID
                || "S4" == strID || "S2" == strID)
        {
           xOrigin = -5;
        } else if("P2" == strID)
        {
            xOrigin = -2;
        }  else if("P3" == strID)
        {
            xOrigin = -3;
        }
        m_PipeRectList.insert(m_DashboardStationIDs[i], PipeRectAndOrientation(QRectF(x, y, PipeWidth, JointHeight), "up", QPoint(xOrigin, 0)));
    }   

    // For Retort, No Station Id
    mp_DashboardStationRetort = new Core::CDashboardStationItem(mp_DataConnector, STATIONS_GROUP_RETORT, "Retort", "Retort", false);
    mp_DashboardStationRetort->setPos(QPoint(119, 89));
    addItem(mp_DashboardStationRetort);

    //Add Retort's rect and letfTop point
    QRectF rectStation = mp_DashboardStationRetort->boundingRect();
    QPointF posStation = mp_DashboardStationRetort->pos();
    qreal x = posStation.rx() + rectStation.width()/2 - 5;
    qreal y = posStation.ry() + rectStation.height();
    m_StationJointList.insert("Retort", QPointF(x, y));
    int hh = m_StationJointList["P1"].ry()  - y + PipeWidth;
    m_PipeRectList.insert("Retort", PipeRectAndOrientation(QRectF(x, y, PipeWidth, hh), "up", QPoint(-3, 0)));

    //P3 Right point
    int paraffinbathBoundingRectWidth = 122;
    QPointF P3RightPoint(m_StationJointList["P3"].rx() + paraffinbathBoundingRectWidth/2 + 24, m_StationJointList["P3"].ry());
    int h = m_StationJointList["S5"].ry() - m_StationJointList["P1"].ry() + PipeWidth;
    QRectF rect;
    rect.setTopLeft(P3RightPoint);//vertical pipe 1
    rect.setWidth(PipeWidth);
    rect.setHeight(h);
    m_PipeRectList.insert("VerticalPipe1", PipeRectAndOrientation(rect, "up", QPoint(-8, 0)));
    m_StationJointList.insert("P3RightPoint", P3RightPoint);

    //S7 Right point
    int bottleBoundingRectWidth = 79;
    QPointF S7RightPoint(m_StationJointList["S7"].rx() + bottleBoundingRectWidth/2 + 24, m_StationJointList["S7"].ry());
    h = m_StationJointList["S13"].ry() - m_StationJointList["S7"].ry() + PipeWidth;
    rect.setTopLeft(S7RightPoint);//vertical pipe 2
    rect.setWidth(PipeWidth);
    rect.setHeight(h);
    m_PipeRectList.insert("VerticalPipe2", PipeRectAndOrientation(rect, "up", QPoint(-7, 0)));
    m_StationJointList.insert("S7RightPoint", S7RightPoint);

    //S13 right point
    QPointF S13RightPoint(S7RightPoint.rx(), m_StationJointList["S13"].ry());
    m_StationJointList.insert("S13RightPoint", S7RightPoint);

    this->CollectPipeRect();
    CreateAllPipe();

    // Add the End Time Widget
    mp_DashboardEndTimeWidget = new Dashboard::CDashboardEndTimeWidget(mp_DataConnector, mp_MainWindow);
    mp_DashboardEndTimeWidget->InitEndTimeWidgetItems();
    mp_GraphicsProxyWidget = this->addWidget(mp_DashboardEndTimeWidget);
    mp_GraphicsProxyWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    mp_GraphicsProxyWidget->setPos(m_DashboardEndTimeWidgetPos);
    addItem(mp_GraphicsProxyWidget);

    CONNECTSIGNALSIGNAL(mp_DashboardEndTimeWidget, OnSelectDateTime(const QDateTime &), this, OnSelectDateTime(const QDateTime &));
    CONNECTSIGNALSLOT(this, ProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool),
                      mp_DashboardEndTimeWidget, OnProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool));

    CONNECTSIGNALSLOT(this, ProgramActionStopped(DataManager::ProgramStatusType_t),
                      mp_DashboardEndTimeWidget, OnProgramActionStopped(DataManager::ProgramStatusType_t));

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

void CDashboardScene::UpdateDashboardSceneReagentsForProgram(QString &programId, int asapEndTime, QList<QString>& selectedStationList)
{
    DataManager::CProgram const *p_Program = NULL;

    if (m_CloneProgramList) {
        *mp_ProgramListClone = *(mp_DataConnector->ProgramList);
    }
    p_Program = mp_ProgramListClone->GetProgram(programId);
    if(p_Program) {
        mp_DashboardEndTimeWidget->UpdateEndTimeWidgetItems(p_Program, asapEndTime);
    }
    else
        mp_DashboardEndTimeWidget->InitEndTimeWidgetItems();

    for(int j = 0 ; j < m_DashboardStationList.count(); j++)
    {
        Core::CDashboardStationItem *pListItem =
            static_cast<Core::CDashboardStationItem *>(mp_DashboardStationItems[j]);

        if(!p_Program)//reset
        {
            pListItem->StationSelected(true);
        }
        else
        {
            if(selectedStationList.contains(pListItem->GetDashboardStation()->GetDashboardStationID()))
                pListItem->StationSelected(true);
            else
                pListItem->StationSelected(false);
        }
        pListItem->UpdateDashboardStationItemReagent();
    }
}

void CDashboardScene::UpdateRetortStatus(DataManager::ContainerStatusType_t retortStatusType)
{
    mp_DashboardStationRetort->SetContainerStatus(retortStatusType);
}

const QTime& CDashboardScene::GetStepRemainingTime()
{
    return mp_DashboardEndTimeWidget->GetStepRemainingTime();
}

const QTime CDashboardScene::GetProgramRemainingTime()
{
    return mp_DashboardEndTimeWidget->GetProgramRemainingTime();
}

const QString CDashboardScene::GetEndDateTime()
{
    return mp_DashboardEndTimeWidget->GetEndDateTime();
}

void CDashboardScene::OnPauseStationSuckDrain()
{
    m_pPipeAnimationTimer->stop();
    //find Station and let Station animation pause
   for (int i = 0; i < mp_DashboardStationItems.size(); i++)
   {
        Core::CDashboardStationItem* item = mp_DashboardStationItems.at(i);
        if (item->StationItemID() == m_SuckDrainStationId)
        {
            item->PauseSuckDrain();
            mp_DashboardStationRetort->StationSelected(true);
            mp_DashboardStationRetort->PauseSuckDrain();
            break;
        }
   }
}

void CDashboardScene::OnStationSuckDrain(const QString& StationId, bool IsStart, bool IsSuck)
{
   m_IsSuck = IsSuck;
   m_SuckDrainStationId = StationId;
   ClearCurrentWorkingPipe();
   if (IsStart)
   {
       RepresentCurrentWorkingPipe(StationId);
   }

   if (IsStart)
   {
       //Get Current selected reagent corlor for Retort's representation
      DataManager::CDashboardStation* pDashboardStation = NULL;
      QHash<QString, DataManager::CDashboardStation*>::const_iterator it = m_DashboardStationList.find(StationId);
      if(it != m_DashboardStationList.constEnd()) {
           pDashboardStation = it.value();
      }
      if (pDashboardStation)
      {
          QString ReagentID = pDashboardStation->GetDashboardReagentID();
          if ("" != ReagentID)
          {
              DataManager::CReagent const *p_Reagent = mp_DataConnector->ReagentList->GetReagent(ReagentID);
              if (p_Reagent)
              {
                  QString ReagentGroupId = p_Reagent->GetGroupID();
                  DataManager::CReagentGroup const *p_ReagentGroup = mp_DataConnector->ReagentGroupList->GetReagentGroup(ReagentGroupId);

                  m_CurrentReagentColorValue = p_ReagentGroup->GetGroupColor();
                  m_CurrentReagentColorValue.prepend("#");
              }
          }
      }

     m_pPipeAnimationTimer->start(500);
   }
   else
   {
        m_pPipeAnimationTimer->stop();
   }

    //find Station and let Station animation start/stop
   for (int i = 0; i < mp_DashboardStationItems.size(); i++)
   {
        Core::CDashboardStationItem* item = mp_DashboardStationItems.at(i);
        if (item->StationItemID() == StationId)
        {
            item->SuckDrain(IsStart, IsSuck);
            mp_DashboardStationRetort->StationSelected(true);
            mp_DashboardStationRetort->SuckDrain(IsStart, IsSuck, m_CurrentReagentColorValue);
            break;
        }
   }
}

bool CDashboardScene::HaveExpiredReagent()
{
    for (int i = 0; i < mp_DashboardStationItems.size(); i++)
    {
         Core::CDashboardStationItem* item = mp_DashboardStationItems.at(i);
         if (item)
         {
             if (item->GetStationSelect() && item->IsReagentExpired())
                 return true;
         }
    }
    return false;
}

} // end namespace Dashboard
