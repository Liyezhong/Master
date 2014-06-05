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
#include "HimalayaDataContainer/Helper/Include/Global.h"


namespace Core {


//!< Self explaining. Just a random value.
#define ANIMATION_COUNT_MAX 10000

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_DataConnector = Instance of Data Container
 *  \iparam p_Parent = Parent item
 *  \param StationGroup
 *  \param StationItemID
 *  \param StationLabel
 *  \param p_DashboardStation
 */
/****************************************************************************/
CDashboardStationItem::CDashboardStationItem(Core::CDataConnector *p_DataConnector,
                                             StationGroupType_t StationGroup,
                                             const QString& StationItemID,
                                             QString StationLabel,
                                             DataManager::CDashboardStation *p_DashboardStation,
                                             QGraphicsItem *p_Parent) : QGraphicsItem(p_Parent),
    mp_DataConnector(p_DataConnector),
    m_CurRMSMode(Global::RMS_UNDEFINED),
    mp_DashboardStation(p_DashboardStation),
    m_StationItemID(StationItemID),
    m_DashboardStationGroup(StationGroup),
    m_StationItemLabel(StationLabel),
    m_RetortBoundingRectWidth(177),
    m_RetortBoundingRectHeight(92),
    m_RetortCoverHeight(25),
    m_ParaffinbathBoundingRectWidth(120),
    m_ParaffinbathBoundingRectHeight(65),
    m_ParaffinbathCoverHeight(11),
    m_BottleBoundingRectWidth(61),
    m_BottleBoundingRectHeight(139),
    m_BottleCoverHeight(38),
    m_CurrentBoundingRectReagentHeight(0),
    m_ReagentExpiredFlag(false),
    m_StationSelected(false),
    m_ContainerStatusType(DataManager::CONTAINER_STATUS_FULL),
    m_ExpiredColorRed(true),
    m_EnableBlink(true),
    m_IsRetortContaminated(false),
    m_RetortLocked(false)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    setCacheMode(QGraphicsItem::ItemCoordinateCache);

    if(m_DashboardStationGroup == STATIONS_GROUP_RETORT) {
        m_Image = QPixmap(m_RetortBoundingRectWidth, m_RetortBoundingRectHeight);
    } else if(m_DashboardStationGroup == STATIONS_GROUP_PARAFFINBATH) {
        m_Image = QPixmap(m_ParaffinbathBoundingRectWidth, m_ParaffinbathBoundingRectHeight);
    } else {
        m_Image = QPixmap(m_BottleBoundingRectWidth, m_BottleBoundingRectHeight);

        //For Cleaning Reagent, generate the raw image used for BDiagpattern
        QPixmap pix(200, 200);
        pix.fill(Qt::transparent);
        QPainter painter(&pix);
        painter.setBrush(Qt::white);
        painter.setPen(Qt::white);
        for (int i=0; i<200; i+=8)
        {
            painter.drawRect(i,0,4,200);
        }
        QMatrix matrix;
        (void)matrix.rotate(45.0);
        QPixmap img = pix.transformed(matrix);
        img = img.copy(200/2, 200/2, 200/2, 200/2);
        m_RawImage4Cleaning = img;
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
        //Loading the Static Images and Drawingthe Static Label
        QPainter Painter(&m_Image);
        Painter.fillRect(this->boundingRect(), QColor(226, 227, 228));
        LoadStationImages(Painter);
        return;
    }

