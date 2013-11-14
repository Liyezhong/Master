/****************************************************************************/
/*! \file DashboardStationItem.cpp
 *
 *  \brief DashboardStationItem implementation.
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

#include "Core/Include/DashboardStationItem.h"
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"



namespace Core {


//!< Self explaining. Just a random value.
#define ANIMATION_COUNT_MAX 10000

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_DataConnector = Instance of Data Container
 *  \iparam p_Parent = Parent item
 */
/****************************************************************************/
CDashboardStationItem::CDashboardStationItem(Core::CDataConnector *p_DataConnector,
                                             StationGroupType_t StationGroup,
                                             const QString& StationItemID,
                                             QString StationLabel,
                                             bool Animation,
                                             DataManager::CDashboardStation *p_DashboardStation,
                                             QGraphicsItem *p_Parent) : QGraphicsItem(p_Parent),
    mp_DataConnector(p_DataConnector),
       m_CurRMSMode(Global::RMS_UNDEFINED),
       mp_DashboardStation(p_DashboardStation),
       m_Animation(Animation),
       m_Enabled(true),
       m_Pressed(false),
       m_StationItemID(StationItemID),
    m_DashboardStationGroup(StationGroup),
    m_StationItemLabel(StationLabel),
    m_RetortBoundingRectWidth(180),
    m_RetortBoundingRectHeight(96),
    m_ParaffinbathBoundingRectWidth(120),
    m_ParaffinbathBoundingRectHeight(70),
    m_BottleBoundingRectWidth(61),
    m_BottleBoundingRectHeight(143),
    m_ReagentExpiredFlag(false),
    m_StationSelected(false),
    m_ContainerStatusType(DataManager::CONTAINER_STATUS_FULL),
    m_ExpiredColorRed(true),
    m_IsRetortContaminated(false)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    setCacheMode(QGraphicsItem::ItemCoordinateCache);

    if(m_DashboardStationGroup == STATIONS_GROUP_RETORT) {
        m_Image = QPixmap(m_RetortBoundingRectWidth, m_RetortBoundingRectHeight);
    } else if(m_DashboardStationGroup == STATIONS_GROUP_PARAFFINBATH) {
        m_Image = QPixmap(m_ParaffinbathBoundingRectWidth, m_ParaffinbathBoundingRectHeight);
    } else {
        m_Image = QPixmap(m_BottleBoundingRectWidth, m_BottleBoundingRectHeight);
    }


    UpdateImage();

    mp_SuckDrainTimer = new QTimer(this);
    mp_SuckDrainTimer->setInterval(500);
    CONNECTSIGNALSLOT(mp_SuckDrainTimer, timeout(), this, SuckDrainAnimation());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDashboardStationItem::~CDashboardStationItem()
{
    try {
        mp_DataConnector = NULL;
        mp_DashboardStation = NULL;
        delete mp_SuckDrainTimer;
    } catch(...) {

    }

}

/****************************************************************************/
/*!
 *  \brief Gets the bounding rectangle
 *
 *  \return Bounding rectangle
 */
/****************************************************************************/
QRectF CDashboardStationItem::boundingRect() const
{
    if(STATIONS_GROUP_RETORT == m_DashboardStationGroup) {
        return QRectF(0, 0, m_RetortBoundingRectWidth, m_RetortBoundingRectHeight);
    } else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup) {
        return QRectF(0, 0, m_ParaffinbathBoundingRectWidth, m_ParaffinbathBoundingRectHeight);
    } else {
        return QRectF(0, 0, m_BottleBoundingRectWidth, m_BottleBoundingRectHeight);
    }
}

/****************************************************************************/
/*!
 *  \brief Paint the item
 *
 *  \iparam p_Painter = Pointer to the painter object
 */
/****************************************************************************/
void CDashboardStationItem::paint(QPainter *p_Painter, const QStyleOptionGraphicsItem *,
                                  QWidget *)
{

    p_Painter->drawPixmap(0, 0, m_Image);

    if (!mp_DashboardStation) {
        return;
    }

}

/****************************************************************************/
/*!
 *  \brief Updated the station item
 *
 */
