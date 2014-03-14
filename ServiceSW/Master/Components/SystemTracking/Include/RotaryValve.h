/****************************************************************************/
/*! \file RotaryValve.h
 *
 *  \brief Declaration of Rotary Valve configuration class.
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

#ifndef SYSTEMTRACKING_ROTARYVALVE_H
#define SYSTEMTRACKING_ROTARYVALVE_H

#include <QWidget>

#include "Core/Include/ServiceGUIConnector.h"

namespace SystemTracking {

namespace Ui {
class CRotaryValve;
}

class CRotaryValve : public QWidget
{
    Q_OBJECT
    
public:
    explicit CRotaryValve(Core::CServiceGUIConnector &DataConnector,
                          QWidget *parent = 0);

    ~CRotaryValve(void);

Q_SIGNALS:
    void ModuleListChanged(void);

public Q_SLOTS:
    void UpdateModule(DataManager::CModule &Module);

    void UpdateSubModule(DataManager::CSubModule &SubModule);

    void ModifyRotaryValve(void);

    void ModifyHeater(void);

    void ModifyMotor(void);

private:
    void ModifySubModule(const QString &ModuleName,
                         const QString &SubModuleName);

    Ui::CRotaryValve *ui;

    Core::CServiceGUIConnector *mp_DateConnector;
};


} // namespace SystemTracking

#endif // SYSTEMTRACKING_ROTARYVALVE_H
