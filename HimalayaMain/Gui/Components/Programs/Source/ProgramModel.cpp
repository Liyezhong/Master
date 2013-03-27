/****************************************************************************/
/*! \file ProgramModel.cpp
 *
 *  \brief Implementation file for class CProgramModel.
 *
 *  \b Description:
 *          This class implements the Program Model required for Program Widget
 *          to display list of programs in the table.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-08-25
 *   $Author:  $ M.Scherer, N.Kamath, C.Adaragunchi1234
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "Programs/Include/ProgramModel.h"
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QApplication>
#include <QEvent>


namespace Programs {
const int VISIBLE_ROW_COUNT = 8;    //!< Number of rows visible in a table
const int NUMBER_OF_COLUMNS = 5;    //!< Number of columns in a table

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CProgramModel::CProgramModel(QObject *p_Parent) : QAbstractTableModel(p_Parent)
{
    mp_ProgramList = NULL;
    m_Columns = 0;
    m_VisibleRowCount = VISIBLE_ROW_COUNT;
}

/****************************************************************************/
/*!
 *  \brief Initializes the program data
 *
 *  \iparam p_ProgramList = Program data
 *  \iparam p_ProgramSequenceList = ProgramSequenceList data
 *  \iparam Columns = Table columns
 */
/****************************************************************************/
void CProgramModel::SetProgramList(DataManager::CDataProgramList *p_ProgramList, qint32 Columns)
{
    mp_ProgramList = p_ProgramList;
    m_Columns = Columns;
}

/****************************************************************************/
/*!
 *  \brief Sets the number of rows visible in the table
 *
 *  \iparam RowCount
 */
/****************************************************************************/
void CProgramModel::SetVisibleRowCount(int RowCount) {
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
int CProgramModel::rowCount(const QModelIndex &) const
{
    if (mp_ProgramList != NULL) {
        //Show empty rows if number of programs is less than visible rows in table.
        int Count = mp_ProgramList->GetNumberOfPrograms();
        return ((Count < m_VisibleRowCount) ? m_VisibleRowCount : Count);
    }
    else {
        return 0;
    }
}

/****************************************************************************/
/*!
 *  \brief Returns the number of columns in the table
 *
 *  \return Columns count
 */
/****************************************************************************/
int CProgramModel::columnCount(const QModelIndex &) const
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
QVariant CProgramModel::data(const QModelIndex &Index, int Role) const
{
    DataManager::CProgram *p_Program = NULL;

    if (mp_ProgramList == NULL) {
        return QVariant();
    }

    if ((p_Program = const_cast<DataManager::CProgram*>(mp_ProgramList->GetProgram(Index.row())))) {
        if (Role == (int)Qt::DisplayRole) {
            switch (5 - m_Columns + Index.column()) {
            case -1:
                return Index.row() + 1;
            case 0:
                return Index.row() + 1;
            case 2:
                return p_Program->GetName();
            case 3:
                 if (p_Program->GetID().at(0) == 'L') {
                 return QPixmap(":/Small/CheckBox/CheckBox-CheckedPressed.png");
                  }
                  else {
                          return QVariant();
                  }
            }
        }
            else if(Role ==(int)Qt::DecorationRole)
            {
                switch (5 - m_Columns + Index.column()) {
            case 1:
                    if (m_CurrentUserRole == MainMenu::CMainWindow::Admin
                        || m_CurrentUserRole == MainMenu::CMainWindow::Service)
                    {
                        if(m_CurrentIndex)
                            return QPixmap(":/Small/CheckBox/CheckBox-Checked.png");

                        else
                        return QPixmap(":/Small/CheckBox/CheckBox-Enabled.png");

                    }

                    else
                    {
                        return QPixmap(":/Small/CheckBox/CheckBox-Disabled.png");
                    }
            }
        }

        else if (Role == (int)Qt::CheckStateRole) {
            if (m_Columns == 6 && NUMBER_OF_COLUMNS - m_Columns + Index.column() == 0) {
                // TODO test code
                if (p_Program->GetID().at(0) == 'L') {
                    return (int)Qt::Checked;
                }
                else {
                    return (int)Qt::Unchecked;
                }
            }
        }
        else if (Role == (int)Qt::UserRole) {
            return p_Program->GetID();
        }
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
Qt::ItemFlags CProgramModel::flags(const QModelIndex &Index) const
{
    if (NUMBER_OF_COLUMNS - m_Columns + Index.column() == 0) {
        return QAbstractItemModel::flags(Index) | Qt::ItemIsUserCheckable;
    }
    if (mp_ProgramList) {
        if (Index.row() >= mp_ProgramList->GetNumberOfPrograms()) {
            return Qt::NoItemFlags;
        }
    }
    return QAbstractItemModel::flags(Index);
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
QVariant CProgramModel::headerData(int Section, Qt::Orientation Orientation, int Role) const
{
    if (Role == (int)Qt::DisplayRole && Orientation == Qt::Horizontal) {
        switch (NUMBER_OF_COLUMNS - m_Columns + Section) {
        case 0:
            return tr("Number");
        case 1:
            return tr("Use");
        case 2:
            return tr("Name");
        case 3:
            return tr("Duration");
        case 4:
            return tr("Icon");
        }
    }
    return QVariant();
}

/****************************************************************************/
/*!
 *  \brief Sets the content of table items
 *
 *  \iparam Index = Index of a table cell
 *  \iparam Value = The new value
 *  \iparam Role = Display role to be modified
 *
 *  \return Changed something (true) or not (false)
 */
/****************************************************************************/
bool CProgramModel::setData(const QModelIndex &Index, const QVariant &Value, int Role)
{
    Q_UNUSED(Value);
    DataManager::CProgram Program;

    if (mp_ProgramList == NULL) {
        return false;
    }
    if (Role == (int)Qt::CheckStateRole) {
       if (mp_ProgramList->GetProgram(Index.row(), Program) == true) {
            if (NUMBER_OF_COLUMNS - m_Columns + Index.column() == 0) {
                //vinay Program.LockProgram();
                return mp_ProgramList->UpdateProgram(&Program);
            }
        }
    }
    return false;
}

/****************************************************************************/
/*!
 *  \brief Updates the model whenever GUI receives an updated XML from Master
 */
/****************************************************************************/
void CProgramModel::UpdateProgramList()
{
    beginResetModel();
    endResetModel();
}

/****************************************************************************/
/*!
 *  \brief Sets the User Role to Current User Role
 */
/****************************************************************************/
void CProgramModel ::SetUserRole(MainMenu::CMainWindow::UserRole_t UserRole)
{
    m_CurrentUserRole = UserRole;
}

void CProgramModel :: SelectedRowIndex(int Index)
{
    m_CurrentIndex = Index;
}

/****************************************************************************/
/*!
 *  \brief Calls the function to Update the model
 */
/****************************************************************************/
void CProgramModel:: ResetandUpdateModel()
{
    UpdateProgramList();
}

} // end namespace Programs
