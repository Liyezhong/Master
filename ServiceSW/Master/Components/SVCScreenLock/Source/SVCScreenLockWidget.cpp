#include "SVCScreenLock/Include/SVCScreenLockWidget.h"
#include <PasswordManager/Include/ServicePassword.h>
#include "ui_SVCScreenLockWidget.h"
#include "Core/Include/ServiceUtils.h"
#include "Diagnostics/Include/TestBase.h"
#include <QTimer>
#include <QDesktopWidget>

const QString REGEXP_NUMERIC_VALIDATOR = "^[0-9]{1,4}$"; //!< Reg expression for the validator

namespace SVCScreenLock {

CSVCScreenLockWidget::CSVCScreenLockWidget(QWidget *p_Parent) :
    //QWidget(p_Parent),
    ui(new Ui::CSVCScreenLockWidget),
    mp_KeyBoardWidget(NULL),
    m_LockStatus(true)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->move(0, 0);

    QFont Font;
    //Font.setWeight((int)QFont::Black);
    Font.setPointSize(15);
    ui->labelText->setFont(Font);

    mp_MessageDlg = new MainMenu::CMessageDlg(p_Parent);
    mp_MessageDlg->setModal(true);
    mp_KeyBoardWidget = new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD, NULL, this);
    m_timer = new QTimer(this);
    m_timer->setInterval(1200000);//20 mintues
    //m_timer->setInterval(30000);//0.5 mins for test
    (void)connect(m_timer, SIGNAL(timeout()), this, SLOT(AppIdleForLongTime()));
    //m_timer->start();
}

CSVCScreenLockWidget::~CSVCScreenLockWidget()
{
    try
    {
        delete ui;
        delete m_timer;
        delete mp_MessageDlg;
        delete mp_KeyBoardWidget;
    }
    catch(...)
    {}
}

void CSVCScreenLockWidget::StartTimer()
{
    m_timer->start();
}

void CSVCScreenLockWidget::AppIdleForLongTime()
{
    m_timer->stop();
    qDebug()<<"test case flag :"<<Diagnostics::CTestBase::IsTestRuning;
    if (Diagnostics::CTestBase::IsTestRuning) {
        m_timer->start();
        return;
    }

    if (m_LockStatus) {        
        ui->labelText->setText(QString("<font color='Red'>%1</font>").arg("Instrument was left in a critical state and must be switched off immediately.<br>"
                                                                          "Do not open the retort and do not remove any reagent bottle or wax bath <br>"
                                                                          "before switching off. Non-compliance might harm your health!<br>"
                                                                          "Leave instrument switched off for at least one minute, switch back on and <br>"
                                                                          "wait for reboot. Then carefully open the retort. If necessary call Service <br>"
                                                                          "to resolve a potential issue."));
    }
    else {
        ui->labelText->setText("Instrument was left in service software.<br>"
                               "Touch the screen to reenter the service software or switch the instrument off.");
    }

    this->show();
}

void CSVCScreenLockWidget::OnInteractStart()
{
    if (m_LockStatus) {
        return;
    }

    m_timer->stop();
    m_timer->start();

    if (this->isVisible()) {
        this->hide();
        DisplayKeyBoard();
    }
}

void CSVCScreenLockWidget::ConnectKeyBoardSignalSlots()
{
    // Connect signals and slots to keyboard.
    CONNECTSIGNALSLOT(mp_KeyBoardWidget, OkButtonClicked(QString), this, OnOkClicked(QString));
    CONNECTSIGNALSLOT(mp_KeyBoardWidget, EscButtonClicked(), this, OnESCClicked());
}

void CSVCScreenLockWidget::DisconnectKeyBoardSignalSlots()
{
    // Disconnect signals and slots connected to keyboard.
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(OkButtonClicked(QString)), this, SLOT(OnOkClicked(QString)));
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(EscButtonClicked()), this, SLOT(OnESCClicked()));
}

