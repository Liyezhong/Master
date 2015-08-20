/****************************************************************************/
/*! \file ViewHistoryDiffDlg.cpp
 *
 *  \brief ViewHistoryDiffDlg implementation.
 *
 *  \b Description:
 *          This class displays difference between two modulelists.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-18
 *   $Author:  $ Soumya. D
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <ServiceDataManager/Include/ModuleDataList.h>
#include <SystemTracking/Include/ViewHistoryDiffDlg.h>
#include <Global/Include/Utils.h>
#include "ui_ViewHistoryDiffDlg.h"
#include <QDebug>

namespace SystemTracking {

const int TREEVIEW_FIXED_WIDTH = 420;           ///< Tree view fixed width
const int TREEVIEW_FIXED_HEIGHT = 700;          ///< Tree view fixed height
const int TREEVIEW_FIXED_COLUMN_WIDTH = 210;    ///< Tree view fixed column width
const int VIEWHISTORY_DIFF_DIALOG_WIDTH = 790;  ///< View history difference dialog width
const int VIEWHISTORY_DIFF_DIALOG_HEIGHT = 550; ///< View history dialog height

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_DataConnector = Service GUI connector object
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CViewHistoryDiffDlg::CViewHistoryDiffDlg(QWidget *p_Parent)
    : MainMenu::CDialogFrame(p_Parent),
      mp_Ui(new Ui::CViewHistoryDiffDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    SetDialogTitle(QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                                           "Module List Difference", 0, QApplication::UnicodeUTF8));
    resize(VIEWHISTORY_DIFF_DIALOG_WIDTH, VIEWHISTORY_DIFF_DIALOG_HEIGHT);

    QFont Font;
    Font.setPixelSize(15);
    setModal(true);
    mp_TreeView = new QTreeView(this);
    mp_TreeView->setModel(&m_Model);
    mp_TreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mp_TreeView->resize(TREEVIEW_FIXED_WIDTH, TREEVIEW_FIXED_HEIGHT);
    mp_TreeView->setFont(Font);

    mp_TreeViewRight = new QTreeView(this);
    mp_TreeViewRight->setModel(&m_ModelRight);
    mp_TreeViewRight->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mp_TreeViewRight->resize(TREEVIEW_FIXED_WIDTH, TREEVIEW_FIXED_HEIGHT);
    mp_TreeViewRight->setFont(Font);

    mp_Ui->moduleListWidget->SetContent(mp_TreeView);
    mp_Ui->moduleListDiffWidget->SetContent(mp_TreeViewRight);   

    mp_MessageDlg = new MainMenu::CMessageDlg(p_Parent);
    mp_MessageDlg->setModal(true);

    CONNECTSIGNALSLOTGUI(mp_Ui->closeBtn, clicked(), this, CloseDialog());
    CONNECTSIGNALSLOTGUI(mp_Ui->expandAllBtn, clicked(), mp_TreeView, expandAll());
    CONNECTSIGNALSLOTGUI(mp_Ui->collapseAllBtn, clicked(), mp_TreeView, collapseAll());
    CONNECTSIGNALSLOTGUI(mp_Ui->expandAllBtn, clicked(), mp_TreeViewRight, expandAll());
    CONNECTSIGNALSLOTGUI(mp_Ui->collapseAllBtn, clicked(), mp_TreeViewRight, collapseAll());

    mp_TreeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    mp_TreeView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mp_TreeViewRight->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    mp_TreeViewRight->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CViewHistoryDiffDlg::~CViewHistoryDiffDlg()
{
    try {        
        delete mp_MessageDlg;
        delete mp_TreeViewRight;
        delete mp_TreeView;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CViewHistoryDiffDlg::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        mp_Ui->retranslateUi(this);
        RetranslateUI();
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief  To initialize the dialog
 *  \iparam ModuleListOne = Modulelist Object
 *  \iparam ModuleListTwo = Modulelist Object
 */
