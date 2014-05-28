/****************************************************************************/
/*! \file LaSystem.h
 *
 *  \brief Declaration of Liquid and Air system configuration class.
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

#ifndef SYSTEMTRACKING_LASYSTEM_H
#define SYSTEMTRACKING_LASYSTEM_H

#include <QWidget>

#include "Core/Include/ServiceGUIConnector.h"

namespace SystemTracking {

namespace Ui {
class CLaSystem;
}

class CLaSystem : public QWidget
{
    Q_OBJECT
    
public:
    explicit CLaSystem(Core::CServiceGUIConnector &DataConnector,
                       QWidget *parent = 0);

    ~CLaSystem(void);

Q_SIGNALS:
    void ModuleListChanged(void);

public Q_SLOTS:
    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    void ModifyPump(void);

    void ModifyValve1(void);

    void ModifyValve2(void);

    void ModifyHeatingBelt1(void);

    void ModifyHeatingBelt2(void);

    void ModifyPressureSensor(void);

    void ModifyCarbonFilter(void);

    void ModifyFan(void);

private:
    void ModifySubModule(const QString &ModuleName,
                         const QString &SubModuleName);

    Ui::CLaSystem *ui;

    Core::CServiceGUIConnector *mp_DateConnector;
};


} // namespace SystemTracking
#endif // SYSTEMTRACKING_LASYSTEM_H
