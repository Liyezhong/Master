
/****************************************************************************/
/*! \file ModifyReagentGroupColorDlg.cpp
 *
 *  \brief ModifyReagentGroupColorDlg Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-03-15
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
#include "Programs/Include/ModifyProgramIconDlg.h"
#include "ui_ModifyProgramIconDlg.h"
#include <QDebug>

namespace Programs {
/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CModifyProgramIconDlg::CModifyProgramIconDlg(QWidget *p_Parent, MainMenu::CMainWindow *p_MainWindow) :
    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CModifyProgramIconDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    m_ProcessRunning = false ;


    mp_Ui->IconButton_1->setIcon(QIcon(":/HimalayaImages/CheckBox/CheckBox-Checked.png"));
    mp_Ui->IconButton_1->setIconSize(QSize(40, 30));


    mp_Ui->IconButton_2->setIcon(QIcon(":/HimalayaImages/CheckBox/CheckBox-CheckedDisabled.png"));
    mp_Ui->IconButton_2->setIconSize(QSize(40, 30));

    mp_Ui->IconButton_3->setIcon(QIcon(":/HimalayaImages/CheckBox/CheckBox-CheckedPressed.png"));
    mp_Ui->IconButton_3->setIconSize(QSize(40, 30));

    mp_Ui->IconButton_4->setIcon(QIcon(":/HimalayaImages/CheckBox/CheckBox-Disabled.png"));
    mp_Ui->IconButton_4->setIconSize(QSize(40, 30));

    mp_Ui->IconButton_5->setIcon(QIcon(":/HimalayaImages/CheckBox/CheckBox-Enabled.png"));
    mp_Ui->IconButton_5->setIconSize(QSize(40, 30));

    mp_Ui->IconButton_6->setIcon(QIcon(":/HimalayaImages/CheckBox/CheckBox-Pressed.png"));
    mp_Ui->IconButton_6->setIconSize(QSize(40, 30));

    mp_Ui->btnCancel->setText("Cancel");
    mp_Ui->btnOk->setText("Ok");

    // Set horizontal header item
//    m_ProgramIconModel.setHorizontalHeaderItem(0, new QStandardItem(tr("Icon")));
    // Connecting Signal and Slots
//    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    CONNECTSIGNALSLOT(&m_ButtonGroup, buttonClicked(int), this, OnIconGroup(int));
    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnOk, clicked(), this, OnOk());

    m_MessageDlg.SetTitle(tr("Information Message"));
    m_MessageDlg.SetIcon(QMessageBox::Information);
    m_MessageDlg.SetButtonText(1, tr("Ok"));
    m_MessageDlg.HideButtons();
}

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \Function to create Map of PushButton and Creation of Button Group.
 */
/****************************************************************************/
void CModifyProgramIconDlg ::SetButtonGroup()
{
        m_ButtonGroup.addButton(mp_Ui->IconButton_1, 0);
        m_ButtonGroup.addButton(mp_Ui->IconButton_2, 1);
        m_ButtonGroup.addButton(mp_Ui->IconButton_3, 2);
        m_ButtonGroup.addButton(mp_Ui->IconButton_4, 3);
        m_ButtonGroup.addButton(mp_Ui->IconButton_5, 4);
        m_ButtonGroup.addButton(mp_Ui->IconButton_6, 5);

        m_QPushButtonMap[0] = mp_Ui->IconButton_1;
        m_QPushButtonMap[1] = mp_Ui->IconButton_2;
        m_QPushButtonMap[2] = mp_Ui->IconButton_3;
        m_QPushButtonMap[3] = mp_Ui->IconButton_4;
        m_QPushButtonMap[4] = mp_Ui->IconButton_5;
        m_QPushButtonMap[5] = mp_Ui->IconButton_6;

}
/****************************************************************************/
/*!
     *  \brief Destructor
     */
