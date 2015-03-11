/****************************************************************************/
/*! \file SystemTracking/Include/RotaryValve.h
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

#include "Core/Include/ServiceDefines.h"
#include "Core/Include/ServiceGUIConnector.h"

namespace SystemTracking {

namespace Ui {
class CRotaryValveConfiguration;
}

/****************************************************************************/
/**
 * \brief This is the widget displaying to modify Rotary valve config.
 */
/****************************************************************************/
class CRotaryValve : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam DataConnector = service Gui connector
     * \iparam p_Parent = parent of widget
     */
    /****************************************************************************/
    explicit CRotaryValve(Core::CServiceGUIConnector &DataConnector,
                          QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CRotaryValve(void);

Q_SIGNALS:
    /****************************************************************************/
    /*!
     *  \brief  signal for module list changed.
     */
    /****************************************************************************/
    void ModuleListChanged(void);

    /****************************************************************************/
    /**
       * \brief Signal emitted to perform manufacturing tests
       * \iparam Test = Test name
       * \iparam AbortId = Test abort id
       */
    /****************************************************************************/
    void PerformManufacturingTest(Service::ModuleTestCaseID Test, Service::ModuleTestCaseID AbortId=Service::TEST_CASE_ID_UNUSED);

    /****************************************************************************/
    /*!
     *  \brief  signal for Slave version update.
     */
    /****************************************************************************/
    void UpdateSlaveVersion(void);
public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief  slot for update module.
     *  \param  Module = module object
     */
    /****************************************************************************/
    void UpdateModule(ServiceDataManager::CModule &Module);

    /****************************************************************************/
    /*!
     *  \brief  slot for update sub module.
     *  \param  SubModule = sub module object
     */
    /****************************************************************************/
    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Rotary valve.
     */
    /****************************************************************************/
    void ModifyRotaryValve(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Heater.
     */
    /****************************************************************************/
    void ModifyHeater(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Motor.
     */
    /****************************************************************************/
    void ModifyMotor(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for finalize configuration.
     */
    /****************************************************************************/
    void OnFinalizeConfiguration(void);

    /****************************************************************************/
    /*!
     *  \brief  call slot when current tab changed.
     *  \param  Index = tab index
     */
    /****************************************************************************/
    void CurrentTabChanged(int Index);

    /****************************************************************************/
    /*!
     *  \brief  slot for confirm module configuration.
     */
    /****************************************************************************/
    void ConfirmModuleConfiguration();

private:
    /****************************************************************************/
    /*!
     *  \brief  To modify sub module info.
     *  \param  ModuleName    = the module name
     *  \param  SubMoudleName = the sub module name
     */
    /****************************************************************************/
    void ModifySubModule(const QString &ModuleName,
                         const QString &SubModuleName);

    /****************************************************************************/
    /*!
     *  \brief  Pop's up to confirm module configuration.
     *  \param  Text = the message to confirm string.
     */
    /****************************************************************************/
    void ConfirmModuleConfiguration(QString& Text);

    /****************************************************************************/
    /*!
     *  \brief  reset message box.
     */
    /****************************************************************************/
    void ResetMessageBox();

    /****************************************************************************/
    /*!
     *  \brief  To reset sub module life cycle.
     */
    /****************************************************************************/
    void ResetSubModuleLifeCycle();

    Ui::CRotaryValveConfiguration *mp_Ui;               //!< User Interface
    Core::CServiceGUIConnector *mp_DateConnector;       //!< Global data connector
    MainMenu::CMessageDlg *mp_MessageDlg;               //!< Information dialog
    ServiceDataManager::CModuleDataList *mp_ModuleList; //!< Current list of modules
    QStringList  m_SubModuleNames;                      //!< Store Updated sub moudule names
    bool         m_ModifiedModule;                       //!< Flag of module has been modified
    bool         m_ModifiedModuleList;                  //!< Flag of module list has been modifyed
};


} // namespace SystemTracking

#endif // SYSTEMTRACKING_ROTARYVALVE_H