void CSVCScreenLockWidget::OnOkClicked(QString EnteredString)
{
    m_EnteredPin = EnteredString;

    PasswordManager::CServicePassword m_ServicePassword(m_EnteredPin, "HISTOCORE PEARL");

#if 0
    /* Code for debug purposes only. This section needs to be removed for release*/
    if(EnteredString.startsWith("1406"))
    {
        HideKeyBoard();
        return;
    }
#endif

    QString MessageTitle, MessageInfo;
    /* Service Key authentication */
    if (m_ServicePassword.ValidateAuthentication()) {
        /*
        UserMode =  m_ServicePassword.ReadServiceID();
        if(UserMode.startsWith("service")) {
            //emit SetSoftwareMode(PlatformService::MANUFACTURING_MODE, UserMode);
        }
        else {
            //emit SetSoftwareMode(PlatformService::SERVICE_MODE, UserMode);
        }
        */
        HideKeyBoard();
    }
    else {
        PasswordManager::ServiceUserAuthentication AuthenticationError = m_ServicePassword.GetAuthenticationError();

        switch (AuthenticationError) {
        case PasswordManager::NO_SERVICE_KEY_FILE:
            //Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_NO_KEY_FILE);
            MessageTitle = QString("Service Key File");
            MessageInfo = QString("Service Key file is not present, Cannot boot into Service/Manufacturing Software.");
            break;
        case PasswordManager::NO_DEVICE_TAG:
            //Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_DEVICE_NAME_NOT_FOUND);
            MessageTitle = QString("Device Name");
            MessageInfo = QString("Device name is not present, Cannot boot into Service/Manufacturing Software.");
            break;
        case PasswordManager::BASIC_TAG_NOT_MATCHING:
            //Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_BASIC_TAG_NOT_MATCHING);
            MessageTitle = QString("Invalid Tag");
            MessageInfo = QString("Service user is not trained for the device.");
            break;
        case PasswordManager::DATE_EXPIRED:
            //Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_KEY_DATE_EXPIRED);
            MessageTitle = QString("Service Key Expired");
            MessageInfo = QString("Date expired, Cannot boot into Service/Manufacturing Software.");
            break;
        default:
            //Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_INVALID_PIN);
            MessageTitle = QString("Invalid Pin");
            MessageInfo = QString("Invalid Pin, Cannot boot into Service/Manufacturing Software.");
            break;
        }

        HideKeyBoard();
        mp_MessageDlg->SetTitle(MessageTitle);
        mp_MessageDlg->SetText(MessageInfo);
        mp_MessageDlg->SetButtonText(1, QApplication::translate("CSVCScreenLock::CSVCScreenLockWidget", "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
        (void)mp_MessageDlg->exec();
        QTimer::singleShot(100, this, SLOT(DisplayKeyBoard()));
    }
}

void CSVCScreenLockWidget::DisplayKeyBoard()
{
    mp_KeyBoardWidget->setModal(true);
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("CSVCScreenLock::CSVCScreenLockWidget", "Enter Pin", 0, QApplication::UnicodeUTF8));
    mp_KeyBoardWidget->SetPasswordMode(true);
    mp_KeyBoardWidget->SetValidation(true);
    mp_KeyBoardWidget->SetMinCharLength(4);
    mp_KeyBoardWidget->SetMaxCharLength(4);
    mp_KeyBoardWidget->DisplayNumericKeyBoard();
    mp_KeyBoardWidget->SetLineEditValidatorExpression(REGEXP_NUMERIC_VALIDATOR);

    ConnectKeyBoardSignalSlots();
    mp_KeyBoardWidget->show();
}

void CSVCScreenLockWidget::HideKeyBoard()
{
    if (mp_KeyBoardWidget) {
        DisconnectKeyBoardSignalSlots();
        mp_KeyBoardWidget->HideKeyBoard();
    }
}

void CSVCScreenLockWidget::OnESCClicked()
{
    DisplayKeyBoard();
}

} //end of namespace SVCScreenLock