/****************************************************************************/
void CViewHistoryDiffDlg::Show(ServiceDataManager::CModuleDataList &ModuleListOne, ServiceDataManager::CModuleDataList &ModuleListTwo)
{
    bool hasDiff = false;
    m_Model.clear();
    m_ModelRight.clear();
    SetTreeProperties();

    ServiceDataManager::CModuleDataList ModuleListClone1, ModuleListClone2;
    ModuleListClone1.SetModuleTimeStamp(ModuleListOne.GetModuleTimeStamp());
    ModuleListClone2.SetModuleTimeStamp(ModuleListTwo.GetModuleTimeStamp());

    ServiceDataManager::CModule ModuleOne, ModuleTwo;
    QStringList SameSubModule;

    for (int i = 0; i < ModuleListOne.GetNumberofModules(); i++) {
        SameSubModule.clear();
        (void) ModuleListOne.GetModule(ModuleListOne.GetModule(i)->GetModuleName(), ModuleOne);
        (void) ModuleListTwo.GetModule(ModuleOne.GetModuleName(), ModuleTwo);

        for (int j = 0; j < ModuleOne.GetNumberofSubModules(); ++j) {
            ServiceDataManager::CSubModule* SubModuleOne = ModuleOne.GetSubModuleInfo(j);
            ServiceDataManager::CSubModule* SubModuleTwo = ModuleTwo.GetSubModuleInfo(SubModuleOne->GetSubModuleName());
            if (SubModuleTwo && *SubModuleOne == *SubModuleTwo) {
                SameSubModule<<SubModuleOne->GetSubModuleName();
            }
        }

        for (int k = 0; k < SameSubModule.count(); ++k) {
            (void)ModuleOne.DeleteSubModule(SameSubModule.at(k));
            (void)ModuleTwo.DeleteSubModule(SameSubModule.at(k));
        }

        if (ModuleOne.GetNumberofSubModules() == 0) {
            if (ModuleOne.GetDateOfProduction() == ModuleTwo.GetDateOfProduction()
                    && ModuleOne.GetSerialNumber() == ModuleTwo.GetSerialNumber()
                    && ModuleOne.GetOperatingHours() == ModuleTwo.GetOperatingHours()) {
                continue;
            }
        }

        (void) ModuleListClone1.AddModule(&ModuleOne);
        (void) ModuleListClone2.AddModule(&ModuleTwo);
        hasDiff = true;
    }

    if (hasDiff) {
        AddItem(ModuleListClone1);
        AddItemToTree(ModuleListClone2);
        this->show();
    }
    else {
        mp_MessageDlg->SetTitle(QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                                                        "View History", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                              "No difference between selected module timestamps.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Information);
        mp_MessageDlg->show();
    }
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the tree
 *  \iparam ModuleList = Module List object
 *  \return Returns tree item
 */
/****************************************************************************/
QStandardItem* CViewHistoryDiffDlg::AddModuleInfo(ServiceDataManager::CModuleDataList& ModuleList)
{
    QStandardItem *RootItem = new QStandardItem(ModuleList.GetModuleTimeStamp());

    for (int i = 0; i < ModuleList.GetNumberofModules(); i++) {
        ServiceDataManager::CModule Module;
        (void) ModuleList.GetModule(ModuleList.GetModule(i)->GetModuleName(), Module);
        QString ModuleName = Module.GetModuleName();

        QStandardItem *ModuleItem = new QStandardItem(ModuleName);
        RootItem->appendRow(ModuleItem);

        QStandardItem *SerialNumber = new QStandardItem(QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                                                       "Serial Number", 0, QApplication::UnicodeUTF8));
        QStandardItem *OperatingHours = new QStandardItem(QApplication::translate
                                                          ("SystemTracking::CViewHistoryDiffDlg", "Operating Hours",
                                                           0, QApplication::UnicodeUTF8));
        QStandardItem *DateOfProduction = new QStandardItem(QApplication::translate
                                                      ("SystemTracking::CViewHistoryDiffDlg", "Date Of Production",
                                                       0, QApplication::UnicodeUTF8));

        QStandardItem *SN = new QStandardItem(Module.GetSerialNumber());
        QFont TextFont;
        TextFont.setPixelSize(14);
        SN->setFont(TextFont);
        QStandardItem *Hours = new QStandardItem(Module.GetOperatingHours());
        QStandardItem *DOP = new QStandardItem(Module.GetDateOfProduction());

        ModuleItem->setChild(0, 0, SerialNumber);
        ModuleItem->setChild(0, 1, SN);

        ModuleItem->setChild(1, 0, OperatingHours);
        ModuleItem->setChild(1, 1, Hours);

        ModuleItem->setChild(2, 0, DateOfProduction);
        ModuleItem->setChild(2, 1, DOP);


        for (int j = 0; j < Module.GetNumberofSubModules(); j++) {
            ServiceDataManager::CSubModule* SubModule;

            SubModule = Module.GetSubModuleInfo(j);
            QStandardItem *SubModuleName = new QStandardItem(SubModule->GetSubModuleName());

            ModuleItem->appendRow(SubModuleName);
            ServiceDataManager::Parameter_t *Param;

            for (int k = 0; k < SubModule->GetNumberOfParameters(); k++) {
                int Count=0;
                Param = SubModule->GetParameterInfo(k);

                QStandardItem *ParamName = new QStandardItem(Param->ParameterName);
                QStandardItem *ParamValue = new QStandardItem(Param->ParameterValue);

                SubModuleName->setChild(k, Count++, ParamName);
                SubModuleName->setChild(k, Count++, ParamValue);
            }
        }
    }
    return RootItem;
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam p_ModuleList = Module List object
 */
/****************************************************************************/
void CViewHistoryDiffDlg::AddItem(ServiceDataManager::CModuleDataList& ModuleList)
{
    m_Model.setColumnCount(2);
    mp_TreeView->setColumnWidth(0, TREEVIEW_FIXED_COLUMN_WIDTH);
    mp_TreeView->setWordWrap(true);
    mp_TreeView->setTextElideMode(Qt::ElideNone);


    m_Model.setItem(0, AddModuleInfo(ModuleList));
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam p_ModuleList = Module List object
 */
/****************************************************************************/
void CViewHistoryDiffDlg::AddItemToTree(ServiceDataManager::CModuleDataList &ModuleList)
{
    m_ModelRight.setColumnCount(2);
    mp_TreeViewRight->setColumnWidth(0, TREEVIEW_FIXED_COLUMN_WIDTH);
    mp_TreeViewRight->setWordWrap(true);
    mp_TreeViewRight->setTextElideMode(Qt::ElideNone);   

    m_ModelRight.setItem(0, AddModuleInfo(ModuleList));
}

/****************************************************************************/
/*!
  *  \brief  Slot called to close the dialog
  */
/****************************************************************************/
void CViewHistoryDiffDlg::CloseDialog()
{
    m_Model.clear();
    m_ModelRight.clear();
    (void) close();
}

/****************************************************************************/
/*!
  *  \brief  To set Tree view properties
  */
/****************************************************************************/
void CViewHistoryDiffDlg::SetTreeProperties()
{
    m_Model.setColumnCount(1);
    m_Model.setHorizontalHeaderItem(0,new QStandardItem(QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                                                       "Module List", 0, QApplication::UnicodeUTF8)));
    m_Model.setHorizontalHeaderItem(1,new QStandardItem(tr("Value")));

    m_ModelRight.setColumnCount(1);
    m_ModelRight.setHorizontalHeaderItem(0,new QStandardItem(QApplication::translate
                                        ("SystemTracking::CViewHistoryDiffDlg", "Module List",
                                         0, QApplication::UnicodeUTF8)));

    m_ModelRight.setHorizontalHeaderItem(1,new QStandardItem(tr("Value")));

}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CViewHistoryDiffDlg::RetranslateUI()
{
    m_Model.setHorizontalHeaderItem(0,new QStandardItem(QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                                                       "Module List", 0, QApplication::UnicodeUTF8)));

    m_ModelRight.setHorizontalHeaderItem(0,new QStandardItem(QApplication::translate
                                        ("SystemTracking::CViewHistoryDiffDlg", "Module List",
                                         0, QApplication::UnicodeUTF8)));

    mp_MessageDlg->SetTitle(QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                                                    "View History", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                          "No difference between selected module timestamps.", 0, QApplication::UnicodeUTF8));

    SetDialogTitle(QApplication::translate("SystemTracking::CViewHistoryDiffDlg",
                                           "Module List Difference", 0, QApplication::UnicodeUTF8));
}

}   // end of namespace SystemTracking
