/****************************************************************************/
/*! \file GlobalHelper.cpp
 *
 *  \brief GlobalHelper implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-01-27
 *   $Author:  $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */

/****************************************************************************/
#include <qglobal.h>
#include <QApplication>
#include "Core/Include/GlobalHelper.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Core/Include/DataConnector.h"

namespace Core {


QString CGlobalHelper::m_selectedProgramId = "";
QString CGlobalHelper::m_strMsgUnselect = "";
QString CGlobalHelper::m_strWarning = "";
QString CGlobalHelper::m_strYes = "";
QString CGlobalHelper::m_strCancel = "";
QString CGlobalHelper::SELECTED_PROGRAM_NAME = "";

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CGlobalHelper::CGlobalHelper(Core::CDataConnector *p_DataConnector) : QObject(),
    mp_DataConnector(p_DataConnector)
{

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CGlobalHelper::~CGlobalHelper()
{
    try {
    }
    catch (...) {}
}

bool CGlobalHelper::CheckSelectedProgram(bool& bRevertSelectProgram,
                          QString operatedProgramID)
{
    RetranslateUI();  // init string

    bRevertSelectProgram = false;
    if (!m_selectedProgramId.isEmpty())
    {
        if (!operatedProgramID.isEmpty())//not empty
        {
            if (operatedProgramID != m_selectedProgramId)
                return true;
        }

        MainMenu::CMessageDlg ConfirmationMessageDlg;
        ConfirmationMessageDlg.SetTitle(m_strWarning);
        QString strMsg;
        strMsg = m_strMsgUnselect.arg(SELECTED_PROGRAM_NAME);
        ConfirmationMessageDlg.SetText(strMsg);
        ConfirmationMessageDlg.SetIcon(QMessageBox::Warning);
        ConfirmationMessageDlg.SetButtonText(1, m_strYes);
        ConfirmationMessageDlg.SetButtonText(3, m_strCancel);
        ConfirmationMessageDlg.HideCenterButton();
        if (!ConfirmationMessageDlg.exec())
            return false;//cancel
        else
            bRevertSelectProgram = true;

    }
    return true;
}


QString  CGlobalHelper::TimeToString(int seconds, bool showSeconds)
{
    QString retStr;

    int hour = seconds/(60*60);
    int minute = (seconds/60)%60;



    if (hour < 100)
        retStr.sprintf("%02d:%02d", hour, minute);
    else
        retStr.sprintf("%d:%02d", hour, minute);

    if (showSeconds)
    {
        int s = seconds%60;
        if ( s < 10)
            retStr.append(":0");
        else
            retStr.append(":");
        retStr.append(QString::number(s));
    }

    return retStr;
}

void CGlobalHelper::OnProgramSelected(QString& programId, QList<QString>& stationList)
{
    Q_UNUSED(stationList);
    m_selectedProgramId = programId;

    const DataManager::CProgram* pProgram = mp_DataConnector->ProgramList->GetProgram(m_selectedProgramId);
    if (pProgram)
    {
        SELECTED_PROGRAM_NAME = pProgram->GetName();
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CGlobalHelper::RetranslateUI()
{
    m_strWarning = QApplication::translate("Core::CGlobalHelper", "Warning", 0, QApplication::UnicodeUTF8);
    m_strMsgUnselect = QApplication::translate("Core::CGlobalHelper", "As the program \"%1\" is selected, this operation will result in an incorrect program result, if you click \"Yes\", the selected program will unselect.", 0, QApplication::UnicodeUTF8);
    m_strYes = QApplication::translate("Core::CGlobalHelper", "Yes", 0, QApplication::UnicodeUTF8);
    m_strCancel = QApplication::translate("Core::CGlobalHelper", "Cancel", 0, QApplication::UnicodeUTF8);
}

} // end namespace Core
