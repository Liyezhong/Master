/****************************************************************************/
/*! \file ReagentRMSModel.h
 *
 *  \brief Definition file for class CReagentRMSModel.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2011-08-23, 2013-01-17
 *   $Author:  $ M.Scherer, N.Kamath, Swati Tiwari
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

#ifndef CORE_REAGENTRMSMODEL_H
#define CORE_REAGENTRMSMODEL_H

#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "HimalayaDataContainer/Containers/ReagentGroups/Include/DataReagentGroupList.h"
#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "Global/Include/GlobalDefines.h"
#include <QAbstractTableModel>
#include <QMap>
#include <QStringList>
#include <QObject>
#include <QPixmap>
namespace Programs {
    class CModifyProgramStepDlg;
}
namespace Core {


/****************************************************************************/
/**
 * \brief This is the data model for the reagent tables
 */
/****************************************************************************/
class CReagentRMSModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CReagentRMSModel(QObject *p_Parent = NULL);
    void SetReagentList(DataManager::CDataReagentList *p_ReagentList, qint32 Columns);
    void SetReagentGroupList(DataManager::CDataReagentGroupList *p_ReagentGroupList);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &Index, int Role) const;  //data Reagent SubMenu
    QVariant dataStatus(const QModelIndex &Index, int Role) const; //data Reagent Staus SubMenu
    QVariant headerData(int Section, Qt::Orientation Orientation, int Role) const;
    Qt::ItemFlags flags(const QModelIndex &Index) const;
    QModelIndex CreateIndex(int Row, int Column);

    void ResetAndUpdateModel();
    /****************************************************************************/
    /**
     * \brief Returns the position of Reagent to be selected
     *
     * \iparam ReagentName
     * \return m_ReagentNames = Position of Reagent in the table
     */
    /****************************************************************************/
    int GetReagentPosition(QString ReagentName){
        return m_ReagentNames.indexOf(ReagentName, 0);
    }

    /****************************************************************************/
    /**
     * \brief Sets the currently selected reagent in the table
     *
     * \iparam ReagentName
     */
    /****************************************************************************/
    void SetCurrentReagent(QString ReagentName){
        m_CurrentReagentName = ReagentName;
    }

    /****************************************************************************/
    /**
     * \brief Sets the parent pointer
     *
     * \iparam p_Parent = Reference to parent widget
     */
    /****************************************************************************/
    void SetParentPtr(Programs::CModifyProgramStepDlg *p_Parent){ mp_Parent = p_Parent; }

    /****************************************************************************/
    /**
     * \brief Function to hide the special reagents in the table
     *
     * \iparam Hide = True hide special reagents else show.
     */
    /****************************************************************************/
    void HideSpecialReagents(bool Hide) { m_HideSpecialReagents = Hide; }

    /****************************************************************************/
    /**
     * \brief Function to hide the Leica reagents in the table.
     *        Note - Call this function before setting Reagent list.
     *
     * \iparam Filter = if true filter Leica reagents else show.
     */
    /****************************************************************************/
    void FilterLeicaReagents(bool Filter) { m_FilterLeicaReagent = Filter; }

    void SetVisibleRowCount(int RowCount);
    QString GetReagentID(const QString ReagentName);
    QString GetReagentLongName(int Row);
    bool ContainsReagent(QString ReagentID);
    bool IsReagentPresentInBL(QString ReagentID);
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);

public slots:
    void UpdateReagentList();
    void OnDeviceModeChanged(QString DeviceMode);

private:
    DataManager::CDataReagentList *mp_ReagentList;      //!< Reagent list
    DataManager::CDataReagentGroupList *mp_ReagentGroupList; //!< Reagent group list
    QMap<QString, QString> m_Identifiers;               //!< Identifiers of reagents currently displayed
    QStringList m_ReagentNames;                         //!< Long names of reagents currently displayed
    QStringList m_ReagentID;
    qint32 m_Columns;                                   //!< Number of table columns
    QString m_CurrentReagentName;                       //!< Current Reagent Name
    Programs::CModifyProgramStepDlg *mp_Parent;         //!< Reference to ModifyProgramStep Dialog
    bool m_HideSpecialReagents;                         //!< Hides the special reagents if true.
    bool m_FilterLeicaReagent;                          //!< Filter Leica Reagent if set to true
    qint32 m_VisibleRowCount;                           //!< Number of rows visible in the table
    QMap<QString, QString> m_ReagentNameMap;            //!< Sorts Reagent Longnames
    QStringList m_VisibleReagentIds;                    //!< Reagent Ids list
    QPixmap m_PixmapTickOk;                             //!< Pixmap for TickOk png
    QString m_DeviceMode;                               //!< Device mode
    DataManager::CUserSettings m_UserSettings;          //!< UserSettings object
};

} // end namespace Core

#endif // CORE_ReagentRMSModel_H
