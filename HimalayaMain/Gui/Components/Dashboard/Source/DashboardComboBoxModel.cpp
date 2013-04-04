/****************************************************************************/
/*! \file DashboardComboBox.cpp
 *
 *  \brief DashboardComboBox implementation.
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

#include "Dashboard/Include/DashboardComboBoxModel.h"
#include <QDebug>

namespace Dashboard {

const int VISIBLE_ROW_COUNT = 5;    //!< Number of programs visible in the combobox

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CDashboardComboBoxModel::CDashboardComboBoxModel(QObject *p_Parent): QAbstractListModel(p_Parent)
{
    mp_ProgramList = NULL;

}

int CDashboardComboBoxModel::rowCount(const QModelIndex &) const
{
   return mp_ProgramList->GetFavoriteProgramIDs().count();

}

int CDashboardComboBoxModel::columnCount(const QModelIndex &) const
{
    return 2;
}

/****************************************************************************/
/*!
 *  \brief Returns the data items displayed in the Combobox
 *
 *  \iparam Index = Index of a Combobox
 *  \iparam Role = Display role to be returned
 *
 *  \return Data item
 */
/****************************************************************************/
QVariant CDashboardComboBoxModel::data(const QModelIndex &Index, int Role) const
{

    if( mp_ProgramList == NULL)
    {
        return QVariant();
    }

    if (mp_ProgramList->GetFavoriteProgramIDs().count() == 0 || ( 0 == mp_ProgramList->GetNumberOfPrograms() || (!Index.isValid())))
    {
        return QVariant();
    }

    /* QString ProgramId = FavProgramList.at(Index.row());

   if (Role == (int)Qt::DisplayRole) {
        if(Index.row() >= 0) {

            QString ProgramName = mp_ProgramList->GetProgram(ProgramId)->GetName();
            return ProgramName;
        }
    } else if(Role == (int)Qt::DecorationRole) {
        if(Index.row() >= 0) {
            if(ProgramId.at(0) == 'L') {
                return QPixmap(":/HimalayaImages/Icons/MISC/Icon_Leica.png");
            } else {
                return QPixmap(":/HimalayaImages/Icons/MISC/TickOk.png");
            }
            //QString Icon = p_Program->GetIcon();
            //return QPixmap(":/Settings/icons" + Icon + ".png");
            //return QPixmap(":/HimalayaImages/Icons/MISC/Icon_Leica.png");
        }
    } else if(Role == (int)Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    }*/

    return QVariant();
}


void CDashboardComboBoxModel::SetProgramListContainer(DataManager::CDataProgramList *p_ProgramList)
{
    mp_ProgramList = p_ProgramList;
}


void CDashboardComboBoxModel:: ResetAndUpdateModel()
{
    beginResetModel();
    endResetModel();

}
}
