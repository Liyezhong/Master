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
    , mp_ModuleList(new DataManager::CModuleDataList)
{
    mp_Ui->setupUi(GetContentFrame());

    mp_Model->setColumnCount(1);
    mp_Model->setHorizontalHeaderItem(0,new QStandardItem(tr("Module List")));

    mp_TreeView = new QTreeView(this);
    mp_TreeView->setModel(mp_Model);
    mp_TreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mp_TreeView->resize(550,500);

    mp_Ui->widget->SetContent(mp_TreeView);

    connect(mp_Ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(mp_Ui->expandBtn, SIGNAL(clicked()), mp_TreeView, SLOT(expandAll()));
    connect(mp_Ui->collapseBtn, SIGNAL(clicked()), mp_TreeView, SLOT(collapseAll()));
}

void CViewHistoryDlg::SetModuleListPath(const QString &FilePath)
{
    m_ModuleListFilePath = FilePath;

    mp_ModuleList->ReadFile(m_ModuleListFilePath);
}

void CViewHistoryDlg::UpdateGUI(void)
{
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

        delete mp_ModuleList;

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
void CViewHistoryDlg::AddItem(DataManager::CModuleDataList *p_ModuleList)
{
    mp_Model->clear();

    mp_Model->setColumnCount(1);
    mp_Model->setHorizontalHeaderItem(0,new QStandardItem(tr("Module List")));

    QStandardItem *RootItem =
            new QStandardItem(p_ModuleList->GetModuleTimeStamp());

    for(int i=0; i<p_ModuleList->GetNumberofModules(); i++)
    {
        DataManager::CModule *p_Module = p_ModuleList->GetModule(i);
        if (0 == p_Module)
        {
            continue;
        }

        QString ModuleName = p_Module->GetModuleName();
        if (ModuleName.isEmpty())
        {
            continue;
        }

        QStandardItem *ModuleItem = new QStandardItem(ModuleName);
        RootItem->appendRow(ModuleItem);

        QString SerialNumber = p_Module->GetSerialNumber();
        if (!SerialNumber.isEmpty())
        {
            QStandardItem *SN = new QStandardItem(SerialNumber);
            ModuleItem->appendRow(SN);
        }

        QString OperatingHours = p_Module->GetOperatingHours();
        if (!OperatingHours.isEmpty())
        {
            QStandardItem *Hours = new QStandardItem(OperatingHours);
            ModuleItem->appendRow(Hours);
        }

        /*QString DateOfProduction = p_Module->GetDateOfProduction();
        if (!DateOfProduction.isEmpty())
        {
            QStandardItem *DOP = new QStandardItem(DateOfProduction);
            ModuleItem->appendRow(DOP);
        }*/

        for(int j=0; j<p_Module->GetNumberofSubModules(); j++)
        {
            DataManager::CSubModule *p_SubModule =
                    p_Module->GetSubModuleInfo(j);
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

            for(int k=0; k<p_SubModule->GetNumberOfParameters(); k++)
            {
                DataManager::Parameter_t *Param =
                        p_SubModule->GetParameterInfo(k);

                if (!(Param->ParameterName.isEmpty()))
                {
                    QStandardItem *ParamName =
                            new QStandardItem(Param->ParameterName);
                    SubModuleItem->appendRow(ParamName);
                }
            }
        }
    }

    mp_Model->setItem(0, RootItem);
}

} // end of namespace SystemTracking
