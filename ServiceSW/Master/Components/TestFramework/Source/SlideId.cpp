/****************************************************************************/
/*! \file SlideId.cpp
 *
 *  \brief SlideId implementation.
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

#include "TestFramework/Include/SlideId.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CSlideId::CSlideId(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_InitTest = new TestCases::CInitialization();
    mp_BarcodeTest = new TestCases::CTimedTest("Place the slides", false);
    mp_Barcode1 = new TestCases::CIdReader("Barcode 1");
    mp_Barcode2 = new TestCases::CIdReader("Barcode 2");
    mp_InvalidCode = new TestCases::CIdReader("Invalid code");

    AddTestCase(mp_InitTest);
    AddTestCase(mp_BarcodeTest);
    AddTestCase(mp_Barcode1);
    AddTestCase(mp_Barcode2);
    AddTestCase(mp_InvalidCode);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSlideId::~CSlideId()
{
    try {
        delete mp_InvalidCode;
        delete mp_Barcode2;
        delete mp_Barcode1;
        delete mp_BarcodeTest;
        delete mp_InitTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