    CONNECTSIGNALSLOT(mp_DataConnector, ReagentsUpdated(), this, UpdateDashboardStationItemReagentWhenReagentUpdated());
    CONNECTSIGNALSLOT(mp_DataConnector, UserSettingsUpdated(), this, UpdateUserSettings());
    CONNECTSIGNALSLOT(mp_DataConnector, ReagentGroupUpdated(), this, UpdateDashboardStationItemWhenReagentGroupUpdated());


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
 *  \iparam p_DashboardStation = Station data object
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
 *  \brief Load the Initial Images of Stations
 *
 */
/****************************************************************************/

void CDashboardStationItem::LoadStationImages(QPainter& Painter)
{
    if(STATIONS_GROUP_RETORT == m_DashboardStationGroup) {
		if (false == m_RetortLocked)
		{
                Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Retort/Retort_Unlocked.png"));
		}
		else
		{
                Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Retort/Retort_Locked01.png"));
                Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Retort/Retort_Locked02.png"));
		}
    } else if( STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup) {
        if (m_StationSelected) {
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Background.png"));
            //Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Cover.png"));
        } else {
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Background.png"));
            //Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Paraffinbath/Paraffinbath_Cover_Grayed.png"));
        }
    } else {
        if(m_StationSelected) {
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Background.png"));
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Handle.png"));
        } else {
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Background.png"));
            Painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Handle.png"));
        }

    }
    DrawStationItemLabel(Painter);
}


/****************************************************************************/
/*!
 *  \brief Update Dashboard Station Items Reagent Properties when Reagents are updated
 *
 */
/****************************************************************************/

void CDashboardStationItem::UpdateDashboardStationItemReagentWhenReagentUpdated()
{
    if (!mp_DashboardStation)
        return;
    QString ReagentID = mp_DashboardStation->GetDashboardReagentID();
    DataManager::CReagent *p_Reagent = const_cast<DataManager::CReagent*>(mp_DataConnector->ReagentList->GetReagent(ReagentID));

    if (p_Reagent)
    {
        bool flag = false ;
        DataManager::ReagentStatusType_t reagentStatus = mp_DashboardStation->GetReagentStatus(*p_Reagent, m_CurRMSMode);

        if ( reagentStatus == DataManager::REAGENT_STATUS_EXPIRED )
            flag = true;
        else
            flag = false;

        if ( flag != m_ReagentExpiredFlag )
        {
            m_ReagentExpiredFlag = flag;
            DrawStationItemImage();
        }
    }

}

/****************************************************************************/
/*!
 *  \brief Update Dashboard Station Items Reagent Properties when Reagents Group Color are updated
 *
 */
/****************************************************************************/

void CDashboardStationItem::UpdateDashboardStationItemWhenReagentGroupUpdated()
{
    if (!mp_DashboardStation)
        return;
    QString ReagentID = mp_DashboardStation->GetDashboardReagentID();
    DataManager::CReagent *p_Reagent = const_cast<DataManager::CReagent*>(mp_DataConnector->ReagentList->GetReagent(ReagentID));

    if (p_Reagent)
    {
        QString ReagentGroupId = p_Reagent->GetGroupID();
        DataManager::CReagentGroup const *p_ReagentGroup = mp_DataConnector->ReagentGroupList->GetReagentGroup(ReagentGroupId);

        QString ColorValue = p_ReagentGroup->GetGroupColor();
        (void)ColorValue.prepend("#");

        if ( ColorValue != m_ReagentDisplayColorValue )
        {
            DrawStationItemImage();
        }
    }
}


/****************************************************************************/
/*!
 *  \brief Update Dashboard Station Items Reagent Properties when Reagents are updated
 *
 *  \param RefreshFlag = bool type parameter
 *
 */
/****************************************************************************/

void CDashboardStationItem::UpdateDashboardStationItemReagent(bool RefreshFlag)
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

            DataManager::ReagentStatusType_t reagentStatus = mp_DashboardStation->GetReagentStatus(*p_Reagent, m_CurRMSMode);

            if ( reagentStatus == DataManager::REAGENT_STATUS_EXPIRED )
                m_ReagentExpiredFlag = true;
            else
                m_ReagentExpiredFlag = false;
        }
    }

    if ( RefreshFlag )
        DrawStationItemImage();
}

void CDashboardStationItem::DrawStationItemImage()
{
    QPainter Painter(&m_Image);
    Painter.fillRect(this->boundingRect(), QColor(226, 227, 228));
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
    TextFont.setPointSize(8);

    if(m_StationSelected) {
        TextFont.setBold(true);
    }

    if (mp_DashboardStation)
    {
        //Now Paint Reagent Names
        QString ReagentID = mp_DashboardStation->GetDashboardReagentID();
        DataManager::CReagent const *p_Reagent = mp_DataConnector->ReagentList->GetReagent(ReagentID);
        if (p_Reagent)
        {
            Painter.rotate(270.0);
            QString reagentName = p_Reagent->GetReagentName();
            QRect rect(-130, 23, 75, 60 );
            DrawGlowBoundaryText(TextFont, reagentName, rect, Painter);
            Painter.rotate(-270.0);
        }
    }
}

void CDashboardStationItem::DrawGlowBoundaryText(QFont& textFont, QString& text, QRect& rect, QPainter& painter)
{
   QPainterPath textPath;
   QFontMetrics fm(textFont);
   textPath.addText(rect.x(), rect.y()+ fm.height() -1 - fm.descent(), textFont, text);
   QPainterPathStroker pps;
   pps.setWidth(3);
   pps.setDashPattern(Qt::NoPen);
   pps.setCurveThreshold(0.001);
   pps.setCapStyle(Qt::RoundCap);
   pps.setJoinStyle(Qt::RoundJoin);
   QPainterPath strokePath = pps.createStroke(textPath);
   painter.fillPath(strokePath, QBrush(QColor(Qt::white)));
   painter.setPen(Qt::black);
   painter.drawText(rect.x(), rect.y()+ fm.height() -1 - fm.descent(), text);
}

