/****************************************************************************/
/*! \file WaitDialog.h
 *
 *  \brief Header file for class CStatusConfirmDialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-21
 *   $Author:  $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef STATUSCONFIRMDIALOG_H
#define STATUSCONFIRMDIALOG_H

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <QTimer>

namespace DiagnosticsManufacturing {

namespace Ui {
    class CStatusConfirmDialog;
}

/****************************************************************************/
/**
 * \brief This dialog is started, when the user has to wait for the result of
 *        a background process.
 */
/****************************************************************************/
class CStatusConfirmDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT
    friend class  CTestMainMenu;

public:
    explicit CStatusConfirmDialog(QWidget *p_Parent = 0);
    virtual ~CStatusConfirmDialog();
    void SetText(QString Text);
    void UpdateLabel(const Service::ModuleTestStatus &Status);
    void SetTimeout(qint32 MilliSeconds);
    void HideAbort(bool HideFlag=true);
    void show();
    void BlgProcessProgress(bool IsBlgProcessStarted);
public slots:
    void done(int Result);

protected:
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private:
    Ui::CStatusConfirmDialog *mp_Ui;     //!< User interface
    bool m_AbortButtonVisible;  //!< Is the abort button visible?
    bool m_DialogLock;          //!< Prevents closing the dialog
    bool m_DialogHide;          //!< Should the dialog be closed?
    QTimer m_Timer;             //!< Timeout timer
    bool m_IsBlgProcessStarted; //!< Bathlayout process indicator flag
    /****************************************************************************/
    /*!
     *  \brief Disable copy and assignment operator.
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CStatusConfirmDialog)

private slots:
    void LockDialog();
    void UnlockDialog();
    void AbortWaitDialog();

signals:
    /****************************************************************************/
    /**
     * \brief This signal is emitted when the timer time out happens.
     */
    /****************************************************************************/
    void Timeout();
    /****************************************************************************/
    /**
     * \brief This signal is emitted when the Abort button is pressed.
     * \iparam IsBLGProcessAborted = BLG process aborted/cancelled indicator flag
     *
     */
    /****************************************************************************/
    void AbortBlgProcess(bool IsBLGProcessAborted);

};

} // end namespace DiagnosticsManufacturing

#endif // STATUSCONFIRMDIALOG_H
