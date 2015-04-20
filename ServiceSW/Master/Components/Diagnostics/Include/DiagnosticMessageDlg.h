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
    /****************************************************************************/
    /**
     * \brief Enum Button type Mode.
     */
    /****************************************************************************/
    enum BUTTON_TYPE {
        YES_NO = 0,     //!< YES and No
        OK_ABORT = 1,   //!< OK and Abort
        NEXT_CANCEL = 2,    //!< Next and Cancel
        NEXT_CANCEL_DISABLE = 3,    //!< Next disable
        YES_CANCEL = 4,  //!< YES and Cancel

        NEXT = 1,       //!< Next
        CANCEL = 0,     //!< Cancel

        OK = 1,         //!< Ok
        ABORT = 0,      //!< Abort

        YES = 1,        //!< Yes
        NO = 0          //!< No
    };

    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = Parent widget
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
    virtual void ShowMessage(const QString& MessageTitle, const QString& MessageText, ErrorCode_t Ret);

    /****************************************************************************/
    /*!
     *  \brief To popup waiting dialog
     *  \iparam MessageTitle = the dialog title
     *  \iparam MessageText  = the dialog text
     */
    /****************************************************************************/
    virtual void ShowWaitingDialog(const QString& MessageTitle, const QString& MessageText);

    /****************************************************************************/
    /*!
     *  \brief To hide wait dialog
     */
    /****************************************************************************/
    virtual void HideWaitingDialog();

    /****************************************************************************/
    /*!
     *  \brief To Show confirm message
     *  \iparam MessageTitle = message dialog title
     *  \iparam MessageText  = message dialog text
     *  \iparam Type = message dialog type
     *  \return reject or except
     */
    /****************************************************************************/
    virtual int ShowConfirmMessage(const QString& MessageTitle, const QString& MessageText, BUTTON_TYPE Type = YES_NO);

    /****************************************************************************/
    /*!
     *  \brief To get parent widget.
     *  \return Pointer of parent widget
     */
    /****************************************************************************/
    QWidget *ParentWidget()
    {
        return mp_Parent;
    }

    /****************************************************************************/
    /*!
     *  \brief To Show failed dialog when rotary valve move failed.
     *  \iparam Title = dialog title string
     */
    /****************************************************************************/
    virtual void ShowRVMoveFailedDlg(const QString& Title);

protected:
    QWidget        *mp_Parent;              //!< Parent widget
    MainMenu::CMessageDlg *mp_WaitDlg;      //!< Wait dialog

};

}

#endif // DIAGNOSTIC_MESSAGEDLG_H
