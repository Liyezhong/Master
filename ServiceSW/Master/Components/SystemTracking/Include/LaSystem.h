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
class CLaSystemConfiguration;
}

/****************************************************************************/
/**
 * \brief This is the widget displaying to modify LA system config.
 */
/****************************************************************************/
class CLaSystem : public QWidget
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
    explicit CLaSystem(Core::CServiceGUIConnector &DataConnector,
                       QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CLaSystem(void);

Q_SIGNALS:
    /****************************************************************************/
    /*!
     *  \brief  signal for module list changed.
     */
    /****************************************************************************/
    void ModuleListChanged(void);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief  slot for update sub module.
     *  \param  SubModule = reference sub module object
     */
    /****************************************************************************/
    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify pump.
     */
    /****************************************************************************/
    void ModifyPump(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify valve1.
     */
    /****************************************************************************/
    void ModifyValve1(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify valve2.
     */
    /****************************************************************************/
    void ModifyValve2(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify heating belt1.
     */
    /****************************************************************************/
    void ModifyHeatingBelt1(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify heating belt2.
     */
    /****************************************************************************/
    void ModifyHeatingBelt2(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify pressure sensor.
     */
    /****************************************************************************/
    void ModifyPressureSensor(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify carbon filter.
     */
    /****************************************************************************/
    void ModifyCarbonFilter(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify fan.
     */
    /****************************************************************************/
    void ModifyFan(void);

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

    Ui::CLaSystemConfiguration *mp_Ui;                   //!< User Interface

    Core::CServiceGUIConnector *mp_DateConnector;        //!< Global data connector

    MainMenu::CMessageDlg *mp_MessageDlg;                //!< Information dialog
    ServiceDataManager::CModuleDataList *mp_ModuleList;  //!< Current list of modules
};


} // namespace SystemTracking
#endif // SYSTEMTRACKING_LASYSTEM_H
