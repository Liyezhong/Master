/****************************************************************************/
/*! \file StepModel.cpp
 *
 *  \brief Implementation file for class CStepModel.
 *
 *  \b Description:
 *          This class implements the Program Step Model required for Edit/Add
 *          Program widget to display list of program steps of selected Program
 *          in the table.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-15
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
#include "Programs/Include/StepModel.h"
#include "Programs/Include/ModifyProgramDlg.h"
#include <QDebug>
#include <QPainter>
#include <QPixmap>

namespace Programs {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CStepModel::CStepModel(QObject *p_Parent) : QAbstractTableModel(p_Parent),
                                             mp_Program(NULL), mp_ReagentList(NULL),
                                             m_Columns(0), m_CurrentRow(0),
                                             mp_ModifyProgramDlg(NULL), m_VisibleRowCount(6)

{

}

/****************************************************************************/
/*!
 *  \brief Initializes the program data
 *
 *  \iparam p_Program = Program data
 *  \iparam p_ReagentList = Reagent list
 *  \iparam Columns = Table columns
 */
/****************************************************************************/
void CStepModel::SetProgram(DataManager::CProgram *p_Program, DataManager::CDataReagentList *p_ReagentList,
                            qint32 Columns)
{
    beginResetModel();
    mp_Program = p_Program;
    mp_ReagentList = p_ReagentList;
    m_Columns = Columns;
    m_CurrentRow = 0;
    endResetModel();
}

/****************************************************************************/
/*!
 *  \brief A copy of Modify Program Dlg pointer
 *
 *  \iparam p_ModifyProgram = Pointer to Modify Program Dlg
 */
/****************************************************************************/
void CStepModel::SetModifyProgramDlgPtr(CModifyProgramDlg *p_ModifyProgram)
{
    mp_ModifyProgramDlg = p_ModifyProgram;
}

/****************************************************************************/
/*!
 *  \brief Sets the number of rows visible in the table
 *
 *  \iparam RowCount
 */
/****************************************************************************/
void CStepModel::SetVisibleRowCount(int RowCount) {
    beginResetModel();
    m_VisibleRowCount = RowCount;
    endResetModel();
}

/****************************************************************************/
/*!
 *  \brief Connect a table move widget to be able to move table rows
 *
 *  \iparam p_TableMoving = Table move widget
 */
/****************************************************************************/
void CStepModel::ConnectTableMoving(MainMenu::CTableMoving *p_TableMoving)
{
    CONNECTSIGNALSLOT(p_TableMoving, OnBeginButtonClicked(), this, OnBeginButtonClicked());
    CONNECTSIGNALSLOT(p_TableMoving, OnUpButtonClicked(), this, OnUpButtonClicked());
    CONNECTSIGNALSLOT(p_TableMoving, OnDownButtonClicked(), this, OnDownButtonClicked());
    CONNECTSIGNALSLOT(p_TableMoving, OnEndButtonClicked(), this, OnEndButtonClicked());
}

/****************************************************************************/
/*!
 *  \brief Returns the number of rows in the table
 *
 *  \return Row count
 */
/****************************************************************************/
int CStepModel::rowCount(const QModelIndex &) const
{
    if (mp_Program != NULL) {
        int NoOfSteps = mp_Program->GetNumberOfSteps();
        return ((NoOfSteps < m_VisibleRowCount) ? m_VisibleRowCount : NoOfSteps);
    }
    else {
        return m_VisibleRowCount;
    }
}

/****************************************************************************/
/*!
 *  \brief Returns the number of columns in the table
 *
 *  \return Columns count
 */
