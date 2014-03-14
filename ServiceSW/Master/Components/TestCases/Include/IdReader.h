/****************************************************************************/
/*! \file IdReader.h
 *
 *  \brief IdReader definition.
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

#ifndef TESTCASES_RFIDREADER_H
#define TESTCASES_RFIDREADER_H

#include "TestCases/Include/TestCaseBase.h"
#include <QLabel>

namespace TestCases {

/****************************************************************************/
/**
 * \brief This test case checks the correctness of barcodes and RFID
 *        information.
 */
/****************************************************************************/
class CIdReader : public TestCases::CTestCaseBase
{
public:
    CIdReader(QString Caption);
    virtual ~CIdReader();

private:
    QLabel *mp_TextLabel;   //!< Holding the sub caption
    QLabel *mp_NumberLabel; //!< Shows the ID read from the device
};

} // end namespace TestCases

#endif // TESTCASES_RFIDREADER_H