/****************************************************************************/
void CDashboardStationItem::UpdateImage()
{
    qDebug() << "STATION GROUP = " << (int)m_DashboardStationGroup;
    if (!mp_DashboardStation)
    {
        //Painter for Drawing the Bakground rectangle, Loading the Static Images and Drawingthe Static Label
        QPainter Painter(&m_Image);

        if (!m_GridDrawn)
        {
            m_GridDrawn = true;
        }
        LoadStationImages(Painter);
        return;
    }

    CONNECTSIGNALSLOT(mp_DataConnector, ReagentsUpdated(), this, UpdateDashboardStationItemReagent());
    CONNECTSIGNALSLOT(mp_DataConnector, DashboardStationChangeReagent(QString), this, UpdateDashboardScene(QString));
    CONNECTSIGNALSLOT(mp_DataConnector, UserSettingsUpdated(), this, UpdateUserSettings());
    update();

}

/****************************************************************************/
/*!
 *  \brief Gets the type string for the image filename
 *
 *  \return Image type string
 */
/****************************************************************************/
QString CDashboardStationItem::GetImageType() const
{
    return m_ImageType;
}

/****************************************************************************/
/*!
 *  \brief Sets the type string for the image filename
 *
 *  \iparam ImageType = Image type string
 */
/****************************************************************************/
void CDashboardStationItem::SetImageType(const QString& ImageType)
{
    m_ImageType = ImageType;
    UpdateImage();
    update();
}

/****************************************************************************/
/*!
*  \brief Gets the type string for the border image filename
*
*  \return Image type string
*/
/****************************************************************************/
QString CDashboardStationItem::GetBorderImage() const
{
    return m_BorderImage;
}

/****************************************************************************/
/*!
 *  \brief Sets the type string for the border image filename
 *
 *  \iparam ImageType = Image type string
 */
/****************************************************************************/
void CDashboardStationItem::SetBorderImage(const QString &ImageType)
{
    m_BorderImage = ImageType;
    update();
}

/****************************************************************************/
/*!
 *  \brief Gets the data manager station data object
 *
 *  \return Station data object
 */
/****************************************************************************/
DataManager::CDashboardStation *CDashboardStationItem::GetDashboardStation() const
{
    if (mp_DashboardStation != NULL) {
        return mp_DashboardStation;
    }
    else {
        return NULL;
    }
}

/****************************************************************************/
/*!
 *  \brief Sets the data manager station data object
 *
 *  \iparam p_Station = Station data object
 */
/****************************************************************************/
void CDashboardStationItem::SetDashboardStation(DataManager::CDashboardStation* p_DashboardStation)
{
    mp_DashboardStation = p_DashboardStation;
    if(mp_DashboardStation) {
        m_DashboardStationID = mp_DashboardStation->GetDashboardStationID();
        UpdateImage();
    }
}


/****************************************************************************/
/*!
 *  \brief Enables or disables the station
 *
 *  \iparam Enabled = Enable (true) or disable (false)
 */
/****************************************************************************/
void CDashboardStationItem::SetEnabled(bool Enabled)
{
    m_Enabled = Enabled;
    update();
}

/****************************************************************************/
/*!
 *  \brief Changes whether the station is pressed or not
 *
 *  \iparam Press = Pressed (true) or not (false)
 */
/****************************************************************************/
void CDashboardStationItem::SetPressed(bool Press)
{
    m_Pressed = Press;
    update();
}

/****************************************************************************/
/*!
 *  \brief Changes whether the station is pressed or not
 *
 *  \return Empty (true) or not (false)
 */