/****************************************************************************/
int CStepModel::columnCount(const QModelIndex &) const
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
QVariant CStepModel::data(const QModelIndex &Index, int Role) const
{
    DataManager::CProgramStep *Step;

    if (mp_Program == NULL || mp_ReagentList == NULL) {
        if (Role == (int)Qt::BackgroundRole) {
            QPalette Palette;
            return QVariant(Palette.color(QPalette::Window));
        }
        return QVariant();
    }

    if ((Step = const_cast<DataManager::CProgramStep*>(mp_Program->GetProgramStep(Index.row()))) != NULL) {
        if (Role == (int)Qt::DisplayRole) {
            switch (Index.column()) {
            case 0:
                return Index.row() + 1;
            case 1:
            {
                DataManager::CReagent Reagent;
                if (mp_ReagentList->GetReagent(Step->GetReagentID(), Reagent) == true) {
                    return Reagent.GetReagentName();
                }
                else {
                    return QVariant();
                }
            }
            case 2:
                return DurationString(Step->GetMinDurationInSeconds());
            case 3:
                return QString("%1%2").arg(Step->GetMaxDurationInPercent()).arg("%");
            }
        }
        else if (Role == (int)Qt::DecorationRole) {
            switch (Index.column()) {
            case 4:
                if (Step->GetExclusive() == true) {
                    return QPixmap(":/Large/Icons/MISC/TickOk.png");
                }
                else {
                    return QVariant();
                }
            }
        }
        else if (Role == (int)Qt::UserRole) {
            return Step->GetStepID();
        }
    }
    else if (Role == (int)Qt::BackgroundRole) {
        //Grays the empty lines
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
QVariant CStepModel::headerData(int Section, Qt::Orientation Orientation, int Role) const
{
    if (Role == (int)Qt::DisplayRole && Orientation == Qt::Horizontal) {
        switch (Section) {
        case 0:
            return tr("Step");
        case 1:
            return tr("Reagent");
        case 2:
            return tr("Duration");
        case 3:
            return tr("Temperature");
        case 4:
            return tr("P/V");
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
Qt::ItemFlags CStepModel::flags(const QModelIndex &Index) const
{
    if (mp_Program) {
        if (Index.row() >= mp_Program->GetNumberOfSteps()) {
            return Qt::NoItemFlags;
        }
    }
    else {
        return Qt::NoItemFlags;
    }
    return QAbstractItemModel::flags(Index);
}
/****************************************************************************/
/*!
 *  \brief Converts a time in seconds into a string of format hh:mm:ss
 *
 *  \iparam Duration = Time in seconds
 *
 *  \return Duration string
 */
/****************************************************************************/
QString CStepModel::DurationString(qint32 Duration) const
{
    return QString("%1:%2:%3")
            .arg(Duration / 3600, 2, 10, QChar('0'))
            .arg((Duration % 3600) / 60, 2, 10, QChar('0'))
            .arg(Duration % 60, 2, 10, QChar('0'));
}

/****************************************************************************/
/*!
 *  \brief Sets the StepModel Index
 *  \iparam Index
 */
/****************************************************************************/
void CStepModel::SetIndex(QModelIndex Index)
{
    m_CurrentRow = Index.row();
}

/****************************************************************************/
/*!
 *  \brief Sets the StepModel Index
 *  \return m_CurrentRow
 */
/****************************************************************************/
qint32 CStepModel::GetIndex()
{
    return  m_CurrentRow;
}
/****************************************************************************/
/*!
 *  \brief Moves the selected row to the beginnig of a table
 */
/****************************************************************************/
void CStepModel::OnBeginButtonClicked()
{
    if (mp_Program && mp_ModifyProgramDlg) {
        beginResetModel();
        (void)mp_Program->MoveProgramStep(m_CurrentRow, 0);
        endResetModel();
        m_CurrentRow = 0;
        mp_ModifyProgramDlg->SetRowFocus(m_CurrentRow);
    }
}

/****************************************************************************/
/*!
 *  \brief Moves the selected row one item up
 */
/****************************************************************************/
void CStepModel::OnUpButtonClicked()
{
    if(m_CurrentRow > 0 && mp_ModifyProgramDlg) {
        if (mp_Program) {
            beginResetModel();
            (void)mp_Program->MoveProgramStep(m_CurrentRow, m_CurrentRow -1);
            endResetModel();
            m_CurrentRow -= 1;
            mp_ModifyProgramDlg->SetRowFocus(m_CurrentRow);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Moves the selected row one item down
 */
/****************************************************************************/
void CStepModel::OnDownButtonClicked()
{
    if (mp_Program && mp_ModifyProgramDlg) {
        if(m_CurrentRow < mp_Program->GetNumberOfSteps()- 1) {
            beginResetModel();
            (void)mp_Program->MoveProgramStep(m_CurrentRow, m_CurrentRow + 1);
            endResetModel();
            m_CurrentRow += 1;
            mp_ModifyProgramDlg->SetRowFocus(m_CurrentRow);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Moves the selected row to the end of a table
 */
/****************************************************************************/
void CStepModel::OnEndButtonClicked()
{
    if (mp_Program && mp_ModifyProgramDlg) {
        beginResetModel();
        int MoveToRow = mp_Program->GetNumberOfSteps() - 1;
        (void)mp_Program->MoveProgramStep(m_CurrentRow, MoveToRow);
        m_CurrentRow = MoveToRow;
        endResetModel();
        mp_ModifyProgramDlg->SetRowFocus(m_CurrentRow);
    }
}

/****************************************************************************/
/*!
 *  \brief Updates Step Model when programs xml is sent from master
 */
/****************************************************************************/
void CStepModel::UpdateStepModel()
{
    beginResetModel();
    endResetModel();
}

} // end namespace Programs
