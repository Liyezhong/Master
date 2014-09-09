/****************************************************************************/
/*! \file MainControl.h
 *
 *  \brief Declaration of Main Control test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-09
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

#ifndef DIAGNOSTICS_MAINCONTROL_H
#define DIAGNOSTICS_MAINCONTROL_H

#include <QWidget>

namespace Diagnostics {

namespace Ui {
class CMainControl;
}

class CMainControl : public QWidget
{
    Q_OBJECT

public:
    explicit CMainControl(QWidget *parent = 0);
    ~CMainControl();

protected:
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void StartASB3Test(void);
    void StartASB5Test(void);
    void StartASB15Test(void);

private:
    Ui::CMainControl *ui;

signals:
    void GuiASB3Test();
    void GuiASB5Test();
    void GuiASB15Test();
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_MAINCONTROL_H