/****************************************************************************/
bool CDashboardStationItem::IsEmpty()
{
    if (mp_DashboardStation) {
        return true;
    } else {
        return false;
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for mouse press events
 *
 *  \iparam p_Event = Mouse event
 */
/****************************************************************************/
void CDashboardStationItem::mousePressEvent(QGraphicsSceneMouseEvent *p_Event)
{
    emit Pressed();
    m_Pressed = true;
    p_Event->accept();
    update();
}

/****************************************************************************/
/*!
 *  \brief Event handler for mouse release events
 *
 *  \iparam p_Event = Mouse event
 */
/****************************************************************************/
void CDashboardStationItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *p_Event)
{
    emit Released();
    if(isSelected() == false) {
        m_Pressed = false;
    }
    p_Event->accept();
    update();
}


/****************************************************************************/
/*!
 *  \brief Load the Initial Images of Stations
 *
 */
/****************************************************************************/

void CDashboardStationItem::LoadStationImages(QPainter& Painter)
{
    if(STATIONS_GROUP_RETORT == m_DashboardStationGroup) {
        Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Retort/Retort_Enabled.png"));        
    } else if( STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup) {
        if (m_StationSelected) {
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Background.png"));
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Cover.png"));
        } else {
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Background_Grayed.png"));
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Cover_Grayed.png"));
        }
    } else {
        if(m_StationSelected) {
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Background.png"));
            //Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Cover.png"));
        } else {
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Background_Grayed.png"));
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Cover_Grayed.png"));
        }

    }
    DrawStationItemLabel(Painter);
    update();
}

/****************************************************************************/
/*!
 *  \brief Update Dashboard Station Items Reagent Properties when Reagents are updated
 *
 */
/****************************************************************************/

void CDashboardStationItem::UpdateDashboardStationItemReagent()
{
    if (!mp_DashboardStation)
        return;
    QString ReagentID = mp_DashboardStation->GetDashboardReagentID();
    DataManager::CReagent *p_Reagent = const_cast<DataManager::CReagent*>(mp_DataConnector->ReagentList->GetReagent(ReagentID));

    if (p_Reagent)
    {
        if(m_CurRMSMode != m_UserSettings.GetModeRMSProcessing())
        {
            m_CurRMSMode = m_UserSettings.GetModeRMSProcessing();
            switch( m_UserSettings.GetModeRMSProcessing())
            {
                case Global::RMS_CASSETTES:
                {
                    qint32 ReagentActualCassettes = mp_DashboardStation->GetDashboardReagentActualCassettes();
                    int ReagentMaxCassettes = p_Reagent->GetMaxCassettes();
                    if((ReagentMaxCassettes - ReagentActualCassettes) < 0)
                        m_ReagentExpiredFlag = true;
                    else
                        m_ReagentExpiredFlag = false;
                }
                break;
                case Global::RMS_CYCLES:
                {
                    qint32 ReagentActualCycles = mp_DashboardStation->GetDashboardReagentActualCycles();
                    int ReagentMaxCycles = p_Reagent->GetMaxCycles();
                    if((ReagentMaxCycles - ReagentActualCycles) < 0)
                        m_ReagentExpiredFlag = true;
                    else
                        m_ReagentExpiredFlag = false;
                }
                break;
                case Global::RMS_DAYS:
                {
                    QDate curDate;
                    QDate ReagentExchangeQDate = mp_DashboardStation->GetDashboardReagentExchangeDate();
                    QDate ReagentExpiryQDate = ReagentExchangeQDate.addDays(p_Reagent->GetMaxDays());
                    int Days_Overdue = curDate.currentDate().dayOfYear() - ReagentExpiryQDate.dayOfYear();

                    if(Days_Overdue > 0)
                        m_ReagentExpiredFlag = true;
                    else
                        m_ReagentExpiredFlag = false;
                }
                break;
                case Global::RMS_OFF:
                {
                    m_ReagentExpiredFlag = false;
                }
                break;
                default:
                {
                    qDebug() << "Do Nothing";
                }
            }
        }
    }

    if(true == m_ReagentExpiredFlag) {
        } else {
        DrawStationItemImage(); // No Blinking
    }
}

void CDashboardStationItem::DrawStationItemImage()
{
    QPainter Painter(&m_Image);

    Painter.eraseRect(boundingRect());

    LoadStationImages(Painter);

    QString ReagentStatus = "";
    if (mp_DashboardStation)
        ReagentStatus = mp_DashboardStation->GetDashboardReagentStatus();

    // If Reagent Status is Not Empty and Station Selected for the Program then fill the Reagent Color
    if(0 != ReagentStatus.compare("Empty", Qt::CaseInsensitive)) {
        FillReagentColor(Painter);
    }

    if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
    {
        DrawReagentName(Painter);
    }
    DrawStationItemLabel(Painter);
    update();
}

/****************************************************************************/
/*!
 *  \brief Draw Reagent Name on the Bottle Image
 *
 */
/****************************************************************************/

void CDashboardStationItem::DrawReagentName(QPainter & Painter)
{
    QFont TextFont;
    Painter.setRenderHint(QPainter::Antialiasing);
    Painter.setPen(Qt::black);
    TextFont.setPointSize(9);

    if(m_StationSelected) {
        TextFont.setBold(true);
    }

    Painter.setFont(TextFont);

    if (mp_DashboardStation)
    {
        //Now Paint Reagent Names
        QString ReagentID = mp_DashboardStation->GetDashboardReagentID();
        DataManager::CReagent const *p_Reagent = mp_DataConnector->ReagentList->GetReagent(ReagentID);
        if (p_Reagent)
        {
            Painter.rotate(270.0);
            Painter.drawText(QRect(-90, -3, 75, 60 ), (int)Qt::TextWordWrap | (int)Qt::AlignVCenter | (int)Qt::AlignLeft, p_Reagent->GetReagentName());
            Painter.rotate(-270.0);
        }
    }
    update();
}

void CDashboardStationItem::DrawStationItemLabel(QPainter &Painter)
{
    QFont TextFont;
    Painter.setRenderHint(QPainter::Antialiasing);
    Painter.setPen(Qt::black);
    TextFont.setPointSize(10);

    if(m_StationSelected) {
        TextFont.setBold(true);
    }

    Painter.setFont(TextFont);

   if(STATIONS_GROUP_RETORT == m_DashboardStationGroup) {
       Painter.drawText(QRectF(32,5,67,15), (int)Qt::AlignCenter, m_StationItemLabel);
   } else if( STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup) {
       // Load the Cover Image , because it will not be visible after filling the Color.
       Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Cover.png"));
       Painter.drawText(QRectF(13,7,67,15), (int)Qt::AlignCenter, m_StationItemLabel);
   } else {
       if(m_StationSelected) {
           //Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Cover.png"));
       } else {
           Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Cover_Grayed.png"));
       }

       Painter.drawText(QRectF(10,1,33,16), (int)Qt::AlignCenter, m_StationItemLabel);
   }
   update();
}

void CDashboardStationItem::FillReagentColor(QPainter & Painter)
{
    int fillBottleWidth;
    int fillBottleHeight;
    int fillParaffinbathWidth;
    int fillParafinbathHeight;
    int fillRetortWidth;
    int fillRetortHeight;
    QString ReagentColorValue;
    if(mp_DashboardStation)
    {
        QString ReagentID = mp_DashboardStation->GetDashboardReagentID();
        if ("" == ReagentID)
            return;
        DataManager::CReagent const *p_Reagent = mp_DataConnector->ReagentList->GetReagent(ReagentID);
        if (!p_Reagent)
            return;
        QString ReagentGroupId = p_Reagent->GetGroupID();
        DataManager::CReagentGroup const *p_ReagentGroup = mp_DataConnector->ReagentGroupList->GetReagentGroup(ReagentGroupId);

        ReagentColorValue = p_ReagentGroup->GetGroupColor();
        (void)ReagentColorValue.prepend("#");
    }
    else
    {
        ReagentColorValue = m_CurrentReagentColorValue;
    }
    if(m_StationSelected) {
        Painter.setRenderHint(QPainter::Antialiasing);
        Painter.setPen(QColor(ReagentColorValue));
        QColor color(ReagentColorValue);
        if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup || STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
        {
            if(true == m_ReagentExpiredFlag && m_StationSelected)
            {
                if (m_ExpiredColorRed)
                {
                    m_ExpiredColorRed = false;
                }
                else
                {
                    color.setRgb(240, 175, 82);
                    m_ExpiredColorRed = true;
                }
            }
        }

        Painter.setBrush(color);
    } else {
        Painter.setRenderHint(QPainter::Antialiasing);
        Painter.setPen(QColor(Qt::gray));
        Painter.setBrush(QColor(Qt::gray));
    }

    Painter.rotate(270.0);

    if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
    {
        // Since the Painter is rotated, Width and Height Axis Changes.
        fillBottleWidth = (m_BottleBoundingRectHeight - 40);  // Manual Pixel Calculation
        fillBottleHeight = (m_BottleBoundingRectWidth- 10);  // Manual Pixel Calculation
        if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_SCUKING
        || m_ContainerStatusType == DataManager::CONTAINER_STATUS_DRAINING)
        {
            fillBottleWidth = m_CurrentBoundingRectReagentHeight;
        }
        else if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_FULL)
        {
            m_CurrentBoundingRectReagentHeight = fillBottleWidth;
        }

        QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        path.addRoundedRect(QRect(-137, 4, fillBottleWidth, fillBottleHeight), 8, 8);
        int cornerHeight = 8;
        if (fillBottleWidth > cornerHeight && fillBottleWidth < m_BottleBoundingRectHeight - 25 - cornerHeight + 4)
        {
            path.addRect(QRect((-137 + fillBottleWidth - 8), 4, cornerHeight, 8));// Top left corner not rounded
            path.addRect(QRect((-137 + fillBottleWidth - 8), (4 + fillBottleHeight - 8), cornerHeight, 8));// Top right corner not rounded
        }
        Painter.drawPath(path);  // Only the Bottome Left and Bottom Right Corner
    }
    else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
    {
        fillParaffinbathWidth = (m_ParaffinbathBoundingRectHeight - 11);
        fillParafinbathHeight = (m_ParaffinbathBoundingRectWidth - 10);
        if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_SCUKING
        || m_ContainerStatusType == DataManager::CONTAINER_STATUS_DRAINING)
        {
            fillParaffinbathWidth = m_CurrentBoundingRectReagentHeight;
        }
        else if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_FULL)
        {
            m_CurrentBoundingRectReagentHeight = fillParaffinbathWidth;
        }

        QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        path.addRoundedRect(QRect(-62, 5, fillParaffinbathWidth, fillParafinbathHeight), 8, 4);
        int cornerHeight = 8;
        if (fillParaffinbathWidth > cornerHeight)
        {
            path.addRect(QRect((-62 + fillParaffinbathWidth - 8), 5, cornerHeight, 8));// Top left corner not rounded
            path.addRect(QRect((-62 + fillParaffinbathWidth - 8), (5 + fillParafinbathHeight - 8), cornerHeight, 8));// Top right corner not rounded
        }
        Painter.drawPath(path.simplified());  // Only the Bottome Left and Bottom Right Corner

    }
    else if(STATIONS_GROUP_RETORT == m_DashboardStationGroup)
    {
        fillRetortHeight = m_RetortBoundingRectWidth;
        fillRetortWidth = m_CurrentBoundingRectReagentHeight;

        QPainterPath path;
        path.setFillRule( Qt::WindingFill);
        if (m_IsRetortContaminated)//Retort CONTAMINATED
        {
            path.addRect((-75 + fillRetortWidth), 3, m_RetortBoundingRectHeight - 36, 3);
            path.addRect((-75 + fillRetortWidth), (fillRetortHeight - 6), m_RetortBoundingRectHeight - 36, 3);
        }

        if (fillRetortWidth > 0)
            path.addRoundedRect(QRect(-75, 3, fillRetortWidth, fillRetortHeight - 5), 8, 8);

        int cornerHeight = 4;
        if (fillRetortWidth > cornerHeight)
        {
            path.addRect(QRect((-79 + fillRetortWidth), 3, cornerHeight, 8));// Top left corner not rounded
            path.addRect(QRect((-79 + fillRetortWidth), (fillRetortHeight - 10), cornerHeight, 8));// Top right corner not rounded
        }
        Painter.drawPath(path);  // Only the Bottome Left and Bottom Right Corner

    }

    Painter.rotate(-270.0);

    update();
}

