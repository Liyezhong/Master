/****************************************************************************/
/*! \file InitialSystemCheck.h
 *
 *  \brief Declaration of Initial system check.
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

#ifndef DIAGNOSTICS_INITIALSYSTEMCHECK_H
#define DIAGNOSTICS_INITIALSYSTEMCHECK_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"

namespace Diagnostics {

namespace InitialSystem {

class CInitialSystemCheck : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CInitialSystemCheck(QWidget *parent=NULL);
    ~CInitialSystemCheck(void);

    int Run(void);
private:
    QWidget                   *mp_Parent;
};

} // namespace InitialSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_INITIALSYSTEMCHECK_H
