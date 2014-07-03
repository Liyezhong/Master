/****************************************************************************/
/*! \file HeatingTestDialog.h
 *
 *  \brief Header file for class CHeatingTestDialog.
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

#ifndef HEATINGTESTDIALOG_H
#define HEATINGTESTDIALOG_H

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <QTimer>

namespace DiagnosticsManufacturing {

namespace Ui {
    class CHeatingTestDialog;
}

/****************************************************************************/
/**
 * \brief This dialog is started, when the user has to wait for the result of
 *        a background process.
 */
/****************************************************************************/
class CHeatingTestDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT
    friend class  CTestMainMenu;

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     * \iparam TestCaseId = Test case id
     *
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CHeatingTestDialog(Service::ModuleTestCaseID TestCaseId, QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CHeatingTestDialog();

    /****************************************************************************/
    /*!
     *  \brief Sets the text displayed in the wait dialog
     *
     *  \iparam Text = Label text
     */
    /****************************************************************************/
    void SetText(QString Text);


    /****************************************************************************/
    /*!
     *  \brief Update label status
     *
     *  \iparam Status = Label test status
     */
    /****************************************************************************/
    void UpdateLabel(const Service::ModuleTestStatus &Status);

    /****************************************************************************/
    /*!
     *  \brief Update label status
     */
    /****************************************************************************/
    void DisplayLSensorLabel(void);

    /****************************************************************************/
    /*!
     *  \brief Activates a timeout timer
     *
     *  \iparam Milliseconds = Timeout in milliseconds
     */
    /****************************************************************************/
    void SetTimeout(qint32 MilliSeconds);

    /****************************************************************************/
    /*!
     *  \brief Hides the abort button of the dialog
     */
    /****************************************************************************/
    void HideAbort(bool HideFlag=false);

    /****************************************************************************/
    /*!
     *  \brief Overrides the show function of QDialog
     *
     *      If the abort button of this message box is enabled, this method will
     *      immediately show the dialog. If this is not the case, the dialog will
     *      be shown after a time of 500 ms.
     */
    /****************************************************************************/
    void Show();

    /****************************************************************************/
    /*!
     *  \brief Abort Bathlayout generating process
     */
    /****************************************************************************/
    void BlgProcessProgress(bool IsBlgProcessStarted);

public slots:
    /****************************************************************************/
    /*!
     *  \brief Overrides the done function of QDialog
     *
     *      Only If the message box is not locked, the dialog will be closed
     *      immediately.
     *
     *  \iparam Result = Dialog result code
     */
    /****************************************************************************/
    void done(int Result);

protected:
    /****************************************************************************/
    /*!
     *  \brief Filters all mouse events
     *
     *      There can be a small period of time in which the wait dialog is not
     *      displayed. This is required so that the wait dialog does not have to
     *      be displayed everytime network communication takes place.
     *
     *  \iparam p_Object = Object that is watched
     *  \iparam p_Event = Current event
     *
     *  \return True if an event should be filtered
     */
    /****************************************************************************/
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private:
    Ui::CHeatingTestDialog *mp_Ui;     //!< User interface
    bool m_AbortButtonVisible;  //!< Is the abort button visible?
    bool m_DialogLock;          //!< Prevents closing the dialog
    bool m_DialogHide;          //!< Should the dialog be closed?
    QTimer m_Timer;             //!< Timeout timer
    bool m_IsBlgProcessStarted; //!< Bathlayout process indicator flag
    Service::ModuleTestCaseID m_TestCaseId;  //!< Test case id
    /****************************************************************************/
    /*!
     *  \brief Disable copy and assignment operator.
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CHeatingTestDialog)

private slots:
    /****************************************************************************/
    /*!
     *  \brief Blocks the dialog for closing
     *
     *      This function helps to assure that the dialog is displayed for at
     *      least 500 ms. This needed avoid the flickering of the dialog, when
     *      it is displayed only for a short period of time.
     */
    /****************************************************************************/
    void LockDialog();

    /****************************************************************************/
    /*!
     *  \brief Unblocks the dialog for closing
     */
    /****************************************************************************/
    void UnlockDialog();

    /****************************************************************************/
    /*!
     *  \brief Abort wait dialog
     */
    /****************************************************************************/
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

    /****************************************************************************/
    /**
       * \brief Signal emitted to perform manufacturing tests
       * \iparam Test = Test name
       * \iparam AbortTestCaseId = Abort test case id
       */
    /****************************************************************************/
    void PerformManufacturingTest(Service::ModuleTestCaseID Test, Service::ModuleTestCaseID AbortTestCaseId);
};

} // end namespace DiagnosticsManufacturing

#endif // HEATINGTESTDIALOG_H
