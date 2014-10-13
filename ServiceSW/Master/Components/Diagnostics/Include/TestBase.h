/****************************************************************************/
/*! \file TestBase.h
 *
 *  \brief Declaration of Retort test.
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

#ifndef DIAGNOSTICS_TESTBASE_H
#define DIAGNOSTICS_TESTBASE_H

#include <QObject>
#include "MainMenu/Include/MessageDlg.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"

namespace Diagnostics {

/****************************************************************************/
/*!
 *  \brief This is the base class of the diagnostics test
 */
/****************************************************************************/
class CTestBase : public QObject
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    CTestBase(QWidget *p_Parent=NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CTestBase(void);

    /****************************************************************************/
    /*!
     *  \brief The function for test start run
     */
    /****************************************************************************/
    virtual int Run(void) = 0;

protected:
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

    virtual int ShowConfirmMessage(QString& MessageTitle, QString& MessageText);

protected:
    QWidget        *mp_Parent;              //!< Parent widget
    MainMenu::CMessageDlg *mp_WaitDlg;      //!< Wait dialog
};

}

#endif // DIAGNOSTICS_TESTBASE_H
