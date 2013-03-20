/****************************************************************************/
/*! \file ReagentModel.cpp
 *
 *  \brief ReagentModel Implementation.
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
#include "Core/Include/ReagentStatusModel.h"
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
CReagentStatusModel::CReagentStatusModel(QObject *p_Parent) : QAbstractTableModel(p_Parent)
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
void CReagentStatusModel::SetRequiredContainers(DataManager::CDataReagentList *p_ReagentList,
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
void CReagentStatusModel::UpdateReagentList()
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
                if (mp_ReagentList) {
                    DataManager::CReagent *p_Reagent = NULL;
                    p_Reagent = const_cast<DataManager::CReagent*>(mp_ReagentList->GetReagent(p_Station->GetDashboardReagentID()));
                    if (p_Reagent) {
                        ReagentName =  p_Reagent->GetReagentName();
                        if(p_Reagent->GetVisibleState()== true){
                            m_VisibleReagentIds << p_Reagent->GetReagentName();
                        }
                    }
                }
                m_ReagentNames << ReagentName;
                // store the station id for the reagent name
                m_StationIdentifiers[p_Station->GetDashboardStationName()] = p_Station->GetDashboardStationID();
                m_StationNames << p_Station->GetDashboardStationName();
                m_Identifiers[p_Station->GetDashboardStationName()] = p_Station->GetDashboardReagentID();
            }
        }
    }
//    if (mp_ReagentList) {
//        for(qint32 i = 0; i < mp_ReagentList->GetNumberOfReagents(); i++) {
//            DataManager::CReagent *p_Reagent = NULL;
//            p_Reagent = const_cast<DataManager::CReagent*>(mp_ReagentList->GetReagent(i));
//            if (p_Reagent) {
//                if (mp_StationList) {
//                    for (qint32 StationCounter = 0; StationCounter < mp_StationList->GetNumberOfDashboardStations(); StationCounter++) {
//                        DataManager::CDashboardStation* p_Station = const_cast<DataManager::CDashboardStation*>(mp_StationList->GetDashboardStation(StationCounter));
//                        if (p_Station) {
//                            if (p_Station->GetDashboardReagentID() == p_Reagent->GetReagentID()) {
//                                // store the station id for the reagent name
//                                m_StationIdentifiers[p_Station->GetDashboardStationName()] = p_Station->GetDashboardStationID();
//                                m_ReagentNames << p_Reagent->GetReagentName();
//                                m_StationNames << p_Station->GetDashboardStationName();
//                                m_Identifiers[p_Station->GetDashboardStationName()] = p_Reagent->GetReagentID();
//                                if(p_Reagent->GetVisibleState()== true){
//                                    m_VisibleReagentIds << p_Reagent->GetReagentName();
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
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
void CReagentStatusModel::SetVisibleRowCount(int RowCount)
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
int CReagentStatusModel::rowCount(const QModelIndex &) const
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
int CReagentStatusModel::columnCount(const QModelIndex &) const
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
QVariant CReagentStatusModel::data(const QModelIndex &Index, int Role) const
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
        QDate t_Date;
        int Days_Overdue =t_Date.currentDate().dayOfYear()-p_Station->GetDashboardReagentExcahngeDate().dayOfYear();
        int Expired = p_Reagent->GetMaxCassettes()-p_Station->GetDashboardReagentActualCassettes();

        if (Role == (int)Qt::DisplayRole) {
            switch (Index.column()) {
            case 0:
                return p_Station->GetDashboardStationName();
            case 1:
                if (p_Reagent) {
                    return p_Reagent->GetReagentName();
                }
                else {
                    return QString("");
                }
            case 2:
                if (p_Reagent) {
                    switch (m_RMSOptions) {
                        default:
                            return QString("");
                        case Global::RMS_CASSETTES:
                            return p_Station->GetDashboardReagentActualCassettes();
                        case Global::RMS_CYCLES:
                            return p_Station->GetDashboardReagentActualCycles();
                        case Global::RMS_DAYS:
                           QDate Tem_QDate = p_Station->GetDashboardReagentExcahngeDate();
                               switch(m_DateFormat){
                               default:
                                   return QString("");
                               case Global::DATE_INTERNATIONAL:
                                   return Tem_QDate.addDays(p_Reagent->GetMaxDays()).toString("dd.MM.yyyy");
                               case Global::DATE_ISO:
                                   return Tem_QDate.addDays(p_Reagent->GetMaxDays()).toString("yyyy-MM-dd");
                               case Global::DATE_US:
                                   return Tem_QDate.addDays(p_Reagent->GetMaxDays()).toString("MM/dd/yyyy");
                             }
                     }
                }
                else {
                    return 0;
                }
            case 3:
                switch (m_RMSOptions) {
                    default:
                        return QString("");
                    case Global::RMS_CASSETTES:
                    if(0>Expired)
                         return p_Station->GetDashboardReagentActualCassettes()-p_Reagent->GetMaxCassettes();
                    else
                      return 0;
                    case Global::RMS_CYCLES:
                    if(0>Expired)
                        return p_Station->GetDashboardReagentActualCycles()-p_Reagent->GetMaxCycles();
                    else
                        return 0;
                    case Global::RMS_DAYS:
                    if(0>Expired)
                        return Days_Overdue;
                    else
                        return 0;
                }                
            case 4:
                switch(m_DateFormat){
                default:
                    return QString("");
                case Global::DATE_INTERNATIONAL:
                    return p_Station->GetDashboardReagentExcahngeDate().toString("dd.MM.yyyy");
                case Global::DATE_ISO:
                    return p_Station->GetDashboardReagentExcahngeDate().toString("yyyy-MM-dd");
                case Global::DATE_US:
                    return p_Station->GetDashboardReagentExcahngeDate().toString("MM/dd/yyyy");
               }
            case 5:
                return p_Station->GetDashboardReagentStatus();
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
QVariant CReagentStatusModel::headerData(int Section, Qt::Orientation Orientation, int Role) const
{
    if (Role == (int)Qt::SizeHintRole && Orientation == Qt::Horizontal) {
        QSize size(100, 64);
        return size;
    }

    if (Role == (int)Qt::DisplayRole && Orientation == Qt::Horizontal) {
        switch (Section) {
        case 0:
            return tr("Station");
        case 1:
            return tr("Reagent");
        case 2:
            switch (m_RMSOptions) {
                default:
                    return QString("");
                case Global::RMS_CASSETTES:
                    return tr("Cassettes\nsince\nchange");
                case Global::RMS_CYCLES:
                    return tr("Cycles\nsince\nchange");
                case Global::RMS_DAYS:
                    return tr("Expiry\nDate");
            }
        case 3:
            switch (m_RMSOptions) {
                default:
                    return QString("");
                case Global::RMS_CASSETTES:
                    return tr("Cassettes\noverdue");
                case Global::RMS_CYCLES:
                    return tr("Cycles\noverdue");
                case Global::RMS_DAYS:
                    return tr("Days\noverdue");
            }
        case 4:            
            return tr("Exchange\nDate");
        case 5:
            return tr("Status");
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
QString CReagentStatusModel::GetReagentID(const QString ReagentName)
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
QString CReagentStatusModel::GetReagentLongName(int Row)
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
bool CReagentStatusModel::ContainsReagent(QString ReagentID)
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
void CReagentStatusModel::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
        m_UserSettings = *p_UserSettings;
}

/****************************************************************************/
QVariant CReagentStatusModel::dataStatus(const QModelIndex &Index, int Role) const
{
    DataManager::CReagent *p_Reagent = NULL;
    if (mp_ReagentList == NULL) {
        return QVariant();
    }

    if (Index.row() < m_ReagentNames.count() && (p_Reagent = const_cast<DataManager::CReagent*>(mp_ReagentList->GetReagent(m_Identifiers[m_ReagentNames[Index.row()]])))){
        if (Role == (int)Qt::DisplayRole) {
            switch (Index.column()) {
//            case 0:
//                return p_Reagent->GetReagentStation();
            case 1:
                return p_Reagent->GetReagentName();
//            case 2:
//                return p_Reagent->GetCassetteUntilChange();
//            case 3:
//                return p_Reagent->GetcassetteOverdue();
//            case 4:
//                return p_Reagent->GetExpirayDate();
//            case 5:
//                return p_Reagent->GetStatus();
            }
        }

        if (Role == (int)Qt::UserRole) {
            return p_Reagent->GetReagentName();
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
 *  \brief Returns item model flags of a cell
 *
 *  \iparam Index = Index of a table cell
 *
 *  \return Flags of the cell
 */
/****************************************************************************/
void CReagentStatusModel::ResetAndUpdateModel()
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
Qt::ItemFlags CReagentStatusModel::flags(const QModelIndex &Index) const
{
    if (mp_ReagentList) {
        if (Index.row() >=  mp_ReagentList->GetNumberOfReagents()) {
            return Qt::NoItemFlags;
        }
    }
    return QAbstractItemModel::flags(Index);
}
} // end namespace Core
