/****************************************************************************/
/*! \file CurrentConfiguration.cpp
 *
 *  \brief Module Current Configuration implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-16
 *   $Author:  $ Soumya. D
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "SystemTracking/Include/CurrentConfiguration.h"
#include "ui_CurrentConfiguration.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

namespace SystemTracking {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CCurrentConfiguration::CCurrentConfiguration(Core::CServiceGUIConnector *p_DataConnector, QWidget *p_Parent) :
    mp_DataConnector(p_DataConnector),
    QWidget(p_Parent),
    mp_Ui(new Ui::CCurrentConfiguration)
{
    mp_Ui->setupUi(this);

    RetranslateUI();

    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(380, 380);

    mp_TableWidget->setModel(&m_Model);

    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    mp_ConfigDialog = new SystemTracking::CCurrentConfigurationDlg(this);
    mp_MessageDialog = new MainMenu::CMessageDlg(this);

    CONNECTSIGNALSLOTGUI(mp_TableWidget, clicked(QModelIndex), this, SelectionChanged(QModelIndex));
    CONNECTSIGNALSLOTGUI(mp_Ui->showDetailsBtn, clicked(), this, ExecDialog());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CCurrentConfiguration::~CCurrentConfiguration()
{
    try {
        delete mp_MessageDialog;
        delete mp_ConfigDialog;
        delete mp_TableWidget;
        delete mp_Ui;
    }
    catch (...) {

    }
}

/****************************************************************************/
/*!
 *  \brief Slot for updating the GUI
 */
/****************************************************************************/
void CCurrentConfiguration::UpdateGUI(void)
{
    mp_ModuleList = mp_DataConnector->GetModuleListContainer();
    if(mp_ModuleList)
    {
        for (int i = 0; i<mp_ModuleList->GetNumberofModules(); i++)
        {
            mp_Module = mp_ModuleList->GetModule(i);
            QString Name = mp_Module->GetModuleName();
            AddItem(Name);
        }
    }
    else
    {
        qDebug()<<"Data container Module List is NULL!!!";
    }
}

/****************************************************************************/
/*!
 *  \brief To get the index of selected row from the table
 *  \iparam Index = Index of the row
 */
/****************************************************************************/
void CCurrentConfiguration::SelectionChanged(QModelIndex Index)
{
    m_ModuleName = Index.data((int)Qt::DisplayRole);
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam ModuleName = Name of the module
 */
/****************************************************************************/
void CCurrentConfiguration::AddItem(const QString &ModuleName)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(ModuleName);

    m_Model.appendRow(ItemList);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CCurrentConfiguration::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        RetranslateUI();
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief  Pop's up a dialog for SubModuleList.
 */
/****************************************************************************/
void CCurrentConfiguration::ExecDialog(void)
{
    if(m_ModuleName.toString().isEmpty())
    {
        mp_MessageDialog->SetTitle(m_strSelectModuleTitle);
        mp_MessageDialog->SetButtonText(1, m_strOk);
        mp_MessageDialog->HideButtons();
        mp_MessageDialog->SetText(m_strSelectModuleText);
        mp_MessageDialog->SetIcon(QMessageBox::Critical);
        mp_MessageDialog->show();
    }
    else
    {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_CURRENTCONFIG_MODULEINFO_REQUESTED,
                                                   Global::tTranslatableStringList() << m_ModuleName.toString());
        if (mp_ModuleList) {
            mp_Module = const_cast<ServiceDataManager::CModule*>(mp_ModuleList->GetModule(m_ModuleName.toString()));
            mp_ConfigDialog->InitDialog(mp_Module);
            mp_ConfigDialog->show();
        }
        else
        {
            qDebug()<<"ExecDialog error :Module List is NULL!!!";
        }
    }
}

void CCurrentConfiguration::RetranslateUI()
{
     m_strOk                = QApplication::translate("SystemTracking::CCurrentConfiguration", "OK", 0, QApplication::UnicodeUTF8);
     m_strSelectModuleTitle = QApplication::translate("SystemTracking::CCurrentConfiguration", "Select Module", 0, QApplication::UnicodeUTF8);
     m_strSelectModuleText  = QApplication::translate("SystemTracking::CCurrentConfiguration", "Please select a Module..", 0, QApplication::UnicodeUTF8);
}


} // end namespace SystemTracking
