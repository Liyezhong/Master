/****************************************************************************/
/*! \file SlideId.h
 *
 *  \brief SlideId definition.
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

#ifndef TESTFRAMEWORK_SLIDEID_H
#define TESTFRAMEWORK_SLIDEID_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/Initialization.h"
#include "TestCases/Include/TimedTest.h"
#include "TestCases/Include/IdReader.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief Tests cases for the slide ID reader.
 */
/****************************************************************************/
class CSlideId : public CTestScreen
{
public:
    CSlideId(QWidget *p_Parent = 0);
    virtual ~CSlideId();

private:
    TestCases::CInitialization *mp_InitTest;    //!< Assembly group registration test
    TestCases::CTimedTest *mp_BarcodeTest;      //!< Test of the barcode reader
    TestCases::CIdReader *mp_Barcode1;          //!< First barcode label
    TestCases::CIdReader *mp_Barcode2;          //!< Second barcode label
    TestCases::CIdReader *mp_InvalidCode;       //!< Testing an unreadable barcode
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_SLIDEID_H
