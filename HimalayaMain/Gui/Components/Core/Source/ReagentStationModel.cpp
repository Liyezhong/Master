/****************************************************************************/
/*! \file ReagentStationModel.cpp
 *
 *  \brief ReagentStationModel Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-02-11 , 2013-02-23
 *   $Author:  $ Swati Tiwari
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

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "Core/Include/ReagentStationModel.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/MainWindow.h"
#include "Programs/Include/ProgramWidget.h"
#include "Application/Include/LeicaStyle.h"

#include <QPixmap>
#include <QDebug>

namespace Core {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CReagentStationModel::CReagentStationModel(QObject *p_Parent) : QAbstractTableModel(p_Parent)
{
    mp_ReagentList = NULL;
    mp_Parent = NULL;
    mp_StationList = NULL;
    m_FilterLeicaReagent = false;
    m_Columns = 0;
    m_VisibleRowCount = 7;
    m_RMSOptions = Global::RMS_CASSETTES;
    m_DateFormat = Global::DATE_INTERNATIONAL;
}

/****************************************************************************/
/*!
 *  \brief Initializes the reagent data
 *
 *  \iparam p_ReagentList = Reagent data
 *  \iparam Columns = Table columns
 *  \iparam BLCheck = True for displaying reagents in bathlayout else False.
 */
 /****************************************************************************/
void CReagentStationModel::SetRequiredContainers(DataManager::CDataReagentList *p_ReagentList,
                                                DataManager::CDataReagentGroupList *p_ReagentGroupList,
                                                DataManager::CDashboardDataStationList *p_StationList, qint32 Columns)
{
    mp_ReagentList = p_ReagentList;
    mp_ReagentGroupList = p_ReagentGroupList;
    mp_StationList = p_StationList;
    m_Columns = Columns;
    UpdateReagentList();
}


/****************************************************************************/
/*!
 *  \brief Updates the reagent data
 *
 *  This slot needs to be called whenever the content of the reagent list is
 *  changed.
 */
/****************************************************************************/
void CReagentStationModel::UpdateReagentList()
{
    beginResetModel();
    m_Identifiers.clear();
    m_ReagentNames.clear();
    m_ReagentNameMap.clear();
    m_StationNames.clear();
    m_StationNameMap.clear();

    if (mp_StationList) {
        for (qint32 StationCounter = 0; StationCounter < mp_StationList->GetNumberOfDashboardStations(); StationCounter++) {
            DataManager::CDashboardStation* p_Station = const_cast<DataManager::CDashboardStation*>(mp_StationList->GetDashboardStation(StationCounter));
            if (p_Station) {
                QString ReagentName;
                QString ReagentID;
                if (mp_ReagentList) {
                    DataManager::CReagent *p_Reagent = NULL;
                    p_Reagent = const_cast<DataManager::CReagent*>(mp_ReagentList->GetReagent(p_Station->GetDashboardReagentID()));
                    if (p_Reagent) {
                        ReagentName =  p_Reagent->GetReagentName();
                        ReagentID =  p_Reagent->GetReagentID();
                        if(p_Reagent->GetVisibleState()== true){
                            m_VisibleReagentIds << p_Reagent->GetReagentName();
                        }
                    }
                }
                m_ReagentNames << ReagentName;
                m_ReagentID<<ReagentID;
                // store the station id for the reagent name
                m_StationIdentifiers[p_Station->GetDashboardStationName()] = p_Station->GetDashboardStationID();
                m_StationNames << p_Station->GetDashboardStationName();
                m_Identifiers[p_Station->GetDashboardStationName()] = p_Station->GetDashboardReagentID();
            }
        }
    }

    foreach (const QString str, m_StationNames)
        (void)m_StationNameMap.insertMulti(str.toLower(), str);
    //m_StationNames = m_StationNameMap.values();

    endResetModel();
}



/****************************************************************************/
/*!
 *  \brief Sets the number of rows visible in the table
 *
 *  \iparam RowCount
 */
/****************************************************************************/
void CReagentStationModel::SetVisibleRowCount(int RowCount)
{
    beginResetModel();
    m_VisibleRowCount = RowCount;
    endResetModel();
}

/****************************************************************************/
/*!
 *  \brief Returns the number of rows in the table
 *
 *  \return Row count
 */
/****************************************************************************/
int CReagentStationModel::rowCount(const QModelIndex &) const
{
    return ((m_ReagentNames.count() < m_VisibleRowCount) ? m_VisibleRowCount : m_ReagentNames.count());
}

/****************************************************************************/
/*!
 *  \brief Returns the number of columns in the table
 *
 *  \return Columns count
 */
/****************************************************************************/
int CReagentStationModel::columnCount(const QModelIndex &) const
{
    return m_Columns;
}

