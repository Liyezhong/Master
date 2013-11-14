/****************************************************************************/
/*! \file DashboardStationItem.h
 *
 *  \brief DashboardStationItem definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 16-Jan-2013
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

#ifndef CORE_DASHBOARDSTATIONITEM_H
#define CORE_DASHBOARDSTATIONITEM_H

#include <QGraphicsItem>
#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/MessageDlg.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardStation.h"
#include "Reagents/Include/ReagentRMSWidget.h"
#include "HimalayaDataContainer/Helper/Include/Global.h"

#include <QGraphicsSceneMouseEvent>


typedef enum {
    STATIONS_GROUP_RETORT,
    STATIONS_GROUP_PARAFFINBATH,
    STATIONS_GROUP_BOTTLE,
    STATIONS_GROUP_INVALID
}StationGroupType_t;


namespace Core {


/****************************************************************************/
/**
 * \brief This class is the graphicsm::UpdateImage,initializer)
 *
 */
/****************************************************************************/
class CDashboardStationItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *p_Event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *p_Event);

    Core::CDataConnector *mp_DataConnector;              //!< Global data container
    DataManager::CHimalayaUserSettings m_UserSettings;
    Global::RMSOptions_t m_CurRMSMode;
    DataManager::CDashboardStation *mp_DashboardStation; //!< Dashboard Station object
    QString m_ImageType;                                 //!< Containing the station type for the image filename
    QString m_BorderImage;                               //!< Containing the selection border image filename
    bool m_Animation;                                    //!< Turns on animation of station blink when true.
    bool m_Enabled;                                      //!< Indicates if the station is enabled
    bool m_Pressed;                                      //!< Indicates if the station is pressed
    QString m_StationItemID;
    bool m_GridDrawn;                                    //!< Flag to verify whether the grid is drawn or not
    StationGroupType_t m_DashboardStationGroup;          //!< Dashboard StationGroup object
    QString m_DashboardStationID;                        //!< Dashboard Station ID
    QPixmap m_Image;                                     //!< Background image.
    QString m_ReagentId;                                 //!< Reagent ID
    QString m_StationItemLabel;                          //!< Station Label
    const int m_RetortBoundingRectWidth;                 //!< Retort Bounding Rect Width
    const int m_RetortBoundingRectHeight;                //!< Retort Bounding Rect Height
    const int m_ParaffinbathBoundingRectWidth;           //!< Paraffinbath Bounding Rect Width
    const int m_ParaffinbathBoundingRectHeight;          //!< Paraffinbath Bounding Rect Height
    const int m_BottleBoundingRectWidth;                 //!< Bottle Bounding Rect Width
    const int m_BottleBoundingRectHeight;                //!< Bottle Bounding Rect Height

    int m_CurrentBoundingRectReagentHeight;                //!< current container Bounding Rect Height

    QTimer* mp_SuckDrainTimer;
    bool m_ReagentExpiredFlag;                           //!< Indicates if the reagent in a station is expired.
    bool m_StationSelected;                              //!< Indicates if the station is selected for the program
    QString m_CurrentReagentColorValue;
    DataManager::ContainerStatusType_t m_ContainerStatusType;
    bool m_ExpiredColorRed;
    bool m_IsRetortContaminated;
    void OnCompletedSuckDrain(bool isSuck);
public:
    explicit CDashboardStationItem(Core::CDataConnector *p_DataConnector,                                   
                                   StationGroupType_t StationGroup, const QString& StationItemID, QString StationLabel,
                                   bool Animation,
                                   DataManager::CDashboardStation *p_DashboardStation = NULL,
                                   QGraphicsItem *p_Parent = NULL);

    virtual ~CDashboardStationItem();
    DataManager::CDashboardStation *GetDashboardStation() const;
    void SetDashboardStation(DataManager::CDashboardStation *p_DashboardStation);
    QString GetImageType() const;
    void SetImageType(const QString &ImageType);
    void SetBorderImage(const QString &ImageType);
    QString GetBorderImage() const;
    void SetEnabled(bool Enabled);
    void SetPressed(bool Press);
    bool IsEmpty();
    void LoadStationImages(QPainter & Painter);
    void DrawReagentName(QPainter & Painter);
    void DrawStationItemLabel(QPainter & Painter);
    void FillReagentColor(QPainter & Painter);
    void SetContainerStatus(DataManager::ContainerStatusType_t containerStatus);
    /****************************************************************************/
    /*!
     *  \brief Sets the selected station
     *
     *  \iparam Selected = True if station is selected else False.
     */
    /****************************************************************************/
    void StationSelected(bool Selected){ m_StationSelected = Selected; }


    /****************************************************************************/
    /*!
     *  \brief Gets station selected
     *
     *  \oparam m_Selected = True if station is selected else False.
     */
    /****************************************************************************/
    bool GetStationSelect(){return m_StationSelected;}

    /****************************************************************************/
    /*!
     *  \brief Sets station gruop for a station
     *
     *  \iparam StationGroup = StationGruop type
     */
    /****************************************************************************/
    void SetStationGroup(StationGroupType_t StationGroup) { m_DashboardStationGroup = StationGroup; }


    /****************************************************************************/
    /*!
     *  \brief Gets Station group type
     *
     *  \iparam StationGroupType_t = Station Group type.
     */
    /****************************************************************************/
    StationGroupType_t GetStationGroup(){ return m_DashboardStationGroup; }

    QRectF boundingRect() const;
    void PauseSuckDrain();
    void SuckDrain(bool isStart = true, bool isSuck = true, const QString& ReagentColorValue = "#FFFFFF");
    const QString& GetStationItemID() const;
    bool IsReagentExpired();
protected:
    void paint(QPainter *p_Painter, const QStyleOptionGraphicsItem *, QWidget *);

signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is sent when the station item is pressed
     */
    /****************************************************************************/
    void Pressed();
    /****************************************************************************/
    /*!
     *  \brief This signal is sent when the station item is released
     */
    /****************************************************************************/
    void Released();

public slots:
    void UpdateImage();
    void UpdateDashboardScene(QString StationID);
    void UpdateDashboardStationItemReagent();
    void SuckDrainAnimation();
    void DrawStationItemImage();

private slots:
    void UpdateUserSettings();
};

} // end namespace Core

#endif // CORE_DASHBOARDSTATIONITEM_H