/****************************************************************************/
CModifyProgramIconDlg::~CModifyProgramIconDlg()
{
    delete mp_Ui;
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CModifyProgramIconDlg::changeEvent(QEvent *p_Event)
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
void CModifyProgramIconDlg::OnCancel()
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
//void CModifyProgramIconDlg::SelectionChanged(QModelIndex Index)
//{
////    m_Program.SetGroupColor(m_ColorNames.value(Index.row()));
//    // use style sheet to highlight the border
//    mp_TableWidget->setStyleSheet("QTableView::item:selected { background-color:white; border-color:red; "
//                                  "border-style:outset; border-width:2px; color:black;}");
//}

/****************************************************************************/
/*!
 *  \brief Saves the changes in the dialog
 */
/****************************************************************************/
void CModifyProgramIconDlg::OnOk()
{
    emit UpdateProgram(m_Program);
    accept();
}
/****************************************************************************/
/*!
 *  \brief Selects the Icon in the Widget
 */
/****************************************************************************/
 void CModifyProgramIconDlg::OnIconGroup(int Id)
 {
     mp_Ui->btnOk->setEnabled(true);
     m_Program.SetIcon(m_IconName.value(Id));
 }

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CModifyProgramIconDlg::OnProcessStateChanged()
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
///*!
// *  \brief Display the day run log files in the list
// *
// *  \iparam FileNames = List of file names
// */
///****************************************************************************/
//void CModifyReagentGroupColorDlg::SetReagentGroupList(DataManager::CReagentGroupColorList &mp_CReagentGroupColorList,const DataManager::CDataReagentGroupList &ReagentGroupList, const DataManager::CReagentGroup &ReagentGroup)
//{
//    // copy the data to local
//    m_ReagentGroup = ReagentGroup;
//    // check the number of rows
//    if (m_ReagentStatusColorModel.rowCount() > 0) {
//        // delete all the items in the table
//        for (qint32 RowCount = 0; RowCount < m_ReagentStatusColorModel.rowCount(); RowCount++) {
//            delete m_ReagentStatusColorModel.takeItem(RowCount);
//        }
//        //clear the table
//        m_ReagentStatusColorModel.clear();
//        // Set horizontal header item
//        m_ReagentStatusColorModel.setHorizontalHeaderItem(0, new QStandardItem(tr("Color")));
//    }

//    // display all the file names
//    for (qint32 ColorCount = 0; ColorCount < const_cast<DataManager::CReagentGroupColorList&>(mp_CReagentGroupColorList).GetNumberOfReagentGroupColor(); ColorCount++) {
//        // for (qint32 ColorCount = 0; ColorCount < const_cast<DataManager::CDataReagentGroupList&>(ReagentGroupList).GetNumberOfReagentGroups(); ColorCount++) {

//        //QString ColorName1 = const_cast<DataManager::CDataReagentGroupList&>(ReagentGroupList).GetReagentGroup(ColorCount)->GetGroupColor();

//        QString ColorName = const_cast<DataManager::CReagentGroupColorList&>(mp_CReagentGroupColorList).GetCReagentGroupColor(ColorCount)->GetColorValue();

//        m_ColorNames << ColorName;

//        QStandardItem *p_Item = new QStandardItem();
//        // append the row in tables
//        m_ReagentStatusColorModel.appendRow(p_Item);
//        QColor Color;
//        // add '#' to hex value to change to color value
//        Color.setNamedColor("#" + ColorName.trimmed());
//        QPalette Palette(Color);
//        m_ReagentStatusColorModel.setData(m_ReagentStatusColorModel.index(ColorCount, 0), Palette.color(QPalette::Window), Qt::BackgroundColorRole);
//    }

//    // use style sheet to highlight the border
//    mp_TableWidget->setStyleSheet("QTableView::item:selected { background-color:#" + m_ColorNames.value(m_ColorNames.indexOf(m_ReagentGroup.GetGroupColor())) + "; border-color:red; "
//                                  "border-style:outset; border-width:2px; color:black; }");
//    mp_TableWidget->selectRow(m_ColorNames.indexOf(m_ReagentGroup.GetGroupColor()));


//    // add empty rows if required
//    AddEmptyRows();

//}


}
