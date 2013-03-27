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
    m_Animate(Animation),
    mp_DashboardStation(p_DashboardStation),
    m_RetortBoundingRectWidth(125),
    m_RetortBoundingRectHeight(78),
    m_ParaffinbathBoundingRectWidth(122),
    m_ParaffinbathBoundingRectHeight(60),
    m_BottleBoundingRectWidth(79),
    m_BottleBoundingRectHeight(97),
    m_ReagentBlinking(true),
    m_ReagentTimeStarted(false),
    m_ReagentExpiredFlag(false),
    m_Enabled(true),
    m_Pressed(false),
    m_Selected(false)

{
    setFlag(QGraphicsItem::ItemIsSelectable);

    setCacheMode(QGraphicsItem::ItemCoordinateCache);

    if(m_DashboardStationGroup == STATIONS_GROUP_RETORT) {
        //m_Image = QPixmap(125, 78);
        m_Image = QPixmap(m_RetortBoundingRectWidth, m_RetortBoundingRectHeight);
    } else if(m_DashboardStationGroup == STATIONS_GROUP_PARAFFINBATH) {
        //m_Image = QPixmap(122, 60);
        m_Image = QPixmap(m_ParaffinbathBoundingRectWidth, m_ParaffinbathBoundingRectHeight);
    } else {
        //m_Image = QPixmap(79, 97);
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
        //return QRectF(0, 0, 125, 78);
        return QRectF(0, 0, m_RetortBoundingRectWidth, m_RetortBoundingRectHeight);
    } else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup) {
        //return QRectF(0, 0, 122, 60);
        return QRectF(0, 0, m_ParaffinbathBoundingRectWidth, m_ParaffinbathBoundingRectHeight);
    } else {
        //return QRectF(0, 0, 79, 97);
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
    QFont TextFont;
    Painter.setRenderHint(QPainter::Antialiasing);
    Painter.setPen(Qt::black);
    TextFont.setPointSize(10);
    TextFont.setBold(true);
    Painter.setFont(TextFont);

    if(STATIONS_GROUP_RETORT == m_DashboardStationGroup) {
        Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Retort/Retort_Enabled.png"));
        Painter.drawText(QRectF(32,5,67,15), Qt::AlignCenter, m_StationItemLabel);
    } else if( STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup){
        Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Background.png"));
        Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Cover.png"));
        Painter.drawText(QRectF(13,7,67,15), Qt::AlignCenter, m_StationItemLabel);
    } else {
        Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Background.png"));
        Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Cover.png"));
        Painter.drawText(QRectF(10,1,33,16), Qt::AlignCenter, m_StationItemLabel);
    }
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
    QPainter Painter(&m_Image);

    Painter.eraseRect(boundingRect());
    LoadStationImages(Painter);
    FillReagentColor(Painter);

    if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
    {
        Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Cover.png"));
        Painter.drawPixmap((m_BottleBoundingRectWidth - 72) , (m_BottleBoundingRectHeight - 75), QPixmap(":/HimalayaImages/Icons/Dashboard/Expiry/Expiry_Expired_Large.png"));
        DrawReagentName(Painter);
    } else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
    {

        Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Cover.png"));
        Painter.drawPixmap((m_ParaffinbathBoundingRectWidth - 90), 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Expiry/Expiry_Expired_Small.png"));
        QFont TextFont;
        Painter.setRenderHint(QPainter::Antialiasing);
        Painter.setPen(Qt::black);
        TextFont.setPointSize(10);
        TextFont.setBold(true);
        Painter.setFont(TextFont);
        Painter.drawText(QRectF(13,7,67,15), Qt::AlignCenter, m_StationItemLabel);

    }
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
    TextFont.setBold(true);
    Painter.setFont(TextFont);

    if (mp_DashboardStation)
    {
        qDebug() << "************" <<mp_DashboardStation->GetDashboardReagentStatus();

        //if(mp_DashboardStation->GetDashboardReagentStatus() > 100) {
            m_ReagentExpiredFlag = true;
        //}
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
        QString ReagentGroupId = p_Reagent->GetGroupID();
        DataManager::CReagentGroup const *p_ReagentGroup = mp_DataConnector->ReagentGroupList->GetReagentGroup(ReagentGroupId);

        QString ReagentStatus = mp_DashboardStation->GetDashboardReagentStatus();
        qDebug() << "Reagent Status" << ReagentStatus;
        QString ReagentColorValue = p_ReagentGroup->GetGroupColor();
        ReagentColorValue.prepend("#");

        Painter.setRenderHint(QPainter::Antialiasing);
        Painter.setPen(QColor(ReagentColorValue));
        Painter.setBrush(QColor(ReagentColorValue));

        Painter.rotate(270.0);

        if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
        {

            //Painter.drawRoundedRect(QRect(-95, 2, 72, 48), 8, 8);
            // Since the Painter is rotated, Width and Height Axis Changes.
            fillBottleWidth = (m_BottleBoundingRectHeight - 25);  // Manual Pixel Calculation
            fillBottleHeight = (m_BottleBoundingRectWidth - 31);  // Manual Pixel Calculation
            Painter.drawRoundedRect(QRect(-95, 2, fillBottleWidth, fillBottleHeight), 8, 8);
        }
        else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
        {
            // Painter.drawRoundedRect(QRect(-57, 3, 52, 90), 8, 4);
            /**
             *QPainterPath path;
             *path.setFillRule( Qt.WindingFill );
             *path.addRoundedRect( QRect(50,50, 200, 100), 20, 20 );
             *path.addRect( QRect( 200, 50, 50, 50 ) ); // Top right corner not rounded
             *path.addRect( QRect( 50, 100, 50, 50 ) ); // Bottom left corner not rounded
             *painter.drawPath( path.simplified() ); // Only Top left & bottom right corner
            **/

            //            QPainterPath path;
            //            path.setFillRule( Qt::WindingFill );
            //            path.addRoundedRect(QRect(-57, 3, 52, 90), 8, 8);
            //            path.addRect(QRect((-57 + 52 - 8), 3, 8, 8));// Top left corner not rounded
            //            path.addRect(QRect((-57 + 52 - 8), (3 + 90 - 8), 8, 8));// Top right corner not rounded
            //            Painter.drawPath(path.simplified());  // Only the Bottome Left and Bottom Right Corner

            fillParaffinbathWidth = (m_ParaffinbathBoundingRectHeight - 8);
            fillParafinbathHeight = (m_ParaffinbathBoundingRectWidth - 32);

            QPainterPath path;
            path.setFillRule( Qt::WindingFill );
            path.addRoundedRect(QRect(-57, 3, fillParaffinbathWidth, fillParafinbathHeight), 8, 4);
            path.addRect(QRect((-57 + fillParaffinbathWidth - 8), 3, 8, 8));// Top left corner not rounded
            path.addRect(QRect((-57 + fillParaffinbathWidth - 8), (3 + fillParafinbathHeight - 8), 8, 8));// Top right corner not rounded
            Painter.drawPath(path.simplified());  // Only the Bottome Left and Bottom Right Corner

            qDebug() << "Height ==>" << boundingRect().height();
            qDebug() << "Width ==>" << boundingRect().width();

        }


        Painter.rotate(-270.0);
    }
    update();
}

/****************************************************************************/
/*!
 *  \brief Blinking Effect for the Station
 */
/****************************************************************************/
void CDashboardStationItem::ShowBlinkingEffect()
{
    if(m_ReagentExpiredFlag ) {

        update();
    }
}

} // end namespace Core
