/****************************************************************************/
/*! \file MainsRelayTest.h
 *
 *  \brief Declaration of Mains relay test.
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

#ifndef DIAGNOSTICS_INITALSYSTEM_MAINSRELAYTEST_H
#define DIAGNOSTICS_INITALSYSTEM_MAINSRELAYTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"

namespace Diagnostics {

namespace InitialSystem {

/****************************************************************************/
/*!
*   \brief This class implements the functionality to initail Mains Relay test
*/
/****************************************************************************/
class CMainsRelayTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    CMainsRelayTest(QWidget *p_Parent=NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CMainsRelayTest(void);

    /****************************************************************************/
    /*!
     *  \brief The function for test start run
     */
    /****************************************************************************/
    int Run(void);
private:
    /****************************************************************************/
    /*!
     *  \brief To show/close waiting dialog
     *  \iparam ShowFlag = true to show and flase to close the dialog
     */
    /****************************************************************************/
    void ShowWaitingMessage(bool ShowFlag=true);

    /****************************************************************************/
    /*!
     *  \brief To show the fail message dialog
     *  \iparam Error = the error type
     */
    /****************************************************************************/
    void ShowFailMessage(int Error);
};

} // namespace InitialSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_INITALSYSTEM_MAINSRELAYTEST_H
