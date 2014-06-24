/****************************************************************************/
/*! \file BasicColorTestDlg.h
 *
 *  \brief  BasicColorTestDlg class definition
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-04-18
 *   $Author:  $ Soumya. D
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

#ifndef MAINMENU_BASICCOLORTESTDLG_H
#define MAINMENU_BASICCOLORTESTDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/MessageDlg.h"
#include <QEvent>

namespace Diagnostics {

namespace Ui {
class CBasicColorTestDlg;
}

/****************************************************************************/
/**
 * \brief This class implements dialog for basic color test
 */
/****************************************************************************/
class CBasicColorTestDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    explicit CBasicColorTestDlg(QWidget *p_Parent = NULL);
    ~CBasicColorTestDlg();

protected:
    void changeEvent(QEvent *p_Event);

private:
    Ui::CBasicColorTestDlg *mp_Ui;              //!< User Interface
    MainMenu::CMessageDlg *mp_MessageDlg;       //!< Information dialog
    int m_ColorCount;                           //!< Stores color count value  

private slots:
    void RetranslateUI();

public slots:
    void BasicColorTest();
    void TestCancelButton();

};

}   // end of namespace Diagnostics

#endif // MAINMENU_BASICCOLORTESTDLG_H
