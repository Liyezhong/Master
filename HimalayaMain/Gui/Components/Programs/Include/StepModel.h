/****************************************************************************/
/*! \file StepModel.h
 *
 *  \brief Include file for class CStepModel.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-15
 *   $Author:  $ M.Scherer, C.Adaragunchi1234
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

#ifndef PROGRAMS_STEPMODEL_H
#define PROGRAMS_STEPMODEL_H

#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "MainMenu/Include/TableMoving.h"
#include "MainMenu/Include/ScrollTable.h"
#include <QAbstractTableModel>
#include <QMap>
#include <QStringList>

namespace Programs {
class CModifyProgramDlg;
/****************************************************************************/
/**
 * \brief This is the data model for the program step tables
 */
/****************************************************************************/
class CStepModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CStepModel(QObject *p_Parent = NULL);
    void SetProgram(DataManager::CProgram *p_Program, DataManager::CDataReagentList *p_ReagentList,
                    qint32 Columns);
    void ConnectTableMoving(MainMenu::CTableMoving *p_TableMoving);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &Index, int Role) const;
    QVariant headerData(int Section, Qt::Orientation Orientation, int Role) const;
    Qt::ItemFlags flags(const QModelIndex &Index) const;
    void SetIndex(QModelIndex Index);
    void SetModifyProgramDlgPtr(CModifyProgramDlg *p_ModifyProgram);
    void SetVisibleRowCount(int RowCount);
    qint32 GetIndex();

private:
    QString DurationString(qint32 Duration) const;
    DataManager::CProgram *mp_Program;                  //!< Program displayed in the table
    DataManager::CDataReagentList *mp_ReagentList;      //!< Global reagent list
    qint32 m_Columns;                                   //!< Number of table columns
    qint32 m_CurrentRow;                                //!< Current row selected
    CModifyProgramDlg *mp_ModifyProgramDlg;             //!< View pointer
    qint32 m_VisibleRowCount;                           //!< Number of rows visible in the table


private slots:
    void OnBeginButtonClicked();
    void OnUpButtonClicked();
    void OnDownButtonClicked();
    void OnEndButtonClicked();
    void UpdateStepModel();
};

} // end namespace Programs

#endif // PROGRAMS_STEPMODEL_H
