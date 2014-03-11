/****************************************************************************/
/*! \file DlgSpeaker.h
 *
 *  \brief Declaration of Speaker test dialog.
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

#ifndef DIAGNOSTICS_SYSTEM_DLGSPEAKER_H
#define DIAGNOSTICS_SYSTEM_DLGSPEAKER_H

#include "MainMenu/Include/DialogFrame.h"

#include "MainMenu/Include/ScrollWheel.h"


namespace Diagnostics {

namespace System {

namespace Ui {
class CDlgSpeaker;
}

class CDlgSpeaker : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    explicit CDlgSpeaker(QWidget *parent = 0);
    ~CDlgSpeaker(void);

Q_SIGNALS:
    void TestSpeaker(int Sound, int Volume);

private Q_SLOTS:
    void EmitTestSpeaker(void);

protected:
    void changeEvent(QEvent *p_Event);
    
private:
    Ui::CDlgSpeaker        *ui;

    MainMenu::CScrollWheel *mp_ScrollSound;
    MainMenu::CScrollWheel *mp_ScrollVolume;
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_DLGSPEAKER_H