/****************************************************************************/
/*!
 *  \brief Slot for handling StationChangeReagent(Station Set As Empty,
 *  Set As Full, Reset Data , Change Reagent) Commands
 *
 *  \iparam StationID = Station ID
 */
/****************************************************************************/
void CDashboardStationItem::UpdateDashboardScene(QString StationID)
{
    mp_DashboardStation = mp_DataConnector->DashboardStationList->GetDashboardStation(m_DashboardStationID);

    if (!mp_DashboardStation) {
        return;
    }
    else {
        if (mp_DashboardStation->GetDashboardStationID() == StationID) {
            qDebug() << "Reagent Status : " << mp_DashboardStation->GetDashboardReagentStatus();
        }

        UpdateDashboardStationItemReagent();

        update();
    }
}

void CDashboardStationItem::UpdateUserSettings()
{
    m_UserSettings = *(mp_DataConnector->SettingsInterface->GetUserSettings());
    UpdateDashboardStationItemReagent();

}

void CDashboardStationItem::SuckDrain(bool isStart, bool isSuck, const QString& ReagentColorValue)
{
    if (isSuck)
        m_ContainerStatusType = DataManager::CONTAINER_STATUS_SCUKING;
    else
        m_ContainerStatusType = DataManager::CONTAINER_STATUS_DRAINING;

    m_CurrentReagentColorValue = ReagentColorValue;

    if (m_StationItemID == "Retort")
    {
        if (isSuck)
            m_CurrentBoundingRectReagentHeight = 0;
        else
            m_CurrentBoundingRectReagentHeight = m_RetortBoundingRectHeight - 33;
    }
    else
    {
        if (!isSuck)//draining
            m_CurrentBoundingRectReagentHeight = 0;
    }

    if (isStart)
    {
        mp_SuckDrainTimer->start();
    }
    else //finished
    {
        mp_SuckDrainTimer->stop();
        OnCompletedSuckDrain(isSuck);
    }
}

