/****************************************************************************/
/*! \file LanguageSetting.cpp
 *
 *  \brief Implementation file for class CLanguageSetting.
 *
 *  \b Description:
 *          This class implements the widget to display list of languages in the table
 *          and provides user interface to change the language.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer, C.Adaragunchi
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

#include "ServiceUpdates/Include/LanguageSetting.h"

#include <QLocale>

#include "Global/Include/Utils.h"

#include "ui_LanguageSetting.h"

namespace ServiceUpdates {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CLanguageSetting::CLanguageSetting(QWidget *p_Parent)
    : MainMenu::CPanelFrame(p_Parent)
    , mp_Ui(new Ui::CLanguageSetting)
    , mp_UserSettings(0)
    , mp_TableWidget(new MainMenu::CBaseTable)
    , mp_LanguageModel(new CLanguageModel)
{
    mp_Ui->setupUi(GetContentFrame());

    SetPanelTitle(tr("Language"));

    mp_LanguageModel->SetVisibleRowCount(6);

    mp_TableWidget->setModel(mp_LanguageModel);

    mp_Ui->tableWidget->SetContent(mp_TableWidget);

    PopulateLanguageList();

    CONNECTSIGNALSLOT(mp_TableWidget,
                      clicked(QModelIndex),
                      this,
                      SelectionChanged(QModelIndex));

    CONNECTSIGNALSLOT(mp_Ui->btnApply,
                      clicked(),
                      this,
                      OnBtnApplyClicked());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CLanguageSetting::~CLanguageSetting()
{
    try
    {
        delete mp_LanguageModel;

        delete mp_TableWidget;

        delete mp_Ui;
    }
    catch (...) { }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CLanguageSetting::changeEvent(QEvent *p_Event)
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
 *  \brief Fills the language selection table
 */
/****************************************************************************/
void CLanguageSetting::PopulateLanguageList()
{
    mp_TableWidget->horizontalHeader()->show();
    mp_TableWidget->SetVisibleRows(6);
    mp_LanguageModel->SetVisibleRowCount(6);
    mp_TableWidget->horizontalHeader()->resizeSection(0, 300);
}

/****************************************************************************/
/*!
 *  \brief Sets the settings data object for this class
 *
 *  \iparam p_UserSettings = Global user settings data
 */
/****************************************************************************/
void CLanguageSetting::SetUserSettings(
        DataManager::CUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;

    InitDialog();
}

/****************************************************************************/
/*!
 *  \brief Sets the LangauageList to the Langauge table.
 *
 *  \iparam LanguageList = List of Languages.
 */
/****************************************************************************/
void CLanguageSetting::SetLanguages(QStringList LanguageList)
{
    QStringList LanguageNativeNames;
    QStringList LanguageNames;

    for (int Index = 0; Index < LanguageList.count(); ++Index)
    {
        QString LanguageName = LanguageList.at(Index);

        QLocale Language = Global::StringToLanguage(LanguageName);

        if (QLocale::C == Language.language())
        {
            continue;
        }

        LanguageName = QLocale::languageToString(Language.language());
        QString LangaugeNativeName = Language.nativeLanguageName();

        if (LangaugeNativeName.isEmpty())
        {
            LangaugeNativeName = LanguageName;
        }

       LanguageNativeNames.append(QString("   " + LangaugeNativeName));
       LanguageNames.append(LanguageName);
    }

    m_LanguageNativeNameList = LanguageNativeNames;
    m_LanguageList           = LanguageNames;

    mp_LanguageModel->SetLanguageList(m_LanguageNativeNameList);

    InitDialog();
}

/****************************************************************************/
/*!
 *  \brief Called when the apply button is clicked
 */
/****************************************************************************/
void CLanguageSetting::OnBtnApplyClicked(void)
{
    if (mp_UserSettings)
    {
        mp_UserSettings->SetLanguage(Global::StringToLanguage(m_SelectedLanguage));

        emit UserSettingsChanged(*mp_UserSettings);
    }
}

/****************************************************************************/
/*!
 *  \brief Called when the selection of the table is changed
 *
 *  \iparam Index = Currently selected cell
 */
/****************************************************************************/
void CLanguageSetting::SelectionChanged(QModelIndex Index)
{
    int SelectedIndex = Index.row();

    if ((-1 < SelectedIndex) && (SelectedIndex < m_LanguageList.count()))
    {
        m_SelectedLanguage = m_LanguageList.at(SelectedIndex);
    }
}

/****************************************************************************/
/*!
 *  \brief Initialises the dialog with the language list
 */
/****************************************************************************/
void CLanguageSetting::InitDialog(void)
{
    if (m_LanguageNativeNameList.isEmpty())
    {
        mp_Ui->btnApply->setEnabled(false);
    }
    else
    {
        mp_Ui->btnApply->setEnabled(true);
    }

    if (0 != mp_UserSettings)
    {
        m_SelectedLanguage =
                Global::LanguageToString(mp_UserSettings->GetLanguage());
    }

    // get the index value
    qint32 IndexValue = m_LanguageList.indexOf(m_SelectedLanguage);
    if (IndexValue != -1)
    {
        mp_TableWidget->selectRow(IndexValue);
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CLanguageSetting::RetranslateUI(void)
{
    SetPanelTitle(tr("Language"));

    mp_LanguageModel->setHeaderData(0,Qt::Horizontal,tr("Language"),0);
}

} // end namespace ServiceUpdates
