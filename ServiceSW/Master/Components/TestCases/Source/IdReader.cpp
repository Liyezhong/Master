/****************************************************************************/
/*! \file IdReader.cpp
 *
 *  \brief IdReader implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-23
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

#include "TestCases/Include/IdReader.h"

namespace TestCases {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam Caption = Subtitle of the test case
 */
/****************************************************************************/
CIdReader::CIdReader(QString Caption) : CTestCaseBase(Caption, "", true)
{
    mp_TextLabel = new QLabel("Identification:");
    mp_NumberLabel = new QLabel("n/a");

    AddWidget(2, mp_TextLabel);
    AddWidget(3, mp_NumberLabel);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CIdReader::~CIdReader()
{
    try {
        delete mp_NumberLabel;
        delete mp_TextLabel;
    }
    catch (...) {}
}

} // end namespace TestCases
