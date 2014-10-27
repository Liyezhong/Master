/****************************************************************************/
/*! \file DiagnosticMessageDlg.h
 *
 *  \brief Declaration of Diagnostic Message dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-29
 *   $Author:  $ Dixiong Li
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
#ifndef DIAGNOSTIC_MESSAGEDLG_H
#define DIAGNOSTIC_MESSAGEDLG_H

#include <QObject>
#include "MainMenu/Include/MessageDlg.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"

namespace Diagnostics {

/****************************************************************************/
/*!
 *  \brief This is the class of the diagnostics message dialog
 */
/****************************************************************************/
class CDiagnosticMessageDlg : public QObject
{
    Q_OBJECT

public:
    enum BUTTON_TYPE {
        YES_NO = 0,
        OK_ABORT = 1,
        NEXT_CANCEL = 2,
        NEXT_CANCEL_DISABLE = 3,

        NEXT = 1,
        CANCEL = 0,

        OK = 1,
        ABORT = 0,

        YES = 1,
        NO = 0
    };
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    CDiagnosticMessageDlg(QWidget *p_Parent=NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CDiagnosticMessageDlg(void);

    /****************************************************************************/
    /*!
     *  \brief To popup message dialog
     *  \iparam MessageTitle = the dialog title
     *  \iparam MessageText  = the dialog text
     *  \iparam Ret = test result for set dialog type
     */
    /****************************************************************************/
    virtual void ShowMessage(QString& MessageTitle, QString& MessageText, ErrorCode_t Ret);

    /****************************************************************************/
    /*!
     *  \brief To popup waiting dialog
     *  \iparam MessageTitle = the dialog title
     *  \iparam MessageText  = the dialog text
     */
    /****************************************************************************/
    virtual void ShowWaitingDialog(QString& MessageTitle, QString& MessageText);

    /****************************************************************************/
    /*!
     *  \brief To hide wait dialog
     */
    /****************************************************************************/
    virtual void HideWaitingDialog();

    virtual int ShowConfirmMessage(QString& MessageTitle, QString& MessageText, BUTTON_TYPE type = YES_NO);
    QWidget *ParentWidget()
    {
        return mp_Parent;
    }

protected:
    QWidget        *mp_Parent;              //!< Parent widget
    MainMenu::CMessageDlg *mp_WaitDlg;      //!< Wait dialog

};

}

#endif // DIAGNOSTIC_MESSAGEDLG_H