/****************************************************************************/
/*!
 *  \brief Returns the data items displayed in the table
 *
 *  \iparam Index = Index of a table cell
 *  \iparam Role = Display role to be returned
 *
 *  \return Data item
 */
/****************************************************************************/
QVariant CReagentStationModel::data(const QModelIndex &Index, int Role) const
{
    DataManager::CReagent *p_Reagent = NULL;
    DataManager::CDashboardStation *p_Station = NULL;
    if (mp_ReagentList == NULL && mp_StationList == NULL) {
        return QVariant();
    }
    if (Index.row() < m_ReagentNames.count()) {
        p_Reagent = const_cast<DataManager::CReagent*>(mp_ReagentList->GetReagent(m_Identifiers[m_StationNames[Index.row()]]));
    }
    if (Index.row() < m_ReagentNames.count()
            && (p_Station = const_cast<DataManager::CDashboardStation*>(mp_StationList->GetDashboardStation(m_StationIdentifiers[m_StationNames[Index.row()]])))){

        if (Role == (int)Qt::DisplayRole) {
            switch (Index.column()) {
            case 0:
                return p_Station->GetDashboardStationName();
            case 1:
                if (p_Reagent) {
                    return p_Reagent->GetReagentName();
                }
                else {
                    return QString("None");
                }

            }
        }

        if (Role == (int)Qt::UserRole) {
            return p_Station->GetDashboardStationID();
        }
    }
    else if (Role == (int)Qt::BackgroundRole) {
        QPalette Palette;
        return QVariant(Palette.color(QPalette::Window));
    }
    return QVariant();
}

/****************************************************************************/
/*!
 *  \brief Returns the caption of the column headers
 *
 *  \iparam Section = Row or column number
 *  \iparam Orientation = Horizontal or vertical header
 *  \iparam Role = Display role to be returned
 *
 *  \return Header data
 */
/****************************************************************************/
QVariant CReagentStationModel::headerData(int Section, Qt::Orientation Orientation, int Role) const
{
    if (Role == (int)Qt::SizeHintRole && Orientation == Qt::Horizontal) {
        QSize size(100, 28);
        return size;
    }

    if (Role == (int)Qt::DisplayRole && Orientation == Qt::Horizontal) {
        switch (Section) {
        case 0:
            return tr("Station");
        case 1:
            return tr("Reagent");
        }
    }
    return QVariant();
}

/****************************************************************************/
/*!
 *  \brief This Function Gets Reagent ID of given Reagent Long name.
 *  \iparam ReagentLongName
 */
/****************************************************************************/
QString CReagentStationModel::GetReagentID(const QString ReagentName)
{
    return m_Identifiers.value(ReagentName);

}

/****************************************************************************/
  /*!
   * \brief Returns the Reagent long Name for the particular row.
   * \iparam Row
   * \return  m_ReagentNames = Reagent Long Name
   */
/****************************************************************************/
QString CReagentStationModel::GetReagentLongName(int Row)
{
    if (!m_ReagentNames.isEmpty()) {
        return m_ReagentNames[Row];
    }
    else {
        return "";
    }
}

/****************************************************************************/
/*!
  * \brief Returns true if reagent is present in the reagent list else false
  *  is returned.
  * \iparam ReagentID
  * \return  ReagentID = Reagent ID
  */
/****************************************************************************/
bool CReagentStationModel::ContainsReagent(QString ReagentID)
{
    if (m_VisibleReagentIds.isEmpty()) {
        return false;
    }
    if (m_VisibleReagentIds.contains(ReagentID)) {
        return true;
    }
    else {
        return false;
    }
}

/****************************************************************************/
/*!
 *  \brief Sets UserSettings for the model.
 *  \iparam p_UserSettings = UserSettings
 */
/****************************************************************************/
void CReagentStationModel::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
        m_UserSettings = *p_UserSettings;
}

/****************************************************************************/
/*!
 *  \brief Returns item model flags of a cell
 *
 *  \iparam Index = Index of a table cell
 *
 *  \return Flags of the cell
 */
/****************************************************************************/
void CReagentStationModel::ResetAndUpdateModel()
{
    beginResetModel();
    endResetModel();
}
/****************************************************************************/
/*!
 *  \brief Returns item model flags of a cell
 *
 *  \iparam Index = Index of a table cell
 *
 *  \return Flags of the cell
 */
/****************************************************************************/
Qt::ItemFlags CReagentStationModel::flags(const QModelIndex &Index) const
{
    if (mp_ReagentList) {
        if (Index.row() >=  mp_ReagentList->GetNumberOfReagents()) {
            return Qt::NoItemFlags;
        }
    }
    return QAbstractItemModel::flags(Index);
}
} // end namespace Core
