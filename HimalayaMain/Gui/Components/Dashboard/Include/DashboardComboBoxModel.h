/****************************************************************************/
/*! \file DashboardComboBoxModel.h
 *
 *  \brief DashboardComboBoxModel definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02-Apr-2013
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

#ifndef DASHBOARD_DASHBOARDCOMBOBOXMODEL_H
#define DASHBOARD_DASHBOARDCOMBOBOXMODEL_H

#include <QAbstractTableModel>
#include <QStandardItem>
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"


namespace Dashboard {

/****************************************************************************/
/**
 * \brief This is the data model for the program items in Combobox
 */
/****************************************************************************/
class CDashboardComboBoxModel : public QAbstractListModel
{
    Q_OBJECT

public:
    CDashboardComboBoxModel(QObject *p_Parent = NULL);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &Index, int Role) const;

    void SetProgramListContainer(DataManager::CDataProgramList *p_ProgramList);
    void ResetAndUpdateModel();


private:
    DataManager::CDataProgramList *mp_ProgramList;   //!< Program list
    int m_CurrentIndex;
};

}    // end of namespace Dashboard

#endif // DASHBOARD_DASHBOARDCOMBOBOXMODEL_H
