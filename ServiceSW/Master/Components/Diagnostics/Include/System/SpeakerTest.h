/****************************************************************************/
/*! \file SpeakerTest.h
 *
 *  \brief Declaration of System speaker test.
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

#ifndef DIAGNOSTICS_SYSTEM_SPEAKERTEST_H
#define DIAGNOSTICS_SYSTEM_SPEAKERTEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace System {

class CSpeakerTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CSpeakerTest(QWidget *p_Parent=NULL);
    ~CSpeakerTest(void);

    int Run(void);

protected:
    //virtual int Show
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_SPEAKERTEST_H
