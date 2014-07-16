/****************************************************************************/
/*! \file Oven.h
 *
 *  \brief Declaration of Oven configuration class.
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

#ifndef SYSTEMTRACKING_OVEN_H
#define SYSTEMTRACKING_OVEN_H

#include <QWidget>

#include "Core/Include/ServiceGUIConnector.h"

namespace SystemTracking {

namespace Ui {
class COven;
}

class COven : public QWidget
{
    Q_OBJECT
    
public:
    explicit COven(Core::CServiceGUIConnector &DataConnector,
                   QWidget *parent = 0);

    ~COven(void);

Q_SIGNALS:
    void ModuleListChanged(void);

public Q_SLOTS:
    void UpdateModule(ServiceDataManager::CModule &Module);

    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    void ModifyOven(void);

    void ModifyHeater(void);

    void ModifyCoverSensor(void);

    void OnFinalizeConfiguration();

private:
    void ModifySubModule(const QString &ModuleName,
                         const QString &SubModuleName);

    Ui::COven *mp_Ui;

    Core::CServiceGUIConnector *mp_DataConnector;
};


} // namespace SystemTracking

#endif // SYSTEMTRACKING_OVEN_H