void CDashboardStationItem::DrawStationItemLabel(QPainter &painter)
{
    QFont textFont;
    textFont.setPointSize(8);
    painter.setRenderHint(QPainter::Antialiasing);
    if(m_StationSelected) {
        textFont.setBold(true);
    }

    painter.setFont(textFont);
    QRect rect;
    if(STATIONS_GROUP_RETORT == m_DashboardStationGroup) {
       textFont.setBold(true);
       rect.setRect(65,45,67,15);
     } else if( STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup) {
       rect.setRect(33,22,67,15);
     } else {//draw text in station label
        if(m_StationSelected) {
           painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Handle.png"));
        } else {
            painter.drawPixmap(0, 0, QPixmap(":/HimalayaImages/Icons/Dashboard/Bottle/Bottle_Handle.png"));
        }
        rect.setRect(23,9,33,16);
     }

     DrawGlowBoundaryText(textFont, m_StationItemLabel, rect, painter);
}

void CDashboardStationItem::EnableBlink(bool bEnable)
{
    m_EnableBlink = bEnable;
    m_ExpiredColorRed = false;
}

void CDashboardStationItem::FillReagentColor(QPainter & Painter)
{
    int fillBottleWidth;
    int fillBottleHeight;
    int fillParaffinbathWidth;
    int fillParaffinbathHeight;
    int fillRetortWidth;
    int fillRetortHeight;
    QString reagentColorValue;
    bool isCleaningReagent = false;

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

        reagentColorValue = p_ReagentGroup->GetGroupColor();
        (void)reagentColorValue.prepend("#");

        m_ReagentDisplayColorValue = reagentColorValue;

		//Check whether the bottle contains cleaning reagnet for later use 
		if (p_ReagentGroup->IsCleaningReagentGroup())
		{
			isCleaningReagent = true;
		}
    }
    else
    {
        reagentColorValue = m_CurrentReagentColorValue;
    }
    if(m_StationSelected) {
        QColor color(reagentColorValue);
        if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup || STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
        {
            if(m_EnableBlink && m_ReagentExpiredFlag && m_StationSelected)
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
        Painter.setPen(color);
        Painter.setBrush(color);
    } else {
        Painter.setPen(QColor(Qt::gray));
        Painter.setBrush(QColor(Qt::gray));
    }
    if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
    {
        // Since the Painter is rotated, Width and Height Axis Changes.
        fillBottleWidth = m_BottleBoundingRectWidth - 10;  // Manual Pixel Calculation
        fillBottleHeight = m_BottleBoundingRectHeight - m_BottleCoverHeight;  // Manual Pixel Calculation
        if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_SCUKING
        || m_ContainerStatusType == DataManager::CONTAINER_STATUS_DRAINING)
        {
            fillBottleHeight = m_CurrentBoundingRectReagentHeight;
        }
        else if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_FULL)
        {
            m_CurrentBoundingRectReagentHeight = fillBottleHeight;
        }

        QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        path.addRoundedRect(QRect(4, m_BottleBoundingRectHeight - fillBottleHeight, fillBottleWidth,
                                  fillBottleHeight -1), 8, 4);

        int cornerHeight = 8;
        if (fillBottleHeight > cornerHeight && fillBottleHeight < m_BottleBoundingRectHeight - 25 - cornerHeight + 4)
        {
            path.addRect(QRect(4, m_BottleBoundingRectHeight - fillBottleHeight, 8, cornerHeight));// Top left corner not rounded
            path.addRect(QRect(fillBottleWidth - 4, m_BottleBoundingRectHeight - fillBottleHeight, 8, cornerHeight));// Top right corner not rounded

        }
        Painter.drawPath(path);  // Only the Bottome Left and Bottom Right Corner
    }
    else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
    {
        fillParaffinbathWidth = m_ParaffinbathBoundingRectWidth - 9;
        fillParaffinbathHeight = m_ParaffinbathBoundingRectHeight - m_ParaffinbathCoverHeight;
        if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_SCUKING
        || m_ContainerStatusType == DataManager::CONTAINER_STATUS_DRAINING)
        {
            fillParaffinbathHeight = m_CurrentBoundingRectReagentHeight;
        }
        else if (m_ContainerStatusType == DataManager::CONTAINER_STATUS_FULL)
        {
            m_CurrentBoundingRectReagentHeight = fillParaffinbathHeight;
        }

        QPainterPath path;
        path.setFillRule( Qt::WindingFill);
        path.addRoundedRect(QRect(4, m_ParaffinbathBoundingRectHeight - fillParaffinbathHeight, fillParaffinbathWidth,
                                  fillParaffinbathHeight -1), 8, 4);
        int cornerHeight = 8;
        if (fillParaffinbathHeight > cornerHeight)
        {
            path.addRect(QRect(4, m_ParaffinbathBoundingRectHeight - fillParaffinbathHeight, 8, cornerHeight));// Top left corner not rounded
            path.addRect(QRect(fillParaffinbathWidth - 4, m_ParaffinbathBoundingRectHeight - fillParaffinbathHeight, 8, cornerHeight));// Top right corner not rounded
        }
        Painter.drawPath(path);  // Only the Bottome Left and Bottom Right Corner

    }
    else if(STATIONS_GROUP_RETORT == m_DashboardStationGroup)
    {
        fillRetortHeight = m_CurrentBoundingRectReagentHeight;
        fillRetortWidth =  m_RetortBoundingRectWidth;

        QPainterPath path;
        path.setFillRule( Qt::WindingFill);
        if (m_IsRetortContaminated)//Retort CONTAMINATED
        {
            //draw two virtical line align to the wall of container
            path.addRect(4, m_RetortCoverHeight, 3, m_RetortBoundingRectHeight - m_RetortCoverHeight - 3);
            path.addRect(fillRetortWidth - 5, m_RetortCoverHeight, 3, m_RetortBoundingRectHeight - m_RetortCoverHeight - 3);
        }

        if (fillRetortHeight > 0)
            path.addRoundedRect(QRect(4, m_RetortBoundingRectHeight - fillRetortHeight, fillRetortWidth - 6, fillRetortHeight - 3), 8, 8);

        int cornerHeight = 4;
        if (fillRetortHeight > cornerHeight)
        {
            path.addRect(QRect(4, m_RetortBoundingRectHeight - fillRetortHeight, 8, cornerHeight));// Top left corner not rounded
            path.addRect(QRect(fillRetortWidth - 10, m_RetortBoundingRectHeight - fillRetortHeight, 8, cornerHeight));// Top right corner not rounded
        }
        Painter.drawPath(path);  // Only the Bottome Left and Bottom Right Corner
    }

	//For Cleaning Reagent, we add the BDiagPattern
    /*if (isCleaningReagent == true)
	{
		qreal startx = 4.0;
        qreal starty = 38.0;
        qreal bottleWidth = fillBottleWidth-47;
        qreal boottleHeight = fillBottleHeight+46;
        QPainterPath clipPath;
        clipPath.setFillRule(Qt::WindingFill);
        clipPath.addRect(startx,starty, bottleWidth, boottleHeight-5);
        clipPath.addRoundedRect(startx, starty+boottleHeight-8, bottleWidth, 10, 8, 8);
        Painter.setClipPath(clipPath);

		//draw the final result
        Painter.setBrush(Qt::white);
        Painter.drawPixmap(startx, starty, m_RawImage4Cleaning);
    }*/
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

    }
}

