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
#include <QDebug>
#include <QPainter>


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
                                             QString StationLabel,
                                             bool Animation,
                                             DataManager::CDashboardStation *p_DashboardStation,
                                             QGraphicsItem *p_Parent) : QGraphicsItem(p_Parent),
    mp_DataConnector(p_DataConnector),
    m_DashboardStationGroup(StationGroup),
    m_StationItemLabel(StationLabel),
    m_Animation(Animation),
    mp_DashboardStation(p_DashboardStation),
    m_RetortBoundingRectWidth(125),
    m_RetortBoundingRectHeight(78),
    m_ParaffinbathBoundingRectWidth(122),
    m_ParaffinbathBoundingRectHeight(60),
    m_BottleBoundingRectWidth(79),
    m_BottleBoundingRectHeight(97),
    m_ReagentExpiredFlag(false),
    m_BlinkingCounter(1),
    m_Enabled(true),
    m_Pressed(false),
    m_StationSelected(false),
    m_CurRMSMode(Global::RMS_UNDEFINED)

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

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDashboardStationItem::~CDashboardStationItem()
{
    try {
        delete mp_BlinkingTimer;
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

    m_BlinkingCounter++;
    if (!(m_BlinkingCounter % ANIMATION_COUNT_MAX)) {
        m_BlinkingCounter = 0;
    }

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
    qDebug() << "STATION GROUP = " << m_DashboardStationGroup;
    if (!mp_DashboardStation)
    {
        //Painter for Drawing the Bakground rectangle, Loading the Static Images and Drawingthe Static Label
        QPainter Painter(&m_Image);

        if (!m_GridDrawn)
        {
            DrawBackgroundRectangle(Painter);
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
 *  \brief Draw Background Rectangle to place the Dashboard Station Items
 *
 *
 */
/****************************************************************************/
void CDashboardStationItem::DrawBackgroundRectangle(QPainter & Painter)
{
    Painter.setRenderHint(QPainter::Antialiasing);
    Painter.setPen(Qt::white);
    Painter.setBrush(QColor(Qt::white));
    Painter.drawRect(boundingRect()); //Draw Rectangle here
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
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Cover.png"));
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
                default:
                {
                    qDebug() << "Do Nothing";
                }
            }
        }
    }

    if(true == m_ReagentExpiredFlag) {
        mp_BlinkingTimer = new QTimer();
        mp_BlinkingTimer->start(500);
        CONNECTSIGNALSLOT(mp_BlinkingTimer, timeout(), this, DrawStationItemImage());
    } else {
        DrawStationItemImage(); // No Blinking
    }
}

void CDashboardStationItem::DrawStationItemImage()
{
    QPainter Painter(&m_Image);

    Painter.eraseRect(boundingRect());

    LoadStationImages(Painter);

    QString ReagentStatus = mp_DashboardStation->GetDashboardReagentStatus();

    // If Reagent Status is Not Empty and Station Selected for the Program then fill the Reagent Color
    if(0 != ReagentStatus.compare("Empty", Qt::CaseInsensitive)) {
        FillReagentColor(Painter);
    }

    if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
    {
        if(true == m_ReagentExpiredFlag && m_StationSelected && (m_BlinkingCounter % 2))
        {
            //Painter.drawPixmap((m_BottleBoundingRectWidth - 72) , (m_BottleBoundingRectHeight - 75), QPixmap(":/HimalayaImages/Icons/Dashboard/Expiry/Expiry_Expired_Large.png"));
            Painter.drawPixmap((m_BottleBoundingRectWidth - 67) , (m_BottleBoundingRectHeight - 65), QPixmap(":/HimalayaImages/Icons/Dashboard/Expiry/Expiry_Expired_Small.png"));
        }
        DrawReagentName(Painter);
    }
    else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
    {
        if(true == m_ReagentExpiredFlag && m_StationSelected && (m_BlinkingCounter % 2))
        {
            Painter.drawPixmap((m_ParaffinbathBoundingRectWidth - 90), 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Expiry/Expiry_Expired_Small.png"));
        }
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
            Painter.drawText(QRect(-90, -3, 75, 60 ), Qt::TextWordWrap | Qt::AlignVCenter | Qt::AlignLeft, p_Reagent->GetReagentName());
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
       Painter.drawText(QRectF(32,5,67,15), Qt::AlignCenter, m_StationItemLabel);
   } else if( STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup) {
       // Load the Cover Image , because it will not be visible after filling the Color.
       Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Cover.png"));
       Painter.drawText(QRectF(13,7,67,15), Qt::AlignCenter, m_StationItemLabel);
   } else {
       if(m_StationSelected) {
           Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Cover.png"));
       } else {
           Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Cover_Grayed.png"));
       }

       Painter.drawText(QRectF(10,1,33,16), Qt::AlignCenter, m_StationItemLabel);
   }
   update();
}

void CDashboardStationItem::FillReagentColor(QPainter & Painter)
{
    int fillBottleWidth;
    int fillBottleHeight;
    int fillParaffinbathWidth;
    int fillParafinbathHeight;

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

        QString ReagentColorValue = p_ReagentGroup->GetGroupColor();
        ReagentColorValue.prepend("#");

        if(m_StationSelected) {
            Painter.setRenderHint(QPainter::Antialiasing);
            Painter.setPen(QColor(ReagentColorValue));
            Painter.setBrush(QColor(ReagentColorValue));
        } else {
            Painter.setRenderHint(QPainter::Antialiasing);
            Painter.setPen(QColor(Qt::gray));
            Painter.setBrush(QColor(Qt::gray));
        }

        Painter.rotate(270.0);

        if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
        {
            // Since the Painter is rotated, Width and Height Axis Changes.
            fillBottleWidth = (m_BottleBoundingRectHeight - 25);  // Manual Pixel Calculation
            fillBottleHeight = (m_BottleBoundingRectWidth - 31);  // Manual Pixel Calculation
            Painter.drawRoundedRect(QRect(-95, 2, fillBottleWidth, fillBottleHeight), 8, 8);
        }
        else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
        {
            fillParaffinbathWidth = (m_ParaffinbathBoundingRectHeight - 8);
            fillParafinbathHeight = (m_ParaffinbathBoundingRectWidth - 32);

            QPainterPath path;
            path.setFillRule( Qt::WindingFill );
            path.addRoundedRect(QRect(-57, 3, fillParaffinbathWidth, fillParafinbathHeight), 8, 4);
            path.addRect(QRect((-57 + fillParaffinbathWidth - 8), 3, 8, 8));// Top left corner not rounded
            path.addRect(QRect((-57 + fillParaffinbathWidth - 8), (3 + fillParafinbathHeight - 8), 8, 8));// Top right corner not rounded
            Painter.drawPath(path.simplified());  // Only the Bottome Left and Bottom Right Corner

        }

        Painter.rotate(-270.0);
    }
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

} // end namespace Core
