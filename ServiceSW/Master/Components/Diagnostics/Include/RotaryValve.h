/****************************************************************************/
/*! \file RotaryValve.h
 *
 *  \brief Declaration of RotaryValve test.
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

#ifndef DIAGNOSTICS_ROTARYVALVE_H
#define DIAGNOSTICS_ROTARYVALVE_H

#include <QWidget>
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Ui {
class CRotaryValve;
}

class CRotaryValve : public QWidget
{
    Q_OBJECT
    
public:
    explicit CRotaryValve(QWidget *parent = 0);
    ~CRotaryValve();

private Q_SLOTS:
    void StartMovementTest(void);

private:
    Ui::CRotaryValve *ui;
    bool              m_InitSuccess;
    CDiagnosticMessageDlg *mp_MessageDlg;
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_ROTARYVALVE_H
