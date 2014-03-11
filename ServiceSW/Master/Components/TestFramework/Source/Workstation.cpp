/****************************************************************************/
/*! \file Workstation.cpp
 *
 *  \brief Workstation implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-20
 *   $Author:  $ M.Scherer
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

#include "TestFramework/Include/Workstation.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CWorkstation::CWorkstation(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_InitTest = new TestCases::CInitialization();

    AddTestCase(mp_InitTest);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CWorkstation::~CWorkstation()
{
    try {
        delete mp_InitTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