void CDashboardStationItem::PauseSuckDrain()
{
    mp_SuckDrainTimer->stop();
}

void CDashboardStationItem::OnCompletedSuckDrain(bool isSuck)
{
    if (m_StationItemID == "Retort")
    {
        if (isSuck)
            m_CurrentBoundingRectReagentHeight = m_RetortBoundingRectHeight - 33;
        else
            m_CurrentBoundingRectReagentHeight = 0;
    }
    else
    {

        if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
        {
            m_CurrentBoundingRectReagentHeight = m_BottleBoundingRectHeight - 25;
        }
        else if (STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
        {
            m_CurrentBoundingRectReagentHeight = m_ParaffinbathBoundingRectHeight - 8;
        }

        if (isSuck)
            m_CurrentBoundingRectReagentHeight = 0;
    }

    DrawStationItemImage();
}

void CDashboardStationItem::SuckDrainAnimation()
{
    if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_SCUKING)
    {
        if (m_StationItemID == "Retort")
        {
            if (m_CurrentBoundingRectReagentHeight < m_RetortBoundingRectHeight -33)
            {
                m_CurrentBoundingRectReagentHeight = m_CurrentBoundingRectReagentHeight + 1;
            }
            else
                m_CurrentBoundingRectReagentHeight = 0;//reset

        }
        else //other container
        {
            if (m_CurrentBoundingRectReagentHeight > 0)
            {
                m_CurrentBoundingRectReagentHeight = m_CurrentBoundingRectReagentHeight - 1;
            }
            else //reset
            {
                if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
                {
                    m_CurrentBoundingRectReagentHeight = m_BottleBoundingRectHeight - 25;
                }
                else if (STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
                {
                    m_CurrentBoundingRectReagentHeight = m_ParaffinbathBoundingRectHeight - 8;
                }
            }
        }
    }
    else if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_DRAINING)//drain
    {
        if (m_StationItemID == "Retort")
        {
            if (m_CurrentBoundingRectReagentHeight > 0)
            {
                m_CurrentBoundingRectReagentHeight = m_CurrentBoundingRectReagentHeight - 1;
            }
            else // reset
            {
                m_CurrentBoundingRectReagentHeight = m_RetortBoundingRectHeight - 33;
            }
        }
        else
        {
            int currentLiquidLevel(0);
            if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
            {
                 currentLiquidLevel = m_BottleBoundingRectHeight - 25;
            }
            else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
            {
                 currentLiquidLevel = m_ParaffinbathBoundingRectHeight - 8;// stop animation when touch to current LiquidLevel
            }

            if (m_CurrentBoundingRectReagentHeight < currentLiquidLevel)
            {
                m_CurrentBoundingRectReagentHeight = m_CurrentBoundingRectReagentHeight + 1;
            }
            else //reset
                m_CurrentBoundingRectReagentHeight = 0;
        }
    }

    DrawStationItemImage();
}

const QString& CDashboardStationItem::GetStationItemID() const
{
    return m_StationItemID;
}

bool CDashboardStationItem::IsReagentExpired()
{
    UpdateDashboardStationItemReagent();
    return m_ReagentExpiredFlag;
}

void CDashboardStationItem::SetContainerStatus(DataManager::ContainerStatusType_t containerStatus)
{
    if (DataManager::CONTAINER_STATUS_CONTAMINATED == containerStatus)
    {
        m_IsRetortContaminated = true;
        m_CurrentBoundingRectReagentHeight  = 3;
    }
    else if (DataManager::CONTAINER_STATUS_EMPTY == containerStatus)
    {
        m_CurrentBoundingRectReagentHeight  = 0;
    }

    DrawStationItemImage();
    m_IsRetortContaminated = false;
}

} // end namespace Core
