/****************************************************************************/
/*! \file CurrentConfigurationDlg.cpp
 *
 *  \brief Current Configuration Dialog implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-02
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

#include "SystemTracking/Include/CurrentConfigurationDlg.h"
#include "ui_CurrentConfigurationDlg.h"

namespace SystemTracking {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CCurrentConfigurationDlg::CCurrentConfigurationDlg(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CCurrentConfigurationDlg)
{
    mp_Ui->setupUi(GetContentFrame());

    mp_Module = new DataManager::CModule;
    mp_SubModule = new DataManager::CSubModule;

    resize(550, 550);

    mp_TableWidget = new MainMenu::CBaseTable();
    mp_TableWidget->resize(380, 280);

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    mp_ExecDialog = new MainMenu::CTextDialog(this);
    mp_MessageDialog = new MainMenu::CMessageDlg(this);

    connect(mp_TableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(SelectionChanged(QModelIndex)));
    connect(mp_Ui->showDetailsBtn, SIGNAL(clicked()), this, SLOT(ExecDialog()));
    connect(mp_Ui->cancelBtn, SIGNAL(clicked()), this, SLOT(close()));
}


/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CCurrentConfigurationDlg::~CCurrentConfigurationDlg()
{
    try {
        delete mp_Ui;
        delete mp_TableWidget;
        delete mp_ExecDialog;
        delete mp_MessageDialog;
        delete mp_Module;
        delete mp_SubModule;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam SubModuleName = Name of the module
 */
/****************************************************************************/
void CCurrentConfigurationDlg::AddItem(QString SubModuleName)
{

    QList<QStandardItem *> ItemList;
    if(ItemList.count() != 0) {
        qDeleteAll(ItemList);
    }
    ItemList << new QStandardItem(SubModuleName);

    m_Model.appendRow(ItemList);

}

/****************************************************************************/
/*!
 *  \brief To get the index of selected row from the table
 *  \iparam Index = Index of the row
 */
/****************************************************************************/
void CCurrentConfigurationDlg::SelectionChanged(QModelIndex Index)
{
    m_ModuleName = Index.data((int)Qt::DisplayRole);
}

/****************************************************************************/
/*!
 *  \brief Pop's up a dialog to display submodule information
 */
/****************************************************************************/
void CCurrentConfigurationDlg::ExecDialog()
{
    if(m_ModuleName.toString().isEmpty())
    {
        mp_MessageDialog->SetTitle(tr("Select SubModule"));
        mp_MessageDialog->SetButtonText(1, tr("OK"));
        mp_MessageDialog->HideButtons();
        mp_MessageDialog->SetText(tr("Please select a SubModule.."));
        mp_MessageDialog->SetIcon(QMessageBox::Critical);
        mp_MessageDialog->show();
    }
    else
    {
        mp_SubModule = const_cast<DataManager::CModule*>(mp_Module)->GetSubModuleInfo(m_ModuleName.toString());
        int Count = mp_SubModule->GetNumberOfParameters();
        QString Text;
        DataManager::Parameter_t *Param;

        for(int i=0; i<Count; i++) {
            Param = mp_SubModule->GetParameterInfo(i);
            Text.append(Param->ParameterName + "\t\t" + Param->ParameterValue + "\n");
        }

        QString Title(mp_SubModule->GetSubModuleName());
        Title.append(tr(" Details"));

        mp_ExecDialog->SetText(Text);
        mp_ExecDialog->SetDialogTitle(Title);
        mp_ExecDialog->SetCaption("");
        mp_ExecDialog->resize(450, 450);
        mp_ExecDialog->show();
    }
}

/****************************************************************************/
/*!
 *  \brief  To initialize the pop up dialog
 *  \iparam Module = Module object
 */
/****************************************************************************/
void CCurrentConfigurationDlg::InitDialog(DataManager::CModule *p_Module)
{
    *mp_Module = *p_Module;

    QString Title(mp_Module->GetModuleName());
    Title.append(tr(" Details"));

    SetDialogTitle(Title);

    QString Text;

    QString SerialNumber = p_Module->GetSerialNumber();
    if (!SerialNumber.isEmpty())
    {
        QString SN = "Serial Number      : " + SerialNumber + "\n";
        Text.append(SN);
    }

    QString OperatingHours = p_Module->GetOperatingHours();
    if (!OperatingHours.isEmpty())
    {
        QString OH = "Operating Hours    : " + OperatingHours + "\n";
        Text.append(OH);
    }

    /*QString DateOfProduction = p_Module->GetDateOfProduction();
    if (!DateOfProduction.isEmpty())
    {
        QString DP = "Date of Production : " + DateOfProduction + "\n";
        Text.append(DP);
    }
    */

    mp_Ui->moduleInfoLabel->setText(Text);
    mp_Ui->moduleInfoLabel->setWordWrap(true);
    mp_Ui->moduleInfoLabel->setAlignment(Qt::AlignLeft);

    if(m_Model.rowCount() > 0)
    {
        m_Model.clear();
    }

    for (int i = 0; i<p_Module->GetNumberofSubModules(); i++)
    {
        mp_SubModule = p_Module->GetSubModuleInfo(i);
        QString Name = mp_SubModule->GetSubModuleName();
        AddItem(Name);
    }
}

}    // end namespace SystemTracking

