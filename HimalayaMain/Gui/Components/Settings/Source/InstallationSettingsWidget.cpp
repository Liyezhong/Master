/****************************************************************************/
/*! \file OvenSettingsWidget.cpp
 *
 *  \brief OvenSettingsWidget implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer
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


#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "Settings/Include/InstallationSettingsWidget.h"
#include "ui_InstallationSettingsWidget.h"
#include <QDebug>

namespace Settings {


/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CInstallationSettingsWidget::CInstallationSettingsWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CInstallationSettingsWidget), mp_UserSettings(NULL)
{
    mp_Ui->setupUi(GetContentFrame());
    mp_Ui->btnEdit->setText("Edit");
    mp_Ui->btnUpdate->setText("Update");
    SetPanelTitle(tr("Installation"));
    CONNECTSIGNALSLOT(mp_Ui->btnEdit,clicked(),this,OnEditclicked());
    CONNECTSIGNALSLOT(mp_Ui->btnUpdate,clicked(),this,OnUpdateclicked());
}


/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CInstallationSettingsWidget::~CInstallationSettingsWidget()
{
    try {
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
void CInstallationSettingsWidget::changeEvent(QEvent *p_Event)
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
 *  \brief Updates the widget content everytime it is displayed
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void CInstallationSettingsWidget::showEvent(QShowEvent *p_Event)
{

}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CInstallationSettingsWidget::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CInstallationSettingsWidget", "Installation", 0, QApplication::UnicodeUTF8));
   mp_Ui->btnEdit->setText("Edit");
   mp_Ui->btnUpdate->setText("Update");
}


/****************************************************************************/
/*!
 *  \brief Used to set pointer to Usersetting,
 *
 */
/****************************************************************************/

void CInstallationSettingsWidget::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;
}


/****************************************************************************/
/*!
 *  \brief Sets KeyBoard object instance
 */
/****************************************************************************/
void CInstallationSettingsWidget::SetKeyBoardInstance(KeyBoard::CKeyBoard *p_KeyBoard)
{
    mp_KeyBoardWidget = p_KeyBoard;
}


/****************************************************************************/
/*!
 *  \brief This function is called when Ok button on keyboard is clicked
 */
/****************************************************************************/
void CInstallationSettingsWidget::Update()
{
    OnOkClicked();
}


/****************************************************************************/
/*!
 *  \brief this function is called when keyboard ok clickeds
 */
/****************************************************************************/
void CInstallationSettingsWidget::OnOkClicked()
{
    QString LineEditString;
    mp_KeyBoardWidget->hide();

    LineEditString = mp_KeyBoardWidget->GetLineEditString();
    mp_Ui->instrumentname->setText(LineEditString);
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void CInstallationSettingsWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Ok button on Keyboard is pressed.
 */
/****************************************************************************/
void CInstallationSettingsWidget::UpdateOnESC()
{
    mp_KeyBoardWidget->Detach();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Update button pressed.
 */
/****************************************************************************/
void CInstallationSettingsWidget:: OnUpdateclicked()
{


}
/****************************************************************************/
/*!
 *  \brief This Slot will be called when edit button is clicked.
 */
/****************************************************************************/
void CInstallationSettingsWidget :: OnEditclicked()
{
    mp_KeyBoardWidget->Attach(this);
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Instrument Name"));

    if (mp_Ui->btnEdit->text() != "") {

        mp_KeyBoardWidget->SetLineEditContent(mp_Ui->btnEdit->text());
    }

    m_ValidationType = KeyBoard::VALIDATION_1;
    mp_KeyBoardWidget->SetValidationType(m_ValidationType);
    mp_KeyBoardWidget->SetPasswordMode(false);
    mp_KeyBoardWidget->SetMaxCharLength(32);
    mp_KeyBoardWidget->SetMinCharLength(2);
    mp_KeyBoardWidget->show();


}
} // end namespace Settings
