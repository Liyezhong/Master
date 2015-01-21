/****************************************************************************/
/*! \file RVPreTest.h
 *
 *  \brief Declaration of Main Control ASB test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-15
 *   $Author:  $ Sunny.Qu
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

#ifndef DIAGNOSTICS_INITIALSYSTEM_RVPRETEST_H
#define DIAGNOSTICS_INITIALSYSTEM_RVPRETEST_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"

namespace Diagnostics {

namespace InitialSystem {

/****************************************************************************/
/*!
*   \brief This class implements the functionality to initail rotary valve test
*/
/****************************************************************************/
class CRVPreTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    CRVPreTest(QWidget *p_Parent=NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CRVPreTest(void);

    /****************************************************************************/
    /*!
     *  \brief The function for test start run
     *  \return error code
     */
    /****************************************************************************/
    int Run(void);

    /****************************************************************************/
    /*!
     *  \brief The function for start pre heating
     */
    /****************************************************************************/
    void StartPreHeating();
protected:
    /****************************************************************************/
    /*!
     *  \brief To show/close waiting dialog
     *  \iparam ShowFlag = true to show and flase to close the dialog
     */
    /****************************************************************************/
    virtual void ShowWaitingMessage(bool ShowFlag=true);

    /****************************************************************************/
    /*!
     *  \brief To show the fail message dialog
     *  \iparam ErrorCode = the error type
     */
    /****************************************************************************/
    virtual void ShowFailMessage(int ErrorCode);
};

} // namespace InitialSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_INITIALSYSTEM_RVPRETEST_H
