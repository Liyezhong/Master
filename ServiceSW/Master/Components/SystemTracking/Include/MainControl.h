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

#include "Core/Include/ServiceDefines.h"
#include "Core/Include/ServiceGUIConnector.h"
#include "DeviceControl/Include/DeviceProcessing/DeviceLifeCycleRecord.h"

namespace SystemTracking {

namespace Ui {
class CMainControlConfiguration;
}

/****************************************************************************/
/**
 * \brief This is the widget displaying to modify main control config.
 */
/****************************************************************************/
class CMainControl : public QWidget
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
    explicit CMainControl(Core::CServiceGUIConnector &DataConnector,
                          QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CMainControl(void);

    /****************************************************************************/
    /**
     * \brief Get Main control Module.
     * \return The pointer of Main control module
     */
    /****************************************************************************/
    ServiceDataManager::CModule* GetModule();

    void DisableASB3();

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

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief  slot for update sub module.
     *  \param  SubModule = sub module object
     */
    /****************************************************************************/
    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify E box.
     */
    /****************************************************************************/
    void ModifyEBox(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify ASB3.
     */
    /****************************************************************************/
    void ModifyASB3(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify ASB5.
     */
    /****************************************************************************/
    void ModifyASB5(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify ASB15.
     */
    /****************************************************************************/
    void ModifyASB15(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Touch screen.
     */
    /****************************************************************************/
    void ModifyTouchScreen(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Vent fan.
     */
    /****************************************************************************/
    void ModifyVentFan(void);

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

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief  slot for auto detect submodule info.
     *  \param  SubModule = submodule object
     */
    /****************************************************************************/
    void AutoDetect(ServiceDataManager::CSubModule &SubModule);

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

    /****************************************************************************/
    /*!
     *  \brief  Save life cycle to device life cycle config file
     *  \iparam SlaveName = the name of slave
     */
    /****************************************************************************/
    void SaveLifeCycle2Device(QString& SlaveName);

    /****************************************************************************/
    /*!
     *  \brief  To set device module life cycle
     *  \iparam p_ModuleRecord = pointer of module life cycle record.
     *  \iparam SubModuleName  = the name of sub module.
     *  \iparam CycleValue     = life cycle value
     */
    /****************************************************************************/
    void SetDeviceModuleLifeCycle(DeviceControl::ModuleLifeCycleRecord* p_ModuleRecord, QString SubModuleName, QString CycleValue);

    /****************************************************************************/
    /*!
     *  \brief  Get operation time
     *  \iparam SubModule = the pointer of sub module
     *  \return the operation time of second.
     */
    /****************************************************************************/
    QString GetOperationTime2Second(ServiceDataManager::CSubModule* SubModule);

    Ui::CMainControlConfiguration *mp_Ui;                 //!< User Interface
    MainMenu::CMessageDlg *mp_MessageDlg;                 //!< Information dialog
    ServiceDataManager::CModuleDataList *mp_ModuleList;   //!< Current list of modules
    Core::CServiceGUIConnector *mp_DateConnector;         //!< Global data connector
    QStringList  m_SubModuleNames;                        //!< Store Updated sub moudule names
};


} // namespace SystemTracking
#endif // SYSTEMTRACKING_MAINCONTROL_H
