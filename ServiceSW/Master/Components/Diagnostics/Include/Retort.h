/****************************************************************************/
/*! \file Retort.h
 *
 *  \brief Declaration of Retort test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-27
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

#ifndef DIAGNOSTICS_RETORT_H
#define DIAGNOSTICS_RETORT_H

#include <QWidget>
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Ui {
class CRetort;
}

class CRetort : public QWidget
{
    Q_OBJECT
    
public:
    explicit CRetort(QWidget *p_Parent = 0);
    ~CRetort();

protected:
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void StartLidLockTest(void);
    void StartDrainReagentTest(void);
    void StartLevelSensorDetectionTest(void);
    void StartHeatingTestEmpty(void);
    void StartHeatingTestWithWater(void);
    
private:
    Ui::CRetort *mp_Ui;
    CDiagnosticMessageDlg*   mp_MessageDlg;     //!< The pointer message dialog

};


} // namespace Diagnostics
#endif // DIAGNOSTICS_RETORT_H
