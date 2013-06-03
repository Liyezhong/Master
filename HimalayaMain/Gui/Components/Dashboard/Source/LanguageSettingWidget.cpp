#include "Dashboard/Include/LanguageSettingWidget.h"
#include "ui_LanguageSettingWidget.h"
#include "Global/Include/Utils.h"

LanguageSettingWidget::LanguageSettingWidget(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::LanguageSettingWidget),
    m_Locale("en")
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    CONNECTSIGNALSLOT(ui->btnOK, clicked(), this, OnOK());
    CONNECTSIGNALSLOT(ui->btnCancel, clicked(), this, OnCancel());
    m_btnGroup.addButton(ui->radioButtonEnglish, 0);
    QLocale Locale(Global::StringToLanguage("English"));
    ui->radioButtonEnglish->setText(Locale.nativeLanguageName());
    m_LocaleList.push_back(Locale);

    m_btnGroup.addButton(ui->radioButtonGerman, 1);
    Locale = QLocale(Global::StringToLanguage("German"));
    ui->radioButtonGerman->setText(Locale.nativeLanguageName());
    m_LocaleList.push_back(Locale);

    m_btnGroup.addButton(ui->radioButtonSpanish, 2);
    Locale = QLocale(Global::StringToLanguage("Spanish"));
    ui->radioButtonSpanish->setText(Locale.nativeLanguageName());
    m_LocaleList.push_back(Locale);

    m_btnGroup.addButton(ui->radioButtonPortuguese, 3);
    Locale = QLocale(Global::StringToLanguage("Portuguese"));
    ui->radioButtonPortuguese->setText(Locale.nativeLanguageName());
    m_LocaleList.push_back(Locale);

    m_btnGroup.addButton(ui->radioButtonJapanese, 4);
    Locale = QLocale(Global::StringToLanguage("Japanese"));
    ui->radioButtonJapanese->setText(Locale.nativeLanguageName());
    m_LocaleList.push_back(Locale);

    m_btnGroup.addButton(ui->radioButtonChinese, 5);
    Locale = QLocale(Global::StringToLanguage("Chinese"));
    ui->radioButtonChinese->setText(Locale.nativeLanguageName());
    m_LocaleList.push_back(Locale);
}

LanguageSettingWidget::~LanguageSettingWidget()
{
    delete ui;
}

void LanguageSettingWidget::OnOK()
{
    int checkedID = m_btnGroup.checkedId();
    m_Locale = m_LocaleList.at(checkedID);
    accept();
    this->close();
}

void LanguageSettingWidget::OnCancel()
{
    reject();
    this->close();
}

void LanguageSettingWidget::UpdateLanguageLsit(QStringList& languageList)
{
    QStringList languageNativeList;
    for (int i = 0; i < languageList.count(); i++) {
        QString LanguageName = languageList.at(i);
        QLocale Locale(Global::StringToLanguage(LanguageName));
        QString LangaugeNativeName = Locale.nativeLanguageName();
        if (LangaugeNativeName.isEmpty())
        {
            continue;
        }
        languageNativeList.append(LangaugeNativeName);
    }

    QAbstractButton *p_Button;
    foreach(p_Button, m_btnGroup.buttons())
    {
        p_Button->setEnabled(false);
        if (languageNativeList.contains(p_Button->text()))
        {
            p_Button->setEnabled(true);
            if (m_Locale.nativeLanguageName() == p_Button->text())
                p_Button->setChecked(true);
        }
    }
}

void LanguageSettingWidget::SetLocale(QLocale& locale)
{
    m_Locale = locale;
}

QLocale::Language LanguageSettingWidget::GetSelectedLanguage()
{
    return m_Locale.language();
}
