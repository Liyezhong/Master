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
    void UpdateModule(DataManager::CModule &Module);

    void UpdateSubModule(DataManager::CSubModule &SubModule);

    void ModifyRetort(void);

    void ModifyHeater(void);

    void ModifyLidLock(void);

    void ModifyLevelSensor(void);

private:
    void ModifySubModule(const QString &ModuleName,
                         const QString &SubModuleName);

    Ui::CRetort *ui;

    Core::CServiceGUIConnector *mp_DateConnector;
};


} // namespace SystemTracking
#endif // SYSTEMTRACKING_RETORT_H
