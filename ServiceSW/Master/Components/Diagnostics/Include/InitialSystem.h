/****************************************************************************/
/*! \file InitialSystem.h
 *
 *  \brief Declaration of Initial system check.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-10
 *   $Author:  $ Dixiong Li
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

#ifndef DIAGNOSTICS_INITIALSYSTEM_H
#define DIAGNOSTICS_INITIALSYSTEM_H

#include "MainMenu/Include/DialogFrame.h"

namespace Diagnostics {

namespace Ui {
class CInitialSystem;
}

class CInitialSystem : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    explicit CInitialSystem(QWidget *parent = 0);
    ~CInitialSystem();

protected:
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void UpdateMainRelayStatus();
    void UpdateOvenTestStatus();
    void UpdateLiquidTestStatus();
    void UpdateRVTestStatus();
    void UpdateRetortTestStatus();

private:
    Ui::CInitialSystem *mp_Ui;

signals:
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_INITIALSYSTEM_H

