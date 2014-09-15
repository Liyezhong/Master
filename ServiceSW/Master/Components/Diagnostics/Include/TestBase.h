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

namespace Diagnostics {

class CTestBase : public QObject
{
    Q_OBJECT

public:
    CTestBase(void) : QObject() { }
    virtual ~CTestBase(void) { }

    virtual int Run(void) = 0;
};

}

#endif // DIAGNOSTICS_TESTBASE_H
