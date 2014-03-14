/****************************************************************************/
/*! \file DlgWaitingBase.h
 *
 *  \brief Declaration of base class of waiting dialog.
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

#ifndef MAINMENU_DLGWAITINGBASE_H
#define MAINMENU_DLGWAITINGBASE_H

#include "MainMenu/Include/DialogFrame.h"
#include <QTimer>

namespace MainMenu {

namespace Ui {
    class CDlgWaitingBase;
}

/****************************************************************************/
/**
 * \brief This dialog is started, when the user has to wait for the result of
 *        a background process.
 */
/****************************************************************************/
class CDlgWaitingBase : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    explicit CDlgWaitingBase(QWidget *p_Parent = 0);
    virtual ~CDlgWaitingBase();

    void AddWidget(QWidget *pContent);

public Q_SLOTS:
    void SetText(const QString &Text);

    void SetTimeout(int MilliSeconds);

    void SetAbortLabel(const QString &Label);

    void ShowAbort(void);

    void HideAbort(void);

    void EnableAbort(void);

    void DisableAbort(void);

protected:
    void changeEvent(QEvent *p_Event);

private:
    Ui::CDlgWaitingBase *ui;     //!< User interface
    QTimer *m_Timer;             //!< Timeout timer

signals:
    void Timeout();
};

} // end namespace MainMenu

#endif // MAINMENU_DLGWAITINGBASE_H
