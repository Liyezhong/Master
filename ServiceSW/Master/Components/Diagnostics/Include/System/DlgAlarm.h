/****************************************************************************/
/*! \file DlgAlarm.h
 *
 *  \brief Declaration of Alarm test dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
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

#ifndef DIAGNOSTICS_SYSTEM_DLGALARM_H
#define DIAGNOSTICS_SYSTEM_DLGALARM_H

#include <QString>

#include "MainMenu/Include/DialogFrame.h"

#include "MainMenu/Include/SliderControl.h"


namespace Diagnostics {

namespace System {

namespace Ui {
class CDlgAlarm;
}

class CDlgAlarm : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    explicit CDlgAlarm(QWidget *parent = 0);
    ~CDlgAlarm(void);

Q_SIGNALS:
    void TestAlarm(bool On);

public Q_SLOTS:
    void SetStatus(const QString &Status);

private Q_SLOTS:
    void EmitTestAlarm(MainMenu::CSliderControl::Position_t Position);

protected:
    void changeEvent(QEvent *p_Event);
    
private:
    Ui::CDlgAlarm *ui;
};


} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_DLGALARM_H
