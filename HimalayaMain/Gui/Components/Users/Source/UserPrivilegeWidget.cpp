/****************************************************************************/
/*! \file UserPrivilegeWidget.cpp
 *
 *  \brief UserPrivilegeWidget implementation.
 *         Implements change in the user level and password change for the
 *         Supervisor user
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-28, 2012-10-30
 *   $Author:  $ M.Scherer, N.Kamath, Raju
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

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "Users/Include/UserPrivilegeWidget.h"
#include "ui_UserPrivilegeWidget.h"

namespace Users {

const QString REGEXP_NUMERIC_VALIDATOR = "^[0-9]*$"; //!< Reg expression for the validator

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 *  \iparam p_KeyBoardWidget = Keyboard widget
 */
/****************************************************************************/
CUserPrivilegeWidget::CUserPrivilegeWidget(QWidget *p_Parent,
                                           KeyBoard::CKeyBoard *p_KeyBoardWidget) :
                                           MainMenu::CPanelFrame(p_Parent),
                                           mp_Ui(new Ui::CUserPrivilegeWidget),
                                           m_strEnterNewPassword(tr("Enter New Password")),
                                           m_strConfirmNewPassword(tr("Confirm New Password"))
{
    mp_Ui->setupUi(GetContentFrame());
    SetPanelTitle(tr("User"));
    mp_MainWindow = static_cast<MainMenu::CMainWindow *>(p_Parent);
    m_UserLevel = MainMenu::CMainWindow::Operator;
    m_StdUserBtnClicked = false;
    m_ServiceUserBtnClicked = false;
    m_SupervisorBtnClicked = false;
    m_ChangePasswdBtnClicked = false;
    m_PwdType = "Old";

    mp_Ui->btnAdministrator->setIcon(QIcon(":/Large/Icons/User_Status_TextButton/Btn_User_Admin.png"));
    mp_Ui->btnAdministrator->setIconSize(QSize(42, 28));
    mp_Ui->btnUser->setIcon(QIcon(":/Large/Icons/User_Status_TextButton/Btn_User_Operator.png"));
    mp_Ui->btnUser->setIconSize(QSize(42, 28));
    mp_Ui->btnService->setIcon(QIcon(":/Large/Icons/User_Status_TextButton/Btn_User_Service.png"));
    mp_Ui->btnService->setIconSize(QSize(42, 28));

    CONNECTSIGNALSLOT(mp_Ui->btnAdministrator, clicked(), this, OnBtnAdministratorClicked());
    CONNECTSIGNALSLOT(mp_Ui->btnChangePassword, clicked(), this, OnBtnChangePasswordClicked());
    CONNECTSIGNALSLOT(mp_Ui->btnService, clicked(), this, OnBtnServiceClicked());
    CONNECTSIGNALSLOT(mp_Ui->btnUser, clicked(), this, OnBtnUserClicked());
    mp_KeyBoardWidget = p_KeyBoardWidget;

    m_Timer = new QTimer(this);
    m_Timer->setInterval(600000);//10 minutes
    //m_Timer->setInterval(3000);//test
    (void)connect(m_Timer, SIGNAL(timeout()), this, SLOT(AppIdleForLongTime()));
    m_Timer->start();
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CUserPrivilegeWidget::~CUserPrivilegeWidget()
{
    try
    {
        delete mp_Ui;
        delete m_Timer;
    }
    catch(...)
    {}
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CUserPrivilegeWidget::changeEvent(QEvent *p_Event)
{
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            mp_Ui->retranslateUi(this);
            RetranslateUI();
            break;
        default:
            break;
    }
    QWidget::changeEvent(p_Event);
}

void CUserPrivilegeWidget::AppIdleForLongTime()
{
    m_Timer->stop();
    if (m_UserLevel == MainMenu::CMainWindow::Admin || m_UserLevel == MainMenu::CMainWindow::Service)
        OnBtnUserClicked();
}

void CUserPrivilegeWidget::OnInteractStart()
{
    if (m_UserLevel == MainMenu::CMainWindow::Admin || m_UserLevel == MainMenu::CMainWindow::Service)
    {
        m_Timer->stop();
        m_Timer->start();
    }
}

/****************************************************************************/
/*!
 *  \brief Switch the user role to the standard user
 */
/****************************************************************************/
void CUserPrivilegeWidget::OnBtnUserClicked()
{
    m_UserLevel = MainMenu::CMainWindow::Operator;
    QByteArray ByteArray;
    QDataStream UserLevelInfo(&ByteArray, QIODevice::ReadWrite);
    UserLevelInfo << (int)m_UserLevel << "";
    emit UserLevelClicked(UserLevelInfo);//master should know this change
}

/****************************************************************************/
/*!
 *  \brief Switch the role to the supervisor after requesting the password
 */
/****************************************************************************/
void CUserPrivilegeWidget::OnBtnAdministratorClicked()
{
    m_ValidationType = KeyBoard::VALIDATION_3;
//    mp_KeyBoardWidget->Attach(this);
    m_SupervisorBtnClicked = true;
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Password"));
//    mp_KeyBoardWidget->SetValidationType(m_ValidationType);
    mp_KeyBoardWidget->SetPasswordMode(true);
    mp_KeyBoardWidget->SetMaxCharLength(16);
    mp_KeyBoardWidget->SetMinCharLength(4);
    mp_KeyBoardWidget->show();
    m_UserLevel = MainMenu::CMainWindow::Admin;
}

/****************************************************************************/
/*!
 *  \brief Switch the role to the service user after requesting the password
 */
/****************************************************************************/
void CUserPrivilegeWidget::OnBtnServiceClicked()
{
    m_ValidationType = KeyBoard::VALIDATION_3;
//    mp_KeyBoardWidget->Attach(this);
    m_ServiceUserBtnClicked = true;
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Password"));
//    mp_KeyBoardWidget->SetValidationType(m_ValidationType);
    mp_KeyBoardWidget->SetPasswordMode(true);
    mp_KeyBoardWidget->SetMaxCharLength(4);
    mp_KeyBoardWidget->SetMinCharLength(4);
    mp_KeyBoardWidget->SetMinCharLength(4);    
    // ^ and $ is used for any character. * is used to enter multiple characters
    // [0-9] is used to allow user to enter only 0 to 9 digits
    mp_KeyBoardWidget->SetLineEditValidatorExpression(REGEXP_NUMERIC_VALIDATOR);

    mp_KeyBoardWidget->show();
    m_UserLevel = MainMenu::CMainWindow::Service;
}

/****************************************************************************/
/*!
 *  \brief Shows the on screen keyboard to change the supervisor password
 */
/****************************************************************************/
void CUserPrivilegeWidget::OnBtnChangePasswordClicked()
{
    m_ValidationType = KeyBoard::VALIDATION_3;
//    mp_KeyBoardWidget->Attach(this);
    m_ChangePasswdBtnClicked = true;
    m_PwdType = "Old";
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Password"));
//    mp_KeyBoardWidget->SetValidationType(m_ValidationType);
    mp_KeyBoardWidget->SetPasswordMode(true);
    mp_KeyBoardWidget->SetMaxCharLength(16);
    mp_KeyBoardWidget->SetMinCharLength(4);
    mp_KeyBoardWidget->show();
}
/****************************************************************************/
/*!
 *  \brief This function is called when Ok button on keyboard is clicked
 */
/****************************************************************************/
void CUserPrivilegeWidget::Update()
{
    OnOkClicked();
}

/****************************************************************************/
/*!
 *  \brief This function is called when ESC button on keyboard is clicked
 */
/****************************************************************************/
void CUserPrivilegeWidget::UpdateOnESC()
{
//   mp_KeyBoardWidget->Detach();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when OK button on Keyboard is clicked.
 */
/****************************************************************************/
void CUserPrivilegeWidget::OnOkClicked()
{
    QString LineEditString = mp_KeyBoardWidget->GetLineEditString();
    qDebug()<<"Password entered is "<< LineEditString;
    mp_KeyBoardWidget->hide();

    if (m_StdUserBtnClicked) {
        m_StdUserBtnClicked = false;
        m_ChangePasswdBtnClicked = false;
        m_PwdType = "Old";
        qDebug("Std User");
    }
    else if (m_SupervisorBtnClicked){
        m_SupervisorBtnClicked = false;
        m_ChangePasswdBtnClicked = false;
        m_PwdType = "Old";
        qDebug("Supervisor");
    }
    else if (m_ServiceUserBtnClicked){
        m_ServiceUserBtnClicked = false;
        m_ChangePasswdBtnClicked = false;
        m_PwdType = "Old";
        qDebug("Service");
    }
    else if (m_ChangePasswdBtnClicked) {
        /// don't reset the change password button flag
        // this resets the password
        qDebug("Change Passwd");
    }
    else {
        return;
    }

    //Test code starts here
    if (LineEditString == "w") {
        (void) mp_MainWindow->SetStatusIcons(MainMenu::CMainWindow::Warning);
    }
    else if (LineEditString == "e") {
        (void) mp_MainWindow->SetStatusIcons(MainMenu::CMainWindow::Error);
    }
    else if (LineEditString == "proc") {
        (void) mp_MainWindow->SetStatusIcons(MainMenu::CMainWindow::ProcessRunning);
    }
    else if (LineEditString == "r") {
        (void) mp_MainWindow->SetStatusIcons(MainMenu::CMainWindow::RemoteCare);
    }
    else if (LineEditString == "uproc") {
        (void) mp_MainWindow->UnsetStatusIcons(MainMenu::CMainWindow::ProcessRunning);
    }
    else if (LineEditString == "a") {
        (void) mp_MainWindow->UnsetStatusIcons(MainMenu::CMainWindow::RemoteCare);
    }
    else if (LineEditString == "s") {
        (void) mp_MainWindow->UnsetStatusIcons(MainMenu::CMainWindow::Warning);
    }
    else if (LineEditString == "d") {
        (void) mp_MainWindow->UnsetStatusIcons(MainMenu::CMainWindow::Error);
    }
    //Test code ends
//    //TODO: From here i can infrom n/w manager that password is for
//    //user/service/supervisor mode
//    switch (m_UserLevel) {
//    case MainMenu::CMainWindow::Operator:
//        mp_MainWindow->SetUserIcon(MainMenu::CMainWindow::Operator);
//        break;
//    case MainMenu::CMainWindow::Admin:
//        mp_MainWindow->SetUserIcon(MainMenu::CMainWindow::Admin);
//        break;
//    case MainMenu::CMainWindow::Service:
//        mp_MainWindow->SetUserIcon(MainMenu::CMainWindow::Service);
//        break;
//    default:
//        break;
//    }

    // check the length of the string
    if (LineEditString.length() > 0) {
        QByteArray ByteArray;
        if (!m_ChangePasswdBtnClicked) {
            QDataStream UserLevelInfo(&ByteArray, QIODevice::ReadWrite);
            UserLevelInfo << (int)m_UserLevel << LineEditString;
            emit UserLevelClicked(UserLevelInfo);
        }
        else {
            QDataStream PasswordInfo(&ByteArray, QIODevice::ReadWrite);
            PasswordInfo << m_PwdType << LineEditString;
            emit ChangePasswordClicked(PasswordInfo);
        }
    }

//    mp_KeyBoardWidget->Detach();
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CUserPrivilegeWidget::RetranslateUI()
{
      MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Users::CUserPrivilegeWidget", "User", 0, QApplication::UnicodeUTF8));
      mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("Users::CUserPrivilegeWidget", "Enter Password", 0, QApplication::UnicodeUTF8));
      m_strEnterNewPassword = QApplication::translate("Users::CUserPrivilegeWidget", "Enter New Password", 0, QApplication::UnicodeUTF8);
      m_strConfirmNewPassword = QApplication::translate("Users::CUserPrivilegeWidget", "Confirm New Password", 0, QApplication::UnicodeUTF8);
}


/****************************************************************************/
/*!
 *  \brief Checks the whether user authentication is done or not
 *
 *  \param AuthenticatedLevel    Authenticated user level
 */
/****************************************************************************/
void CUserPrivilegeWidget::UserAuthenticated(const qint32 &AuthenticatedLevel)
{
    m_UserLevel = (MainMenu::CMainWindow::UserRole_t)AuthenticatedLevel;
    //m_UserLevel
    mp_MainWindow->SetUserRole(m_UserLevel);
    //user/service/supervisor mode
    switch (m_UserLevel) {
    case MainMenu::CMainWindow::Operator:
        mp_MainWindow->SetUserIcon(MainMenu::CMainWindow::Operator);
        break;
    case MainMenu::CMainWindow::Admin:
        mp_MainWindow->SetUserIcon(MainMenu::CMainWindow::Admin);
        break;
    case MainMenu::CMainWindow::Service:
        mp_MainWindow->SetUserIcon(MainMenu::CMainWindow::Service);
        /// \todo no information about the service
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief Changes the admin password step by step
 *
 *  \param PasswordType     Gives the password type
 */
/****************************************************************************/
void CUserPrivilegeWidget::ChangeInAdminPassword(const QString &PasswordType)
{
    bool InitKeyboard = true;
    QString KeyboardTitleText;
    // check the password type
    if (PasswordType.compare("New") == 0) {
        m_PwdType = "New";
        KeyboardTitleText = m_strEnterNewPassword;
        // make it true so that still we are in change password
        m_ChangePasswdBtnClicked = true;
    }
    else if(PasswordType.compare("Confirm") == 0) {
        m_PwdType = "Confirm";
        KeyboardTitleText = m_strConfirmNewPassword;
        // make it true so that still we are in change password
        m_ChangePasswdBtnClicked = true;
    }
    else {
        if (PasswordType.compare("Failed") == 0) {
            // if the authentication fails then change the user level to operator
            m_UserLevel = MainMenu::CMainWindow::Operator;
            mp_MainWindow->SetUserIcon(MainMenu::CMainWindow::Operator);
        }
        // reset the falgs
        m_PwdType = "Old";
        // make it false , there is some problem in the change password
        m_ChangePasswdBtnClicked = false;
        InitKeyboard = false;
    }
    // initialize the keyboard
    if (InitKeyboard) {
        m_ValidationType = KeyBoard::VALIDATION_3;
//        mp_KeyBoardWidget->Attach(this);
        mp_KeyBoardWidget->SetKeyBoardDialogTitle(KeyboardTitleText);
//        mp_KeyBoardWidget->SetValidationType(m_ValidationType);
        mp_KeyBoardWidget->SetPasswordMode(true);
        mp_KeyBoardWidget->SetMaxCharLength(16);
        mp_KeyBoardWidget->SetMinCharLength(4);
        mp_KeyBoardWidget->show();
    }
}

} // end namespace Users
