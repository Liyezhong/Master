/****************************************************************************/
/*! \file DlgWizardBase.h
 *
 *  \brief Declaration of base class of wizard dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MAINMENU_DLGWIZARDBASE_H
#define MAINMENU_DLGWIZARDBASE_H

#include "MainMenu/Include/DialogFrame.h"

namespace MainMenu {

namespace Ui {
class CDlgWizardBase;
}

/****************************************************************************/
/**
 * \brief This class provides a base class for all dialogs which requires
 *        the user to make a choice from  "Next" or "Cancel".
 *        If "Next" is selected, QDialog signal "accepted()" will emit;
 *        If "Cancel" is selected, QDialog signal "rejected()" will emit.
 */
/****************************************************************************/
class CDlgWizardBase : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    explicit CDlgWizardBase(QWidget *parent = 0, QMainWindow *pMainWindow = 0);
    ~CDlgWizardBase();

    void AddWidget(QWidget *pContent);

    void AddButton(QPushButton *pButton);

public Q_SLOTS:
    void ShowNext(void);

    void ShowCancel(void);

    void HideNext(void);

    void HideCancel(void);

    void EnableNext(void);

    void EnableCancel(void);

    void DisableNext(void);

    void DisableCancel(void);

protected:
    void changeEvent(QEvent *p_Event);

private:
    Ui::CDlgWizardBase *ui;
};


} // namespace MainMenu

#endif // MAINMENU_DLGWIZARDBASE_H
