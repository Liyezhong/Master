/****************************************************************************/
/*! \file System.cpp
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-07-16
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "ServiceUpdates/Include/System.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "Global/Include/Utils.h"

#include "ui_ServiceSystem.h"

namespace ServiceUpdates {

CSystem::CSystem(QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_Ui(new Ui::CServiceSystem)
    , mp_MessageDlg(new MainMenu::CMessageDlg(p_Parent))
{
    mp_Ui->setupUi(this);

    mp_MessageDlg->SetButtonText(1, QApplication::translate("ServiceUpdates::CSystem", "Ok", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->setModal(true);
    mp_MessageDlg->HideButtons();

    CONNECTSIGNALSLOT(mp_Ui->shutDownBtn, clicked(), this, OnShutDown());
}

CSystem::~CSystem(void)
{
    try
    {
        delete mp_MessageDlg;

        delete mp_Ui;
    }
    catch (...) { }

}

void CSystem::changeEvent(QEvent *p_Event)
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

void CSystem::OnShutDown(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_SYSTEM_SHUT_DOWN);
    mp_MessageDlg->SetText(QApplication::translate("ServiceUpdates::CSystem", "System is shutting down ...\n", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->HideAllButtons();
    mp_MessageDlg->Show();

    emit ShutdownSystem(true);
}

void CSystem::RetranslateUI()
{
    mp_MessageDlg->SetButtonText(1, QApplication::translate("ServiceUpdates::CSystem", "Ok", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetText(QApplication::translate("ServiceUpdates::CSystem", "System is shutting down ...\n", 0, QApplication::UnicodeUTF8));
}

} // end namespace ServiceUpdates

