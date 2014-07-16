/****************************************************************************/
/*! \file System.h
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-07-16
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai.. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SERVICEUPDATES_SYSTEM_H
#define SERVICEUPDATES_SYSTEM_H

#include <QWidget>

#include "MainMenu/Include/WaitDialog.h"
#include "MainMenu/Include/MessageDlg.h"


namespace ServiceUpdates {

namespace Ui {
    class CSystem;
}
/****************************************************************************/
/**
 * \brief
 */
/****************************************************************************/
class CSystem : public QWidget
{
    Q_OBJECT

public:
    explicit CSystem(QWidget *p_Parent = 0);
    ~CSystem(void);

private Q_SLOTS:
    void OnShutDown(void);


private:
    Ui::CSystem   *mp_Ui;

    MainMenu::CWaitDialog *mp_WaitDialog;
    MainMenu::CMessageDlg *mp_MessageDlg;
};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_SHUTDOWN_H
