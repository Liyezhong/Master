#ifndef LANGUAGESETTINGWIDGET_H
#define LANGUAGESETTINGWIDGET_H

#include <QWidget>
#include "MainMenu/Include/DialogFrame.h"
#include <QButtonGroup>
#include <QLocale>
#include <QList>

namespace Ui {
class LanguageSettingWidget;
}

class LanguageSettingWidget : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    explicit LanguageSettingWidget(QWidget *parent = 0);
    ~LanguageSettingWidget();
    void UpdateLanguageLsit(QStringList&);
    void SetLocale(QLocale&);
    QLocale::Language GetSelectedLanguage();
private:
    Ui::LanguageSettingWidget *ui;
    QButtonGroup m_btnGroup;
    QLocale m_Locale;
    QList<QLocale> m_LocaleList;
private slots:
    void OnOK();
    void OnCancel();
};

#endif // LANGUAGESETTINGWIDGET_H
