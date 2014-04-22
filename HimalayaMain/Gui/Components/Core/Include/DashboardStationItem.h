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
    Core::CDataConnector *mp_DataConnector;              //!< Global data container
    DataManager::CHimalayaUserSettings m_UserSettings;       ///<  Definition/Declaration of variable m_UserSettings
    Global::RMSOptions_t m_CurRMSMode;       ///<  Definition/Declaration of variable m_CurRMSMode
    DataManager::CDashboardStation *mp_DashboardStation; //!< Dashboard Station object
    QString m_StationItemID;       ///<  Definition/Declaration of variable m_StationItemID
    StationGroupType_t m_DashboardStationGroup;          //!< Dashboard StationGroup object
    QString m_DashboardStationID;                        //!< Dashboard Station ID
    QPixmap m_Image;                                     //!< Background image.
    QString m_ReagentId;                                 //!< Reagent ID
    QString m_StationItemLabel;                          //!< Station Label
    const int m_RetortBoundingRectWidth;                 //!< Retort Bounding Rect Width
    const int m_RetortBoundingRectHeight;                //!< Retort Bounding Rect Height
    const int m_RetortCoverHeight;                       //!< Retort Cover Height

    const int m_ParaffinbathBoundingRectWidth;           //!< Paraffinbath Bounding Rect Width
    const int m_ParaffinbathBoundingRectHeight;          //!< Paraffinbath Bounding Rect Height
    const int m_ParaffinbathCoverHeight;                 //!< Paraffinbath Cover Height

    const int m_BottleBoundingRectWidth;                 //!< Bottle Bounding Rect Width
    const int m_BottleBoundingRectHeight;                //!< Bottle Bounding Rect Height
    const int m_BottleCoverHeight;                       //!< Bottle Cover Height

    int m_CurrentBoundingRectReagentHeight;                //!< current container Bounding Rect Height

    QTimer* mp_SuckDrainTimer;       ///<  Definition/Declaration of variable mp_SuckDrainTimer
    bool m_ReagentExpiredFlag;                           //!< Indicates if the reagent in a station is expired.
    bool m_StationSelected;                              //!< Indicates if the station is selected for the program
    QString m_CurrentReagentColorValue;       ///<  Definition/Declaration of variable m_CurrentReagentColorValue
    QString m_ReagentDisplayColorValue;       ///<  Definition/Declaration of variable m_ReagentDisplayColorValue
    DataManager::ContainerStatusType_t m_ContainerStatusType;       ///<  Definition/Declaration of variable m_ContainerStatusType
    bool m_ExpiredColorRed;       ///<  Definition/Declaration of variable m_ExpiredColorRed
    bool m_EnableBlink;       ///<  Definition/Declaration of variable m_EnableBlink
    bool m_IsRetortContaminated;       ///<  Definition/Declaration of variable m_IsRetortContaminated
    bool m_RetortLocked;								 //!< Indicates if the retort is locked or not
    QPixmap m_RawImage4Cleaning;                         //!< Big Image with customized BDiagPattern for Cleaning reagent
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function OnCompletedSuckDrain
     *
     *  \return from OnCompletedSuckDrain
     */
    /****************************************************************************/
    void OnCompletedSuckDrain(bool isSuck);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DrawGlowBoundaryText
     *
     *  \return from DrawGlowBoundaryText
     */
    /****************************************************************************/
    void DrawGlowBoundaryText(QFont& textFont, QString& text, QRect& rect, QPainter& painter);
public:
    explicit CDashboardStationItem(Core::CDataConnector *p_DataConnector,                                   
                                   StationGroupType_t StationGroup, const QString& StationItemID, QString StationLabel,
                                   DataManager::CDashboardStation *p_DashboardStation = NULL,
                                   QGraphicsItem *p_Parent = NULL);

    virtual ~CDashboardStationItem();
    DataManager::CDashboardStation *GetDashboardStation() const;
    void SetDashboardStation(DataManager::CDashboardStation *p_DashboardStation);
    bool IsEmpty();
    void LoadStationImages(QPainter & Painter);
    void DrawReagentName(QPainter & Painter);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DrawStationItemLabel
     *
     *  \return from DrawStationItemLabel
     */
    /****************************************************************************/
    void DrawStationItemLabel(QPainter & painter);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function FillReagentColor
     *
     *  \return from FillReagentColor
     */
    /****************************************************************************/
    void FillReagentColor(QPainter & Painter);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetContainerStatus
     *
     *  \return from SetContainerStatus
     */
    /****************************************************************************/
    void SetContainerStatus(DataManager::ContainerStatusType_t containerStatus);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetContainerRetortLockedStatus
     *
     *  \return from SetContainerRetortLockedStatus
     */
    /****************************************************************************/
    void SetContainerRetortLockedStatus(bool locked);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnableBlink
     *
     *  \return from EnableBlink
     */
    /****************************************************************************/
    void EnableBlink(bool bEnable);
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
     */
    /****************************************************************************/
    StationGroupType_t GetStationGroup(){ return m_DashboardStationGroup; }

    QRectF boundingRect() const;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function PauseSuckDrain
     *
     *  \return from PauseSuckDrain
     */
    /****************************************************************************/
    void PauseSuckDrain();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SuckDrain
     *
     *  \return from SuckDrain
     */
    /****************************************************************************/
    void SuckDrain(bool isStart = true, bool isSuck = true, const QString& ReagentColorValue = "#FFFFFF");
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetStationItemID
     *
     *  \return from GetStationItemID
     */
    /****************************************************************************/
    const QString& GetStationItemID() const;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsReagentExpired
     *
     *  \return from IsReagentExpired
     */
    /****************************************************************************/
    bool IsReagentExpired();
protected:
    void paint(QPainter *p_Painter, const QStyleOptionGraphicsItem *, QWidget *);

signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is sent when the station item is released
     */
    /****************************************************************************/
    void Released();

public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateImage();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateDashboardScene(QString StationID);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateDashboardStationItemReagent(bool RefreshFlag=true);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void SuckDrainAnimation();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void DrawStationItemImage();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateDashboardStationItemReagentWhenReagentUpdated();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateDashboardStationItemWhenReagentGroupUpdated();

private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateUserSettings();
};

} // end namespace Core

#endif // CORE_DASHBOARDSTATIONITEM_H

