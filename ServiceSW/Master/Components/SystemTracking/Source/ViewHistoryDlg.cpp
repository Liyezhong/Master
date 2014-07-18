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
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
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
    , mp_ModuleList(new ServiceDataManager::CModuleDataList)
{
    mp_Ui->setupUi(GetContentFrame());
    mp_Model->setColumnCount(1);
    mp_Model->setHorizontalHeaderItem(0,new QStandardItem(tr("Module List")));

    mp_TreeView = new QTreeView(this);
    mp_TreeView->setModel(mp_Model);
    mp_TreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mp_TreeView->resize(600,500);
    mp_TreeView->setWordWrap(true);

    mp_Ui->widget->SetContent(mp_TreeView);

    connect(mp_Ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(mp_Ui->expandBtn, SIGNAL(clicked()), mp_TreeView, SLOT(expandAll()));
    connect(mp_Ui->collapseBtn, SIGNAL(clicked()), mp_TreeView, SLOT(collapseAll()));
}

void CViewHistoryDlg::SetModuleList(ServiceDataManager::CModuleDataList *ModuleList)
{
    mp_ModuleList = ModuleList;
}

void CViewHistoryDlg::UpdateGUI(void)
{
    mp_Model->clear();

    mp_Model->setColumnCount(1);
    mp_Model->setHorizontalHeaderItem(0,new QStandardItem(tr("Module List")));

    if (mp_ModuleList == NULL) {
        qDebug()<<"invalid module list";
        return;
    }
    QStandardItem *RootItem = new QStandardItem(mp_ModuleList->GetModuleTimeStamp());

    for(int i=0; i<mp_ModuleList->GetNumberofModules(); i++)
    {
        AddItem(RootItem, mp_ModuleList->GetModule(i));
    }

    mp_Model->setItem(0, RootItem);
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
void CViewHistoryDlg::AddItem(QStandardItem *RootItem, ServiceDataManager::CModule *Module)
{
    //ServiceDataManager::CModule *p_Module = mp_ModuleList->GetModule(i);
    if (0 == Module)
    {
        return;
    }

    QString ModuleName = Module->GetModuleName();
    if (ModuleName.isEmpty())
    {
        return;
    }

    QStandardItem *ModuleItem = new QStandardItem(ModuleName);
    RootItem->appendRow(ModuleItem);

    QString SerialNumber = Module->GetSerialNumber();
    if (!SerialNumber.isEmpty())
    {
        QStandardItem *SN = new QStandardItem(SerialNumber);
        ModuleItem->appendRow(SN);
    }

    QString OperatingHours = Module->GetOperatingHours();
    if (!OperatingHours.isEmpty())
    {
        QStandardItem *Hours = new QStandardItem(OperatingHours);
        ModuleItem->appendRow(Hours);
    }

    QString DateOfProduction = Module->GetDateOfProduction();
    if (!DateOfProduction.isEmpty())
    {
        QStandardItem *DOP = new QStandardItem(DateOfProduction);
        ModuleItem->appendRow(DOP);
    }

    for(int i=0; i<Module->GetNumberofSubModules(); i++)
    {
        ServiceDataManager::CSubModule *p_SubModule =
                Module->GetSubModuleInfo(i);
        if (0 == p_SubModule)
        {
            continue;
        }

        QString SubModuleName = p_SubModule->GetSubModuleName();
        if (SubModuleName.isEmpty())
        {
            continue;
        }

        QStandardItem *SubModuleItem = new QStandardItem(SubModuleName);
        ModuleItem->appendRow(SubModuleItem);

        for(int j=0; j<p_SubModule->GetNumberOfParameters(); j++)
        {
            ServiceDataManager::Parameter_t *Param =
                    p_SubModule->GetParameterInfo(j);

            if (!(Param->ParameterName.isEmpty()))
            {
                QStandardItem *ParamName = new QStandardItem(Param->ParameterName);
                SubModuleItem->appendRow(ParamName);
            }
        }
    }
}

} // end of namespace SystemTracking
