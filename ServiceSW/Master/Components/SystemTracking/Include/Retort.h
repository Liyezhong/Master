/****************************************************************************/
/*! \file Retort.h
 *
 *  \brief Declaration of Retort configuration class.
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

#ifndef SYSTEMTRACKING_RETORT_H
#define SYSTEMTRACKING_RETORT_H

#include <QWidget>

#include "Core/Include/ServiceGUIConnector.h"

namespace SystemTracking {

namespace Ui {
class CRetort;
}

class CRetort : public QWidget
{
    Q_OBJECT
    
public:
    explicit CRetort(Core::CServiceGUIConnector &DataConnector,
                     QWidget *parent = 0);

    ~CRetort(void);

Q_SIGNALS:
    void ModuleListChanged(void);

public Q_SLOTS:
    void UpdateModule(ServiceDataManager::CModule &Module);

    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    void ModifyRetort(void);

    void ModifyHeater(void);

    void ModifyLidLock(void);

    void ModifyLevelSensor(void);

    void OnFinalizeConfiguration(void);

    void CurrentTabChanged(int Index);

    void ConfirmModuleConfiguration();

private:
    void ModifySubModule(const QString &ModuleName,
                         const QString &SubModuleName);

    void ConfirmModuleConfiguration(QString& Text);

    void ResetMessageBox();

    Ui::CRetort *mp_Ui;

    Core::CServiceGUIConnector *mp_DateConnector;

    MainMenu::CMessageDlg *mp_MessageDlg;                                   //!< Information dialog
    ServiceDataManager::CModuleDataList *mp_ModuleList;
};


} // namespace SystemTracking
#endif // SYSTEMTRACKING_RETORT_H
