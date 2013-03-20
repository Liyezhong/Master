/****************************************************************************/
/*! \file ModifyReagentGroupColorDlg.cpp
 *
 *  \brief ModifyReagentGroupColorDlg Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-29
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
#include "Reagents/Include/ModifyReagentGroupColorDlg.h"
#include "ui_ModifyReagentGroupColorDlg.h"
#include <QDebug>

namespace Reagents {
/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CModifyReagentGroupColorDlg::CModifyReagentGroupColorDlg(QWidget *p_Parent, MainMenu::CMainWindow *p_MainWindow) :
    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CModifyReagentGroupColorDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    m_ProcessRunning = false ;

    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->setModel(&m_ReagentStatusColorModel);
    // Set horizontal header item
    m_ReagentStatusColorModel.setHorizontalHeaderItem(0, new QStandardItem(tr("Color")));

    mp_Ui->scrollTable->SetContent(mp_TableWidget);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_TableWidget->SetVisibleRows(8);
    mp_TableWidget->horizontalHeader()->show();
    mp_TableWidget->horizontalHeader()->resizeSection(0, 100);
    mp_TableWidget->selectRow(0);
    // add empty items
    AddEmptyRows();

    CONNECTSIGNALSLOT(mp_TableWidget, pressed(QModelIndex), this, SelectionChanged(QModelIndex));
    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnOk, clicked(), this, OnOk());
//    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    // Init Message dialog
    m_MessageDlg.SetTitle(tr("Information Message"));
    m_MessageDlg.SetIcon(QMessageBox::Information);
    m_MessageDlg.SetButtonText(1, tr("Ok"));
    m_MessageDlg.HideButtons();
}

/****************************************************************************/
/*!
     *  \brief Destructor
     */
/****************************************************************************/
CModifyReagentGroupColorDlg::~CModifyReagentGroupColorDlg()
{
    try {
        // delete all the items in the table
        for (qint32 RowCount = 0; RowCount < m_ReagentStatusColorModel.rowCount(); RowCount++) {
            delete m_ReagentStatusColorModel.takeItem(RowCount);
        }
        delete mp_TableWidget;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint.
    }
}


/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        mp_Ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


/****************************************************************************/
/*!
 *  \brief This slot is called when Cancel button in ModifyReagentDlg is pressed.
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::OnCancel()
{
    emit CancelPressed();
    reject();
}

/****************************************************************************/
/*!
 *  \brief Called when the selection of the table is changed
 *
 *  \iparam Index = Model Index
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::SelectionChanged(QModelIndex Index)
{
    m_ReagentGroup.SetGroupColor(m_ColorNames.value(Index.row()));
    // use style sheet to highlight the border
    mp_TableWidget->setStyleSheet("QTableView::item:selected { background-color:#" + m_ColorNames.value(Index.row()) + "; border-color:red; "
                                  "border-style:outset; border-width:2px; color:black;}");

}

/****************************************************************************/
/*!
 *  \brief Saves the changes in the dialog
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::OnOk()
{
    emit UpdateReagentGroup(m_ReagentGroup);
    accept();
}
/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if (!m_ProcessRunning) {
        //Edit Mode
        mp_Ui->btnOk->setEnabled(true);
        mp_Ui->btnCancel->setText(tr("Cancel"));
    }
    else {
        //View Mode
        mp_Ui->btnOk->setEnabled(false);
        mp_Ui->btnCancel->setText(tr("Close"));
    }
}


/****************************************************************************/
/**
 * \brief Add empty rows in the model
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::AddEmptyRows()
{
    if (m_ReagentStatusColorModel.rowCount() < 9) {
        // display all the file names
        for (qint32 RowCount = m_ReagentStatusColorModel.rowCount(); RowCount < 8; RowCount++) {
            QStandardItem *p_Item = new QStandardItem();
            p_Item->setSelectable(false);
            // append the row in tables
            m_ReagentStatusColorModel.appendRow(p_Item);
            QPalette Palette;
            m_ReagentStatusColorModel.setData(m_ReagentStatusColorModel.index(RowCount, 0), Palette.color(QPalette::Window), Qt::BackgroundColorRole);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Display the day run log files in the list
 *
 *  \iparam FileNames = List of file names
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::SetReagentGroupList(DataManager::CReagentGroupColorList &mp_CReagentGroupColorList,const DataManager::CDataReagentGroupList &ReagentGroupList, const DataManager::CReagentGroup &ReagentGroup)
{   
    // copy the data to local
    m_ReagentGroup = ReagentGroup;
    // check the number of rows
    if (m_ReagentStatusColorModel.rowCount() > 0) {
        // delete all the items in the table
        for (qint32 RowCount = 0; RowCount < m_ReagentStatusColorModel.rowCount(); RowCount++) {
            delete m_ReagentStatusColorModel.takeItem(RowCount);
        }
        //clear the table
        m_ReagentStatusColorModel.clear();
        // Set horizontal header item
        m_ReagentStatusColorModel.setHorizontalHeaderItem(0, new QStandardItem(tr("Color")));
    }

    // display all the file names
    for (qint32 ColorCount = 0; ColorCount < const_cast<DataManager::CReagentGroupColorList&>(mp_CReagentGroupColorList).GetNumberOfReagentGroupColor(); ColorCount++) {
        // for (qint32 ColorCount = 0; ColorCount < const_cast<DataManager::CDataReagentGroupList&>(ReagentGroupList).GetNumberOfReagentGroups(); ColorCount++) {

        //QString ColorName1 = const_cast<DataManager::CDataReagentGroupList&>(ReagentGroupList).GetReagentGroup(ColorCount)->GetGroupColor();

        QString ColorName = const_cast<DataManager::CReagentGroupColorList&>(mp_CReagentGroupColorList).GetCReagentGroupColor(ColorCount)->GetColorValue();

        m_ColorNames << ColorName;

        QStandardItem *p_Item = new QStandardItem();
        // append the row in tables
        m_ReagentStatusColorModel.appendRow(p_Item);
        QColor Color;
        // add '#' to hex value to change to color value
        Color.setNamedColor("#" + ColorName.trimmed());
        QPalette Palette(Color);
        m_ReagentStatusColorModel.setData(m_ReagentStatusColorModel.index(ColorCount, 0), Palette.color(QPalette::Window), Qt::BackgroundColorRole);
    }

    // use style sheet to highlight the border
    mp_TableWidget->setStyleSheet("QTableView::item:selected { background-color:#" + m_ColorNames.value(m_ColorNames.indexOf(m_ReagentGroup.GetGroupColor())) + "; border-color:red; "
                                  "border-style:outset; border-width:2px; color:black; }");
    mp_TableWidget->selectRow(m_ColorNames.indexOf(m_ReagentGroup.GetGroupColor()));


    // add empty rows if required
    AddEmptyRows();

}
}
