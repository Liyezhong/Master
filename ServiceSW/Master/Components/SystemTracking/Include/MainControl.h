/****************************************************************************/
/*! \file MainControl.h
 *
 *  \brief Declaration of Main control configuration class.
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

#ifndef SYSTEMTRACKING_MAINCONTROL_H
#define SYSTEMTRACKING_MAINCONTROL_H

#include <QWidget>

#include "Core/Include/ServiceGUIConnector.h"

namespace SystemTracking {

namespace Ui {
class CMainControl;
}

class CMainControl : public QWidget
{
    Q_OBJECT
    
public:
    explicit CMainControl(Core::CServiceGUIConnector &DataConnector,
                          QWidget *parent = 0);

    ~CMainControl(void);

Q_SIGNALS:
    void ModuleListChanged(void);

public Q_SLOTS:
    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    void ModifyASB3(void);

    void ModifyASB5(void);

    void ModifyASB15(void);

    void ModifyTouchScreen(void);

    void ModifyVentFan(void);

private Q_SLOTS:
    void AutoDetect(ServiceDataManager::CSubModule &SubModule);

private:
    void ModifySubModule(const QString &ModuleName,
                         const QString &SubModuleName);

    Ui::CMainControl *ui;

    Core::CServiceGUIConnector *mp_DateConnector;
};


} // namespace SystemTracking
#endif // SYSTEMTRACKING_MAINCONTROL_H
