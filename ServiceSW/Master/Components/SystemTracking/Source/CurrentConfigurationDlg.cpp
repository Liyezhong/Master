/****************************************************************************/
/*! \file TestCase.h
 *
 *  \brief Definition file for class CTestCase.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-5-21
 *   $Author:  $ Dixiong.li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "SystemTracking/Include/CurrentConfigurationDlg.h"
#include "ui_CurrentConfigurationDlg.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Global/Include/Utils.h"

namespace SystemTracking {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CCurrentConfigurationDlg::CCurrentConfigurationDlg(QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent)
  , mp_Ui(new Ui::CCurrentConfigurationDlg)
  , mp_Module(NULL)
  , mp_SubModule(NULL)
{
    mp_Ui->setupUi(GetContentFrame());

    RetranslateUI();

    resize(550, 550);

    mp_TableWidget = new MainMenu::CBaseTable();
    mp_TableWidget->resize(500, 300);

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    mp_ParameterDlg = new SystemTracking::CCurrentConfigParameterDlg(this);
    mp_ParameterDlg->setModal(true);

    mp_MessageDialog = new MainMenu::CMessageDlg(this);
    mp_MessageDialog->setModal(true);

    mp_Ui->serialNumberEdit->setEnabled(false);
    mp_Ui->opHrsEdit->setEnabled(false);
    mp_Ui->DateOfProEdit->setEnabled(false);

    CONNECTSIGNALSLOTGUI(mp_TableWidget, clicked(QModelIndex), this, SelectionChanged(QModelIndex));
    CONNECTSIGNALSLOTGUI(mp_Ui->detailBtn, clicked(), this, ExecDialog());
    CONNECTSIGNALSLOTGUI(mp_Ui->cancelBtn, clicked(), this, close());
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
        delete mp_ParameterDlg;
        delete mp_MessageDialog;
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
    m_SubModuleName = Index.data((int)Qt::DisplayRole);
}

void CCurrentConfigurationDlg::changeEvent(QEvent *p_Event)
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
 *  \brief Pop's up a dialog to display submodule information
 */
/****************************************************************************/
void CCurrentConfigurationDlg::ExecDialog()
{
    if(m_SubModuleName.toString().isEmpty())
    {
        mp_MessageDialog->SetTitle(m_strSelectSubModuleMsgTitle);
        mp_MessageDialog->SetButtonText(1, m_strOk);
        mp_MessageDialog->HideButtons();
        mp_MessageDialog->SetText(m_strSelectSubModuleMsgText);
        mp_MessageDialog->SetIcon(QMessageBox::Critical);
        mp_MessageDialog->show();
    }
    else
    {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_CURRENTCONFIG_SUBMODULEINFO_REQUESTED,
                                                   Global::tTranslatableStringList() << m_SubModuleName.toString());
        mp_SubModule = const_cast<ServiceDataManager::CModule*>(mp_Module)->GetSubModuleInfo(m_SubModuleName.toString());
        int Count = mp_SubModule->GetNumberOfParameters();
        ServiceDataManager::Parameter_t *Param;
        mp_ParameterDlg->ClearModel();

        for(int i=0; i<Count; i++) {
            Param = mp_SubModule->GetParameterInfo(i);
            mp_ParameterDlg->InitDialog(Param);
        }

        mp_ParameterDlg->SetDialogTitle(mp_SubModule->GetSubModuleName());
        mp_ParameterDlg->resize(380, 420);
        mp_ParameterDlg->show();
    }
}

/****************************************************************************/
/*!
 *  \brief  To initialize the pop up dialog
 *  \iparam Module = Module object
 */
/****************************************************************************/
void CCurrentConfigurationDlg::InitDialog(ServiceDataManager::CModule *p_Module)
{
    mp_Module = p_Module;

    QString Title(mp_Module->GetModuleName());

    SetDialogTitle(Title);

    mp_Ui->serialNumberEdit->setText(mp_Module->GetSerialNumber());
    mp_Ui->opHrsEdit->setText(mp_Module->GetOperatingHours());
    mp_Ui->DateOfProEdit->setText(mp_Module->GetDateOfProduction());

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

void CCurrentConfigurationDlg::RetranslateUI()
{
    m_strOk                      = QApplication::translate("SystemTracking::CCurrentConfigurationDlg", "OK",
                                                            0, QApplication::UnicodeUTF8);
    m_strSelectSubModuleMsgTitle = QApplication::translate("SystemTracking::CCurrentConfigurationDlg", "Select SubModule",
                                                            0, QApplication::UnicodeUTF8);
    m_strSelectSubModuleMsgText  = QApplication::translate("SystemTracking::CCurrentConfigurationDlg", "Please select a SubModule..",
                                                            0, QApplication::UnicodeUTF8);
}

}    // end namespace SystemTracking

