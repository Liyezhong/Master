/****************************************************************************/
/*! \file ViewHistoryDlg.cpp
 *
 *  \brief History for moduleList implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-10
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

#include "SystemTracking/Include/ViewHistoryDlg.h"
#include "ui_ViewHistoryDlg.h"

namespace SystemTracking {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CViewHistoryDlg::CViewHistoryDlg(QWidget *p_Parent)
    : MainMenu::CDialogFrame(p_Parent)
    , mp_Ui(new Ui::CViewHistoryDlg)
    , mp_Model(new QStandardItemModel)
    , mp_ModuleList(NULL)
{
    mp_Ui->setupUi(GetContentFrame());
    mp_Model->setColumnCount(1);
    mp_Model->setHorizontalHeaderItem(0,new QStandardItem(tr("Module List")));
    mp_Model->setHorizontalHeaderItem(1,new QStandardItem(tr("Value")));

    mp_TreeView = new QTreeView(this);
    mp_TreeView->setModel(mp_Model);
    mp_TreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mp_TreeView->resize(600,500);
    mp_TreeView->setWordWrap(true);

    mp_Ui->widget->SetContent(mp_TreeView);

    (void)connect(mp_Ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));
    (void)connect(mp_Ui->expandBtn, SIGNAL(clicked()), mp_TreeView, SLOT(expandAll()));
    (void)connect(mp_Ui->collapseBtn, SIGNAL(clicked()), mp_TreeView, SLOT(collapseAll()));
}

void CViewHistoryDlg::SetModuleList(ServiceDataManager::CModuleDataList *ModuleList)
{
    mp_ModuleList = ModuleList;
}

void CViewHistoryDlg::UpdateGUI(void)
{
    mp_Model->clear();

    //mp_Model->setColumnCount(1);
    mp_Model->setHorizontalHeaderItem(0,new QStandardItem(tr("Module List")));
    mp_Model->setHorizontalHeaderItem(1,new QStandardItem(tr("Value")));

    if (mp_ModuleList == NULL) {
        qDebug()<<"invalid module list";
        return;
    }
    AddItem(mp_ModuleList);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CViewHistoryDlg::~CViewHistoryDlg()
{
    try {
        delete mp_TreeView;

        delete mp_Model;

        delete mp_Ui;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam ModuleList = Module List object
 */
/****************************************************************************/
void CViewHistoryDlg::AddItem(ServiceDataManager::CModuleDataList *p_ModuleList)
{
    mp_Model->setColumnCount(2);
    mp_TreeView->setColumnWidth(0, 300);
    mp_TreeView->setWordWrap(true);
    QStandardItem *RootItem = new QStandardItem(p_ModuleList->GetModuleTimeStamp());
    ServiceDataManager::CModule *Module;
    ServiceDataManager::CSubModule *SubModule;

    for (int i = 0; i < p_ModuleList->GetNumberofModules(); i++) {
        Module = p_ModuleList->GetModule(i);
        QString ModuleName = Module->GetModuleName();

        QStandardItem *ModuleItem = new QStandardItem(ModuleName);
        RootItem->appendRow(ModuleItem);

        QStandardItem *SerialNumber     = new QStandardItem(QApplication::translate
                                            ("SystemTracking::CViewHistoryDlg", "Serial Number",
                                             0, QApplication::UnicodeUTF8));
        QStandardItem *OperatingHours   = new QStandardItem(QApplication::translate
                                            ("SystemTracking::CViewHistoryDlg", "Operating Hours",
                                             0, QApplication::UnicodeUTF8));
        QStandardItem *DateOfProduction = new QStandardItem(QApplication::translate
                                            ("SystemTracking::CViewHistoryDlg", "Date Of Production",
                                             0, QApplication::UnicodeUTF8));


        QStandardItem *SN    = new QStandardItem(Module->GetSerialNumber());
        QStandardItem *Hours = new QStandardItem(Module->GetOperatingHours());
        QStandardItem *DOP   = new QStandardItem(Module->GetDateOfProduction());

        ModuleItem->setChild(0, 0, SerialNumber);
        ModuleItem->setChild(0, 1, SN);

        ModuleItem->setChild(1, 0, OperatingHours);
        ModuleItem->setChild(1, 1, Hours);

        ModuleItem->setChild(2, 0, DateOfProduction);
        ModuleItem->setChild(2, 1, DOP);


        for (int j = 0; j < Module->GetNumberofSubModules(); j++) {
            SubModule = const_cast<ServiceDataManager::CModule*>(Module)->GetSubModuleInfo(j);
            QStandardItem *SubModuleName = new QStandardItem(SubModule->GetSubModuleName());

            ModuleItem->appendRow(SubModuleName);

            ServiceDataManager::Parameter_t *Param;

            for (int k = 0; k < SubModule->GetNumberOfParameters(); k++) {
                int Count = 0;
                Param = SubModule->GetParameterInfo(k);

                QStandardItem *ParamName = new QStandardItem(Param->ParameterName);
                QStandardItem *ParamValue = new QStandardItem(Param->ParameterValue);

                SubModuleName->setChild(k, Count++, ParamName);
                SubModuleName->setChild(k, Count++, ParamValue);

            }
        }
    }

    mp_Model->setItem(0, RootItem);
}

} // end of namespace SystemTracking