void CDashboardStationItem::UpdateUserSettings()
{
    m_UserSettings = *(mp_DataConnector->SettingsInterface->GetUserSettings());
    UpdateDashboardStationItemReagent(false);
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
            m_CurrentBoundingRectReagentHeight = m_RetortBoundingRectHeight - m_RetortCoverHeight;
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
            m_CurrentBoundingRectReagentHeight = m_RetortBoundingRectHeight - m_RetortCoverHeight;
        else
            m_CurrentBoundingRectReagentHeight = 0;
    }
    else
    {

        if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
        {
            m_CurrentBoundingRectReagentHeight = m_BottleBoundingRectHeight - m_BottleCoverHeight;
        }
        else if (STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
        {
            m_CurrentBoundingRectReagentHeight = m_ParaffinbathBoundingRectHeight - m_ParaffinbathCoverHeight;
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
            if (m_CurrentBoundingRectReagentHeight < m_RetortBoundingRectHeight - m_RetortCoverHeight)
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
                    m_CurrentBoundingRectReagentHeight = m_BottleBoundingRectHeight - m_BottleCoverHeight;
                }
                else if (STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
                {
                    m_CurrentBoundingRectReagentHeight = m_ParaffinbathBoundingRectHeight - m_ParaffinbathCoverHeight;
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
                m_CurrentBoundingRectReagentHeight = m_RetortBoundingRectHeight - m_RetortCoverHeight;
            }
        }
        else
        {
            int currentLiquidLevel(0);
            if(STATIONS_GROUP_BOTTLE == m_DashboardStationGroup)
            {
                 currentLiquidLevel = m_BottleBoundingRectHeight - m_BottleCoverHeight;
            }
            else if(STATIONS_GROUP_PARAFFINBATH == m_DashboardStationGroup)
            {
                 currentLiquidLevel = m_ParaffinbathBoundingRectHeight - m_ParaffinbathCoverHeight;// stop animation when touch to current LiquidLevel
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

void CDashboardStationItem::SetContainerRetortLockedStatus(bool locked)
{
	m_RetortLocked = locked;
    DrawStationItemImage();

}
} // end namespace Core
